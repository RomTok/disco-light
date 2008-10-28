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
            (*(this->high_freq_surface))->setBlittingFlags((MMSFBSurfaceBlittingFlags) DSBLIT_NOFX);
            (*(this->high_freq_surface))->blit(*(this->high_freq_saved_surface), NULL, 0, 0);
        }
        DEBUGOUT("flipped not fast enough");
        mmsfbwindowmanager->flipSurface(*(this->high_freq_surface), NULL,
                                       (MMSFBSurfaceFlipFlags)0, true);
        *(this->high_freq_surface) = NULL;
        *(this->high_freq_saved_surface) = NULL;
        *(this->high_freq_lastflip) = 0;
        lock->unlock();
    }
}

