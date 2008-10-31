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

class MMSFB {
    private:
        int             argc;       /* commandline arguments */
        char            **argv;

        IDirectFB       *dfb;       /* interface to dfb */

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
#endif

    public:
        MMSFB();
        virtual ~MMSFB();

        bool init(int argc, char **argv, string outputtype, int w, int h);
        bool release();
        bool isInitialized();

        MMSFB_BACKEND getBackend();

        bool getLayer(int id, MMSFBLayer **layer);

        void *getX11Window();
        void *getX11Display();

        bool createSurface(MMSFBSurface **surface, int w, int h, string pixelformat, int backbuffer = 0, bool systemonly = false);

        bool createImageProvider(IDirectFBImageProvider **provider, string filename);
        bool createFont(IDirectFBFont **font, string filename, DFBFontDescription *desc);

    friend class MMSFBLayer;
    friend class MMSFBSurface;
};

/* access to global mmsfb */
extern MMSFB *mmsfb;

#endif /*MMSFB_H_*/
