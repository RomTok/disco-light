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
#include "mmsgui/fb/mmsfbbase.h"
#include "mmsgui/fb/mmsfbfont.h"

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
	//! width
    int     sbw;
    //! height
    int     sbh;
    //! pixel format
    string  pixelformat;
    //! the pixel format has alphachannel
    bool    alphachannel;
    //! premultiplied surface
    bool    premultiplied;
    //! premultiplied surface
    int     backbuffer;
    //! true, if surface is stored in system memory
    bool	systemonly;
    //! real storage
    void	*buffers[MMSFBSurfaceMaxBuffers];
    int 	numbuffers;
    int 	currbuffer_read;
    int 	currbuffer_write;
    int 	pitch;
#ifdef __HAVE_XLIB__
    XvImage *xv_image[MMSFBSurfaceMaxBuffers];
#endif
} MMSFBSurfaceBuffer;

typedef struct {
	//! width
    int     w;
    //! height
    int     h;
    //! color for drawing/blitting
    MMSFBColor  	color;
    //! is a clip region set?
    bool			clipped;
    //! current clip region
    DFBRegion		clip;
    //! the surface is a window surface
    bool        	iswinsurface;
    //! the surface is the layer surface
    //!note: for example it is possible to have a window surface in combination with this layer flag
    bool        	islayersurface;
    //! drawing flags
    MMSFBSurfaceDrawingFlags 	drawingflags;
    //! blitting flags
    MMSFBSurfaceBlittingFlags 	blittingflags;
    //! font
    MMSFBFont					*font;
    //! the surface buffer(s)
    MMSFBSurfaceBuffer	*surface_buffer;
} MMSFBSurfaceConfig;


typedef struct {
	void	*ptr;
	int 	pitch;
	void	*ptr2;
	int 	pitch2;
	void	*ptr3;
	int 	pitch3;
} MMSFBExternalSurfaceBuffer;

//! This class describes a surface.
/*!
\author Jens Schneider
*/
class MMSFBSurface {
    private:
        IDirectFBSurface    *dfbsurface;/* dfbsurface for drawing/blitting */
        bool				surface_read_locked;
        int					surface_read_lock_cnt;
        bool				surface_write_locked;
        int					surface_write_lock_cnt;
        bool				surface_invert_lock;

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

        // first time flag for eAB_airgb_to_airgb()
        static bool 			firsttime_eAB_airgb_to_airgb;

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


        // first time flag for eAB_yv12_to_yv12()
        static bool 			firsttime_eAB_yv12_to_yv12;

        // first time flag for eAB_blend_argb_to_yv12()
        static bool 			firsttime_eAB_blend_argb_to_yv12;

        // first time flag for eAB_blend_srcalpha_argb_to_yv12()
        static bool 			firsttime_eAB_blend_srcalpha_argb_to_yv12;

        // first time flag for eAB_blend_ayuv_to_yv12()
        static bool 			firsttime_eAB_blend_ayuv_to_yv12;

        // first time flag for eAB_blend_srcalpha_ayuv_to_yv12()
        static bool 			firsttime_eAB_blend_srcalpha_ayuv_to_yv12;


        // first time flag for eADB_blend_argb_to_yv12()
        static bool 			firsttime_eADB_blend_argb_to_yv12;



        // first time flag for eASB_blend_argb_to_argb()
        static bool 			firsttime_eASB_blend_argb_to_argb;

        // first time flag for eASB_blend_srcalpha_argb_to_argb()
        static bool 			firsttime_eASB_blend_srcalpha_argb_to_argb;

        // first time flag for eASB_blend_airgb_to_airgb()
        static bool 			firsttime_eASB_blend_airgb_to_airgb;

        // first time flag for eASB_blend_srcalpha_airgb_to_airgb()
        static bool 			firsttime_eASB_blend_srcalpha_airgb_to_airgb;


        // first time flag for eAB_ayuv_to_ayuv()
        static bool 			firsttime_eAB_ayuv_to_ayuv;

        // first time flag for eASB_blend_ayuv_to_ayuv()
        static bool 			firsttime_eASB_blend_ayuv_to_ayuv;

        // first time flag for eASB_blend_srcalpha_ayuv_to_ayuv()
        static bool 			firsttime_eASB_blend_srcalpha_ayuv_to_ayuv;

        // first time flag for eASB_yv12_to_yv12()
        static bool 			firsttime_eASB_yv12_to_yv12;

        // first time flag for eAFR_argb()
        static bool				firsttime_eAFR_argb;

        // first time flag for eAFR_blend_argb()
        static bool				firsttime_eAFR_blend_argb;

        // first time flag for eAFR_rgb16()
        static bool				firsttime_eAFR_rgb16;

        // first time flag for eAFR_ayuv()
        static bool				firsttime_eAFR_ayuv;

        // first time flag for eAFR_blend_ayuv()
        static bool				firsttime_eAFR_blend_ayuv;

        // first time flag for eAFR_yv12()
        static bool				firsttime_eAFR_yv12;

        // first time flag for eADL_argb()
        static bool				firsttime_eADL_argb;

        // first time flag for eADL_blend_argb()
        static bool				firsttime_eADL_blend_argb;

#ifdef __HAVE_XLIB__
        // first time flag for blend_text_to_argb()
        static bool				firsttime_blend_text_to_argb;

        // first time flag for blend_text_srcalpha_to_argb()
        static bool				firsttime_blend_text_srcalpha_to_argb;
#endif

        void freeSurfaceBuffer();

        void deleteSubSurface(MMSFBSurface *surface);

        int calcPitch(int width);
        int calcSize(int pitch, int height);

        void getRealSubSurfacePos(MMSFBSurface *surface = NULL, bool refreshChilds = false);

        bool clipSubSurface(DFBRegion *region, bool regionset, DFBRegion *tmp, bool *tmpset);

        bool setWinSurface(bool iswinsurface = true);
        bool setLayerSurface(bool islayersurface = true);


        //////////
        bool extendedLock(MMSFBSurface *src, void **src_ptr, int *src_pitch,
        				  MMSFBSurface *dst, void **dst_ptr, int *dst_pitch,
        				  MMSFBSurface *dstsrc = NULL, void **dstsrc_ptr = NULL, int *dstsrc_pitch = NULL);
        void extendedUnlock(MMSFBSurface *src, MMSFBSurface *dst, MMSFBSurface *dstsrc = NULL);
        bool printMissingCombination(char *method, MMSFBSurface *source = NULL, MMSFBExternalSurfaceBuffer *extbuf = NULL,
									 string src_pixelformat = "", int src_width = 0, int src_height = 0);
        //////////


        //////////
        void eAB_argb_to_argb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        					  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_argb_to_argb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        							unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_srcalpha_argb_to_argb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        									 unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy,
        									 unsigned char alpha);

        void eAB_blend_argb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        							 unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);

        void eAB_rgb16_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        					    unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_argb_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        					   unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_argb_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        						     unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);


        void eAB_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        					    unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        							  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_srcalpha_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        									   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy,
        									   unsigned char alpha);
        void eAB_airgb_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        					    unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_airgb_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        						      unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);




        void eAB_ayuv_to_ayuv(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        					  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_ayuv_to_ayuv(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        							unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_srcalpha_ayuv_to_ayuv(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        									 unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy,
        									 unsigned char alpha);
        void eAB_ayuv_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        					   unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_ayuv_to_rgb16(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        						     unsigned short int *dst, int dst_pitch, int dst_height, int dx, int dy);


        void eAB_yv12_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        					  unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_argb_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        				 			unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_ayuv_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        				 			unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy);
        void eAB_blend_srcalpha_argb_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        				 					 unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy,
        				 					 unsigned char alpha);
        void eAB_blend_srcalpha_ayuv_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        				 					 unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy,
        				 					 unsigned char alpha);

        bool extendedAccelBlitEx(MMSFBSurface *source,
								 MMSFBExternalSurfaceBuffer *extbuf, string src_pixelformat, int src_width, int src_height,
        						 DFBRectangle *src_rect, int x, int y);
        bool extendedAccelBlit(MMSFBSurface *source, DFBRectangle *src_rect, int x, int y);
        bool extendedAccelBlitBuffer(MMSFBExternalSurfaceBuffer *extbuf, string src_pixelformat, int src_width, int src_height,
									 DFBRectangle *src_rect, int x, int y);
        ////////



        ////////
        void eADB_blend_argb_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        				 			 unsigned char *dst, int dst_pitch, int dst_height, unsigned char *dstsrc, int dx, int dy);

        bool extendedAccelDoubleBlitEx(MMSFBSurface *dstsrc, MMSFBSurface *source,
        							   MMSFBExternalSurfaceBuffer *extbuf, string src_pixelformat, int src_width, int src_height,
        							   DFBRectangle *src_rect, int x, int y);
        bool extendedAccelDoubleBlit(MMSFBSurface *dstsrc, MMSFBSurface *source, DFBRectangle *src_rect, int x, int y);
        //////////


        //////////
        void eASB_blend_argb_to_argb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									 unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh);
        void eASB_blend_srcalpha_argb_to_argb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        									  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
        									  unsigned char alpha);

        void eASB_blend_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									   unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh);
        void eASB_blend_srcalpha_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        									    unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
        									    unsigned char alpha);

        void eASB_blend_ayuv_to_ayuv(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									 unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh);
        void eASB_blend_srcalpha_ayuv_to_ayuv(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
        									  unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
        									  unsigned char alpha);

        void eASB_yv12_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
							   unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh);

        bool extendedAccelStretchBlitEx(MMSFBSurface *source,
										MMSFBExternalSurfaceBuffer *extbuf, string src_pixelformat, int src_width, int src_height,
										DFBRectangle *src_rect, DFBRectangle *dest_rect);
        bool extendedAccelStretchBlit(MMSFBSurface *source, DFBRectangle *src_rect, DFBRectangle *dest_rect);
        bool extendedAccelStretchBlitBuffer(MMSFBExternalSurfaceBuffer *extbuf, string src_pixelformat, int src_width, int src_height,
											DFBRectangle *src_rect, DFBRectangle *dest_rect);
        //////////



        //////////
        void eAFR_argb(unsigned int *dst, int dst_pitch, int dst_height,
					   int dx, int dy, int dw, int dh, MMSFBColor color);

        void eAFR_blend_argb(unsigned int *dst, int dst_pitch, int dst_height,
        						int dx, int dy, int dw, int dh, MMSFBColor color);

        void eAFR_rgb16(unsigned short int *dst, int dst_pitch, int dst_height,
					    int dx, int dy, int dw, int dh, MMSFBColor color);

        void eAFR_ayuv(unsigned int *dst, int dst_pitch, int dst_height,
					   int dx, int dy, int dw, int dh, MMSFBColor color);

        void eAFR_blend_ayuv(unsigned int *dst, int dst_pitch, int dst_height,
        						int dx, int dy, int dw, int dh, MMSFBColor color);

        void eAFR_yv12(unsigned char *dst, int dst_pitch, int dst_height,
					   int dx, int dy, int dw, int dh, MMSFBColor color);

        bool extendedAccelFillRectangleEx(int x, int y, int w, int h);
        bool extendedAccelFillRectangle(int x, int y, int w, int h);
        //////////


        //////////
        void eADL_argb(unsigned int *dst, int dst_pitch, int dst_height,
					   DFBRegion &clipreg, int x1, int y1, int x2, int y2, MMSFBColor &color);
        void eADL_blend_argb(unsigned int *dst, int dst_pitch, int dst_height,
							 DFBRegion &clipreg, int x1, int y1, int x2, int y2, MMSFBColor &color);

        bool extendedAccelDrawLineEx(int x1, int y1, int x2, int y2);
        bool extendedAccelDrawLine(int x1, int y1, int x2, int y2);
        //////////

#ifdef __HAVE_XLIB__
        void blend_text_to_argb(DFBRegion &clipreg, string &text, int len, int x, int y, MMSFBColor &color);
        void blend_text_srcalpha_to_argb(DFBRegion &clipreg, string &text, int len, int x, int y, MMSFBColor &color);

        bool blit_text(string &text, int len, int x, int y);
#endif


        MMSFBSurfaceFlipFlags	flipflags;		/* flags which are used when flipping */

        MMSMutex  				Lock;       		/* to make it thread-safe */
        unsigned long       	TID;        		/* save the id of the thread which has locked the surface */
        int       				Lock_cnt;   		/* count the number of times the thread has call lock() */

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

        void lock(MMSFBSurfaceLockFlags flags, void **ptr, int *pitch, bool pthread_lock);
        void unlock(bool pthread_unlock);

    public:
        MMSFBSurface(int w, int h, string pixelformat, int backbuffer, bool systemonly);
        MMSFBSurface(IDirectFBSurface *dfbsurface,
					 MMSFBSurface *parent = NULL,
					 DFBRectangle *sub_surface_rect = NULL);
#ifdef __HAVE_XLIB__
        MMSFBSurface(int w, int h, string pixelformat, XvImage *xv_image1, XvImage *xv_image2);
#endif

        virtual ~MMSFBSurface();

        bool isInitialized();

        void *getDFBSurface();

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
        bool setClip(int x1, int y1, int x2, int y2);
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
        bool blitBuffer(MMSFBExternalSurfaceBuffer *extbuf, string src_pixelformat, int src_width, int src_height,
						DFBRectangle *src_rect, int x, int y);
        bool blitBuffer(void *src_ptr, int src_pitch, string src_pixelformat, int src_width, int src_height,
						DFBRectangle *src_rect, int x, int y);
        bool stretchBlit(MMSFBSurface *source, DFBRectangle *src_rect, DFBRectangle *dest_rect);
        bool stretchBlitBuffer(MMSFBExternalSurfaceBuffer *extbuf, string src_pixelformat, int src_width, int src_height,
							   DFBRectangle *src_rect, DFBRectangle *dest_rect);
        bool stretchBlitBuffer(void *src_ptr, int src_pitch, string src_pixelformat, int src_width, int src_height,
							   DFBRectangle *src_rect, DFBRectangle *dest_rect);

        bool doubleBlit(MMSFBSurface *dstsrc, MMSFBSurface *source, DFBRectangle *src_rect, int x, int y);

        bool flip(DFBRegion *region = NULL);
        bool refresh();

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

        bool setFont(MMSFBFont *font);
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
