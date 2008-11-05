/*
 * mmsprocessmonitor.cpp
 *
 *  Created on: 05.11.2008
 *      Author: sxs
 */

#include "mmstools/mmsprocessmonitor.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

MMSProcessMonitor::MMSProcessMonitor() {
	this->shutdown = false;
}

MMSProcessMonitor::~MMSProcessMonitor() {

}

void MMSProcessMonitor::commenceShutdown() {
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
		// I am the new...
		execv(it->cmdline.c_str(),argv);
		//something went wrong...
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
		sleep(1);
	}

}

