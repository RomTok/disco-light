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
	this->osd0.fbdev	= NULL;
	this->vid.fbdev		= NULL;
	this->osd1.fbdev	= NULL;
	this->primary		= NULL;
}

MMSFBDevOmap::~MMSFBDevOmap() {
	closeDevice();
}

bool MMSFBDevOmap::openDevice(char *device_file, int console) {
	// close the device if opened
	closeDevice();

	// open omap frame buffers
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

		if (memcmp(fbdev->fix_screeninfo.id, "omapfb", 6) == 0) {
			switch (i) {
			case 0:
				this->osd0.fbdev = fbdev;
				strcpy(this->osd0.device, dev);
				this->osd0.width = 0;
				this->primary = &this->osd0;
				// disable device
				this->osd0.fbdev->initLayer(0, 0, 0, MMSFB_PF_NONE, false);
				break;
			case 1:
				this->vid.fbdev = fbdev;
				strcpy(this->vid.device, dev);
				this->vid.width = 0;
				if (!this->primary)
					this->primary = &this->vid;
				// disable device
				this->vid.fbdev->initLayer(0, 0, 0, MMSFB_PF_NONE, false);
				break;
			case 2:
				this->osd1.fbdev = fbdev;
				strcpy(this->osd1.device, dev);
				this->osd1.width = -1;
				this->primary = &this->osd1;
				break;
			}
		}
		else {
			// not supported
			printf("MMSFBDevOmap: unsupported accelerator %d (%.16s)\n", fbdev->fix_screeninfo.accel, fbdev->fix_screeninfo.id);
			delete fbdev;
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
	if (this->osd1.fbdev) {
		delete this->osd1.fbdev;
		this->osd1.fbdev = NULL;
	}
	if (this->vid.fbdev) {
		delete this->vid.fbdev;
		this->vid.fbdev = NULL;
	}
	if (this->osd0.fbdev) {
		delete this->osd0.fbdev;
		this->osd0.fbdev = NULL;
	}
	this->primary = NULL;

	// reset all other
	this->isinitialized = false;
}

bool MMSFBDevOmap::waitForVSync() {
	// is initialized?
	INITCHECK;

	if (!this->primary->fbdev)
		return false;

	static const int s = 0;
	if (ioctl(this->primary->fbdev->fd, FBIO_WAITFORVSYNC, &s)) {
		// failed, well then???
	}

	return true;
}

bool MMSFBDevOmap::panDisplay(int buffer_id, void *framebuffer_base) {
	// is initialized?
	INITCHECK;

	if (framebuffer_base == this->osd0.fbdev->framebuffer_base) {
		// Graphic layer (OSD0)
		if (this->osd0.fbdev)
			return this->osd0.fbdev->panDisplay(buffer_id);
		return false;
	}
	else
	if (framebuffer_base == this->vid.fbdev->framebuffer_base) {
		// Video layer (VID)
		if (this->vid.fbdev)
			return this->vid.fbdev->panDisplay(buffer_id);
		return false;
	}
	else
	if (framebuffer_base == this->osd1.fbdev->framebuffer_base) {
		// Graphic layer (OSD1)
		if (this->osd1.fbdev)
			return this->osd1.fbdev->panDisplay(buffer_id);
		return false;
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
		// Video layer
		return true;
	case 2:
		// OSD layer
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
		if (!this->osd0.fbdev) {
			printf("MMSFBDevOmap: OSD Layer %d not initialized\n", layer_id);
			return false;
		}

		if   ((pixelformat != MMSFB_PF_ARGB)
			&&(pixelformat != MMSFB_PF_RGB32)
			&&(pixelformat != MMSFB_PF_RGB16)) {
			printf("MMSFBDevOmap: OSD Layer %d needs pixelformat ARGB, RGB32 or RGB16, but %s given\n",
						layer_id, getMMSFBPixelFormatString(pixelformat).c_str());
			return false;
		}

		if (backbuffer) {
			printf("MMSFBDevOmap: OSD Layer %d does not support backbuffer handling\n", layer_id);
			return false;
		}

		// enable OSD0
		if (this->osd0.fbdev->initLayer(0, width, height, pixelformat, backbuffer)) {
			// set values
			this->layers[layer_id].width = width;
			this->layers[layer_id].height = height;
			this->layers[layer_id].pixelformat = pixelformat;

			// save the buffers
			memcpy(this->layers[layer_id].buffers, this->osd0.fbdev->layers[0].buffers, sizeof(this->osd0.fbdev->layers[0].buffers));

			// clear layer
			MMSFBColor color(0x00, 0x00, 0x00, 0xff);
			switch (pixelformat) {
			case MMSFB_PF_ARGB:
				mmsfb_fillrectangle_argb(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
										 0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);
				break;
			case MMSFB_PF_RGB32:
				mmsfb_fillrectangle_rgb32(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
										  0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);
				break;
			case MMSFB_PF_RGB16:
				mmsfb_fillrectangle_rgb16(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
										  0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);
				break;
			default:
				break;
			}

			// layer is initialized
			this->layers[layer_id].isinitialized = true;

			printf("MMSFBDevOmap: OSD Layer %d initialized with %dx%dx%d, pixelformat %s\n",
						layer_id, width, height, backbuffer+1, getMMSFBPixelFormatString(pixelformat).c_str());

			return true;
		}
		return false;

	case 1:
		// Video layer (VID)
		if (!this->vid.fbdev) {
			printf("MMSFBDevOmap: Video Layer %d not initialized\n", layer_id);
			return false;
		}

		if   ((pixelformat != MMSFB_PF_YUY2)
			&&(pixelformat != MMSFB_PF_RGB32)) {
			printf("MMSFBDevOmap: Video Layer %d needs pixelformat YUY2 or RGB32, but %s given\n",
						layer_id, getMMSFBPixelFormatString(pixelformat).c_str());
			return false;
		}

		// enable VID
		if (this->vid.fbdev->initLayer(0, width, height, pixelformat, backbuffer)) {
			// set values
			this->layers[layer_id].width = width;
			this->layers[layer_id].height = height;
			this->layers[layer_id].pixelformat = pixelformat;

			// save the buffers
			memcpy(this->layers[layer_id].buffers, this->vid.fbdev->layers[0].buffers, sizeof(this->vid.fbdev->layers[0].buffers));

			// clear layer
			MMSFBColor color(0x00, 0x00, 0x00, 0xff);
			switch (pixelformat) {
			case MMSFB_PF_YUY2:
				mmsfb_fillrectangle_yuy2(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
										 0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);
				break;
			case MMSFB_PF_RGB32:
				mmsfb_fillrectangle_rgb32(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
										  0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);
				break;
			default:
				break;
			}

			// layer is initialized
			this->layers[layer_id].isinitialized = true;

			printf("MMSFBDevOmap: Video Layer %d initialized with %dx%dx%d, pixelformat %s\n",
						layer_id, width, height, backbuffer+1, getMMSFBPixelFormatString(pixelformat).c_str());

			return true;
		}
		return false;

	case 2:
		// OSD layer (OSD1)
		if (!this->osd1.fbdev) {
			printf("MMSFBDevOmap: OSD Layer %d not initialized\n", layer_id);
			return false;
		}

		if   ((pixelformat != MMSFB_PF_ARGB)
			&&(pixelformat != MMSFB_PF_RGB32)
			&&(pixelformat != MMSFB_PF_RGB16)) {
			printf("MMSFBDevOmap: OSD Layer %d needs pixelformat ARGB, RGB32 or RGB16, but %s given\n",
						layer_id, getMMSFBPixelFormatString(pixelformat).c_str());
			return false;
		}

		if (backbuffer) {
			printf("MMSFBDevOmap: OSD Layer %d does not support backbuffer handling\n", layer_id);
			return false;
		}

		// enable OSD1
		if (this->osd1.fbdev->initLayer(0, width, height, pixelformat, backbuffer)) {
			// set values
			this->layers[layer_id].width = width;
			this->layers[layer_id].height = height;
			this->layers[layer_id].pixelformat = pixelformat;

			// save the buffers
			memcpy(this->layers[layer_id].buffers, this->osd1.fbdev->layers[0].buffers, sizeof(this->osd1.fbdev->layers[0].buffers));

			// clear layer
			MMSFBColor color(0x00, 0x00, 0x00, 0xff);
			switch (pixelformat) {
			case MMSFB_PF_ARGB:
				mmsfb_fillrectangle_argb(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
										 0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);
				break;
			case MMSFB_PF_RGB32:
				mmsfb_fillrectangle_rgb32(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
										  0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);
				break;
			case MMSFB_PF_RGB16:
				mmsfb_fillrectangle_rgb16(&(this->layers[layer_id].buffers[0]), this->layers[layer_id].height,
										  0, 0, this->layers[layer_id].width, this->layers[layer_id].height, color);
				break;
			default:
				break;
			}

			// layer is initialized
			this->layers[layer_id].isinitialized = true;

			printf("MMSFBDevOmap: OSD Layer %d initialized with %dx%dx%d, pixelformat %s\n",
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

bool MMSFBDevOmap::releaseLayer(int layer_id) {
	// is initialized?
	INITCHECK;

	switch (layer_id) {
	case 0:
		// default fbdev primary layer 0 on primary screen 0
		printf("MMSFBDevOmap: layer %d cannot be released\n", layer_id);
		return false;
	case 1:
		// Video layer (VID)
		if (this->vid.fbdev) {
			// disable
			this->vid.fbdev->initLayer(0, 0, 0, MMSFB_PF_NONE, 0);
			// close
			this->vid.fbdev->closeDevice();
			return true;
		}
		printf("MMSFBDevOmap: Video Layer %d not initialized\n", layer_id);
		return false;
	case 2:
		// OSD layer
		printf("MMSFBDevOmap: layer %d cannot be released\n", layer_id);
		return false;
	default:
    	printf("MMSFBDevOmap: layer %d is not supported\n", layer_id);
    	break;
	}

	return false;
}

bool MMSFBDevOmap::restoreLayer(int layer_id) {
	// is initialized?
	INITCHECK;

	switch (layer_id) {
	case 0:
		// default fbdev primary layer 0 on primary screen 0
		printf("MMSFBDevOmap: layer %d cannot be restored\n", layer_id);
	    return false;
	case 1:
		// Video layer (VID)
		if (this->vid.fbdev) {
			if (this->vid.fbdev->openDevice(this->vid.device, -2)) {
				if (!this->vid.width)
					return this->vid.fbdev->initLayer(0, 0, 0, MMSFB_PF_NONE, 0);
				else
				if (this->vid.width > 0)
					return this->vid.fbdev->initLayer(0, this->vid.width, this->vid.height,
													  this->vid.pixelformat, this->vid.backbuffer);
				return true;
			}
			return false;
		}
		printf("MMSFBDevOmap: Video Layer %d not initialized\n", layer_id);
		return false;
	case 2:
		// OSD layer (OSD1)
		printf("MMSFBDevOmap: layer %d cannot be restored\n", layer_id);
	    return false;
	default:
    	printf("MMSFBDevOmap: layer %d is not supported\n", layer_id);
    	break;
	}

	return false;
}

bool MMSFBDevOmap::vtGetFd(int *fd) {
	if ((this->primary)&&(this->primary->fbdev)) {
		if (this->primary->fbdev->vt.fd != -1) {
			*fd = this->primary->fbdev->vt.fd;
			return true;
		}
	}
	return false;
}

#endif
