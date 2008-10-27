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

#ifndef MMSFBSURFACE_H_
#define MMSFBSURFACE_H_

#include "mmstools/mmslogger.h"
#include "mmstools/mmsmutex.h"
#include "mmsgui/fb/mmsfbbase.h"

/* use DFB subsurfaces? */
//#define USE_DFB_SUBSURFACE

#define MMSFBSurfaceBlittingFlags   DFBSurfaceBlittingFlags
#define MMSFBSurfaceDrawingFlags    DFBSurfaceDrawingFlags
#define MMSFBSurfaceFlipFlags       DFBSurfaceFlipFlags
#define MMSFBSurfaceLockFlags		int

typedef enum {
	//! using directfb surfaces
	MMSFBSurfaceAllocMethod_dfb = 0,
	//! using malloc
	MMSFBSurfaceAllocMethod_malloc
} MMSFBSurfaceAllocMethod;

#define MMSFBSurfaceMaxBuffers		3

typedef struct {
    void	*buffers[MMSFBSurfaceMaxBuffers];
    int 	numbuffers;
    int 	currbuffer_read;
    int 	currbuffer_write;
    int 	pitch;
} MMSFBSurfaceBuffer;

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

    //! the surface buffer(s)
    MMSFBSurfaceBuffer	surface_buffer;
} MMSFBSurfaceConfig;


class MMSFBSurface {
    private:
        IDirectFBSurface    *dfbsurface;/* dfbsurface for drawing/blitting */
        bool				dfbsurface_locked;
        int					dfbsurface_lock_cnt;

        MMSFBSurfaceConfig  config;     /* surface configuration */

        // using own allocated surfaces?
        bool				use_own_alloc;

        // if set to true, a few self-coded blend/stretch methods will be used instead of the according DFB functions
        static bool			extendedaccel;

        // how surface memory will be allocated?
        static MMSFBSurfaceAllocMethod	allocmethod;

        // first time flag for eAB_argb_to_argb()
        static bool 			firsttime_eAB_argb_to_argb;

        // first time flag for eAB_blend_argb_to_argb()
        static bool 			firsttime_eAB_blend_argb_to_argb;

        // first time flag for eAB_blend_srcalpha_argb_to_argb()
        static bool 			firsttime_eAB_blend_srcalpha_argb_to_argb;

        // first time flag for eAB_blend_argb_to_airgb()
        static bool 			firsttime_eAB_blend_argb_to_airgb;

        // first time flag for eAB_rgb16_to_rgb16()
        static bool 			firsttime_eAB_rgb16_to_rgb16;

        // first time flag for eAB_argb_to_rgb16()
        static bool 			firsttime_eAB_argb_to_rgb16;

        // first time flag for eAB_blend_argb_to_rgb16()
        static bool 			firsttime_eAB_blend_argb_to_rgb16;

        // first time flag for eAB_blend_airgb_to_airgb()
        static bool 			firsttime_eAB_blend_airgb_to_airgb;

        // first time flag for eAB_blend_srcalpha_airgb_to_airgb()
        static bool 			firsttime_eAB_blend_srcalpha_airgb_to_airgb;

        // first time flag for eAB_airgb_to_rgb16()
        static bool 			firsttime_eAB_airgb_to_rgb16;

        // first time flag for eAB_blend_airgb_to_rgb16()
        static bool 			firsttime_eAB_blend_airgb_to_rgb16;

        // first time flag for eAB_blend_ayuv_to_ayuv()
        static bool 			firsttime_eAB_blend_ayuv_to_ayuv;

        // first time flag for eAB_blend_srcalpha_ayuv_to_ayuv()
        static bool 			firsttime_eAB_blend_srcalpha_ayuv_to_ayuv;

        // first time flag for eAB_ayuv_to_rgb16()
        static bool 			firsttime_eAB_ayuv_to_rgb16;

        // first time flag for eAB_blend_ayuv_to_rgb16()
        static bool 			firsttime_eAB_blend_ayuv_to_rgb16;


        // first time flag for eAB_blend_argb_to_yv12()
        static bool 			firsttime_eAB_blend_argb_to_yv12;

        // first time flag for eAB_blend_srcalpha_argb_to_yv12()
        static bool 			firsttime_eAB_blend_srcalpha_argb_to_yv12;

        // first time flag for eAB_blend_ayuv_to_yv12()
        static bool 			firsttime_eAB_blend_ayuv_to_yv12;

        // first time flag for eAB_blend_srcalpha_ayuv_to_yv12()
        static bool 			firsttime_eAB_blend_srcalpha_ayuv_to_yv12;


        // first time flag for eASB_blend_argb_to_argb()
        static bool 			firsttime_eASB_blend_argb_to_argb;

        // first time flag for eASB_blend_srcalpha_argb_to_argb()
        static bool 			firsttime_eASB_blend_srcalpha_argb_to_argb;

        // first time flag for eASB_blend_airgb_to_airgb()
        static bool 			firsttime_eASB_blend_airgb_to_airgb;

        // first time flag for eASB_blend_srcalpha_airgb_to_airgb()
        static bool 			firsttime_eASB_blend_srcalpha_airgb_to_airgb;


        // first time flag for eASB_blend_ayuv_to_ayuv()
        static bool 			firsttime_eASB_blend_ayuv_to_ayuv;

        // first time flag for eASB_blend_srcalpha_ayuv_to_ayuv()
        static bool 			firsttime_eASB_blend_srcalpha_ayuv_to_ayuv;


        // first time flag for eAFR_argb()
        static bool				firsttime_eAFR_argb;

        // first time flag for eAFR_blend_argb()
        static bool				firsttime_eAFR_blend_argb;

        // first time flag for eAFR_blend_ayuv()
        static bool				firsttime_eAFR_blend_ayuv;

        void freeSurfaceBuffer();

        void deleteSubSurface(MMSFBSurface *surface);

        void getRealSubSurfacePos(MMSFBSurface *surface = NULL, bool refreshChilds = false);

        bool clipSubSurface(DFBRegion *region, bool regionset, DFBRegion *tmp, bool *tmpset);

        bool setWinSurface(bool iswinsurface = true);
        bool setLayerSurface(bool islayersurface = true);


        void eAB_argb_to_argb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        					  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_argb_to_argb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        							unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_srcalpha_argb_to_argb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        									 unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy,
        									 unsigned char alpha);

        void eAB_blend_argb_to_airgb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        							 unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);

        void eAB_rgb16_to_rgb16(unsigned short int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        					    unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_argb_to_rgb16(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        					   unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_argb_to_rgb16(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        						     unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);


        void eAB_blend_airgb_to_airgb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        							  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_srcalpha_airgb_to_airgb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        									   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy,
        									   unsigned char alpha);
        void eAB_airgb_to_rgb16(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        					    unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_airgb_to_rgb16(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        						      unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);




        void eAB_blend_ayuv_to_ayuv(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        							unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_srcalpha_ayuv_to_ayuv(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        									 unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy,
        									 unsigned char alpha);
        void eAB_ayuv_to_rgb16(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        					   unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_ayuv_to_rgb16(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        						     unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);


        void eAB_blend_argb_to_yv12(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        				 			unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_ayuv_to_yv12(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        				 			unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_srcalpha_argb_to_yv12(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        				 					 unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy,
        				 					 unsigned char alpha);
        void eAB_blend_srcalpha_ayuv_to_yv12(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        				 					 unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy,
        				 					 unsigned char alpha);


        bool extendedLock(MMSFBSurface *src, void **src_ptr, int *src_pitch,
        				  MMSFBSurface *dst, void **dst_ptr, int *dst_pitch);
        void extendedUnlock(MMSFBSurface *src, MMSFBSurface *dst);

        bool extendedAccelBlit(MMSFBSurface *source, DFBRectangle *src_rect, int x, int y);

        void eASB_blend_argb_to_argb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
									 unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh);
        void eASB_blend_srcalpha_argb_to_argb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        									  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
        									  unsigned char alpha);

        void eASB_blend_airgb_to_airgb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
									   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh);
        void eASB_blend_srcalpha_airgb_to_airgb(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        									    unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
        									    unsigned char alpha);

        void eASB_blend_ayuv_to_ayuv(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
									 unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh);
        void eASB_blend_srcalpha_ayuv_to_ayuv(unsigned int *src, int src_pitch, int src_height, int sx, int sy, int sw, int sh,
        									  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
        									  unsigned char alpha);

        bool extendedAccelStretchBlit(MMSFBSurface *source, DFBRectangle *src_rect, DFBRectangle *dest_rect);



        void eAFR_argb(unsigned int *dst, int dst_pitch, int dst_height,
					   int dx, int dy, int dw, int dh, MMSFBColor color);

        void eAFR_blend_argb(unsigned int *dst, int dst_pitch, int dst_height,
        						int dx, int dy, int dw, int dh, MMSFBColor color);

        void eAFR_blend_ayuv(unsigned int *dst, int dst_pitch, int dst_height,
        						int dx, int dy, int dw, int dh, MMSFBColor color);

        bool extendedAccelFillRectangle(int x, int y, int w, int h);


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


        void init(IDirectFBSurface *dfbsurface,
				  MMSFBSurface *parent,
				  DFBRectangle *sub_surface_rect);

    public:
        MMSFBSurface(int w, int h, string pixelformat, int backbuffer, bool systemonly);
        MMSFBSurface(IDirectFBSurface *dfbsurface,
					 MMSFBSurface *parent = NULL,
					 DFBRectangle *sub_surface_rect = NULL);
        virtual ~MMSFBSurface();

        bool isInitialized();

        IDirectFBSurface *getDFBSurface();

        bool getConfiguration(MMSFBSurfaceConfig *config = NULL);

        void setExtendedAcceleration(bool extendedaccel);
        bool getExtendedAcceleration();

        void setAllocMethod(MMSFBSurfaceAllocMethod allocmethod, bool reset = false);
        MMSFBSurfaceAllocMethod getAllocMethod();

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

        void lock(MMSFBSurfaceLockFlags flags = 0, void **ptr = NULL, int *pitch = NULL);
        void unlock();

        MMSFBSurface *getSubSurface(DFBRectangle *rect);
        bool setSubSurface(DFBRectangle *rect);
        bool setSubSurface(DFBRegion *region);
        bool moveTo(int x, int y);
        bool move(int x, int y);

    friend class MMSFBLayer;
    friend class MMSFBSurfaceManager;
    friend class MMSFBWindowManager;
};

#endif /*MMSFBSURFACE_H_*/
