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

#ifndef MMSFBWINDOW_H_
#define MMSFBWINDOW_H_

#include "mmstools/mmslogger.h"
#include "mmsgui/fb/mmsfbbase.h"
#include "mmsgui/fb/mmsfbsurface.h"

typedef struct {
    MMSFBSurfaceConfig  surface_config; /* windows surface config */
    int                 posx;           /* pos x */
    int                 posy;           /* pos y */
    unsigned char       opacity;        /* opacity of the window */
    bool                shown;          /* is the window currently shown */
} MMSFBWindowConfig;

//! This class describes a window on a specific layer.
/*!
\author Jens Schneider
*/
class MMSFBWindow {
    private:
#ifdef  __HAVE_DIRECTFB__
    	//! dfb window if used
        IDirectFBWindow     *dfbwindow;
#endif

        MMSFBSurface        *surface;   /* windows surface */

        MMSFBWindowConfig   config;     /* surface configuration */

    public:
#ifdef USE_DFB_WINMAN
        MMSFBWindow(IDirectFBWindow *dfbwindow, int x, int y);
#endif
#ifdef USE_MMSFB_WINMAN
        MMSFBWindow(MMSFBSurface *surface, int x, int y);
#endif
        virtual ~MMSFBWindow();

        bool getSurface(MMSFBSurface **surface);

        bool getConfiguration(MMSFBWindowConfig *config = NULL);

        bool isShown();
        bool show();
        bool hide();

        bool getOpacity(unsigned char *opacity);
        bool setOpacity(unsigned char opacity);

        bool getPosition(int *x, int *y);
        bool moveTo(int x, int y);

        bool getSize(int *w, int *h);
        bool resize(int w, int h);

        bool raiseToTop();
        bool lowerToBottom();
};

#endif /*MMSFBWINDOW_H_*/
