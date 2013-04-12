/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2013 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <jens.schneider@diskohq.org>,                   *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#ifdef __HAVE_KMS__

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
//#include <linux/vt.h>
#include "mmsgui/fb/mmskms.h"

#define INITCHECK  if(!this->isinitialized){MMSFB_SetError(0,"MMSKms is not initialized");return false;}

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

MMSKms::MMSKms() {
	// init fb vals
	this->isinitialized = false;
	this->drm.fd = -1;
//	this->framebuffer_base = NULL;
//	this->reset_console_accel = false;
//	memset(this->modes, 0, sizeof(this->modes));
//	this->modes_cnt = 0;
	memset(this->layers, 0, sizeof(this->layers));
	this->layers_cnt = 0;
	this->active_screen = 0;

}

MMSKms::~MMSKms() {
	closeDevice();
}


bool MMSKms::init_drm()
{
	static const char *modules[] = { "i915", "radeon", "nouveau", "vmwgfx", "omapdrm", "exynos"	};
	drmModeRes *resources;
	drmModeConnector *connector = NULL;
	drmModeEncoder *encoder = NULL;
	int i, area;

	for (i = 0; i < ARRAY_SIZE(modules); i++) {
		printf("trying to load module %s...", modules[i]);
		this->drm.fd = drmOpen(modules[i], NULL);

		if (this->drm.fd < 0) {
			printf("failed.\n");
		} else {
			printf("success.\n");
			break;
		}
	}

	if (this->drm.fd < 0) {
		printf("could not open drm device\n");
		return false;
	}

	resources = drmModeGetResources(this->drm.fd);
	if (!resources) {
		printf("drmModeGetResources failed: %s\n", strerror(errno));
        drmClose(this->drm.fd);
        this->drm.fd = -1;
		return false;
	}

	/* find a connected connector: */
	for (i = 0; i < resources->count_connectors; i++) {
		connector = drmModeGetConnector(this->drm.fd, resources->connectors[i]);

		if (connector->connection == DRM_MODE_CONNECTED) {
			/* it's connected, let's use this! */
			break;
		}

		drmModeFreeConnector(connector);
		connector = NULL;
	}

	if (!connector) {
		/* we could be fancy and listen for hotplug events and wait for
		* a connector..
		*/
		printf("no connected connector!\n");
        drmClose(this->drm.fd);
        this->drm.fd = -1;
		return false;
	}

	/* find highest resolution mode: */
	for (i = 0, area = 0; i < connector->count_modes; i++) {
		drmModeModeInfo *current_mode = &connector->modes[i];
		int current_area = current_mode->hdisplay * current_mode->vdisplay;

		if (current_area > area) {
			this->drm.mode = current_mode;
			area = current_area;
		}
	}

	if (!this->drm.mode) {
		printf("could not find mode!\n");
        drmClose(this->drm.fd);
        this->drm.fd = -1;
		return false;
	}

	/* find encoder: */
	for (i = 0; i < resources->count_encoders; i++) {
		encoder = drmModeGetEncoder(this->drm.fd, resources->encoders[i]);

		if (encoder->encoder_id == connector->encoder_id)
			break;

		drmModeFreeEncoder(encoder);
		encoder = NULL;
	}

	if (!encoder) {
		printf("no encoder!\n");
        drmClose(this->drm.fd);
        this->drm.fd = -1;
		return false;
	}

	this->drm.crtc_id = encoder->crtc_id;
	this->drm.connector_id = connector->connector_id;

	return true;
}

bool MMSKms::init_gbm()
{
	this->gbm.dev = gbm_create_device(this->drm.fd);

	this->gbm.surface = gbm_surface_create(this->gbm.dev, this->drm.mode->hdisplay, this->drm.mode->vdisplay,
							GBM_FORMAT_XRGB8888, GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);

	if (!gbm.surface) {
		printf("failed to create gbm surface\n");
        drmClose(this->drm.fd);
        this->drm.fd = -1;
		return false;
	}

	return true;
}

static void
drm_fb_destroy_callback(struct gbm_bo *bo, void *data)
{
	DRM_FB *fb = (DRM_FB*)data;
	struct gbm_device *gbm = gbm_bo_get_device(bo);
	if(gbm) {
		int fd = gbm_device_get_fd(gbm);
		if (fb->fb_id)
			drmModeRmFB(fd, fb->fb_id);
	}

	free(fb);
}

DRM_FB* MMSKms::drm_fb_get_from_bo(struct gbm_bo *bo)
{
	DRM_FB *fb = (DRM_FB*)gbm_bo_get_user_data(bo);
	uint32_t width, height, stride, handle;
	int ret;

	if (fb)
		return fb;

	fb = (DRM_FB*)calloc(1, sizeof *fb);
	fb->bo = bo;

	width = gbm_bo_get_width(bo);
	height = gbm_bo_get_height(bo);
	stride = gbm_bo_get_stride(bo);
	handle = gbm_bo_get_handle(bo).u32;

	ret = drmModeAddFB(drm.fd, width, height, 24, 32, stride, handle, &fb->fb_id);
	if (ret) {
		printf("failed to create fb: %s\n", strerror(errno));
		free(fb);
		return NULL;
	}

	gbm_bo_set_user_data(bo, fb, drm_fb_destroy_callback);

	return fb;
}

bool MMSKms::openDevice() {
	// close the device if opened
	closeDevice();

//	fd_set fds;

	if (!init_drm()) {
		printf("failed to initialize DRM\n");
		return false;
	}

//	FD_ZERO(&fds);
//	FD_SET(0, &fds);
//	FD_SET(this->drm.fd, &fds);

	if (!init_gbm()) {
		printf("failed to initialize GBM\n");
		return false;
	}

	this->bo = gbm_surface_lock_front_buffer(this->gbm.surface);
	this->fb = drm_fb_get_from_bo(this->bo);

//
//    if (device_file) {
//        // open given device
//    	this->fd = open(device_file, O_RDWR);
//        if (this->fd < 0) {
//        	printf("MMSKms: opening device %s failed\n", device_file);
//            return false;
//        }
//        this->device_file = device_file;
//    }
//    else {
//        // open standard device
//        this->fd = open("/dev/fb0", O_RDWR);
//        if (this->fd < 0) {
//            this->fd = open("/dev/fb/0", O_RDWR);
//            if (this->fd < 0) {
//            	printf("MMSKms: opening device /dev/fb0 and /dev/fb/0 failed\n");
//                return false;
//            }
//            this->device_file = "/dev/fb/0";
//        }
//        else
//            this->device_file = "/dev/fb0";
//    }
//
//    // file descriptor should be closed when an exec function is invoked
//    fcntl(this->fd, F_SETFD, FD_CLOEXEC);
//
//    // build device abbreviation
//    memset(this->device, 0, sizeof(this->device));
//    sprintf(this->device, "fb0");
//    if (this->device_file.substr(0, 8) == "/dev/fb/")
//        sprintf(this->device, "fb%s", this->device_file.substr(8, 5).c_str());
//    else
//    if (this->device_file.substr(0, 7) == "/dev/fb")
//        sprintf(this->device, "fb%s", this->device_file.substr(7, 5).c_str());
//
//    // read video modes
//    readModes();
//	printf("MMSKms: %d modes loaded from /etc/fb.modes\n", this->modes_cnt);
//
//    // initialize virtual terminal
//	if (console >= -1)
//		if (!vtOpen(console)) {
//			closeDevice();
//			return false;
//		}
//
//    // get fix screen infos
//    if (ioctl(this->fd, FBIOGET_FSCREENINFO, &this->fix_screeninfo) < 0) {
//    	printf("MMSKms: could not get fix screen infos from %s\n", this->device_file.c_str());
//    	closeDevice();
//        return false;
//    }
//    printFixScreenInfo();
//
//    // map framebuffer to memory (try shared, if it doesn't work, i.e. on non-mmu based machines, try private)
//    if ((this->framebuffer_base=mmap(NULL, this->fix_screeninfo.smem_len,
//                                     PROT_READ | PROT_WRITE, MAP_SHARED, this->fd, 0)) == MAP_FAILED) {
//        if ((this->framebuffer_base=mmap(NULL, this->fix_screeninfo.smem_len,
//                                         PROT_READ | PROT_WRITE, MAP_PRIVATE, this->fd, 0)) == MAP_FAILED) {
//			printf("MMSKms: could not mmap framebuffer memory for %s\n", this->device_file.c_str());
//			this->framebuffer_base = NULL;
//			closeDevice();
//			return false;
//        }
//    }
//
//    // get variable screen infos
//    if (ioctl(this->fd, FBIOGET_VSCREENINFO, &this->org_var_screeninfo) < 0) {
//    	printf("MMSKms: could not get var screen infos from %s\n", this->device_file.c_str());
//    	closeDevice();
//        return false;
//    }
//
//    // disable console acceleration
//    this->var_screeninfo = this->org_var_screeninfo;
//    this->var_screeninfo.accel_flags = 0;
//    if (ioctl(this->fd, FBIOPUT_VSCREENINFO, &this->var_screeninfo) < 0) {
//    	printf("MMSKms: could not disable console acceleration for %s\n", this->device_file.c_str());
//    	closeDevice();
//        return false;
//    }

	// build the preset pixelformat
//	buildPixelFormat();

    // all initialized :)
	this->isinitialized = true;

    return true;
}

void MMSKms::closeDevice() {

//	// free resources if allocated
//	if (this->reset_console_accel) {
//	    // reset console acceleration
//	    ioctl(this->fd, FBIOPUT_VSCREENINFO, &this->org_var_screeninfo);
//		this->reset_console_accel = false;
//	}
//
//	if (this->framebuffer_base) {
//		munmap(this->framebuffer_base, this->fix_screeninfo.smem_len);
//		this->framebuffer_base = NULL;
//	}
//
	if (this->drm.fd != -1) {
        drmClose(this->drm.fd);
        this->drm.fd = -1;
	}

	// reset all other
	this->isinitialized = false;
//	memset(this->modes, 0, sizeof(this->modes));
//	this->modes_cnt = 0;
	memset(this->layers, 0, sizeof(this->layers));
	this->layers_cnt = 0;
	this->active_screen = 0;
}

bool MMSKms::isInitialized() {
	return this->isinitialized;
}

bool MMSKms::waitForVSync() {
	// is initialized?
	INITCHECK;

//	// default fbdev does support only primary screen 0
//	if (this->active_screen != 0) {
//    	printf("MMSKms: screen %d is not supported\n", this->active_screen);
//		return false;
//	}
//
//	static const int s = 0;
//	if (ioctl(this->fd, FBIO_WAITFORVSYNC, &s)) {
//		// failed, well then???
//	}

	return true;
}

bool MMSKms::panDisplay(int buffer_id, void *framebuffer_base) {
	// is initialized?
	INITCHECK;

//	// check framebuffer_base pointer
//	if (framebuffer_base) {
//		if (framebuffer_base != this->framebuffer_base) {
//			printf("MMSKms: framebuffer base pointer not correct\n");
//			return false;
//		}
//	}
//
//	// calc new y offset
//	int yoffset = buffer_id * this->var_screeninfo.yres;
//	if ((yoffset < 0) || (yoffset + this->var_screeninfo.yres > this->var_screeninfo.yres_virtual)) {
//		return false;
//	}
//	int xoffset_save = this->var_screeninfo.xoffset;
//	int yoffset_save = this->var_screeninfo.yoffset;
//
//	// set new x/y offsets
//	this->var_screeninfo.xoffset = 0;
//	this->var_screeninfo.yoffset = yoffset;
//	if (this->fix_screeninfo.ypanstep)
//		this->var_screeninfo.vmode &= ~FB_VMODE_YWRAP;
//	else
//		this->var_screeninfo.vmode |= FB_VMODE_YWRAP;
//
//	// switch display
//	this->var_screeninfo.activate = FB_ACTIVATE_VBL;
//	if (ioctl(this->fd, FBIOPAN_DISPLAY, &this->var_screeninfo) < 0) {
//    	printf("MMSKms: display panning not supported\n");
//		this->var_screeninfo.xoffset = xoffset_save;
//		this->var_screeninfo.yoffset = yoffset_save;
//		return false;
//	}

	return true;
}

bool MMSKms::testLayer(int layer_id) {
	// is initialized?
	INITCHECK;

	// default fbdev does support only primary layer 0 on primary screen 0
	if (layer_id != 0) {
    	printf("MMSKms: layer %d is not supported\n", layer_id);
		return false;
	}

	return true;
}

bool MMSKms::initLayer(int layer_id, int width, int height, MMSFBSurfacePixelFormat pixelformat, int backbuffer) {
	// is initialized?
	INITCHECK;

	// default fbdev does support only primary layer 0 on primary screen 0
	if (layer_id != 0) {
    	printf("MMSKms: layer %d is not supported\n", layer_id);
		return false;
	}

	/* set mode: */
	int ret = 0;
	ret = drmModeSetCrtc(this->drm.fd, this->drm.crtc_id, this->fb->fb_id, 0, 0,
						&this->drm.connector_id, 1, this->drm.mode);

	if (ret) {
		printf("failed to set mode: %s\n", strerror(errno));
		return false;
	}

//	//switch video mode
//	if (!setMode(width, height, pixelformat, backbuffer))
//		return false;

	if (width <= 0 || height <= 0) {
		// the layer is disabled now
		this->layers[layer_id].isinitialized = false;
		return true;
	}

	// save dimension of the layer
	this->layers[layer_id].width = width;
	this->layers[layer_id].height = height;
	this->layers[layer_id].pixelformat = pixelformat;

	// save the buffers
	memset(&this->layers[layer_id].buffers, 0, sizeof(this->layers[layer_id].buffers));
//	switch (backbuffer) {
//	case 2:
//		this->layers[layer_id].buffers[2].ptr  = ((char *)this->framebuffer_base)
//												+ 2 * this->fix_screeninfo.line_length * this->var_screeninfo.yres;
//		this->layers[layer_id].buffers[2].pitch= this->fix_screeninfo.line_length;
//		this->layers[layer_id].buffers[2].hwbuffer = true;
//	case 1:
//		this->layers[layer_id].buffers[1].ptr  = ((char *)this->framebuffer_base)
//												 + this->fix_screeninfo.line_length * this->var_screeninfo.yres;
//		this->layers[layer_id].buffers[1].pitch= this->fix_screeninfo.line_length;
//		this->layers[layer_id].buffers[1].hwbuffer = true;
//	case 0:
//		this->layers[layer_id].buffers[0].ptr  = this->framebuffer_base;
//		this->layers[layer_id].buffers[0].pitch= this->fix_screeninfo.line_length;
//		this->layers[layer_id].buffers[0].hwbuffer = true;
//		break;
//	default:
//		return false;
//	}

	// layer is initialized
	this->layers[layer_id].isinitialized = true;

	// this layer is on screen 0 (default)
	this->active_screen = 0;

	return true;
}

bool MMSKms::releaseLayer(int layer_id) {
	printf("MMSKms: layer %d cannot be released\n", layer_id);
	return false;
}

bool MMSKms::restoreLayer(int layer_id) {
	printf("MMSKms: layer %d cannot be restored\n", layer_id);
	return false;
}

bool MMSKms::getPixelFormat(int layer_id, MMSFBSurfacePixelFormat *pf) {
	// is initialized?
	INITCHECK;

	// is layer initialized?
	if (!this->layers[layer_id].isinitialized)
		return false;

	// return pixelformat
	*pf = this->layers[layer_id].pixelformat;
	return true;
}

bool MMSKms::getFrameBufferPtr(int layer_id, MMSFBSurfacePlanesBuffer buffers, int *width, int *height) {
	// is initialized?
	INITCHECK;

	// is layer initialized?
	if (!this->layers[layer_id].isinitialized) {
		return false;
	}

	// return buffer infos
	if (buffers)
		memcpy(buffers, this->layers[layer_id].buffers, sizeof(this->layers[layer_id].buffers));
	*width = this->layers[layer_id].width;
	*height = this->layers[layer_id].height;

	return true;
}


void MMSKms::genFBPixelFormat(MMSFBSurfacePixelFormat pf, unsigned int *nonstd_format, MMSFBPixelDef *pixeldef) {

	// generate std format
	if (nonstd_format) *nonstd_format = 0;
	getBitsPerPixel(pf, pixeldef);

	// try to get a fb specific format
	this->onGenFBPixelFormat.emit(pf, nonstd_format, pixeldef);
}

void MMSKms::disable(int fd, string device_file) {
	// have to disable the framebuffer
	if (!this->onDisable.emit(fd, device_file)) {
		// use FBIOPUT_VSCREENINFO and FBIOBLANK together
//		struct fb_var_screeninfo var_screeninfo;
//		ioctl(fd, FBIOGET_VSCREENINFO, &var_screeninfo);
//		var_screeninfo.activate = FB_ACTIVATE_NOW;
//		var_screeninfo.accel_flags = 0;
//		var_screeninfo.xres = 0;
//		var_screeninfo.yres = 0;
//		var_screeninfo.xres_virtual = 0;
//		var_screeninfo.yres_virtual = 0;
//		var_screeninfo.xoffset = 0;
//		var_screeninfo.yoffset = 0;
//		var_screeninfo.grayscale = 0;
//		ioctl(fd, FBIOPUT_VSCREENINFO, &var_screeninfo);
//		ioctl(fd, FBIOBLANK, 1);
	}
}

bool MMSKms::activate(int fd, string device_file, struct fb_var_screeninfo *var_screeninfo,
						int width, int height, MMSFBSurfacePixelFormat pixelformat, bool switch_mode) {

	// try callback
	if (!this->onActivate.emit(fd, device_file, var_screeninfo, width, height, pixelformat, switch_mode)) {
		// no callback set or callback failed
//		if (switch_mode) {
//			if (ioctl(fd, FBIOPUT_VSCREENINFO, var_screeninfo) < 0) {
//				printf("MMSKms: could not switch to mode %dx%d, pixelformat %s (%d bits, nonstd %d), %s\n",
//						width, height, getMMSFBPixelFormatString(pixelformat).c_str(),
//						var_screeninfo->bits_per_pixel, var_screeninfo->nonstd,
//						device_file.c_str());
//				return false;
//			}
//		}
	}

//    // get fix screen infos
//    if (ioctl(this->fd, FBIOGET_FSCREENINFO, &this->fix_screeninfo) < 0) {
//    	printf("MMSKms: could not get fix screen infos from %s\n", this->device_file.c_str());
//        return false;
//    }
//
//    // get variable screen infos
//    if (ioctl(this->fd, FBIOGET_VSCREENINFO, &this->var_screeninfo) < 0) {
//    	printf("MMSKms: could not get var screen infos from %s\n", this->device_file.c_str());
//        return false;
//    }

    return true;
}

bool MMSKms::setMode(int width, int height, MMSFBSurfacePixelFormat pixelformat, int backbuffer) {
	bool do_switch = false;

	// is initialized?
	INITCHECK;

	if (width <= 0 || height <= 0) {
		// have to disable the framebuffer
		//disable(this->drm.fd, this->device_file);
		return true;
	}

	/* set mode: */
	int ret = 0;
	ret = drmModeSetCrtc(this->drm.fd, this->drm.crtc_id, this->fb->fb_id, 0, 0,
						&this->drm.connector_id, 1, this->drm.mode);

	if (ret) {
		printf("failed to set mode: %s\n", strerror(errno));
		return false;
	}

	return true;

//    // reload fix screen infos before changing it
//    if (ioctl(this->fd, FBIOGET_FSCREENINFO, &this->fix_screeninfo) < 0) {
//    	printf("MMSKms: could not get fix screen infos from %s\n", this->device_file.c_str());
//        return false;
//    }
//    printFixScreenInfo();
//
//    // reload variable screen infos before changing it
//    if (ioctl(this->fd, FBIOGET_VSCREENINFO, &this->var_screeninfo) < 0) {
//    	printf("MMSKms: could not get var screen infos from %s\n", this->device_file.c_str());
//        return false;
//    }
//    printVarScreenInfo();
//
//    if (backbuffer) {
//    	if ((!this->fix_screeninfo.ypanstep)&&(!this->fix_screeninfo.ywrapstep)) {
//        	printf("MMSKms: backbuffer requested, but hardware does not support it for %s\n", this->device_file.c_str());
//            return false;
//    	}
//    }
//
//	// get bits per pixel and its length/offset
//	unsigned int nonstd_format;
//	MMSFBPixelDef pixeldef;
//	genFBPixelFormat(pixelformat, &nonstd_format, &pixeldef);
//
//	// check if mode is already set
//    if (!nonstd_format) {
//    	// do it only if a std format is requested
//		if    ((width == (int)this->var_screeninfo.xres) && (height == (int)this->var_screeninfo.yres)
//			&& (pixeldef.bits == (int)this->var_screeninfo.bits_per_pixel) && (this->layers[0].pixelformat == pixelformat)
//			&& (!backbuffer || (this->var_screeninfo.yres_virtual >= this->var_screeninfo.yres * (backbuffer+1)))) {
//			// mode already set, no switch required
//			printf("MMSKms: using preset mode %dx%d, pixelformat %s (%d bits), %s\n",
//					width, height, getMMSFBPixelFormatString(pixelformat).c_str(), pixeldef.bits,
//					this->device_file.c_str());
//
//			// we have to activate the device (can be disabled), but mode switch is not required
//		    activate(this->fd, this->device_file, &this->var_screeninfo, width, height, pixelformat, false);
//			return true;
//		}
//    }
//
//    if (!do_switch) {
//		// searching for mode
//		for (int cnt = 0; cnt < this->modes_cnt; cnt++) {
//			struct fb_var_screeninfo *mode = &this->modes[cnt];
//			if ((width == (int)mode->xres) && (height == (int)mode->yres) && (pixeldef.bits == (int)mode->bits_per_pixel)) {
//				// mode matches, switch to it
//				this->var_screeninfo = *mode;
//
//				this->var_screeninfo.activate = FB_ACTIVATE_NOW;
//				this->var_screeninfo.accel_flags = 0;
//
//        	    this->var_screeninfo.nonstd = nonstd_format;
//
//				this->var_screeninfo.red.length = pixeldef.red_length;
//				this->var_screeninfo.red.offset = pixeldef.red_offset;
//				this->var_screeninfo.green.length = pixeldef.green_length;
//				this->var_screeninfo.green.offset = pixeldef.green_offset;
//				this->var_screeninfo.blue.length = pixeldef.blue_length;
//				this->var_screeninfo.blue.offset = pixeldef.blue_offset;
//				this->var_screeninfo.transp.length = pixeldef.transp_length;
//				this->var_screeninfo.transp.offset = pixeldef.transp_offset;
//
//				this->var_screeninfo.xres_virtual = this->var_screeninfo.xres;
//				this->var_screeninfo.yres_virtual = this->var_screeninfo.yres * (backbuffer+1);
//				this->var_screeninfo.xoffset = 0;
//				this->var_screeninfo.yoffset = 0;
//				this->var_screeninfo.grayscale = 0;
//
//				do_switch = true;
//				break;
//			}
//		}
//    }
//
//    if (!do_switch) {
//        // no mode found
//    	printf("MMSKms: no mode %dx%d, bit depth %d (%s) found in /etc/fb.modes\n",
//    			width, height, pixeldef.bits, getMMSFBPixelFormatString(pixelformat).c_str());
//
//    	// searching for mode with any pixelformat
//        for (int cnt = 0; cnt < this->modes_cnt; cnt++) {
//        	struct fb_var_screeninfo *mode = &this->modes[cnt];
//        	if ((width == (int)mode->xres) && (height == (int)mode->yres)) {
//        		// mode matches, switch to it
//        	    this->var_screeninfo = *mode;
//
//				printf("MMSKms: trying to use first mode %dx%d, bit depth %d from /etc/fb.modes\n", width, height, this->var_screeninfo.bits_per_pixel);
//
//                this->var_screeninfo.activate = FB_ACTIVATE_NOW;
//        	    this->var_screeninfo.accel_flags = 0;
//
//        	    this->var_screeninfo.nonstd = nonstd_format;
//
//        	    this->var_screeninfo.bits_per_pixel = pixeldef.bits;
//        	    this->var_screeninfo.red.length = pixeldef.red_length;
//    			this->var_screeninfo.red.offset = pixeldef.red_offset;
//    			this->var_screeninfo.green.length = pixeldef.green_length;
//    			this->var_screeninfo.green.offset = pixeldef.green_offset;
//    			this->var_screeninfo.blue.length = pixeldef.blue_length;
//    			this->var_screeninfo.blue.offset = pixeldef.blue_offset;
//    			this->var_screeninfo.transp.length = pixeldef.transp_length;
//    			this->var_screeninfo.transp.offset = pixeldef.transp_offset;
//
//    			this->var_screeninfo.xres_virtual = this->var_screeninfo.xres;
//    			this->var_screeninfo.yres_virtual = this->var_screeninfo.yres * (backbuffer+1);
//    			this->var_screeninfo.xoffset = 0;
//    			this->var_screeninfo.yoffset = 0;
//    			this->var_screeninfo.grayscale = 0;
//
//    			do_switch = true;
//    			break;
//        	}
//    	}
//    }
//
//    if (!do_switch) {
//        // no mode found
//    	printf("MMSKms: no mode %dx%d with any bit depth found in /etc/fb.modes\n", width, height);
//
//        // check if resolution has changed
//    	if  ((width == (int)this->var_screeninfo.xres) && (height == (int)this->var_screeninfo.yres)) {
//			// resolution has not changed, so try to change only the pixelformat
//			printf("MMSKms: resolution is the same, so try to change the pixelformat to %s, %s\n",
//					getMMSFBPixelFormatString(pixelformat).c_str(),
//					this->device_file.c_str());
//
//			this->var_screeninfo.activate = FB_ACTIVATE_NOW;
//			this->var_screeninfo.accel_flags = 0;
//
//    	    this->var_screeninfo.nonstd = nonstd_format;
//
//    	    this->var_screeninfo.bits_per_pixel = pixeldef.bits;
//			this->var_screeninfo.red.length = pixeldef.red_length;
//			this->var_screeninfo.red.offset = pixeldef.red_offset;
//			this->var_screeninfo.green.length = pixeldef.green_length;
//			this->var_screeninfo.green.offset = pixeldef.green_offset;
//			this->var_screeninfo.blue.length = pixeldef.blue_length;
//			this->var_screeninfo.blue.offset = pixeldef.blue_offset;
//			this->var_screeninfo.transp.length = pixeldef.transp_length;
//			this->var_screeninfo.transp.offset = pixeldef.transp_offset;
//
//			if (backbuffer) {
//    			this->var_screeninfo.xres_virtual = this->var_screeninfo.xres;
//				this->var_screeninfo.yres_virtual = this->var_screeninfo.yres * (backbuffer+1);
//    			this->var_screeninfo.xoffset = 0;
//    			this->var_screeninfo.yoffset = 0;
//			}
//
//			do_switch = true;
//		}
//    	else
//		if  (this->layers[0].pixelformat == pixelformat) {
//			// pixelformat has not changed, so try to change only the resolution
//			printf("MMSKms: pixelformat is the same, so try to change the resolution to %dx%d, %s\n",
//					width, height,
//					this->device_file.c_str());
//
//			this->var_screeninfo.activate = FB_ACTIVATE_NOW;
//			this->var_screeninfo.accel_flags = 0;
//
//			this->var_screeninfo.xres = width;
//			this->var_screeninfo.yres = height;
//
//			this->var_screeninfo.xres_virtual = this->var_screeninfo.xres;
//			this->var_screeninfo.yres_virtual = this->var_screeninfo.yres * (backbuffer+1);
//			this->var_screeninfo.xoffset = 0;
//			this->var_screeninfo.yoffset = 0;
//			this->var_screeninfo.grayscale = 0;
//
//			do_switch = true;
//		}
//    }
//
//	if (do_switch) {
//		// switch now
//	    if (!activate(this->fd, this->device_file, &this->var_screeninfo, width, height, pixelformat)) {
//	        return false;
//	    }
//
//	    // check the result of activation
//		if    ((width == (int)this->var_screeninfo.xres) && (height == (int)this->var_screeninfo.yres)
//			&& (pixeldef.bits == (int)this->var_screeninfo.bits_per_pixel)) {
//
//			printf("MMSKms: mode successfully switched to %dx%d, pixelformat %s (%d bits), %s\n",
//					width, height, getMMSFBPixelFormatString(pixelformat).c_str(), pixeldef.bits,
//					this->device_file.c_str());
//
//			if (backbuffer) {
//				if (this->var_screeninfo.yres_virtual < this->var_screeninfo.yres * (backbuffer+1)) {
//					printf("MMSKms: buffer size %dx%d is to small (%dx%d requested), %s\n",
//							this->var_screeninfo.xres_virtual, this->var_screeninfo.yres_virtual,
//							this->var_screeninfo.xres, this->var_screeninfo.yres * (backbuffer+1),
//							this->device_file.c_str());
//					return false;
//				}
//			}
//		}
//		else {
//			printf("MMSKms: mode switch to %dx%d, pixelformat %s (%d bits) failed, %s\n",
//					width, height, getMMSFBPixelFormatString(pixelformat).c_str(), pixeldef.bits,
//					this->device_file.c_str());
//			return false;
//		}
//
//		// build the pixelformat
//		if (!buildPixelFormat()) {
//		    printf("MMSKms: unsupported pixelformat: r=%d(offs=%d), g=%d(offs=%d), b=%d(offs=%d), a=%d(offs=%d) (%d bit)\n",
//					this->var_screeninfo.red.length,    this->var_screeninfo.red.offset,
//					this->var_screeninfo.green.length,  this->var_screeninfo.green.offset,
//					this->var_screeninfo.blue.length,   this->var_screeninfo.blue.offset,
//					this->var_screeninfo.transp.length, this->var_screeninfo.transp.offset,
//					this->var_screeninfo.bits_per_pixel);
//			return false;
//		}
//
//		if (this->layers[0].pixelformat != pixelformat) {
//			printf("MMSKms: pixelformat not correctly set, %s set, %s requested\n",
//					getMMSFBPixelFormatString(this->layers[0].pixelformat).c_str(),
//					getMMSFBPixelFormatString(pixelformat).c_str());
//			return false;
//		}
//
//		return true;
//	}

	return false;
}


#endif
