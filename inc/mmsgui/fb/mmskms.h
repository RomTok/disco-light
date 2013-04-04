/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2012 BerLinux Solutions GmbH                       *
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

#include "mmsgui/fb/mmsfbconv.h"
#include "mmsgui/fb/fb.h"
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>

//#include "esUtil.h"

////! openDevice() console parameter, use free virtual console
//#define MMSFBDEV_QUERY_CONSOLE	-1
//
////! openDevice() console parameter, do not open a virtual console
//#define MMSFBDEV_NO_CONSOLE		-2
//
//// added missing ioctl
//#ifndef FBIO_WAITFORVSYNC
//#define FBIO_WAITFORVSYNC	_IOW('F', 0x20, u_int32_t)
//#endif

#define MMSKMS_MAX_MODES	128
#define MMSKMS_MAX_LAYERS	32

typedef struct {
	struct gbm_device *dev;
	struct gbm_surface *surface;
} GBM;

typedef struct {
	int fd;
	drmModeModeInfo *mode;
	unsigned int crtc_id;
	unsigned int connector_id;
} DRM;

typedef struct {
	struct gbm_bo *bo;
	unsigned int fb_id;
} DRM_FB;

class MMSKms {
    private:
    	//! is initialized?
    	bool	isinitialized;

//        //! name of the device file
//        string  device_file;
//
//        //! device abbreviation
//        char    device[8];
//
//        //! file descriptor of the framebuffer
//        int     fd;

//        //! virtual framebuffer address
//        void    *framebuffer_base;
//
//        //! have to reset the console acceleration
//        bool	reset_console_accel;
//
//		//! saved settings from general terminal interface
//        struct termios saved_ts;

//        //! fix screen infos
//        struct fb_fix_screeninfo    fix_screeninfo;
//
//        //! variable screen infos
//        struct fb_var_screeninfo    var_screeninfo;
//
//        //! original variable screen infos
//        struct fb_var_screeninfo    org_var_screeninfo;
//
//        //! available modes read from /etc/fb.modes
//        struct fb_var_screeninfo	modes[MMSFBDEV_MAX_MODES];
//
//        //! number of loaded modes
//        int modes_cnt;

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

    	struct gbm_bo *bo;
    	DRM 	drm;
    	GBM		gbm;
    	DRM_FB *fb;

//        void printFixScreenInfo();
//        void printVarScreenInfo();
//        bool buildPixelFormat();
//
//        bool readModes();

		bool init_drm();
		bool init_gbm();
		DRM_FB* drm_fb_get_from_bo(struct gbm_bo *bo);

        void genFBPixelFormat(MMSFBSurfacePixelFormat pf, unsigned int *nonstd_format, MMSFBPixelDef *pixeldef);
        void disable(int fd, string device_file);
        bool activate(int fd, string device_file, struct fb_var_screeninfo *var_screeninfo,
        			  int width, int height, MMSFBSurfacePixelFormat pixelformat, bool switch_mode = true);

    public:
        MMSKms();
        virtual ~MMSKms();

        virtual bool openDevice();
        virtual void closeDevice();
        bool isInitialized();

        virtual bool waitForVSync();
        virtual bool panDisplay(int buffer_id, void *framebuffer_base = NULL);

        virtual bool testLayer(int layer_id);
        virtual bool initLayer(int layer_id, int width, int height, MMSFBSurfacePixelFormat pixelformat, int backbuffer = 0);

        virtual bool releaseLayer(int layer_id);
        virtual bool restoreLayer(int layer_id);

        bool getPixelFormat(int layer_id, MMSFBSurfacePixelFormat *pf);
        bool getFrameBufferPtr(int layer_id, MMSFBSurfacePlanesBuffer buffers, int *width, int *height);

        bool mapMmio(unsigned char **mmio);
        bool unmapMmio(unsigned char *mmio);

        bool setMode(int width, int height, MMSFBSurfacePixelFormat pixelformat, int backbuffer = 0);

        sigc::signal<bool, MMSFBSurfacePixelFormat, unsigned int*, MMSFBPixelDef*>::accumulated<neg_bool_accumulator> onGenFBPixelFormat;
        sigc::signal<bool, int, string>::accumulated<neg_bool_accumulator> onDisable;
        sigc::signal<bool, int, string, fb_var_screeninfo *, int, int, MMSFBSurfacePixelFormat, bool>::accumulated<neg_bool_accumulator> onActivate;

    private:
//        typedef struct {
//        	//! /dev/tty0 file descriptor
//			int fd0;
//
//			//! file descriptor of the allocated tty
//			int fd;
//
//			//! number of vt where disko is running
//			int number;
//
//			//! number of vt from which disko was started
//			int previous;
//
//			//! save original fb
//			int org_fb;
//        } VT;
//
//        VT	vt;

//        bool vtOpen(int console = MMSFBDEV_QUERY_CONSOLE);
//        void vtClose();
//        virtual bool vtGetFd(int *fd);

//        friend class MMSFBDevVesa;
//        friend class MMSFBDevMatrox;
//        friend class MMSFBDevDavinci;
//        friend class MMSFBDevOmap;
        friend class MMSInputLISHandler;

};

#endif

#endif /* MMSKMS_H_ */
