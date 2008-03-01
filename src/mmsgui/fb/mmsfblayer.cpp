/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
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


D_DEBUG_DOMAIN( MMS_Layer, "MMS/Layer", "MMS Layer" );


#define INITCHECK  if(!this->dfblayer){MMSFB_SetError(0,"not initialized");return false;}

MMSFBLayer::MMSFBLayer(IDirectFB *dfb, IDirectFBDisplayLayer *dfblayer) {
    /* init me */
    this->dfb = dfb;
    this->dfblayer = dfblayer;
    this->surface = NULL;
    this->flipflags = (MMSFBSurfaceFlipFlags)0;

    /* get the current config */
    if (this->dfblayer) {
    	MMSFBLayerConfig config;
    	getConfiguration(&config);
    }
}

MMSFBLayer::~MMSFBLayer() {
    if (this->dfblayer)
        this->dfblayer->Release(this->dfblayer);
}

bool MMSFBLayer::getID(int *id) {
    DFBResult           dfbres;
    DFBDisplayLayerID   di;

    /* check if initialized */
    INITCHECK;

    /* get the id of the layer */
    if ((dfbres = this->dfblayer->GetID(this->dfblayer, &di)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::GetID() failed");
        return false;
    }
    *id = di;

    return true;
}

bool MMSFBLayer::setExclusiveAccess() {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* set cooperative level to exclusive */
    if ((dfbres=this->dfblayer->SetCooperativeLevel(this->dfblayer, DLSCL_EXCLUSIVE)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::SetCooperativeLevel(DLSCL_EXCLUSIVE) failed");
        return false;
    }
    
    return true;
}

bool MMSFBLayer::getConfiguration(MMSFBLayerConfig *config) {
    DFBResult               dfbres;
    DFBDisplayLayerConfig   dlc;     

    /* check if initialized */
    INITCHECK;

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

    if (!config) {
    	DEBUGMSG("MMSGUI", "Layer properties:");
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

bool MMSFBLayer::setConfiguration(int w, int h, string pixelformat, string buffermode, string options) {
    DFBResult               dfbres;
    DFBDisplayLayerConfig   dlc;     

    /* check if initialized */
    INITCHECK;

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
        printf("\nSET OPTIONS 0x%08x!!!!\n", dlc.options);
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

    return true;
}

bool MMSFBLayer::setOpacity(unsigned char opacity) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

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
}

bool MMSFBLayer::setLevel(int level) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* set the opacity */
    if ((dfbres=this->dfblayer->SetLevel(this->dfblayer, level)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::SetLevel(" + iToStr(level) + ") failed");
        return false;
    }
    return true;
}

bool MMSFBLayer::getSurface(MMSFBSurface **surface) {
    DFBResult           dfbres;
    IDirectFBSurface    *dfbsurface;

    /* check if initialized */
    INITCHECK;

    if (this->surface) {
        /* i have already a surface */
        *surface = this->surface;
        return true;
    }

    /* get layers surface */
    if ((dfbres=this->dfblayer->GetSurface(this->dfblayer, &dfbsurface)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBDisplayLayer::GetSurface() failed");
        return false;
    }
	dfbsurface->SetBlittingFlags( dfbsurface, DSBLIT_NOFX );
    /* create a new surface instance */
    *surface = new MMSFBSurface(dfbsurface);
    if (!*surface) {
        dfbsurface->Release(dfbsurface);
        MMSFB_SetError(0, "cannot create new instance of MMSFBSurface");
        return false;
    }

    /* clear the surface */
    dfbsurface->Clear(dfbsurface,0,0,0,0);
    dfbsurface->Flip(dfbsurface,NULL,(DFBSurfaceFlipFlags)0);
    dfbsurface->Clear(dfbsurface,0,0,0,0);
    dfbsurface->Flip(dfbsurface,NULL,(DFBSurfaceFlipFlags)0);
    dfbsurface->Clear(dfbsurface,0,0,0,0);

    /* save this for the next call */
    this->surface = *surface;
    
    /* initialize the flip flags for the layer surface */
    this->surface->setFlipFlags(this->flipflags);

    return true;
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
        if (!isAlphaPixelFormat(pixelformat))
            /* the gui internally needs surfaces with alpha channel */
            /* so switch all non-alpha pixelformats to ARGB */
            pixelformat = MMSFB_PF_ARGB;
        else
        if (isIndexedPixelFormat(pixelformat))
            /* the gui internally needs non-indexed surfaces */
            /* so switch all indexed pixelformats to ARGB */
            pixelformat = MMSFB_PF_ARGB;
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
        if (usealpha)
            /* switch all non-alpha pixelformats to ARGB */
            pixelformat = MMSFB_PF_ARGB;
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
        surface = new MMSFBSurface(this->surface->getDFBSurface());
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




