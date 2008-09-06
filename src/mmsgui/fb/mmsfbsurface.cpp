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

#include "mmsgui/fb/mmsfbsurface.h"
#include "mmsgui/fb/mmsfb.h"
#include "mmsgui/fb/mmsfbsurfacemanager.h"
#include <math.h>
#include <string.h>

D_DEBUG_DOMAIN( MMS_Surface, "MMS/Surface", "MMS FB Surface" );


// static variables
bool MMSFBSurface::extendedaccel								= false;
bool MMSFBSurface::firsttime_eAB_blend_argb_to_argb 			= true;
bool MMSFBSurface::firsttime_eAB_blend_srcalpha_argb_to_argb	= true;
bool MMSFBSurface::firsttime_eAB_rgb16_to_rgb16				    = true;
bool MMSFBSurface::firsttime_eAB_argb_to_rgb16				    = true;
bool MMSFBSurface::firsttime_eAB_blend_argb_to_rgb16			= true;
bool MMSFBSurface::firsttime_eAB_blend_airgb_to_airgb 			= true;
bool MMSFBSurface::firsttime_eAB_blend_ayuv_to_ayuv				= true;

bool MMSFBSurface::firsttime_eASB_blend_argb_to_argb 			= true;
bool MMSFBSurface::firsttime_eASB_blend_srcalpha_argb_to_argb 	= true;


#define INITCHECK  if((!mmsfb->isInitialized())||(!this->dfbsurface)){MMSFB_SetError(0,"not initialized");return false;}

#define CLIPSUBSURFACE \
	DFBRegion reg, tmp; \
	bool tmpset; \
	if (clipSubSurface(&reg, false, &tmp, &tmpset)) {

#define UNCLIPSUBSURFACE \
	clipSubSurface(NULL, false, &tmp, &tmpset); }

#define SETSUBSURFACE_DRAWINGFLAGS \
	MMSFBColor ccc = this->config.color; \
	this->dfbsurface->SetColor(this->dfbsurface, ccc.r, ccc.g, ccc.b, ccc.a); \
	this->dfbsurface->SetDrawingFlags(this->dfbsurface, (DFBSurfaceDrawingFlags) this->config.drawingflags);

#define RESETSUBSURFACE_DRAWINGFLAGS \
    ccc = this->root_parent->config.color; \
    this->dfbsurface->SetColor(this->dfbsurface, ccc.r, ccc.g, ccc.b, ccc.a); \
    this->dfbsurface->SetDrawingFlags(this->dfbsurface, (DFBSurfaceDrawingFlags) this->root_parent->config.drawingflags);

#define SETSUBSURFACE_BLITTINGFLAGS \
	MMSFBColor ccc = this->config.color; \
	this->dfbsurface->SetColor(this->dfbsurface, ccc.r, ccc.g, ccc.b, ccc.a); \
	this->dfbsurface->SetBlittingFlags(this->dfbsurface, (DFBSurfaceBlittingFlags) this->config.blittingflags);

#define RESETSUBSURFACE_BLITTINGFLAGS \
    ccc = this->root_parent->config.color; \
    this->dfbsurface->SetColor(this->dfbsurface, ccc.r, ccc.g, ccc.b, ccc.a); \
    this->dfbsurface->SetBlittingFlags(this->dfbsurface, (DFBSurfaceBlittingFlags) this->root_parent->config.blittingflags);



MMSFBSurface::MMSFBSurface(IDirectFBSurface *dfbsurface,
	        		       MMSFBSurface *parent,
						   DFBRectangle *sub_surface_rect) {
    /* init me */
    this->dfbsurface = dfbsurface;
    this->flipflags = (MMSFBSurfaceFlipFlags)0;
    this->TID = 0;
    this->Lock_cnt = 0;

    /* init subsurface */
    this->parent = parent;
    this->root_parent =  NULL;
    this->sub_surface_xoff = 0;
    this->sub_surface_yoff = 0;
    if (this->parent) {
    	if (!this->parent->is_sub_surface)
			this->root_parent = this->parent;
    	else
    		this->root_parent = this->parent->root_parent;

    	this->is_sub_surface = true;

    	this->sub_surface_rect = *sub_surface_rect;

#ifndef USE_DFB_SUBSURFACE
  		this->dfbsurface = this->root_parent->dfbsurface;

  		getRealSubSurfacePos();
#endif

    }
    else {
    	this->is_sub_surface = false;
    	this->sub_surface_rect.x = 0;
    	this->sub_surface_rect.y = 0;
    	this->sub_surface_rect.w = 0;
    	this->sub_surface_rect.h = 0;
    }
    	
    /* get the current config */
    if (this->dfbsurface) {
        getConfiguration();

        /* init the color */
        this->config.color.r = 0;
        this->config.color.g = 0;
        this->config.color.b = 0;
        this->config.color.a = 0;
        this->config.clipped = false;
        this->config.iswinsurface = false;
        this->config.islayersurface = false;
        this->config.drawingflags = DSDRAW_NOFX;
        this->config.blittingflags = DSBLIT_NOFX;
    }
}

MMSFBSurface::~MMSFBSurface() {

    if (!mmsfb->isInitialized()) return;

    /* release memory - only if not the layer surface */
    if (this->dfbsurface)
        if (!this->config.islayersurface)
        	if (!this->is_sub_surface) {
#ifndef USE_DFB_SUBSURFACE
        		/* delete all sub surfaces */
    			deleteSubSurface(NULL);
#endif
        		mmsfbsurfacemanager->releaseSurface(this);
        	}
        	else {
#ifdef USE_DFB_SUBSURFACE
        		this->dfbsurface->Release(this->dfbsurface);
#endif

        		if (this->parent)
        			this->parent->deleteSubSurface(this);
        	}
}

void MMSFBSurface::deleteSubSurface(MMSFBSurface *surface) {
	if (surface) {
		/* remove a sub surface from the list */
		for (unsigned int i = 0; i < this->children.size(); i++)
			if (this->children.at(i) == surface) {
	            this->children.erase(this->children.begin()+i);
				break;
			}
	}
	else {
		/* delete all sub surfaces */
		for (unsigned int i = 0; i < this->children.size(); i++) {
			this->children.at(i)->deleteSubSurface(NULL);
			delete this->children.at(i);
		}
	}
}
#ifdef sdsfs
void MMSFBSurface::getRealSubSurfacePos(MMSFBSurface *surface, bool refreshChilds) {
	if (this->is_sub_surface) {
		if (!surface) {
			surface = this;
			surface->sub_surface_xoff = 0;
			surface->sub_surface_yoff = 0;
		}
/*		surface->sub_surface_xoff += this->parent->sub_surface_rect.x + this->parent->sub_surface_xoff;
		surface->sub_surface_yoff += this->parent->sub_surface_rect.y + this->parent->sub_surface_yoff;*/
		this->parent->getRealSubSurfacePos(surface);
		surface->sub_surface_xoff = this->parent->sub_surface_rect.x + this->parent->sub_surface_xoff;
		surface->sub_surface_yoff = this->parent->sub_surface_rect.y + this->parent->sub_surface_yoff;
		
		if (refreshChilds)
			for (unsigned int i = 0; i < this->children.size(); i++)
				this->children.at(i)->getRealSubSurfacePos(NULL, refreshChilds);
	}
}
#endif
void MMSFBSurface::getRealSubSurfacePos(MMSFBSurface *surface, bool refreshChilds) {
	if (this->is_sub_surface) {
		this->sub_surface_xoff = this->sub_surface_rect.x + this->parent->sub_surface_xoff;
		this->sub_surface_yoff = this->sub_surface_rect.y + this->parent->sub_surface_yoff;
		
		if (refreshChilds)
			for (unsigned int i = 0; i < this->children.size(); i++)
				this->children.at(i)->getRealSubSurfacePos(NULL, refreshChilds);
	}
	else {
		this->sub_surface_xoff = 0;
		this->sub_surface_yoff = 0;
	}
}


bool MMSFBSurface::clipSubSurface(DFBRegion *region, bool regionset, DFBRegion *tmp, bool *tmpset) {
	DFBRegion myregion;

	if (!region) {
		if (*tmpset)
			this->root_parent->setClip(tmp);
		else
			this->root_parent->setClip(NULL);
		this->root_parent->unlock();
		return true;
	}
	
	/* get my region */
	getClip(&myregion);
	if (this->is_sub_surface) {
	    myregion.x1+=sub_surface_xoff;
	    myregion.y1+=sub_surface_yoff;
	    myregion.x2+=sub_surface_xoff;
	    myregion.y2+=sub_surface_yoff;
	}
	
	if (!regionset) {
		/* init region */
		*region = myregion;
		if(this->parent)
    		return this->parent->clipSubSurface(region, true, tmp, tmpset);
	}
    
    /* check if input region is within my region */
    if (region->x1 < myregion.x1)
    	region->x1 = myregion.x1;
    else
    if (region->x1 > myregion.x2)
    	return false;

    if (region->y1 < myregion.y1)
    	region->y1 = myregion.y1;
    else
    if (region->y1 > myregion.y2)
    	return false;
    
    if (region->x2 > myregion.x2)
    	region->x2 = myregion.x2;
    else
    if (region->x2 < myregion.x1)
    	return false;

    if (region->y2 > myregion.y2)
    	region->y2 = myregion.y2;
    else
    if (region->y2 < myregion.y1)
    	return false;

    /* have a parent, call recursive */
	if (this->is_sub_surface)
		return this->parent->clipSubSurface(region, true, tmp, tmpset);

	/* i am the root, set clip now */
	lock();
	if (this->config.clipped) {
		getClip(tmp);
		*tmpset=true;
	}
	else
		*tmpset=false;
	setClip(region);
	return true;
}

IDirectFBSurface *MMSFBSurface::getDFBSurface() {
    return this->dfbsurface;
}

bool MMSFBSurface::getConfiguration(MMSFBSurfaceConfig *config) {
    DFBResult               dfbres;
    DFBSurfacePixelFormat   mypf;
    DFBSurfaceCapabilities  caps;

    /* check if initialized */
    INITCHECK;

    /* get size */
    if (!this->is_sub_surface) {
	    if ((dfbres=this->dfbsurface->GetSize(this->dfbsurface, &(this->config.w), &(this->config.h))) != DFB_OK) {
	        MMSFB_SetError(dfbres, "IDirectFBSurface::GetSize() failed");
	        return false;
	    }
    }
    else {
#ifdef USE_DFB_SUBSURFACE
	    if ((dfbres=this->dfbsurface->GetSize(this->dfbsurface, &(this->config.w), &(this->config.h))) != DFB_OK) {
	        MMSFB_SetError(dfbres, "IDirectFBSurface::GetSize() failed");
	        return false;
	    }
#else
	    this->config.w = this->sub_surface_rect.w;
	    this->config.h = this->sub_surface_rect.h;
#endif
    }

    /* get pixelformat */
    if ((dfbres=this->dfbsurface->GetPixelFormat(this->dfbsurface, &mypf)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetPixelFormat() failed");
        return false;
    }

    /* build a format string */
    this->config.pixelformat = getDFBPixelFormatString(mypf);
    this->config.alphachannel = isAlphaPixelFormat(this->config.pixelformat);

    /* get capabilities */
    if ((dfbres=this->dfbsurface->GetCapabilities(this->dfbsurface, &caps)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetCapabilities() failed");
        return false;
    }

    /* is it a premultiplied surface? */
    this->config.premultiplied = caps & DSCAPS_PREMULTIPLIED;
    
    /* get the buffer mode */
    this->config.backbuffer = 0;
    if (caps & DSCAPS_DOUBLE)
        this->config.backbuffer = 1;
    else
    if (caps & DSCAPS_TRIPLE)
        this->config.backbuffer = 2;

    /* system only? */
	this->config.systemonly = false;
    if (caps & DSCAPS_SYSTEMONLY)
    	this->config.systemonly = true;
    
    /* fill return config */
    if (config)
        *config = this->config;

    /* log some infos */
    if ((!config)&&(!this->is_sub_surface)) {
    	DEBUGMSG("MMSGUI", "Surface properties:");

    	DEBUGMSG("MMSGUI", " size:         " + iToStr(this->config.w) + "x" + iToStr(this->config.h));

	    if (this->config.alphachannel)
	    	DEBUGMSG("MMSGUI", " pixelformat:  " + this->config.pixelformat + ",ALPHACHANNEL");
	    else
	    	DEBUGMSG("MMSGUI", " pixelformat:  " + this->config.pixelformat);
	    
	    DEBUGMSG("MMSGUI", " capabilities:");
	
	    if (caps & DSCAPS_PRIMARY)
	    	DEBUGMSG("MMSGUI", "  PRIMARY");
	    if (caps & DSCAPS_SYSTEMONLY)
	    	DEBUGMSG("MMSGUI", "  SYSTEMONLY");
	    if (caps & DSCAPS_VIDEOONLY)
	    	DEBUGMSG("MMSGUI", "  VIDEOONLY");
	    if (caps & DSCAPS_DOUBLE)
	    	DEBUGMSG("MMSGUI", "  DOUBLE");
	    if (caps & DSCAPS_TRIPLE)
	    	DEBUGMSG("MMSGUI", "  TRIPLE");
	    if (caps & DSCAPS_PREMULTIPLIED)
	    	DEBUGMSG("MMSGUI", "  PREMULTIPLIED");
    }

    return true;
}

void MMSFBSurface::setExtendedAcceleration(bool extendedaccel) {
	this->extendedaccel = extendedaccel;
}

bool MMSFBSurface::getExtendedAcceleration() {
	return this->extendedaccel;
}

bool MMSFBSurface::isWinSurface() {
    return this->config.iswinsurface;
}

bool MMSFBSurface::isLayerSurface() {
    return this->config.islayersurface;
}

bool MMSFBSurface::setWinSurface(bool iswinsurface) {

    /* check if initialized */
    INITCHECK;

    /* set the flag */
    this->config.iswinsurface = iswinsurface;

    return true;
}

bool MMSFBSurface::setLayerSurface(bool islayersurface) {

    /* check if initialized */
    INITCHECK;

    /* set the flag */
    this->config.islayersurface = islayersurface;

    return true;
}


bool MMSFBSurface::getPixelFormat(string *pixelformat) {

    /* check if initialized */
    INITCHECK;

    /* return the pixelformat */
    *pixelformat = config.pixelformat;

    return true;
}

bool MMSFBSurface::getSize(int *w, int *h) {

    /* check if initialized */
    INITCHECK;

    /* return values */
    *w = config.w;
    *h = config.h;

    return true;
}

bool MMSFBSurface::getMemSize(int *size) {
	
	/* check if initialized */
    INITCHECK;

    /* init size */
    if (!size)
    	return false;
    *size = 0;
 
    string pf = config.pixelformat;
    int    px = config.w * config.h * (config.backbuffer+1);
    
    if(pf == MMSFB_PF_ARGB1555)
    	*size = px * 2;
    else
    if(pf == MMSFB_PF_RGB16)
    	*size = px * 2;
    else
    if(pf == MMSFB_PF_RGB24)
    	*size = px * 3;
    else
    if(pf == MMSFB_PF_RGB32)
    	*size = px * 4;
    else
    if(pf == MMSFB_PF_ARGB)
    	*size = px * 4;
    else
    if(pf == MMSFB_PF_A8)
    	*size = px;
    else
    if(pf == MMSFB_PF_YUY2)
    	*size = px * 2;
    else
    if(pf == MMSFB_PF_RGB332)
    	*size = px;
    else
    if(pf == MMSFB_PF_UYVY)
    	*size = px * 2;
    else
    if(pf == MMSFB_PF_I420)
    	*size = (px * 3) / 2;
    else
    if(pf == MMSFB_PF_YV12)
    	*size = (px * 3) / 2;
    else
    if(pf == MMSFB_PF_LUT8)
    	*size = px;
    else
    if(pf == MMSFB_PF_ALUT44)
    	*size = px;
    else
    if(pf == MMSFB_PF_AiRGB)
    	*size = px * 4;
    else
    if(pf == MMSFB_PF_A1)
    	*size = px / 8;
    else
    if(pf == MMSFB_PF_NV12)
    	*size = (px * 3) / 2;
    else
    if(pf == MMSFB_PF_NV16)
    	*size = px * 2;
    else
    if(pf == MMSFB_PF_ARGB2554)
    	*size = px * 2;
    else
    if(pf == MMSFB_PF_ARGB4444)
    	*size = px * 2;
    else
    if(pf == MMSFB_PF_NV21)
    	*size = (px * 3) / 2;
    else
    if(pf == MMSFB_PF_AYUV)
    	*size = px * 4;

    return true;
}


bool MMSFBSurface::setFlipFlags(MMSFBSurfaceFlipFlags flags) {
	this->flipflags = flags;
	return true;
}



bool MMSFBSurface::clear(unsigned char r, unsigned char g,
                         unsigned char b, unsigned char a) {
    DFBResult   dfbres;

    D_DEBUG_AT( MMS_Surface, "clear( argb %02x %02x %02x %02x ) <- %dx%d\n",
                a, r, g, b, this->config.w, this->config.h );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

    if ((a < 0xff)&&(this->config.premultiplied)) {
    	// premultiplied surface, have to premultiply the color
        register int aa = a + 1;
        r = (aa * r) >> 8;
        g = (aa * g) >> 8;
        b = (aa * b) >> 8;
    }
    
    if (!this->is_sub_surface) {
	    /* save color and set it to given values */
//	    MMSFBColor mc = this->config.color;
//	    setColor(r,g,b,a);
	
	    /* clear surface */
	    if ((dfbres=this->dfbsurface->Clear(this->dfbsurface, r, g, b, a)) != DFB_OK) {
	        MMSFB_SetError(dfbres, "IDirectFBSurface::Clear() failed");
	        return false;
	    }
	
	    /* reset color */
//	    setColor(mc.r,mc.g,mc.b,mc.a);
    }
    else {
    	
#ifndef USE_DFB_SUBSURFACE
	    CLIPSUBSURFACE
#endif

	    /* clear surface */
	    this->dfbsurface->Clear(this->dfbsurface, r, g, b, a);

#ifndef USE_DFB_SUBSURFACE
		UNCLIPSUBSURFACE
#endif

    }

    return true;
}

bool MMSFBSurface::setColor(unsigned char r, unsigned char g,
                            unsigned char b, unsigned char a) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* set color */
#ifdef USE_DFB_SUBSURFACE
    if ((dfbres=this->dfbsurface->SetColor(this->dfbsurface, r, g, b, a)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::SetColor() failed");
        return false;
    }
#else
    if (!this->is_sub_surface) {
	    if ((dfbres=this->dfbsurface->SetColor(this->dfbsurface, r, g, b, a)) != DFB_OK) {
	        MMSFB_SetError(dfbres, "IDirectFBSurface::SetColor() failed");
	        return false;
	    }
    }
#endif

    /* save the color */
    this->config.color.r = r;
    this->config.color.g = g;
    this->config.color.b = b;
    this->config.color.a = a;

    // set the default drawing flags
    // reason a): if it is an PREMULTIPLIED surface, the given color has to
    //            premultiplied internally before using it
    // reason b): if an alpha value is specified, the next draw function
    //            should blend over the surface 
	this->setDrawingFlagsByAlpha(a);
    
    return true;
}

bool MMSFBSurface::getColor(MMSFBColor *color) {

    /* check if initialized */
    INITCHECK;

    /* return the color */
    *color = this->config.color;
    
    return true;
}

bool MMSFBSurface::setClip(DFBRegion *clip) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* set clip */
#ifdef USE_DFB_SUBSURFACE
    if ((dfbres=this->dfbsurface->SetClip(this->dfbsurface, clip)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::SetClip() failed");
        return false;
    }
#else
    if (!this->is_sub_surface) {
        if ((dfbres=this->dfbsurface->SetClip(this->dfbsurface, clip)) != DFB_OK) {
            MMSFB_SetError(dfbres, "IDirectFBSurface::SetClip() failed");
            return false;
        }
    }
#endif

    /* save the region */
    if (clip) {
    	this->config.clipped = true;
	    this->config.clip = *clip;
    }
    else
    	this->config.clipped = false;
    
    return true;
}

bool MMSFBSurface::getClip(DFBRegion *clip) {

	/* check if initialized */
    INITCHECK;

    /* return the clip region */
    if (this->config.clipped) {
    	*clip = this->config.clip;
    }
    else {
    	clip->x1 = 0;
    	clip->y1 = 0;
    	clip->x2 = this->config.w - 1;
    	clip->y2 = this->config.h - 1;
    }

	return true;
}


bool MMSFBSurface::setDrawingFlags(MMSFBSurfaceDrawingFlags flags) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* set the drawing flags */
#ifdef USE_DFB_SUBSURFACE
    if ((dfbres=this->dfbsurface->SetDrawingFlags(this->dfbsurface, (DFBSurfaceDrawingFlags) flags)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::SetDrawingFlags() failed");
        return false;
    }
#else
    if (!this->is_sub_surface) {
        if ((dfbres=this->dfbsurface->SetDrawingFlags(this->dfbsurface, (DFBSurfaceDrawingFlags) flags)) != DFB_OK) {
            MMSFB_SetError(dfbres, "IDirectFBSurface::SetDrawingFlags() failed");
            return false;
        }
    }
#endif

    /* save the flags */
    this->config.drawingflags = flags;    
    
    return true;
}



bool MMSFBSurface::drawLine(int x1, int y1, int x2, int y2) {
    DFBResult   dfbres;

    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* draw a line */
    if (!this->is_sub_surface) {
        if ((dfbres=this->dfbsurface->DrawLine(this->dfbsurface, x1, y1, x2, y2)) != DFB_OK) {
            MMSFB_SetError(dfbres, "IDirectFBSurface::DrawLine() failed");
            return false;
        }
    }
    else {

#ifndef USE_DFB_SUBSURFACE
	    CLIPSUBSURFACE

	    x1+=this->sub_surface_xoff;
	    y1+=this->sub_surface_yoff;
	    x2+=this->sub_surface_xoff;
	    y2+=this->sub_surface_yoff;

	    SETSUBSURFACE_DRAWINGFLAGS;
#endif

	    this->dfbsurface->DrawLine(this->dfbsurface, x1, y1, x2, y2);

#ifndef USE_DFB_SUBSURFACE
	    RESETSUBSURFACE_DRAWINGFLAGS;

        UNCLIPSUBSURFACE
#endif

    }
    
    return true;
}

bool MMSFBSurface::drawRectangle(int x, int y, int w, int h) {
    DFBResult   dfbres;

    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* draw rectangle */
    if (!this->is_sub_surface) {
	    if ((dfbres=this->dfbsurface->DrawRectangle(this->dfbsurface, x, y, w, h)) != DFB_OK) {
	        MMSFB_SetError(dfbres, "IDirectFBSurface::DrawRectangle() failed");
	        return false;
	    }
    }
    else {

#ifndef USE_DFB_SUBSURFACE
	    CLIPSUBSURFACE

	    x+=this->sub_surface_xoff;
	    y+=this->sub_surface_yoff;
	    
	    SETSUBSURFACE_DRAWINGFLAGS;
#endif

	    this->dfbsurface->DrawRectangle(this->dfbsurface, x, y, w, h);

#ifndef USE_DFB_SUBSURFACE
	    RESETSUBSURFACE_DRAWINGFLAGS;

        UNCLIPSUBSURFACE
#endif

    }
        
    return true;
}

bool MMSFBSurface::fillRectangle(int x, int y, int w, int h) {
    DFBResult   dfbres;

    D_DEBUG_AT( MMS_Surface, "fill( %d,%d - %dx%d ) <- %dx%d, %02x %02x %02x %02x\n",
                x, y, w, h, this->config.w, this->config.h,
                this->config.color.a, this->config.color.r, this->config.color.g, this->config.color.b );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

    /* fill rectangle */
    if (!this->is_sub_surface) {
	    if ((dfbres=this->dfbsurface->FillRectangle(this->dfbsurface, x, y, w, h)) != DFB_OK) {
	        MMSFB_SetError(dfbres, "IDirectFBSurface::FillRectangle() failed");
	        return false;
	    }
    }
    else {

#ifndef USE_DFB_SUBSURFACE
	    CLIPSUBSURFACE

	    x+=this->sub_surface_xoff;
	    y+=this->sub_surface_yoff;
	    
	    SETSUBSURFACE_DRAWINGFLAGS;
#endif

	    this->dfbsurface->FillRectangle(this->dfbsurface, x, y, w, h);

#ifndef USE_DFB_SUBSURFACE
	    RESETSUBSURFACE_DRAWINGFLAGS;

        UNCLIPSUBSURFACE
#endif

    }

    return true;
}

bool MMSFBSurface::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* draw triangle */
    drawLine(x1, y1, x2, y2);
    drawLine(x1, y1, x3, y3);
    drawLine(x2, y2, x3, y3);

    return true;
}

bool MMSFBSurface::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    DFBResult   dfbres;

    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* fill triangle */
    if (!this->is_sub_surface) {
	    if ((dfbres=this->dfbsurface->FillTriangle(this->dfbsurface, x1, y1, x2, y2, x3, y3)) != DFB_OK) {
	        MMSFB_SetError(dfbres, "IDirectFBSurface::FillTriangle() failed");
	        return false;
	    }
    }
    else {

#ifndef USE_DFB_SUBSURFACE
	    CLIPSUBSURFACE

	    x1+=this->sub_surface_xoff;
	    y1+=this->sub_surface_yoff;
	    x2+=this->sub_surface_xoff;
	    y2+=this->sub_surface_yoff;
	    x3+=this->sub_surface_xoff;
	    y3+=this->sub_surface_yoff;
	    
	    SETSUBSURFACE_DRAWINGFLAGS;
#endif

	    this->dfbsurface->FillTriangle(this->dfbsurface, x1, y1, x2, y2, x3, y3);

#ifndef USE_DFB_SUBSURFACE
	    RESETSUBSURFACE_DRAWINGFLAGS;

        UNCLIPSUBSURFACE
#endif

    }

    return true;
}

bool MMSFBSurface::drawCircle(int x, int y, int radius, int start_octant, int end_octant) {

    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* draw circle */
    if (end_octant < start_octant) end_octant = start_octant;
    if ((start_octant<=4)&&(end_octant>=3))
        drawLine(x, y + radius, x, y + radius);
    if ((start_octant==0)||(end_octant==7))
        drawLine(x, y - radius, x, y - radius);
    if ((start_octant<=2)&&(end_octant>=1))
        drawLine(x + radius, y, x + radius, y);
    if ((start_octant<=6)&&(end_octant>=5))
        drawLine(x - radius, y, x - radius, y);
    int mr = radius * radius;
    int mx = 1;
    int my = (int) (sqrt(mr - 1) + 0.5);

    while (mx < my) {
        if ((start_octant<=0)&&(end_octant>=0))
            drawLine(x + mx, y - my, x + mx, y - my); /* octant 0 */
        if ((start_octant<=1)&&(end_octant>=1))
            drawLine(x + my, y - mx, x + my, y - mx); /* octant 1 */
        if ((start_octant<=2)&&(end_octant>=2))
            drawLine(x + my, y + mx, x + my, y + mx); /* octant 2 */
        if ((start_octant<=3)&&(end_octant>=3))
            drawLine(x + mx, y + my, x + mx, y + my); /* octant 3 */
        if ((start_octant<=4)&&(end_octant>=4))
            drawLine(x - mx, y + my, x - mx, y + my); /* octant 4 */
        if ((start_octant<=5)&&(end_octant>=5))
            drawLine(x - my, y + mx, x - my, y + mx); /* octant 5 */
        if ((start_octant<=6)&&(end_octant>=6))
            drawLine(x - my, y - mx, x - my, y - mx); /* octant 6 */
        if ((start_octant<=7)&&(end_octant>=7))
            drawLine(x - mx, y - my, x - mx, y - my); /* octant 7 */

        mx++;
        my = (int) (sqrt(mr - mx*mx) + 0.5);
    }

    if (mx == my) {
        if ((start_octant<=3)&&(end_octant>=2))
            drawLine(x + mx, y + my, x + mx, y + my);
        if ((start_octant<=1)&&(end_octant>=0))
            drawLine(x + mx, y - my, x + mx, y - my);
        if ((start_octant<=5)&&(end_octant>=4))
            drawLine(x - mx, y + my, x - mx, y + my);
        if ((start_octant<=7)&&(end_octant>=6))
            drawLine(x - mx, y - my, x - mx, y - my);
    }

    return true;
}



bool MMSFBSurface::setBlittingFlags(MMSFBSurfaceBlittingFlags flags) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    if ((flags & DSBLIT_BLEND_ALPHACHANNEL)||(flags & DSBLIT_BLEND_COLORALPHA)) {
    	/* if we do alpha channel blitting, we have to change the default settings to become correct results */
    	if (this->config.alphachannel)
    		dfbsurface->SetSrcBlendFunction(dfbsurface,(DFBSurfaceBlendFunction)DSBF_ONE);
        else
    		dfbsurface->SetSrcBlendFunction(dfbsurface,(DFBSurfaceBlendFunction)DSBF_SRCALPHA);
    	dfbsurface->SetDstBlendFunction(dfbsurface,(DFBSurfaceBlendFunction)(DSBF_INVSRCALPHA));

    	if (flags & DSBLIT_BLEND_COLORALPHA)
             flags = (MMSFBSurfaceBlittingFlags)(flags | DSBLIT_SRC_PREMULTCOLOR);
    }

    /* set the blitting flags */
    if ((dfbres=this->dfbsurface->SetBlittingFlags(this->dfbsurface, (DFBSurfaceBlittingFlags) flags)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::SetBlittingFlags() failed");

        return false;
    }

    /* save the flags */
    this->config.blittingflags = flags;    

    return true;
}


void MMSFBSurface::eAB_blend_argb_to_argb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
										  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy) {

	// first time?
	if (firsttime_eAB_blend_argb_to_argb) {
		printf("DISKO: Using accelerated blend ARGB to ARGB.\n");
		firsttime_eAB_blend_argb_to_argb = false;
	}

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	if (dst_height - dy < sh)
		sh = dst_height - dy;
	
	unsigned int OLDDST = (*dst) + 1;
	unsigned int OLDSRC  = (*src) + 1;
	unsigned int *src_end = src + src_pitch_pix * sh;
	int src_pitch_diff = src_pitch_pix - sw;
	int dst_pitch_diff = dst_pitch_pix - sw;
	register unsigned int d;

	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;
		while (src < line_end) {
			// load pixel from memory and check if the previous pixel is the same
			register unsigned int SRC = *src;

			// is the source alpha channel 0x00 or 0xff?
			register unsigned int A = SRC >> 24;
			if (A == 0xff) {
				// source pixel is not transparent, copy it directly to the destination
				*dst = SRC;
			}
			else
			if (A) {
				// source alpha is > 0x00 and < 0xff  
				register unsigned int DST = *dst; 

				if ((DST==OLDDST)&&(SRC==OLDSRC)) {
					// same pixel, use the previous value
					*dst = d;
				    dst++;
				    src++;
					continue;
				}
				OLDDST = DST;
				OLDSRC = SRC;

				register unsigned int SA= 0x100 - A;
				unsigned int a = DST >> 24;
				unsigned int r = (DST << 8) >> 24;
				unsigned int g = (DST << 16) >> 24;
				unsigned int b = DST & 0xff;

				// invert src alpha
			    a = (SA * a) >> 8;
			    r = (SA * r) >> 8;
			    g = (SA * g) >> 8;
			    b = (SA * b) >> 8;

			    // add src to dst
			    a += A;
			    r += (SRC << 8) >> 24;
			    g += (SRC << 16) >> 24;
			    b += SRC & 0xff;
			    d =   ((a >> 8) ? 0xff000000 : (a << 24))
					| ((r >> 8) ? 0xff0000   : (r << 16))
					| ((g >> 8) ? 0xff00     : (g << 8))
			    	| ((b >> 8) ? 0xff 		 :  b);
				*dst = d;
			}
		    
		    dst++;
		    src++;
		}

		// go to the next line
		src+= src_pitch_diff; 
		dst+= dst_pitch_diff;
	}
}


void MMSFBSurface::eAB_blend_srcalpha_argb_to_argb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
										  		   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy,
										  		   unsigned char alpha) {

	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		eAB_blend_argb_to_argb(src, src_pitch, src_height, sx, sy, sw, sh,
							   dst, dst_pitch, dst_height, dx, dy);
		return;
	}
	
	// first time?
	if (firsttime_eAB_blend_srcalpha_argb_to_argb) {
		printf("DISKO: Using accelerated blend srcalpha ARGB to ARGB.\n");
		firsttime_eAB_blend_srcalpha_argb_to_argb = false;
	}

	// something to do?
	if (!alpha)
		// source should blitted full transparent, so leave destination as is
		return;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	if (dst_height - dy < sh)
		sh = dst_height - dy;
	
	unsigned int OLDDST = (*dst) + 1;
	unsigned int OLDSRC  = (*src) + 1;
	unsigned int *src_end = src + src_pitch_pix * sh;
	int src_pitch_diff = src_pitch_pix - sw;
	int dst_pitch_diff = dst_pitch_pix - sw;
	register unsigned int d;

	register unsigned int ALPHA = alpha;
	ALPHA++;
	
	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;
		while (src < line_end) {
			// load pixel from memory and check if the previous pixel is the same
			register unsigned int SRC = *src;

			// is the source alpha channel 0x00 or 0xff?
			register unsigned int A = SRC >> 24;
			if (A) {
				// source alpha is > 0x00 and < 0xff  
				register unsigned int DST = *dst; 

				if ((DST==OLDDST)&&(SRC==OLDSRC)) {
					// same pixel, use the previous value
					*dst = d;
				    dst++;
				    src++;
					continue;
				}
				OLDDST = DST;
				OLDSRC = SRC;

				// load source pixel and multiply it with given ALPHA
			    A = (ALPHA * A) >> 8;
				unsigned int sr = (ALPHA * (SRC & 0xff0000)) >> 24;
				unsigned int sg = (ALPHA * (SRC & 0xff00)) >> 16;
				unsigned int sb = (ALPHA * (SRC & 0xff)) >> 8;
				register unsigned int SA= 0x100 - A;
				
				unsigned int a = DST >> 24;
				unsigned int r = (DST << 8) >> 24;
				unsigned int g = (DST << 16) >> 24;
				unsigned int b = DST & 0xff;

				// invert src alpha
			    a = (SA * a) >> 8;
			    r = (SA * r) >> 8;
			    g = (SA * g) >> 8;
			    b = (SA * b) >> 8;

			    // add src to dst
			    a += A;
			    r += sr;
			    g += sg;
			    b += sb;
			    d =   ((a >> 8) ? 0xff000000 : (a << 24))
					| ((r >> 8) ? 0xff0000   : (r << 16))
					| ((g >> 8) ? 0xff00     : (g << 8))
			    	| ((b >> 8) ? 0xff 		 :  b);
				*dst = d;
			}
		    
		    dst++;
		    src++;
		}

		// go to the next line
		src+= src_pitch_diff; 
		dst+= dst_pitch_diff;
	}
}


void MMSFBSurface::eAB_rgb16_to_rgb16(unsigned short int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
									  unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_rgb16_to_rgb16) {
		printf("DISKO: Using accelerated copy RGB16 to RGB16.\n");
		firsttime_eAB_rgb16_to_rgb16 = false;
	}
	
	// prepare...
	int src_pitch_pix = src_pitch >> 1;
	int dst_pitch_pix = dst_pitch >> 1;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	if (dst_height - dy < sh)
		sh = dst_height - dy;
	
	unsigned short int *src_end = src + src_pitch_pix * sh;
	
	// for all lines
	while (src < src_end) {
		// copy the line
		memcpy(dst, src, sw << 1);

		// go to the next line
		src+= src_pitch_pix; 
		dst+= dst_pitch_pix;
	}
}



void MMSFBSurface::eAB_argb_to_rgb16(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
									 unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_argb_to_rgb16) {
		printf("DISKO: Using accelerated conversion ARGB to RGB16.\n");
		firsttime_eAB_argb_to_rgb16 = false;
	}
	
	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 1;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	if (dst_height - dy < sh)
		sh = dst_height - dy;
	
	unsigned int OLDSRC  = (*src) + 1;
	unsigned int *src_end = src + src_pitch_pix * sh;
	int src_pitch_diff = src_pitch_pix - sw;
	int dst_pitch_diff = dst_pitch_pix - sw;
	register unsigned short int d;
	
	
	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;
		while (src < line_end) {
			// load pixel from memory and check if the previous pixel is the same
			register unsigned int SRC  = *src;

			// same?
			if (SRC==OLDSRC) {
				// same pixel, use the previous value
				*dst = d;
			    dst++;
			    src++;
				continue;
			}
			OLDSRC = SRC;
			
			// copy source directly to the destination
			unsigned int r = (SRC << 8) >> 24;
			unsigned int g = (SRC << 16) >> 24;
			unsigned int b = SRC & 0xff;
		    d =    ((r >> 3) << 11)
		    	 | ((g >> 2) << 5)
		    	 | (b >> 3);
		    *dst = d;
		    
		    dst++;
		    src++;
		}

		// go to the next line
		src+= src_pitch_diff; 
		dst+= dst_pitch_diff;
	}
}


void MMSFBSurface::eAB_blend_argb_to_rgb16(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
										   unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_blend_argb_to_rgb16) {
		printf("DISKO: Using accelerated blend ARGB to RGB16.\n");
		firsttime_eAB_blend_argb_to_rgb16 = false;
	}
	
	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 1;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	if (dst_height - dy < sh)
		sh = dst_height - dy;
	
	unsigned short int OLDDST = (*dst) + 1;
	unsigned int OLDSRC  = (*src) + 1;
	unsigned int *src_end = src + src_pitch_pix * sh;
	int src_pitch_diff = src_pitch_pix - sw;
	int dst_pitch_diff = dst_pitch_pix - sw;
	register unsigned short int d;

	
	
	
	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;
		while (src < line_end) {
			// load pixel from memory and check if the previous pixel is the same
			register unsigned int SRC  = *src;

			// is the source alpha channel 0x00 or 0xff?
			register unsigned int A = SRC >> 24;
			if (A == 0xff) {
				// source pixel is not transparent, copy it directly to the destination
				unsigned int r = (SRC << 8) >> 24;
				unsigned int g = (SRC << 16) >> 24;
				unsigned int b = SRC & 0xff;
			    *dst =    ((r >> 3) << 11)
			    		| ((g >> 2) << 5)
			    		| (b >> 3);
			}
			else
			if (A) {
				// source alpha is > 0x00 and < 0xff  
				register unsigned short int DST = *dst;
				
				if ((DST==OLDDST)&&(SRC==OLDSRC)) {
					// same pixel, use the previous value
					*dst = d;
				    dst++;
				    src++;
					continue;
				}
				OLDDST = DST;
				OLDSRC = SRC;

				register unsigned int SA= 0x100 - A;
				unsigned int r = DST >> 11;
				unsigned int g = DST & 0x07e0;
				unsigned int b = DST & 0x1f;
				
				// invert src alpha
			    r = SA * r;
			    g = SA * g;
			    b = (SA * b) >> 5;

			    // add src to dst
			    r += (SRC & 0x00f80000) >> 11;
			    g += (SRC & 0xfc00) << 3;
			    b += SRC & 0xff;
			    d =   ((r & 0xffe000)   ? 0xf800 : ((r >> 8) << 11))
			    	| ((g & 0xfff80000) ? 0x07e0 : ((g >> 13) << 5))
			    	| ((b & 0xff00)     ? 0x1f 	 : (b >> 3));
				*dst = d;
			}
		    
		    dst++;
		    src++;
		}

		// go to the next line
		src+= src_pitch_diff; 
		dst+= dst_pitch_diff;
	}
}


void MMSFBSurface::eAB_blend_airgb_to_airgb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
										  	unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy) {

	// first time?
	if (firsttime_eAB_blend_airgb_to_airgb) {
		printf("DISKO: Using accelerated blend AiRGB to AiRGB.\n");
		firsttime_eAB_blend_airgb_to_airgb = false;
	}

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	if (dst_height - dy < sh)
		sh = dst_height - dy;
	
	unsigned int OLDDST = (*dst) + 1;
	unsigned int OLDSRC  = (*src) + 1;
	unsigned int *src_end = src + src_pitch_pix * sh;
	int src_pitch_diff = src_pitch_pix - sw;
	int dst_pitch_diff = dst_pitch_pix - sw;
	register unsigned int d;

	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;
		while (src < line_end) {
			// load pixel from memory and check if the previous pixel is the same
			register unsigned int SRC = *src;

			// is the source alpha channel 0x00 or 0xff?
			register unsigned int A = SRC >> 24;
			if (A == 0x00) {
				// source pixel is not transparent, copy it directly to the destination
				*dst = SRC;
			}
			else
			if (A < 0xff) {
				// source alpha is > 0x00 and < 0xff  
				register unsigned int DST = *dst; 

				if ((DST==OLDDST)&&(SRC==OLDSRC)) {
					// same pixel, use the previous value
					*dst = d;
				    dst++;
				    src++;
					continue;
				}
				OLDDST = DST;
				OLDSRC = SRC;

				unsigned int a = (DST >> 24);
				unsigned int r = (DST << 8) >> 24;
				unsigned int g = (DST << 16) >> 24;
				unsigned int b = DST & 0xff;

				// invert src alpha
			    a = (A * (0x100 - a)) >> 8;
			    r = (A * r) >> 8;
			    g = (A * g) >> 8;
			    b = (A * b) >> 8;

			    // add src to dst
			    a += 0x100 - A;
			    r += (SRC << 8) >> 24;
			    g += (SRC << 16) >> 24;
			    b += SRC & 0xff;
				d =	  ((r >> 8) ? 0xff0000   : (r << 16))
					| ((g >> 8) ? 0xff00     : (g << 8))
			    	| ((b >> 8) ? 0xff 		 :  b);
			    if (!(a >> 8)) d |= (0x100 - a) << 24;
				*dst = d;
			}
		    
		    dst++;
		    src++;
		}

		// go to the next line
		src+= src_pitch_diff; 
		dst+= dst_pitch_diff;
	}
}




void MMSFBSurface::eAB_blend_ayuv_to_ayuv(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
										  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy) {

	// first time?
	if (firsttime_eAB_blend_ayuv_to_ayuv) {
		printf("DISKO: Using accelerated blend AYUV to AYUV.\n");
		firsttime_eAB_blend_ayuv_to_ayuv = false;
	}

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	if (dst_height - dy < sh)
		sh = dst_height - dy;
	
	unsigned int OLDDST = (*dst) + 1;
	unsigned int OLDSRC  = (*src) + 1;
	unsigned int *src_end = src + src_pitch_pix * sh;
	int src_pitch_diff = src_pitch_pix - sw;
	int dst_pitch_diff = dst_pitch_pix - sw;
	register unsigned int d;

	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;
		while (src < line_end) {
			// load pixel from memory and check if the previous pixel is the same
			register unsigned int SRC = *src;

			// is the source alpha channel 0x00 or 0xff?
			register unsigned int A = SRC >> 24;
			if (A == 0xff) {
				// source pixel is not transparent, copy it directly to the destination
				*dst = SRC;
			}
			else
			if (A) {
				// source alpha is > 0x00 and < 0xff  
				register unsigned int DST = *dst; 

				if ((DST==OLDDST)&&(SRC==OLDSRC)) {
					// same pixel, use the previous value
					*dst = d;
				    dst++;
				    src++;
					continue;
				}
				OLDDST = DST;
				OLDSRC = SRC;

				// source alpha is > 0x00 and < 0xff  
				register unsigned int SA= 0x100 - A;
				unsigned int a = DST >> 24;
				unsigned int y = (DST << 8) >> 24;
				unsigned int u = (DST << 16) >> 24;
				unsigned int v = DST & 0xff;

				// invert src alpha
			    a = (SA * a) >> 8;
			    y = (SA * y) >> 8;
			    u = (SA * u) >> 8;
			    v = (SA * v) >> 8;

			    // add src to dst
			    a += A;
			    y += (SRC << 8) >> 24;
			    u += (A * ((SRC << 16) >> 24)) >> 8;
			    v += (A * (SRC & 0xff)) >> 8;
			    d =   ((a >> 8) ? 0xff000000 : (a << 24))
			    	| ((y >> 8) ? 0xff0000   : (y << 16))
			    	| ((u >> 8) ? 0xff00     : (u << 8))
			    	| ((v >> 8) ? 0xff 		 :  v);
				*dst = d;
			
			}
		    
		    dst++;
		    src++;
		}

		// go to the next line
		src+= src_pitch_diff; 
		dst+= dst_pitch_diff;
	}
}



bool MMSFBSurface::extendedLock(MMSFBSurface *src, void **src_ptr, int *src_pitch,
								MMSFBSurface *dst, void **dst_ptr, int *dst_pitch) {
	if (src->dfbsurface->Lock(src->dfbsurface, DSLF_READ, src_ptr, src_pitch) != DFB_OK)
		return false;
	if (dst->dfbsurface->Lock(dst->dfbsurface, DSLF_WRITE, dst_ptr, dst_pitch) != DFB_OK) {
		src->dfbsurface->Unlock(src->dfbsurface);
		return false;
	}
	return true;
}

void MMSFBSurface::extendedUnlock(MMSFBSurface *src, MMSFBSurface *dst) {
	src->dfbsurface->Unlock(src->dfbsurface);
	dst->dfbsurface->Unlock(dst->dfbsurface);
}


bool MMSFBSurface::extendedAccelBlit(MMSFBSurface *source, DFBRectangle *src_rect, int x, int y) {
	// extended acceleration on?
	if (!this->extendedaccel)
		return false;

	// premultiplied surface?
	if (!source->config.premultiplied)
		return false;
	
	// a few help and clipping values
	void *src_ptr, *dst_ptr;
	int  src_pitch, dst_pitch;
	int sx = src_rect->x;
	int sy = src_rect->y;
	int sw = src_rect->w;
	int sh = src_rect->h;
	DFBRegion clipreg;
#ifndef USE_DFB_SUBSURFACE
	if (!this->is_sub_surface) {
#endif
		// normal surface or dfb subsurface
		if (!this->config.clipped) {
			clipreg.x1 = 0;
			clipreg.y1 = 0;
			clipreg.x2 = this->config.w - 1;
			clipreg.y2 = this->config.h - 1;
		}
		else
			clipreg = this->config.clip;
#ifndef USE_DFB_SUBSURFACE
	}
	else {
		// subsurface
		if (!this->root_parent->config.clipped) {
			clipreg.x1 = 0;
			clipreg.y1 = 0;
			clipreg.x2 = this->root_parent->config.w - 1;
			clipreg.y2 = this->root_parent->config.h - 1;
		}
		else
			clipreg = this->root_parent->config.clip;
	}
#endif
	
	if (x < clipreg.x1) {
		// left outside
		sx+= clipreg.x1 - x;
		sw-= clipreg.x1 - x;
		if (sw <= 0)
			return true;
		x = clipreg.x1;
	}
	else
	if (x > clipreg.x2)
		// right outside
		return true;
	if (y < clipreg.y1) {
		// top outside
		sy+= clipreg.y1 - y;
		sh-= clipreg.y1 - y;
		if (sh <= 0)
			return true;
		y = clipreg.y1;
	}
	else
	if (y > clipreg.y2)
		// bottom outside
		return true;
	if (x + sw - 1 > clipreg.x2)
		// to width
		sw = clipreg.x2 - x + 1;
	if (y + sh - 1 > clipreg.y2)
		// to height
		sh = clipreg.y2 - y + 1;
	
	// checking pixelformats...
	if (source->config.pixelformat == MMSFB_PF_ARGB) {
		// source is ARGB
		if (this->config.pixelformat == MMSFB_PF_ARGB) {
			// destination is ARGB
			if (this->config.blittingflags == (MMSFBSurfaceBlittingFlags)DSBLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &src_ptr, &src_pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_argb_to_argb((unsigned int *)src_ptr, src_pitch, (!source->root_parent)?source->config.h:source->root_parent->config.h,
										   sx, sy, sw, sh,
										   (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										   x, y);
					extendedUnlock(source, this);
					return true;
				}
				
				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA|DSBLIT_SRC_PREMULTCOLOR))) {
				// blitting with alpha channel and coloralpha
				if (extendedLock(source, &src_ptr, &src_pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_srcalpha_argb_to_argb((unsigned int *)src_ptr, src_pitch, (!source->root_parent)?source->config.h:source->root_parent->config.h,
										   			sx, sy, sw, sh,
										   			(unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										   			x, y,
										   			this->config.color.a);
					extendedUnlock(source, this);
					return true;
				}
				
				return false;
			}

			// does not match
			return false;
		}
		else
		if (this->config.pixelformat == MMSFB_PF_RGB16) {
			// destination is RGB16 (RGB565)
			if (this->config.blittingflags == (MMSFBSurfaceBlittingFlags)DSBLIT_NOFX) {
				// convert without alpha channel
				if (extendedLock(source, &src_ptr, &src_pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_argb_to_rgb16((unsigned int *)src_ptr, src_pitch, (!source->root_parent)?source->config.h:source->root_parent->config.h,
									   sx, sy, sw, sh,
									   (unsigned short int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
									   x, y);
					extendedUnlock(source, this);
					return true;
				}
				
				return false;
			}
			else
			if (this->config.blittingflags == (MMSFBSurfaceBlittingFlags)DSBLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &src_ptr, &src_pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_argb_to_rgb16((unsigned int *)src_ptr, src_pitch, (!source->root_parent)?source->config.h:source->root_parent->config.h,
											sx, sy, sw, sh,
										    (unsigned short int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										    x, y);
					extendedUnlock(source, this);
					return true;
				}
				
				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA|DSBLIT_SRC_PREMULTCOLOR))) {
//				printf("fehlt noch-\n");
			}
			
			// does not match
			return false;
		}
			
		// does not match
		return false;
	}
	else
	if (source->config.pixelformat == MMSFB_PF_RGB16) {
		// source is RGB16 (RGB565)
		if (this->config.pixelformat == MMSFB_PF_RGB16) {
			// destination is RGB16 (RGB565)
			if (this->config.blittingflags == (MMSFBSurfaceBlittingFlags)DSBLIT_NOFX) {
				// blitting with alpha channel
				if (extendedLock(source, &src_ptr, &src_pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_rgb16_to_rgb16((unsigned short int *)src_ptr, src_pitch, (!source->root_parent)?source->config.h:source->root_parent->config.h,
									   sx, sy, sw, sh,
									   (unsigned short int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
									   x, y);
					extendedUnlock(source, this);
					return true;
				}
				
				return false;
			}

			// does not match
			return false;
		}

		// does not match
		return false;
	}
	else
	if (source->config.pixelformat == MMSFB_PF_AiRGB) {
		// source is AiRGB
		if (this->config.pixelformat == MMSFB_PF_AiRGB) {
			// destination is AiRGB
			if (this->config.blittingflags == (MMSFBSurfaceBlittingFlags)DSBLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &src_ptr, &src_pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_airgb_to_airgb((unsigned int *)src_ptr, src_pitch, (!source->root_parent)?source->config.h:source->root_parent->config.h,
										   	 sx, sy, sw, sh,
										     (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										     x, y);
					extendedUnlock(source, this);
					return true;
				}
				
				return false;
			}
			
			// does not match
			return false;
		}

		// does not match
		return false;
	}
	else
	if (source->config.pixelformat == MMSFB_PF_AYUV) {
		// source is AYUV
		if (this->config.pixelformat == MMSFB_PF_AYUV) {
			// destination is AYUV
			if (this->config.blittingflags == (MMSFBSurfaceBlittingFlags)DSBLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &src_ptr, &src_pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_ayuv_to_ayuv((unsigned int *)src_ptr, src_pitch, (!source->root_parent)?source->config.h:source->root_parent->config.h,
										   sx, sy, sw, sh,
										   (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										   x, y);
					extendedUnlock(source, this);
					return true;
				}
				
				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA|DSBLIT_SRC_PREMULTCOLOR))) {
//				printf("fehlt noch#2\n");
			}
			
			// does not match
			return false;
		}

		// does not match
		return false;
	}

	// does not match
	return false;
}


void MMSFBSurface::eASB_blend_argb_to_argb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
										   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh) {

	// first time?
	if (firsttime_eASB_blend_argb_to_argb) {
		printf("DISKO: Using accelerated stretch & blend ARGB to ARGB.\n");
		firsttime_eASB_blend_argb_to_argb = false;
	}

	// prepare...
	int  src_pitch_pix = src_pitch >> 2;
	int  dst_pitch_pix = dst_pitch >> 2;
	unsigned int *src_end = src + sx + src_pitch_pix * (sy + sh);
	if (src_end > src + src_pitch_pix * src_height)
		src_end = src + src_pitch_pix * src_height;
	unsigned int *dst_end = dst + dst_pitch_pix * dst_height;
	src+=sx + sy * src_pitch_pix;
	dst+=dx + dy * dst_pitch_pix;

	
//	printf("sw=%d,sh=%d\n", sw,sh);	
	
	int horifact = (dw<<16)/sw;
	int vertfact = (dh<<16)/sh;
	
	
	
	// for all lines
	int vertcnt = 0x8000;
	while ((src < src_end)&&(dst < dst_end)) {
		// for all pixels in the line
		vertcnt+=vertfact;
		if (vertcnt & 0xffff0000) {
			unsigned int *line_end = src + sw;
			unsigned int *old_dst = dst;
	
			do {
				int horicnt = 0x8000;
				while (src < line_end) {
					// load pixel from memory and check if the previous pixel is the same
		
					horicnt+=horifact;
					
					if (horicnt & 0xffff0000) {
						register unsigned int SRC  = *src;
						register unsigned int A = SRC >> 24;

						if (A == 0xff) {
							// source pixel is not transparent, copy it directly to the destination
							do {
								*dst = SRC;
								dst++;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}
						else
						if (!A) {
							// source pixel is full transparent, do not change the destination
							do {
								dst++;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}
						else
						{
							// source alpha is > 0x00 and < 0xff  
							register unsigned int SA= 0x100 - A;
							register unsigned int DST = *dst; 
							unsigned int OLDDST = DST + 1;
							register unsigned int d;
							
							do {
								if (DST==OLDDST) {
									// same pixel, use the previous value
									if (A) {
										// source has an alpha
										*dst = d;
									}
								    dst++;
								    DST = *dst;
									horicnt-=0x10000;
									continue;
								}
								OLDDST = DST;
								
								
								// extract destination
								unsigned int a = DST >> 24;
								unsigned int r = (DST << 8) >> 24;
								unsigned int g = (DST << 16) >> 24;
								unsigned int b = DST & 0xff;
	
								// invert src alpha
							    a = (SA * a) >> 8;
							    r = (SA * r) >> 8;
							    g = (SA * g) >> 8;
							    b = (SA * b) >> 8;
	
							    // add src to dst
							    a += A;
							    r += (SRC << 8) >> 24;
							    g += (SRC << 16) >> 24;
							    b += SRC & 0xff;
							    d =   ((a >> 8) ? 0xff000000 : (a << 24))
									| ((r >> 8) ? 0xff0000   : (r << 16))
									| ((g >> 8) ? 0xff00     : (g << 8))
							    	| ((b >> 8) ? 0xff 		 :  b);
								*dst = d;
								dst++;
								DST = *dst;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}
					}
				    
				    src++;
				}
				src-=sw;
				vertcnt-=0x10000;
				dst = old_dst +  dst_pitch/4;
				old_dst = dst;
			} while (vertcnt & 0xffff0000);
		}
		
		// next line
		src+=src_pitch/4;
	}
}




void MMSFBSurface::eASB_blend_srcalpha_argb_to_argb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
										   			unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
										   			unsigned char alpha) {

	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		eASB_blend_argb_to_argb(src, src_pitch, src_height, sx, sy, sw, sh,
							    dst, dst_pitch, dst_height, dx, dy, dw, dh);
		return;
	}
	
	// first time?
	if (firsttime_eASB_blend_srcalpha_argb_to_argb) {
		printf("DISKO: Using accelerated stretch & blend srcalpha ARGB to ARGB.\n");
		firsttime_eASB_blend_srcalpha_argb_to_argb = false;
	}

	// something to do?
	if (!alpha)
		// source should blitted full transparent, so leave destination as is
		return;
	
	// prepare...
	int  src_pitch_pix = src_pitch >> 2;
	int  dst_pitch_pix = dst_pitch >> 2;
	unsigned int *src_end = src + sx + src_pitch_pix * (sy + sh);
	if (src_end > src + src_pitch_pix * src_height)
		src_end = src + src_pitch_pix * src_height;
	unsigned int *dst_end = dst + dst_pitch_pix * dst_height;
	src+=sx + sy * src_pitch_pix;
	dst+=dx + dy * dst_pitch_pix;

	
//	printf("sw=%d,sh=%d\n", sw,sh);	
	
	int horifact = (dw<<16)/sw;
	int vertfact = (dh<<16)/sh;
	

	register unsigned int ALPHA = alpha;
	ALPHA++;

	
	// for all lines
	int vertcnt = 0x8000;
	while ((src < src_end)&&(dst < dst_end)) {
		// for all pixels in the line
		vertcnt+=vertfact;
		if (vertcnt & 0xffff0000) {
			unsigned int *line_end = src + sw;
			unsigned int *old_dst = dst;
	
			do {
				int horicnt = 0x8000;
				while (src < line_end) {
					// load pixel from memory and check if the previous pixel is the same
		
					horicnt+=horifact;
					
					if (horicnt & 0xffff0000) {
						register unsigned int SRC = *src;
						register unsigned int A = SRC >> 24;

						if (!A) {
							// source pixel is full transparent, do not change the destination
							do {
								dst++;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}
						else
						{
							// source alpha is > 0x00 and < 0xff  
							register unsigned int DST = *dst; 
							unsigned int OLDDST = DST + 1;
							register unsigned int d;

							// load source pixel and multiply it with given ALPHA
						    A = (ALPHA * A) >> 8;
							unsigned int sr = (ALPHA * (SRC & 0xff0000)) >> 24;
							unsigned int sg = (ALPHA * (SRC & 0xff00)) >> 16;
							unsigned int sb = (ALPHA * (SRC & 0xff)) >> 8;
							register unsigned int SA= 0x100 - A;
							
							do {
								if (DST==OLDDST) {
									// same pixel, use the previous value
									if (A) {
										// source has an alpha
										*dst = d;
									}
								    dst++;
								    DST = *dst;
									horicnt-=0x10000;
									continue;
								}
								OLDDST = DST;
								
								// extract destination
								unsigned int a = DST >> 24;
								unsigned int r = (DST << 8) >> 24;
								unsigned int g = (DST << 16) >> 24;
								unsigned int b = DST & 0xff;
								
								// invert src alpha
							    a = (SA * a) >> 8;
							    r = (SA * r) >> 8;
							    g = (SA * g) >> 8;
							    b = (SA * b) >> 8;
	
							    // add src to dst
							    a += A;
							    r += sr;
							    g += sg;
							    b += sb;
							    d =   ((a >> 8) ? 0xff000000 : (a << 24))
									| ((r >> 8) ? 0xff0000   : (r << 16))
									| ((g >> 8) ? 0xff00     : (g << 8))
							    	| ((b >> 8) ? 0xff 		 :  b);
								*dst = d;
								dst++;
								DST = *dst;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}
					}
				    
				    src++;
				}
				src-=sw;
				vertcnt-=0x10000;
				dst = old_dst +  dst_pitch/4;
				old_dst = dst;
			} while (vertcnt & 0xffff0000);
		}
		
		// next line
		src+=src_pitch/4;
	}
}


bool MMSFBSurface::extendedAccelStretchBlit(MMSFBSurface *source, DFBRectangle *src_rect, DFBRectangle *dest_rect) {
	// extended acceleration on?
	if (!this->extendedaccel)
		return false;

	// a few help and clipping values
	void *src_ptr, *dst_ptr;
	int  src_pitch, dst_pitch;
	int sx = src_rect->x;
	int sy = src_rect->y;
	int sw = src_rect->w;
	int sh = src_rect->h;
	int dx = dest_rect->x;
	int dy = dest_rect->y;
	int dw = dest_rect->w;
	int dh = dest_rect->h;
	int wf = (dw*1000) / sw;
	int hf = (dh*1000) / sh;
	
//printf("sx=%d,sy=%d,sw=%d,sh=%d,dx=%d,dy=%d,dw=%d,dh=%d\n", sx,sy,sw,sh,dx,dy,dw,dh);
	
	
	DFBRegion clipreg;
#ifndef USE_DFB_SUBSURFACE
	if (!this->is_sub_surface) {
#endif
		// normal surface or dfb subsurface
		if (!this->config.clipped) {
			clipreg.x1 = 0;
			clipreg.y1 = 0;
			clipreg.x2 = this->config.w - 1;
			clipreg.y2 = this->config.h - 1;
		}
		else
			clipreg = this->config.clip;
#ifndef USE_DFB_SUBSURFACE
	}
	else {
		// subsurface
		if (!this->root_parent->config.clipped) {
			clipreg.x1 = 0;
			clipreg.y1 = 0;
			clipreg.x2 = this->root_parent->config.w - 1;
			clipreg.y2 = this->root_parent->config.h - 1;
		}
		else
			clipreg = this->root_parent->config.clip;
	}
#endif

//printf("cx1=%d,cy1=%d,cx2=%d,cy2=%d\n", clipreg.x1,clipreg.y1,clipreg.x2,clipreg.y2);
	
	
	if (dx < clipreg.x1) {
		// left outside
		sx+= ((clipreg.x1 - dx)*1000) / wf;
/*		sw-= (clipreg.x1 - dx) / wf;
		if (sw <= 0)
			return true;*/
		dw-= clipreg.x1 - dx;
		if (dw <= 0)
			return true;
		sw = (dw*1000) / wf;
		dx = clipreg.x1;
	}
	else
	if (dx > clipreg.x2)
		// right outside
		return true;
	if (dy < clipreg.y1) {
		// top outside
		sy+= ((clipreg.y1 - dy)*1000) / hf;
/*		sh-= (clipreg.y1 - dy) / hf;
		if (sh <= 0)
			return true;*/
		dh-= clipreg.y1 - dy;
		if (dh <= 0)
			return true;
		sh = (dh*1000) / hf;
		dy = clipreg.y1;
	}
	else
	if (dy > clipreg.y2)
		// bottom outside
		return true;
	if (dx + dw - 1 > clipreg.x2) {
		// to width
		dw = clipreg.x2 - dx + 1;
		sw = (dw*1000) / wf;
	}
	if (dy + dh - 1 > clipreg.y2) {
		// to height
		dh = clipreg.y2 - dy + 1;
		sh = (dh*1000) / hf;
	}
	if (sw<=0) sw = 1;
	if (sh<=0) sh = 1;
	if (dw<=0) dw = 1;
	if (dh<=0) dh = 1;

//printf(">sx=%d,sy=%d,sw=%d,sh=%d,dx=%d,dy=%d,dw=%d,dh=%d\n", sx,sy,sw,sh,dx,dy,dw,dh);
	
//if (source->is_sub_surface) {
//	sx+=source->sub_surface_xoff;	
//	sy+=source->sub_surface_yoff;	
//}

//printf("!sx=%d,sy=%d,sw=%d,sh=%d,dx=%d,dy=%d,dw=%d,dh=%d\n", sx,sy,sw,sh,dx,dy,dw,dh);

//dy-=10;
	// checking pixelformats...
	if (source->config.pixelformat == MMSFB_PF_ARGB) {
		// source is ARGB
		if (this->config.pixelformat == MMSFB_PF_ARGB) {
			// destination is ARGB
			if (this->config.blittingflags == (MMSFBSurfaceBlittingFlags)DSBLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &src_ptr, &src_pitch, this, &dst_ptr, &dst_pitch)) {
					eASB_blend_argb_to_argb((unsigned int *)src_ptr, src_pitch, (!source->root_parent)?source->config.h:source->root_parent->config.h,
											sx, sy, sw, sh,
											(unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
											dx, dy, dw, dh);
					extendedUnlock(source, this);                        

					return true;
				}
				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA|DSBLIT_SRC_PREMULTCOLOR))) {
				// blitting with alpha channel and coloralpha
				if (extendedLock(source, &src_ptr, &src_pitch, this, &dst_ptr, &dst_pitch)) {
					eASB_blend_srcalpha_argb_to_argb((unsigned int *)src_ptr, src_pitch, (!source->root_parent)?source->config.h:source->root_parent->config.h,
													 sx, sy, sw, sh,
													 (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
													 dx, dy, dw, dh,
													 this->config.color.a);
					extendedUnlock(source, this);                        

					return true;
				}
				return false;
			}

			// does not match
			return false;
		}
			
		// does not match
		return false;
	}

	// does not match
	return false;
}


bool MMSFBSurface::blit(MMSFBSurface *source, DFBRectangle *src_rect, int x, int y) {
    DFBResult    dfbres;
    DFBRectangle srcr;

    if (src_rect) {
         srcr = *src_rect;
    }
    else {
         srcr.x = 0;
         srcr.y = 0;
         srcr.w = source->config.w;
         srcr.h = source->config.h;
    }

    D_DEBUG_AT( MMS_Surface, "blit( %d,%d - %dx%d -> %d,%d ) <- %dx%d\n",
                DFB_RECTANGLE_VALS(&srcr), x, y, this->config.w, this->config.h );

    MMSFB_TRACE();


    /* check if initialized */
    INITCHECK;

    /* blit */
    if (!this->is_sub_surface) {
    	if (!extendedAccelBlit(source, &srcr, x, y))
		    if ((dfbres=this->dfbsurface->Blit(this->dfbsurface, source->getDFBSurface(), &srcr, x, y)) != DFB_OK) {
		        MMSFB_SetError(dfbres, "IDirectFBSurface::Blit() failed, srcr="
		        		               +iToStr(srcr.x)+","+iToStr(srcr.y)+","+iToStr(srcr.w)+","+iToStr(srcr.h)
		        		               +", dst="+iToStr(x)+","+iToStr(y));
		        return false;
		    }
    }
    else {

#ifndef USE_DFB_SUBSURFACE
	    CLIPSUBSURFACE

	    x+=this->sub_surface_xoff;
	    y+=this->sub_surface_yoff;
	    
	    SETSUBSURFACE_BLITTINGFLAGS;
#endif

	    if (!extendedAccelBlit(source, &srcr, x, y))
    		this->dfbsurface->Blit(this->dfbsurface, source->getDFBSurface(), &srcr, x, y);
        
#ifndef USE_DFB_SUBSURFACE
	    RESETSUBSURFACE_BLITTINGFLAGS;

        UNCLIPSUBSURFACE
#endif

    }

    return true;
}


bool MMSFBSurface::stretchBlit(MMSFBSurface *source, DFBRectangle *src_rect, DFBRectangle *dest_rect) {
    DFBResult    dfbres;
    DFBRectangle src;
    DFBRectangle dst;
    bool         blit_done = false;

    if (src_rect) {
         src = *src_rect;
    }
    else {
         src.x = 0;
         src.y = 0;
         src.w = source->config.w;
         src.h = source->config.h;
    }

    if (dest_rect) {
         dst = *dest_rect;
    }
    else {
         dst.x = 0;
         dst.y = 0;
         dst.w = this->config.w;
         dst.h = this->config.h;
    }

    D_DEBUG_AT( MMS_Surface, "stretchBlit( %d,%d - %dx%d  ->  %d,%d - %dx%d ) <- %dx%d\n",
                DFB_RECTANGLE_VALS(&src), DFB_RECTANGLE_VALS(&dst), this->config.w, this->config.h );

    /* check if i can blit without stretching */    
    if (src.w == dst.w && src.h == dst.h)
        return blit(source, &src, dst.x, dst.y);


    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;


    if (this->config.blittingflags != DSBLIT_NOFX) {
	    /* stretch blit with blitting flags */

	    if (!this->is_sub_surface) {
	    	if (extendedAccelStretchBlit(source, &src, &dst))
	    		blit_done = true;
	    }
	    else {
	    	
#ifndef USE_DFB_SUBSURFACE
		    CLIPSUBSURFACE
	
		    dst.x+=this->sub_surface_xoff;
		    dst.y+=this->sub_surface_yoff;
		    
		    SETSUBSURFACE_BLITTINGFLAGS;
#endif

	    	if (extendedAccelStretchBlit(source, &src, &dst))
	    		blit_done = true;
	        
#ifndef USE_DFB_SUBSURFACE
		    RESETSUBSURFACE_BLITTINGFLAGS;
	
		    dst.x-=this->sub_surface_xoff;
		    dst.y-=this->sub_surface_yoff;

		    UNCLIPSUBSURFACE
#endif
	    }
	        
	    if (!blit_done) {
	    	/* we use a temporary surface to separate the stretchblit from the extra blitting functions */
	    	MMSFBSurface *tempsuf = mmsfbsurfacemanager->getTemporarySurface(dst.w, dst.h);
	    	
	    	if (tempsuf) {
		    	DFBRectangle temp;
		    	temp.x=0;
		    	temp.y=0;
		    	temp.w=dst.w;
		    	temp.h=dst.h;
	
		    	dfbres = DFB_OK;
	    		dfbres=tempsuf->getDFBSurface()->StretchBlit(tempsuf->getDFBSurface(), source->getDFBSurface(), &src, &temp);
		    	if (dfbres == DFB_OK) {
		    	    if (!this->is_sub_surface) {
		    	    	if (extendedAccelBlit(tempsuf, &temp, dst.x, dst.y))
			    			blit_done = true;
		    	    	else
			    		if ((dfbres=this->dfbsurface->Blit(this->dfbsurface, tempsuf->getDFBSurface(), &temp, dst.x, dst.y)) == DFB_OK)
			    			blit_done = true;
		    	    }
		    	    else {
	
#ifndef USE_DFB_SUBSURFACE
					    CLIPSUBSURFACE
				
					    dst.x+=this->sub_surface_xoff;
					    dst.y+=this->sub_surface_yoff;
					    
					    SETSUBSURFACE_BLITTINGFLAGS;
#endif
	
		    	    	if (!extendedAccelBlit(tempsuf, &temp, dst.x, dst.y))
		    	    		this->dfbsurface->Blit(this->dfbsurface, tempsuf->getDFBSurface(), &temp, dst.x, dst.y);
	        
#ifndef USE_DFB_SUBSURFACE
			    	    RESETSUBSURFACE_BLITTINGFLAGS;
				
					    dst.x+=this->sub_surface_xoff;
					    dst.y+=this->sub_surface_yoff;

					    UNCLIPSUBSURFACE
#endif
	
					    blit_done = true;
	
		    	    }
		    	}
		    	
		    	mmsfbsurfacemanager->releaseTemporarySurface(tempsuf);
	    	}
	    }
    }

    if (!blit_done) {
        /* normal stretch blit */
	    if (!this->is_sub_surface) {
	    	dfbres = DFB_OK;
	    	if (!extendedAccelStretchBlit(source, &src, &dst))
		        dfbres=this->dfbsurface->StretchBlit(this->dfbsurface, source->getDFBSurface(), &src, &dst);
	        if (dfbres != DFB_OK) {
	            MMSFB_SetError(dfbres, "IDirectFBSurface::StretchBlit() failed");
	            return false;
	        }
	    }
	    else {
	    	
#ifndef USE_DFB_SUBSURFACE
		    CLIPSUBSURFACE
	
		    dst.x+=this->sub_surface_xoff;
		    dst.y+=this->sub_surface_yoff;
		    
		    SETSUBSURFACE_BLITTINGFLAGS;
#endif

//printf("@sx=%d,sy=%d,sw=%d,sh=%d,dx=%d,dy=%d,dw=%d,dh=%d\n", src.x,src.y,src.w,src.h,dst.x,dst.y,dst.w,dst.h);
	    	if (!extendedAccelStretchBlit(source, &src, &dst))
	    		this->dfbsurface->StretchBlit(this->dfbsurface, source->getDFBSurface(), &src, &dst);
	        
#ifndef USE_DFB_SUBSURFACE
		    RESETSUBSURFACE_BLITTINGFLAGS;
	
		    dst.x+=this->sub_surface_xoff;
		    dst.y+=this->sub_surface_yoff;

		    UNCLIPSUBSURFACE
#endif
	    }
    }
    
    return true;
}

bool MMSFBSurface::flip(DFBRegion *region) {
    DFBResult   dfbres;

    if (region)
         D_DEBUG_AT( MMS_Surface, "flip( %d,%d - %dx%d ) <- %dx%d\n",
                     DFB_RECTANGLE_VALS_FROM_REGION(region), this->config.w, this->config.h );
    else
         D_DEBUG_AT( MMS_Surface, "flip( %d,%d - %dx%d ) <- %dx%d\n",
                     0, 0, this->config.w, this->config.h, this->config.w, this->config.h );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

#ifdef USE_DFB_WINMAN

    /* flip */
    if ((dfbres=this->dfbsurface->Flip(this->dfbsurface, region, this->flipflags)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::Flip() failed");

        return false;
    }

#endif

#ifdef USE_MMSFB_WINMAN

    if (!this->config.islayersurface) {
        /* flip */
	    if (!this->is_sub_surface) {
	        if ((dfbres=this->dfbsurface->Flip(this->dfbsurface, region, this->flipflags)) != DFB_OK) {
	            MMSFB_SetError(dfbres, "IDirectFBSurface::Flip() failed");
	
	            return false;
	        }
	    }
	    else {
#ifndef USE_DFB_SUBSURFACE
		    CLIPSUBSURFACE
	
		    DFBRegion myregion;
		    if (!region) {
		    	myregion.x1 = 0;
		    	myregion.y1 = 0;
		    	myregion.x2 = this->config.w - 1;
		    	myregion.y2 = this->config.h - 1;
		    }
		    else
		    	myregion = *region;

		    myregion.x1+=this->sub_surface_xoff;
		    myregion.y1+=this->sub_surface_yoff;
		    myregion.x2+=this->sub_surface_xoff;
		    myregion.y2+=this->sub_surface_yoff;

		    this->dfbsurface->Flip(this->dfbsurface, &myregion, this->flipflags);

#else
	        this->dfbsurface->Flip(this->dfbsurface, region, this->flipflags);
#endif

#ifndef USE_DFB_SUBSURFACE
	        UNCLIPSUBSURFACE
#endif
	    }
    }

    if (this->config.iswinsurface) {
        /* inform the window manager */
        mmsfbwindowmanager->flipSurface(this, region);
    }

#endif

    return true;
}


bool MMSFBSurface::createCopy(MMSFBSurface **dstsurface, int w, int h,
                              bool copycontent, bool withbackbuffer) {

    D_DEBUG_AT( MMS_Surface, "createCopy( %dx%d -> %dx%d, %s, %s )\n",
                this->config.w, this->config.h, w, h,
                copycontent ? "CONTENT COPY!" : "no content copy",
                withbackbuffer ? "BACK BUFFER!" : "no back buffer" );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

    if (this->is_sub_surface)
    	return false;

    *dstsurface = NULL;

    if (!w) w = config.w;
    if (!h) h = config.h;

    /* create new surface */
    if (!mmsfb->createSurface(dstsurface, w, h, this->config.pixelformat,
                             (withbackbuffer)?this->config.backbuffer:0,this->config.systemonly)) {
        if (*dstsurface)
            delete *dstsurface;
        *dstsurface = NULL;
        return false;
    }

    if (copycontent) {
        /* copy the content */
        DFBRectangle dstrect;
        dstrect.x = 0;
        dstrect.y = 0;
        dstrect.w = w;
        dstrect.h = h;
        (*dstsurface)->setDrawingFlags((MMSFBSurfaceDrawingFlags) DSDRAW_NOFX);
        (*dstsurface)->setBlittingFlags((MMSFBSurfaceBlittingFlags) DSBLIT_NOFX);
        (*dstsurface)->stretchBlit(this, NULL, &dstrect);
        if (withbackbuffer) {
            (*dstsurface)->flip();
        }
    }

    return true;
}

bool MMSFBSurface::resize(int w, int h) {

    D_DEBUG_AT( MMS_Surface, "resize( %dx%d -> %dx%d )\n",
                this->config.w, this->config.h, w, h );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

    if (!this->is_sub_surface) {
        /* normal surface */
	    lock();

	    /* create a copy */
	    MMSFBSurface *dstsurface;
	    createCopy(&dstsurface, w, h, true, true);
	
	    /* move the dfb pointers */
	    IDirectFBSurface *s;
	    s = this->dfbsurface;
	    this->dfbsurface = dstsurface->dfbsurface;
	    dstsurface->dfbsurface = s;
	
	    /* load the new configuration */
	    this->getConfiguration();
	    dstsurface->getConfiguration();
	
	    /* free dstsurface */
	    delete dstsurface;
	
	    unlock();
	    return true;
    }
    else  {
    	/* sub surface */
	    DFBRectangle rect = this->sub_surface_rect;
	    rect.w = w;
	    rect.h = h;
	    return setSubSurface(&rect);
	    
    }
}



void MMSFBSurface::modulateBrightness(DFBColor *color, unsigned char brightness) {

    /* full brightness? */
    if (brightness == 255) return;

    /* full darkness? */
    if (brightness == 0) {
        color->r = 0;
        color->g = 0;
        color->b = 0;
        return;
    }
    
    /* modulate the color */
    unsigned int bn = 100000 * (255-brightness);
    if (color->r > 0) {
        unsigned int i = (10000 * 255) / (unsigned int)color->r;
        color->r = (5+((10 * (unsigned int)color->r) - (bn / i))) / 10;
    }
    if (color->g > 0) {
        unsigned int i = (10000 * 255) / (unsigned int)color->g;
        color->g = (5+((10 * (unsigned int)color->g) - (bn / i))) / 10;
    }
    if (color->b > 0) {
        unsigned int i = (10000 * 255) / (unsigned int)color->b;
        color->b = (5+((10 * (unsigned int)color->b) - (bn / i))) / 10;
    }
}

void MMSFBSurface::modulateOpacity(DFBColor *color, unsigned char opacity) {

    /* full opacity? */
    if (opacity == 255) return;

    /* complete transparent? */
    if (opacity == 0) {
        color->a = 0;
        return;
    }
    
    /* modulate the alpha value */
    unsigned int bn = 100000 * (255-opacity);
    if (color->a > 0) {
        unsigned int i = (10000 * 255) / (unsigned int)color->a;
        color->a = (5+((10 * (unsigned int)color->a) - (bn / i))) / 10;
    }
}


bool MMSFBSurface::setBlittingFlagsByBrightnessAlphaAndOpacity(
                    unsigned char brightness, unsigned char alpha, unsigned char opacity) {
    DFBColor color;

    /* check if initialized */
    INITCHECK;

    /* modulate the opacity into the color */
    color.a = alpha;
    modulateOpacity(&color, opacity);

    /* set color for blitting */
    setColor(brightness, brightness, brightness, color.a);

    /* set blitting flags */
    if (brightness != 255) {
        if (color.a == 255)
            setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_COLORIZE|DSBLIT_BLEND_ALPHACHANNEL));
        else
            setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_COLORIZE|DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA));
    }
    else {
        if (color.a == 255)
            setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL));
        else
            setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA));
    }
    
    return true;
}

bool MMSFBSurface::setDrawingFlagsByAlpha(unsigned char alpha) {

    // check if initialized
    INITCHECK;

    // set the drawing flags
    if (this->config.premultiplied) {
    	// premultiplied surface, have to premultiply the color
	    if (alpha == 255)
	        setDrawingFlags((MMSFBSurfaceDrawingFlags)(DSDRAW_NOFX|DSDRAW_SRC_PREMULTIPLY));
	    else
	        setDrawingFlags((MMSFBSurfaceDrawingFlags)(DSDRAW_BLEND|DSDRAW_SRC_PREMULTIPLY));
    }
    else {
	    if (alpha == 255)
	        setDrawingFlags((MMSFBSurfaceDrawingFlags)DSDRAW_NOFX);
	    else
	        setDrawingFlags((MMSFBSurfaceDrawingFlags)DSDRAW_BLEND);
    }

    return true;
}


bool MMSFBSurface::setDrawingColorAndFlagsByBrightnessAndOpacity(
                        DFBColor color, unsigned char brightness, unsigned char opacity) {

    /* check if initialized */
    INITCHECK;

    /* modulate the brightness into the color */
    modulateBrightness(&color, brightness);

    /* modulate the opacity into the color */
    modulateOpacity(&color, opacity);

    /* set the color for drawing */
    setColor(color.r, color.g, color.b, color.a);

    /* set the drawing flags */
    setDrawingFlagsByAlpha(color.a);
    
    return true;
}


bool MMSFBSurface::setFont(IDirectFBFont *font) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* set font */
    if ((dfbres=this->dfbsurface->SetFont(this->dfbsurface, font)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::SetFont() failed");
        return false;
    }

    /* save the font */
    this->config.font = font;    

    return true;
}


bool MMSFBSurface::drawString(string text, int len, int x, int y) {
    DFBResult   dfbres;

    D_DEBUG_AT( MMS_Surface, "drawString( '%s', %d, %d,%d ) <- %dx%d\n",
                text.c_str(), len, x, y, this->config.w, this->config.h );

    MMSFB_TRACE();


    /* check if initialized */
    INITCHECK;

    /* draw a string */
    if (!this->is_sub_surface) {
	    if ((dfbres=this->dfbsurface->DrawString(this->dfbsurface, text.c_str(), len, x, y, DSTF_TOPLEFT)) != DFB_OK) {
	        MMSFB_SetError(dfbres, "IDirectFBSurface::DrawString() failed");
	
	        return false;
	    }
    }
    else {
#ifndef USE_DFB_SUBSURFACE
	    CLIPSUBSURFACE

	    x+=this->sub_surface_xoff;
	    y+=this->sub_surface_yoff;
	    
	    SETSUBSURFACE_DRAWINGFLAGS;
#endif

		this->dfbsurface->DrawString(this->dfbsurface, text.c_str(), len, x, y, DSTF_TOPLEFT);

#ifndef USE_DFB_SUBSURFACE
	    RESETSUBSURFACE_DRAWINGFLAGS;

        UNCLIPSUBSURFACE
#endif
    }

    return true;
}

void MMSFBSurface::lock() {
    if (this->Lock.trylock() == 0) {
        /* I have got the lock the first time */
        this->TID = pthread_self();
        this->Lock_cnt = 1;
    }
    else {
        if ((this->TID == pthread_self())&&(this->Lock_cnt > 0)) {
            /* I am the thread which has already locked this surface */
            this->Lock_cnt++;
        }
        else {
            /* another thread has already locked this surface, waiting for... */
            Lock.lock();
            this->TID = pthread_self();
            this->Lock_cnt = 1;
        }
    }
}

void MMSFBSurface::unlock() {
    if (this->TID != pthread_self())
        return;
    
    if(this->Lock_cnt==0) 
    	return;
    	
    this->Lock_cnt--;

    if (this->Lock_cnt == 0)
        Lock.unlock();
}


MMSFBSurface *MMSFBSurface::getSubSurface(DFBRectangle *rect) {
    IDirectFBSurface    *subsuf = NULL;
    MMSFBSurface 		*surface;

    /* check if initialized */
    INITCHECK;

#ifdef USE_DFB_SUBSURFACE
    /* get a sub surface */
    DFBResult dfbres;
    if ((dfbres=this->dfbsurface->GetSubSurface(this->dfbsurface, rect, &subsuf)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetSubSurface() failed");
        return false;
    }
#endif

    /* create a new surface instance */
    surface = new MMSFBSurface(subsuf, this, rect);
    if (!surface) {
    	if (subsuf)
    		subsuf->Release(subsuf);
        MMSFB_SetError(0, "cannot create new instance of MMSFBSurface");
        return NULL;
    }

    /* add to my list */
    this->children.push_back(surface);
    
    return surface;
}

bool MMSFBSurface::setSubSurface(DFBRectangle *rect) {

	/* check if initialized */
    INITCHECK;

    /* only sub surfaces can be moved */
    if (!this->is_sub_surface)
		return false;

    lock();

    if (memcmp(rect, &(this->sub_surface_rect), sizeof(this->sub_surface_rect)) == 0) {
    	/* nothing changed */
    	unlock();
    	return false;
    }

#ifdef USE_DFB_SUBSURFACE
    /* because dfb has no IDirectFBSurface::setSubSurface(), allocate a new and release the old one */
    DFBResult dfbres;
    IDirectFBSurface *subsuf = NULL;
    if ((dfbres=this->parent->dfbsurface->GetSubSurface(this->parent->dfbsurface, rect, &subsuf)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetSubSurface() failed");
        unlock();
        return false;
    }

    if (this->dfbsurface)
    	this->dfbsurface->Release(this->dfbsurface);

    this->dfbsurface = subsuf;

#endif

    this->sub_surface_rect = *rect;

#ifndef USE_DFB_SUBSURFACE

    getRealSubSurfacePos(NULL, true);
	
#endif
    
    unlock();

    return true;
}

bool MMSFBSurface::setSubSurface(DFBRegion *region) {
	DFBRectangle rect;
	
	if (!region)
		return false;
	
	rect.x = region->x1;
	rect.y = region->y1;
	rect.w = region->x2 - region->x1 + 1;
	rect.h = region->y2 - region->y1 + 1;

	return setSubSurface(&rect);
}

bool MMSFBSurface::moveTo(int x, int y) {
	DFBRectangle rect;

	rect = this->sub_surface_rect;
	rect.x = x;
	rect.y = y;

	return setSubSurface(&rect);
}

bool MMSFBSurface::move(int x, int y) {
	DFBRectangle rect;
	
	rect = this->sub_surface_rect;
	rect.x += x;
	rect.y += y;
	
	return setSubSurface(&rect);
}

