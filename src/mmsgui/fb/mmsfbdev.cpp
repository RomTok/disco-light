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
	this->pixelformat = MMSFB_PF_NONE;

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
	this->pixelformat = MMSFB_PF_NONE;
    switch (var_screeninfo.transp.length) {
    case 0:
    	// pixelformat with no alphachannel
    	if    ((var_screeninfo.red.length == 5)  && (var_screeninfo.green.length == 6) && (var_screeninfo.blue.length == 5)
    		&& (var_screeninfo.red.offset == 11) && (var_screeninfo.green.offset == 5) && (var_screeninfo.blue.offset == 0)) {
    		this->pixelformat = MMSFB_PF_RGB16;
    	}
    	else
    	if    ((var_screeninfo.red.length == 8)  && (var_screeninfo.green.length == 8) && (var_screeninfo.blue.length == 8)
    		&& (var_screeninfo.red.offset == 16) && (var_screeninfo.green.offset == 8) && (var_screeninfo.blue.offset == 0)) {
    		if (var_screeninfo.bits_per_pixel == 24)
    			this->pixelformat = MMSFB_PF_RGB24;
    		else
    			this->pixelformat = MMSFB_PF_RGB32;
    	}
    	break;
    case 8:
    	// pixelformat with 8 bit alphachannel
    	if    ((var_screeninfo.red.length == 8)  && (var_screeninfo.green.length == 8) && (var_screeninfo.blue.length == 8)
    		&& (var_screeninfo.red.offset == 16) && (var_screeninfo.green.offset == 8) && (var_screeninfo.blue.offset == 0)) {
    		this->pixelformat = MMSFB_PF_ARGB;
    	}
    	break;
    }

    if (this->pixelformat != MMSFB_PF_NONE) {
    	printf("MMSFBDev: detected pixelformat is %s\n", getMMSFBPixelFormatString(this->pixelformat).c_str());
    	return true;
    }

    printf("MMSFBDev: unsupported pixelformat: r=%d(offs=%d), g=%d(offs=%d), b=%d(offs=%d), a=%d(offs=%d) (%d bit)\n",
			var_screeninfo.red.length,    var_screeninfo.red.offset,
			var_screeninfo.green.length,  var_screeninfo.green.offset,
			var_screeninfo.blue.length,   var_screeninfo.blue.offset,
			var_screeninfo.transp.length, var_screeninfo.transp.offset,
			var_screeninfo.bits_per_pixel);

	return false;
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

    // build the pixelformat
    if (!buildPixelFormat()) {
    	closeDevice();
        return false;
    }

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
	this->pixelformat = MMSFB_PF_NONE;
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

bool MMSFBDev::getPixelFormat(MMSFBSurfacePixelFormat *pf) {
	// is initialized?
	INITCHECK;
	*pf = this->pixelformat;
	return true;
}

bool MMSFBDev::getFrameBufferBase(unsigned char **base) {
	// is initialized?
	INITCHECK;
	*base = (unsigned char *)this->framebuffer_base;
	return true;
}

bool MMSFBDev::getFrameBufferPtr(unsigned int number, void **ptr, int *pitch, int *width, int *height) {
	// is initialized?
	INITCHECK;
	if (number == 0) {
		// first buffer
		*ptr = (unsigned char *)this->framebuffer_base;
		*pitch = this->fix_screeninfo.line_length;
		*width = this->var_screeninfo.xres;
		*height = this->var_screeninfo.yres;
	}
	else {
		//TODO ...
	}
	return true;
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

