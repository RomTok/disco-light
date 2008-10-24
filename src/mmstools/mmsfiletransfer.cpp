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

size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	return fread(ptr, size, nmemb, (FILE*) stream);
}

size_t write_callback(void *buffer, size_t size, size_t nmemb, void *stream) {
	FtpFile *out = (FtpFile*) stream;
	if (out && !out->stream) {
		/* open file for appending */
		out->stream = fopen(out->filename, "ab");
		if (!out->stream)
			return -1; /* failure, can't open file to write */
	}

	return fwrite(buffer, size, nmemb, out->stream);
}

int progress_callback(void *pclient, double dltotal, double dlnow, double ultotal, double ulnow) {
	((MMSFiletransfer::MMSFiletransfer*) pclient)->progress.emit(dltotal != 0 ? (int) (.5 + 100* dlnow / dltotal) : (int) (.5 + 100* ulnow / ultotal));
	return 0;
}

MMSFiletransfer::MMSFiletransfer(const string url, const unsigned int ftpPort = 0) :
	timeout(10),
	lowSpeedLimit(102400),
	port(ftpPort) {
	setRemoteUrl(url);

	/* initialise curl */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	this->ehandle = curl_easy_init();

	if (!this->ehandle) {
		this->lasterror = CURLE_FAILED_INIT;
	} else {
		/* set timeout behaviour */
		curl_easy_setopt(this->ehandle, CURLOPT_LOW_SPEED_LIMIT, 	this->lowSpeedLimit);
		curl_easy_setopt(this->ehandle, CURLOPT_LOW_SPEED_TIME, 	this->timeout);
		if(this->port != 0)
			curl_easy_setopt(this->ehandle, CURLOPT_PORT, this->port);
	}
}


MMSFiletransfer::~MMSFiletransfer() {
	curl_easy_cleanup(this->ehandle);
 	curl_global_cleanup();
}


void MMSFiletransfer::setVerboseInformation(bool enable) {
	curl_easy_setopt(this->ehandle, CURLOPT_VERBOSE, (enable ? 1L : 0L));
}


void MMSFiletransfer::setAuthData(const string user, const string password) {
	std::string auth = user;
	if (0 != password.length())
		auth += ":" + password;

	curl_easy_setopt(this->ehandle, CURLOPT_USERPWD, auth.c_str());
}


bool MMSFiletransfer::performUpload(const string localfile, const string remoteName, bool resume) {
	FILE *hd_src;
	struct stat file_info;

	/* enable uploading */
	curl_easy_setopt(this->ehandle, CURLOPT_UPLOAD, 1L);
	/* enable curl to create missing dirs on upload */
	curl_easy_setopt(this->ehandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);

	/* register progress callback */
	curl_easy_setopt(this->ehandle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(this->ehandle, CURLOPT_PROGRESSFUNCTION, progress_callback);
	curl_easy_setopt(this->ehandle, CURLOPT_PROGRESSDATA, this);

	if (resume) {
		curl_easy_setopt(this->ehandle, CURLOPT_RESUME_FROM, -1L);
	}

	/* specify target */
	curl_easy_setopt(this->ehandle, CURLOPT_URL, (this->remoteUrl + remoteName).c_str());

	/* get the file size of the local file */
	if (stat(localfile.c_str(), &file_info)) {
		/* throw error */
		this->lasterror = CURLE_FILE_COULDNT_READ_FILE;
		return false;
	}

	/* get a FILE * of the same file */
	hd_src = fopen(localfile.c_str(), "rb");
	if(!hd_src) {
		this->lasterror = CURLE_FILE_COULDNT_READ_FILE;
		return false;
	}

	/* now specify which file to upload */
	curl_easy_setopt(this->ehandle, CURLOPT_READFUNCTION, read_callback);
	curl_easy_setopt(this->ehandle, CURLOPT_READDATA, hd_src);

	curl_easy_setopt(this->ehandle, CURLOPT_INFILESIZE, (long)file_info.st_size);

	/* Now run off and do what you've been told! */
	this->lasterror = curl_easy_perform(this->ehandle);
	if (this->lasterror != CURLE_OK)
		curl_easy_setopt(this->ehandle, CURLOPT_FRESH_CONNECT, 1);
	else
		curl_easy_setopt(this->ehandle, CURLOPT_FRESH_CONNECT, 0);

	fclose(hd_src); /* close the local file */

	return (this->lasterror == CURLE_OK);
}


bool MMSFiletransfer::performDownload(const string localfile, const string remoteName, bool resume) {
	struct stat file_info;
	FtpFile ftpfile = { localfile.c_str(), /* name to store the file as if succesful */
	NULL };

	/* register progress callback */
	curl_easy_setopt(this->ehandle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(this->ehandle, CURLOPT_PROGRESSFUNCTION, progress_callback);
	curl_easy_setopt(this->ehandle, CURLOPT_PROGRESSDATA, this);

	if (resume) {
		if (!stat(localfile.c_str(), &file_info)) {
			curl_easy_setopt(this->ehandle, CURLOPT_RESUME_FROM, (long)file_info.st_size);
		}
	}

	if (!fopen(ftpfile.filename, (resume ? "ab" : "wb"))) {
		this->lasterror = CURLE_FILE_COULDNT_READ_FILE;
		return false;
	}

	curl_easy_setopt(this->ehandle, CURLOPT_URL, (this->remoteUrl + remoteName).c_str());

	/* Set a pointer to our struct to pass to the callback */
	curl_easy_setopt(this->ehandle, CURLOPT_WRITEDATA, &ftpfile);

	/* Define our callback to get called when there's data to be written */
	curl_easy_setopt(this->ehandle, CURLOPT_WRITEFUNCTION, write_callback);

	this->lasterror = curl_easy_perform(this->ehandle);
	if (this->lasterror != CURLE_OK)
		curl_easy_setopt(this->ehandle, CURLOPT_FRESH_CONNECT, 1);
	else
		curl_easy_setopt(this->ehandle, CURLOPT_FRESH_CONNECT, 0);


	if (ftpfile.stream) {
		fclose(ftpfile.stream); /* close the local file */
	}

	return (this->lasterror == CURLE_OK);
}


bool MMSFiletransfer::deleteRemoteFile(const string remoteFile) {
	std::string ftpcommand = "";
	struct curl_slist *slist=NULL;

	ftpcommand = "DELE " + remoteFile;
	slist = curl_slist_append(slist, "CWD /");
	slist = curl_slist_append(slist, ftpcommand.c_str());
	/* pass the list of custom commands to the handle */
	curl_easy_setopt(this->ehandle, CURLOPT_QUOTE, slist);

	curl_easy_setopt(this->ehandle, CURLOPT_URL, this->remoteUrl.c_str());
	curl_easy_setopt(this->ehandle, CURLOPT_HEADER, 1);

	/* turn off file transfer */
	curl_easy_setopt(this->ehandle, CURLOPT_NOBODY, 1);
	this->lasterror = curl_easy_perform(this->ehandle);
	if (this->lasterror != CURLE_OK)
		curl_easy_setopt(this->ehandle, CURLOPT_FRESH_CONNECT, 1);
	else
		curl_easy_setopt(this->ehandle, CURLOPT_FRESH_CONNECT, 0);

	curl_easy_setopt(this->ehandle, CURLOPT_HEADER, 0);
	curl_easy_setopt(this->ehandle, CURLOPT_NOBODY, 0);
	curl_slist_free_all(slist);
	curl_easy_setopt(this->ehandle, CURLOPT_QUOTE, NULL);

	return (this->lasterror == CURLE_OK);
}


void MMSFiletransfer::setRemoteUrl(const string url) {
	this->remoteUrl = "ftp://" + url;

	/* append trailing / if necessary */
	if ((this->remoteUrl.length() - 1) != this->remoteUrl.find('/', this->remoteUrl.length() - 1)) {
		this->remoteUrl.append("/");
	}
}


const string MMSFiletransfer::getRemoteUrl() {
	return this->remoteUrl;
}


void MMSFiletransfer::setTimeout(const long timeout) {
	this->timeout = timeout;
	curl_easy_setopt(this->ehandle, CURLOPT_LOW_SPEED_TIME, timeout);
}


const long MMSFiletransfer::getTimeout() {
	return this->timeout;
}


void MMSFiletransfer::setFtpPort(const unsigned int ftpPort) {
	if((this->port != (long)ftpPort) && (0 != ftpPort)) {
		this->port = ftpPort;
		curl_easy_setopt(this->ehandle, CURLOPT_PORT, this->port);
	}
}


const unsigned int MMSFiletransfer::getFtpPort() {
	return this->port;
}


void MMSFiletransfer::setLowSpeedLimit(const long limit) {
	this->lowSpeedLimit = limit;
	curl_easy_setopt(this->ehandle, CURLOPT_LOW_SPEED_LIMIT, limit);
}


const long MMSFiletransfer::getLowSpeedLimit() {
	return this->lowSpeedLimit;
}


int MMSFiletransfer::getLastError(string *errormsg = NULL) {
	if (errormsg) {
		*errormsg = (this->lasterror ? curl_easy_strerror(this->lasterror) : NULL);
	}
	return this->lasterror;
}
