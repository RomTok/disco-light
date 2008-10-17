/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mmstools/mmsfiletransfer.h"

size_t MMSFiletransfer::read_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	return fread(ptr, size, nmemb, (FILE*) stream);
}

size_t MMSFiletransfer::write_callback(void *buffer, size_t size, size_t nmemb, void *stream) {
	FtpFile *out = (FtpFile*) stream;
	if (out && !out->stream) {
		/* open file for appending */
		out->stream = fopen(out->filename, "ab");
		if (!out->stream)
			return -1; /* failure, can't open file to write */
	}

	return fwrite(buffer, size, nmemb, out->stream);
}

int MMSFiletransfer::progress_callback(void *pclient, double dltotal, double dlnow, double ultotal, double ulnow) {
	((MMSFiletransfer::MMSFiletransfer*) pclient)->progress.emit(dltotal != 0 ? (int) (.5 + 100* dlnow / dltotal) : (int) (.5 + 100* ulnow / ultotal));
	return 0;
}

MMSFiletransfer::MMSFiletransfer(const string& url, const unsigned int ftpPort = 0) {
	timeout = 10;
	lowSpeedLimit = 1024 * 100;

	this->setRemoteUrl(url);
	this->setFtpPort(ftpPort);

	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	ehandle = curl_easy_init();

	if (!ehandle) {
		lasterror = CURLE_FAILED_INIT;
	} else {
		/* set timeout behaviour */
		curl_easy_setopt(ehandle, CURLOPT_LOW_SPEED_LIMIT, lowSpeedLimit);
		curl_easy_setopt(ehandle, CURLOPT_LOW_SPEED_TIME, timeout);
		/* register progress callback */
		curl_easy_setopt(ehandle, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(ehandle, CURLOPT_PROGRESSFUNCTION, MMSFiletransfer::progress_callback);
		curl_easy_setopt(ehandle, CURLOPT_PROGRESSDATA, this);
		/* enable curl to create missing dirs on upload */
//		curl_easy_setopt(ehandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
	}
}


MMSFiletransfer::~MMSFiletransfer() {
	curl_easy_cleanup(ehandle);
	curl_global_cleanup();
}


void MMSFiletransfer::setVerboseInformation(bool enable) {
	curl_easy_setopt(ehandle, CURLOPT_VERBOSE, (enable ? 1 : 0));
}


void MMSFiletransfer::setAuthData(const string& user, const string& password) {
	curl_easy_setopt(ehandle, CURLOPT_USERPWD, (user + ":" + password).c_str());
}


bool MMSFiletransfer::performUpload(const string& localfile, const string& remoteName, bool resume) {
	FILE *hd_src;
	struct stat file_info;

	/* enable uploading */
	curl_easy_setopt(ehandle, CURLOPT_UPLOAD, 1L);

	if (resume) {
		curl_easy_setopt(ehandle, CURLOPT_RESUME_FROM, -1L);
	}

	/* specify target */
	curl_easy_setopt(ehandle, CURLOPT_URL, (remoteUrl + remoteName).c_str());

	/* get the file size of the local file */
	if (stat(localfile.c_str(), &file_info)) {
		/* throw error */
		lasterror = CURLE_FILE_COULDNT_READ_FILE;
		return false;
	}

	/* get a FILE * of the same file */
	hd_src = fopen(localfile.c_str(), "rb");

	/* now specify which file to upload */
	curl_easy_setopt(ehandle, CURLOPT_READFUNCTION, MMSFiletransfer::read_callback);
	curl_easy_setopt(ehandle, CURLOPT_READDATA, hd_src);

	curl_easy_setopt(ehandle, CURLOPT_INFILESIZE, file_info.st_size);

	/* Now run off and do what you've been told! */
	lasterror = curl_easy_perform(ehandle);
	fclose(hd_src); /* close the local file */

	return (lasterror == CURLE_OK);
}


bool MMSFiletransfer::performDownload(const string& localfile, const string& remoteName, bool resume) {
	struct stat file_info;
	FtpFile ftpfile = { localfile.c_str(), /* name to store the file as if succesful */
	NULL };

	if (resume) {
		if (!stat(localfile.c_str(), &file_info)) {
			curl_easy_setopt(ehandle, CURLOPT_RESUME_FROM, file_info.st_size);
		}
	}

	if (!fopen(ftpfile.filename, (resume ? "ab" : "wb"))) {
		lasterror = CURLE_FILE_COULDNT_READ_FILE;
		return false;
	}

	curl_easy_setopt(ehandle, CURLOPT_URL, (remoteUrl + remoteName).c_str());

	/* Set a pointer to our struct to pass to the callback */
	curl_easy_setopt(ehandle, CURLOPT_WRITEDATA, &ftpfile);

	/* Define our callback to get called when there's data to be written */
	curl_easy_setopt(ehandle, CURLOPT_WRITEFUNCTION,
			MMSFiletransfer::write_callback);

	lasterror = curl_easy_perform(ehandle);

	if (ftpfile.stream) {
		fclose(ftpfile.stream); /* close the local file */
	}

	return (lasterror == CURLE_OK);
}


void MMSFiletransfer::setRemoteUrl(const string& url) {
	remoteUrl = "ftp://" + url;

	/* append trailing / if necessary */
	if ((remoteUrl.length() - 1) != remoteUrl.find('/', remoteUrl.length() - 1)) {
		remoteUrl.append("/");
	}
}


string MMSFiletransfer::getRemoteUrl() {
	return remoteUrl;
}


void MMSFiletransfer::setTimeout(long timeouts) {
	timeout = timeouts;
	curl_easy_setopt(ehandle, CURLOPT_LOW_SPEED_TIME, timeouts);
}


long MMSFiletransfer::getTimeout() {
	return timeout;
}


void MMSFiletransfer::setFtpPort(const unsigned int ftpPort) {
	if (this->port != ftpPort) {
		this->port = ftpPort;
		if (0 != ftpPort) {
			curl_easy_setopt(ehandle, CURLOPT_PORT, ftpPort);
		}
	}
}


unsigned int MMSFiletransfer::getFtpPort() {
	return this->port;
}


void MMSFiletransfer::setLowSpeedLimit(long limit) {
	lowSpeedLimit = limit;
	curl_easy_setopt(ehandle, CURLOPT_LOW_SPEED_LIMIT, limit);
}


long MMSFiletransfer::getLowSpeedLimit() {
	return lowSpeedLimit;
}


int MMSFiletransfer::getLastError(string *errormsg = NULL) {
	if (errormsg) {
		*errormsg = (lasterror ? curl_easy_strerror(lasterror) : NULL);
	}
	return lasterror;
}
