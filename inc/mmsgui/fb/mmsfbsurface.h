/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
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

#ifndef MMSFBSURFACE_H_
#define MMSFBSURFACE_H_

#include "mmstools/mmslogger.h"
#include "mmsgui/fb/mmsfbbase.h"
#include "mmsgui/fb/mmsfbfont.h"
#include "mmsgui/fb/mmsfbconv.h"

/* use DFB subsurfaces? */
//#define USE_DFB_SUBSURFACE

typedef enum {
	//! using directfb surfaces
	MMSFBSurfaceAllocMethod_dfb = 0,
	//! using malloc
	MMSFBSurfaceAllocMethod_malloc,
	//! using opengl surfaces
	MMSFBSurfaceAllocMethod_ogl
} MMSFBSurfaceAllocMethod;

typedef enum {
	//! allocated by directfb
	MMSFBSurfaceAllocatedBy_dfb = 0,
	//! allocated with malloc
	MMSFBSurfaceAllocatedBy_malloc,
	//! allocated by xv
	MMSFBSurfaceAllocatedBy_xvimage,
	//! allocated by x
	MMSFBSurfaceAllocatedBy_ximage,
	//! allocated by opengl
	MMSFBSurfaceAllocatedBy_ogl
} MMSFBSurfaceAllocatedBy;

//! dump mode
typedef enum {
	//! dump byte-by-byte hex values
	MMSFBSURFACE_DUMPMODE_BYTE = 0,
	//! dump pixels as 1, 2, 3 or 4 byte hex values
	MMSFBSURFACE_DUMPMODE_PIXEL
} MMSFBSurfaceDumpMode;

//! this is the maximum number of buffers for a surface (backbuffers + 1)
#define MMSFBSurfaceMaxBuffers		3

typedef struct {
	//! width
    int     sbw;
    //! height
    int     sbh;
    //! pixel format
    MMSFBSurfacePixelFormat pixelformat;
    //! the pixel format has alphachannel
    bool    alphachannel;
    //! premultiplied surface
    bool    premultiplied;
    //! number of backbuffers (e.g. 0 means FRONTONLY)
    int     backbuffer;
    //! true, if surface is stored in system memory
    bool	systemonly;
    //! real storage
    MMSFBSurfacePlanes buffers[MMSFBSurfaceMaxBuffers];
    //! real number of buffers allocated for a surface
    int 	numbuffers;
    //! index to the current read buffer (used if surface is the blit/stretchblit source)
    int 	currbuffer_read;
    //! index to the current write buffer (used as destination for all blitting/drawing routines)
    int 	currbuffer_write;
    //! surface buffer attached to this MMSFBSurface is externally allocated?
    bool	external_buffer;
#ifdef __HAVE_FBDEV__
    class MMSFBSurface	*mmsfbdev_surface;
#endif
#ifdef __HAVE_XLIB__
    XImage	*x_image[MMSFBSurfaceMaxBuffers];
#endif
#ifdef __HAVE_XV__
    XvImage	*xv_image[MMSFBSurfaceMaxBuffers];
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
    MMSFBRegion		clip;
    //! the surface is a window surface
    bool        	iswinsurface;
    //! the surface is the layer surface
    //! note: for example it is possible to have a window surface in combination with this layer flag
    bool        	islayersurface;
    //! drawing flags
    MMSFBDrawingFlags 	drawingflags;
    //! blitting flags
    MMSFBBlittingFlags 	blittingflags;
    //! font
    MMSFBFont			*font;
    //! the surface buffer(s)
    MMSFBSurfaceBuffer	*surface_buffer;
} MMSFBSurfaceConfig;



//! This class describes a surface.
/*!
\author Jens Schneider
*/
class MMSFBSurface {
    private:
#ifdef  __HAVE_DIRECTFB__
		//! dfb surface for drawing/blitting
		IDirectFBSurface	*dfb_surface;
#endif
#ifdef  __HAVE_OPENGL__
		//! opengl context
		GLXContext glx_context;

		//! opengl framebuffer object (FBO)
		GLuint	ogl_fbo;
#endif

		//! which system has allocated the memory?
		MMSFBSurfaceAllocatedBy	allocated_by;

		//! surface initialized?
		bool	initialized;


        bool				surface_read_locked;
        int					surface_read_lock_cnt;
        bool				surface_write_locked;
        int					surface_write_lock_cnt;
        bool				surface_invert_lock;

#ifdef __HAVE_XLIB__
        MMSFBSurface 		*scaler;
#endif

        //! surface configuration
        MMSFBSurfaceConfig  config;

        // if set to true, a few self-coded blend/stretch methods will be used instead of the according DFB functions
        static bool			extendedaccel;

        // how surface memory should be allocated?
        static MMSFBSurfaceAllocMethod	allocmethod;

        void freeSurfaceBuffer();

        void deleteSubSurface(MMSFBSurface *surface);

        int  calcPitch(int width);
        int  calcSize(int pitch, int height);
        void initPlanePointers(MMSFBSurfacePlanes *planes, int height);

        void getRealSubSurfacePos(MMSFBSurface *surface = NULL, bool refreshChilds = false);

        bool clipSubSurface(MMSFBRegion *region, bool regionset, MMSFBRegion *tmp, bool *tmpset);

        bool setWinSurface(bool iswinsurface = true);
        bool setLayerSurface(bool islayersurface = true);


        bool extendedLock(MMSFBSurface *src, MMSFBSurfacePlanes *src_planes,
        				  MMSFBSurface *dst, MMSFBSurfacePlanes *dst_planes);
        void extendedUnlock(MMSFBSurface *src, MMSFBSurface *dst, MMSFBSurfacePlanes *dst_planes = NULL);

        bool printMissingCombination(string method, MMSFBSurface *source = NULL, MMSFBSurfacePlanes *src_planes = NULL,
									 MMSFBSurfacePixelFormat src_pixelformat = MMSFB_PF_NONE, int src_width = 0, int src_height = 0);

        bool extendedAccelBlitEx(MMSFBSurface *source,
								 MMSFBSurfacePlanes *src_planes, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
        						 MMSFBRectangle *src_rect, int x, int y);
        bool extendedAccelBlit(MMSFBSurface *source, MMSFBRectangle *src_rect, int x, int y);
        bool extendedAccelBlitBuffer(MMSFBSurfacePlanes *src_planes, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
									 MMSFBRectangle *src_rect, int x, int y);

        bool extendedAccelStretchBlitEx(MMSFBSurface *source,
										MMSFBSurfacePlanes *src_planes, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
										MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect,
										MMSFBRectangle *real_dest_rect, bool calc_dest_rect);
        bool extendedAccelStretchBlit(MMSFBSurface *source, MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect,
									  MMSFBRectangle *real_dest_rect, bool calc_dest_rect);
        bool extendedAccelStretchBlitBuffer(MMSFBSurfacePlanes *src_planes, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
											MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect,
											MMSFBRectangle *real_dest_rect, bool calc_dest_rect);

        bool extendedAccelFillRectangleEx(int x, int y, int w, int h);
        bool extendedAccelFillRectangle(int x, int y, int w, int h);

        bool extendedAccelDrawLineEx(int x1, int y1, int x2, int y2);
        bool extendedAccelDrawLine(int x1, int y1, int x2, int y2);

        bool blit_text(string &text, int len, int x, int y);


        //! flags which are used when flipping
        MMSFBFlipFlags			flipflags;

        //! to make it thread-safe
        MMSMutex  				Lock;
        //! save the id of the thread which has locked the surface
        unsigned long       	TID;
        //! count the number of times the thread has call lock()
        int       				Lock_cnt;

        //! is it a sub surface?
        bool					is_sub_surface;
        //! parent surface in case of subsurface
        MMSFBSurface    		*parent;
        //! root parent surface in case of subsurface
        MMSFBSurface    		*root_parent;
        //! sub surface position and size relative to the parent
        MMSFBRectangle 			sub_surface_rect;
        //! x offset which is added to sub_surface_rect
        int						sub_surface_xoff;
        //! y offset which is added to sub_surface_rect
        int						sub_surface_yoff;
        //! list of sub surfaces connected to this surface
        vector<MMSFBSurface *>  children;



        void init(MMSFBSurfaceAllocatedBy allocated_by, MMSFBSurface *parent,
				  MMSFBRectangle *sub_surface_rect);


        void lock(MMSFBLockFlags flags, MMSFBSurfacePlanes *planes, bool pthread_lock);
        void unlock(bool pthread_unlock);

    public:
        MMSFBSurface(int w, int h, MMSFBSurfacePixelFormat pixelformat, int backbuffer=0, bool systemonly=true);
#ifdef  __HAVE_DIRECTFB__
        MMSFBSurface(IDirectFBSurface *dfb_surface, MMSFBSurface *parent = NULL,
					 MMSFBRectangle *sub_surface_rect = NULL);
#endif
        MMSFBSurface(MMSFBSurface *parent = NULL, MMSFBRectangle *sub_surface_rect = NULL);
        MMSFBSurface(int w, int h, MMSFBSurfacePixelFormat pixelformat, MMSFBSurfacePlanes *planes);
        MMSFBSurface(int w, int h, MMSFBSurfacePixelFormat pixelformat, int backbuffer, MMSFBSurfacePlanes *planes);
#ifdef __HAVE_XLIB__
        MMSFBSurface(int w, int h, MMSFBSurfacePixelFormat pixelformat, XImage *x_image1, XImage *x_image2, MMSFBSurface *scaler);
#endif
#ifdef __HAVE_XV__
        MMSFBSurface(int w, int h, MMSFBSurfacePixelFormat pixelformat, XvImage *xv_image1, XvImage *xv_image2);
#endif
#ifdef __HAVE_OPENGL__
        MMSFBSurface(int w, int h, MMSFBSurfacePixelFormat pixelformat, GLXContext glx_context);
#endif

        virtual ~MMSFBSurface();

        bool isInitialized();

        void *getDFBSurface();

        bool getConfiguration(MMSFBSurfaceConfig *config = NULL);

        void setExtendedAcceleration(bool extendedaccel);
        bool getExtendedAcceleration();

        void setAllocMethod(MMSFBSurfaceAllocMethod allocmethod);
        MMSFBSurfaceAllocMethod getAllocMethod();

        bool isWinSurface();
        bool isLayerSurface();
        bool isSubSurface();
        MMSFBSurface *getParent();
        MMSFBSurface *getRootParent();

        bool getPixelFormat(MMSFBSurfacePixelFormat *pf);
        bool getSize(int *w, int *h);
        bool getNumberOfBuffers(int *num);
        bool getMemSize(int *size);

        bool setFlipFlags(MMSFBFlipFlags flags);

        bool clear(unsigned char r = 0, unsigned char g = 0,
                   unsigned char b = 0, unsigned char a = 0);

        bool setColor(unsigned char r, unsigned char g,
                      unsigned char b, unsigned char a);
        bool setColor(MMSFBColor &color);
        bool getColor(MMSFBColor *color);

        bool setClip(MMSFBRegion *clip);
        bool setClip(int x1, int y1, int x2, int y2);
        bool getClip(MMSFBRegion *clip);

        bool setDrawingFlags(MMSFBDrawingFlags flags);
        bool drawLine(int x1, int y1, int x2, int y2);
        bool drawRectangle(int x, int y, int w, int h);
        bool fillRectangle(int x = 0, int y = 0, int w = 0, int h = 0);
        bool drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
        bool fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
        bool drawCircle(int x, int y, int radius, int start_octant = 0, int end_octant = 7);

        bool setBlittingFlags(MMSFBBlittingFlags flags);
        bool getBlittingFlags(MMSFBBlittingFlags *flags);
        bool blit(MMSFBSurface *source, MMSFBRectangle *src_rect = NULL, int x = 0, int y = 0);
        bool blitBuffer(MMSFBExternalSurfaceBuffer *extbuf, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
						MMSFBRectangle *src_rect = NULL, int x = 0, int y = 0);
        bool blitBuffer(void *src_ptr, int src_pitch, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
						MMSFBRectangle *src_rect = NULL, int x = 0, int y = 0);
        bool stretchBlit(MMSFBSurface *source, MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect,
						 MMSFBRectangle *real_dest_rect = NULL, bool calc_dest_rect = false);
        bool stretchBlitBuffer(MMSFBExternalSurfaceBuffer *extbuf, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
							   MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect,
							   MMSFBRectangle *real_dest_rect = NULL, bool calc_dest_rect = false);
        bool stretchBlitBuffer(void *src_ptr, int src_pitch, MMSFBSurfacePixelFormat src_pixelformat, int src_width, int src_height,
							   MMSFBRectangle *src_rect, MMSFBRectangle *dest_rect,
							   MMSFBRectangle *real_dest_rect = NULL, bool calc_dest_rect = false);


        bool flip(MMSFBRegion *region = NULL);
        bool refresh();

        bool createCopy(MMSFBSurface **dstsurface, int w = 0, int h = 0,
                        bool copycontent = false, bool withbackbuffer = false,
                        MMSFBSurfacePixelFormat pixelformat = MMSFB_PF_NONE);
        bool resize(int w = 0, int h = 0);

        void modulateBrightness(MMSFBColor *color, unsigned char brightness);
        void modulateOpacity(MMSFBColor *color, unsigned char opacity);

        bool setBlittingFlagsByBrightnessAlphaAndOpacity(
                    unsigned char brightness, unsigned char alpha, unsigned char opacity);
        bool setDrawingFlagsByAlpha(unsigned char alpha);
        bool setDrawingColorAndFlagsByBrightnessAndOpacity(
                    MMSFBColor color, unsigned char brightness, unsigned char opacity);

        bool setFont(MMSFBFont *font);
        bool drawString(string text, int len, int x, int y);

        void lock(MMSFBLockFlags flags = MMSFB_LOCK_NONE, void **ptr = NULL, int *pitch = NULL);
        void lock(MMSFBLockFlags flags, MMSFBSurfacePlanes *planes);
        void unlock();

        MMSFBSurface *getSubSurface(MMSFBRectangle *rect);
        bool setSubSurface(MMSFBRectangle *rect);
        bool setSubSurface(MMSFBRegion *region);
        bool moveTo(int x, int y);
        bool move(int x, int y);

        bool dump2fcb(bool (*fcb)(char *, int, void *, int *), void *argp, int *argi,
					  int x, int y, int w, int h, MMSFBSurfaceDumpMode dumpmode);
        int  dump2buffer(char *buffer, int buffer_len, int x = 0, int y = 0, int w = 0, int h = 0,
						 MMSFBSurfaceDumpMode dumpmode = MMSFBSURFACE_DUMPMODE_BYTE);
        bool dump2file(string filename, int x = 0, int y = 0, int w = 0, int h = 0,
					   MMSFBSurfaceDumpMode dumpmode = MMSFBSURFACE_DUMPMODE_BYTE);
        bool dump2file(string filename, MMSFBSurfaceDumpMode dumpmode);
        bool dump(int x = 0, int y = 0, int w = 0, int h = 0,
				  MMSFBSurfaceDumpMode dumpmode = MMSFBSURFACE_DUMPMODE_BYTE);
        bool dump(MMSFBSurfaceDumpMode dumpmode);

    friend class MMSFBLayer;
    friend class MMSFBSurfaceManager;
    friend class MMSFBWindowManager;
};

bool mmsfb_create_cached_surface(MMSFBSurface **cs, int width, int height,
								 MMSFBSurfacePixelFormat pixelformat);

#endif /*MMSFBSURFACE_H_*/
