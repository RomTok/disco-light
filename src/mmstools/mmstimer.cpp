/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mmstools/mmstimer.h"
#include <cerrno>
#include <cstring>

extern "C" {
#include <sys/time.h>
#include <time.h>
}

MMSTimer::MMSTimer(bool singleShot) :
	MMSThread("MMSTimer"),
	singleShot(singleShot),
	action(START),
	secs(0),
	nSecs(0) {
	MMSThread::setStacksize(131072);

	pthread_mutex_init(&this->mutex, NULL);
	pthread_cond_init(&this->cond, NULL);
}

MMSTimer::~MMSTimer() {
	this->action = QUIT;
	stop();
	join();

	pthread_cond_destroy(&this->cond);
	pthread_mutex_destroy(&this->mutex);
}

void MMSTimer::start(unsigned int milliSeconds) {
	if(isRunning()) {
		return;
	}

	this->nSecs = (milliSeconds % 1000) * 1000000;
	this->secs = milliSeconds / 1000;

	MMSThread::start();
}

void MMSTimer::restart() {
	if(!isRunning()) {
		return;
	}

	pthread_mutex_lock(&mutex);
	this->action = RESTART;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
}

void MMSTimer::stop() {
	if(!isRunning()) {
		return;
	}

	pthread_mutex_lock(&mutex);
	this->action = STOP;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
}

void* callTimeout(void *data) {
	sigc::signal<void> *timeOutHandler = (sigc::signal<void>*)data;

	timeOutHandler->emit();

	return NULL;
}

void MMSTimer::threadMain() {
	if(this->secs == 0 && this->nSecs == 0) {
		return;
	}

	struct timespec absTime;
	struct timeval  absTimeGet;

	pthread_mutex_lock(&this->mutex);
	while(this->action != QUIT) {
		gettimeofday(&absTimeGet, NULL);
		absTime.tv_sec  = absTimeGet.tv_sec + secs;
		absTime.tv_nsec = (absTimeGet.tv_usec * 1000) + nSecs;

		if(absTime.tv_nsec > 999999999) {
			absTime.tv_sec += 1;
			absTime.tv_nsec -= 999999999;
		}

		if(pthread_cond_timedwait(&this->cond, &this->mutex, &absTime) == ETIMEDOUT) {
			pthread_t t;
			pthread_create(&t, NULL, callTimeout, &this->timeOut);
			if(this->singleShot)
				break;
		} else {
			if(this->action == STOP)
				pthread_cond_wait(&this->cond, &this->mutex);
		}
	}
	pthread_mutex_unlock(&this->mutex);
}
