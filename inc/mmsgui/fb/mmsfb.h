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

#ifndef MMSFB_H_
#define MMSFB_H_

#include "mmstools/mmstypes.h"
#include "mmstools/mmslogger.h"

#include "mmsgui/fb/mmsfbdev.h"
#include "mmsgui/fb/mmsfbdevmatrox.h"
#include "mmsgui/fb/mmsfblayer.h"
#include "mmsgui/fb/mmsfbwindowmanager.h"
#include "mmsgui/fb/mmsfbfont.h"

#define MMSFBLAYER_MAXNUM 32

//! The lowest layer to the backends like DFB, X11(XSHM/XVSHM) or FBDEV.
/*!
\author Jens Schneider
*/
class MMSFB {
    private:
        int             argc;       /* commandline arguments */
        char            **argv;

    	//! is initialized?
    	bool initialized;

#ifdef  __HAVE_DIRECTFB__
        // interface to dfb
        IDirectFB       *dfb;
#endif

#ifdef  __HAVE_FBDEV__
        // interface to own FB device
        MMSFBDev		*mmsfbdev;
#endif

        MMSFBLayer 		*layer[MMSFBLAYER_MAXNUM];

        MMSFBBackend	backend;
        MMSFBOutputType	outputtype;
        int				w;
        int				h;
        int				x;
        int				y;

#ifdef __HAVE_XLIB__
        Display 		*x_display;
        int				x_screen;
        Window 			x_window;
        GC 				x_gc;
        Visual			*x_visual;
        int				x_depth;
        int 			xv_port;
        MMSMutex		xlock;
        int				display_w;
        int				display_h;
        int				target_window_w;
        int				target_window_h;
        MMSFBFullScreenMode fullscreen;
        bool            resized;
        bool 			resizeWindow();
#endif

    public:
        MMSFB();
        virtual ~MMSFB();

        bool init(int argc, char **argv, MMSFBBackend backend, MMSFBOutputType outputtype, int w, int h, bool extendedaccel, MMSFBFullScreenMode fullscreen, string pointer,
				  string appl_name = "Disko Application", string appl_icon_name = "Disko Application",int x=50, int y=50);
        bool release();
        bool isInitialized();

        MMSFBBackend getBackend();

        bool getLayer(int id, MMSFBLayer **layer);

        void *getX11Window();
        void *getX11Display();
        bool refresh();

        bool createSurface(MMSFBSurface **surface, int w, int h, MMSFBSurfacePixelFormat pixelformat, int backbuffer = 0, bool systemonly = false);

#ifdef  __HAVE_DIRECTFB__
        bool createImageProvider(IDirectFBImageProvider **provider, string filename);
#endif
        bool createFont(MMSFBFont **font, string filename, int width = 0, int height = 0);

    friend class MMSFBLayer;
    friend class MMSFBSurface;
    friend class MMSFBFont;
    friend class MMSInputX11Handler;
    friend class MMSInputLISHandler;
};

/* access to global mmsfb */
extern MMSFB *mmsfb;

#endif /*MMSFB_H_*/
