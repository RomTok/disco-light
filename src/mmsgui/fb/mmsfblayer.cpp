/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
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

#include "mmsgui/fb/mmsfblayer.h"
#include "mmsgui/fb/mmsfb.h"

#ifdef __HAVE_XLIB__
#include <sys/shm.h>
#endif


D_DEBUG_DOMAIN( MMS_Layer, "MMS/Layer", "MMS Layer" );

#ifdef __HAVE_XLIB__
#define GUID_YUV12_PLANAR 0x32315659
#endif


// static variables
bool MMSFBLayer::firsttime_createsurface		= true;
bool MMSFBLayer::firsttime_createwindow_usealpha= true;
bool MMSFBLayer::firsttime_createwindow_noalpha	= true;


#define INITCHECK  if(!this->dfblayer){MMSFB_SetError(0,"not initialized");return false;}


MMSFBLayer::MMSFBLayer(int id) {
    // init me
	this->dfblayer = NULL;
    this->surface = NULL;
    this->flipflags = (MMSFBSurfaceFlipFlags)0;
    this->config.id = id;
    this->config.window_pixelformat = MMSFB_PF_ARGB;
    this->config.surface_pixelformat = MMSFB_PF_ARGB;

    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		// get the layer
		DFBResult	dfbres;
		if ((dfbres = mmsfb->dfb->GetDisplayLayer(mmsfb->dfb, this->config.id, &this->dfblayer)) != DFB_OK) {
			this->dfblayer = NULL;
			MMSFB_SetError(dfbres, "IDirectFB::GetDisplayLayer(" + iToStr(id) + ") failed");
			return;
		}
#endif
    }
    else {
#ifdef __HAVE_XLIB__
        if (this->config.id != 0) {
			MMSFB_SetError(0, "x11 support needs layer 0!");
        	return;
        }
        this->dfblayer = (IDirectFBDisplayLayer*)1;

		// fill my config partly from mmsfb
		this->config.w = mmsfb->w;
		this->config.h = mmsfb->h;
		this->config.pixelformat = MMSFB_PF_YV12;
		this->config.buffermode = MMSFB_BM_BACKSYSTEM;
		this->config.options = MMSFB_LO_NONE;

		// create x11 buffer #1
        this->xv_image1 = XvShmCreateImage(mmsfb->x_display, mmsfb->xv_port, GUID_YUV12_PLANAR, 0, this->config.w, this->config.h, &this->xv_shminfo1);
        if (!this->xv_image1) {
			MMSFB_SetError(0, "XvShmCreateImage() failed");
        	return;
        }

        // map shared memory for x-server commuinication
        this->xv_shminfo1.shmid    = shmget(IPC_PRIVATE, this->xv_image1->data_size, IPC_CREAT | 0777);
        this->xv_shminfo1.shmaddr  = this->xv_image1->data = (char *)shmat(this->xv_shminfo1.shmid, 0, 0);
        this->xv_shminfo1.readOnly = False;

        // attach the x-server to that segment
        if (!XShmAttach(mmsfb->x_display, &this->xv_shminfo1)) {
        	XFree(this->xv_image1);
        	this->xv_image1 = NULL;
			MMSFB_SetError(0, "XShmAttach() failed");
        	return;
        }

		// create x11 buffer #2
        this->xv_image2 = XvShmCreateImage(mmsfb->x_display, mmsfb->xv_port, GUID_YUV12_PLANAR, 0, this->config.w, this->config.h, &this->xv_shminfo2);
        if (!this->xv_image2) {
        	XFree(this->xv_image1);
        	this->xv_image1 = NULL;
			MMSFB_SetError(0, "XvShmCreateImage() failed");
        	return;
        }

        // map shared memory for x-server commuinication
        this->xv_shminfo2.shmid    = shmget(IPC_PRIVATE, this->xv_image2->data_size, IPC_CREAT | 0777);
        this->xv_shminfo2.shmaddr  = this->xv_image2->data = (char *)shmat(this->xv_shminfo2.shmid, 0, 0);
        this->xv_shminfo2.readOnly = False;

        // attach the x-server to that segment
        if (!XShmAttach(mmsfb->x_display, &this->xv_shminfo2)) {
        	XFree(this->xv_image1);
        	XFree(this->xv_image2);
        	this->xv_image1 = NULL;
        	this->xv_image2 = NULL;
			MMSFB_SetError(0, "XShmAttach() failed");
        	return;
        }
#endif
    }

    // get the current config
    if (this->dfblayer) {
    	MMSFBLayerConfig config;
    	getConfiguration(&config);
    }
}


MMSFBLayer::~MMSFBLayer() {
    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		if (this->dfblayer)
			this->dfblayer->Release(this->dfblayer);
#endif
    }
    else {
#ifdef __HAVE_XLIB__
    	if (this->xv_image1)
    		XFree(this->xv_image1);
    	if (this->xv_image2)
    		XFree(this->xv_image2);
#endif
    }
}

bool MMSFBLayer::isInitialized() {
    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
    	return (this->dfblayer != NULL);
#endif
    }
    else {
#ifdef __HAVE_XLIB__
    	return (this->xv_image1 != NULL);
#endif
    }
}

bool MMSFBLayer::getID(int *id) {

    // check if initialized
    INITCHECK;

    // get configuration
	MMSFBLayerConfig config;
    if (!getConfiguration(&config))
        return false;

    // fill return values
    *id = this->config.id;

    return true;
}

bool MMSFBLayer::setExclusiveAccess() {

	/* check if initialized */
    INITCHECK;

    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		DFBResult   dfbres;

		/* set cooperative level to exclusive */
		if ((dfbres=this->dfblayer->SetCooperativeLevel(this->dfblayer, DLSCL_EXCLUSIVE)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::SetCooperativeLevel(DLSCL_EXCLUSIVE) failed");
			return false;
		}

		return true;
#endif
    }
    else {
#ifdef __HAVE_XLIB__
    	return true;
#endif
    }
}

bool MMSFBLayer::getConfiguration(MMSFBLayerConfig *config) {

	/* check if initialized */
    INITCHECK;

    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		DFBResult               dfbres;
		DFBDisplayLayerConfig   dlc;

		/* get configuration */
		if ((dfbres=this->dfblayer->GetConfiguration(this->dfblayer, &dlc)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::GetConfiguration() failed");
			return false;
		}

		/* fill my config */
		this->config.w = dlc.width;
		this->config.h = dlc.height;
		this->config.pixelformat = getDFBPixelFormatString(dlc.pixelformat);
		this->config.buffermode = getDFBLayerBufferModeString(dlc.buffermode);
		this->config.options = getDFBLayerOptionsString(dlc.options);
#endif
    }

    if (!config) {
    	DEBUGMSG("MMSGUI", "Layer properties:");

        if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
            DEBUGMSG("MMSGUI", " backend:     dfb");
#endif
        }
        else {
#ifdef __HAVE_XLIB__
            DEBUGMSG("MMSGUI", " backend:     x11");
#endif
        }

        DEBUGMSG("MMSGUI", " size:        " + iToStr(this->config.w) + "x" + iToStr(this->config.h));

    	DEBUGMSG("MMSGUI", " pixelformat: " + this->config.pixelformat);

	    if (this->config.buffermode!="")
	    	DEBUGMSG("MMSGUI", " buffermode:  " + this->config.buffermode);
	    else
	    	DEBUGMSG("MMSGUI", " buffermode:  NONE");

	    if (this->config.options!="")
	    	DEBUGMSG("MMSGUI", " options:     " + this->config.options);
	    else
	    	DEBUGMSG("MMSGUI", " options:     NONE");
    }

    /* fill return config */
    if (config)
        *config = this->config;

    return true;
}

bool MMSFBLayer::getResolution(int *w, int *h) {

    /* check if initialized */
    INITCHECK;

    /* get configuration */
	MMSFBLayerConfig config;
    if (!getConfiguration(&config))
        return false;

    /* fill return values */
    *w = this->config.w;
    *h = this->config.h;

    return true;
}

bool MMSFBLayer::getPixelformat(string *pixelformat) {

    /* check if initialized */
    INITCHECK;

    /* get configuration */
	MMSFBLayerConfig config;
    if (!getConfiguration(&config))
        return false;

    /* fill return values */
    *pixelformat = this->config.pixelformat;

    return true;
}

bool MMSFBLayer::setConfiguration(int w, int h, string pixelformat, string buffermode, string options,
								  string window_pixelformat, string surface_pixelformat) {

    /* check if initialized */
    INITCHECK;

    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		DFBResult               dfbres;
		DFBDisplayLayerConfig   dlc;

		/* get configuration */
		MMSFBLayerConfig config;
		if (!getConfiguration(&config))
			return false;


		/* change config data */
		dlc.flags = DLCONF_NONE;
		dlc.width = w;
		dlc.height = h;
		dlc.pixelformat = getDFBPixelFormatFromString(pixelformat);
		dlc.buffermode = getDFBLayerBufferModeFromString(buffermode);
		dlc.options = getDFBLayerOptionsFromString(options);

		if (dlc.width > 0)
			dlc.flags = (DFBDisplayLayerConfigFlags)(dlc.flags | DLCONF_WIDTH);
		if (dlc.height > 0)
			dlc.flags = (DFBDisplayLayerConfigFlags)(dlc.flags | DLCONF_HEIGHT);
		if (dlc.pixelformat != DSPF_UNKNOWN)
			dlc.flags = (DFBDisplayLayerConfigFlags)(dlc.flags | DLCONF_PIXELFORMAT);
		if (dlc.buffermode != DLBM_UNKNOWN)
			dlc.flags = (DFBDisplayLayerConfigFlags)(dlc.flags | DLCONF_BUFFERMODE);
	  //  if (dlc.options != DLOP_NONE) {
			DEBUGOUT("\nSET OPTIONS 0x%08x!!!!\n", dlc.options);
			dlc.flags = (DFBDisplayLayerConfigFlags)(dlc.flags | DLCONF_OPTIONS);
	  //  }

		/* test configuration */
		DFBDisplayLayerConfigFlags failedFlags;
		if ((dfbres=this->dfblayer->TestConfiguration(this->dfblayer, &dlc, &failedFlags)) != DFB_OK) {
			if(failedFlags & DLCONF_PIXELFORMAT) {
				MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::TestConfiguration(" + iToStr(w) + "x" + iToStr(h) + "," + pixelformat + "," + buffermode + "," + options + ") failed");
				DEBUGMSG("MMSGUI", "Your configuration contains a pixelformat that is not supported.");
				return false;
			}
			if(failedFlags & DLCONF_BUFFERMODE) {
				MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::TestConfiguration(" + iToStr(w) + "x" + iToStr(h) + "," + pixelformat + "," + buffermode + "," + options + ") failed");
				DEBUGMSG("MMSGUI", "Your configuration contains a buffermode that is not supported.");
				return false;
			}
			if(failedFlags & DLCONF_OPTIONS) {
				MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::TestConfiguration(" + iToStr(w) + "x" + iToStr(h) + "," + pixelformat + "," + buffermode + "," + options + ") failed");
				DEBUGMSG("MMSGUI", "Your configuration contains options that are not supported.");
				return false;
			}

			/* check if desired resolution is unsupported */
			if(failedFlags & DLCONF_WIDTH)
				dlc.flags = (DFBDisplayLayerConfigFlags)(dlc.flags & ~DLCONF_WIDTH);
			if(failedFlags & DLCONF_HEIGHT)
				dlc.flags = (DFBDisplayLayerConfigFlags)(dlc.flags & ~DLCONF_HEIGHT);
			if ((dfbres=this->dfblayer->TestConfiguration(this->dfblayer, &dlc, &failedFlags)) != DFB_OK) {
				MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::TestConfiguration(" + iToStr(w) + "x" + iToStr(h) + "," + pixelformat + "," + buffermode + "," + options + ") failed");
				return false;
			}
			DEBUGMSG("MMSGUI", "Your configuration contains a resolution that is not supported.");
		}

		/* set configuration */
		if((dfbres = this->dfblayer->SetConfiguration(this->dfblayer, &dlc)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::SetConfiguration(" + iToStr(w) + "x" + iToStr(h) + "," + pixelformat + "," + buffermode + "," + options + ") failed");
			return false;
		}

		/* get configuration */
		if (!getConfiguration())
			return false;

		/* set background */
		this->dfblayer->SetBackgroundMode(this->dfblayer, DLBM_COLOR);
		this->dfblayer->SetBackgroundColor(this->dfblayer, 0, 0, 0, 0);

		// set special config
		this->config.window_pixelformat = window_pixelformat;
		this->config.surface_pixelformat = surface_pixelformat;

		return true;
#endif
    }
    else {
#ifdef __HAVE_XLIB__
		// if we use XLIB, currently we cannot change the layer attributes
		this->config.window_pixelformat = window_pixelformat;
		this->config.surface_pixelformat = surface_pixelformat;

		return true;
#endif
    }
}

bool MMSFBLayer::setOpacity(unsigned char opacity) {

	/* check if initialized */
    INITCHECK;

    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		DFBResult   dfbres;

		/* invert the opacity for inverted pixelformats */
		if (this->config.pixelformat == MMSFB_PF_AiRGB) {
			opacity = 255 - opacity;
		}

		/* set the opacity */
		if ((dfbres=this->dfblayer->SetOpacity(this->dfblayer, opacity)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::SetOpacity(" + iToStr(opacity) + ") failed");
			return false;
		}

		return true;
#endif
    }

    return false;
}

bool MMSFBLayer::setLevel(int level) {

	/* check if initialized */
    INITCHECK;

    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		DFBResult   dfbres;

		/* set the opacity */
		if ((dfbres=this->dfblayer->SetLevel(this->dfblayer, level)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::SetLevel(" + iToStr(level) + ") failed");
			return false;
		}
		return true;
#endif
    }

    return false;
}

bool MMSFBLayer::getSurface(MMSFBSurface **surface) {

	// check if initialized
    INITCHECK;

    if (this->surface) {
        // i have already a surface
        *surface = this->surface;
        DEBUGMSG("MMSGUI", "have already a surface");
        return true;
    }

    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		// get layers surface
		DFBResult           dfbres;
		IDirectFBSurface    *dfbsurface;
    	DEBUGMSG("MMSGUI", "calling DFB->GetSurface()");
		if ((dfbres=this->dfblayer->GetSurface(this->dfblayer, &dfbsurface)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::GetSurface() failed");
			return false;
		}
    	DEBUGMSG("MMSGUI", "setting blitting flags");
		dfbsurface->SetBlittingFlags(dfbsurface, DSBLIT_NOFX);

		// create a new surface instance
		*surface = new MMSFBSurface(dfbsurface);
		if (!*surface) {
			dfbsurface->Release(dfbsurface);
			MMSFB_SetError(0, "cannot create new instance of MMSFBSurface");
			return false;
		}
#endif
    }
    else {
#ifdef __HAVE_XLIB__
        if ((!this->xv_image1)||(!this->xv_image2)) {
			MMSFB_SetError(0, "xv_image not available, cannot get surface");
        	return false;
        }

        // create a new surface instance
		*surface = new MMSFBSurface(this->config.w, this->config.h, this->config.pixelformat, this->xv_image1, this->xv_image2);
		if (!*surface) {
			MMSFB_SetError(0, "cannot create new instance of MMSFBSurface");
			return false;
		}

		// we must switch extended accel on
		(*surface)->setExtendedAcceleration(true);
#endif
    }

    // save this for the next call
    this->surface = *surface;

    if (this->surface) {
		// clear the surface
    	this->surface->clear();
		if ((this->config.buffermode == MMSFB_BM_BACKSYSTEM) || (this->config.buffermode == MMSFB_BM_BACKVIDEO)) {
			this->surface->flip();
			this->surface->clear();
		}
		if (this->config.buffermode == MMSFB_BM_TRIPLE) {
			this->surface->flip();
			this->surface->clear();
		}

	    // initialize the flip flags for the layer surface
	    this->surface->setFlipFlags(this->flipflags);

	    return true;
    }

    return false;
}

bool MMSFBLayer::setFlipFlags(MMSFBSurfaceFlipFlags flags) {
	this->flipflags = flags;

	/* if the layer surface does exist, update it */
	if (this->surface)
	    this->surface->setFlipFlags(this->flipflags);

	return true;
}

bool MMSFBLayer::createSurface(MMSFBSurface **surface, int w, int h,
                               string pixelformat, int backbuffer) {


    D_DEBUG_AT( MMS_Layer, "createSurface( %dx%d, %s, %s buffer )\n", w, h, pixelformat.c_str(),
                (backbuffer != 0) ? "double" : "single" );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

    if (pixelformat == MMSFB_PF_NONE) {
        pixelformat = this->config.pixelformat;
        if (!isAlphaPixelFormat(pixelformat)) {
        	// the gui internally needs surfaces with alpha channel
        	// now we have to decide if we are working in RGB or YUV color space
        	pixelformat = this->config.surface_pixelformat;
        	if ((pixelformat == "")||((pixelformat != MMSFB_PF_ARGB)&&(pixelformat != MMSFB_PF_AiRGB)&&(pixelformat != MMSFB_PF_AYUV))) {
        		// use autodetection
	        	if (!isRGBPixelFormat(pixelformat))
		            // so switch all non-alpha pixelformats to AYUV
		            pixelformat = MMSFB_PF_AYUV;
	            else
		            // so switch all non-alpha pixelformats to ARGB
		            pixelformat = MMSFB_PF_ARGB;
        	}
        }
        else
        if (isIndexedPixelFormat(pixelformat))
            // the gui internally needs non-indexed surfaces
            // so switch all indexed pixelformats to ARGB
            pixelformat = MMSFB_PF_ARGB;
    }

    if (firsttime_createsurface) {
    	printf("DISKO: Pixelformat %s is used for surfaces.\n", pixelformat.c_str());
    	firsttime_createsurface = false;
    }
    return mmsfb->createSurface(surface, w, h, pixelformat, backbuffer, (this->config.buffermode == MMSFB_BM_BACKSYSTEM));
}

bool MMSFBLayer::createWindow(MMSFBWindow **window, int x, int y, int w, int h,
                              string pixelformat, bool usealpha, bool uselayersurface) {

    /* check if initialized */
    INITCHECK;

    /* check if i am the right layer */
    MMSFBLayer *layer;
    mmsfbwindowmanager->getLayer(&layer);
    if (layer != this) {
        MMSFB_SetError(0, "not the right layer, cannot create MMSFBWindow");
        return false;
    }

    /* get pixelformat */
    if (pixelformat == MMSFB_PF_NONE)
        pixelformat = this->config.pixelformat;

    if (!isAlphaPixelFormat(pixelformat)) {
        /* non-alpha pixelformats  */
        if (usealpha) {
        	// switch all non-alpha pixelformats to alpha
        	// now we have to decide if we are working in RGB or YUV color space
        	pixelformat = this->config.window_pixelformat;
        	if ((pixelformat == "")||((pixelformat != MMSFB_PF_ARGB)&&(pixelformat != MMSFB_PF_AiRGB)&&(pixelformat != MMSFB_PF_AYUV))) {
        		// use autodetection
	        	if (!isRGBPixelFormat(pixelformat))
		            // so switch all non-alpha pixelformats to AYUV
		            pixelformat = MMSFB_PF_AYUV;
	            else
		            // so switch all non-alpha pixelformats to ARGB
		            pixelformat = MMSFB_PF_ARGB;
        	}
        }
    }
    else {
        /* alpha pixelformats  */
        if (!usealpha)
            /* switch all alpha pixelformats to RGB32 */
            pixelformat = MMSFB_PF_RGB32;
    }

    if (isIndexedPixelFormat(pixelformat)) {
        /* indexed pixelformats  */
        if (!usealpha)
            /* switch all indexed pixelformats to RGB32 */
            pixelformat = MMSFB_PF_RGB32;
        else
            /* switch all indexed pixelformats to ARGB */
            pixelformat = MMSFB_PF_ARGB;
    }

    if (usealpha) {
	    if (firsttime_createwindow_usealpha) {
	    	printf("DISKO: Pixelformat %s is used for windows with alphachannel.\n", pixelformat.c_str());
	    	firsttime_createwindow_usealpha = false;
	    }
    }
    else
	    if (firsttime_createwindow_noalpha) {
	    	printf("DISKO: Pixelformat %s is used for windows with no alphachannel.\n", pixelformat.c_str());
	    	firsttime_createwindow_noalpha = false;
	    }


#ifdef USE_DFB_WINMAN

    DFBResult               dfbres;
    IDirectFBWindow         *dfbwindow;
    DFBWindowDescription    window_desc;

    /* create window description */
    window_desc.flags = (DFBWindowDescriptionFlags)(DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_PIXELFORMAT | DWDESC_CAPS);
    window_desc.posx = x;
    window_desc.posy = y;
    window_desc.width = w;
    window_desc.height = h;
    window_desc.pixelformat = getDFBPixelFormatFromString(pixelformat);

    /* set caps - differs between alpha and non-alpha pixelformats */
    if (!isAlphaPixelFormat(pixelformat))
        window_desc.caps = (DFBWindowCapabilities)(DWCAPS_DOUBLEBUFFER);
    else
        window_desc.caps = (DFBWindowCapabilities)(DWCAPS_DOUBLEBUFFER | DWCAPS_ALPHACHANNEL);

    /* create the window */
    if ((dfbres=this->dfblayer->CreateWindow(this->dfblayer, &window_desc, &dfbwindow)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::CreateWindow(" + iToStr(w) + "x" + iToStr(h) + "," + getDFBPixelFormatString(window_desc.pixelformat) + ") failed");
        return false;
    }

    /* create a new window instance */
    *window = new MMSFBWindow(dfbwindow, window_desc.posx, window_desc.posy);
    if (!*window) {
        dfbwindow->Release(dfbwindow);
        MMSFB_SetError(0, "cannot create new instance of MMSFBWindow");
        return false;
    }

#endif

#ifdef USE_MMSFB_WINMAN

    MMSFBSurface *surface;
    bool usels = false;

    if (uselayersurface) {
        /* check: i can only accept this flag */
        /* if the window uses the complete layer surface */
//
//temporary disabled, because of some timing problems
//biggest problem: the DVD freeze-frame sends NO frames to the GUI - therefore the autodetection cannot run correctly
//                 -> so saved_surface in mmsfb_windowmanager can be removed in the next release!!!
//
//        usels = ((x == 0)&&(y == 0)&&(w == this->config.w)&&(h == this->config.h));
//
    }

    if (!usels) {
        /* create a window surface with one backbuffer (double buffered) */
        if (!mmsfb->createSurface(&surface, w, h, pixelformat, 1, (this->config.buffermode == MMSFB_BM_BACKSYSTEM)))
            return false;
    }
    else {
        /* use the layer surface */

        /* create a new surface instance */
        surface = new MMSFBSurface((IDirectFBSurface *)this->surface->getDFBSurface());
        if (!surface) {
            MMSFB_SetError(0, "cannot create new instance of MMSFBSurface");
            return false;
        }

        /* that is the layer surface */
        surface->setLayerSurface();
    }

    /* create a new window instance */
    *window = new MMSFBWindow(surface, x, y);
    if (!*window) {
        delete surface;
        MMSFB_SetError(0, "cannot create new instance of MMSFBWindow");
        return false;
    }

    /* that is a window surface */
    surface->setWinSurface();

    /* inform the window manager */
    mmsfbwindowmanager->addWindow(*window);

#endif

    return true;
}




