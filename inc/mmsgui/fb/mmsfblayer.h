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
    string  pixelformat;
    //! buffer mode
    string  buffermode;
    //! options
    string  options;
    //! pixelformat for windows
    string  window_pixelformat;
    //! pixelformat for surfaces
    string  surface_pixelformat;
} MMSFBLayerConfig;

class MMSFBLayer {
    private:
    	// interface to the dfb layer
        IDirectFBDisplayLayer   *dfblayer;

#ifdef __HAVE_XLIB__
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
        MMSFBSurfaceFlipFlags	flipflags;

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
        bool getPixelformat(string *pixelformat);
        bool setConfiguration(int w=0, int h=0, string pixelformat="", string buffermode="", string options="",
        					  string window_pixelformat = "", string surface_pixelformat = "");
        bool setOpacity(unsigned char opacity);
        bool setLevel(int level);
        bool getSurface(MMSFBSurface **surface);

        bool setFlipFlags(MMSFBSurfaceFlipFlags flags);

        bool createSurface(MMSFBSurface **surface, int w, int h,
                           string pixelformat = MMSFB_PF_NONE, int backbuffer = 0);
        bool createWindow(MMSFBWindow **window, int x, int y, int w, int h,
                           string pixelformat = MMSFB_PF_NONE,
                           bool usealpha = true, bool uselayersurface = false);

		friend class MMSFBManager;
};

#endif /*MMSFBLAYER_H_*/
