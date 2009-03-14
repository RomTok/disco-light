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
//	this->scart_rgb_cable = false;
//	this->tv_std_pal = true;
	this->mmio_base = NULL;
}

MMSFBDevDavinci::~MMSFBDevDavinci() {
	closeDevice();
}

bool MMSFBDevDavinci::openDevice(char *device_file, int console) {
	// open frame buffer
	if (!MMSFBDev::openDevice(device_file, console))
		return false;

	// check fb accel, but accel is 0, so have to check id
	if (memcmp(this->fix_screeninfo.id, "dm_osd0_fb", 10)) {
		// not supported
		printf("MMSFBDevDavinci: unsupported accelerator %d (%.16s)\n", this->fix_screeninfo.accel, this->fix_screeninfo.id);
		closeDevice();
		return false;
	}

    // map mmio
	if (!mapMmio(&this->mmio_base)) {
		closeDevice();
		return false;
	}

	return true;
}

void MMSFBDevDavinci::closeDevice() {
	if (this->mmio_base)
		unmapMmio(this->mmio_base);

	// close frame buffer
	MMSFBDev::closeDevice();
}

bool MMSFBDevDavinci::testLayer(int layer_id) {
	// is initialized?
	INITCHECK;

	switch (layer_id) {
	case 0:
		// default fbdev primary layer 0 on primary screen 0
	    return MMSFBDev::testLayer(layer_id);
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
		return MMSFBDev::initLayer(layer_id, width, height, pixelformat);
/*	case 2:
		// TVOut layer
		// check input
		if (width != 720) {
			printf("MMSFBDevMatrox: TVOut needs layer width 720, but %d given\n", width);
			return false;
		}
		if ((height != 576)&&(height != 480)) {
			printf("MMSFBDevMatrox: TVOut needs layer height 576 (PAL) or 480 (NTSC), but %d given\n", height);
			return false;
		}
		if ((pixelformat != MMSFB_PF_I420)&&(pixelformat != MMSFB_PF_YV12)) {
			printf("MMSFBDevMatrox: TVOut needs pixelformat I420 or YV12, but %s given\n", getMMSFBPixelFormatString(pixelformat).c_str());
			return false;
		}

		// set values
		if (width % 128)
			this->layers[layer_id].pitch = ((width / 128) + 1) * 128;
		else
			this->layers[layer_id].pitch = width;
		this->layers[layer_id].width = width;
		this->layers[layer_id].height = height;
		this->layers[layer_id].pixelformat = pixelformat;

		// pal or ntsc?
		this->tv_std_pal = (this->layers[layer_id].height == 576);

		// switch to layer
		buildCRTC2Regs();
		buildCRTC2Buffer();
		enableCRTC2();

		// layer is initialized
		this->layers[layer_id].isinitialized = true;

		// this layer is on screen 1
		this->active_screen = 1;

		printf("MMSFBDevMatrox: TVOut layer %d initialized with %dx%d (%s), pixelformat %s\n",
				layer_id, width, height, (this->tv_std_pal)?"PAL":"NTSC", getMMSFBPixelFormatString(pixelformat).c_str());

		return true;*/
	default:
		printf("MMSFBDevDavinci: layer %d is not supported\n", layer_id);
		break;
	}

	return false;
}


#endif
