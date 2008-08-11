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

#include "mmsgui/mmsfbmanager.h"
#include "mmsgui/fb/mmsfbsurfacemanager.h"
#include <string.h>
#include <stdlib.h>

MMS_CREATEERROR(MMSFBManagerError);

/* initialize the mmsfbmanager object */ 
MMSFBManager mmsfbmanager;

/* exit handler routine */
void mmsfbmanager_onexit(int num, void *arg) {
    MMSFBManager *o=(MMSFBManager*)arg;
    o->release();
}

MMSFBManager::MMSFBManager() {
    /* init me */
    this->graphicslayer = NULL;
    this->videolayer = NULL;
    this->videolayerid = -1;
    this->graphicslayerid = -1;
    this->layercount = 0;     
}

MMSFBManager::~MMSFBManager() {
}

bool MMSFBManager::init(int argc, char **argv) {
	int myargc=argc;
	char *myargv[255];
	int i;

    this->layercount=1; 
	
	for(i=0;i<argc;i++)
		myargv[i]=strdup(argv[i]);
		
	if(config.getOutputType() == MMS_OT_X11FB) {
		myargv[myargc]=strdup("--dfb:system=x11");
		myargc++;
	}
    DEBUGMSG("MMSGUI", "init mmsfb");
    if (!mmsfb->init(myargc, myargv))
        throw new MMSFBManagerError(0, MMSFB_LastErrorString);

    DEBUGMSG("MMSGUI", "get video layer");
    if (!mmsfb->getLayer(this->config.getVideoLayerId(), &this->videolayer))
        throw new MMSFBManagerError(0, MMSFB_LastErrorString);
     
    if(this->config.getVideoLayerId() == this->config.getGraphicsLayerId()) {
    	DEBUGMSG("MMSGUI", "video layer and graphics layer are the same");
        this->graphicslayer = this->videolayer;
    }
    else {
        this->layercount++; 
        DEBUGMSG("MMSGUI", "get graphics layer");
        if (!mmsfb->getLayer(this->config.getGraphicsLayerId(), &this->graphicslayer))
            throw new MMSFBManagerError(0, MMSFB_LastErrorString);

    	this->graphicslayer->setFlipFlags((MMSFBSurfaceFlipFlags)DSFLIP_ONSYNC);
    }

	if (config.getOutputType() == MMS_OT_MATROXFB)
    	this->videolayer->setFlipFlags((MMSFBSurfaceFlipFlags)DSFLIP_WAITFORSYNC);
    else
    	this->videolayer->setFlipFlags((MMSFBSurfaceFlipFlags)DSFLIP_ONSYNC);
    
    if (!this->graphicslayer->getID(&this->graphicslayerid))
        throw new MMSFBManagerError(0, MMSFB_LastErrorString);

    if (!this->videolayer->getID(&this->videolayerid))
        throw new MMSFBManagerError(0, MMSFB_LastErrorString);

    /* set on exit handler */
    on_exit(mmsfbmanager_onexit, this);
    
    return true;
}

void MMSFBManager::release() {
	DEBUGMSG("MMSGUI", "release mmsfb");
    if (this->videolayer)
        delete this->videolayer;
    mmsfb->release();
} 

void MMSFBManager::applySettings() {
	DEBUGMSG("MMSGUI", "configure graphics layer");

	DEBUGMSG("MMSGUI", "set exclusive access");
    /* set exclusive access to the graphics layer */
    if (!this->graphicslayer->setExclusiveAccess())
        throw new MMSFBManagerError(0, MMSFB_LastErrorString);

    if (!this->graphicslayer->setConfiguration(config.getXres(), config.getYres(),
                                               config.getGraphicsLayerPixelformat(),
                                               config.getGraphicsLayerBufferMode(),
                                               config.getGraphicsLayerOptions(),
                                               config.getGraphicsWindowPixelformat(),
                                               config.getGraphicsSurfacePixelformat()))
        throw new MMSFBManagerError(0, MMSFB_LastErrorString);

    if (this->videolayerid != this->graphicslayerid) {
        if(config.getOutputType() == MMS_OT_X11FB) 
        //give a little time to window routines 
        usleep(300000); 

        /* use both layers */
        DEBUGMSG("MMSGUI", "configure video layer");

        DEBUGMSG("MMSGUI", "set exclusive access");
        /* set exclusive access to the video layer */
        if (!this->videolayer->setExclusiveAccess())
            throw new MMSFBManagerError(0, MMSFB_LastErrorString);

        /* set video layer's config */
        if (!this->videolayer->setConfiguration(config.getXres(), config.getYres(),
                                                config.getVideoLayerPixelformat(),
                                                config.getVideoLayerBufferMode(),
                                                config.getVideoLayerOptions()))
            throw new MMSFBManagerError(0, MMSFB_LastErrorString);
		//this->videolayer->dfblayer->SetFieldParity(this->videolayer->dfblayer,0);

        /* set the full opacity of the graphics layer */    
        this->graphicslayer->setOpacity(0);

        if (config.getOutputType() == MMS_OT_VIAFB) {
            /* set the video layer behind the graphics layer */
        	DEBUGMSG("MMSGUI", "set the video layer behind the graphics layer");
            this->videolayer->setLevel(-1);
        }
    }

    /* init the mmsfbwindowmanager */
    mmsfbwindowmanager->init(this->graphicslayer, config.getPointer());

    // create a global temporary surface
    string pixelformat = config.getGraphicsLayerPixelformat(); 
    if (!isAlphaPixelFormat(pixelformat)) {
    	// the gui internally needs surfaces with alpha channel
    	// now we have to decide if we are working in RGB or YUV color space
    	pixelformat = config.getGraphicsSurfacePixelformat();
    	if ((pixelformat == "")||((pixelformat != MMSFB_PF_ARGB)&&(pixelformat != MMSFB_PF_AYUV))) {
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

    string buffermode = config.getGraphicsLayerBufferMode(); 
    DEBUGMSG("MMSGUI", "creating temporary surface: %dx%d ,%s", config.getXres(), config.getYres(), pixelformat.c_str());
    mmsfbsurfacemanager->createTemporarySurface(config.getXres(), config.getYres(), pixelformat, (buffermode == MMSFB_BM_BACKSYSTEM));
    MMSFBSurface *ts = mmsfbsurfacemanager->getTemporarySurface(10,10);
    if (ts) {
    	// currently we do not accelerate using video hardware
    	if (buffermode == MMSFB_BM_BACKSYSTEM)
    		ts->setExtendedAcceleration(config.getExtendedAccel());
    	mmsfbsurfacemanager->releaseTemporarySurface(ts);
    }
}


MMSFBLayer *MMSFBManager::getVideoLayer() {
    return this->videolayer;
}

MMSFBLayer *MMSFBManager::getGraphicsLayer() {
    return this->graphicslayer;
}

int MMSFBManager::getLayerCount() {
    return this->layercount;
}

