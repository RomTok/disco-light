/***************************************************************************
 *   Copyright (C) 2005-2008 by Berlinux Solutions                         *
 *                                                                         *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>                *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#ifndef MMSFLASHTHREAD_H_
#define MMSFLASHTHREAD_H_

#include "mms.h"

typedef enum {
	MMSFLASHTHREAD_MODE_LOADER = 0,
	MMSFLASHTHREAD_MODE_PLAYER = 1
} MMSFLASHTHREAD_MODE;

//class MMSFlash;

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

        //! is started
        bool isStarted(void);

        void invokeStop(void);
        void waitUntilStopped(void);
};

#endif /*MMSFLASHTHREAD_H_*/
