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

#ifndef MMSFBSURFACE_H_
#define MMSFBSURFACE_H_

#include "mmstools/mmstools.h"
#include "mmstools/mmslogger.h"
#include "mmsgui/fb/mmsfbbase.h"

/* use DFB subsurfaces? */
//#define USE_DFB_SUBSURFACE

#define MMSFBSurfaceBlittingFlags   DFBSurfaceBlittingFlags
#define MMSFBSurfaceDrawingFlags    DFBSurfaceDrawingFlags
#define MMSFBSurfaceFlipFlags       DFBSurfaceFlipFlags

typedef struct {
    int         	w;              /* width */
    int         	h;              /* height */
    string      	pixelformat;    /* pixel format */
    bool        	alphachannel;   /* the pixel format has alphachannel */
    bool            premultiplied;	/* premultiplied surface */
    MMSFBColor  	color;          /* color for drawing/blitting */
    bool			clipped;		/* is a clip region set? */
    DFBRegion		clip;			/* current clip region */
    int         	backbuffer;     /* 0-none, 1-double, 2-triple buffered */
    bool			systemonly;		/* true, if surface is stored in system memory */
    bool        	iswinsurface;   /* the surface is a window surface */
    bool        	islayersurface; /* the surface is the layer surface */
                                	/* note: for example it is possible to have */
                                	/*       a window surface in combination with */
                                	/*       this layer flag */
    MMSFBSurfaceDrawingFlags 	drawingflags;	/* drawing flags */
    MMSFBSurfaceBlittingFlags 	blittingflags;	/* blitting flags */
    IDirectFBFont				*font;			/* font */
} MMSFBSurfaceConfig;

class MMSFBSurface {
    private:
        IDirectFBSurface    *dfbsurface;/* dfbsurface for drawing/blitting */

        MMSFBSurfaceConfig  config;     /* surface configuration */

        void deleteSubSurface(MMSFBSurface *surface);

        void getRealSubSurfacePos(MMSFBSurface *surface = NULL, bool refreshChilds = false);

        bool clipSubSurface(DFBRegion *region, bool regionset, DFBRegion *tmp, bool *tmpset);
        
        bool setWinSurface(bool iswinsurface = true);
        bool setLayerSurface(bool islayersurface = true);

        MMSFBSurfaceFlipFlags	flipflags;		/* flags which are used when flipping */
        
        MMSMutex  				Lock;       		/* to make it thread-safe */
        unsigned long       	TID;        		/* save the id of the thread which has locked the surface */
        unsigned long       	Lock_cnt;   		/* count the number of times the thread has call lock() */

        bool					is_sub_surface;		/* is it a sub surface? */
        MMSFBSurface    		*parent;			/* parent surface in case of subsurface */
        MMSFBSurface    		*root_parent;		/* root parent surface in case of subsurface */
        DFBRectangle 			sub_surface_rect;   /* sub surface position and size relative to the parent */
        int						sub_surface_xoff;	/* x offset which is added to sub_surface_rect */
        int						sub_surface_yoff;	/* y offset which is added to sub_surface_rect */
        vector<MMSFBSurface *>  children;			/* list of sub surfaces connected to this surface */

    public:
        MMSFBSurface(IDirectFBSurface *dfbsurface,
        			 MMSFBSurface *parent = NULL,
        		     DFBRectangle *sub_surface_rect = NULL);
        virtual ~MMSFBSurface(); 

        IDirectFBSurface *getDFBSurface();

        bool getConfiguration(MMSFBSurfaceConfig *config = NULL);

        bool isWinSurface();
        bool isLayerSurface();

        bool getPixelFormat(string *pf);
        bool getSize(int *w, int *h);
        bool getMemSize(int *size);

        bool setFlipFlags(MMSFBSurfaceFlipFlags flags);        
        
        bool clear(unsigned char r = 0, unsigned char g = 0,
                   unsigned char b = 0, unsigned char a = 0);

        bool setColor(unsigned char r, unsigned char g,
                      unsigned char b, unsigned char a);
        bool getColor(MMSFBColor *color);

        bool setClip(DFBRegion *clip);
        bool getClip(DFBRegion *clip);

        bool setDrawingFlags(MMSFBSurfaceDrawingFlags flags);
        bool drawLine(int x1, int y1, int x2, int y2);
        bool drawRectangle(int x, int y, int w, int h);
        bool fillRectangle(int x, int y, int w, int h);
        bool drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
        bool fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
        bool drawCircle(int x, int y, int radius, int start_octant = 0, int end_octant = 7);

        bool setBlittingFlags(MMSFBSurfaceBlittingFlags flags);
        bool blit(MMSFBSurface *source, DFBRectangle *src_rect, int x, int y);
        bool stretchBlit(MMSFBSurface *source, DFBRectangle *src_rect, DFBRectangle *dest_rect);

        bool flip(DFBRegion *region = NULL);

        bool createCopy(MMSFBSurface **dstsurface, int w = 0, int h = 0,
                        bool copycontent = false, bool withbackbuffer = false);
        bool resize(int w = 0, int h = 0);

        void modulateBrightness(DFBColor *color, unsigned char brightness);
        void modulateOpacity(DFBColor *color, unsigned char opacity);

        bool setBlittingFlagsByBrightnessAlphaAndOpacity(
                    unsigned char brightness, unsigned char alpha, unsigned char opacity);
        bool setDrawingFlagsByAlpha(unsigned char alpha);
        bool setDrawingColorAndFlagsByBrightnessAndOpacity(
                    DFBColor color, unsigned char brightness, unsigned char opacity);

        bool setFont(IDirectFBFont *font);
        bool drawString(string text, int len, int x, int y);

        void lock();
        void unlock();

        MMSFBSurface *getSubSurface(DFBRectangle *rect);
        bool setSubSurface(DFBRectangle *rect);
        bool setSubSurface(DFBRegion *region);
        bool moveTo(int x, int y);
        bool move(int x, int y);

    friend class MMSFBLayer;
    friend class MMSFBSurfaceManager;
};

#endif /*MMSFBSURFACE_H_*/
