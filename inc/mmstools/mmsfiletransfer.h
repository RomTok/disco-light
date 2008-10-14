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

#ifndef MMSFILETRANSFER_H_
#define MMSFILETRANSFER_H_

#include "mmstools/base.h"

#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>

/** Specifies a structure for file operations */
typedef struct {
	const char *filename;
	FILE *stream;
} FtpFile;

/**
 *  A ftp operations class.
 *
 *  This is a class that provides the functions to down- and upload files from/to a ftp server.
 *  As far as it is supported by the ftp server, resuming is also possible.
 *
 *  @author Patrick Helterhoff
 */
class MMSFiletransfer {

private:
	CURL *ehandle;
	string remoteUrl;
	string logindata;
	CURLcode lasterror;
	long timeout;
	long lowSpeedLimit;

	static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);
	static size_t write_callback(void *buffer, size_t size, size_t nmemb, void *stream);

public:
	/**
	 * Constructor of class MMSFiletransfer.
	 *
	 * @param url     [in] the remote host and desired directory ("localhost/dir")
	 */
	MMSFiletransfer(const string& url);

	/** Destructor of class MMSFiletransfer. */
	~MMSFiletransfer();

	/**
	 * Performs a ftp upload for the specified local file.
	 *
	 * @param   localfile    [in] the local file to be uploaded
	 * @param   remoteName   [in] name of the remote file
	 * @param   resume    	 [in] resume a prior upload
	 */
	bool performUpload(const string& localfile, const string& remoteName, bool resume = false);

	/**
	 * Performs a ftp download for the specified remote file.
	 *
	 * @param   localfile    [in] the local file to be saved
	 * @param   remoteName   [in] name of the remote file
	 * @param   resume    	 [in] resume a prior download
	 */
	bool performDownload(const string& localfile, const string& remoteName, bool resume = false);

	/**
	 * Enables verbose output of from the curl lib.
	 */
	void setVerboseInformation(bool enable);

	/**
	 * Use this to set user and password for the ftp server connection, if necessary.
	 *
	 * @param   user    	[in] the ftp user
	 * @param   password   	[in] the password
	 */
	void setAuthData(const string& user, const string& password);

	/**
	 * Changes the remote url.
	 * The change will be performed on the following ftp operation (upload / download).
	 *
	 * @param url	[in] the remote host and desired directory ("localhost/dir")
	 */
	void setRemoteUrl(const string& url);

	/** Returns the current remote url. */
	string getRemoteUrl();

	/**
	 * Sets the timeout.
	 *
	 * @param timeouts 		[in] The timeout in seconds.
	 */
	void setTimeout(const long timemouts);

	/** Returns the current timeout in seconds. */
	long getTimeout();

	/**
	 * Sets the low speed limit to be considered as timeout (default: 100 kb/s).
	 *
	 * @param limit		[in] The low speed limit in byte per second
	 */
	void setLowSpeedLimit(const long limit);

	/** Returns the current speed limit (bytes per second) to be considered as timeout. */
	long getLowSpeedLimit();

	/**
	 * Returns the error number of the last operation, or 0 if no error has occured.
	 * If the errormsg parameter is supplied it will be filled with a human readable
	 * errormessage.
	 *
	 * @param errormsg 		[out] If supplied it will be filled with an error message.
	 *
	 * @return the errornumber or 0
	 */
	int getLastError(string& errormsg);
};

#endif /*MMSFILETRANSFER_H_*/
