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

#ifndef MMSKMS_H_
#define MMSKMS_H_

#ifdef __HAVE_KMS__

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
extern "C" {
#include <omap_drmif.h>
}

#define MMSKMS_MAX_LAYERS	2

typedef struct {
	struct gbm_device *dev;
	struct gbm_surface *surface;
} GBM;

typedef struct {
	int fd;
	drmModeModeInfo *mode;
	unsigned int crtc_id;
	unsigned int connector_id;
	unsigned int plane_id;

	int width;
	int height;

	struct gbm_device *dev;
	struct gbm_surface *surface;
	struct omap_device *o_dev;

} DRM;

typedef struct {
	struct gbm_bo *bo;
//	struct omap_bo *bo;
	unsigned int fb_id;
} DRM_FB;

#include "mmsgui/fb/mmsfbbase.h"

class MMSKms {
    private:
    	//! is initialized?
    	bool	isinitialized;

        //! virtual framebuffer address
        void    *framebuffer_base;
        uint32_t stride;

        typedef struct {
        	//! is initialized?
        	bool	isinitialized;
        	//! width of the layer
        	int width;
        	//! height of the layer
        	int height;
        	//! describes the surface buffers
        	MMSFBSurfacePlanesBuffer buffers;
			//! pixelformat of the layer
			MMSFBSurfacePixelFormat	pixelformat;
        } MMSKMS_LAYER;

        //! layer infos
        MMSKMS_LAYER layers[MMSKMS_MAX_LAYERS];

        //! number of layers
        int layers_cnt;

        //! id of the active screen (this is for fbs != vesa)
        int active_screen;

        MMSFBOutputType outputtype;

        struct gbm_bo *bo;
        struct omap_bo *o_bo;

    	DRM 	drm;
    	DRM_FB *fb;

    	drmEventContext evctx;

    	bool rotate180;

		bool init_drm();
		bool init_gbm();
		DRM_FB* drm_fb_get_from_bo(struct gbm_bo *bo);
		bool init_omap();

		void dump_prop(uint32_t prop_id, uint64_t value);
		void dump_blob(uint32_t blob_id);

        void genFBPixelFormat(MMSFBSurfacePixelFormat pf, unsigned int *nonstd_format, MMSFBPixelDef *pixeldef);

    public:
        MMSKms();
        virtual ~MMSKms();

        virtual bool openDevice(MMSFBOutputType outputtype);
        virtual void closeDevice();
        bool isInitialized();

        virtual bool panDisplay(int buffer_id, void *framebuffer_base = NULL);

        virtual bool testLayer(int layer_id);
        virtual bool initLayer(int layer_id, int width, int height, MMSFBSurfacePixelFormat pixelformat, int backbuffer = 0);

        virtual bool releaseLayer(int layer_id);
        virtual bool restoreLayer(int layer_id);

        bool getPixelFormat(int layer_id, MMSFBSurfacePixelFormat *pf);
        bool getFrameBufferPtr(int layer_id, MMSFBSurfacePlanesBuffer buffers, int *width, int *height);

        DRM *getDrm();
        void pageFlip();

        sigc::signal<bool, MMSFBSurfacePixelFormat, unsigned int*, MMSFBPixelDef*>::accumulated<neg_bool_accumulator> onGenFBPixelFormat;

    private:

        friend class MMSInputLISHandler;

};

#endif

#endif /* MMSKMS_H_ */
