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

#ifdef __HAVE_FBDEV__

#include "mmsgui/fb/mmsfbdevdavinci.h"
#include <sys/ioctl.h>

#define INITCHECK  if(!this->isinitialized){MMSFB_SetError(0,"MMSFBDevDavinci is not initialized");return false;}

MMSFBDevDavinci::MMSFBDevDavinci() {
	this->osd0 = NULL;
	this->osd1 = NULL;
	this->vid0 = NULL;
	this->vid1 = NULL;
}

MMSFBDevDavinci::~MMSFBDevDavinci() {
	closeDevice();
}

bool MMSFBDevDavinci::openDevice(char *device_file, int console) {
	// open davinci frame buffers
	for (int i = 0; i < 4; i++) {
		MMSFBDev *fbdev;
		char      dev[100];
		sprintf(dev, "/dev/fb%d", i);
		fbdev = new MMSFBDev();
		if (!fbdev->openDevice(dev, (!i)?-1:-2)) {
			delete fbdev;
			closeDevice();
			return false;
		}

		if (memcmp(fbdev->fix_screeninfo.id, "dm_osd0_fb", 10) == 0)
			this->osd0 = fbdev;
		else
		if (memcmp(fbdev->fix_screeninfo.id, "dm_vid0_fb", 10) == 0)
			this->vid0 = fbdev;
		else
		if (memcmp(fbdev->fix_screeninfo.id, "dm_osd1_fb", 10) == 0)
			this->osd1 = fbdev;
		else
		if (memcmp(fbdev->fix_screeninfo.id, "dm_vid1_fb", 10) == 0)
			this->vid1 = fbdev;
		else {
			// not supported
			printf("MMSFBDevDavinci: unsupported accelerator %d (%.16s)\n", fbdev->fix_screeninfo.accel, fbdev->fix_screeninfo.id);
			delete fbdev;
			closeDevice();
			return false;
		}

		if (!i && !this->osd0) {
			// osd0 must be at /dev/fb0
			printf("MMSFBDevDavinci: /dev/fb0 is not osd0\n");
			closeDevice();
			return false;
		}
	}

    // all initialized :)
	this->isinitialized = true;

	return true;
}

void MMSFBDevDavinci::closeDevice() {
	// close frame buffers
	if (this->vid1) delete this->vid1;
	if (this->vid0) delete this->vid0;
	if (this->osd1) delete this->osd1;
	if (this->osd0) delete this->osd0;

	// reset all other
	this->isinitialized = false;
}

bool MMSFBDevDavinci::waitForVSync() {
	// is initialized?
	INITCHECK;

	static const int s = 0;
	if (ioctl(this->osd0->fd, FBIO_WAITFORVSYNC, &s)) {
		// failed, well then???
	}

	return true;
}

bool MMSFBDevDavinci::testLayer(int layer_id) {
	// is initialized?
	INITCHECK;

	switch (layer_id) {
	case 0:
		// default fbdev primary layer 0 on primary screen 0
	    return true;
	case 1:
		// Video layer
		return true;
	default:
    	printf("MMSFBDevDavinci: layer %d is not supported\n", layer_id);
    	break;
	}

	return false;
}


bool MMSFBDevDavinci::initLayer(int layer_id, int width, int height, MMSFBSurfacePixelFormat pixelformat) {
	// is initialized?
	INITCHECK;

	switch (layer_id) {
	case 0:
		// default fbdev primary layer 0 on primary screen 0
		if (pixelformat == MMSFB_PF_ARGB3565) {
			// init the two davinci osd "windows" using the 3 bit alpha channel
			if (this->osd0->initLayer(0, width, height, MMSFB_PF_RGB16)) {
				// init osd1 attribute plane
				if (this->osd1->initLayer(0, width, height, MMSFB_PF_A4)) {
	memset(this->osd1->framebuffer_base, 0x44, 768*480/2);

					// set values
					this->layers[layer_id].planes.ptr = this->osd0->framebuffer_base;
					this->layers[layer_id].planes.ptr2 = this->osd1->framebuffer_base;
					this->layers[layer_id].planes.ptr3 = NULL;
					this->layers[layer_id].planes.pitch = this->osd0->layers[0].planes.pitch;
					this->layers[layer_id].planes.pitch2 = this->osd1->layers[0].planes.pitch;
					this->layers[layer_id].planes.pitch3 = 0;
					this->layers[layer_id].width = width;
					this->layers[layer_id].height = height;
					this->layers[layer_id].pixelformat = pixelformat;

					// layer is initialized
					this->layers[layer_id].isinitialized = true;

					printf("MMSFBDevDavinci: OSD Layer %d initialized with %dx%d, pixelformat ARGB3565\n", layer_id, width, height);

					return true;
				}
			}
			return false;
		}
		else
		if (pixelformat == MMSFB_PF_RGB16) {
			// init only one davinci osd "window" without transparency
			if (this->osd0->initLayer(0, width, height, MMSFB_PF_RGB16)) {
				// set values
				this->layers[layer_id].planes.ptr = this->osd0->framebuffer_base;
				this->layers[layer_id].planes.ptr2 = NULL;
				this->layers[layer_id].planes.ptr3 = NULL;
				this->layers[layer_id].planes.pitch = this->osd0->layers[0].planes.pitch;
				this->layers[layer_id].planes.pitch2 = 0;
				this->layers[layer_id].planes.pitch3 = 0;
				this->layers[layer_id].width = width;
				this->layers[layer_id].height = height;
				this->layers[layer_id].pixelformat = pixelformat;

				// layer is initialized
				this->layers[layer_id].isinitialized = true;

				printf("MMSFBDevDavinci: OSD Layer %d initialized with %dx%d, pixelformat RGB16, no transparency!\n", layer_id, width, height);

				return true;
			}
			return false;
		}
		else {
			printf("MMSFBDevDavinci: OSD Layer needs pixelformat ARGB3565 or RGB16, but %s given\n", getMMSFBPixelFormatString(pixelformat).c_str());
			return false;
		}
	case 1:
		// Video layer
		if (pixelformat != MMSFB_PF_YUY2) {
			printf("MMSFBDevDavinci: Video Layer needs pixelformat YUY2, but %s given\n", getMMSFBPixelFormatString(pixelformat).c_str());
			return false;
		}

		if (this->vid1->initLayer(0, width, height, MMSFB_PF_YUY2)) {
memset(this->vid1->framebuffer_base, 0xaa, 720*240*2);
memset(((char*)this->vid1->framebuffer_base)+720*240*2, 0x66, 720*240*2);


			// set values
			this->layers[layer_id].planes.ptr = this->vid1->framebuffer_base;
			this->layers[layer_id].planes.ptr2 = NULL;
			this->layers[layer_id].planes.ptr3 = NULL;
			this->layers[layer_id].planes.pitch = this->vid1->layers[0].planes.pitch;
			this->layers[layer_id].planes.pitch2 = 0;
			this->layers[layer_id].planes.pitch3 = 0;
			this->layers[layer_id].width = width;
			this->layers[layer_id].height = height;
			this->layers[layer_id].pixelformat = pixelformat;

			// layer is initialized
			this->layers[layer_id].isinitialized = true;

			return true;
		}
		return false;
	default:
		printf("MMSFBDevDavinci: layer %d is not supported\n", layer_id);
		break;
	}

	return false;
}

bool MMSFBDevDavinci::vtGetFd(int *fd) {
	if (this->osd0->vt.fd != -1) {
		*fd = this->osd0->vt.fd;
		return true;
	}
	return false;
}

#endif
