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
#include "mmsgui/fb/mmskms.h"
#include "mmsgui/fb/mmsfbconv.h"
#include "mmsgui/fb/fb.h"

#define INITCHECK  if(!this->isinitialized){MMSFB_SetError(0,"MMSKms is not initialized");return false;}

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static void page_flip_handler(int fd, unsigned int frame, unsigned int sec, unsigned int usec, void *data)
{
	int *waiting_for_flip = (int*)data;
	*waiting_for_flip = 0;
}

MMSKms::MMSKms() {
	// init fb vals
	this->isinitialized = false;
	this->framebuffer_base = NULL;
	this->drm.fd = -1;
	memset(this->layers, 0, sizeof(this->layers));
	this->layers_cnt = 0;
	this->active_screen = 0;

	if (MMSFBBase_rotate180) {
		this->rotate180 = true;
		MMSFBBase_rotate180 = false;
	} else {
		this->rotate180 = false;
	}

}

MMSKms::~MMSKms() {
	closeDevice();
}


void MMSKms::dump_blob(uint32_t blob_id)
{
        uint32_t i;
        unsigned char *blob_data;
        drmModePropertyBlobPtr blob;

        blob = drmModeGetPropertyBlob(this->drm.fd, blob_id);
        if (!blob)
                return;

        blob_data = (unsigned char *)blob->data;

        for (i = 0; i < blob->length; i++) {
                if (i % 16 == 0)
                        printf("\n\t\t\t");
                printf("%.2hhx", blob_data[i]);
        }
        printf("\n");

        drmModeFreePropertyBlob(blob);
}


void MMSKms::dump_prop(uint32_t prop_id, uint64_t value)
{
        int i;
        drmModePropertyPtr prop;

        prop = drmModeGetProperty(this->drm.fd, prop_id);

        printf("\t%d", prop_id);
        if (!prop) {
                printf("\n");
                return;
        }

        printf(" %s:\n", prop->name);

        printf("\t\tflags:");
        if (prop->flags & DRM_MODE_PROP_PENDING)
                printf(" pending");
        if (prop->flags & DRM_MODE_PROP_RANGE)
                printf(" range");
        if (prop->flags & DRM_MODE_PROP_IMMUTABLE)
                printf(" immutable");
        if (prop->flags & DRM_MODE_PROP_ENUM)
                printf(" enum");
        if (prop->flags & DRM_MODE_PROP_BITMASK)
                printf(" bitmask");
        if (prop->flags & DRM_MODE_PROP_BLOB)
                printf(" blob");
        printf("\n");

        if (prop->flags & DRM_MODE_PROP_RANGE) {
                printf("\t\tvalues:");
                for (i = 0; i < prop->count_values; i++)
                        printf(" %llu", prop->values[i]);
                printf("\n");
        }

        if (prop->flags & DRM_MODE_PROP_ENUM) {
                printf("\t\tenums:");
                for (i = 0; i < prop->count_enums; i++)
                        printf(" %s=%llu", prop->enums[i].name,
                               prop->enums[i].value);
                printf("\n");
        } else if (prop->flags & DRM_MODE_PROP_BITMASK) {
                printf("\t\tvalues:");
                for (i = 0; i < prop->count_enums; i++)
                        printf(" %s=0x%llx", prop->enums[i].name,
                               (1LL << prop->enums[i].value));
                printf("\n");
        } else {
        }

        if (prop->flags & DRM_MODE_PROP_BLOB) {
                printf("\t\tblobs:\n");
                for (i = 0; i < prop->count_blobs; i++)
                        dump_blob(prop->blob_ids[i]);
                printf("\n");
        } else {
        }

        printf("\t\tvalue:");
        if (prop->flags & DRM_MODE_PROP_BLOB)
                dump_blob(value);
        else
                printf(" %llu\n", value);

        drmModeFreeProperty(prop);
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
//		printf("curr mode h: %d, v: %d\n", current_mode->hdisplay, current_mode->vdisplay);
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

	if (this->rotate180) {
		drmModeObjectSetProperty(this->drm.fd, encoder->crtc_id, DRM_MODE_OBJECT_CRTC, 8, 4);
	}

//	drmModeObjectPropertiesPtr tmp = drmModeObjectGetProperties(this->drm.fd, this->drm.crtc_id, DRM_MODE_OBJECT_CRTC);
//
//	if (tmp)
//		printf("properties count: %d\n", tmp->count_props);
//
//	printf("  props:\n");
//    int j = 0;
//	if (tmp) {
//			for (j = 0; j < tmp->count_props; j++)
//					dump_prop(tmp->props[j],
//							tmp->prop_values[j]);
//			drmModeFreeObjectProperties(tmp);
//	} else {
//			printf("\tcould not get mode properties: %s\n",
//				   strerror(errno));
//	}

//    drmModeObjectPropertiesPtr props;
    drmModePlaneRes *plane_resources;
    drmModePlane *ovr;

	plane_resources = drmModeGetPlaneResources(this->drm.fd);
	if (!plane_resources) {
			fprintf(stderr, "drmModeGetPlaneResources failed: %s\n",
					strerror(errno));
			return false;
	}

//	printf("Planes:\n");
//	printf("id\tcrtc\tfb\tCRTC x,y\tx,y\tgamma size\n");
	for (i = 0; i < plane_resources->count_planes; i++) {
			ovr = drmModeGetPlane(this->drm.fd, plane_resources->planes[i]);
			if (!ovr) {
					fprintf(stderr, "drmModeGetPlane failed: %s\n",
							strerror(errno));
					continue;
			}

//			printf("%d\t%d\t%d\t%d,%d\t\t%d,%d\t%d\n",
//				   ovr->plane_id, ovr->crtc_id, ovr->fb_id,
//				   ovr->crtc_x, ovr->crtc_y, ovr->x, ovr->y,
//				   ovr->gamma_size);
//
//			if (!ovr->count_formats)
//					continue;
//
//			printf("  formats:");
//			for (j = 0; j < ovr->count_formats; j++)
//					printf(" %4.4s", (char *)&ovr->formats[j]);
//			printf("\n");
//
//			printf("  props:\n");
//			props = drmModeObjectGetProperties(this->drm.fd, ovr->plane_id,
//											   DRM_MODE_OBJECT_PLANE);
//			if (props) {
//					for (j = 0; j < props->count_props; j++)
//							dump_prop(props->props[j],
//									  props->prop_values[j]);
//					drmModeFreeObjectProperties(props);
//			} else {
//					printf("\tcould not get plane properties: %s\n",
//						   strerror(errno));
//			}

            if (ovr->possible_crtcs & this->drm.crtc_id){
                    this->drm.plane_id = ovr->plane_id;
                    break;
            }

//			drmModeFreePlane(ovr);
	}
//	printf("\n");


	return true;
}

bool MMSKms::init_gbm()
{
	this->drm.dev = gbm_create_device(this->drm.fd);

	if (!init_omap()) {
		printf("failed to initialize OMAP\n");
		return false;
	}

//	this->bo = gbm_bo_import(this->drm.dev, GBM_BO_IMPORT_WL_BUFFER, this->framebuffer_base, GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);

	//	this->drm.surface = (struct gbm_surface*) calloc(1, sizeof *this->drm.surface);
	//
	//	this->drm.surface->gbm_device = gbm_bo_get_device(this->bo);
	//	this->drm.surface->width = 	gbm_bo_get_width(this->bo);
	//	this->drm.surface->height = gbm_bo_get_height(this->bo);
	//	this->drm.surface->format = gbm_bo_get_format(this->bo);
	//	this->drm.surface->flags = GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING;

	if (outputtype == MMSFB_OT_OGL) {
		this->drm.surface = gbm_surface_create(this->drm.dev, this->drm.mode->hdisplay, this->drm.mode->vdisplay,
							GBM_FORMAT_ARGB8888, GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);

		if (!this->drm.surface) {
			printf("failed to create gbm surface\n");
	        drmClose(this->drm.fd);
	        this->drm.fd = -1;
			return false;
		}

		this->bo = gbm_surface_lock_front_buffer(this->drm.surface);
		this->fb = (DRM_FB*)drm_fb_get_from_bo(this->bo);
		gbm_surface_release_buffer(this->drm.surface, this->bo);
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

	ret = drmModeAddFB(this->drm.fd, width, height, 24, 32, stride, handle, &fb->fb_id);
	if (ret) {
		printf("failed to create fb: %s\n", strerror(errno));
		free(fb);
		return NULL;
	}

	ret =0;
	ret = drmModeSetCrtc(this->drm.fd, this->drm.crtc_id, fb->fb_id, 0, 0,
						&this->drm.connector_id, 1, this->drm.mode);

	if (ret) {
		printf("failed to set mode: %s\n", strerror(errno));
	}

	if (this->rotate180) {
		drmModeObjectSetProperty(this->drm.fd, this->drm.plane_id, DRM_MODE_OBJECT_PLANE, 8, 4);
	}

	if ((this->drm.width != this->drm.mode->hdisplay) || (this->drm.height != this->drm.mode->vdisplay)) {
		ret = drmModeSetPlane(this->drm.fd, this->drm.plane_id, this->drm.crtc_id,
				fb->fb_id, 0, 0, 0, this->drm.mode->hdisplay, this->drm.mode->vdisplay,
				0, (this->drm.mode->vdisplay-this->drm.height) << 16, this->drm.width << 16, this->drm.height << 16);

		if (ret)
			printf("drmModeSetPlane failed\n");
	}

	gbm_bo_set_user_data(bo, fb, drm_fb_destroy_callback);

	return fb;
}

bool MMSKms::init_omap()
{
	this->drm.o_dev = omap_device_new(this->drm.fd);

	if (!this->drm.dev) {
		printf("ERROR:couldn't create omap device\n");
		return false;
	}

	uint32_t bo_flags = OMAP_BO_TILED_32 | OMAP_BO_SCANOUT | OMAP_BO_WC;

	if (bo_flags & OMAP_BO_TILED) {
		this->o_bo = omap_bo_new_tiled(this->drm.o_dev, this->drm.mode->hdisplay, this->drm.mode->vdisplay, bo_flags);
	} else {
		this->o_bo = omap_bo_new(this->drm.o_dev, this->drm.mode->hdisplay * this->drm.mode->vdisplay * 32 / 8, bo_flags);
	}

	if (!this->o_bo) {
		printf("failed to create omap bo\n");
        drmClose(this->drm.fd);
        this->drm.fd = -1;
		return false;
	}


	uint32_t handle;
	int ret;

	this->fb = (DRM_FB*)calloc(1, sizeof *this->fb);

	unsigned int alignedWidth;
	alignedWidth = (this->drm.mode->hdisplay + (32 - 1)) & ~(32 - 1);
	this->stride = ((alignedWidth * 32) + 7) / 8;
	if (bo_flags & OMAP_BO_TILED)
		this->stride = (stride + (4096 - 1)) & ~(4096 - 1);

	handle = omap_bo_handle(this->o_bo);

	ret = drmModeAddFB(this->drm.fd, this->drm.mode->hdisplay, this->drm.mode->vdisplay, 24, 32, this->stride, handle, &fb->fb_id);
	if (ret) {
		printf("failed to create fb: %s\n", strerror(errno));
		free(this->fb);
		return false;
	}

//	omap_bo_cpu_prep(this->o_bo, OMAP_GEM_WRITE);
	this->framebuffer_base = omap_bo_map(this->o_bo);
//	omap_bo_cpu_fini(this->o_bo, OMAP_GEM_WRITE);

	return true;
}

bool MMSKms::openDevice(MMSFBOutputType outputtype) {
	// close the device if opened
	this->outputtype = outputtype;
	closeDevice();

	if (!init_drm()) {
		printf("failed to initialize DRM\n");
		return false;
	}

	if (!init_gbm()) {
		printf("failed to initialize GBM\n");
		return false;
	}

	memset(&this->evctx, 0, sizeof(this->evctx));
	this->evctx.version = DRM_EVENT_CONTEXT_VERSION;
	this->evctx.page_flip_handler = page_flip_handler;

    // all initialized :)
	this->isinitialized = true;

    return true;
}

void MMSKms::closeDevice() {

	if (this->drm.fd != -1) {
        drmClose(this->drm.fd);
        this->drm.fd = -1;
	}

	// reset all other
	this->isinitialized = false;
	memset(this->layers, 0, sizeof(this->layers));
	this->layers_cnt = 0;
	this->active_screen = 0;
}

bool MMSKms::isInitialized() {
	return this->isinitialized;
}

bool MMSKms::panDisplay(int buffer_id, void *framebuffer_base) {
	// is initialized?
	INITCHECK;

	// check framebuffer_base pointer
	if (framebuffer_base) {
		if (framebuffer_base != this->framebuffer_base) {
			printf("MMSFBDev: framebuffer base pointer not correct\n");
			return false;
		}
	}

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	FD_SET(this->drm.fd, &fds);

	int waiting_for_flip = 1;
	int ret = 0;

	printf("MMSKMS: page flip (pan display)\n");

	ret = drmModePageFlip(this->drm.fd, this->drm.crtc_id, this->fb->fb_id,	DRM_MODE_PAGE_FLIP_EVENT, &waiting_for_flip);
	if (ret) {
		printf("OPENGL_SWAP: failed to queue page flip\n");
		return false;
	}

	while (waiting_for_flip) {
		ret = select(this->drm.fd + 1, &fds, NULL, NULL, NULL);
		if (ret < 0) {
			printf("OPENGL_SWAP: select err\n");
			return false;
		} else if (ret == 0) {
			printf("OPENGL_SWAP: select timeout!\n");
			return false;
		} else if (FD_ISSET(0, &fds)) {
			printf("OPENGL_SWAP: user interrupted!\n");
			break;
		}
		drmHandleEvent(this->drm.fd, &this->evctx);
	}

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

//	printf("init layer w: %d, h: %d\n", width, height);

	this->drm.width = width;
	this->drm.height = height;

	// default fbdev does support only primary layer 0 on primary screen 0
	if (layer_id != 0) {
    	printf("MMSKms: layer %d is not supported\n", layer_id);
		return false;
	}

	int ret =0;

	/* set mode: */
	ret = 0;
	ret = drmModeSetCrtc(this->drm.fd, this->drm.crtc_id, this->fb->fb_id, 0, 0,
						&this->drm.connector_id, 1, this->drm.mode);

	if (ret) {
		printf("failed to set mode: %s\n", strerror(errno));
		return false;
	}

	if (this->rotate180) {
		drmModeObjectSetProperty(this->drm.fd, this->drm.plane_id, DRM_MODE_OBJECT_PLANE, 8, 4);
	}

	if ((this->drm.width != this->drm.mode->hdisplay) || (this->drm.height != this->drm.mode->vdisplay)) {
		ret = drmModeSetPlane(this->drm.fd, this->drm.plane_id, this->drm.crtc_id,
				this->fb->fb_id, 0, 0, 0, this->drm.mode->hdisplay, this->drm.mode->vdisplay,
				0, 0, this->drm.width << 16, this->drm.height << 16);

		if (ret)
			printf("drmModeSetPlane failed\n");
	}

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

	this->layers[layer_id].buffers[0].ptr  = this->framebuffer_base;
	this->layers[layer_id].buffers[0].pitch= this->stride;
	this->layers[layer_id].buffers[0].hwbuffer = true;

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

DRM *MMSKms::getDrm() {
	return &(this->drm);
}

void MMSKms::pageFlip() {

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	FD_SET(this->drm.fd, &fds);

	DRM_FB *my_fb;
	struct gbm_bo *next_bo;
	int waiting_for_flip = 1;

	next_bo = gbm_surface_lock_front_buffer(this->drm.surface);
	my_fb = (DRM_FB*)drm_fb_get_from_bo(next_bo);
//	my_fb = (DRM_FB*)drm_fb_get_from_bo(this->bo);
//	my_fb = this->fb;

	int ret =0;

//	ret = drmModeSetCrtc(this->drm.fd, this->drm.crtc_id, my_fb->fb_id, 0, 0,
//						&this->drm.connector_id, 1, this->drm.mode);
//
//	if (ret) {
//		printf("failed to set mode: %s\n", strerror(errno));
//		return;
//	}
//
//	drmModeObjectSetProperty(this->drm.fd, this->drm.plane_id, DRM_MODE_OBJECT_PLANE, 8, 4);
//
//	if ((this->drm.width != this->drm.mode->hdisplay) || (this->drm.height != this->drm.mode->vdisplay)) {
//		ret = drmModeSetPlane(this->drm.fd, this->drm.plane_id, this->drm.crtc_id,
//				my_fb->fb_id, 0, 0, 0, this->drm.mode->hdisplay, this->drm.mode->vdisplay,
//				0, (this->drm.mode->vdisplay-this->drm.height) << 16, this->drm.width << 16, this->drm.height << 16);
//
//		if (ret)
//			printf("drmModeSetPlane failed\n");
//	}

	ret = 0;
	ret = drmModePageFlip(this->drm.fd, this->drm.crtc_id, my_fb->fb_id,	DRM_MODE_PAGE_FLIP_EVENT, &waiting_for_flip);
	if (ret) {
		printf("OPENGL_SWAP: failed to queue page flip\n");
		return;
	}

	while (waiting_for_flip) {
		ret = select(this->drm.fd + 1, &fds, NULL, NULL, NULL);
		if (ret < 0) {
			printf("OPENGL_SWAP: select err\n");
			return;
		} else if (ret == 0) {
			printf("OPENGL_SWAP: select timeout!\n");
			return;
		} else if (FD_ISSET(0, &fds)) {
			printf("OPENGL_SWAP: user interrupted!\n");
			break;
		}
		drmHandleEvent(this->drm.fd, &this->evctx);
	}

	gbm_surface_release_buffer(this->drm.surface, next_bo);
//	bo = next_bo;
}

#endif
