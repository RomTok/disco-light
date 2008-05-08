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

#include "mmstools/mmstools.h"
#include "mmstools/mmslogger.h"
#include "mmsgui/fb/mmsfbbase.h"
#include "mmsgui/fb/mmsfbsurface.h"
#include "mmsgui/fb/mmsfbwindow.h"


typedef struct {
    int     w;              /* width */
    int     h;              /* height */
    string  pixelformat;    /* pixel format */
    string  buffermode;     /* buffer mode */
    string  options;        /* options */
} MMSFBLayerConfig;

class MMSFBLayer {
    private:
        IDirectFB               *dfb;       /* interface to dfb */
        IDirectFBDisplayLayer   *dfblayer;  /* interface to the layer */

        MMSFBLayerConfig        config;     /* layer configuration */

        MMSFBSurface            *surface;   /* layers surface */

        MMSFBSurfaceFlipFlags	flipflags;	/* flags which are used when flipping */
        
    public:
        MMSFBLayer(IDirectFB *dfb, IDirectFBDisplayLayer *dfblayer);
        virtual ~MMSFBLayer(); 

        bool getID(int *id);
        bool setExclusiveAccess();
        bool getConfiguration(MMSFBLayerConfig *config = NULL);
        bool getResolution(int *w, int *h);
        bool getPixelformat(string *pixelformat);
        bool setConfiguration(int w=0, int h=0, string pixelformat="", string buffermode="", string options="");
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
