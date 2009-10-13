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

#ifndef MMSFLASHTHREAD_H_
#define MMSFLASHTHREAD_H_

#ifndef _NO_MMSFLASH

#include "mms.h"
#include "mmsflash/mmsflash.h"

typedef enum {
	MMSFLASHTHREAD_MODE_LOADER = 0,
	MMSFLASHTHREAD_MODE_PLAYER = 1
} MMSFLASHTHREAD_MODE;

class MMSFlashThread : public MMSThread {
    private:
        //! access to the flash object
        MMSFlash	*flash;

    	//! mode of the thread
    	MMSFLASHTHREAD_MODE	mode;

    	//! start flag, means that the thread was started after start(), but it can be finished
    	bool 	started;

    	//! stop flag
    	bool 	stop;

        //! main routine
        void threadMain();
    public:
        MMSFlashThread(MMSFlash *flash, MMSFLASHTHREAD_MODE mode, string identity = "MMSFlashThread");

        //! start the thread
        void start(void);

        //! is the thread started
        bool isStarted(void);

        //! thread should stop
        void invokeStop(void);

        //! waiting until end of the thread
        void waitUntilStopped(void);
};

#endif /* _NO_MMSFLASH */

#endif /*MMSFLASHTHREAD_H_*/
