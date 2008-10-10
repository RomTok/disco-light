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

#include "mmstools/mmslogger.h"
#include "mmstools/mmsfile.h"
#include "mmstools/tools.h"
#include <stdio.h>

MMSLogger::MMSLogger() {
    this->identity = "[UKN]";
    this->stdout = false;
}

MMSLogger::MMSLogger(string identity) {
    this->identity = "[" + identity + "]";
    this->stdout = false;
}

MMSLogger::~MMSLogger() {
}

void MMSLogger::setIdentity(string identity) {
    this->identity = "[" + identity + "]";
}

string MMSLogger::getLogfile() {
    return this->backend.getLogFile();
}

void MMSLogger::writeLog(string message) {
    struct  timeval tv;
    int     num;
    char    buffer[1280000];
    char    timebuf[12];

#ifndef MMSLOGGER_STDOUT_ONLY
    MMSFile file(this->backend.getLogFile(),MMSFM_APPEND);
#endif

    gettimeofday(&tv, NULL);

    getCurrentTimeBuffer(NULL, NULL, timebuf, NULL);

    num = snprintf( buffer, sizeof(buffer), "%s:%02ld %s: %s\n", timebuf,
                    tv.tv_usec/10000, this->identity.c_str(), message.c_str() );
#ifdef MMSLOGGER_STDOUT_ONLY
    write(fileno(::stdout), buffer, num);
#else
    file.writeBuffer(buffer,NULL,1,num);
    if(stdout)
         printf("%s\n",message.c_str());
#endif
}

void MMSLogger::clearLog() {
#ifndef MMSLOGGER_STDOUT_ONLY
    MMSFile file(this->backend.getLogFile(),MMSFM_WRITE);
#endif
}

void MMSLogger::setStdout(bool writestdout) {
	this->stdout=writestdout;
}
