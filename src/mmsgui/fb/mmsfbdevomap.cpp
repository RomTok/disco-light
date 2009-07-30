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

#include "mmsgui/fb/mmsfbdevomap.h"
#include <sys/ioctl.h>
#include <cstring>

#define INITCHECK  if(!this->isinitialized){MMSFB_SetError(0,"MMSFBDevOmap is not initialized");return false;}

MMSFBDevOmap::MMSFBDevOmap() {
	this->osd  = NULL;
	this->vid0 = NULL;
	this->vid1 = NULL;
}

MMSFBDevOmap::~MMSFBDevOmap() {
	closeDevice();
}

bool MMSFBDevOmap::openDevice(char *device_file, int console) {
	// open davinci frame buffers
	for (int i = 0; i < 3; i++) {
		MMSFBDev *fbdev;
		char      dev[100];
		sprintf(dev, "/dev/fb%d", i);
		fbdev = new MMSFBDev();
		if (!fbdev->openDevice(dev, (!i)?-1:-2)) {
			delete fbdev;
			closeDevice();
			return false;
		}

		if (memcmp(fbdev->fix_screeninfo.id, "omapfb", 6) == 0)
			this->osd = fbdev;
		else
		if (memcmp(fbdev->fix_screeninfo.id, "omapfbv0", 8) == 0)
			this->vid0 = fbdev;
		else
		if (memcmp(fbdev->fix_screeninfo.id, "omapfbv1", 8) == 0)
			this->vid1 = fbdev;
		else {
			// not supported
			printf("MMSFBDevOmap: unsupported accelerator %d (%.16s)\n", fbdev->fix_screeninfo.accel, fbdev->fix_screeninfo.id);
			delete fbdev;
			closeDevice();
			return false;
		}

		if (!i && !this->osd) {
			// osd must be at /dev/fb0
			printf("MMSFBDevOmap: /dev/fb0 is not osd\n");
			closeDevice();
			return false;
		}
	}

    // all initialized :)
	this->isinitialized = true;

	return true;
}

void MMSFBDevOmap::closeDevice() {
	// close frame buffers
	if (this->vid1) delete this->vid1;
	if (this->vid0) delete this->vid0;
	if (this->osd)  delete this->osd;

	// reset all other
	this->isinitialized = false;
}

bool MMSFBDevOmap::waitForVSync() {
	// is initialized?
	INITCHECK;

	static const int s = 0;
	if (ioctl(this->osd->fd, FBIO_WAITFORVSYNC, &s)) {
		// failed, well then???
	}

	return true;
}

bool MMSFBDevOmap::panDisplay(int buffer_id, void *framebuffer_base) {
	// is initialized?
	INITCHECK;

	if (framebuffer_base == this->osd->framebuffer_base) {
		// Graphic layer (OSD)
		return this->osd->panDisplay(buffer_id);
	}
	else
	if (framebuffer_base == this->vid0->framebuffer_base) {
		// Video layer (VID0)
		return this->vid0->panDisplay(buffer_id);
	}
	else
	if (framebuffer_base == this->vid1->framebuffer_base) {
		// Video layer (VID1)
		return this->vid1->panDisplay(buffer_id);
	}

	// check framebuffer_base pointer
	printf("MMSFBDevOmap: framebuffer base pointer not correct\n");
	return false;
}

bool MMSFBDevOmap::testLayer(int layer_id) {
	// is initialized?
	INITCHECK;

	switch (layer_id) {
	case 0:
		// default fbdev primary layer 0 on primary screen 0
	    return true;
	case 1:
		// Video layer (VID0)
		return true;
	case 2:
		// Video layer (VID1)
		return true;
	default:
    	printf("MMSFBDevOmap: layer %d is not supported\n", layer_id);
    	break;
	}

	return false;
}


bool MMSFBDevOmap::initLayer(int layer_id, int width, int height, MMSFBSurfacePixelFormat pixelformat, int backbuffer) {
	// is initialized?
	INITCHECK;

	switch (layer_id) {
	case 0:
		// default fbdev primary layer 0 on primary screen 0
		if   ((pixelformat != MMSFB_PF_ARGB3565)
			&&(pixelformat != MMSFB_PF_RGB16)) {
			printf("MMSFBDevOmap: OSD Layer needs pixelformat ARGB3565 or RGB16, but %s given\n", getMMSFBPixelFormatString(pixelformat).c_str());
			return false;
		}

		if (backbuffer) {
			printf("MMSFBDevOmap: OSD Layer does not support backbuffer handling\n");
			return false;
		}

		// enable OSD
		if (this->osd->initLayer(0, width, height, MMSFB_PF_RGB16, backbuffer)) {
			// set values
			this->layers[layer_id].width = width;
			this->layers[layer_id].height = height;
			this->layers[layer_id].pixelformat = pixelformat;

			// save the buffers
			memcpy(this->layers[layer_id].buffers, this->osd->layers[0].buffers, sizeof(this->osd->layers[0].buffers));

			// clear layer
			MMSFBColor color(0x00, 0x00, 0x00, 0xff);
			mmsfb_fillrectangle_rgb16(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
									  0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);

			// layer is initialized
			this->layers[layer_id].isinitialized = true;

			printf("MMSFBDevOmap: OSD Layer %d initialized with %dx%dx%d, pixelformat %s\n",
						layer_id, width, height, backbuffer+1, getMMSFBPixelFormatString(pixelformat).c_str());

			return true;
		}
		return false;

	case 1:
		// Video layer (VID0)
		if (pixelformat != MMSFB_PF_YUY2) {
			printf("MMSFBDevOmap: Video Layer needs pixelformat YUY2, but %s given\n", getMMSFBPixelFormatString(pixelformat).c_str());
			return false;
		}

		// disable VID1
		this->vid1->initLayer(0, 0, 0, MMSFB_PF_NONE, false);

		// enable VID0
		if (this->vid0->initLayer(0, width, height, MMSFB_PF_YUY2, backbuffer)) {
			// set values
			this->layers[layer_id].width = width;
			this->layers[layer_id].height = height;
			this->layers[layer_id].pixelformat = pixelformat;

			// save the buffers
			memcpy(this->layers[layer_id].buffers, this->vid0->layers[0].buffers, sizeof(this->vid0->layers[0].buffers));

			// clear layer
			MMSFBColor color(0x00, 0x00, 0x00, 0xff);
			mmsfb_fillrectangle_yuy2(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
			                         0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);

			// layer is initialized
			this->layers[layer_id].isinitialized = true;

			printf("MMSFBDevOmap: Video Layer %d initialized with %dx%dx%d, pixelformat %s\n",
						layer_id, width, height, backbuffer+1, getMMSFBPixelFormatString(pixelformat).c_str());

			return true;
		}
		return false;

	case 2:
		// Video layer (VID1)
		if (pixelformat != MMSFB_PF_YUY2) {
			printf("MMSFBDevOmap: Video Layer needs pixelformat YUY2, but %s given\n", getMMSFBPixelFormatString(pixelformat).c_str());
			return false;
		}

		// disable VID0
		this->vid0->initLayer(0, 0, 0, MMSFB_PF_NONE, false);

		// enable VID1
		if (this->vid1->initLayer(0, width, height, MMSFB_PF_YUY2, backbuffer)) {
			// set values
			this->layers[layer_id].width = width;
			this->layers[layer_id].height = height;
			this->layers[layer_id].pixelformat = pixelformat;

			// save the buffers
			memcpy(this->layers[layer_id].buffers, this->vid1->layers[0].buffers, sizeof(this->vid1->layers[0].buffers));

			// clear layer
			MMSFBColor color(0x00, 0x00, 0x00, 0xff);
			mmsfb_fillrectangle_yuy2(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
			                         0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);

			// layer is initialized
			this->layers[layer_id].isinitialized = true;

			printf("MMSFBDevOmap: Video Layer %d initialized with %dx%dx%d, pixelformat %s\n",
						layer_id, width, height, backbuffer+1, getMMSFBPixelFormatString(pixelformat).c_str());

			return true;
		}
		return false;
	default:
		printf("MMSFBDevOmap: layer %d is not supported\n", layer_id);
		break;
	}

	return false;
}

bool MMSFBDevOmap::vtGetFd(int *fd) {
	if (this->osd->vt.fd != -1) {
		*fd = this->osd->vt.fd;
		return true;
	}
	return false;
}

#endif
