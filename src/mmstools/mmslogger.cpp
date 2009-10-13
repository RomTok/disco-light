/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
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
