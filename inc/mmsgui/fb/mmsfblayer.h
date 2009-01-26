/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#ifndef MMSFBLAYER_H_
#define MMSFBLAYER_H_

#include "mmstools/mmslogger.h"
#include "mmsgui/fb/mmsfbbase.h"
#include "mmsgui/fb/mmsfbsurface.h"
#include "mmsgui/fb/mmsfbwindow.h"

//! describes the config of a layer
typedef struct {
	//! layer's id
	int 	id;
	//! width
    int     w;
    //! height
    int     h;
    //! pixelformat
    MMSFBSurfacePixelFormat pixelformat;
    //! buffer mode
    string  buffermode;
    //! options
    string  options;
    //! pixelformat for windows
    MMSFBSurfacePixelFormat window_pixelformat;
    //! pixelformat for surfaces
    MMSFBSurfacePixelFormat surface_pixelformat;
} MMSFBLayerConfig;

//! This class describes a display layer.
/*!
\author Jens Schneider
*/
class MMSFBLayer {
    private:
    	//! is initialized?
    	bool initialized;

#ifdef  __HAVE_DIRECTFB__
    	//! interface to the dfb layer
        IDirectFBDisplayLayer   *dfblayer;
#endif

#ifdef  __HAVE_FBDEV__
        MMSFBSurface	*mmsfbdev_surface;
#endif

#ifdef __HAVE_XLIB__
        XImage  		*x_image1;
        XShmSegmentInfo x_shminfo1;
        XImage  		*x_image2;
        XShmSegmentInfo x_shminfo2;

        XvImage  		*xv_image1;
        XShmSegmentInfo xv_shminfo1;
        XvImage  		*xv_image2;
        XShmSegmentInfo xv_shminfo2;
#endif

        // layer configuration
        MMSFBLayerConfig        config;

        // layers surface
        MMSFBSurface            *surface;

        // flags which are used when flipping
        MMSFBFlipFlags			flipflags;

        // first time flag for createSurface()
        static bool 			firsttime_createsurface;

        // first time flags for createWindow()
        static bool 			firsttime_createwindow_usealpha;
        static bool 			firsttime_createwindow_noalpha;

    public:
        MMSFBLayer(int id);
        virtual ~MMSFBLayer();

        bool isInitialized();

        bool getID(int *id);
        bool setExclusiveAccess();
        bool getConfiguration(MMSFBLayerConfig *config = NULL);
        bool getResolution(int *w, int *h);
        bool getPixelFormat(MMSFBSurfacePixelFormat *pixelformat);
        bool setConfiguration(int w=0, int h=0, MMSFBSurfacePixelFormat pixelformat=MMSFB_PF_NONE, string buffermode="", string options="",
							  MMSFBSurfacePixelFormat window_pixelformat=MMSFB_PF_NONE, MMSFBSurfacePixelFormat surface_pixelformat=MMSFB_PF_NONE);
        bool setOpacity(unsigned char opacity);
        bool setLevel(int level);
        bool getSurface(MMSFBSurface **surface);

        bool setFlipFlags(MMSFBFlipFlags flags);

        bool createSurface(MMSFBSurface **surface, int w, int h,
						   MMSFBSurfacePixelFormat pixelformat = MMSFB_PF_NONE, int backbuffer = 0);
        bool createWindow(MMSFBWindow **window, int x, int y, int w, int h,
						   MMSFBSurfacePixelFormat pixelformat = MMSFB_PF_NONE,
                           bool usealpha = true, bool uselayersurface = false);

		friend class MMSFBManager;
		friend class MMSFBSurface;
};

#endif /*MMSFBLAYER_H_*/
