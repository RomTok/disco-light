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

#include "mmsgui/fb/mmsfbwindowmanagerthread.h"
#include "mmsgui/fb/mmsfbwindowmanager.h"

MMSFBWindowManagerThread::MMSFBWindowManagerThread(MMSFBSurface **high_freq_surface,
                                                   MMSFBSurface **high_freq_saved_surface,
                                                   int *high_freq_lastflip,
                                                   MMSMutex *lock) {
    this->high_freq_surface = high_freq_surface;
    this->high_freq_saved_surface = high_freq_saved_surface;
    this->high_freq_lastflip = high_freq_lastflip;
    this->lock = lock;
}

void MMSFBWindowManagerThread::threadMain() {
	int	pointer_opacity = 0;
	int	hidecnt = 1;

	while (1) {

		// fade out the pointer
		if (!mmsfbwindowmanager->button_pressed) {
			pointer_opacity = mmsfbwindowmanager->getPointerOpacity();
			if (pointer_opacity > 0) {
				if (pointer_opacity == 255)
					hidecnt=1;
				else
					hidecnt*=3;
				if (pointer_opacity > hidecnt)
					mmsfbwindowmanager->setPointerOpacity(pointer_opacity - hidecnt);
				else
					mmsfbwindowmanager->setPointerOpacity(0);
			}
		}


        if (!*(this->high_freq_surface)) {
            /* have no region */
            sleep(1);
            continue;
        }

        /* get the flip time */
        struct  timeval tv;
        gettimeofday(&tv, NULL);
        int newfliptime = (((int)tv.tv_sec)%1000000)*1000+((int)tv.tv_usec)/1000;
        int diff = newfliptime - *(this->high_freq_lastflip);
        if ((diff > 0)&&(diff < 1000)) {
            /* already running */
            msleep(200);
            continue;
        }

        /* have a surface but it was not flipped fast enough */
        lock->lock();
        if (!*(this->high_freq_surface)) {
            lock->unlock();
            continue;
        }
        if (*(this->high_freq_saved_surface)) {
            /* copy saved surface because window works direct with layer */
            (*(this->high_freq_surface))->setBlittingFlags(MMSFB_BLIT_NOFX);
            (*(this->high_freq_surface))->blit(*(this->high_freq_saved_surface), NULL, 0, 0);
        }
        DEBUGOUT("flipped not fast enough");
        mmsfbwindowmanager->flipSurface(*(this->high_freq_surface), NULL,
                                       MMSFB_FLIP_NONE, true);
        *(this->high_freq_surface) = NULL;
        *(this->high_freq_saved_surface) = NULL;
        *(this->high_freq_lastflip) = 0;
        lock->unlock();
    }
}

