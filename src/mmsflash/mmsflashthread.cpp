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

#ifndef _NO_MMSFLASH

#include "mmsflash/mmsflashthread.h"

MMSFlashThread::MMSFlashThread(MMSFlash *flash, MMSFLASHTHREAD_MODE mode, string identity) : MMSThread(identity) {
	this->flash = flash;
	this->mode = mode;
	this->started = false;
	this->stop = false;
}

void MMSFlashThread::threadMain() {
	this->started = true;
	this->stop = false;
	if (mode == MMSFLASHTHREAD_MODE_LOADER)
		this->flash->loader(this->stop);
	else
	if (mode == MMSFLASHTHREAD_MODE_PLAYER)
		this->flash->player(this->stop);
}

void MMSFlashThread::start(void) {
	this->started = false;
	MMSThread::start();
}

bool MMSFlashThread::isStarted(void) {
	return this->started;
}

void MMSFlashThread::invokeStop(void) {
	this->stop = true;
}

void MMSFlashThread::waitUntilStopped(void) {
	while (isRunning()) msleep(50);
}

#endif /* _NO_MMSFLASH */

