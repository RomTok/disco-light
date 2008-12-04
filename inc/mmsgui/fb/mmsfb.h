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

#ifndef MMSFB_H_
#define MMSFB_H_

#include "mmstools/mmslogger.h"

#include "mmsgui/fb/mmsfblayer.h"
#include "mmsgui/fb/mmsfbwindowmanager.h"
#include "mmsgui/fb/mmsfbfont.h"

#define MMSFBLAYER_MAXNUM 32

//! The lowest layer to the backends like DFB or X11(XV).
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

        MMSFBLayer 		*layer[MMSFBLAYER_MAXNUM];

        string 			outputtype;
        int				w;
        int				h;
        MMSFB_BACKEND	backend;

#ifdef __HAVE_XLIB__
        Display 		*x_display;
        int				x_screen;
        Window 			x_window;
        GC 				x_gc;
        int 			xv_port;
        MMSMutex		xlock;
        int				display_w;
        int				display_h;
        int				target_window_w;
        int				target_window_h;
        bool            fullscreen;
        bool            resized;
        bool 			resizewindow();
#endif

    public:
        MMSFB();
        virtual ~MMSFB();

        bool init(int argc, char **argv, string outputtype, int w, int h, bool extendedaccel, bool fullscreen);
        bool release();
        bool isInitialized();

        MMSFB_BACKEND getBackend();

        bool getLayer(int id, MMSFBLayer **layer);

        void *getX11Window();
        void *getX11Display();
        bool refresh();

        bool createSurface(MMSFBSurface **surface, int w, int h, MMSFBSurfacePixelFormat pixelformat, int backbuffer = 0, bool systemonly = false);

#ifdef  __HAVE_DIRECTFB__
        bool createImageProvider(IDirectFBImageProvider **provider, string filename);
#endif
        bool createFont(MMSFBFont **font, string filename, int width = 0, int height = 0);
        bool toggleFullscreen();

    friend class MMSFBLayer;
    friend class MMSFBSurface;
    friend class MMSFBFont;
    friend class MMSInputX11Handler;
};

/* access to global mmsfb */
extern MMSFB *mmsfb;

#endif /*MMSFB_H_*/
