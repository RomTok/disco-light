/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

/*
 * mmsprocessmonitor.cpp
 *
 *  Created on: 05.11.2008
 *      Author: sxs
 */

#include "mmstools/mmsprocessmonitor.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

MMSProcessMonitor::MMSProcessMonitor(unsigned int interval) : monitoringInterval(interval) {
	this->shutdown = false;
}

MMSProcessMonitor::~MMSProcessMonitor() {

}

void MMSProcessMonitor::commenceShutdown() {
    DEBUGMSG("PROCESSMONITOR", "Processmonitor shutdown initiated.");
	this->shutdown = true;
}

void MMSProcessMonitor::addProcess(std::string process) {
	MMSPROCESS_TASK task;
	task.cmdline = process;
	processes.push_back(task);
}

void MMSProcessMonitor::addProcess(const char *process) {
	MMSPROCESS_TASK task;
	task.cmdline = process;
	processes.push_back(task);
}

bool MMSProcessMonitor::startprocess(MMSPROCESS_TASKLIST::iterator &it) {
	pid_t pid = fork();

	if(pid == -1) {
		return false;
	}
	if(pid == 0) {
		char *argv[255];
		argv[0]=(char *)it->cmdline.c_str();
		argv[1]=NULL;
		DEBUGMSG("PROCESSMONITOR", "Starting process %s", it->cmdline.c_str());
		execv(it->cmdline.c_str(),argv);
		DEBUGMSG("PROCESSMONITOR", "Starting of process %s failed. (ERRNO: %d)", it->cmdline.c_str(), errno);
		exit(1);
	}
	//Iam the caller...
	it->pid=pid;
	return true;
}

bool MMSProcessMonitor::checkprocess(MMSPROCESS_TASKLIST::iterator &it) {
	if(kill(it->pid,0)==0)
		return true;
	else
		return false;

}
bool MMSProcessMonitor::killprocess(MMSPROCESS_TASKLIST::iterator &it) {
    DEBUGMSG("PROCESSMONITOR", "Killing process %s (%d)", it->cmdline.c_str(), it->pid);
    if(kill(it->pid,SIGTERM)==0)
		return true;
	else
		return false;

}

void MMSProcessMonitor::threadMain() {
	for(MMSPROCESS_TASKLIST::iterator it = this->processes.begin();it != this->processes.end();it++) {
	    startprocess(it);
	}

	while(1) {
		if(shutdown) {
			for(MMSPROCESS_TASKLIST::iterator it = this->processes.begin();it != this->processes.end();it++) {
				killprocess(it);
			}
			return;
		}

		for(MMSPROCESS_TASKLIST::iterator it = this->processes.begin();it != this->processes.end();it++) {
			if(!checkprocess(it)) {
				startprocess(it);
			}
		}
		int status;
		while(waitpid(-1, &status, WNOHANG)>0);
		sleep(this->monitoringInterval);
	}

}

