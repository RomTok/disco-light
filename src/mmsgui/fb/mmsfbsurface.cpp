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
#include <stdlib.h>

#ifdef __HAVE_XLIB__
#include <ft2build.h>
#include FT_GLYPH_H
#endif


#ifdef  __HAVE_DIRECTFB__
D_DEBUG_DOMAIN( MMS_Surface, "MMS/Surface", "MMS FB Surface" );
#endif

// static variables
bool MMSFBSurface::extendedaccel								= false;
MMSFBSurfaceAllocMethod MMSFBSurface::allocmethod				= MMSFBSurfaceAllocMethod_malloc;
bool MMSFBSurface::firsttime_eAB_argb_to_argb 					= true;
bool MMSFBSurface::firsttime_eAB_blend_argb_to_argb 			= true;
bool MMSFBSurface::firsttime_eAB_blend_srcalpha_argb_to_argb	= true;
bool MMSFBSurface::firsttime_eAB_blend_argb_to_airgb 			= true;
bool MMSFBSurface::firsttime_eAB_rgb16_to_rgb16				    = true;
bool MMSFBSurface::firsttime_eAB_argb_to_rgb16				    = true;
bool MMSFBSurface::firsttime_eAB_blend_argb_to_rgb16			= true;
bool MMSFBSurface::firsttime_eAB_airgb_to_airgb 				= true;
bool MMSFBSurface::firsttime_eAB_blend_airgb_to_airgb 			= true;
bool MMSFBSurface::firsttime_eAB_blend_srcalpha_airgb_to_airgb	= true;
bool MMSFBSurface::firsttime_eAB_airgb_to_rgb16				    = true;
bool MMSFBSurface::firsttime_eAB_blend_airgb_to_rgb16			= true;

bool MMSFBSurface::firsttime_eAB_ayuv_to_ayuv 					= true;
bool MMSFBSurface::firsttime_eAB_blend_ayuv_to_ayuv				= true;
bool MMSFBSurface::firsttime_eAB_blend_srcalpha_ayuv_to_ayuv	= true;
bool MMSFBSurface::firsttime_eAB_ayuv_to_rgb16				    = true;
bool MMSFBSurface::firsttime_eAB_blend_ayuv_to_rgb16			= true;

bool MMSFBSurface::firsttime_eAB_yv12_to_yv12 					= true;
bool MMSFBSurface::firsttime_eAB_blend_argb_to_yv12 			= true;
bool MMSFBSurface::firsttime_eAB_blend_srcalpha_argb_to_yv12	= true;
bool MMSFBSurface::firsttime_eAB_blend_ayuv_to_yv12 			= true;
bool MMSFBSurface::firsttime_eAB_blend_srcalpha_ayuv_to_yv12	= true;

bool MMSFBSurface::firsttime_eADB_blend_argb_to_yv12 			= true;

bool MMSFBSurface::firsttime_eASB_blend_argb_to_argb 			= true;
bool MMSFBSurface::firsttime_eASB_blend_srcalpha_argb_to_argb 	= true;
bool MMSFBSurface::firsttime_eASB_blend_airgb_to_airgb 			= true;
bool MMSFBSurface::firsttime_eASB_blend_srcalpha_airgb_to_airgb = true;

bool MMSFBSurface::firsttime_eASB_blend_ayuv_to_ayuv 			= true;
bool MMSFBSurface::firsttime_eASB_blend_srcalpha_ayuv_to_ayuv 	= true;
bool MMSFBSurface::firsttime_eASB_yv12_to_yv12 					= true;

bool MMSFBSurface::firsttime_eAFR_argb							= true;
bool MMSFBSurface::firsttime_eAFR_blend_argb					= true;
bool MMSFBSurface::firsttime_eAFR_rgb16							= true;
bool MMSFBSurface::firsttime_eAFR_ayuv							= true;
bool MMSFBSurface::firsttime_eAFR_blend_ayuv					= true;
bool MMSFBSurface::firsttime_eAFR_yv12							= true;

bool MMSFBSurface::firsttime_eADL_argb							= true;
bool MMSFBSurface::firsttime_eADL_blend_argb					= true;

#ifdef __HAVE_XLIB__
bool MMSFBSurface::firsttime_blend_text_to_argb					= true;
bool MMSFBSurface::firsttime_blend_text_srcalpha_to_argb		= true;
#endif



#define INITCHECK  if((!mmsfb->isInitialized())||(!this->llsurface)){MMSFB_SetError(0,"MMSFBSurface is not initialized");return false;}

#define CLIPSUBSURFACE \
	MMSFBRegion reg, tmp; \
	bool tmpset; \
	if (clipSubSurface(&reg, false, &tmp, &tmpset)) {

#define UNCLIPSUBSURFACE \
	clipSubSurface(NULL, false, &tmp, &tmpset); }

#define SETSUBSURFACE_DRAWINGFLAGS \
	MMSFBColor ccc = this->config.color; \
	this->llsurface->SetColor(this->llsurface, ccc.r, ccc.g, ccc.b, ccc.a); \
	this->llsurface->SetDrawingFlags(this->llsurface, getDFBSurfaceDrawingFlagsFromMMSFBDrawingFlags(this->config.drawingflags));

#define RESETSUBSURFACE_DRAWINGFLAGS \
    ccc = this->root_parent->config.color; \
    this->llsurface->SetColor(this->llsurface, ccc.r, ccc.g, ccc.b, ccc.a); \
    this->llsurface->SetDrawingFlags(this->llsurface, getDFBSurfaceDrawingFlagsFromMMSFBDrawingFlags(this->root_parent->config.drawingflags));

#define SETSUBSURFACE_BLITTINGFLAGS \
	MMSFBColor ccc = this->config.color; \
	this->llsurface->SetColor(this->llsurface, ccc.r, ccc.g, ccc.b, ccc.a); \
	this->llsurface->SetBlittingFlags(this->llsurface, getDFBSurfaceBlittingFlagsFromMMSFBBlittingFlags(this->config.blittingflags));

#define RESETSUBSURFACE_BLITTINGFLAGS \
    ccc = this->root_parent->config.color; \
    this->llsurface->SetColor(this->llsurface, ccc.r, ccc.g, ccc.b, ccc.a); \
    this->llsurface->SetBlittingFlags(this->llsurface, getDFBSurfaceBlittingFlagsFromMMSFBBlittingFlags(this->root_parent->config.blittingflags));



#define MMSFBSurface_RGB2Y(r,g,b) ((((66*r+129*g+25*b+128)>>8)+16) & 0xff)
#define MMSFBSurface_RGB2U(r,g,b) ((((-38*r-74*g+112*b+128)>>8)+128) & 0xff)
#define MMSFBSurface_RGB2V(r,g,b) ((((112*r-94*g-18*b+128)>>8)+128) & 0xff)

#define MMSFBSurface_PREPARE_YUV2RGB(y,u,v)  y=(int)y-16;u=(int)u-128;v=(int)v-128;
#define MMSFBSurface_PREPARE_YUVBLEND(y,u,v) y=(int)y-16;u=(int)u-128;v=(int)v-128;
#define MMSFBSurface_RESET_YUVBLEND(y,u,v) y=(int)y+16;u=(int)u+128;v=(int)v+128;
/*#define MMSFBSurface_YUV2R(y,u,v,r) if ((r=((298*(int)y+409*(int)v+128)>>8)&0xffff)>>8) r=0xff;
#define MMSFBSurface_YUV2G(y,u,v,g) if ((g=((298*(int)y-100*(int)u-208*(int)v+128)>>8)&0xffff)>>8) g=0xff;
#define MMSFBSurface_YUV2B(y,u,v,b) if ((b=((298*(int)y+516*(int)u+128)>>8)&0xffff)>>8) b=0xff;
#define MMSFBSurface_YUV2RX(y,u,v,r) if ((r=(298*(int)y+409*(int)v+128)&0xffff00)>>16) r=0xff00;
#define MMSFBSurface_YUV2GX(y,u,v,g) if ((g=(298*(int)y-100*(int)u-208*(int)v+128)&0xffff00)>>16) g=0xff00;
#define MMSFBSurface_YUV2BX(y,u,v,b) if ((b=(298*(int)y+516*(int)u+128)&0xffff00)>>16) b=0xff00;*/


#define MMSFBSurface_YUV2R(y,u,v,r) if ((r=(298*(int)y+409*(int)v+128+0x200)>>8)>>8) r=0xff;
#define MMSFBSurface_YUV2G(y,u,v,g) if ((g=(298*(int)y-100*(int)u-208*(int)v+128+0x200)>>8)>>8) g=0xff;
#define MMSFBSurface_YUV2B(y,u,v,b) if ((b=(298*(int)y+516*(int)u+128+0x200)>>8)>>8) b=0xff;
/*#define MMSFBSurface_YUV2RX(y,u,v,r) if ((r=(298*(int)y+409*(int)v+128+0x200)&0xffff00)>>16) r=0xff00;
#define MMSFBSurface_YUV2GX(y,u,v,g) if ((g=(298*(int)y-100*(int)u-208*(int)v+128+0x200)&0xffff00)>>16) g=0xff00;
#define MMSFBSurface_YUV2BX(y,u,v,b) if ((b=(298*(int)y+516*(int)u+128+0x200)&0xffff00)>>16) b=0xff00;*/
#define MMSFBSurface_YUV2RX(y,u,v,r) if ((r=298*(int)y+409*(int)v+128+0x200)>>16) r=0xff00;
#define MMSFBSurface_YUV2GX(y,u,v,g) if ((g=298*(int)y-100*(int)u-208*(int)v+128+0x200)>>16) g=0xff00;
#define MMSFBSurface_YUV2BX(y,u,v,b) if ((b=298*(int)y+516*(int)u+128+0x200)>>16) b=0xff00;


/*
int c=(int)D[0].YUV.y-16;
int d=(int)D[0].YUV.u-128;
int e=(int)D[0].YUV.v-128;

if ((D[0].RGB.r=((298*c+409*e+128)>>8)&0xffff)>0xff) D[0].RGB.r=0xff;
if ((D[0].RGB.g=((298*c-100*d-208*e+128)>>8)&0xffff)>0xff) D[0].RGB.g=0xff;
if ((D[0].RGB.b=((298*c+516*d+128)>>8)&0xffff)>0xff) D[0].RGB.b=0xff;
*/


MMSFBSurface::MMSFBSurface(int w, int h, MMSFBSurfacePixelFormat pixelformat, int backbuffer, bool systemonly) {
    // init me
    this->llsurface = NULL;
    this->surface_read_locked = false;
    this->surface_read_lock_cnt = 0;
    this->surface_write_locked = false;
    this->surface_write_lock_cnt = 0;
    this->surface_invert_lock = false;
    this->config.surface_buffer = new MMSFBSurfaceBuffer;
	this->config.surface_buffer->numbuffers = 0;
#ifdef __HAVE_XLIB__
    this->config.surface_buffer->xv_image[0] = NULL;
#endif
	this->use_own_alloc = (this->allocmethod != MMSFBSurfaceAllocMethod_dfb);

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
		// create surface description
		DFBSurfaceDescription   surface_desc;
		surface_desc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT);
		surface_desc.width = w;
		surface_desc.height = h;
		surface_desc.pixelformat = getDFBPixelFormatFromMMSFBPixelFormat(pixelformat);

		if (surface_desc.pixelformat==DSPF_UNKNOWN)
			surface_desc.flags = (DFBSurfaceDescriptionFlags)(surface_desc.flags & ~DSDESC_PIXELFORMAT);

		// we use premultiplied surfaces because of alphachannel blitting with better performance
		surface_desc.flags = (DFBSurfaceDescriptionFlags)(surface_desc.flags | DSDESC_CAPS);
		surface_desc.caps = DSCAPS_PREMULTIPLIED;

		switch (backbuffer) {
			case 1: // front + one back buffer (double)
				surface_desc.caps = (DFBSurfaceCapabilities)(surface_desc.caps | DSCAPS_DOUBLE);
				break;
			case 2: // front + two back buffer (triple)
				surface_desc.caps = (DFBSurfaceCapabilities)(surface_desc.caps | DSCAPS_TRIPLE);
				break;
		}

		// surface should stored in system memory only?
		if (systemonly)
			surface_desc.caps = (DFBSurfaceCapabilities)(surface_desc.caps | DSCAPS_SYSTEMONLY);

		// create the surface
		DFBResult			dfbres;
		IDirectFBSurface	*dfbsurface;
		if ((dfbres=mmsfb->dfb->CreateSurface(mmsfb->dfb, &surface_desc, &dfbsurface)) != DFB_OK) {
			this->llsurface = NULL;
			DEBUGMSG("MMSGUI", "ERROR");
			MMSFB_SetError(dfbres, "IDirectFB::CreateSurface(" + iToStr(w) + "x" + iToStr(h) + ") failed");
			return;
		}

		init(dfbsurface, NULL, NULL);
#endif
	}
	else {
		// setup surface attributes
		MMSFBSurfaceBuffer *sb = this->config.surface_buffer;
		this->config.w = sb->sbw = w;
		this->config.h = sb->sbh = h;
		sb->pixelformat = pixelformat;
		sb->alphachannel = isAlphaPixelFormat(sb->pixelformat);
		sb->premultiplied = true;
		sb->backbuffer = backbuffer;
		sb->systemonly = systemonly;

		// allocate my surface buffers
		sb->numbuffers = backbuffer + 1;
		if (sb->numbuffers > MMSFBSurfaceMaxBuffers) sb->numbuffers = MMSFBSurfaceMaxBuffers;
		sb->currbuffer_read = 0;
		if (sb->numbuffers > 1)
			// using backbuffer(s)
			sb->currbuffer_write = 1;
		else
			// using only a single buffer for read/write
			sb->currbuffer_write = 0;
		sb->pitch = calcPitch(w);
		for (int i = 0; i < sb->numbuffers; i++)
			sb->buffers[i] = malloc(calcSize(sb->pitch, sb->sbh));

		init((void*)1, NULL, NULL);
	}
}

MMSFBSurface::MMSFBSurface(void *llsurface,
	        		       MMSFBSurface *parent,
						   MMSFBRectangle *sub_surface_rect) {
    // init me
    if ((!parent)||(!this->use_own_alloc))
    	this->config.surface_buffer = new MMSFBSurfaceBuffer;
    else
    	this->config.surface_buffer = NULL;

   if (this->config.surface_buffer)
	   this->config.surface_buffer->numbuffers = 0;
#ifdef __HAVE_XLIB__
    if (this->config.surface_buffer)
    	this->config.surface_buffer->xv_image[0] = NULL;
#endif
	if (llsurface > (void *)1)
		this->use_own_alloc = false;
	else
		this->use_own_alloc = (this->allocmethod != MMSFBSurfaceAllocMethod_dfb);

	init(llsurface, parent, sub_surface_rect);
}

#ifdef __HAVE_XLIB__
MMSFBSurface::MMSFBSurface(int w, int h, MMSFBSurfacePixelFormat pixelformat, XvImage *xv_image1, XvImage *xv_image2) {
    // init me
    this->llsurface = NULL;
    this->surface_read_locked = false;
    this->surface_read_lock_cnt = 0;
    this->surface_write_locked = false;
    this->surface_write_lock_cnt = 0;
    this->surface_invert_lock = false;
	this->use_own_alloc = true;

    // setup surface attributes
	this->config.surface_buffer = new MMSFBSurfaceBuffer;
	MMSFBSurfaceBuffer *sb = this->config.surface_buffer;
	this->config.w = sb->sbw = w;
	this->config.h = sb->sbh = h;
	sb->pixelformat = pixelformat;
	sb->alphachannel = isAlphaPixelFormat(sb->pixelformat);
	sb->premultiplied = true;
	sb->backbuffer = true;
	sb->systemonly = true;

	// set the surface buffer
	sb->numbuffers = 2;
	sb->xv_image[0] = xv_image1;
	sb->buffers[0] = sb->xv_image[0]->data;
	sb->xv_image[1] = xv_image2;
	sb->buffers[1] = sb->xv_image[1]->data;
	sb->currbuffer_read = 0;
	sb->currbuffer_write = 1;
	sb->pitch = *(sb->xv_image[0]->pitches);
	init((void*)1, NULL, NULL);
}
#endif

MMSFBSurface::~MMSFBSurface() {

    if (!mmsfb->isInitialized()) return;

    // release memory - only if not the layer surface
    if (this->llsurface)
        if (!this->config.islayersurface)
        	if (!this->is_sub_surface) {
#ifndef USE_DFB_SUBSURFACE
        		// delete all sub surfaces
    			deleteSubSurface(NULL);
#endif
        		mmsfbsurfacemanager->releaseSurface(this);
        	}
        	else {
#ifdef USE_DFB_SUBSURFACE
        		this->llsurface->Release(this->llsurface);
#endif

        		if (this->parent)
        			this->parent->deleteSubSurface(this);
        	}
}


void MMSFBSurface::init(void *llsurface,
	        		    MMSFBSurface *parent,
						MMSFBRectangle *sub_surface_rect) {
    /* init me */
#ifdef  __HAVE_DIRECTFB__
    this->llsurface = (IDirectFBSurface*)llsurface;
#else
    this->llsurface = llsurface;
#endif
    this->surface_read_locked = false;
    this->surface_read_lock_cnt = 0;
    this->surface_write_locked = false;
    this->surface_write_lock_cnt = 0;
    this->surface_invert_lock = false;
    this->flipflags = MMSFB_FLIP_NONE;
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

		this->config.surface_buffer = this->root_parent->config.surface_buffer;

#ifndef USE_DFB_SUBSURFACE

    	this->llsurface = this->root_parent->llsurface;

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
    if (this->llsurface) {
        getConfiguration();

        /* init the color */
        this->config.color.r = 0;
        this->config.color.g = 0;
        this->config.color.b = 0;
        this->config.color.a = 0;
        this->config.clipped = false;
        this->config.iswinsurface = false;
        this->config.islayersurface = false;
        this->config.drawingflags = MMSFB_DRAW_NOFX;
        this->config.blittingflags = MMSFB_BLIT_NOFX;
        this->config.font = NULL;
    }
}



bool MMSFBSurface::isInitialized() {
	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
		return (this->llsurface != NULL);
#else
		return false;
#endif
	}
	else {
		//TODO
		return true;
	}
}

void MMSFBSurface::freeSurfaceBuffer() {

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
		if (this->llsurface > (IDirectFBSurface*)1)
			this->llsurface->Release(this->llsurface);
#endif
	}
	else {
#ifdef __HAVE_XLIB__
		//free my surface buffers
		MMSFBSurfaceBuffer *sb = this->config.surface_buffer;
		if (!sb->xv_image[0]) {
			if (!this->is_sub_surface) {
				for (int i = 0; i < sb->numbuffers; i++)
					if (sb->buffers[i]) {
						free(sb->buffers[i]);
						sb->buffers[i] = NULL;
					}
				delete sb;
			}
		}
		sb->numbuffers = 0;
#endif
	}
	this->llsurface = NULL;
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

int MMSFBSurface::calcPitch(int width) {

	MMSFBSurfacePixelFormat pf = this->config.surface_buffer->pixelformat;
    int    pitch = width;

    if(pf == MMSFB_PF_ARGB1555)
    	pitch = width * 2;
    else
    if(pf == MMSFB_PF_RGB16)
    	pitch = width * 2;
    else
    if(pf == MMSFB_PF_RGB24)
    	pitch = width * 3;
    else
    if(pf == MMSFB_PF_RGB32)
    	pitch = width * 4;
    else
    if(pf == MMSFB_PF_ARGB)
    	pitch = width * 4;
    else
    if(pf == MMSFB_PF_A8)
    	pitch = width;
    else
    if(pf == MMSFB_PF_YUY2)
    	pitch = width * 2;
    else
    if(pf == MMSFB_PF_RGB332)
    	pitch = width;
    else
    if(pf == MMSFB_PF_UYVY)
    	pitch = width * 2;
    else
    if(pf == MMSFB_PF_I420)
    	pitch = width;
    else
    if(pf == MMSFB_PF_YV12)
    	pitch = width;
    else
    if(pf == MMSFB_PF_LUT8)
    	pitch = width;
    else
    if(pf == MMSFB_PF_ALUT44)
    	pitch = width;
    else
    if(pf == MMSFB_PF_AiRGB)
    	pitch = width * 4;
    else
    if(pf == MMSFB_PF_A1)
    	pitch = width / 8;
    else
    if(pf == MMSFB_PF_NV12)
    	pitch = width;
    else
    if(pf == MMSFB_PF_NV16)
    	pitch = width;
    else
    if(pf == MMSFB_PF_ARGB2554)
    	pitch = width * 2;
    else
    if(pf == MMSFB_PF_ARGB4444)
    	pitch = width * 2;
    else
    if(pf == MMSFB_PF_NV21)
    	pitch = width;
    else
    if(pf == MMSFB_PF_AYUV)
    	pitch = width * 4;

    if (pitch <= 0) pitch = 1;
    if (pitch % 4)
    	pitch += 4 - pitch % 4;

    return pitch;
}

int MMSFBSurface::calcSize(int pitch, int height) {

	MMSFBSurfacePixelFormat pf = this->config.surface_buffer->pixelformat;
    int    size = pitch * height;

    if(pf == MMSFB_PF_I420)
    	size += pitch + (size / 2);
    else
    if(pf == MMSFB_PF_YV12)
    	size += pitch + (size / 2);

    return size;
}

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


bool MMSFBSurface::clipSubSurface(MMSFBRegion *region, bool regionset, MMSFBRegion *tmp, bool *tmpset) {
	MMSFBRegion myregion;

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

void *MMSFBSurface::getDFBSurface() {
#ifdef  __HAVE_DIRECTFB__
	if (!this->use_own_alloc)
		return this->llsurface;
#endif
	return NULL;
}

bool MMSFBSurface::getConfiguration(MMSFBSurfaceConfig *config) {

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
		DFBSurfaceCapabilities  caps;
		DFBResult               dfbres;
		DFBSurfacePixelFormat   mypf;

		/* get size */
		if (!this->is_sub_surface) {
			if ((dfbres=this->llsurface->GetSize(this->llsurface, &(this->config.w), &(this->config.h))) != DFB_OK) {
				MMSFB_SetError(dfbres, "IDirectFBSurface::GetSize() failed");
				return false;
			}
			this->config.surface_buffer->sbw = this->config.w;
			this->config.surface_buffer->sbh = this->config.h;
		}
		else {
#ifdef USE_DFB_SUBSURFACE
			if ((dfbres=this->llsurface->GetSize(this->llsurface, &(this->config.w), &(this->config.h))) != DFB_OK) {
				MMSFB_SetError(dfbres, "IDirectFBSurface::GetSize() failed");
				return false;
			}
#else
			this->config.w = this->sub_surface_rect.w;
			this->config.h = this->sub_surface_rect.h;
#endif
		}

		/* get pixelformat */
		if ((dfbres=this->llsurface->GetPixelFormat(this->llsurface, &mypf)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBSurface::GetPixelFormat() failed");
			return false;
		}

		/* build a format string */
		this->config.surface_buffer->pixelformat = getMMSFBPixelFormatFromDFBPixelFormat(mypf);
		this->config.surface_buffer->alphachannel = isAlphaPixelFormat(this->config.surface_buffer->pixelformat);

		/* get capabilities */
		if ((dfbres=this->llsurface->GetCapabilities(this->llsurface, &caps)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBSurface::GetCapabilities() failed");
			return false;
		}

	    /* is it a premultiplied surface? */
		this->config.surface_buffer->premultiplied = caps & DSCAPS_PREMULTIPLIED;

	    /* get the buffer mode */
		this->config.surface_buffer->backbuffer = 0;
	    if (caps & DSCAPS_DOUBLE)
	    	this->config.surface_buffer->backbuffer = 1;
	    else
	    if (caps & DSCAPS_TRIPLE)
	    	this->config.surface_buffer->backbuffer = 2;

	    /* system only? */
	    this->config.surface_buffer->systemonly = false;
	    if (caps & DSCAPS_SYSTEMONLY)
	    	this->config.surface_buffer->systemonly = true;

	    /* fill return config */
	    if (config)
	        *config = this->config;

	    /* log some infos */
	    if ((!config)&&(!this->is_sub_surface)) {
	    	DEBUGMSG("MMSGUI", "Surface properties:");

	    	DEBUGMSG("MMSGUI", " size:         " + iToStr(this->config.w) + "x" + iToStr(this->config.h));

		    if (this->config.surface_buffer->alphachannel)
		    	DEBUGMSG("MMSGUI", " pixelformat:  " + getMMSFBPixelFormatString(this->config.surface_buffer->pixelformat) + ",ALPHACHANNEL");
		    else
		    	DEBUGMSG("MMSGUI", " pixelformat:  " + getMMSFBPixelFormatString(this->config.surface_buffer->pixelformat));

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
#endif
	}
	else {
		/* get size */
		if (this->is_sub_surface) {
			this->config.w = this->sub_surface_rect.w;
			this->config.h = this->sub_surface_rect.h;
		}

		/* fill return config */
		if (config)
			*config = this->config;

		/* log some infos */
		if ((!config)&&(!this->is_sub_surface)) {
			DEBUGMSG("MMSGUI", "Surface properties:");

			DEBUGMSG("MMSGUI", " size:         " + iToStr(this->config.w) + "x" + iToStr(this->config.h));

			if (this->config.surface_buffer->alphachannel)
				DEBUGMSG("MMSGUI", " pixelformat:  " + getMMSFBPixelFormatString(this->config.surface_buffer->pixelformat) + ",ALPHACHANNEL");
			else
				DEBUGMSG("MMSGUI", " pixelformat:  " + getMMSFBPixelFormatString(this->config.surface_buffer->pixelformat));

			DEBUGMSG("MMSGUI", " capabilities:");

			if (this->config.surface_buffer->systemonly)
				DEBUGMSG("MMSGUI", "  SYSTEMONLY");
			if (this->config.surface_buffer->backbuffer == 1)
				DEBUGMSG("MMSGUI", "  DOUBLE");
			if (this->config.surface_buffer->backbuffer == 2)
				DEBUGMSG("MMSGUI", "  TRIPLE");
			if (this->config.surface_buffer->premultiplied)
				DEBUGMSG("MMSGUI", "  PREMULTIPLIED");
		}
	    return true;
	}
	return false;
}

void MMSFBSurface::setExtendedAcceleration(bool extendedaccel) {
	this->extendedaccel = extendedaccel;
}

bool MMSFBSurface::getExtendedAcceleration() {
	return this->extendedaccel;
}

void MMSFBSurface::setAllocMethod(MMSFBSurfaceAllocMethod allocmethod, bool reset) {
	this->allocmethod = allocmethod;
	if (reset) {
		if (this->use_own_alloc != (this->allocmethod != MMSFBSurfaceAllocMethod_dfb)) {
			this->use_own_alloc = (this->allocmethod != MMSFBSurfaceAllocMethod_dfb);
			getConfiguration();
		}
	}
	else
		if (this->allocmethod != MMSFBSurfaceAllocMethod_dfb)
			printf("DISKO: Using own surface memory management.\n");
}

MMSFBSurfaceAllocMethod MMSFBSurface::getAllocMethod() {
	return this->allocmethod;
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


bool MMSFBSurface::getPixelFormat(MMSFBSurfacePixelFormat *pixelformat) {

    /* check if initialized */
    INITCHECK;

    /* return the pixelformat */
    *pixelformat = this->config.surface_buffer->pixelformat;

    return true;
}

bool MMSFBSurface::getSize(int *w, int *h) {

    /* check if initialized */
    INITCHECK;

    /* return values */
    *w = this->config.w;
    *h = this->config.h;

    return true;
}

bool MMSFBSurface::getMemSize(int *size) {

	/* check if initialized */
    INITCHECK;

    /* init size */
    if (!size)
    	return false;
    *size = 0;

    MMSFBSurfacePixelFormat pf = this->config.surface_buffer->pixelformat;
    int    px = this->config.surface_buffer->pitch * this->config.h * (this->config.surface_buffer->backbuffer+1);

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


bool MMSFBSurface::setFlipFlags(MMSFBFlipFlags flags) {
	this->flipflags = flags;
	return true;
}



bool MMSFBSurface::clear(unsigned char r, unsigned char g,
                         unsigned char b, unsigned char a) {
    bool ret = false;

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult   dfbres;
	    D_DEBUG_AT( MMS_Surface, "clear( argb %02x %02x %02x %02x ) <- %dx%d\n",
	                a, r, g, b, this->config.surface_buffer->w, this->config.surface_buffer->h );
	    MMSFB_TRACE();

	    if ((a < 0xff)&&(this->config.surface_buffer->premultiplied)) {
			// premultiplied surface, have to premultiply the color
			register int aa = a + 1;
			r = (aa * r) >> 8;
			g = (aa * g) >> 8;
			b = (aa * b) >> 8;
		}

		if (!this->is_sub_surface) {
			/* clear surface */
			if ((dfbres=this->llsurface->Clear(this->llsurface, r, g, b, a)) != DFB_OK) {
				MMSFB_SetError(dfbres, "IDirectFBSurface::Clear() failed");
				return false;
			}
		}
		else {

#ifndef USE_DFB_SUBSURFACE
			CLIPSUBSURFACE
#endif

			/* clear surface */
			if (this->llsurface->Clear(this->llsurface, r, g, b, a) == DFB_OK)
				ret = true;

#ifndef USE_DFB_SUBSURFACE
			UNCLIPSUBSURFACE
#endif
		}
#endif
	}
	else {
		MMSFBColor *col = &this->config.color;
		MMSFBColor savedcol = *col;
		col->r = r;
		col->g = g;
		col->b = b;
		col->a = a;
		MMSFBDrawingFlags saveddf = this->config.drawingflags;
		this->config.drawingflags = MMSFB_DRAW_SRC_PREMULTIPLY;

		if (!this->is_sub_surface) {
			ret = extendedAccelFillRectangle(0, 0, this->config.w, this->config.h);
		}
		else {
			CLIPSUBSURFACE

			ret = extendedAccelFillRectangle(0, 0, this->config.w, this->config.h);
			UNCLIPSUBSURFACE
		}

		*col = savedcol;
		this->config.drawingflags = saveddf;
	}


    return ret;
}

bool MMSFBSurface::setColor(unsigned char r, unsigned char g,
                            unsigned char b, unsigned char a) {

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult   dfbres;

	    /* set color */
#ifdef USE_DFB_SUBSURFACE
		if ((dfbres=this->llsurface->SetColor(this->llsurface, r, g, b, a)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBSurface::SetColor() failed");
			return false;
		}
#else
		if (!this->is_sub_surface) {
			if ((dfbres=this->llsurface->SetColor(this->llsurface, r, g, b, a)) != DFB_OK) {
				MMSFB_SetError(dfbres, "IDirectFBSurface::SetColor() failed");
				return false;
			}
		}
#endif
#endif
	}

    /* save the color */
	MMSFBColor *col = &this->config.color;
	col->r = r;
	col->g = g;
	col->b = b;
	col->a = a;

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

bool MMSFBSurface::setClip(MMSFBRegion *clip) {

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult   dfbres;

	    /* set clip */
#ifdef USE_DFB_SUBSURFACE
		if ((dfbres=this->llsurface->SetClip(this->llsurface, (DFBRegion*)clip)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBSurface::SetClip() failed");
			return false;
		}
#else
		if (!this->is_sub_surface) {
			if ((dfbres=this->llsurface->SetClip(this->llsurface, (DFBRegion*)clip)) != DFB_OK) {
				MMSFB_SetError(dfbres, "IDirectFBSurface::SetClip() failed");
				return false;
			}
		}
#endif
#endif
	}

    /* save the region */
    if (clip) {
    	this->config.clipped = true;
	    this->config.clip = *clip;
    }
    else
    	this->config.clipped = false;

    return true;
}

bool MMSFBSurface::setClip(int x1, int y1, int x2, int y2) {
	MMSFBRegion clip;
	clip.x1=x1;
	clip.y1=y1;
	clip.x2=x2;
	clip.y2=y2;
	return setClip(&clip);
}

bool MMSFBSurface::getClip(MMSFBRegion *clip) {

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


bool MMSFBSurface::setDrawingFlags(MMSFBDrawingFlags flags) {

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult   dfbres;

	    /* set the drawing flags */
#ifdef USE_DFB_SUBSURFACE
		if ((dfbres=this->llsurface->SetDrawingFlags(this->llsurface, getDFBSurfaceDrawingFlagsFromMMSFBDrawingFlags(flags))) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBSurface::SetDrawingFlags() failed");
			return false;
		}
#else
		if (!this->is_sub_surface) {
			if ((dfbres=this->llsurface->SetDrawingFlags(this->llsurface, getDFBSurfaceDrawingFlagsFromMMSFBDrawingFlags(flags))) != DFB_OK) {
				MMSFB_SetError(dfbres, "IDirectFBSurface::SetDrawingFlags() failed");
				return false;
			}
		}
#endif
#endif
	}

    /* save the flags */
    this->config.drawingflags = flags;

    return true;
}



bool MMSFBSurface::drawLine(int x1, int y1, int x2, int y2) {
    bool ret = false;

    // check if initialized
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult   dfbres;
	    MMSFB_BREAK();

	    // draw a line
		if (!this->is_sub_surface) {
			if (!extendedAccelDrawLine(x1, y1, x2, y2))
				if ((dfbres=this->llsurface->DrawLine(this->llsurface, x1, y1, x2, y2)) != DFB_OK) {
					MMSFB_SetError(dfbres, "IDirectFBSurface::DrawLine() failed");
					return false;
				}
			ret = true;
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

			if (extendedAccelDrawLine(x1, y1, x2, y2))
				ret = true;
			else
				if (this->llsurface->DrawLine(this->llsurface, x1, y1, x2, y2) == DFB_OK)
					ret = true;

#ifndef USE_DFB_SUBSURFACE
			RESETSUBSURFACE_DRAWINGFLAGS;

			UNCLIPSUBSURFACE
#endif
		}

#endif
	}
	else {

		if (!this->is_sub_surface) {
			ret = extendedAccelDrawLine(x1, y1, x2, y2);
		}
		else {
			CLIPSUBSURFACE

			x1+=this->sub_surface_xoff;
			y1+=this->sub_surface_yoff;
			x2+=this->sub_surface_xoff;
			y2+=this->sub_surface_yoff;

			ret = extendedAccelDrawLine(x1, y1, x2, y2);

			UNCLIPSUBSURFACE
		}

	}

    return ret;
}

bool MMSFBSurface::drawRectangle(int x, int y, int w, int h) {
	bool ret = false;

    // check if initialized
    INITCHECK;
    if (w < 1 || h < 1)
    	return false;

    // draw lines...
    if (w==1)
    	ret = drawLine(x, y, x, y+h-1);
    else
    if (h==1)
    	ret = drawLine(x, y, x+w-1, y);
    else {
    	ret = drawLine(x, y, x+w-1, y);
    	ret = drawLine(x, y+h-1, x+w-1, y+h-1);
    	if (h>2) {
        	ret = drawLine(x, y+1, x, y+h-2);
        	ret = drawLine(x+w-1, y+1, x+w-1, y+h-2);
    	}
    }

    return ret;
}

bool MMSFBSurface::fillRectangle(int x, int y, int w, int h) {
    bool		ret = false;

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult   dfbres;
	    D_DEBUG_AT( MMS_Surface, "fill( %d,%d - %dx%d ) <- %dx%d, %02x %02x %02x %02x\n",
	                x, y, w, h, this->config.surface_buffer->w, this->config.surface_buffer->h,
	                this->config.color.a, this->config.color.r, this->config.color.g, this->config.color.b );
	    MMSFB_TRACE();

	    /* fill rectangle */
		if (!this->is_sub_surface) {
			if (!extendedAccelFillRectangle(x, y, w, h))
				if ((dfbres=this->llsurface->FillRectangle(this->llsurface, x, y, w, h)) != DFB_OK) {
					MMSFB_SetError(dfbres, "IDirectFBSurface::FillRectangle() failed");
					return false;
				}
			ret = true;
		}
		else {

#ifndef USE_DFB_SUBSURFACE
			CLIPSUBSURFACE

			x+=this->sub_surface_xoff;
			y+=this->sub_surface_yoff;

			SETSUBSURFACE_DRAWINGFLAGS;
#endif

			if (extendedAccelFillRectangle(x, y, w, h))
				ret = true;
			else
				if (this->llsurface->FillRectangle(this->llsurface, x, y, w, h) == DFB_OK)
					ret = true;

#ifndef USE_DFB_SUBSURFACE
			RESETSUBSURFACE_DRAWINGFLAGS;

			UNCLIPSUBSURFACE
#endif
		}
#endif
	}
	else {

		if (!this->is_sub_surface) {
			ret = extendedAccelFillRectangle(x, y, w, h);
		}
		else {
			CLIPSUBSURFACE

			x+=this->sub_surface_xoff;
			y+=this->sub_surface_yoff;

			ret = extendedAccelFillRectangle(x, y, w, h);

			UNCLIPSUBSURFACE
		}

	}

    return ret;
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

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult   dfbres;
	    MMSFB_BREAK();

	    /* fill triangle */
		if (!this->is_sub_surface) {
			if ((dfbres=this->llsurface->FillTriangle(this->llsurface, x1, y1, x2, y2, x3, y3)) != DFB_OK) {
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

			this->llsurface->FillTriangle(this->llsurface, x1, y1, x2, y2, x3, y3);

#ifndef USE_DFB_SUBSURFACE
			RESETSUBSURFACE_DRAWINGFLAGS;

			UNCLIPSUBSURFACE
#endif

		}
#endif
	}
	else {
		//TODO
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



bool MMSFBSurface::setBlittingFlags(MMSFBBlittingFlags flags) {

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult   dfbres;

	    if ((flags & MMSFB_BLIT_BLEND_ALPHACHANNEL)||(flags & MMSFB_BLIT_BLEND_COLORALPHA)) {
			/* if we do alpha channel blitting, we have to change the default settings to become correct results */
			if (this->config.surface_buffer->alphachannel)
				llsurface->SetSrcBlendFunction(llsurface,(DFBSurfaceBlendFunction)DSBF_ONE);
			else
				llsurface->SetSrcBlendFunction(llsurface,(DFBSurfaceBlendFunction)DSBF_SRCALPHA);
			llsurface->SetDstBlendFunction(llsurface,(DFBSurfaceBlendFunction)(DSBF_INVSRCALPHA));

			if (flags & MMSFB_BLIT_BLEND_COLORALPHA)
				 flags = (MMSFBBlittingFlags)(flags | MMSFB_BLIT_SRC_PREMULTCOLOR);
		}

		/* set the blitting flags */
		if ((dfbres=this->llsurface->SetBlittingFlags(this->llsurface, getDFBSurfaceBlittingFlagsFromMMSFBBlittingFlags(flags))) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBSurface::SetBlittingFlags() failed");

			return false;
		}
#endif
	}

    /* save the flags */
    this->config.blittingflags = flags;

    return true;
}


bool MMSFBSurface::extendedLock(MMSFBSurface *src, void **src_ptr, int *src_pitch,
								MMSFBSurface *dst, void **dst_ptr, int *dst_pitch) {
	if (src) {
		src->lock(MMSFB_LOCK_READ, src_ptr, src_pitch, false);
		if (!*src_ptr)
			return false;
	}
	if (dst) {
		dst->lock(MMSFB_LOCK_WRITE, dst_ptr, dst_pitch, false);
		if (!*dst_ptr) {
			if (src)
				src->unlock(false);
			return false;
		}
	}

	if (this->surface_invert_lock) {
		if (src_ptr && dst_ptr && src_pitch && dst_pitch) {
			void *t_ptr;
			int t_pitch;
			t_ptr = *src_ptr;
			t_pitch = *src_pitch;
			*src_ptr = *dst_ptr;
			*src_pitch = *dst_pitch;
			*dst_ptr = t_ptr;
			*dst_pitch = t_pitch;
		}
	}

	return true;
}

void MMSFBSurface::extendedUnlock(MMSFBSurface *src, MMSFBSurface *dst) {
	if (dst)
		dst->unlock(false);
	if (src)
		src->unlock(false);
}


bool MMSFBSurface::printMissingCombination(char *method, MMSFBSurface *source, MMSFBExternalSurfaceBuffer *extbuf,
										   MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height) {
#ifdef  __HAVE_DIRECTFB__
	// failed, check if it must not
	if ((!this->use_own_alloc)&&(!source->use_own_alloc))
		return false;
#endif

	// fatal error!!!
	// we use our own surface buffer handling, but we have not found a matching routine!!!
	// so print the missing combination and return true
	printf("DISKO: Missing following combination in method %s\n", method);
	if (source) {
		printf("  source type:             %s\n", (source->is_sub_surface)?"subsurface":"surface");
		printf("  source memory:           %s\n", (source->use_own_alloc)?"managed by disko":"managed by dfb");
		printf("  source pixelformat:      %s\n", getMMSFBPixelFormatString(source->config.surface_buffer->pixelformat).c_str());
		printf("  source premultiplied:    %s\n", (source->config.surface_buffer->premultiplied)?"yes":"no");
	}
	if (extbuf) {
		printf("  source type:             surface\n");
		printf("  source memory:           extern (0x%08x, pitch=%d)\n", (unsigned long)extbuf->ptr, extbuf->pitch);
		if (extbuf->ptr2) {
			printf("                                  (0x%08x, pitch=%d)\n",  (unsigned long)extbuf->ptr2, extbuf->pitch2);
			if (extbuf->ptr3)
				printf("                                  (0x%08x, pitch=%d)\n",  (unsigned long)extbuf->ptr3, extbuf->pitch3);
		}
		printf("  source memory:           extern (0x%08x, pitch=%d)\n",  (unsigned long)extbuf->ptr, extbuf->pitch);
		printf("  source pixelformat:      %s\n", getMMSFBPixelFormatString(src_pixelformat).c_str());
	}
	printf("  destination type:        %s\n", (this->is_sub_surface)?"subsurface":"surface");
	printf("  destination memory:      %s\n", (this->use_own_alloc)?"managed by disko":"managed by dfb");
	printf("  destination pixelformat: %s\n", getMMSFBPixelFormatString(this->config.surface_buffer->pixelformat).c_str());
	printf("  destination color:       r=%d, g=%d, b=%d, a=%d\n", this->config.color.r, this->config.color.g, this->config.color.b, this->config.color.a);
	if ((source)||(extbuf)) {
		printf("  blitting flags (%06x):", this->config.blittingflags);
		if (this->config.blittingflags == MMSFB_BLIT_NOFX)
			printf(" NOFX");
		if (this->config.blittingflags & MMSFB_BLIT_BLEND_ALPHACHANNEL)
			printf(" BLEND_ALPHACHANNEL");
		if (this->config.blittingflags & MMSFB_BLIT_BLEND_COLORALPHA)
			printf(" BLEND_COLORALPHA");
		if (this->config.blittingflags & MMSFB_BLIT_COLORIZE)
			printf(" COLORIZE");
		if (this->config.blittingflags & MMSFB_BLIT_SRC_PREMULTIPLY)
			printf(" SRC_PREMULTIPLY");
		printf("\n");
	}
	else {
		printf("  drawing flags (%06x): ", this->config.drawingflags);
		if (this->config.drawingflags == MMSFB_DRAW_NOFX)
			printf(" NOFX");
		if (this->config.drawingflags & MMSFB_DRAW_BLEND)
			printf(" BLEND");
		if (this->config.drawingflags & MMSFB_DRAW_SRC_PREMULTIPLY)
			printf(" SRC_PREMULTIPLY");
		printf("\n");
	}
	printf("*****\n");
	return true;
}




void MMSFBSurface::eAB_argb_to_argb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
								    unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_argb_to_argb) {
		printf("DISKO: Using accelerated copy ARGB to ARGB.\n");
		firsttime_eAB_argb_to_argb = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

	unsigned int *src_end = src + src_pitch_pix * sh;

	// for all lines
	while (src < src_end) {
		// copy the line
		memcpy(dst, src, sw << 2);

		// go to the next line
		src+= src_pitch_pix;
		dst+= dst_pitch_pix;
	}
}


void MMSFBSurface::eAB_blend_argb_to_argb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy) {

	// first time?
	if (firsttime_eAB_blend_argb_to_argb) {
		printf("DISKO: Using accelerated blend ARGB to ARGB.\n");
		firsttime_eAB_blend_argb_to_argb = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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


void MMSFBSurface::eAB_blend_srcalpha_argb_to_argb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										  		   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy,
										  		   unsigned char alpha) {

	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		eAB_blend_argb_to_argb(extbuf, src_height, sx, sy, sw, sh,
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

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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
				// source alpha is > 0x00 and <= 0xff
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


void MMSFBSurface::eAB_blend_argb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy) {

	// first time?
	if (firsttime_eAB_blend_argb_to_airgb) {
		printf("DISKO: Using accelerated blend ARGB to AiRGB.\n");
		firsttime_eAB_blend_argb_to_airgb = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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
				*dst = SRC & 0x00ffffff;
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
			    a = (SA * (0x100 - a)) >> 8;
			    r = (SA * r) >> 8;
			    g = (SA * g) >> 8;
			    b = (SA * b) >> 8;

			    // add src to dst
			    a += A;
			    r += (SRC << 8) >> 24;
			    g += (SRC << 16) >> 24;
			    b += SRC & 0xff;
				d =   ((r >> 8) ? 0xff0000   : (r << 16))
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



void MMSFBSurface::eAB_rgb16_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									  unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_rgb16_to_rgb16) {
		printf("DISKO: Using accelerated copy RGB16 to RGB16.\n");
		firsttime_eAB_rgb16_to_rgb16 = false;
	}

	// get the first source ptr/pitch
	unsigned short int *src = (unsigned short int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 1;
	int dst_pitch_pix = dst_pitch >> 1;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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



void MMSFBSurface::eAB_argb_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									 unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_argb_to_rgb16) {
		printf("DISKO: Using accelerated conversion ARGB to RGB16.\n");
		firsttime_eAB_argb_to_rgb16 = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 1;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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
			unsigned int r = (SRC << 8) >> 27;
			unsigned int g = (SRC << 16) >> 26;
			unsigned int b = (SRC & 0xff) >> 3;
			d =   (r << 11)
				| (g << 5)
				| b;
		    *dst = d;

		    dst++;
		    src++;
		}

		// go to the next line
		src+= src_pitch_diff;
		dst+= dst_pitch_diff;
	}
}


void MMSFBSurface::eAB_blend_argb_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										   unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_blend_argb_to_rgb16) {
		printf("DISKO: Using accelerated blend ARGB to RGB16.\n");
		firsttime_eAB_blend_argb_to_rgb16 = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 1;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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
				unsigned int r = (SRC << 8) >> 27;
				unsigned int g = (SRC << 16) >> 26;
				unsigned int b = (SRC & 0xff) >> 3;
			    *dst =    (r << 11)
			    		| (g << 5)
			    		| b;
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
			    r += (A*(SRC & 0xf80000)) >> 19;
				g += (A*(SRC & 0xfc00)) >> 5;
				b += (A*(SRC & 0xf8)) >> 8;
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


void MMSFBSurface::eAB_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_airgb_to_airgb) {
		printf("DISKO: Using accelerated copy AiRGB to AiRGB.\n");
		firsttime_eAB_airgb_to_airgb = false;
		firsttime_eAB_argb_to_argb = false;
	}

	// no difference to argb, use argb to argb routine
	eAB_argb_to_argb(extbuf, src_height, sx, sy, sw, sh,
					 dst, dst_pitch, dst_height, dx, dy);
}

void MMSFBSurface::eAB_blend_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										  	unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy) {

	// first time?
	if (firsttime_eAB_blend_airgb_to_airgb) {
		printf("DISKO: Using accelerated blend AiRGB to AiRGB.\n");
		firsttime_eAB_blend_airgb_to_airgb = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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


void MMSFBSurface::eAB_blend_srcalpha_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										  		     unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy,
										  		     unsigned char alpha) {

	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		eAB_blend_airgb_to_airgb(extbuf, src_height, sx, sy, sw, sh,
							     dst, dst_pitch, dst_height, dx, dy);
		return;
	}

	// first time?
	if (firsttime_eAB_blend_srcalpha_airgb_to_airgb) {
		printf("DISKO: Using accelerated blend srcalpha AiRGB to AiRGB.\n");
		firsttime_eAB_blend_srcalpha_airgb_to_airgb = false;
	}

	// something to do?
	if (!alpha)
		// source should blitted full transparent, so leave destination as is
		return;

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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
			if (A < 0xff) {
				// source alpha is >= 0x00 and < 0xff
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
			    A = 0x100 - ((ALPHA * (0x100 - A)) >> 8);
				unsigned int sr = (ALPHA * (SRC & 0xff0000)) >> 24;
				unsigned int sg = (ALPHA * (SRC & 0xff00)) >> 16;
				unsigned int sb = (ALPHA * (SRC & 0xff)) >> 8;

				unsigned int a = DST >> 24;
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
			    r += sr;
			    g += sg;
			    b += sb;
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


void MMSFBSurface::eAB_airgb_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									  unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_airgb_to_rgb16) {
		printf("DISKO: Using accelerated conversion AiRGB to RGB16.\n");
		firsttime_eAB_airgb_to_rgb16 = false;
		firsttime_eAB_argb_to_rgb16 = false;
	}

	// no difference to argb, use argb to rgb16 routine
	eAB_argb_to_rgb16(extbuf, src_height, sx, sy, sw, sh,
					  dst, dst_pitch, dst_height, dx, dy);
}

void MMSFBSurface::eAB_blend_airgb_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										    unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_blend_airgb_to_rgb16) {
		printf("DISKO: Using accelerated blend AiRGB to RGB16.\n");
		firsttime_eAB_blend_airgb_to_rgb16 = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 1;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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
			if (!A) {
				// source pixel is not transparent, copy it directly to the destination
				unsigned int r = (SRC << 8) >> 27;
				unsigned int g = (SRC << 16) >> 26;
				unsigned int b = (SRC & 0xff) >> 3;
			    *dst =    (r << 11)
			    		| (g << 5)
			    		| b;
			}
			else
			if (A < 0xff) {
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
			    r = A * r;
			    g = A * g;
			    b = (A * b) >> 5;

			    // add src to dst
			    r += (SA*(SRC & 0xf80000)) >> 19;
				g += (SA*(SRC & 0xfc00)) >> 5;
				b += (SA*(SRC & 0xf8)) >> 8;
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



void MMSFBSurface::eAB_ayuv_to_ayuv(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_ayuv_to_ayuv) {
		printf("DISKO: Using accelerated copy AYUV to AYUV.\n");
		firsttime_eAB_ayuv_to_ayuv = false;
		firsttime_eAB_argb_to_argb = false;
	}

	// no difference to argb, use argb to argb routine
	eAB_argb_to_argb(extbuf, src_height, sx, sy, sw, sh,
					 dst, dst_pitch, dst_height, dx, dy);
}


void MMSFBSurface::eAB_blend_ayuv_to_ayuv(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy) {

	// first time?
	if (firsttime_eAB_blend_ayuv_to_ayuv) {
		printf("DISKO: Using accelerated blend AYUV to AYUV.\n");
		firsttime_eAB_blend_ayuv_to_ayuv = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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
				register int SA= 0x100 - A;
				unsigned int a = DST >> 24;
				int y = (DST << 8) >> 24;
				int u = (DST << 16) >> 24;
				int v = DST & 0xff;

				// we have to move the 0 point of the coordinate system
				// this make it a little slower than ARGB to ARGB blending
				MMSFBSurface_PREPARE_YUVBLEND(y,u,v);

				// invert src alpha
			    a = (SA * a) >> 8;
			    y = (SA * y) >> 8;
			    u = (SA * u) >> 8;
			    v = (SA * v) >> 8;

			    // add src to dst
			    a += A;
			    y += (SRC << 8) >> 24;
			    u += (SRC << 16) >> 24;
			    v += SRC & 0xff;

			    // build destination pixel, have to check for negative values
				// this make it a little slower than ARGB to ARGB blending
			    d = ((a >> 8) ? 0xff000000 : (a << 24));
			    if (y > 0)
			    	d |= ((y >> 8) ? 0xff0000 : (y << 16));
			    if (u > 0)
			    	d |= ((u >> 8) ? 0xff00 : (u << 8));
			    if (v > 0)
			    	d |= ((v >> 8) ? 0xff : v);

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


void MMSFBSurface::eAB_blend_srcalpha_ayuv_to_ayuv(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										  		   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy,
										  		   unsigned char alpha) {

	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		eAB_blend_ayuv_to_ayuv(extbuf, src_height, sx, sy, sw, sh,
							   dst, dst_pitch, dst_height, dx, dy);
		return;
	}

	// first time?
	if (firsttime_eAB_blend_srcalpha_ayuv_to_ayuv) {
		printf("DISKO: Using accelerated blend srcalpha AYUV to AYUV.\n");
		firsttime_eAB_blend_srcalpha_ayuv_to_ayuv = false;
	}

	// something to do?
	if (!alpha)
		// source should blitted full transparent, so leave destination as is
		return;

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 2;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

	unsigned int OLDDST = (*dst) + 1;
	unsigned int OLDSRC  = (*src) + 1;
	unsigned int *src_end = src + src_pitch_pix * sh;
	int src_pitch_diff = src_pitch_pix - sw;
	int dst_pitch_diff = dst_pitch_pix - sw;
	register unsigned int d;

	register int ALPHA = alpha;
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

				// load source pixel
			    A = (ALPHA * A) >> 8;
				int sy = (SRC << 8) >> 24;
				int su = (SRC << 16) >> 24;
				int sv = SRC & 0xff;
				register int SA= 0x100 - A;

				// multiply source with given ALPHA
				// we have to move the 0 point of the coordinate system
				// this make it a little slower than ARGB to ARGB blending
				MMSFBSurface_PREPARE_YUVBLEND(sy,su,sv);
			    sy = (ALPHA * sy) >> 8;
			    su = (ALPHA * su) >> 8;
			    sv = (ALPHA * sv) >> 8;
				MMSFBSurface_RESET_YUVBLEND(sy,su,sv);

				// extract destination
				unsigned int a = DST >> 24;
				int y = (DST << 8) >> 24;
				int u = (DST << 16) >> 24;
				int v = DST & 0xff;

				// we have to move the 0 point of the coordinate system
				// this make it a little slower than ARGB to ARGB blending
				MMSFBSurface_PREPARE_YUVBLEND(y,u,v);

			    // add src to dst
			    a += A;
			    y += sy;
			    u += su;
			    v += sv;

			    // build destination pixel, have to check for negative values
				// this make it a little slower than ARGB to ARGB blending
			    d = ((a >> 8) ? 0xff000000 : (a << 24));
			    if (y > 0)
			    	d |= ((y >> 8) ? 0xff0000 : (y << 16));
			    if (u > 0)
			    	d |= ((u >> 8) ? 0xff00 : (u << 8));
			    if (v > 0)
			    	d |= ((v >> 8) ? 0xff : v);

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


void MMSFBSurface::eAB_ayuv_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									 unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_ayuv_to_rgb16) {
		printf("DISKO: Using accelerated conversion AYUV to RGB16.\n");
		firsttime_eAB_ayuv_to_rgb16 = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 1;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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
			unsigned int y = (SRC << 8) >> 24;
			unsigned int u = (SRC << 16) >> 24;
			unsigned int v = SRC & 0xff;
			unsigned int R;
			unsigned int G;
			unsigned int B;

			MMSFBSurface_PREPARE_YUV2RGB(y,u,v);
			MMSFBSurface_YUV2R(y,u,v,R);
			MMSFBSurface_YUV2G(y,u,v,G);
			MMSFBSurface_YUV2B(y,u,v,B);

			d =    ((R >> 3) << 11)
		    	 | ((G >> 2) << 5)
		    	 | (B >> 3);
		    *dst = d;

		    dst++;
		    src++;
		}

		// go to the next line
		src+= src_pitch_diff;
		dst+= dst_pitch_diff;
	}
}



void MMSFBSurface::eAB_blend_ayuv_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										   unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_blend_ayuv_to_rgb16) {
		printf("DISKO: Using accelerated blend AYUV to RGB16.\n");
		firsttime_eAB_blend_ayuv_to_rgb16 = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 1;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

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
				unsigned int y = (SRC << 8) >> 24;
				unsigned int u = (SRC << 16) >> 24;
				unsigned int v = SRC & 0xff;
				unsigned int R;
				unsigned int G;
				unsigned int B;

				MMSFBSurface_PREPARE_YUV2RGB(y,u,v);
				MMSFBSurface_YUV2R(y,u,v,R);
				MMSFBSurface_YUV2G(y,u,v,G);
				MMSFBSurface_YUV2B(y,u,v,B);

			    *dst =    ((R >> 3) << 11)
			    		| ((G >> 2) << 5)
			    		| (B >> 3);
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
				unsigned int r = (DST >> 11) << 3;
				unsigned int g = (DST & 0x07e0) >> 3;
				unsigned int b = (DST & 0x1f) << 3;

				// invert src alpha
			    r = SA * r;
			    g = SA * g;
			    b = SA * b;

				// convert source YUV to RGB colorspace
				unsigned int y = (SRC << 8) >> 24;
				unsigned int u = (SRC << 16) >> 24;
				unsigned int v = SRC & 0xff;
				unsigned int R;
				unsigned int G;
				unsigned int B;
				MMSFBSurface_PREPARE_YUV2RGB(y,u,v);
				MMSFBSurface_YUV2RX(y,u,v,R);
				MMSFBSurface_YUV2GX(y,u,v,G);
				MMSFBSurface_YUV2BX(y,u,v,B);

			    // add src to dst
			    r += (A*R) >> 8;
				g += (A*G) >> 8;
				b += (A*B) >> 8;
			    d =   ((r >> 16) ? 0xf800 : (r & 0xf800))
			    	| ((g >> 16) ? 0x07e0 : ((g >> 10) << 5))
			    	| ((b >> 16) ? 0x1f   : (b >> 11));
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



#define MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR \
	unsigned int  *saved_src   = src;   \
	unsigned char *saved_dst_y = dst_y; \
	unsigned char *saved_dst_u = dst_u; \
	unsigned char *saved_dst_v = dst_v;

#define MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR \
	src   = saved_src;   \
	dst_y = saved_dst_y; \
	dst_u = saved_dst_u; \
	dst_v = saved_dst_v;




#define MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src, dst_y, dst_u, dst_v, d_u, d_v) \
	SRC = src;											\
	A = SRC >> 24;										\
														\
	if (A == 0xff) {									\
		if (SRC!=OLDSRC) {								\
			int r = (SRC >> 16) & 0xff;					\
			int g = (SRC >> 8) & 0xff;					\
			int b = SRC  & 0xff;						\
			old_y = MMSFBSurface_RGB2Y(r,g,b);			\
			old_u = MMSFBSurface_RGB2U(r,g,b);			\
			old_v = MMSFBSurface_RGB2V(r,g,b);			\
			OLDSRC = SRC;								\
		}												\
		dst_y = old_y;									\
		d_u     old_u;									\
		d_v     old_v;									\
	}													\
	else												\
	if (!A) {											\
		d_u dst_u;										\
		d_v dst_v;										\
	}													\
	else												\
	{													\
		if (SRC!=OLDSRC) {								\
			int r = (SRC >> 16) & 0xff;					\
			int g = (SRC >> 8) & 0xff;					\
			int b = SRC  & 0xff;						\
			old_y = A * MMSFBSurface_RGB2Y(r,g,b);		\
			old_u = A * MMSFBSurface_RGB2U(r,g,b);		\
			old_v = A * MMSFBSurface_RGB2V(r,g,b);		\
			OLDSRC = SRC;								\
		}												\
														\
		register unsigned int SA = 0x100 - A;			\
		unsigned int y = dst_y;							\
		unsigned int u = dst_u;							\
		unsigned int v = dst_v;							\
														\
	    y = SA * y + old_y;								\
	    u = SA * u + old_u;								\
	    v = SA * v + old_v;								\
	    												\
	    dst_y = y >> 8;									\
		d_u     u >> 8;									\
		d_v     v >> 8;									\
	}






#define MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(src, dst_y, dst_u, dst_v, d_u, d_v) \
	SRC = src;											\
	A = SRC >> 24;										\
														\
	if (!A) {											\
		d_u dst_u;										\
		d_v dst_v;										\
	}													\
	else												\
	{													\
		if (SRC!=OLDSRC) {								\
			int r = (ALPHA * (SRC & 0xff0000)) >> 24;	\
			int g = (ALPHA * (SRC & 0xff00)) >> 16;		\
			int b = (ALPHA * (SRC & 0xff)) >> 8;		\
			old_y = A * MMSFBSurface_RGB2Y(r,g,b);		\
			A = (ALPHA * A) >> 8;						\
			old_u = A * MMSFBSurface_RGB2U(r,g,b);		\
			old_v = A * MMSFBSurface_RGB2V(r,g,b);		\
			OLDSRC = SRC;								\
		}												\
		else											\
			A = (ALPHA * A) >> 8;						\
														\
		register unsigned int SA = 0x100 - A;			\
		unsigned int y = dst_y;							\
		unsigned int u = dst_u;							\
		unsigned int v = dst_v;							\
														\
	    y = SA * y + old_y;								\
	    u = SA * u + old_u;								\
	    v = SA * v + old_v;								\
	    												\
	    dst_y = y >> 8;									\
		d_u     u >> 8;									\
		d_v     v >> 8;									\
	}


#define MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(src, dst_y, dst_u, dst_v, d_u, d_v) \
	SRC = src;											\
	A = SRC >> 24;										\
														\
	if (A == 0xff) {									\
		if (SRC!=OLDSRC) {								\
			old_y = (SRC >> 16) & 0xff;					\
			old_u = (SRC >> 8) & 0xff;					\
			old_v = SRC  & 0xff;						\
			OLDSRC = SRC;								\
		}												\
		dst_y = old_y;									\
		d_u     old_u;									\
		d_v     old_v;									\
	}													\
	else												\
	if (!A) {											\
		d_u dst_u;										\
		d_v dst_v;										\
	}													\
	else												\
	{													\
		if (SRC!=OLDSRC) {								\
			old_y = A * ((SRC >> 16) & 0xff);			\
			old_u = A * ((SRC >> 8) & 0xff);			\
			old_v = A * (SRC  & 0xff);					\
			OLDSRC = SRC;								\
		}												\
														\
		register unsigned int SA = 0x100 - A;			\
		unsigned int y = dst_y;							\
		unsigned int u = dst_u;							\
		unsigned int v = dst_v;							\
														\
	    y = SA * y + old_y;								\
	    u = SA * u + old_u;								\
	    v = SA * v + old_v;								\
	    												\
	    dst_y = y >> 8;									\
		d_u     u >> 8;									\
		d_v     v >> 8;									\
	}



#define MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(src, dst_y, dst_u, dst_v, d_u, d_v) \
	SRC = src;											\
	A = SRC >> 24;										\
														\
	if (!A) {											\
		d_u dst_u;										\
		d_v dst_v;										\
	}													\
	else												\
	{													\
		if (SRC!=OLDSRC) {								\
			int sy = (SRC << 8) >> 24;					\
			int su = (SRC << 16) >> 24;					\
			int sv = SRC & 0xff;						\
			MMSFBSurface_PREPARE_YUVBLEND(sy,su,sv);	\
			sy = (ALPHA * sy) >> 8;						\
			su = (ALPHA * su) >> 8;						\
			sv = (ALPHA * sv) >> 8;						\
			MMSFBSurface_RESET_YUVBLEND(sy,su,sv);		\
			old_y = A * sy;								\
			A = (ALPHA * A) >> 8;						\
			old_u = A * su;								\
			old_v = A * sv;								\
			OLDSRC = SRC;								\
		}												\
		else 											\
			A = (ALPHA * A) >> 8;						\
														\
		register unsigned int SA = 0x100 - A;			\
		unsigned int y = dst_y;							\
		unsigned int u = dst_u;							\
		unsigned int v = dst_v;							\
														\
	    y = SA * y + old_y;								\
	    u = SA * u + old_u;								\
	    v = SA * v + old_v;								\
	    												\
	    dst_y = y >> 8;									\
		d_u     u >> 8;									\
		d_v     v >> 8;									\
	}



void MMSFBSurface::eAB_yv12_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
								    unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	if (firsttime_eAB_yv12_to_yv12) {
		printf("DISKO: Using accelerated copy YV12 to YV12.\n");
		firsttime_eAB_yv12_to_yv12 = false;
	}

	// get the first source ptr/pitch
	unsigned char *src = (unsigned char *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix 		= src_pitch;
	int src_pitch_pix_half	= src_pitch_pix >> 1;
	int dst_pitch_pix 		= dst_pitch;
	int dst_pitch_pix_half	= dst_pitch_pix >> 1;

//	src+= sx + sy * src_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

//	unsigned int OLDSRC  = (*src) + 1;
//	unsigned int old_y;
//	unsigned int old_u;
//	unsigned int old_v;

	int  src_pixels = src_pitch_pix * sh;

	// check odd/even
	bool odd_left 	= (dx & 0x01);
	bool odd_top 	= (dy & 0x01);
	bool odd_right 	= ((dx + sw) & 0x01);
	bool odd_bottom = ((dy + sh) & 0x01);

	// pointer to the pixel components of the first pixel
	unsigned char *src_y = src + sx + sy * src_pitch_pix;
	unsigned char *src_u;
	unsigned char *src_v;
	if ((extbuf->ptr2)&&(extbuf->ptr3)) {
		src_u = (unsigned char *)extbuf->ptr2 + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
		src_v = (unsigned char *)extbuf->ptr3 + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
	}
	else {
		src_u = src + src_pitch_pix * (src_height + (src_height >> 2)) + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
		src_v = src + src_pitch_pix *  src_height                      + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
	}
	unsigned char *dst_y = dst + dx + dy * dst_pitch_pix;
	unsigned char *dst_u = dst + dst_pitch_pix * (dst_height + (dst_height >> 2)) + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	unsigned char *dst_v = dst + dst_pitch_pix *  dst_height                      + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;

	// offsets to the other three pixels
	unsigned int src_y2_offs = 1;
	unsigned int src_y3_offs = src_pitch_pix;
	unsigned int src_y4_offs = src_y3_offs + 1;
	unsigned int dst_y2_offs = 1;
	unsigned int dst_y3_offs = dst_pitch_pix;
	unsigned int dst_y4_offs = dst_y3_offs + 1;

	// arithmetic mean
	register unsigned int d_u;
	register unsigned int d_v;

	// draw odd pixels around the even rectangle
	if (odd_top && odd_left) {
/*		// odd top-left pixel
		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;*/
	}

	if (odd_top && odd_right) {
/*		// odd top-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom && odd_left) {
/*		// odd bottom-left pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom && odd_right) {
/*		// odd bottom-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_top) {
/*		// odd top line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom) {
/*		// odd bottom line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_left) {
/*		// odd left line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_right) {
/*		// odd right line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	// calc even positions...
	if (odd_top) {
		// odd top
		dy++;
		sh--;
		src_y+=src_pitch;
		src_pixels-=src_pitch_pix;
		dst_y+=dst_pitch;
		dst_u+=dst_pitch >> 1;
		dst_v+=dst_pitch >> 1;
	}

	if (odd_bottom) {
		// odd bottom
		src_height--;
		src_pixels-=src_pitch_pix;
	}

	if (odd_left) {
		// odd left
		dx++;
		sw--;
		src_y++;
		dst_y++;
		dst_u++;
		dst_v++;
	}

	if (odd_right) {
		// odd right
		sw--;
	}

	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
	unsigned char *src_end = src_y + src_pixels;
	int src_pitch_diff = src_pitch_pix << 1;
	int src_pitch_uvdiff = src_pitch_pix_half;
	int dst_pitch_diff = dst_pitch_pix << 1;
	int dst_pitch_uvdiff = dst_pitch_pix_half;

	// for all lines
	while (src_y < src_end) {
/*		// for all pixels in the line
		unsigned int *line_end = src + sw;

		// go through two lines in parallel (square 2x2 pixel)
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

		    // calculate the four pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u=, d_v=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src4_offs], dst_y[dst_y4_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			*dst_y = *src_y;

			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src_y+=2;
		    src_u++;
		    src_v++;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}
*/

		memcpy(dst_y, src_y, sw);
		memcpy(&dst_y[dst_y3_offs], &src_y[src_y3_offs], sw);
		memcpy(dst_u, src_u, sw >> 1);
		memcpy(dst_v, src_v, sw >> 1);

		// go to the next two lines
		src_y += src_pitch_diff;
		src_u += src_pitch_uvdiff;
		src_v += src_pitch_uvdiff;
		dst_y += dst_pitch_diff;
		dst_u += dst_pitch_uvdiff;
		dst_v += dst_pitch_uvdiff;
	}

}

void MMSFBSurface::eAB_blend_argb_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
				 			 			  unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy) {

	// first time?
	if (firsttime_eAB_blend_argb_to_yv12) {
		printf("DISKO: Using accelerated blend ARGB to YV12.\n");
		firsttime_eAB_blend_argb_to_yv12 = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int  src_pitch_pix 		= src_pitch >> 2;
	int dst_pitch_pix 		= dst_pitch;
	int dst_pitch_pix_half	= dst_pitch_pix >> 1;

	src+= sx + sy * src_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

	unsigned int OLDSRC  = (*src) + 1;
	unsigned int old_y;
	unsigned int old_u;
	unsigned int old_v;

	int  src_pixels = src_pitch_pix * sh;

	// check odd/even
	bool odd_left 	= (dx & 0x01);
	bool odd_top 	= (dy & 0x01);
	bool odd_right 	= ((dx + sw) & 0x01);
	bool odd_bottom = ((dy + sh) & 0x01);

	// pointer to the pixel components of the first pixel
	unsigned char *dst_y = dst + dx + dy * dst_pitch_pix;
	unsigned char *dst_u = dst + dst_pitch_pix * (dst_height + (dst_height >> 2)) + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	unsigned char *dst_v = dst + dst_pitch_pix *  dst_height                      + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;

	// offsets to the other three pixels
	unsigned int dst_y2_offs = 1;
	unsigned int dst_y3_offs = dst_pitch;
	unsigned int dst_y4_offs = dst_y3_offs + 1;
	unsigned int src2_offs = 1;
	unsigned int src3_offs = src_pitch_pix;
	unsigned int src4_offs = src3_offs + 1;

	// arithmetic mean
	register unsigned int d_u;
	register unsigned int d_v;

	// draw odd pixels around the even rectangle
	if (odd_top && odd_left) {
		// odd top-left pixel
		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;
	}

	if (odd_top && odd_right) {
		// odd top-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom && odd_left) {
		// odd bottom-left pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom && odd_right) {
		// odd bottom-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_top) {
		// odd top line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom) {
		// odd bottom line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_left) {
		// odd left line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_right) {
		// odd right line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	// calc even positions...
	if (odd_top) {
		// odd top
		dy++;
		sh--;
		src+=src_pitch_pix;
		src_pixels-=src_pitch_pix;
		dst_y+=dst_pitch;
		dst_u+=dst_pitch >> 1;
		dst_v+=dst_pitch >> 1;
	}

	if (odd_bottom) {
		// odd bottom
		src_height--;
		src_pixels-=src_pitch_pix;
	}

	if (odd_left) {
		// odd left
		dx++;
		sw--;
		src++;
		dst_y++;
		dst_u++;
		dst_v++;
	}

	if (odd_right) {
		// odd right
		sw--;
	}

	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
	unsigned int *src_end = src + src_pixels;
	int src_pitch_diff = (src_pitch_pix << 1) - sw;
	int dst_pitch_diff = (dst_pitch_pix << 1) - sw;
	int dst_pitch_uvdiff = (dst_pitch_pix - sw) >> 1;

	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;

		// go through two lines in parallel (square 2x2 pixel)
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

		    // calculate the four pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u=, d_v=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src4_offs], dst_y[dst_y4_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src  +=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// go to the next two lines
		src   += src_pitch_diff;
		dst_y += dst_pitch_diff;
		dst_u += dst_pitch_uvdiff;
		dst_v += dst_pitch_uvdiff;
	}
}


void MMSFBSurface::eAB_blend_srcalpha_argb_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
				 			 		  			   unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy,
				 			 		  			   unsigned char alpha) {

	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		eAB_blend_argb_to_yv12(extbuf, src_height, sx, sy, sw, sh,
						       dst, dst_pitch, dst_height, dx, dy);
		return;
	}

	// first time?
	if (firsttime_eAB_blend_srcalpha_argb_to_yv12) {
		printf("DISKO: Using accelerated blend srcalpha ARGB to YV12.\n");
		firsttime_eAB_blend_srcalpha_argb_to_yv12 = false;
	}

	// something to do?
	if (!alpha)
		// source should blitted full transparent, so leave destination as is
		return;

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int  src_pitch_pix 		= src_pitch >> 2;
	int dst_pitch_pix 		= dst_pitch;
	int dst_pitch_pix_half	= dst_pitch_pix >> 1;

	src+= sx + sy * src_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

	unsigned int OLDSRC  = (*src) + 1;
	unsigned int old_y;
	unsigned int old_u;
	unsigned int old_v;

	int  src_pixels = src_pitch_pix * sh;

	// check odd/even
	bool odd_left 	= (dx & 0x01);
	bool odd_top 	= (dy & 0x01);
	bool odd_right 	= ((dx + sw) & 0x01);
	bool odd_bottom = ((dy + sh) & 0x01);

	// pointer to the pixel components of the first pixel
	unsigned char *dst_y = dst + dx + dy * dst_pitch_pix;
	unsigned char *dst_u = dst + dst_pitch_pix * (dst_height + (dst_height >> 2)) + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	unsigned char *dst_v = dst + dst_pitch_pix *  dst_height                      + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;

	// offsets to the other three pixels
	unsigned int dst_y2_offs = 1;
	unsigned int dst_y3_offs = dst_pitch;
	unsigned int dst_y4_offs = dst_y3_offs + 1;
	unsigned int src2_offs = 1;
	unsigned int src3_offs = src_pitch_pix;
	unsigned int src4_offs = src3_offs + 1;

	// arithmetic mean
	register unsigned int d_u;
	register unsigned int d_v;

	// calc alpha
	register unsigned int ALPHA = alpha;
	ALPHA++;

	// draw odd pixels around the even rectangle
	if (odd_top && odd_left) {
		// odd top-left pixel
		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;
	}

	if (odd_top && odd_right) {
		// odd top-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom && odd_left) {
		// odd bottom-left pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom && odd_right) {
		// odd bottom-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_top) {
		// odd top line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom) {
		// odd bottom line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_left) {
		// odd left line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_right) {
		// odd right line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	// calc even positions...
	if (odd_top) {
		// odd top
		dy++;
		sh--;
		src+=src_pitch_pix;
		src_pixels-=src_pitch_pix;
		dst_y+=dst_pitch;
		dst_u+=dst_pitch >> 1;
		dst_v+=dst_pitch >> 1;
	}

	if (odd_bottom) {
		// odd bottom
		src_height--;
		src_pixels-=src_pitch_pix;
	}

	if (odd_left) {
		// odd left
		dx++;
		sw--;
		src++;
		dst_y++;
		dst_u++;
		dst_v++;
	}

	if (odd_right) {
		// odd right
		sw--;
	}

	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
	unsigned int *src_end = src + src_pixels;
	int src_pitch_diff = (src_pitch_pix << 1) - sw;
	int dst_pitch_diff = (dst_pitch_pix << 1) - sw;
	int dst_pitch_uvdiff = (dst_pitch_pix - sw) >> 1;

	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;

		// go through two lines in parallel (square 2x2 pixel)
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

		    // calculate the four pixels...
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u=, d_v=);
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(src[src4_offs], dst_y[dst_y4_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src  +=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// go to the next two lines
		src   += src_pitch_diff;
		dst_y += dst_pitch_diff;
		dst_u += dst_pitch_uvdiff;
		dst_v += dst_pitch_uvdiff;
	}
}



void MMSFBSurface::eAB_blend_ayuv_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
				 			 			  unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy) {

	// first time?
	if (firsttime_eAB_blend_ayuv_to_yv12) {
		printf("DISKO: Using accelerated blend AYUV to YV12.\n");
		firsttime_eAB_blend_ayuv_to_yv12 = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int  src_pitch_pix 		= src_pitch >> 2;
	int dst_pitch_pix 		= dst_pitch;
	int dst_pitch_pix_half	= dst_pitch_pix >> 1;

	src+= sx + sy * src_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

	unsigned int OLDSRC  = (*src) + 1;
	unsigned int old_y;
	unsigned int old_u;
	unsigned int old_v;

	int  src_pixels = src_pitch_pix * sh;

	// check odd/even
	bool odd_left 	= (dx & 0x01);
	bool odd_top 	= (dy & 0x01);
	bool odd_right 	= ((dx + sw) & 0x01);
	bool odd_bottom = ((dy + sh) & 0x01);

	// pointer to the pixel components of the first pixel
	unsigned char *dst_y = dst + dx + dy * dst_pitch_pix;
	unsigned char *dst_u = dst + dst_pitch_pix * (dst_height + (dst_height >> 2)) + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	unsigned char *dst_v = dst + dst_pitch_pix *  dst_height                      + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;

	// offsets to the other three pixels
	unsigned int dst_y2_offs = 1;
	unsigned int dst_y3_offs = dst_pitch;
	unsigned int dst_y4_offs = dst_y3_offs + 1;
	unsigned int src2_offs = 1;
	unsigned int src3_offs = src_pitch_pix;
	unsigned int src4_offs = src3_offs + 1;

	// arithmetic mean
	register unsigned int d_u;
	register unsigned int d_v;

	// draw odd pixels around the even rectangle
	if (odd_top && odd_left) {
		// odd top-left pixel
		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;
	}

	if (odd_top && odd_right) {
		// odd top-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom && odd_left) {
		// odd bottom-left pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom && odd_right) {
		// odd bottom-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_top) {
		// odd top line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom) {
		// odd bottom line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_left) {
		// odd left line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_right) {
		// odd right line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	// calc even positions...
	if (odd_top) {
		// odd top
		dy++;
		sh--;
		src+=src_pitch_pix;
		src_pixels-=src_pitch_pix;
		dst_y+=dst_pitch;
		dst_u+=dst_pitch >> 1;
		dst_v+=dst_pitch >> 1;
	}

	if (odd_bottom) {
		// odd bottom
		src_height--;
		src_pixels-=src_pitch_pix;
	}

	if (odd_left) {
		// odd left
		dx++;
		sw--;
		src++;
		dst_y++;
		dst_u++;
		dst_v++;
	}

	if (odd_right) {
		// odd right
		sw--;
	}

	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
	unsigned int *src_end = src + src_pixels;
	int src_pitch_diff = (src_pitch_pix << 1) - sw;
	int dst_pitch_diff = (dst_pitch_pix << 1) - sw;
	int dst_pitch_uvdiff = (dst_pitch_pix - sw) >> 1;

	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;

		// go through two lines in parallel (square 2x2 pixel)
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

		    // calculate the four pixels...
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u=, d_v=);
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_AYUV_TO_YV12_PIXEL(src[src4_offs], dst_y[dst_y4_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src  +=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// go to the next two lines
		src   += src_pitch_diff;
		dst_y += dst_pitch_diff;
		dst_u += dst_pitch_uvdiff;
		dst_v += dst_pitch_uvdiff;
	}
}



void MMSFBSurface::eAB_blend_srcalpha_ayuv_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
				 			 		  			   unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy,
				 			 		  			   unsigned char alpha) {

	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		eAB_blend_ayuv_to_yv12(extbuf, src_height, sx, sy, sw, sh,
							   dst, dst_pitch, dst_height, dx, dy);
		return;
	}

	// first time?
	if (firsttime_eAB_blend_srcalpha_ayuv_to_yv12) {
		printf("DISKO: Using accelerated blend srcalpha AYUV to YV12.\n");
		firsttime_eAB_blend_srcalpha_ayuv_to_yv12 = false;
	}

	// something to do?
	if (!alpha)
		// source should blitted full transparent, so leave destination as is
		return;

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int  src_pitch_pix 		= src_pitch >> 2;
	int dst_pitch_pix 		= dst_pitch;
	int dst_pitch_pix_half	= dst_pitch_pix >> 1;

	src+= sx + sy * src_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

	unsigned int OLDSRC  = (*src) + 1;
	unsigned int old_y;
	unsigned int old_u;
	unsigned int old_v;

	int  src_pixels = src_pitch_pix * sh;

	// check odd/even
	bool odd_left 	= (dx & 0x01);
	bool odd_top 	= (dy & 0x01);
	bool odd_right 	= ((dx + sw) & 0x01);
	bool odd_bottom = ((dy + sh) & 0x01);

	// pointer to the pixel components of the first pixel
	unsigned char *dst_y = dst + dx + dy * dst_pitch_pix;
	unsigned char *dst_u = dst + dst_pitch_pix * (dst_height + (dst_height >> 2)) + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	unsigned char *dst_v = dst + dst_pitch_pix *  dst_height                      + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;

	// offsets to the other three pixels
	unsigned int dst_y2_offs = 1;
	unsigned int dst_y3_offs = dst_pitch;
	unsigned int dst_y4_offs = dst_y3_offs + 1;
	unsigned int src2_offs = 1;
	unsigned int src3_offs = src_pitch_pix;
	unsigned int src4_offs = src3_offs + 1;

	// arithmetic mean
	register unsigned int d_u;
	register unsigned int d_v;

	// calc alpha
	register unsigned int ALPHA = alpha;
	ALPHA++;

	// draw odd pixels around the even rectangle
	if (odd_top && odd_left) {
		// odd top-left pixel
		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;
	}

	if (odd_top && odd_right) {
		// odd top-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom && odd_left) {
		// odd bottom-left pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom && odd_right) {
		// odd bottom-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_top) {
		// odd top line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_bottom) {
		// odd bottom line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_left) {
		// odd left line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	if (odd_right) {
		// odd right line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;
	}

	// calc even positions...
	if (odd_top) {
		// odd top
		dy++;
		sh--;
		src+=src_pitch_pix;
		src_pixels-=src_pitch_pix;
		dst_y+=dst_pitch;
		dst_u+=dst_pitch >> 1;
		dst_v+=dst_pitch >> 1;
	}

	if (odd_bottom) {
		// odd bottom
		src_height--;
		src_pixels-=src_pitch_pix;
	}

	if (odd_left) {
		// odd left
		dx++;
		sw--;
		src++;
		dst_y++;
		dst_u++;
		dst_v++;
	}

	if (odd_right) {
		// odd right
		sw--;
	}

	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
	unsigned int *src_end = src + src_pixels;
	int src_pitch_diff = (src_pitch_pix << 1) - sw;
	int dst_pitch_diff = (dst_pitch_pix << 1) - sw;
	int dst_pitch_uvdiff = (dst_pitch_pix - sw) >> 1;

	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;

		// go through two lines in parallel (square 2x2 pixel)
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

		    // calculate the four pixels...
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u=, d_v=);
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_SRCALPHA_AYUV_TO_YV12_PIXEL(src[src4_offs], dst_y[dst_y4_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src  +=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// go to the next two lines
		src   += src_pitch_diff;
		dst_y += dst_pitch_diff;
		dst_u += dst_pitch_uvdiff;
		dst_v += dst_pitch_uvdiff;
	}
}








bool MMSFBSurface::extendedAccelBlitEx(MMSFBSurface *source,
									   MMSFBExternalSurfaceBuffer *extbuf, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
									   MMSFBRectangle *src_rect, int x, int y) {
	MMSFBExternalSurfaceBuffer myextbuf;
	if (source) {
		// premultiplied surface?
		if (!source->config.surface_buffer->premultiplied)
			return false;

		src_pixelformat = source->config.surface_buffer->pixelformat;
		src_width = (!source->root_parent)?source->config.w:source->root_parent->config.w;
		src_height = (!source->root_parent)?source->config.h:source->root_parent->config.h;
		memset(&myextbuf, 0, sizeof(MMSFBExternalSurfaceBuffer));
	}
	else
		myextbuf = *extbuf;

	// a few help and clipping values
	void *dst_ptr;
	int  dst_pitch;
	int sx = src_rect->x;
	int sy = src_rect->y;
	int sw = src_rect->w;
	int sh = src_rect->h;
	MMSFBRegion clipreg;
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
	if (src_pixelformat == MMSFB_PF_ARGB) {
		// source is ARGB
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_ARGB) {
			// destination is ARGB
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_NOFX) {
				// convert without alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_argb_to_argb(&myextbuf, src_height,
									   sx, sy, sw, sh,
									   (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
									   x, y);
					extendedUnlock(source, this);
					return true;
				}

				return false;
			}
			else
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_argb_to_argb(&myextbuf, src_height,
										   sx, sy, sw, sh,
										   (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										   x, y);
					extendedUnlock(source, this);
					return true;
				}

				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA|MMSFB_BLIT_SRC_PREMULTCOLOR))) {
				// blitting with alpha channel and coloralpha
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_srcalpha_argb_to_argb(&myextbuf, src_height,
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
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_AiRGB) {
			// destination is AiRGB
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_argb_to_airgb(&myextbuf, src_height,
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
		else
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_RGB16) {
			// destination is RGB16 (RGB565)
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_NOFX) {
				// convert without alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_argb_to_rgb16(&myextbuf, src_height,
									   sx, sy, sw, sh,
									   (unsigned short int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
									   x, y);
					extendedUnlock(source, this);
					return true;
				}
				return false;
			}
			else
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_argb_to_rgb16(&myextbuf, src_height,
											sx, sy, sw, sh,
										    (unsigned short int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										    x, y);
					extendedUnlock(source, this);
					return true;
				}
				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA|MMSFB_BLIT_SRC_PREMULTCOLOR))) {
//				printf("fehlt noch-!!!\n");
			}

			// does not match
			return false;
		}
		else
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_YV12) {
			// destination is YV12
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_argb_to_yv12(&myextbuf, src_height,
										   sx, sy, sw, sh,
										   (unsigned char *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										   x, y);
					extendedUnlock(source, this);
					return true;
				}

				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA|MMSFB_BLIT_SRC_PREMULTCOLOR))) {
				// blitting with alpha channel and coloralpha
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_srcalpha_argb_to_yv12(&myextbuf, src_height,
										   			sx, sy, sw, sh,
										   			(unsigned char *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
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

		// does not match
		return false;
	}
	else
	if (src_pixelformat == MMSFB_PF_RGB16) {
		// source is RGB16 (RGB565)
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_RGB16) {
			// destination is RGB16 (RGB565)
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_NOFX) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_rgb16_to_rgb16(&myextbuf, src_height,
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
	if (src_pixelformat == MMSFB_PF_AiRGB) {
		// source is AiRGB
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_AiRGB) {
			// destination is AiRGB
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_NOFX) {
				// convert without alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_airgb_to_airgb(&myextbuf, src_height,
									   sx, sy, sw, sh,
									   (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
									   x, y);
					extendedUnlock(source, this);
					return true;
				}

				return false;
			}
			else
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_airgb_to_airgb(&myextbuf, src_height,
										   	 sx, sy, sw, sh,
										     (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										     x, y);
					extendedUnlock(source, this);
					return true;
				}

				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA|MMSFB_BLIT_SRC_PREMULTCOLOR))) {
				// blitting with alpha channel and coloralpha
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_srcalpha_airgb_to_airgb(&myextbuf, src_height,
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
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_RGB16) {
			// destination is RGB16 (RGB565)
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_NOFX) {
				// convert without alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_airgb_to_rgb16(&myextbuf, src_height,
									    sx, sy, sw, sh,
									    (unsigned short int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
									    x, y);
					extendedUnlock(source, this);
					return true;
				}

				return false;
			}
			else
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_airgb_to_rgb16(&myextbuf, src_height,
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
	if (src_pixelformat == MMSFB_PF_AYUV) {
		// source is AYUV
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_AYUV) {
			// destination is AYUV
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_NOFX) {
				// convert without alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_ayuv_to_ayuv(&myextbuf, src_height,
									   sx, sy, sw, sh,
									   (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
									   x, y);
					extendedUnlock(source, this);
					return true;
				}

				return false;
			}
			else
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_ayuv_to_ayuv(&myextbuf, src_height,
										   sx, sy, sw, sh,
										   (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										   x, y);
					extendedUnlock(source, this);
					return true;
				}

				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA|MMSFB_BLIT_SRC_PREMULTCOLOR))) {
				// blitting with alpha channel and coloralpha
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_srcalpha_ayuv_to_ayuv(&myextbuf, src_height,
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
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_RGB16) {
			// destination is RGB16 (RGB565)
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_NOFX) {
				// convert without alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_ayuv_to_rgb16(&myextbuf, src_height,
									   sx, sy, sw, sh,
									   (unsigned short int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
									   x, y);
					extendedUnlock(source, this);
					return true;
				}

				return false;
			}
			else
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_ayuv_to_rgb16(&myextbuf, src_height,
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
		else
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_YV12) {
			// destination is YV12
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_ayuv_to_yv12(&myextbuf, src_height,
										   sx, sy, sw, sh,
										   (unsigned char *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
										   x, y);
					extendedUnlock(source, this);
					return true;
				}

				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA|MMSFB_BLIT_SRC_PREMULTCOLOR))) {
				// blitting with alpha channel and coloralpha
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_blend_srcalpha_ayuv_to_yv12(&myextbuf, src_height,
										   			sx, sy, sw, sh,
										   			(unsigned char *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
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

		// does not match
		return false;
	}
	else
	if (src_pixelformat == MMSFB_PF_YV12) {
		// source is YV12
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_YV12) {
			// destination is YV12
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_NOFX) {
				// convert without alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eAB_yv12_to_yv12(&myextbuf, src_height,
									   sx, sy, sw, sh,
									   (unsigned char *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
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

	// does not match
	return false;
}

bool MMSFBSurface::extendedAccelBlit(MMSFBSurface *source, MMSFBRectangle *src_rect, int x, int y) {
	// extended acceleration on?
	if (!this->extendedaccel)
		return false;

	if (!extendedAccelBlitEx(source,
			                 NULL, MMSFB_PF_NONE, 0, 0,
			                 src_rect, x, y))
		return printMissingCombination("extendedAccelBlit()", source);
	else
		return true;
}

bool MMSFBSurface::extendedAccelBlitBuffer(MMSFBExternalSurfaceBuffer *extbuf, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
										   MMSFBRectangle *src_rect, int x, int y) {
	// extended acceleration on?
	if (!this->extendedaccel)
		return false;

	if (!extendedAccelBlitEx(NULL,
							 extbuf, src_pixelformat, src_width, src_height,
			                 src_rect, x, y))
		return printMissingCombination("extendedAccelBlitBuffer()", NULL, extbuf, src_pixelformat, src_width, src_height);
	else
		return true;
}







void MMSFBSurface::eASB_blend_argb_to_argb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh) {

	// first time?
	if (firsttime_eASB_blend_argb_to_argb) {
		printf("DISKO: Using accelerated stretch & blend ARGB to ARGB.\n");
		firsttime_eASB_blend_argb_to_argb = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

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




void MMSFBSurface::eASB_blend_srcalpha_argb_to_argb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										   			unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
										   			unsigned char alpha) {

	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		eASB_blend_argb_to_argb(extbuf, src_height, sx, sy, sw, sh,
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

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

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
							// source alpha is > 0x00 and <= 0xff
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


void MMSFBSurface::eASB_blend_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										     unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh) {

	// first time?
	if (firsttime_eASB_blend_airgb_to_airgb) {
		printf("DISKO: Using accelerated stretch & blend AiRGB to AiRGB.\n");
		firsttime_eASB_blend_airgb_to_airgb = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

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

						if (!A) {
							// source pixel is not transparent, copy it directly to the destination
							do {
								*dst = SRC;
								dst++;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}
						else
						if (A == 0xff) {
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

							do {
								if (DST==OLDDST) {
									// same pixel, use the previous value
									if (A < 0xff) {
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
							    a = (A * (0x100 - a)) >> 8;
							    r = (A * r) >> 8;
							    g = (A * g) >> 8;
							    b = (A * b) >> 8;

							    // add src to dst
							    a += 0x100 - A;
							    r += (SRC << 8) >> 24;
							    g += (SRC << 16) >> 24;
							    b += SRC & 0xff;
								d =   ((r >> 8) ? 0xff0000   : (r << 16))
									| ((g >> 8) ? 0xff00     : (g << 8))
							    	| ((b >> 8) ? 0xff 		 :  b);
							    if (!(a >> 8)) d |= (0x100 - a) << 24;
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


void MMSFBSurface::eASB_blend_srcalpha_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										   			  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
										   			  unsigned char alpha) {

	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		eASB_blend_airgb_to_airgb(extbuf, src_height, sx, sy, sw, sh,
							      dst, dst_pitch, dst_height, dx, dy, dw, dh);
		return;
	}

	// first time?
	if (firsttime_eASB_blend_srcalpha_airgb_to_airgb) {
		printf("DISKO: Using accelerated stretch & blend srcalpha AiRGB to AiRGB.\n");
		firsttime_eASB_blend_srcalpha_airgb_to_airgb = false;
	}

	// something to do?
	if (!alpha)
		// source should blitted full transparent, so leave destination as is
		return;

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int  src_pitch_pix = src_pitch >> 2;
	int  dst_pitch_pix = dst_pitch >> 2;
	unsigned int *src_end = src + sx + src_pitch_pix * (sy + sh);
	if (src_end > src + src_pitch_pix * src_height)
		src_end = src + src_pitch_pix * src_height;
	unsigned int *dst_end = dst + dst_pitch_pix * dst_height;
	src+=sx + sy * src_pitch_pix;
	dst+=dx + dy * dst_pitch_pix;

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

						if (A == 0xff) {
							// source pixel is full transparent, do not change the destination
							do {
								dst++;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}
						else
						{
							// source alpha is >= 0x00 and < 0xff
							register unsigned int DST = *dst;
							unsigned int OLDDST = DST + 1;
							register unsigned int d;

							// load source pixel and multiply it with given ALPHA
						    A = 0x100 - ((ALPHA * (0x100 - A)) >> 8);
							unsigned int sr = (ALPHA * (SRC & 0xff0000)) >> 24;
							unsigned int sg = (ALPHA * (SRC & 0xff00)) >> 16;
							unsigned int sb = (ALPHA * (SRC & 0xff)) >> 8;

							do {
								if (DST==OLDDST) {
									// same pixel, use the previous value
									if (A < 0xff) {
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
							    a = (A * (0x100 - a)) >> 8;
							    r = (A * r) >> 8;
							    g = (A * g) >> 8;
							    b = (A * b) >> 8;

							    // add src to dst
							    a += 0x100 - A;
							    r += sr;
							    g += sg;
							    b += sb;
								d =   ((r >> 8) ? 0xff0000   : (r << 16))
									| ((g >> 8) ? 0xff00     : (g << 8))
							    	| ((b >> 8) ? 0xff 		 :  b);
							    if (!(a >> 8)) d |= (0x100 - a) << 24;
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


void MMSFBSurface::eASB_blend_ayuv_to_ayuv(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh) {

	// first time?
	if (firsttime_eASB_blend_ayuv_to_ayuv) {
		printf("DISKO: Using accelerated stretch & blend AYUV to AYUV.\n");
		firsttime_eASB_blend_ayuv_to_ayuv = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

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
							register int SA= 0x100 - A;
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
								int y = (DST << 8) >> 24;
								int u = (DST << 16) >> 24;
								int v = DST & 0xff;

								// we have to move the 0 point of the coordinate system
								// this make it a little slower than ARGB to ARGB blending
								MMSFBSurface_PREPARE_YUVBLEND(y,u,v);

								// invert src alpha
							    a = (SA * a) >> 8;
							    y = (SA * y) >> 8;
							    u = (SA * u) >> 8;
							    v = (SA * v) >> 8;

							    // add src to dst
							    a += A;
							    y += (SRC << 8) >> 24;
							    u += (SRC << 16) >> 24;
							    v += SRC & 0xff;

							    // build destination pixel, have to check for negative values
								// this make it a little slower than ARGB to ARGB blending
							    d = ((a >> 8) ? 0xff000000 : (a << 24));
							    if (y > 0)
							    	d |= ((y >> 8) ? 0xff0000 : (y << 16));
							    if (u > 0)
							    	d |= ((u >> 8) ? 0xff00 : (u << 8));
							    if (v > 0)
							    	d |= ((v >> 8) ? 0xff : v);

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

void MMSFBSurface::eASB_blend_srcalpha_ayuv_to_ayuv(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										   			unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
										   			unsigned char alpha) {

	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		eASB_blend_ayuv_to_ayuv(extbuf, src_height, sx, sy, sw, sh,
							    dst, dst_pitch, dst_height, dx, dy, dw, dh);
		return;
	}

	// first time?
	if (firsttime_eASB_blend_srcalpha_ayuv_to_ayuv) {
		printf("DISKO: Using accelerated stretch & blend srcalpha AYUV to AYUV.\n");
		firsttime_eASB_blend_srcalpha_ayuv_to_ayuv = false;
	}

	// something to do?
	if (!alpha)
		// source should blitted full transparent, so leave destination as is
		return;

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

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


	register int ALPHA = alpha;
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
							// source alpha is > 0x00 and <= 0xff
							register unsigned int DST = *dst;
							unsigned int OLDDST = DST + 1;
							register unsigned int d;

							// load source pixel
						    A = (ALPHA * A) >> 8;
							int sy = (SRC << 8) >> 24;
							int su = (SRC << 16) >> 24;
							int sv = SRC & 0xff;
							register int SA= 0x100 - A;

							// multiply source with given ALPHA
							// we have to move the 0 point of the coordinate system
							// this make it a little slower than ARGB to ARGB blending
							MMSFBSurface_PREPARE_YUVBLEND(sy,su,sv);
						    sy = (ALPHA * sy) >> 8;
						    su = (ALPHA * su) >> 8;
						    sv = (ALPHA * sv) >> 8;
							MMSFBSurface_RESET_YUVBLEND(sy,su,sv);

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
								int y = (DST << 8) >> 24;
								int u = (DST << 16) >> 24;
								int v = DST & 0xff;

								// we have to move the 0 point of the coordinate system
								// this make it a little slower than ARGB to ARGB blending
								MMSFBSurface_PREPARE_YUVBLEND(y,u,v);

								// invert src alpha
							    a = (SA * a) >> 8;
							    y = (SA * y) >> 8;
							    u = (SA * u) >> 8;
							    v = (SA * v) >> 8;

							    // add src to dst
							    a += A;
							    y += sy;
							    u += su;
							    v += sv;

							    // build destination pixel, have to check for negative values
								// this make it a little slower than ARGB to ARGB blending
							    d = ((a >> 8) ? 0xff000000 : (a << 24));
							    if (y > 0)
							    	d |= ((y >> 8) ? 0xff0000 : (y << 16));
							    if (u > 0)
							    	d |= ((u >> 8) ? 0xff00 : (u << 8));
							    if (v > 0)
							    	d |= ((v >> 8) ? 0xff : v);

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

void MMSFBSurface::eASB_yv12_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
								     unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh) {

	// first time?
	if (firsttime_eASB_yv12_to_yv12) {
		printf("DISKO: Using accelerated stretch YV12 to YV12.\n");
		firsttime_eASB_yv12_to_yv12 = false;
	}

	// get the first source ptr/pitch
	unsigned char *src = (unsigned char *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix 		= src_pitch;
	int src_pitch_pix_half	= src_pitch_pix >> 1;
	int dst_pitch_pix 		= dst_pitch;
	int dst_pitch_pix_half	= dst_pitch_pix >> 1;

//	src+= sx + sy * src_pitch_pix;

	// check the surface range
	if ((sw <= 0)||(sh <= 0))
		return;

//	unsigned int OLDSRC  = (*src) + 1;
//	unsigned int old_y;
//	unsigned int old_u;
//	unsigned int old_v;

	int  src_pixels = src_pitch_pix * sh;

	// check odd/even
	bool odd_left 	= (dx & 0x01);
	bool odd_top 	= (dy & 0x01);
	bool odd_right 	= ((dx + sw) & 0x01);
	bool odd_bottom = ((dy + sh) & 0x01);

	// pointer to the pixel components of the first pixel
	unsigned char *src_y = src + sx + sy * src_pitch_pix;
	unsigned char *src_u;
	unsigned char *src_v;
	if ((extbuf->ptr2)&&(extbuf->ptr3)) {
		src_u = (unsigned char *)extbuf->ptr2 + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
		src_v = (unsigned char *)extbuf->ptr3 + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
	}
	else {
		src_u = src + src_pitch_pix * (src_height + (src_height >> 2)) + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
		src_v = src + src_pitch_pix *  src_height                      + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
	}
	unsigned char *dst_y = dst + dx + dy * dst_pitch_pix;
	unsigned char *dst_u = dst + dst_pitch_pix * (dst_height + (dst_height >> 2)) + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	unsigned char *dst_v = dst + dst_pitch_pix *  dst_height                      + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;

	// offsets to the other three pixels
	unsigned int src_y2_offs = 1;
	unsigned int src_y3_offs = src_pitch_pix;
	unsigned int src_y4_offs = src_y3_offs + 1;
	unsigned int dst_y2_offs = 1;
	unsigned int dst_y3_offs = dst_pitch_pix;
	unsigned int dst_y4_offs = dst_y3_offs + 1;

	// arithmetic mean
	register unsigned int d_u;
	register unsigned int d_v;

	// draw odd pixels around the even rectangle
	if (odd_top && odd_left) {
/*		// odd top-left pixel
		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;*/
	}

	if (odd_top && odd_right) {
/*		// odd top-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom && odd_left) {
/*		// odd bottom-left pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom && odd_right) {
/*		// odd bottom-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_top) {
/*		// odd top line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom) {
/*		// odd bottom line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_left) {
/*		// odd left line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_right) {
/*		// odd right line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	// calc even positions...
	if (odd_top) {
		// odd top
		dy++;
		sh--;
		src_y+=src_pitch;
		src_pixels-=src_pitch_pix;
		dst_y+=dst_pitch;
		dst_u+=dst_pitch >> 1;
		dst_v+=dst_pitch >> 1;
	}

	if (odd_bottom) {
		// odd bottom
		src_height--;
		src_pixels-=src_pitch_pix;
	}

	if (odd_left) {
		// odd left
		dx++;
		sw--;
		src_y++;
		dst_y++;
		dst_u++;
		dst_v++;
	}

	if (odd_right) {
		// odd right
		sw--;
	}

	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
/*	unsigned char *src_end = src_y + src_pixels;
	int src_pitch_diff = src_pitch_pix << 1;
	int src_pitch_uvdiff = src_pitch_pix_half;
	int dst_pitch_diff = dst_pitch_pix << 1;
	int dst_pitch_uvdiff = dst_pitch_pix_half;

	// for all lines
	while (src_y < src_end) {
		// for all pixels in the line
		memcpy(dst_y, src_y, sw);
		memcpy(&dst_y[dst_y3_offs], &src_y[src_y3_offs], sw);
		memcpy(dst_u, src_u, sw >> 1);
		memcpy(dst_v, src_v, sw >> 1);

		// go to the next two lines
		src_y += src_pitch_diff;
		src_u += src_pitch_uvdiff;
		src_v += src_pitch_uvdiff;
		dst_y += dst_pitch_diff;
		dst_u += dst_pitch_uvdiff;
		dst_v += dst_pitch_uvdiff;
	}*/



	src = src_y;
	dst = dst_y;


//	int  src_pitch_pix = src_pitch >> 2;
//	int  dst_pitch_pix = dst_pitch >> 2;
	unsigned char *src_end = src + sx + src_pitch_pix * (sy + sh);
	if (src_end > src + src_pitch_pix * src_height)
		src_end = src + src_pitch_pix * src_height;
	unsigned char *dst_end = dst + dst_pitch_pix * dst_height;
//	src+=sx + sy * src_pitch_pix;
//	dst+=dx + dy * dst_pitch_pix;


	int horifact = (dw<<16)/sw;
	int vertfact = (dh<<16)/sh;


if (0) {
	// for all lines
	int vertcnt = 0x8000;
	while ((src < src_end)&&(dst < dst_end)) {
		// for all pixels in the line
		vertcnt+=vertfact;
		if (vertcnt & 0xffff0000) {
			unsigned char *line_end = src + sw;
			unsigned char *old_dst = dst;

			do {
				int horicnt = 0x8000;
				while (src < line_end) {
					horicnt+=horifact;
					if (horicnt & 0xffff0000) {
						register unsigned char SRC  = *src;

						do {
							*dst = SRC;
							dst++;
							horicnt-=0x10000;
						} while (horicnt & 0xffff0000);
					}

				    src++;
				}
				src-=sw;
				vertcnt-=0x10000;
				dst = old_dst +  dst_pitch;
				old_dst = dst;
			} while (vertcnt & 0xffff0000);
		}

		// next line
		src+=src_pitch;
	}
}
else {

	if (0) {
		// horizontal antialiasing

		// for all lines
		int vertcnt = 0x8000;
		while ((src < src_end)&&(dst < dst_end)) {
			// for all pixels in the line
			vertcnt+=vertfact;
			if (vertcnt & 0xffff0000) {
				unsigned char *line_end = src + sw;
				unsigned char *old_dst = dst;

				do {
					int horicnt = 0x8000;
					register unsigned char SRC;
					register bool haa = false;
					while (src < line_end) {
						horicnt+=horifact;
						if (horicnt & 0xffff0000) {
							// check for antialiasing
							if (haa) {
								*(dst-1) = (SRC + *src) >> 1;
								haa = false;
							}

							// load pixel
							SRC = *src;

							// put first pixel
							*dst = SRC;
							dst++;
							horicnt-=0x10000;

							// have to put further?
							if ((haa=(horicnt & 0xffff0000))) {
								do {
									*dst = SRC;
									dst++;
									horicnt-=0x10000;
								} while (horicnt & 0xffff0000);
							}
						}

						src++;
					}
					src-=sw;
					vertcnt-=0x10000;
					dst = old_dst + dst_pitch;
					old_dst = dst;
				} while (vertcnt & 0xffff0000);
			}

			// next line
			src+=src_pitch;
		}
	}
	else {
		// horizontal and vertical antialiasing

		// for all lines
		int vertcnt = 0x8000;
		register unsigned char vcnt = 0;
		while ((src < src_end)&&(dst < dst_end)) {
			// for all pixels in the line
			vertcnt+=vertfact;
			if (vertcnt & 0xffff0000) {
				unsigned char *line_end = src + sw;
				unsigned char *old_dst = dst;

				bool vaa = (vcnt > 1);
				vcnt = 0;
				if (!vaa) {
					do {
						int horicnt = 0x8000;
						register unsigned char SRC;
						register bool haa = false;
						while (src < line_end) {
							horicnt+=horifact;
							if (horicnt & 0xffff0000) {
								// check for antialiasing
								if (haa) {
									*(dst-1) = (SRC + *src) >> 1;
									haa = false;
								}

								// load pixel
								SRC = *src;

								// put first pixel
								*dst = SRC;
								dst++;
								horicnt-=0x10000;

								// have to put further?
								if ((haa=(horicnt & 0xffff0000))) {
									do {
										*dst = SRC;
										dst++;
										horicnt-=0x10000;
									} while (horicnt & 0xffff0000);
								}
							}

							src++;
						}
						src-=sw;
						vertcnt-=0x10000;
						dst = old_dst + dst_pitch;
						old_dst = dst;
						vcnt++;
					} while (vertcnt & 0xffff0000);
				}
				else {
					do {
						int horicnt = 0x8000;
						register unsigned char SRC;
						register bool haa = false;
						while (src < line_end) {
							horicnt+=horifact;
							if (horicnt & 0xffff0000) {
								// check for antialiasing
								if (haa) {
									*(dst-1) = (SRC + *src) >> 1;
									haa = false;
								}

								// load pixel
								SRC = *src;

								// put first pixel
								*dst = SRC;
								*(dst-dst_pitch) = (*(dst-dst_pitch) + SRC) >> 1;
								dst++;
								horicnt-=0x10000;

								// have to put further?
								if ((haa=(horicnt & 0xffff0000))) {
									do {
										*dst = SRC;
										*(dst-dst_pitch) = (*(dst-dst_pitch) + SRC) >> 1;
										dst++;
										horicnt-=0x10000;
									} while (horicnt & 0xffff0000);
								}
							}

							src++;
						}
						src-=sw;
						vertcnt-=0x10000;
						dst = old_dst + dst_pitch;
						old_dst = dst;
						vcnt++;
					} while (vertcnt & 0xffff0000);
				}
			}

			// next line
			src+=src_pitch;
		}


	}
}







	sw /= 2;
	dw /= 2;
	sh /= 2;
	dh /= 2;

	{
	src = src_u;
	dst = dst_u;

//	int  src_pitch_pix = src_pitch >> 2;
//	int  dst_pitch_pix = dst_pitch >> 2;
	unsigned char *src_end = src + sx + src_pitch_pix_half * (sy + sh);
	if (src_end > src + src_pitch_pix_half * src_height)
		src_end = src + src_pitch_pix_half * src_height;
	unsigned char *dst_end = dst + dst_pitch_pix_half * dst_height;
//	src+=sx + sy * src_pitch_pix;
//	dst+=dx + dy * dst_pitch_pix;


	int horifact = (dw<<16)/sw;
	int vertfact = (dh<<16)/sh;



	// for all lines
	int vertcnt = 0x8000;
	while ((src < src_end)&&(dst < dst_end)) {
		// for all pixels in the line
		vertcnt+=vertfact;
		if (vertcnt & 0xffff0000) {
			unsigned char *line_end = src + sw;
			unsigned char *old_dst = dst;

			do {
				int horicnt = 0x8000;
				while (src < line_end) {
					// load pixel from memory and check if the previous pixel is the same

					horicnt+=horifact;

					if (horicnt & 0xffff0000) {
						register unsigned char SRC  = *src;

						do {
							*dst = SRC;
							dst++;
							horicnt-=0x10000;
						} while (horicnt & 0xffff0000);
					}

				    src++;
				}
				src-=sw;
				vertcnt-=0x10000;
//				dst = old_dst +  dst_pitch/4;
				dst = old_dst +  dst_pitch_pix_half;
				old_dst = dst;
			} while (vertcnt & 0xffff0000);
		}

		// next line
		src+=src_pitch_pix_half;
	}
	}




	{
	src = src_v;
	dst = dst_v;

//	int  src_pitch_pix = src_pitch >> 2;
//	int  dst_pitch_pix = dst_pitch >> 2;
	unsigned char *src_end = src + sx + src_pitch_pix_half * (sy + sh);
	if (src_end > src + src_pitch_pix_half * src_height)
		src_end = src + src_pitch_pix_half * src_height;
	unsigned char *dst_end = dst + dst_pitch_pix_half * dst_height;
//	src+=sx + sy * src_pitch_pix;
//	dst+=dx + dy * dst_pitch_pix;


	int horifact = (dw<<16)/sw;
	int vertfact = (dh<<16)/sh;



	// for all lines
	int vertcnt = 0x8000;
	while ((src < src_end)&&(dst < dst_end)) {
		// for all pixels in the line
		vertcnt+=vertfact;
		if (vertcnt & 0xffff0000) {
			unsigned char *line_end = src + sw;
			unsigned char *old_dst = dst;

			do {
				int horicnt = 0x8000;
				while (src < line_end) {
					// load pixel from memory and check if the previous pixel is the same

					horicnt+=horifact;

					if (horicnt & 0xffff0000) {
						register unsigned char SRC  = *src;

						do {
							*dst = SRC;
							dst++;
							horicnt-=0x10000;
						} while (horicnt & 0xffff0000);
					}

				    src++;
				}
				src-=sw;
				vertcnt-=0x10000;
//				dst = old_dst +  dst_pitch/4;
				dst = old_dst +  dst_pitch_pix_half;
				old_dst = dst;
			} while (vertcnt & 0xffff0000);
		}

		// next line
		src+=src_pitch_pix_half;
	}
	}

}


bool MMSFBSurface::extendedAccelStretchBlitEx(MMSFBSurface *source,
											  MMSFBExternalSurfaceBuffer *extbuf, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
											  MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect) {
	MMSFBExternalSurfaceBuffer myextbuf;
	if (source) {
		// premultiplied surface?
		if (!source->config.surface_buffer->premultiplied)
			return false;

		src_pixelformat = source->config.surface_buffer->pixelformat;
		src_width = (!source->root_parent)?source->config.w:source->root_parent->config.w;
		src_height = (!source->root_parent)?source->config.h:source->root_parent->config.h;
		memset(&myextbuf, 0, sizeof(MMSFBExternalSurfaceBuffer));
	}
	else
		myextbuf = *extbuf;

	// a few help and clipping values
	void *dst_ptr;
	int  dst_pitch;
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


	MMSFBRegion clipreg;
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
	if (src_pixelformat == MMSFB_PF_ARGB) {
		// source is ARGB
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_ARGB) {
			// destination is ARGB
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eASB_blend_argb_to_argb(&myextbuf, src_height,
											sx, sy, sw, sh,
											(unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
											dx, dy, dw, dh);
					extendedUnlock(source, this);

					return true;
				}
				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA|MMSFB_BLIT_SRC_PREMULTCOLOR))) {
				// blitting with alpha channel and coloralpha
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eASB_blend_srcalpha_argb_to_argb(&myextbuf, src_height,
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
	else
	if (src_pixelformat == MMSFB_PF_AiRGB) {
		// source is AiRGB
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_AiRGB) {
			// destination is AiRGB
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eASB_blend_airgb_to_airgb(&myextbuf, src_height,
											  sx, sy, sw, sh,
											  (unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
											  dx, dy, dw, dh);
					extendedUnlock(source, this);

					return true;
				}
				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA|MMSFB_BLIT_SRC_PREMULTCOLOR))) {
				// blitting with alpha channel and coloralpha
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eASB_blend_srcalpha_airgb_to_airgb(&myextbuf, src_height,
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
	else
	if (src_pixelformat == MMSFB_PF_AYUV) {
		// source is AYUV
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_AYUV) {
			// destination is AYUV
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_BLEND_ALPHACHANNEL) {
				// blitting with alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eASB_blend_ayuv_to_ayuv(&myextbuf, src_height,
											sx, sy, sw, sh,
											(unsigned int *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
											dx, dy, dw, dh);
					extendedUnlock(source, this);

					return true;
				}
				return false;
			}
			else
			if   ((this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA))
				||(this->config.blittingflags == (MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA|MMSFB_BLIT_SRC_PREMULTCOLOR))) {
				// blitting with alpha channel and coloralpha
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eASB_blend_srcalpha_ayuv_to_ayuv(&myextbuf, src_height,
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


	else
	if (src_pixelformat == MMSFB_PF_YV12) {
		// source is YV12
		if (this->config.surface_buffer->pixelformat == MMSFB_PF_YV12) {
			// destination is YV12
			if (this->config.blittingflags == (MMSFBBlittingFlags)MMSFB_BLIT_NOFX) {
				// convert without alpha channel
				if (extendedLock(source, &myextbuf.ptr, &myextbuf.pitch, this, &dst_ptr, &dst_pitch)) {
					eASB_yv12_to_yv12(&myextbuf, src_height,
									   sx, sy, sw, sh,
									   (unsigned char *)dst_ptr, dst_pitch, (!this->root_parent)?this->config.h:this->root_parent->config.h,
									   dx, dy, dw, dh);
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


bool MMSFBSurface::extendedAccelStretchBlit(MMSFBSurface *source, MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect) {
	// extended acceleration on?
	if (!this->extendedaccel)
		return false;

	if (!extendedAccelStretchBlitEx(source,
			                        NULL, MMSFB_PF_NONE, 0, 0,
			                        src_rect, dest_rect))
		return printMissingCombination("extendedAccelStretchBlit()", source);
	else
		return true;
}

bool MMSFBSurface::extendedAccelStretchBlitBuffer(MMSFBExternalSurfaceBuffer *extbuf, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
												  MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect) {
	// extended acceleration on?
	if (!this->extendedaccel)
		return false;

	if (!extendedAccelStretchBlitEx(NULL,
							        extbuf, src_pixelformat, src_width, src_height,
							        src_rect, dest_rect))
		return printMissingCombination("extendedAccelStretchBlitBuffer()", NULL, extbuf, src_pixelformat, src_width, src_height);
	else
		return true;
}


void MMSFBSurface::eAFR_argb(unsigned int *dst, int dst_pitch, int dst_height,
						     int dx, int dy, int dw, int dh, MMSFBColor color) {

	// first time?
	if (firsttime_eAFR_argb) {
		printf("DISKO: Using accelerated fill rectangle to ARGB.\n");
		firsttime_eAFR_argb = false;
	}

	// prepare...
	int dst_pitch_pix = dst_pitch >> 2;
	dst+= dx + dy * dst_pitch_pix;

	unsigned int *dst_end = dst + dst_pitch_pix * dh;
	int dst_pitch_diff = dst_pitch_pix - dw;
	register unsigned int d;

	// prepare the color
	register unsigned int A = color.a;
	register unsigned int SRC;
	SRC =     (A << 24)
			| (color.r << 16)
			| (color.g << 8)
			| color.b;

	// copy pixel directly to the destination
	// for all lines
	while (dst < dst_end) {
		// for all pixels in the line
		unsigned int *line_end = dst + dw;
		while (dst < line_end) {
			*dst = SRC;
			dst++;
		}

		// go to the next line
		dst+= dst_pitch_diff;
	}
}


void MMSFBSurface::eAFR_blend_argb(unsigned int *dst, int dst_pitch, int dst_height,
									int dx, int dy, int dw, int dh, MMSFBColor color) {

	// first time?
	if (firsttime_eAFR_blend_argb) {
		printf("DISKO: Using accelerated blend rectangle to ARGB.\n");
		firsttime_eAFR_blend_argb = false;
	}

	// return immediately if alpha channel of the color is 0x00
	if (!color.a)
		return;

	// prepare...
	int dst_pitch_pix = dst_pitch >> 2;
	dst+= dx + dy * dst_pitch_pix;

	unsigned int OLDDST = (*dst) + 1;
	unsigned int *dst_end = dst + dst_pitch_pix * dh;
	int dst_pitch_diff = dst_pitch_pix - dw;
	register unsigned int d;

	// prepare the color
	register unsigned int A = color.a;
	register unsigned int SRC;
	SRC =     (A << 24)
			| (color.r << 16)
			| (color.g << 8)
			| color.b;

	if (color.a == 0xff) {
		// source pixel is not transparent, copy it directly to the destination
		// for all lines
		while (dst < dst_end) {
			// for all pixels in the line
			unsigned int *line_end = dst + dw;
			while (dst < line_end) {
				*dst = SRC;
			    dst++;
			}

			// go to the next line
			dst+= dst_pitch_diff;
		}

	}
	else {
		// source alpha is > 0x00 and < 0xff
		// for all lines
		while (dst < dst_end) {
			// for all pixels in the line
			unsigned int *line_end = dst + dw;
			while (dst < line_end) {
				// read the destination
				register unsigned int DST = *dst;
				if (DST==OLDDST) {
					// same pixel, use the previous value
					*dst = d;
				    dst++;
					continue;
				}
				OLDDST = DST;

				register int SA= 0x100 - A;
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
			}

			// go to the next line
			dst+= dst_pitch_diff;
		}
	}
}


void MMSFBSurface::eAFR_rgb16(unsigned short int *dst, int dst_pitch, int dst_height,
						      int dx, int dy, int dw, int dh, MMSFBColor color) {

	// first time?
	if (firsttime_eAFR_rgb16) {
		printf("DISKO: Using accelerated fill rectangle to RGB16.\n");
		firsttime_eAFR_rgb16 = false;
	}

	// prepare...
	int dst_pitch_pix = dst_pitch >> 1;
	dst+= dx + dy * dst_pitch_pix;

	unsigned short int *dst_end = dst + dst_pitch_pix * dh;
	int dst_pitch_diff = dst_pitch_pix - dw;

	// prepare the color
	register unsigned short int SRC;
	unsigned int r = color.r >> 3;
	unsigned int g = color.g >> 2;
	unsigned int b = color.b >> 3;
	SRC =     (r << 11)
			| (g << 5)
			| b;

	// copy pixel directly to the destination
	// for all lines
	while (dst < dst_end) {
		// for all pixels in the line
		unsigned short int *line_end = dst + dw;
		while (dst < line_end) {
			*dst = SRC;
			dst++;
		}

		// go to the next line
		dst+= dst_pitch_diff;
	}
}


void MMSFBSurface::eAFR_ayuv(unsigned int *dst, int dst_pitch, int dst_height,
						     int dx, int dy, int dw, int dh, MMSFBColor color) {

	// first time?
	if (firsttime_eAFR_ayuv) {
		printf("DISKO: Using accelerated fill rectangle to AYUV.\n");
		firsttime_eAFR_ayuv = false;
	}

	// prepare...
	int dst_pitch_pix = dst_pitch >> 2;
	dst+= dx + dy * dst_pitch_pix;

	unsigned int *dst_end = dst + dst_pitch_pix * dh;
	int dst_pitch_diff = dst_pitch_pix - dw;
	register unsigned int d;

	// prepare the color
	register unsigned int A = color.a;
	register unsigned int SRC;
	SRC =     (A << 24)
			| ((MMSFBSurface_RGB2Y(color.r, color.g, color.b)) << 16)
			| ((MMSFBSurface_RGB2U(color.r, color.g, color.b)) << 8)
			| MMSFBSurface_RGB2V(color.r, color.g, color.b);

	// copy pixel directly to the destination
	// for all lines
	while (dst < dst_end) {
		// for all pixels in the line
		unsigned int *line_end = dst + dw;
		while (dst < line_end) {
			*dst = SRC;
			dst++;
		}

		// go to the next line
		dst+= dst_pitch_diff;
	}
}


void MMSFBSurface::eAFR_blend_ayuv(unsigned int *dst, int dst_pitch, int dst_height,
									int dx, int dy, int dw, int dh, MMSFBColor color) {

	// first time?
	if (firsttime_eAFR_blend_ayuv) {
		printf("DISKO: Using accelerated blend rectangle to AYUV.\n");
		firsttime_eAFR_blend_ayuv = false;
	}

	// return immediately if alpha channel of the color is 0x00
	if (!color.a)
		return;

	// prepare...
	int dst_pitch_pix = dst_pitch >> 2;
	dst+= dx + dy * dst_pitch_pix;

	unsigned int OLDDST = (*dst) + 1;
	unsigned int *dst_end = dst + dst_pitch_pix * dh;
	int dst_pitch_diff = dst_pitch_pix - dw;
	register unsigned int d;

	// prepare the color
	register unsigned int A = color.a;
	register unsigned int SRC;
	SRC =     (A << 24)
			| ((MMSFBSurface_RGB2Y(color.r, color.g, color.b)) << 16)
			| ((MMSFBSurface_RGB2U(color.r, color.g, color.b)) << 8)
			| MMSFBSurface_RGB2V(color.r, color.g, color.b);

	if (color.a == 0xff) {
		// source pixel is not transparent, copy it directly to the destination
		// for all lines
		while (dst < dst_end) {
			// for all pixels in the line
			unsigned int *line_end = dst + dw;
			while (dst < line_end) {
				*dst = SRC;
			    dst++;
			}

			// go to the next line
			dst+= dst_pitch_diff;
		}

	}
	else {
		// source alpha is > 0x00 and < 0xff
		// for all lines
		while (dst < dst_end) {
			// for all pixels in the line
			unsigned int *line_end = dst + dw;
			while (dst < line_end) {
				// read the destination
				register unsigned int DST = *dst;
				if (DST==OLDDST) {
					// same pixel, use the previous value
					*dst = d;
				    dst++;
					continue;
				}
				OLDDST = DST;

				register int SA= 0x100 - A;
				unsigned int a = DST >> 24;
				int y = (DST << 8) >> 24;
				int u = (DST << 16) >> 24;
				int v = DST & 0xff;

				// we have to move the 0 point of the coordinate system
				// this make it a little slower than ARGB to ARGB blending
				MMSFBSurface_PREPARE_YUVBLEND(y,u,v);

				// invert src alpha
			    a = (SA * a) >> 8;
			    y = (SA * y) >> 8;
			    u = (SA * u) >> 8;
			    v = (SA * v) >> 8;

			    // add src to dst
			    a += A;
			    y += (SRC << 8) >> 24;
			    u += (SRC << 16) >> 24;
			    v += SRC & 0xff;

			    // build destination pixel, have to check for negative values
				// this make it a little slower than ARGB to ARGB blending
			    d = ((a >> 8) ? 0xff000000 : (a << 24));
			    if (y > 0)
			    	d |= ((y >> 8) ? 0xff0000 : (y << 16));
			    if (u > 0)
			    	d |= ((u >> 8) ? 0xff00 : (u << 8));
			    if (v > 0)
			    	d |= ((v >> 8) ? 0xff : v);

			    *dst = d;

			    dst++;
			}

			// go to the next line
			dst+= dst_pitch_diff;
		}
	}
}


void MMSFBSurface::eAFR_yv12(unsigned char *dst, int dst_pitch, int dst_height,
						     int dx, int dy, int dw, int dh, MMSFBColor color) {

	// first time?
	if (firsttime_eAFR_yv12) {
		printf("DISKO: Using accelerated fill rectangle to YV12.\n");
		firsttime_eAFR_yv12 = false;
	}

	// prepare...
//	int  src_pitch_pix 		= src_pitch >> 2;
	int dst_pitch_pix 		= dst_pitch;
	int dst_pitch_pix_half	= dst_pitch_pix >> 1;

//	src+= sx + sy * src_pitch_pix;

//	unsigned int OLDSRC  = (*src) + 1;
//	unsigned int old_y;
//	unsigned int old_u;
//	unsigned int old_v;

//	int  src_pixels = src_pitch_pix * sh;

	// check odd/even
	bool odd_left 	= (dx & 0x01);
	bool odd_top 	= (dy & 0x01);
	bool odd_right 	= ((dx + dw) & 0x01);
	bool odd_bottom = ((dy + dh) & 0x01);

	// pointer to the pixel components of the first pixel
	unsigned char *dst_y = dst + dx + dy * dst_pitch_pix;
	unsigned char *dst_u = dst + dst_pitch_pix * (dst_height + (dst_height >> 2)) + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	unsigned char *dst_v = dst + dst_pitch_pix *  dst_height                      + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;

	// offsets to the other three pixels
	unsigned int dst_y2_offs = 1;
	unsigned int dst_y3_offs = dst_pitch;
	unsigned int dst_y4_offs = dst_y3_offs + 1;
//	unsigned int src2_offs = 1;
//	unsigned int src3_offs = src_pitch_pix;
//	unsigned int src4_offs = src3_offs + 1;

	// arithmetic mean
	register unsigned int d_u;
	register unsigned int d_v;

	// prepare the color
	unsigned int SRC_Y = MMSFBSurface_RGB2Y(color.r, color.g, color.b);
	unsigned int SRC_U = MMSFBSurface_RGB2U(color.r, color.g, color.b);
	unsigned int SRC_V = MMSFBSurface_RGB2V(color.r, color.g, color.b);

	// draw odd pixels around the even rectangle
	if (odd_top && odd_left) {
/*		// odd top-left pixel
		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;*/
	}

	if (odd_top && odd_right) {
/*		// odd top-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom && odd_left) {
/*		// odd bottom-left pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom && odd_right) {
/*		// odd bottom-right pixel
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_top) {
/*		// odd top line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom) {
/*		// odd bottom line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_left) {
/*		// odd left line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_right) {
/*		// odd right line
		MMSFBSurface_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFBSurface_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFBSurface_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	// calc even positions...
	if (odd_top) {
		// odd top
		dy++;
		dh--;
		dst_y+=dst_pitch;
		dst_u+=dst_pitch >> 1;
		dst_v+=dst_pitch >> 1;
	}

	if (odd_bottom) {
		// odd bottom
		dh--;
	}

	if (odd_left) {
		// odd left
		dx++;
		dw--;
		dst_y++;
		dst_u++;
		dst_v++;
	}

	if (odd_right) {
		// odd right
		dw--;
	}


	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
	// clear Y
	unsigned char *dst_end = dst_y + dst_pitch_pix * dh;
	int dw_half = dw >> 1;
	while (dst_y < dst_end) {
		memset(dst_y, SRC_Y, dw);
		dst_y+= dst_pitch_pix;
	}
	// clear U
	dst_end = dst_u + dst_pitch_pix_half * (dh >> 1);
	while (dst_u < dst_end) {
		memset(dst_u, SRC_U, dw_half);
		dst_u+= dst_pitch_pix_half;
	}
	// clear V
	dst_end = dst_v + dst_pitch_pix_half * (dh >> 1);
	while (dst_v < dst_end) {
		memset(dst_v, SRC_V, dw_half);
		dst_v+= dst_pitch_pix_half;
	}
}



bool MMSFBSurface::extendedAccelFillRectangleEx(int x, int y, int w, int h) {

	// a few help and clipping values
	void *dst_ptr;
	int  dst_pitch;
	int sx = x;
	int sy = y;
	int sw = w;
	int sh = h;
	MMSFBRegion clipreg;
	int dst_height = (!this->root_parent)?this->config.h:this->root_parent->config.h;

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

	// calculate the color
	MMSFBColor color = this->config.color;
	if (this->config.drawingflags & (MMSFBDrawingFlags)MMSFB_DRAW_SRC_PREMULTIPLY) {
		// pre-multiplication needed
		if (color.a != 0xff) {
			color.r = ((color.a+1) * color.r) >> 8;
			color.g = ((color.a+1) * color.g) >> 8;
			color.b = ((color.a+1) * color.b) >> 8;
		}
	}

	// checking pixelformats...
	if (this->config.surface_buffer->pixelformat == MMSFB_PF_ARGB) {
		// destination is ARGB
		if   ((this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX))
			| (this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX|MMSFB_DRAW_SRC_PREMULTIPLY))) {
			// drawing without alpha channel
			if (extendedLock(NULL, NULL, NULL, this, &dst_ptr, &dst_pitch)) {
				eAFR_argb((unsigned int *)dst_ptr, dst_pitch, dst_height,
						  sx, sy, sw, sh, color);
				extendedUnlock(NULL, this);
				return true;
			}

			return false;
		}
		else
		if   ((this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_BLEND))
			| (this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_BLEND|MMSFB_DRAW_SRC_PREMULTIPLY))) {
			// drawing with alpha channel
			if (extendedLock(NULL, NULL, NULL, this, &dst_ptr, &dst_pitch)) {
				eAFR_blend_argb((unsigned int *)dst_ptr, dst_pitch, dst_height,
							    sx, sy, sw, sh, color);
				extendedUnlock(NULL, this);
				return true;
			}

			return false;
		}

		// does not match
		return false;
	}
	else
	if (this->config.surface_buffer->pixelformat == MMSFB_PF_AYUV) {
		// destination is AYUV
		if   ((this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX))
			| (this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX|MMSFB_DRAW_SRC_PREMULTIPLY))) {
			// drawing without alpha channel
			if (extendedLock(NULL, NULL, NULL, this, &dst_ptr, &dst_pitch)) {
				eAFR_ayuv((unsigned int *)dst_ptr, dst_pitch, dst_height,
						  sx, sy, sw, sh, color);
				extendedUnlock(NULL, this);
				return true;
			}

			return false;
		}
		else
		if   ((this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_BLEND))
			| (this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_BLEND|MMSFB_DRAW_SRC_PREMULTIPLY))) {
			// drawing with alpha channel
			if (extendedLock(NULL, NULL, NULL, this, &dst_ptr, &dst_pitch)) {
				eAFR_blend_ayuv((unsigned int *)dst_ptr, dst_pitch, dst_height,
							    sx, sy, sw, sh, color);
				extendedUnlock(NULL, this);
				return true;
			}

			return false;
		}

		// does not match
		return false;
	}
	else
	if (this->config.surface_buffer->pixelformat == MMSFB_PF_RGB16) {
		// destination is RGB16 (RGB565)
		if   ((this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX))
			| (this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX|MMSFB_DRAW_SRC_PREMULTIPLY))) {
			// drawing without alpha channel
			if (extendedLock(NULL, NULL, NULL, this, &dst_ptr, &dst_pitch)) {
				eAFR_rgb16((unsigned short *)dst_ptr, dst_pitch, dst_height,
						  sx, sy, sw, sh, color);
				extendedUnlock(NULL, this);
				return true;
			}

			return false;
		}

		// does not match
		return false;
	}
	else
	if (this->config.surface_buffer->pixelformat == MMSFB_PF_YV12) {
		// destination is YV12
		if   ((this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX))
			| (this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX|MMSFB_DRAW_SRC_PREMULTIPLY))) {
			// drawing without alpha channel
			if (extendedLock(NULL, NULL, NULL, this, &dst_ptr, &dst_pitch)) {
				eAFR_yv12((unsigned char *)dst_ptr, dst_pitch, dst_height,
						  sx, sy, sw, sh, color);
				extendedUnlock(NULL, this);
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


bool MMSFBSurface::extendedAccelFillRectangle(int x, int y, int w, int h) {

	// extended acceleration on?
	if (!this->extendedaccel)
		return false;

	if (!extendedAccelFillRectangleEx(x, y, w, h))
		return printMissingCombination("extendedAccelFillRectangle()");
	else
		return true;
}


#define MMSFB_DRAWLINE_PUT_PIXEL \
	if ((x >= clipreg.x1)&&(x <= clipreg.x2)&&(y >= clipreg.y1)&&(y <= clipreg.y2)) \
		dst[x+y*dst_pitch_pix]=SRC;

#define MMSFB_DRAWLINE_BLEND_PIXEL \
	if ((x >= clipreg.x1)&&(x <= clipreg.x2)&&(y >= clipreg.y1)&&(y <= clipreg.y2)) { \
		register unsigned int DST = dst[x+y*dst_pitch_pix]; \
		if (DST==OLDDST) dst[x+y*dst_pitch_pix] = d;  else { \
		OLDDST = DST; \
		register int SA= 0x100 - A; \
		unsigned int a = DST >> 24; \
		unsigned int r = (DST << 8) >> 24; \
		unsigned int g = (DST << 16) >> 24; \
		unsigned int b = DST & 0xff; \
	    a = (SA * a) >> 8; \
	    r = (SA * r) >> 8; \
	    g = (SA * g) >> 8; \
	    b = (SA * b) >> 8; \
	    a += A; \
	    r += (SRC << 8) >> 24; \
	    g += (SRC << 16) >> 24; \
	    b += SRC & 0xff; \
	    d =   ((a >> 8) ? 0xff000000 : (a << 24)) \
			| ((r >> 8) ? 0xff0000   : (r << 16)) \
			| ((g >> 8) ? 0xff00     : (g << 8)) \
	    	| ((b >> 8) ? 0xff 		 :  b); \
	    dst[x+y*dst_pitch_pix] = d; } }

#define MMSFB_DRAWLINE_BRESENHAM(putpixel) { \
	int x = x1; int y = y1; \
	int dx = x2 - x1; int dy = y2 - y1; \
	int ix = (dx > 0) ? 1 : (dx < 0) ? -1 : 0; int iy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0; \
	if (!dx && !dy) { putpixel; } else { \
	if (dx < 0) dx = -dx; if (dy < 0) dy = -dy; \
	int pdx, pdy, ddx, ddy, es, el; \
	if (dx > dy) { pdx=ix; pdy=0; ddx=ix; ddy=iy; es=dy; el=dx; } else { pdx=0;  pdy=iy; ddx=ix; ddy=iy; es=dx; el=dy; } \
	int err = el >> 1; putpixel; \
	for(int i = 0; i < el; ++i) { err-=es; if (err < 0) { err+=el; x+=ddx; y+=ddy; } else { x+=pdx; y+=pdy; } putpixel; } } }


void MMSFBSurface::eADL_argb(unsigned int *dst, int dst_pitch, int dst_height,
							 MMSFBRegion &clipreg, int x1, int y1, int x2, int y2, MMSFBColor &color) {
	// first time?
	if (firsttime_eADL_argb) {
		printf("DISKO: Using accelerated draw line to ARGB.\n");
		firsttime_eADL_argb = false;
	}

	// prepare...
	int dst_pitch_pix = dst_pitch >> 2;

	// prepare the color
	register unsigned int A = color.a;
	register unsigned int SRC;
	SRC =     (A << 24)
			| (color.r << 16)
			| (color.g << 8)
			| color.b;

	// draw a line with Bresenham-Algorithm
	MMSFB_DRAWLINE_BRESENHAM(MMSFB_DRAWLINE_PUT_PIXEL);
}


void MMSFBSurface::eADL_blend_argb(unsigned int *dst, int dst_pitch, int dst_height,
							       MMSFBRegion &clipreg, int x1, int y1, int x2, int y2, MMSFBColor &color) {
	if (color.a == 0xff) {
		// source pixel is not transparent
		eADL_argb(dst, dst_pitch, dst_height, clipreg, x1, y1, x2, y2, color);
		return;
	}

	// first time?
	if (firsttime_eADL_blend_argb) {
		printf("DISKO: Using accelerated blend line to ARGB.\n");
		firsttime_eADL_blend_argb = false;
	}

	// return immediately if alpha channel of the color is 0x00
	if (!color.a)
		return;

	// prepare...
	int dst_pitch_pix = dst_pitch >> 2;
	unsigned int OLDDST = 0;
	register unsigned int d = 0;

	// prepare the color
	register unsigned int A = color.a;
	register unsigned int SRC;
	SRC =     (A << 24)
			| (color.r << 16)
			| (color.g << 8)
			| color.b;
	d = SRC;

	// draw a line with Bresenham-Algorithm
	MMSFB_DRAWLINE_BRESENHAM(MMSFB_DRAWLINE_BLEND_PIXEL);
}


bool MMSFBSurface::extendedAccelDrawLineEx(int x1, int y1, int x2, int y2) {

	// check if we can use fill rectangle
	if (x1 == x2)
		return extendedAccelFillRectangle(x1, y1, 1, y2-y1+1);
	else
	if (y1 == y2)
		return extendedAccelFillRectangle(x1, y1, x2-x1+1, 1);

	// a few help and clipping values
	void *dst_ptr;
	int  dst_pitch;
	MMSFBRegion clipreg;
	int dst_height = (!this->root_parent)?this->config.h:this->root_parent->config.h;

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

	// calculate the color
	MMSFBColor color = this->config.color;
	if (this->config.drawingflags & (MMSFBDrawingFlags)MMSFB_DRAW_SRC_PREMULTIPLY) {
		// pre-multiplication needed
		if (color.a != 0xff) {
			color.r = ((color.a+1) * color.r) >> 8;
			color.g = ((color.a+1) * color.g) >> 8;
			color.b = ((color.a+1) * color.b) >> 8;
		}
	}

	// checking pixelformats...
	if (this->config.surface_buffer->pixelformat == MMSFB_PF_ARGB) {
		// destination is ARGB
		if   ((this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX))
			| (this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX|MMSFB_DRAW_SRC_PREMULTIPLY))) {
			// drawing without alpha channel
			if (extendedLock(NULL, NULL, NULL, this, &dst_ptr, &dst_pitch)) {
				eADL_argb((unsigned int *)dst_ptr, dst_pitch, dst_height, clipreg, x1, y1, x2, y2, color);
				extendedUnlock(NULL, this);
				return true;
			}

			return false;
		}
		else
		if   ((this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_BLEND))
			| (this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_BLEND|MMSFB_DRAW_SRC_PREMULTIPLY))) {
			if (extendedLock(NULL, NULL, NULL, this, &dst_ptr, &dst_pitch)) {
				// drawing with alpha channel
				eADL_blend_argb((unsigned int *)dst_ptr, dst_pitch, dst_height, clipreg, x1, y1, x2, y2, color);
				extendedUnlock(NULL, this);
				return true;
			}
		}
	}

	// does not match
	return false;
}


bool MMSFBSurface::extendedAccelDrawLine(int x1, int y1, int x2, int y2) {

	// extended acceleration on?
	if (!this->extendedaccel)
		return false;

	if (!extendedAccelDrawLineEx(x1, y1, x2, y2))
		return printMissingCombination("extendedAccelDrawLine()");
	else
		return true;
}





bool MMSFBSurface::blit(MMSFBSurface *source, MMSFBRectangle *src_rect, int x, int y) {
    MMSFBRectangle src;
    bool 		 ret = false;

    /* check if initialized */
    INITCHECK;

    if (src_rect) {
         src = *src_rect;
    }
    else {
         src.x = 0;
         src.y = 0;
         src.w = source->config.w;
         src.h = source->config.h;
    }

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult    dfbres;
	    D_DEBUG_AT( MMS_Surface, "blit( %d,%d - %dx%d -> %d,%d ) <- %dx%d\n",
	                DFB_RECTANGLE_VALS(&srcr), x, y, this->config.w, this->config.h );
	    MMSFB_TRACE();

	    /* blit */
		if (!this->is_sub_surface) {
			if (!extendedAccelBlit(source, &src, x, y))
				if ((dfbres=this->llsurface->Blit(this->llsurface, (IDirectFBSurface *)source->getDFBSurface(), (DFBRectangle*)&src, x, y)) != DFB_OK) {
					MMSFB_SetError(dfbres, "IDirectFBSurface::Blit() failed, src rect="
										   +iToStr(src.x)+","+iToStr(src.y)+","+iToStr(src.w)+","+iToStr(src.h)
										   +", dst="+iToStr(x)+","+iToStr(y));
					return false;
				}
			ret = true;
		}
		else {

#ifndef USE_DFB_SUBSURFACE
			CLIPSUBSURFACE

			x+=this->sub_surface_xoff;
			y+=this->sub_surface_yoff;

			SETSUBSURFACE_BLITTINGFLAGS;
#endif

			if (extendedAccelBlit(source, &src, x, y))
				ret = true;
			else
				if (this->llsurface->Blit(this->llsurface, (IDirectFBSurface *)source->getDFBSurface(), (DFBRectangle*)&src, x, y) == DFB_OK)
					ret = true;

#ifndef USE_DFB_SUBSURFACE
			RESETSUBSURFACE_BLITTINGFLAGS;

			UNCLIPSUBSURFACE
#endif

		}
#endif
	}
	else {

		/* blit */
		if (!this->is_sub_surface) {
			ret = extendedAccelBlit(source, &src, x, y);
		}
		else {
			CLIPSUBSURFACE

			x+=this->sub_surface_xoff;
			y+=this->sub_surface_yoff;

			ret = extendedAccelBlit(source, &src, x, y);

			UNCLIPSUBSURFACE

		}

	}

    return ret;
}


bool MMSFBSurface::blitBuffer(MMSFBExternalSurfaceBuffer *extbuf, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
							  MMSFBRectangle *src_rect, int x, int y) {
    MMSFBRectangle src;
    bool 		 ret = false;

    if (src_rect) {
         src = *src_rect;
    }
    else {
         src.x = 0;
         src.y = 0;
         src.w = src_width;
         src.h = src_height;
    }

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
#endif
	}
	else {
		/* blit buffer */
		if (!this->is_sub_surface) {
			ret = extendedAccelBlitBuffer(extbuf, src_pixelformat, src_width, src_height, &src, x, y);
		}
		else {
			CLIPSUBSURFACE

			x+=this->sub_surface_xoff;
			y+=this->sub_surface_yoff;

			ret = extendedAccelBlitBuffer(extbuf, src_pixelformat, src_width, src_height, &src, x, y);

			UNCLIPSUBSURFACE

		}
	}

    return ret;
}

bool MMSFBSurface::blitBuffer(void *src_ptr, int src_pitch, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
							  MMSFBRectangle *src_rect, int x, int y) {
	MMSFBExternalSurfaceBuffer extbuf;
	memset(&extbuf, 0, sizeof(extbuf));
	extbuf.ptr = src_ptr;
	extbuf.pitch = src_pitch;
	return blitBuffer(&extbuf, src_pixelformat, src_width, src_height, src_rect, x, y);
}

bool MMSFBSurface::stretchBlit(MMSFBSurface *source, MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect) {
    MMSFBRectangle src;
    MMSFBRectangle dst;
    bool 		 ret = false;

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

    /* check if i can blit without stretching */
    if (src.w == dst.w && src.h == dst.h)
        return blit(source, &src, dst.x, dst.y);

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult    dfbres;
	    bool         blit_done = false;
	    D_DEBUG_AT( MMS_Surface, "stretchBlit( %d,%d - %dx%d  ->  %d,%d - %dx%d ) <- %dx%d\n",
	                DFB_RECTANGLE_VALS(&src), DFB_RECTANGLE_VALS(&dst), this->config.w, this->config.h);
	    MMSFB_BREAK();

		if (this->config.blittingflags != MMSFB_BLIT_NOFX) {
			/* stretch blit with blitting flags */

			if (!this->is_sub_surface) {
				if (extendedAccelStretchBlit(source, &src, &dst)) {
					blit_done = true;
					ret = true;
				}
			}
			else {

#ifndef USE_DFB_SUBSURFACE
				CLIPSUBSURFACE

				dst.x+=this->sub_surface_xoff;
				dst.y+=this->sub_surface_yoff;

				SETSUBSURFACE_BLITTINGFLAGS;
#endif

				if (extendedAccelStretchBlit(source, &src, &dst)) {
					blit_done = true;
					ret = true;
				}


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
					MMSFBRectangle temp;
					temp.x=0;
					temp.y=0;
					temp.w=dst.w;
					temp.h=dst.h;

					dfbres = DFB_OK;
					dfbres=((IDirectFBSurface *)tempsuf->getDFBSurface())->StretchBlit((IDirectFBSurface *)tempsuf->getDFBSurface(), (IDirectFBSurface *)source->getDFBSurface(), (DFBRectangle*)&src, (DFBRectangle*)&temp);
					if (dfbres == DFB_OK) {
						if (!this->is_sub_surface) {
							if (extendedAccelBlit(tempsuf, &temp, dst.x, dst.y)) {
								blit_done = true;
								ret = true;
							}
							else
							if ((dfbres=this->llsurface->Blit(this->llsurface, (IDirectFBSurface *)tempsuf->getDFBSurface(), (DFBRectangle*)&temp, dst.x, dst.y)) == DFB_OK) {
								blit_done = true;
								ret = true;
							}
						}
						else {

#ifndef USE_DFB_SUBSURFACE
							CLIPSUBSURFACE

							dst.x+=this->sub_surface_xoff;
							dst.y+=this->sub_surface_yoff;

							SETSUBSURFACE_BLITTINGFLAGS;
#endif

							if (!extendedAccelBlit(tempsuf, &temp, dst.x, dst.y))
								this->llsurface->Blit(this->llsurface, (IDirectFBSurface *)tempsuf->getDFBSurface(), (DFBRectangle*)&temp, dst.x, dst.y);

#ifndef USE_DFB_SUBSURFACE
							RESETSUBSURFACE_BLITTINGFLAGS;

							dst.x-=this->sub_surface_xoff;
							dst.y-=this->sub_surface_yoff;

							UNCLIPSUBSURFACE
#endif

							blit_done = true;
							ret = true;

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
					dfbres=this->llsurface->StretchBlit(this->llsurface, (IDirectFBSurface *)source->getDFBSurface(), (DFBRectangle*)&src, (DFBRectangle*)&dst);
				if (dfbres != DFB_OK) {
					MMSFB_SetError(dfbres, "IDirectFBSurface::StretchBlit() failed");
					return false;
				}
				ret = true;
			}
			else {

#ifndef USE_DFB_SUBSURFACE
				CLIPSUBSURFACE

				dst.x+=this->sub_surface_xoff;
				dst.y+=this->sub_surface_yoff;

				SETSUBSURFACE_BLITTINGFLAGS;
#endif

				if (!extendedAccelStretchBlit(source, &src, &dst))
					this->llsurface->StretchBlit(this->llsurface, (IDirectFBSurface *)source->getDFBSurface(), (DFBRectangle*)&src, (DFBRectangle*)&dst);
				ret = true;

#ifndef USE_DFB_SUBSURFACE
				RESETSUBSURFACE_BLITTINGFLAGS;

				dst.x-=this->sub_surface_xoff;
				dst.y-=this->sub_surface_yoff;

				UNCLIPSUBSURFACE
#endif
			}
		}
#endif
	}
	else {

		/* normal stretch blit */
		if (!this->is_sub_surface) {
			ret = extendedAccelStretchBlit(source, &src, &dst);
		}
		else {
			CLIPSUBSURFACE

			dst.x+=this->sub_surface_xoff;
			dst.y+=this->sub_surface_yoff;

			ret = extendedAccelStretchBlit(source, &src, &dst);

			dst.x-=this->sub_surface_xoff;
			dst.y-=this->sub_surface_yoff;

			UNCLIPSUBSURFACE
		}

	}

    return ret;
}

bool MMSFBSurface::stretchBlitBuffer(MMSFBExternalSurfaceBuffer *extbuf, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
									 MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect) {
    MMSFBRectangle src;
    MMSFBRectangle dst;
    bool ret = false;

    if (src_rect) {
         src = *src_rect;
    }
    else {
         src.x = 0;
         src.y = 0;
         src.w = src_width;
         src.h = src_height;
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

    // check if i can blit without stretching
    if (src.w == dst.w && src.h == dst.h)
        return blitBuffer(extbuf->ptr, extbuf->pitch, src_pixelformat, src_width, src_height, &src, dst.x, dst.y);

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
#endif
	}
	else {
		/* normal stretch blit */
		if (!this->is_sub_surface) {
			ret = extendedAccelStretchBlitBuffer(extbuf, src_pixelformat, src_width, src_height, &src, &dst);
		}
		else {
			CLIPSUBSURFACE

			dst.x+=this->sub_surface_xoff;
			dst.y+=this->sub_surface_yoff;

			ret = extendedAccelStretchBlitBuffer(extbuf, src_pixelformat, src_width, src_height, &src, &dst);

			dst.x-=this->sub_surface_xoff;
			dst.y-=this->sub_surface_yoff;

			UNCLIPSUBSURFACE
		}

	}

    return ret;
}

bool MMSFBSurface::stretchBlitBuffer(void *src_ptr, int src_pitch, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
									 MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect) {
	MMSFBExternalSurfaceBuffer extbuf;
	memset(&extbuf, 0, sizeof(extbuf));
	extbuf.ptr = src_ptr;
	extbuf.pitch = src_pitch;
	return stretchBlitBuffer(&extbuf, src_pixelformat, src_width, src_height, src_rect, dest_rect);
}



bool MMSFBSurface::flip(MMSFBRegion *region) {

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    if (region)
	         D_DEBUG_AT( MMS_Surface, "flip( %d,%d - %dx%d ) <- %dx%d\n",
	                     DFB_RECTANGLE_VALS_FROM_REGION(region), this->config.w, this->config.h );
	    else
	         D_DEBUG_AT( MMS_Surface, "flip( %d,%d - %dx%d ) <- %dx%d\n",
	                     0, 0, this->config.w, this->config.h, this->config.w, this->config.h );

	    MMSFB_TRACE();

	    DFBResult   dfbres;

#ifdef USE_DFB_WINMAN

		/* flip */
		if ((dfbres=this->llsurface->Flip(this->llsurface, region, this->flipflags)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBSurface::Flip() failed");

			return false;
		}

#endif

#ifdef USE_MMSFB_WINMAN

		if (!this->config.islayersurface) {
			/* flip */
			if (!this->is_sub_surface) {
				if ((dfbres=this->llsurface->Flip(this->llsurface, (DFBRegion*)region, getDFBSurfaceFlipFlagsFromMMSFBFlipFlags(this->flipflags))) != DFB_OK) {
					MMSFB_SetError(dfbres, "IDirectFBSurface::Flip() failed");

					return false;
				}
			}
			else {
#ifndef USE_DFB_SUBSURFACE
				CLIPSUBSURFACE

				MMSFBRegion myregion;
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

				this->llsurface->Flip(this->llsurface, (DFBRegion*)&myregion, getDFBSurfaceFlipFlagsFromMMSFBFlipFlags(this->flipflags));

#else
				this->llsurface->Flip(this->llsurface, region, getDFBSurfaceFlipFlagsFromMMSFBFlipFlags(this->flipflags));
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
#else
		return false;
#endif
	}
	else {
		// flip my own surfaces
		MMSFBSurfaceBuffer *sb = this->config.surface_buffer;
		if (sb->numbuffers > 1) {
			// flip is only needed, if we have at least one backbuffer
			if (!this->config.islayersurface) {
				// flip
				if (!this->is_sub_surface) {
					// not a subsurface
					if (!region) {
						// flip my buffers without blitting
						sb->currbuffer_read++;
						if (sb->currbuffer_read >= sb->numbuffers)
							sb->currbuffer_read = 0;
						sb->currbuffer_write++;
						if (sb->currbuffer_write >= sb->numbuffers)
							sb->currbuffer_write = 0;
					}
					else {
						MMSFBRectangle src_rect;
						src_rect.x = region->x1;
						src_rect.y = region->y1;
						src_rect.w = region->x2 - region->x1 + 1;
						src_rect.h = region->y2 - region->y1 + 1;

						// check if region is equal to the whole surface
						if   ((src_rect.x == 0) && (src_rect.y == 0)
							&&(src_rect.w == this->config.w) && (src_rect.h == this->config.h)) {
							// yes, flip my buffers without blitting
							sb->currbuffer_read++;
							if (sb->currbuffer_read >= sb->numbuffers)
								sb->currbuffer_read = 0;
							sb->currbuffer_write++;
							if (sb->currbuffer_write >= sb->numbuffers)
								sb->currbuffer_write = 0;
						}
						else {
							// blit region from write to read buffer of the same MMSFBSurface
							MMSFBBlittingFlags savedbf = this->config.blittingflags;
							this->config.blittingflags = (MMSFBBlittingFlags)MMSFB_BLIT_NOFX;

							this->surface_invert_lock = true;
							this->extendedAccelBlit(this, &src_rect, src_rect.x, src_rect.y);
							this->surface_invert_lock = false;

							this->config.blittingflags = savedbf;
						}
					}
				}
				else {
					CLIPSUBSURFACE

					MMSFBRectangle src_rect;
					if (!region) {
						src_rect.x = 0;
						src_rect.y = 0;
						src_rect.w = this->config.w;
						src_rect.h = this->config.h;
					}
					else {
						src_rect.x = region->x1;
						src_rect.y = region->y1;
						src_rect.w = region->x2 - region->x1 + 1;
						src_rect.h = region->y2 - region->y1 + 1;
					}

					src_rect.x+=this->sub_surface_xoff;
					src_rect.y+=this->sub_surface_yoff;

					// blit region from write to read buffer of the same MMSFBSurface
					MMSFBBlittingFlags savedbf = this->config.blittingflags;
					this->config.blittingflags = (MMSFBBlittingFlags)MMSFB_BLIT_NOFX;

					this->surface_invert_lock = true;
					this->extendedAccelBlit(this, &src_rect, src_rect.x, src_rect.y);
					this->surface_invert_lock = false;

					this->config.blittingflags = savedbf;

					UNCLIPSUBSURFACE
				}
			}
		}

#ifdef __HAVE_XLIB__
		if (sb->xv_image[0]) {
			// put the image to the x-server
			mmsfb->xlock.lock();
			XLockDisplay(mmsfb->x_display);
			if(mmsfb->fullscreen) {
				XvShmPutImage(mmsfb->x_display, mmsfb->xv_port, mmsfb->x_window, mmsfb->x_gc, sb->xv_image[sb->currbuffer_read],
							  0, 0, mmsfb->w, mmsfb->h,
							  0, 0, mmsfb->display_w, mmsfb->display_h, True);

			} else if(mmsfb->resized) {
				printf("stretch to %d:%d\n",mmsfb->target_window_w, mmsfb->target_window_h);
				XvShmPutImage(mmsfb->x_display, mmsfb->xv_port, mmsfb->x_window, mmsfb->x_gc, sb->xv_image[sb->currbuffer_read],
							  0, 0, mmsfb->w, mmsfb->h,
							  0, 0, mmsfb->target_window_w, mmsfb->target_window_h, True);
			}else{
				XvShmPutImage(mmsfb->x_display, mmsfb->xv_port, mmsfb->x_window, mmsfb->x_gc, sb->xv_image[sb->currbuffer_read],
							  0, 0, mmsfb->w, mmsfb->h,
							  0, 0, mmsfb->w, mmsfb->h, True);
			}
			XFlush(mmsfb->x_display);
#ifndef __NO_XSYNC__
//			XSync(mmsfb->x_display, True);
#endif
			XUnlockDisplay(mmsfb->x_display);
			mmsfb->xlock.unlock();
		}
#endif

		if (this->config.iswinsurface) {
			/* inform the window manager */
			mmsfbwindowmanager->flipSurface(this, region);
		}

		return true;
	}
}

bool MMSFBSurface::refresh() {
    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
#endif
	}
	else {
#ifdef __HAVE_XLIB__
		MMSFBSurfaceBuffer *sb = this->config.surface_buffer;
		if (sb->xv_image[0]) {
			// put the image to the x-server
			this->lock();
			mmsfb->xlock.lock();
			XLockDisplay(mmsfb->x_display);
			if(mmsfb->fullscreen) {
				XvShmPutImage(mmsfb->x_display, mmsfb->xv_port, mmsfb->x_window, mmsfb->x_gc, sb->xv_image[sb->currbuffer_read],
							  0, 0, mmsfb->w, mmsfb->h,
							  0, 0, mmsfb->display_w, mmsfb->display_h, True);
			} else if(mmsfb->resized) {
				XvShmPutImage(mmsfb->x_display, mmsfb->xv_port, mmsfb->x_window, mmsfb->x_gc, sb->xv_image[sb->currbuffer_read],
							  0, 0, mmsfb->w, mmsfb->h,
							  0, 0, mmsfb->target_window_w, mmsfb->target_window_h, True);
			}else{
				XvShmPutImage(mmsfb->x_display, mmsfb->xv_port, mmsfb->x_window, mmsfb->x_gc, sb->xv_image[sb->currbuffer_read],
							  0, 0, mmsfb->w, mmsfb->h,
							  0, 0, mmsfb->w, mmsfb->h, True);
			}
			XFlush(mmsfb->x_display);
#ifndef __NO_XSYNC__
//			XSync(mmsfb->x_display, True);
#endif
			XUnlockDisplay(mmsfb->x_display);
			mmsfb->xlock.unlock();
			this->unlock();
		}
#endif
	}

	return true;
}

bool MMSFBSurface::createCopy(MMSFBSurface **dstsurface, int w, int h,
                              bool copycontent, bool withbackbuffer) {

    /* check if initialized */
    INITCHECK;

    if (this->is_sub_surface)
    	return false;

    *dstsurface = NULL;

    if (!w) w = config.w;
    if (!h) h = config.h;

    /* create new surface */
    if (!mmsfb->createSurface(dstsurface, w, h, this->config.surface_buffer->pixelformat,
                             (withbackbuffer)?this->config.surface_buffer->backbuffer:0,this->config.surface_buffer->systemonly)) {
        if (*dstsurface)
            delete *dstsurface;
        *dstsurface = NULL;
        return false;
    }

    if (copycontent) {
        /* copy the content */
        MMSFBRectangle dstrect;
        dstrect.x = 0;
        dstrect.y = 0;
        dstrect.w = w;
        dstrect.h = h;
        (*dstsurface)->setDrawingFlags((MMSFBDrawingFlags) MMSFB_DRAW_NOFX);
        (*dstsurface)->setBlittingFlags((MMSFBBlittingFlags) MMSFB_BLIT_NOFX);
        (*dstsurface)->stretchBlit(this, NULL, &dstrect);
        if (withbackbuffer) {
            (*dstsurface)->flip();
        }
    }

    return true;
}

bool MMSFBSurface::resize(int w, int h) {

    /* check if initialized */
    INITCHECK;

    if (!this->is_sub_surface) {
        /* normal surface */
	    lock();

	    /* create a copy */
	    MMSFBSurface *dstsurface;
	    createCopy(&dstsurface, w, h, true, true);

		if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
		    D_DEBUG_AT( MMS_Surface, "resize( %dx%d -> %dx%d )\n",
		                this->config.w, this->config.h, w, h );

		    MMSFB_TRACE();

		    /* move the dfb pointers */
			IDirectFBSurface *s = this->llsurface;
			this->llsurface = dstsurface->llsurface;
			dstsurface->llsurface = s;

			/* load the new configuration */
			this->getConfiguration();
			dstsurface->getConfiguration();
#endif
		}
		else {
			// move the surface buffer data
			MMSFBSurfaceBuffer *sb = this->config.surface_buffer;
			this->config.surface_buffer = dstsurface->config.surface_buffer;
			dstsurface->config.surface_buffer = sb;

			// load the new configuration
			this->getConfiguration();
		}

	    /* free dstsurface */
	    delete dstsurface;

	    unlock();
	    return true;
    }
    else  {
    	/* sub surface */
	    MMSFBRectangle rect = this->sub_surface_rect;
	    rect.w = w;
	    rect.h = h;
	    return setSubSurface(&rect);

    }
}



void MMSFBSurface::modulateBrightness(MMSFBColor *color, unsigned char brightness) {

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

void MMSFBSurface::modulateOpacity(MMSFBColor *color, unsigned char opacity) {

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
    MMSFBColor color;

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
            setBlittingFlags((MMSFBBlittingFlags)(MMSFB_BLIT_COLORIZE|MMSFB_BLIT_BLEND_ALPHACHANNEL));
        else
            setBlittingFlags((MMSFBBlittingFlags)(MMSFB_BLIT_COLORIZE|MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA));
    }
    else {
        if (color.a == 255)
            setBlittingFlags((MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL));
        else
            setBlittingFlags((MMSFBBlittingFlags)(MMSFB_BLIT_BLEND_ALPHACHANNEL|MMSFB_BLIT_BLEND_COLORALPHA));
    }

    return true;
}

bool MMSFBSurface::setDrawingFlagsByAlpha(unsigned char alpha) {

    // check if initialized
    INITCHECK;

    // set the drawing flags
    if (this->config.surface_buffer->premultiplied) {
    	// premultiplied surface, have to premultiply the color
	    if (alpha == 255)
	        setDrawingFlags((MMSFBDrawingFlags)(MMSFB_DRAW_NOFX|MMSFB_DRAW_SRC_PREMULTIPLY));
	    else
	        setDrawingFlags((MMSFBDrawingFlags)(MMSFB_DRAW_BLEND|MMSFB_DRAW_SRC_PREMULTIPLY));
    }
    else {
	    if (alpha == 255)
	        setDrawingFlags((MMSFBDrawingFlags)MMSFB_DRAW_NOFX);
	    else
	        setDrawingFlags((MMSFBDrawingFlags)MMSFB_DRAW_BLEND);
    }

    return true;
}


bool MMSFBSurface::setDrawingColorAndFlagsByBrightnessAndOpacity(
                        MMSFBColor color, unsigned char brightness, unsigned char opacity) {

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


bool MMSFBSurface::setFont(MMSFBFont *font) {

    /* check if initialized */
    INITCHECK;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    DFBResult   dfbres;

	    /* set font */
		if ((dfbres=this->llsurface->SetFont(this->llsurface, (IDirectFBFont*)font->dfbfont)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFBSurface::SetFont() failed");
			return false;
		}
#endif
	}
	else {
		//TODO
	}

    /* save the font */
    this->config.font = font;

    return true;
}

#ifdef __HAVE_XLIB__

#define MMSFBSURFACE_BLIT_TEXT_LOCK(pw) \
	MMSFBFont *font = this->config.font; \
	font->lock(); \
	int DY = font->height - font->descender - 1; \
	void *ptr; int dst_pitch; \
	this->lock(MMSFB_LOCK_WRITE, &ptr, &dst_pitch); \
	int dst_pitch_pix = dst_pitch >> pw;

#define MMSFBSURFACE_BLIT_TEXT_UNLOCK this->unlock(); font->unlock();

#define MMSFBSURFACE_BLIT_TEXT_LOAD_GLYPH(character) \
	int			  src_pitch_pix; \
	int 		  src_w; \
	int 		  src_h; \
	unsigned char *src; \
	MMSFBFont_Glyph *glyph = font->getGlyph(character); \
	if (glyph) { \
		src_pitch_pix = glyph->pitch; \
		src_w         = glyph->width; \
		src_h         = glyph->height; \
		src           = glyph->buffer; \
	}

#define MMSFBSURFACE_BLIT_TEXT_START_RENDER(pt) \
	if (glyph) { \
		if   (((x + src_w > clipreg.x1)&&(x <= clipreg.x2)) \
			&&((y + src_h > clipreg.y1)&&(y <= clipreg.y2))) { \
			int dx = x + glyph->left; \
			int dy = y + DY - glyph->top; \
			if (dx < clipreg.x1) { \
				src_w -= clipreg.x1 - dx; \
				src   += clipreg.x1 - dx; \
				dx     = clipreg.x1; } \
			if (dx + src_w - 1 > clipreg.x2) src_w = clipreg.x2 - dx + 1; \
			if (dy < clipreg.y1) { \
				src_h -= clipreg.y1 - dy; \
				src   +=(clipreg.y1 - dy) * src_pitch_pix; \
				dy     = clipreg.y1; } \
			if (dy + src_h - 1 > clipreg.y2) src_h = clipreg.y2 - dy + 1; \
			unsigned char *src_end = src + src_h * src_pitch_pix; \
			unsigned char *line_end = src + src_w; \
			int src_pitch_pix_diff = src_pitch_pix - src_w; \
			int dst_pitch_pix_diff = dst_pitch_pix - src_w; \
			pt *dst = ((pt *)ptr) + dx + dy * dst_pitch_pix;

#define MMSFBSURFACE_BLIT_TEXT_END_RENDER } x+=glyph->advanceX >> 6; }



void MMSFBSurface::blend_text_to_argb(MMSFBRegion &clipreg, string &text, int len, int x, int y, MMSFBColor &color) {

	// first time?
	if (firsttime_blend_text_to_argb) {
		printf("DISKO: Using blend text to ARGB.\n");
		firsttime_blend_text_to_argb = false;
	}

	// lock font and destination surface
	MMSFBSURFACE_BLIT_TEXT_LOCK(2);

	// for all characters
	unsigned int OLDDST = 0;
	unsigned int OLDSRC = 0;
	register unsigned int d = 0;
	register unsigned int SRCPIX = 0xff000000 | (((unsigned int)color.r) << 16) | (((unsigned int)color.g) << 8) | (unsigned int)color.b;
	MMSFBFONT_GET_UNICODE_CHAR(text, len) {
		// load the glyph
		MMSFBSURFACE_BLIT_TEXT_LOAD_GLYPH(character);

		// start rendering of glyph to destination
		MMSFBSURFACE_BLIT_TEXT_START_RENDER(unsigned int);

		// through the pixels
		while (src < src_end) {
			while (src < line_end) {
				// load pixel from memory
				register unsigned int SRC = *src;

				// is the source alpha channel 0x00 or 0xff?
				register unsigned int A = SRC;
				if (A == 0xff) {
					// source pixel is not transparent, copy it directly to the destination
					*dst = SRCPIX;
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
				    A++;
				    r += (A * color.r) >> 8;
				    g += (A * color.g) >> 8;
				    b += (A * color.b) >> 8;
				    d =   ((a >> 8) ? 0xff000000 : (a << 24))
						| ((r >> 8) ? 0xff0000   : (r << 16))
						| ((g >> 8) ? 0xff00     : (g << 8))
				    	| ((b >> 8) ? 0xff 		 :  b);
					*dst = d;
				}

				src++;
				dst++;
			}
			line_end+= src_pitch_pix;
			src     += src_pitch_pix_diff;
			dst     += dst_pitch_pix_diff;
		}

		// prepare for next loop
		MMSFBSURFACE_BLIT_TEXT_END_RENDER;
	}}

	// unlock
	MMSFBSURFACE_BLIT_TEXT_UNLOCK;
}

void MMSFBSurface::blend_text_srcalpha_to_argb(MMSFBRegion &clipreg, string &text, int len, int x, int y, MMSFBColor &color) {

	// check for full alpha value
	if (color.a == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		blend_text_to_argb(clipreg, text, len, x, y, color);
		return;
	}

	// first time?
	if (firsttime_blend_text_srcalpha_to_argb) {
		printf("DISKO: Using blend text srcalpha to ARGB.\n");
		firsttime_blend_text_srcalpha_to_argb = false;
	}

	// something to do?
	if (!color.a)
		// source should blitted full transparent, so leave destination as is
		return;

	// lock font and destination surface
	MMSFBSURFACE_BLIT_TEXT_LOCK(2);

	// for all characters
	unsigned int OLDDST = 0;
	unsigned int OLDSRC = 0;
	register unsigned int d = 0;
	register unsigned int ALPHA = color.a;
	ALPHA++;
	MMSFBFONT_GET_UNICODE_CHAR(text, len) {
		// load the glyph
		MMSFBSURFACE_BLIT_TEXT_LOAD_GLYPH(character);

		// start rendering of glyph to destination
		MMSFBSURFACE_BLIT_TEXT_START_RENDER(unsigned int);

		// through the pixels
		while (src < src_end) {
			while (src < line_end) {
				// load pixel from memory
				register unsigned int SRC = *src;

				// is the source alpha channel 0x00 or 0xff?
				register unsigned int A = SRC;
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

				    A = (ALPHA * A) >> 8;
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
				    A++;
				    r += (A * color.r) >> 8;
				    g += (A * color.g) >> 8;
				    b += (A * color.b) >> 8;
				    d =   ((a >> 8) ? 0xff000000 : (a << 24))
						| ((r >> 8) ? 0xff0000   : (r << 16))
						| ((g >> 8) ? 0xff00     : (g << 8))
				    	| ((b >> 8) ? 0xff 		 :  b);
					*dst = d;
				}

				src++;
				dst++;
			}
			line_end+= src_pitch_pix;
			src     += src_pitch_pix_diff;
			dst     += dst_pitch_pix_diff;
		}

		// prepare for next loop
		MMSFBSURFACE_BLIT_TEXT_END_RENDER;
	}}

	// unlock
	MMSFBSURFACE_BLIT_TEXT_UNLOCK;
}

bool MMSFBSurface::blit_text(string &text, int len, int x, int y) {
	MMSFBRegion clipreg;

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

	// calculate the color
	MMSFBColor color = this->config.color;
	if (this->config.drawingflags & (MMSFBDrawingFlags)MMSFB_DRAW_SRC_PREMULTIPLY) {
		// pre-multiplication needed
		if (color.a != 0xff) {
			color.r = ((color.a+1) * color.r) >> 8;
			color.g = ((color.a+1) * color.g) >> 8;
			color.b = ((color.a+1) * color.b) >> 8;
		}
	}

	// checking pixelformats...
	if (this->config.surface_buffer->pixelformat == MMSFB_PF_ARGB) {
		// destination is ARGB
		if   ((this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX))
			| (this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_NOFX|MMSFB_DRAW_SRC_PREMULTIPLY))) {
			blend_text_to_argb(clipreg, text, len, x, y, color);
			return true;
		}
		else
		if   ((this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_BLEND))
			| (this->config.drawingflags == (MMSFBDrawingFlags)(MMSFB_DRAW_BLEND|MMSFB_DRAW_SRC_PREMULTIPLY))) {
			blend_text_srcalpha_to_argb(clipreg, text, len, x, y, color);
			return true;
		}
	}
	return printMissingCombination("blit_text()", NULL, NULL, MMSFB_PF_NONE, 0, 0);
}

#endif

bool MMSFBSurface::drawString(string text, int len, int x, int y) {

    // check if initialized
    INITCHECK;

    if (!this->config.font)
    	return false;

	// get the length of the string
	if (len < 0) len = text.size();
	if (!len) return true;

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
	    D_DEBUG_AT( MMS_Surface, "drawString( '%s', %d, %d,%d ) <- %dx%d\n",
	                text.c_str(), len, x, y, this->config.w, this->config.h );
	    MMSFB_TRACE();

	    // draw a string
	    DFBResult dfbres;
		if (!this->is_sub_surface) {
			if ((dfbres=this->llsurface->DrawString(this->llsurface, text.c_str(), len, x, y, DSTF_TOPLEFT)) != DFB_OK) {
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

			this->llsurface->DrawString(this->llsurface, text.c_str(), len, x, y, DSTF_TOPLEFT);

#ifndef USE_DFB_SUBSURFACE
			RESETSUBSURFACE_DRAWINGFLAGS;

			UNCLIPSUBSURFACE
#endif
		}
#endif
	}
	else {
#ifdef __HAVE_XLIB__
		// draw a string
		if (!this->is_sub_surface) {
			blit_text(text, len, x, y);
		}
		else {
			CLIPSUBSURFACE

			x+=this->sub_surface_xoff;
			y+=this->sub_surface_yoff;

			blit_text(text, len, x, y);

			UNCLIPSUBSURFACE
		}
#endif
	}

    return true;
}

void MMSFBSurface::lock(MMSFBLockFlags flags, void **ptr, int *pitch, bool pthread_lock) {
	if (!pthread_lock) {
		// no pthread lock needed
		if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
			if (flags && ptr && pitch) {
				// get the access to the surface buffer
				*ptr = NULL;
				*pitch = 0;
				if (flags == MMSFB_LOCK_READ) {
					if (this->llsurface->Lock(this->llsurface, DSLF_READ, ptr, pitch) != DFB_OK) {
						*ptr = NULL;
						*pitch = 0;
					}
				}
				else
				if (flags == MMSFB_LOCK_WRITE) {
					if (this->llsurface->Lock(this->llsurface, DSLF_WRITE, ptr, pitch) != DFB_OK) {
						*ptr = NULL;
						*pitch = 0;
					}
				}
			}
#endif
		}
		else {
			if (flags && ptr && pitch) {
				// get the access to the surface buffer
				*ptr = NULL;
				*pitch = 0;
				MMSFBSurfaceBuffer *sb = this->config.surface_buffer;
				if (flags == MMSFB_LOCK_READ) {
					*ptr = sb->buffers[sb->currbuffer_read];
					*pitch = sb->pitch;
				}
				else
				if (flags == MMSFB_LOCK_WRITE) {
					*ptr = sb->buffers[sb->currbuffer_write];
					*pitch = sb->pitch;
				}
			}
		}
		return;
	}

	// which surface is to lock?
	MMSFBSurface *tolock = this;
	if (this->root_parent)
		tolock = this->root_parent;
	else
	if (this->parent)
		tolock = this->parent;

    if (tolock->Lock.trylock() == 0) {
        // I have got the lock the first time
    	tolock->TID = pthread_self();
    	tolock->Lock_cnt = 1;
    }
    else {
        if ((tolock->TID == pthread_self())&&(tolock->Lock_cnt > 0)) {
            // I am the thread which has already locked this surface
        	tolock->Lock_cnt++;
        }
        else {
            // another thread has already locked this surface, waiting for...
        	tolock->Lock.lock();
        	tolock->TID = pthread_self();
        	tolock->Lock_cnt = 1;
        }
    }

	if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
		if (flags && ptr && pitch) {
			// get the access to the surface buffer
			*ptr = NULL;
			*pitch = 0;
			if (flags == MMSFB_LOCK_READ) {
				if (!tolock->surface_read_locked) {
					if (this->llsurface->Lock(this->llsurface, DSLF_READ, ptr, pitch) != DFB_OK) {
						*ptr = NULL;
						*pitch = 0;
					}
					else {
						tolock->surface_read_locked = true;
						tolock->surface_read_lock_cnt = tolock->Lock_cnt;
					}
				}
			}
			else
			if (flags == MMSFB_LOCK_WRITE) {
				if (!tolock->surface_write_locked) {
					if (this->llsurface->Lock(this->llsurface, DSLF_WRITE, ptr, pitch) != DFB_OK) {
						*ptr = NULL;
						*pitch = 0;
					}
					else {
						tolock->surface_write_locked = true;
						tolock->surface_write_lock_cnt = tolock->Lock_cnt;
					}
				}
			}
		}
#endif
	}
	else {
		if (flags && ptr && pitch) {
			// get the access to the surface buffer
			*ptr = NULL;
			*pitch = 0;
			MMSFBSurfaceBuffer *sb = this->config.surface_buffer;
			if (flags == MMSFB_LOCK_READ) {
				if (!tolock->surface_read_locked) {
					*ptr = sb->buffers[sb->currbuffer_read];
					*pitch = sb->pitch;
					tolock->surface_read_locked = true;
					tolock->surface_read_lock_cnt = tolock->Lock_cnt;
				}
			}
			else
			if (flags == MMSFB_LOCK_WRITE) {
				if (!tolock->surface_write_locked) {
					*ptr = sb->buffers[sb->currbuffer_write];
					*pitch = sb->pitch;
					tolock->surface_write_locked = true;
					tolock->surface_write_lock_cnt = tolock->Lock_cnt;
				}
			}
		}
	}
}

void MMSFBSurface::lock(MMSFBLockFlags flags, void **ptr, int *pitch) {
	lock(flags, ptr, pitch, true);
}

void MMSFBSurface::unlock(bool pthread_unlock) {
	if (!pthread_unlock) {
		// no pthread unlock needed
		if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
			this->llsurface->Unlock(this->llsurface);
#endif
		}
		return;
	}

	// which surface is to lock?
	MMSFBSurface *tolock = this;
	if (this->root_parent)
		tolock = this->root_parent;
	else
	if (this->parent)
		tolock = this->parent;

	if (tolock->TID != pthread_self())
        return;

    if (tolock->Lock_cnt==0)
    	return;

	// unlock dfb surface?
	if ((tolock->surface_read_locked)&&(tolock->surface_read_lock_cnt == tolock->Lock_cnt)) {
		if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
			this->llsurface->Unlock(this->llsurface);
#endif
		}
		tolock->surface_read_locked = false;
		tolock->surface_read_lock_cnt = 0;
	}
	else
	if ((tolock->surface_write_locked)&&(tolock->surface_write_lock_cnt == tolock->Lock_cnt)) {
		if (!this->use_own_alloc) {
#ifdef  __HAVE_DIRECTFB__
			this->llsurface->Unlock(this->llsurface);
#endif
		}
		tolock->surface_write_locked = false;
		tolock->surface_write_lock_cnt = 0;
	}

    tolock->Lock_cnt--;

    if (tolock->Lock_cnt == 0)
    	tolock->Lock.unlock();
}

void MMSFBSurface::unlock() {
	unlock(true);
}

MMSFBSurface *MMSFBSurface::getSubSurface(MMSFBRectangle *rect) {
    void   			*subsuf = NULL;
    MMSFBSurface 	*surface;

    /* check if initialized */
    INITCHECK;

#ifdef USE_DFB_SUBSURFACE
    /* get a sub surface */
    DFBResult dfbres;
    if ((dfbres=this->llsurface->GetSubSurface(this->llsurface, rect, (IDirectFBSurface*)&subsuf)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetSubSurface() failed");
        return false;
    }
#endif

    /* create a new surface instance */
    surface = new MMSFBSurface(subsuf, this, rect);
    if (!surface) {
#ifdef USE_DFB_SUBSURFACE
    	if (subsuf)
    		((IDirectFBSurface*)subsuf)->Release((IDirectFBSurface*)subsuf);
#endif
        MMSFB_SetError(0, "cannot create new instance of MMSFBSurface");
        return NULL;
    }

    /* add to my list */
    this->children.push_back(surface);

    return surface;
}

bool MMSFBSurface::setSubSurface(MMSFBRectangle *rect) {

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
    if ((dfbres=this->parent->llsurface->GetSubSurface(this->parent->llsurface, rect, &subsuf)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetSubSurface() failed");
        unlock();
        return false;
    }

    if (this->llsurface)
    	this->llsurface->Release(this->llsurface);

    this->llsurface = subsuf;

#endif

    this->sub_surface_rect = *rect;

#ifndef USE_DFB_SUBSURFACE

    getRealSubSurfacePos(NULL, true);

#endif

    unlock();

    return true;
}

bool MMSFBSurface::setSubSurface(MMSFBRegion *region) {
	MMSFBRectangle rect;

	if (!region)
		return false;

	rect.x = region->x1;
	rect.y = region->y1;
	rect.w = region->x2 - region->x1 + 1;
	rect.h = region->y2 - region->y1 + 1;

	return setSubSurface(&rect);
}

bool MMSFBSurface::moveTo(int x, int y) {
	MMSFBRectangle rect;

	rect = this->sub_surface_rect;
	rect.x = x;
	rect.y = y;

	return setSubSurface(&rect);
}

bool MMSFBSurface::move(int x, int y) {
	MMSFBRectangle rect;

	rect = this->sub_surface_rect;
	rect.x += x;
	rect.y += y;

	return setSubSurface(&rect);
}

