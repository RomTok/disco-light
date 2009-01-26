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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/kd.h>
#include <linux/vt.h>

#include "mmsgui/fb/mmsfbdev.h"

#define INITCHECK  if(!this->isinitialized){MMSFB_SetError(0,"MMSFBDev is not initialized");return false;}

MMSFBDev::MMSFBDev() {
	// init fb vals
	this->isinitialized = false;
	this->fd = -1;
	this->framebuffer_base = NULL;
	this->reset_console_accel = false;
	memset(this->modes, 0, sizeof(this->modes));
	this->modes_cnt = 0;
	memset(this->layers, 0, sizeof(this->layers));
	this->layers_cnt = 0;

	// init terminal vals
	this->vt.fd0 = -1;
	this->vt.fd = -1;
	this->vt.number = -1;
	this->vt.previous = -1;
	this->vt.org_fb = -1;
}

MMSFBDev::~MMSFBDev() {
	closeDevice();
}

void MMSFBDev::printFixScreenInfo() {
	char id[17];
	id[16]=0;
    printf("MMSFBDev: fix screen info ------------\n");
    memcpy(id, fix_screeninfo.id, 16);
    printf("    id             = %s\n", id);
    printf("    smem_start     = 0x%x\n", (unsigned int)fix_screeninfo.smem_start);
    printf("    smem_len       = %d\n", fix_screeninfo.smem_len);
    printf("    type           = %d\n", fix_screeninfo.type);
    printf("    type_aux       = %d\n", fix_screeninfo.type_aux);
    printf("    visual         = %d\n", fix_screeninfo.visual);
    printf("    xpanstep       = %d\n", fix_screeninfo.xpanstep);
    printf("    ypanstep       = %d\n", fix_screeninfo.ypanstep);
    printf("    ywrapstep      = %d\n", fix_screeninfo.ywrapstep);
    printf("    line_length    = %d\n", fix_screeninfo.line_length);
    printf("    mmio_start     = 0x%x\n", (unsigned int)fix_screeninfo.mmio_start);
    printf("    mmio_len       = %d\n", fix_screeninfo.mmio_len);
    printf("    accel          = %d\n", fix_screeninfo.accel);
    printf("    reserved[3]    = %d, %d, %d\n", fix_screeninfo.reserved[0], fix_screeninfo.reserved[1], fix_screeninfo.reserved[2]);
}

void MMSFBDev::printVarScreenInfo() {
    printf("MMSFBDev: var screen info ------------\n");
    printf("    xres           = %d\n", var_screeninfo.xres);
    printf("    yres           = %d\n", var_screeninfo.yres);
    printf("    xres_virtual   = %d\n", var_screeninfo.xres_virtual);
    printf("    yres_virtual   = %d\n", var_screeninfo.yres_virtual);
    printf("    xoffset        = %d\n", var_screeninfo.xoffset);
    printf("    yoffset        = %d\n", var_screeninfo.yoffset);
    printf("    bits_per_pixel = %d\n", var_screeninfo.bits_per_pixel);
    printf("    grayscale      = %d\n", var_screeninfo.grayscale);
    printf("    red            = %d(offs=%d)\n", var_screeninfo.red.length, var_screeninfo.red.offset);
    printf("    green          = %d(offs=%d)\n", var_screeninfo.green.length, var_screeninfo.green.offset);
    printf("    blue           = %d(offs=%d)\n", var_screeninfo.blue.length, var_screeninfo.blue.offset);
    printf("    transp         = %d(offs=%d)\n", var_screeninfo.transp.length, var_screeninfo.transp.offset);
    printf("    nonstd         = %d\n", var_screeninfo.nonstd);
    printf("    activate       = %d\n", var_screeninfo.activate);
    printf("    height         = %d\n", var_screeninfo.height);
    printf("    width          = %d\n", var_screeninfo.width);
    printf("    accel_flags    = %d\n", var_screeninfo.accel_flags);

    printf("    pixclock       = %d\n", var_screeninfo.pixclock);
    printf("    left_margin    = %d\n", var_screeninfo.left_margin);
    printf("    right_margin   = %d\n", var_screeninfo.right_margin);
    printf("    upper_margin   = %d\n", var_screeninfo.upper_margin);
    printf("    lower_margin   = %d\n", var_screeninfo.lower_margin);
    printf("    hsync_len      = %d\n", var_screeninfo.hsync_len);
    printf("    vsync_len      = %d\n", var_screeninfo.vsync_len);
    printf("    sync           = %d\n", var_screeninfo.sync);
    printf("    vmode          = %d\n", var_screeninfo.vmode);
    printf("    rotate         = %d\n", var_screeninfo.rotate);
    printf("    accel_flags    = %d\n", var_screeninfo.accel_flags);
    printf("    reserved[5]    = %d, %d, %d, %d, %d\n", var_screeninfo.reserved[0], var_screeninfo.reserved[1], var_screeninfo.reserved[2], var_screeninfo.reserved[3], var_screeninfo.reserved[4]);
}

bool MMSFBDev::buildPixelFormat() {
	this->layers[0].pixelformat = MMSFB_PF_NONE;
    switch (var_screeninfo.transp.length) {
    case 0:
    	// pixelformat with no alphachannel
    	if    ((var_screeninfo.red.length == 5)  && (var_screeninfo.green.length == 6) && (var_screeninfo.blue.length == 5)
    		&& (var_screeninfo.red.offset == 11) && (var_screeninfo.green.offset == 5) && (var_screeninfo.blue.offset == 0)) {
    		this->layers[0].pixelformat = MMSFB_PF_RGB16;
    	}
    	else
    	if    ((var_screeninfo.red.length == 8)  && (var_screeninfo.green.length == 8) && (var_screeninfo.blue.length == 8)
    		&& (var_screeninfo.red.offset == 16) && (var_screeninfo.green.offset == 8) && (var_screeninfo.blue.offset == 0)) {
    		if (var_screeninfo.bits_per_pixel == 24)
    			this->layers[0].pixelformat = MMSFB_PF_RGB24;
    		else
    			this->layers[0].pixelformat = MMSFB_PF_RGB32;
    	}
    	break;
    case 8:
    	// pixelformat with 8 bit alphachannel
    	if    ((var_screeninfo.red.length == 8)  && (var_screeninfo.green.length == 8) && (var_screeninfo.blue.length == 8)
    		&& (var_screeninfo.red.offset == 16) && (var_screeninfo.green.offset == 8) && (var_screeninfo.blue.offset == 0)) {
    		this->layers[0].pixelformat = MMSFB_PF_ARGB;
    	}
    	break;
    }

    if (this->layers[0].pixelformat != MMSFB_PF_NONE) {
   		printf("MMSFBDev: current pixelformat is %s\n", getMMSFBPixelFormatString(this->layers[0].pixelformat).c_str());
    	return true;
    }

	return false;
}

bool MMSFBDev::readModes() {
	// reset mode list
	this->modes_cnt = 0;

	// open fb.modes
	FILE *fp;
	if (!(fp = fopen("/etc/fb.modes","r")))
		return false;

	// through all lines
	char line[128];
	while (fgets(line, sizeof(line)-1, fp)) {
		char label[32];
		if (sscanf(line, "mode \"%31[^\"]\"", label) == 1) {
			// mode found
			bool geom_set = false;
			bool timings_set = false;
			struct fb_var_screeninfo *mode = &this->modes[this->modes_cnt];
			memset(mode, 0, sizeof(struct fb_var_screeninfo));

			// parse for settings
			while (fgets(line, sizeof(line)-1, fp) && !(strstr(line, "endmode"))) {
				char value[16];
				int dummy;
				if (sscanf(line, " geometry %d %d %d %d %d",
								&mode->xres, &mode->yres, &dummy, &dummy, &mode->bits_per_pixel) == 5)
					geom_set = true;
				else
				if (sscanf(line, " timings %d %d %d %d %d %d %d",
								&mode->pixclock, &mode->left_margin,  &mode->right_margin,
								&mode->upper_margin, &mode->lower_margin, &mode->hsync_len, &mode->vsync_len) == 7)
					timings_set = true;
				else
				if ((sscanf(line, " hsync %15s", value) == 1) && !strcasecmp(value, "high"))
					mode->sync |= FB_SYNC_HOR_HIGH_ACT;
				else
				if ((sscanf(line, " vsync %15s", value) == 1) && !strcasecmp(value, "high"))
					mode->sync |= FB_SYNC_VERT_HIGH_ACT;
				else
				if ((sscanf(line, " csync %15s", value) == 1) && !strcasecmp(value, "high"))
					mode->sync |= FB_SYNC_COMP_HIGH_ACT;
				else
				if ((sscanf(line, " gsync %15s", value) == 1) && !strcasecmp(value, "true"))
					mode->sync |= FB_SYNC_ON_GREEN;
				else
				if ((sscanf(line, " extsync %15s", value) == 1) && !strcasecmp(value, "true"))
					mode->sync |= FB_SYNC_EXT;
				else
				if ((sscanf(line, " bcast %15s", value) == 1) && !strcasecmp(value, "true"))
					mode->sync |= FB_SYNC_BROADCAST;
				else
				if ((sscanf(line, " laced %15s", value) == 1) && !strcasecmp(value, "true"))
					mode->vmode |= FB_VMODE_INTERLACED;
				else
				if ((sscanf(line, " double %15s", value) == 1) && !strcasecmp(value, "true"))
					mode->vmode |= FB_VMODE_DOUBLE;
			}

			if (geom_set &&	timings_set) {
				// add mode to list
				this->modes_cnt++;

			    printf("MMSFBDev: mode %s (%dx%d, %d bits) loaded from /etc/fb.modes\n", label, mode->xres, mode->yres, mode->bits_per_pixel);
			}
			else {
				// ignore mode
			    printf("MMSFBDev: ignore mode %s (%dx%d, %d bits) from /etc/fb.modes\n", label, mode->xres, mode->yres, mode->bits_per_pixel);
			}
		}
	}

	fclose (fp);
	return true;
}


bool MMSFBDev::openDevice(char *device_file, int console) {
	// close the device if opened
	closeDevice();

    if (device_file) {
        // open given device
        this->fd = open(device_file, O_RDWR);
        if (this->fd < 0) {
        	printf("MMSFBDev: opening device %s failed\n", device_file);
            return false;
        }
        this->device_file = device_file;
    }
    else {
        // open standard device
        this->fd = open("/dev/fb0", O_RDWR);
        if (this->fd < 0) {
            this->fd = open("/dev/fb/0", O_RDWR);
            if (this->fd < 0) {
            	printf("MMSFBDev: opening device /dev/fb0 and /dev/fb/0 failed\n");
                return false;
            }
            this->device_file = "/dev/fb/0";
        }
        else
            this->device_file = "/dev/fb0";
    }

    // file descriptor should be closed when an exec function is invoked
    fcntl(this->fd, F_SETFD, FD_CLOEXEC);

    // build device abbreviation
    memset(this->device, 0, sizeof(this->device));
    sprintf(this->device, "fb0");
    if (this->device_file.substr(0, 8) == "/dev/fb/")
        sprintf(this->device, "fb%s", this->device_file.substr(8, 5).c_str());
    else
    if (this->device_file.substr(0, 7) == "/dev/fb")
        sprintf(this->device, "fb%s", this->device_file.substr(7, 5).c_str());

    // read video modes
    readModes();
	printf("MMSFBDev: %d modes loaded from /etc/fb.modes\n", this->modes_cnt);

    // initialize virtual terminal
    if (!vtOpen(console)) {
    	closeDevice();
        return false;
    }

    // get fix screen infos
    if (ioctl(this->fd, FBIOGET_FSCREENINFO, &this->fix_screeninfo) < 0) {
    	printf("MMSFBDev: could not get fix screen infos\n");
    	closeDevice();
        return false;
    }
    printFixScreenInfo();

    // map framebuffer to memory
    if ((this->framebuffer_base=mmap(NULL, this->fix_screeninfo.smem_len,
                                     PROT_READ | PROT_WRITE, MAP_SHARED, this->fd, 0)) == MAP_FAILED) {
    	printf("MMSFBDev: could not mmap framebuffer memory\n");
    	closeDevice();
        return false;
    }

    // get variable screen infos
    if (ioctl(this->fd, FBIOGET_VSCREENINFO, &this->org_var_screeninfo) < 0) {
    	printf("MMSFBDev: could not get var screen infos\n");
    	closeDevice();
        return false;
    }

    // disable console acceleration
    this->var_screeninfo = this->org_var_screeninfo;
    this->var_screeninfo.accel_flags = 0;
    if (ioctl(this->fd, FBIOPUT_VSCREENINFO, &this->var_screeninfo) < 0) {
    	printf("MMSFBDev: could not disable console acceleration\n");
    	closeDevice();
        return false;
    }
    printVarScreenInfo();

	// build the preset pixelformat
	buildPixelFormat();

    // all initialized :)
	this->isinitialized = true;

    return true;
}

void MMSFBDev::closeDevice() {
	// reset virtual terminal
	vtClose();

	// free resources if allocated
	if (this->reset_console_accel) {
	    // reset console acceleration
	    ioctl(this->fd, FBIOPUT_VSCREENINFO, &this->org_var_screeninfo);
		this->reset_console_accel = false;
	}

	if (this->framebuffer_base) {
		munmap(this->framebuffer_base, this->fix_screeninfo.smem_len);
		this->framebuffer_base = NULL;
	}

	if (this->fd != -1) {
        close(this->fd);
        this->fd = -1;
	}

	// reset all other
	this->isinitialized = false;
	memset(this->modes, 0, sizeof(this->modes));
	this->modes_cnt = 0;
	memset(this->layers, 0, sizeof(this->layers));
	this->layers_cnt = 0;
}

bool MMSFBDev::isInitialized() {
	return this->isinitialized;
}

void MMSFBDev::waitForVSync() {
	if (this->fd != -1) {
		static const int z = 0;
		if (ioctl(this->fd, FBIO_WAITFORVSYNC, &z)) {
			// failed, ...
		}
	}
}

bool MMSFBDev::testLayer(int layer_id) {
	// is initialized?
	INITCHECK;

	// default fbdev does support only primary layer 0 on primary screen 0
	if (layer_id != 0) {
    	printf("MMSFBDev: layer %d is not supported\n", layer_id);
		return false;
	}

	return true;
}

bool MMSFBDev::initLayer(int layer_id, int width, int height, MMSFBSurfacePixelFormat pixelformat) {
	// is initialized?
	INITCHECK;

	// default fbdev does support only primary layer 0 on primary screen 0
	if (layer_id != 0) {
    	printf("MMSFBDev: layer %d is not supported\n", layer_id);
		return false;
	}

	//switch video mode
	if (!setMode(width, height, pixelformat))
		return false;

	// save dimension of the layer
	this->layers[layer_id].pitch = this->fix_screeninfo.line_length;
	this->layers[layer_id].width = this->var_screeninfo.xres;
	this->layers[layer_id].height = this->var_screeninfo.yres;

	// layer is initialized
	this->layers[layer_id].isinitialized = true;

	return true;
}


bool MMSFBDev::getPixelFormat(int layer_id, MMSFBSurfacePixelFormat *pf) {
	// is initialized?
	INITCHECK;

	// is layer initialized?
	if (!this->layers[layer_id].isinitialized)
		return false;

	// return pixelformat
	*pf = this->layers[layer_id].pixelformat;
	return true;
}

bool MMSFBDev::getPhysicalMemory(unsigned long *mem) {
	// is initialized?
	INITCHECK;
    *mem = this->fix_screeninfo.smem_start;
	return true;
}

bool MMSFBDev::getFrameBufferBase(unsigned char **base) {
	// is initialized?
	INITCHECK;
	*base = (unsigned char *)this->framebuffer_base;
	return true;
}

bool MMSFBDev::getFrameBufferPtr(int layer_id, void **ptr, int *pitch, int *width, int *height) {
	// is initialized?
	INITCHECK;

	// is layer initialized?
	if (!this->layers[layer_id].isinitialized)
		return false;

	// return buffer infos
	*ptr = (unsigned char *)this->framebuffer_base;
	*pitch = this->layers[layer_id].pitch;
	*width = this->layers[layer_id].width;
	*height = this->layers[layer_id].height;

	return true;
}


bool MMSFBDev::mapMmio(unsigned char **mmio) {
	// is initialized?
	INITCHECK;

    *mmio = (unsigned char *)mmap(NULL, this->fix_screeninfo.mmio_len, PROT_READ | PROT_WRITE, MAP_SHARED,
								  this->fd, this->fix_screeninfo.smem_len);
    if (!*mmio) {
    	printf("MMSFBDev: could not mmap mmio buffer\n");
    	return false;
    }

    long page_size = sysconf(_SC_PAGESIZE);
    unsigned long page_mask = page_size < 0 ? 0 : (page_size - 1);

    *mmio = (*mmio) + (this->fix_screeninfo.mmio_start & page_mask);

	return true;
}

bool MMSFBDev::unmapMmio(unsigned char *mmio) {
	// is initialized?
	INITCHECK;

    long page_size = sysconf(_SC_PAGESIZE);
    unsigned long page_mask = page_size < 0 ? 0 : (page_size - 1);

	munmap((void*)(mmio - (this->fix_screeninfo.mmio_start & page_mask)), this->fix_screeninfo.mmio_len);

	return true;
}

bool MMSFBDev::setMode(int width, int height, MMSFBSurfacePixelFormat pixelformat) {
	bool do_switch = false;

	// is initialized?
	INITCHECK;

	// get bits per pixel and its length/offset
	int red_length, red_offset;
	int green_length, green_offset;
	int blue_length, blue_offset;
	int transp_length, transp_offset;
	int bits_per_pixel = getBitsPerPixel(pixelformat,
										 &red_length, &red_offset,
										 &green_length, &green_offset,
										 &blue_length, &blue_offset,
										 &transp_length, &transp_offset);

	// check if mode is already set
	if    ((width == (int)this->var_screeninfo.xres) && (height == (int)this->var_screeninfo.yres)
		&& (bits_per_pixel == (int)this->var_screeninfo.bits_per_pixel) && (this->layers[0].pixelformat == pixelformat)) {
		// mode already set, no switch required
		printf("MMSFBDev: using preset mode %dx%d, pixelformat %s (%d bits)\n",
				width, height, getMMSFBPixelFormatString(pixelformat).c_str(), bits_per_pixel);
		return true;
	}

	// searching for mode
    for (int cnt = 0; cnt < this->modes_cnt; cnt++) {
    	struct fb_var_screeninfo *mode = &this->modes[cnt];
    	if ((width == (int)mode->xres) && (height == (int)mode->yres) && (bits_per_pixel == (int)mode->bits_per_pixel)) {
    		// mode matches, switch to it
    	    this->var_screeninfo = *mode;

    	    this->var_screeninfo.activate = FB_ACTIVATE_NOW;
    	    this->var_screeninfo.accel_flags = 0;

    	    this->var_screeninfo.red.length = red_length;
			this->var_screeninfo.red.offset = red_offset;
			this->var_screeninfo.green.length = green_length;
			this->var_screeninfo.green.offset = green_offset;
			this->var_screeninfo.blue.length = blue_length;
			this->var_screeninfo.blue.offset = blue_offset;
			this->var_screeninfo.transp.length = transp_length;
			this->var_screeninfo.transp.offset = transp_offset;

			this->var_screeninfo.xres_virtual = this->var_screeninfo.xres;
			this->var_screeninfo.yres_virtual = this->var_screeninfo.yres;
			this->var_screeninfo.xoffset = 0;
			this->var_screeninfo.yoffset = 0;
			this->var_screeninfo.grayscale = 0;

			do_switch = true;
			break;
    	}
	}

    if (!do_switch) {
        // no mode found
    	printf("MMSFBDev: no mode %dx%d, bit depth %d (%s) found in /etc/fb.modes\n",
    			width, height, bits_per_pixel, getMMSFBPixelFormatString(pixelformat).c_str());

    	// searching for mode with any pixelformat
        for (int cnt = 0; cnt < this->modes_cnt; cnt++) {
        	struct fb_var_screeninfo *mode = &this->modes[cnt];
        	if ((width == (int)mode->xres) && (height == (int)mode->yres)) {
        		// mode matches, switch to it
        	    this->var_screeninfo = *mode;

				printf("MMSFBDev: trying to use first mode %dx%d, bit depth %d from /etc/fb.modes\n", width, height, this->var_screeninfo.bits_per_pixel);

                this->var_screeninfo.activate = FB_ACTIVATE_NOW;
        	    this->var_screeninfo.accel_flags = 0;

        	    this->var_screeninfo.bits_per_pixel = bits_per_pixel;
        	    this->var_screeninfo.red.length = red_length;
    			this->var_screeninfo.red.offset = red_offset;
    			this->var_screeninfo.green.length = green_length;
    			this->var_screeninfo.green.offset = green_offset;
    			this->var_screeninfo.blue.length = blue_length;
    			this->var_screeninfo.blue.offset = blue_offset;
    			this->var_screeninfo.transp.length = transp_length;
    			this->var_screeninfo.transp.offset = transp_offset;

    			this->var_screeninfo.xres_virtual = this->var_screeninfo.xres;
    			this->var_screeninfo.yres_virtual = this->var_screeninfo.yres;
    			this->var_screeninfo.xoffset = 0;
    			this->var_screeninfo.yoffset = 0;
    			this->var_screeninfo.grayscale = 0;

    			do_switch = true;
    			break;
        	}
    	}
    }

    if (!do_switch) {
        // no mode found
    	printf("MMSFBDev: no mode %dx%d with any bit depth found in /etc/fb.modes\n", width, height);

        // check if resolution has changed
    	if  ((width == (int)this->var_screeninfo.xres) && (height == (int)this->var_screeninfo.yres)) {
			// resolution has not changed, so try to change only the pixelformat
			printf("MMSFBDev: resolution is the same, so try to change the pixelformat to %s\n",
					getMMSFBPixelFormatString(pixelformat).c_str());

			this->var_screeninfo.activate = FB_ACTIVATE_NOW;
			this->var_screeninfo.accel_flags = 0;

			this->var_screeninfo.bits_per_pixel = bits_per_pixel;
			this->var_screeninfo.red.length = red_length;
			this->var_screeninfo.red.offset = red_offset;
			this->var_screeninfo.green.length = green_length;
			this->var_screeninfo.green.offset = green_offset;
			this->var_screeninfo.blue.length = blue_length;
			this->var_screeninfo.blue.offset = blue_offset;
			this->var_screeninfo.transp.length = transp_length;
			this->var_screeninfo.transp.offset = transp_offset;

			do_switch = true;
		}
    }

	if (do_switch) {
		// switch now
	    if (ioctl(this->fd, FBIOPUT_VSCREENINFO, &this->var_screeninfo) < 0) {
	    	printf("MMSFBDev: could not switch to mode %dx%d, pixelformat %s (%d bits)\n",
	    			width, height, getMMSFBPixelFormatString(pixelformat).c_str(), bits_per_pixel);
	        return false;
	    }

	    // get fix screen infos
	    if (ioctl(this->fd, FBIOGET_FSCREENINFO, &this->fix_screeninfo) < 0) {
	    	printf("MMSFBDev: could not get fix screen infos\n");
	        return false;
	    }
	    printFixScreenInfo();

	    // get variable screen infos
	    if (ioctl(this->fd, FBIOGET_VSCREENINFO, &this->var_screeninfo) < 0) {
	    	printf("MMSFBDev: could not get var screen infos\n");
	        return false;
	    }
	    printVarScreenInfo();

    	if ((width == (int)this->var_screeninfo.xres) && (height == (int)this->var_screeninfo.yres) && (bits_per_pixel == (int)this->var_screeninfo.bits_per_pixel)) {
			printf("MMSFBDev: mode successfully switched to %dx%d, pixelformat %s (%d bits)\n",
					width, height, getMMSFBPixelFormatString(pixelformat).c_str(), bits_per_pixel);
    	}
    	else {
			printf("MMSFBDev: mode switch to %dx%d, pixelformat %s (%d bits) failed\n",
					width, height, getMMSFBPixelFormatString(pixelformat).c_str(), bits_per_pixel);
			return false;
    	}

		// build the pixelformat
		if (!buildPixelFormat()) {
		    printf("MMSFBDev: unsupported pixelformat: r=%d(offs=%d), g=%d(offs=%d), b=%d(offs=%d), a=%d(offs=%d) (%d bit)\n",
					this->var_screeninfo.red.length,    this->var_screeninfo.red.offset,
					this->var_screeninfo.green.length,  this->var_screeninfo.green.offset,
					this->var_screeninfo.blue.length,   this->var_screeninfo.blue.offset,
					this->var_screeninfo.transp.length, this->var_screeninfo.transp.offset,
					this->var_screeninfo.bits_per_pixel);
			return false;
		}

		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////

bool MMSFBDev::vtOpen(int console) {
	// close the vt device if opened
	vtClose();

	// run a program in a new session
    setsid();

    // open the tty0 device
    this->vt.fd0 = open("/dev/tty0", O_RDONLY | O_NOCTTY);
    if (this->vt.fd0 < 0) {
		if (errno == ENOENT) {
			this->vt.fd0 = open("/dev/vc/0", O_RDONLY | O_NOCTTY);
			if (this->vt.fd0 < 0) {
				printf("MMSFBDev: opening device /dev/tty0 and /dev/vc/0 failed\n");
				return false;
			}
		}
		else {
			printf("MMSFBDev: opening device /dev/tty0 failed\n");
			return false;
		}
    }

    // get the current vt
    struct vt_stat vs;
    if (ioctl(this->vt.fd0, VT_GETSTATE, &vs) < 0) {
		printf("MMSFBDev: could not get vt state\n");
		vtClose();
		return false;
    }
    this->vt.previous = vs.v_active;
	printf("MMSFBDev: started from virtual terminal #%d\n", this->vt.previous);

	if (console >= 0) {
		this->vt.number = console;
	}
	else {
		// query the vt number which can be used
		int n = ioctl(this->vt.fd0, VT_OPENQRY, &this->vt.number);
		if ((n < 0) || (this->vt.number == -1)) {
			printf("MMSFBDev: query vt number failed\n");
			vtClose();
			return false;
		}
	}
	printf("MMSFBDev: using virtual terminal #%d\n", this->vt.number);

	// save original fb
    struct fb_con2fbmap c2f;
    c2f.console = this->vt.number;
    if (ioctl(this->fd, FBIOGET_CON2FBMAP, &c2f)) {
		printf("MMSFBDev: get original framebuffer failed for vt #%d\n", this->vt.number);
		vtClose();
		return false;
    }
    this->vt.org_fb = c2f.framebuffer;

    // set console for our fb
    struct stat fbs;
    if (fstat(this->fd, &fbs)) {
		printf("MMSFBDev: stat fb device failed\n");
	    this->vt.org_fb = -1;
		vtClose();
		return false;
    }
    c2f.framebuffer = (fbs.st_rdev & 0xFF) >> 5;
    c2f.console = this->vt.number;
    if (ioctl(this->fd, FBIOPUT_CON2FBMAP, &c2f) < 0) {
		printf("MMSFBDev: set console for framebuffer failed\n");
	    this->vt.org_fb = -1;
		vtClose();
		return false;
    }

	// switch to our vt
	while (ioctl(this->vt.fd0, VT_ACTIVATE, this->vt.number) < 0) {
		if (errno == EINTR) continue;
		printf("MMSFBDev: cannot switch (VT_ACTIVATE) to console #%d\n", this->vt.number);
		vtClose();
		return false;
	}
	while (ioctl(this->vt.fd0, VT_WAITACTIVE, this->vt.number) < 0) {
		if (errno == EINTR) continue;
		printf("MMSFBDev: cannot switch (VT_WAITACTIVE) to console #%d\n", this->vt.number);
		vtClose();
		return false;
	}
	usleep(50*1000);

    // open my tty device
	char tty[16];
    sprintf(tty, "/dev/tty%d", this->vt.number);
    this->vt.fd = open(tty, O_RDWR | O_NOCTTY);
    if (this->vt.fd < 0) {
		if (errno == ENOENT) {
			sprintf(tty, "/dev/vc/%d", this->vt.number);
			this->vt.fd = open(tty, O_RDWR | O_NOCTTY);
			if (this->vt.fd < 0) {
				printf("MMSFBDev: opening device /dev/tty%d and /dev/vc/%d failed\n", this->vt.number, this->vt.number);
				vtClose();
				return false;
			}
		}
		else {
			printf("MMSFBDev: opening device /dev/tty%d failed\n", this->vt.number);
			vtClose();
			return false;
		}
    }

    // attach to the device
    ioctl(this->vt.fd, TIOCSCTTY, 0);

    // switch cursor off
    const char cursor_off[] = "\033[?1;0;0c";
    write(this->vt.fd, cursor_off, sizeof(cursor_off));

    // put terminal into graphics mode
	ioctl(this->vt.fd, KDSETMODE, KD_GRAPHICS);

    // init keyboard
    ioctl(this->vt.fd, KDSKBMODE, K_MEDIUMRAW);
    ioctl(this->vt.fd, KDSKBLED, 0);
    tcgetattr(this->vt.fd, &this->saved_ts);
    struct termios ts;
    ts = this->saved_ts;
    ts.c_cc[VTIME] = 0;
    ts.c_cc[VMIN] = 1;
    ts.c_lflag &= ~(ICANON|ECHO|ISIG);
    ts.c_iflag = 0;
    tcsetattr(this->vt.fd, TCSAFLUSH, &ts);
    tcsetpgrp(this->vt.fd, getpgrp());

    return true;
}

void MMSFBDev::vtClose() {
	if (this->vt.fd != -1) {
		// close tty
		tcsetattr(this->vt.fd, TCSAFLUSH, &this->saved_ts);
		ioctl(this->vt.fd, KDSKBMODE, K_XLATE);
		ioctl(this->vt.fd, KDSETMODE, KD_TEXT);
	    const char cursor_on[] = "\033[?0;0;0c";
	    write(this->vt.fd, cursor_on, sizeof(cursor_on));
        close(this->vt.fd);
        this->vt.fd = -1;
	}

	if (this->vt.org_fb != -1) {
		// reset the fb for used console
	    struct fb_con2fbmap c2f;
	    c2f.framebuffer = this->vt.org_fb;
	    c2f.console = this->vt.number;
	    ioctl(this->fd, FBIOPUT_CON2FBMAP, &c2f);
		this->vt.org_fb = -1;
	}

	if (this->vt.previous != -1) {
		// switch back to previous vt
		ioctl(this->vt.fd0, VT_ACTIVATE, this->vt.previous);
		ioctl(this->vt.fd0, VT_WAITACTIVE, this->vt.previous);
		usleep(50*1000);
        ioctl(this->vt.fd0, VT_DISALLOCATE, this->vt.number);
		this->vt.number = -1;
		this->vt.previous = -1;
	}

	if (this->vt.fd0 != -1) {
		// close tty
        close(this->vt.fd0);
        this->vt.fd0 = -1;
	}
}

bool MMSFBDev::vtGetFd(int *fd) {
	if (this->vt.fd != -1) {
		*fd = this->vt.fd;
		return true;
	}
	return false;
}

#endif