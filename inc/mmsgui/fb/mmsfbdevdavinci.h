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

#ifndef MMSFBDEVDAVINCI_H_
#define MMSFBDEVDAVINCI_H_

#ifdef __HAVE_FBDEV__

#include "mmsgui/fb/mmsfbdev.h"

class MMSFBDevDavinci : public MMSFBDev {
    private:
//    	bool scart_rgb_cable;
//    	bool tv_std_pal;

    	unsigned char *mmio_base;

    public:
        MMSFBDevDavinci();
        virtual ~MMSFBDevDavinci();

        bool openDevice(char *device_file = NULL, int console = -1);
        void closeDevice();

        bool testLayer(int layer_id);
        bool initLayer(int layer_id, int width, int height, MMSFBSurfacePixelFormat pixelformat);

        bool getFrameBufferPtr(int layer_id, void **ptr, int *pitch, int *width, int *height);
};

#endif

#endif /* MMSFBDEVDAVINCI_H_ */
