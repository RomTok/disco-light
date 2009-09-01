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

#ifndef MMSFBDEVOMAP_H_
#define MMSFBDEVOMAP_H_

#ifdef __HAVE_FBDEV__

#include "mmsgui/fb/mmsfbdev.h"

class MMSFBDevOmap : public MMSFBDev {
    private:
    	typedef struct {
        	MMSFBDevOmap   			*fbdev;
        	char					device[100];
        	int						width;
        	int						height;
        	MMSFBSurfacePixelFormat	pixelformat;
        	int						backbuffer;
    	} MMSFBDevOmapLayer;

    	MMSFBDevOmapLayer	osd0;
    	MMSFBDevOmapLayer	vid;
    	MMSFBDevOmapLayer	osd1;
    	MMSFBDevOmapLayer	*primary;

    public:
        MMSFBDevOmap();
        virtual ~MMSFBDevOmap();

        bool openDevice(char *device_file = NULL, int console = -1);
        void closeDevice();

        bool waitForVSync();
        bool panDisplay(int buffer_id, void *framebuffer_base);

        bool testLayer(int layer_id);
        bool initLayer(int layer_id, int width, int height, MMSFBSurfacePixelFormat pixelformat, int backbuffer = 0);

        bool releaseLayer(int layer_id);
        bool restoreLayer(int layer_id);

        bool setMode(int width, int height, MMSFBSurfacePixelFormat pixelformat, int backbuffer = 0);

    private:

        bool vtGetFd(int *fd);
};

#endif

#endif /* MMSFBDEVOMAP_H_ */
