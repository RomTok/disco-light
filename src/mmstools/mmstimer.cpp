/***************************************************************************
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      René Bählkow     <rbaehlkow@berlinux-solutions.de                  *
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

#include "mmstools/mmstimer.h"
#include <cerrno>
#include <cstring>

MMSTimer::MMSTimer(bool singleShot) :
	MMSThread("MMSTimer"), repeat(false), quit(false), secs(0),
			nSecs(0), threadID(0)
{
	this->singleShot = singleShot;

	MMSThread::setStacksize(131072);

	pthread_mutex_init(&interruptMutex, NULL);
	pthread_mutex_init(&startMutex, NULL);
	pthread_mutex_init(&stopMutex, NULL);

	pthread_cond_init(&interruptCond, NULL);
	pthread_cond_init(&startCond, NULL);
	pthread_cond_init(&stopCond, NULL);
}

MMSTimer::~MMSTimer()
{
	quit = true;
	stop();
	join();

	pthread_mutex_destroy(&interruptMutex);
	pthread_mutex_destroy(&startMutex);
	pthread_mutex_destroy(&stopMutex);

	pthread_cond_destroy(&interruptCond);
	pthread_cond_destroy(&startCond);
	pthread_cond_destroy(&stopCond);
}

void MMSTimer::start(unsigned int milliSeconds)
{
	if(isRunning()) {
		return;
	}

	nSecs = (milliSeconds % 1000) * 1000000;
	secs = milliSeconds / 1000;

	repeat = true;
	MMSThread::start();
}

void MMSTimer::restart()
{
	/* see comment in stop() */
	if(!isRunning() || pthread_self() == threadID) {
		return;
	}

	stopWithoutCheck();

	pthread_mutex_lock(&interruptMutex);
	repeat = true;
	pthread_mutex_unlock(&interruptMutex);


	/* signal to outer thread loop to restart */
	pthread_mutex_lock(&startMutex);
	pthread_cond_signal(&startCond);
	pthread_mutex_unlock(&startMutex);
}

void MMSTimer::stop()
{
	if(!isRunning()) {
		return;
	}

	/*
	 * This happens if timeOut.emit() is called in threadMain and its
	 * signal handler contains a call of this function. It causes
	 * deadlocks so we have to return here.
	 */
	if(pthread_self() == threadID) {
		return;
	}

	stopWithoutCheck();
}

void MMSTimer::stopWithoutCheck()
{
	/* signal to inner thread loop to stop waiting for timeout */
	pthread_mutex_lock(&interruptMutex);
	pthread_cond_signal(&interruptCond);
	pthread_mutex_unlock(&interruptMutex);


	/* make sure inner thread loop is left */
	pthread_mutex_lock(&stopMutex);
	/* TODO: Check what happens on compiler optimizations. */
	//printf("stopped\n");
	pthread_mutex_unlock(&stopMutex);
}

void MMSTimer::threadMain()
{
	threadID = pthread_self();
	if(secs == 0 && nSecs == 0) {
		return;
	}

	struct timespec absTime;
	int rc = 0;
	while(!quit) {
		pthread_mutex_lock(&stopMutex);
		while(repeat) {
			clock_gettime(CLOCK_REALTIME, &absTime);
			absTime.tv_sec += secs;
			absTime.tv_nsec += nSecs;

			if(absTime.tv_nsec > 999999999) {
				absTime.tv_sec += 1;
				absTime.tv_nsec -= 999999999;
			}

			pthread_mutex_lock(&interruptMutex);
			rc = pthread_cond_timedwait(&interruptCond,
					&interruptMutex, &absTime);

			if(rc == ETIMEDOUT) {
				//printf("timer: emit\n");
				timeOut.emit();
			} else if(rc == 0) {
				//printf("timer: cond_signal\n");
				repeat = false;
			} else {
				//printf("timer: error: %d %s\n", rc, strerror(rc));
			}

			if(singleShot) {
				repeat = false;
			}
			pthread_mutex_unlock(&interruptMutex);
		}
		pthread_mutex_unlock(&stopMutex);

		if(quit) {
			break;
		}

		/* wait for a restart request */
		pthread_mutex_lock(&startMutex);
		pthread_cond_wait(&startCond, &startMutex);
		pthread_mutex_unlock(&startMutex);
	}
}
