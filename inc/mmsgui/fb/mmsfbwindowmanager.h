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

#ifndef MMSFBWINDOWMANAGER_H_
#define MMSFBWINDOWMANAGER_H_

#include "mmstools/tools.h"
#include "mmstools/mmslogger.h"
#include "mmsgui/fb/mmsfblayer.h"
#include "mmsgui/fb/mmsfbwindowmanagerthread.h"

typedef struct {
    MMSFBWindow     *window;        /* window */
    bool            islayersurface; /* the window works direct on the layer */
    MMSFBSurface    *saved_surface; /* copy of surface if window works direct on the layer */
} AVAILABLE_WINDOWS;

typedef struct {
    MMSFBWindow     *window;        /* window */
    MMSFBSurface    *surface;       /* surface of the window */
    DFBRegion       region;         /* region of the window within layer */
    bool            alphachannel;   /* use of alpha value */
    unsigned char   opacity;        /* opacity of the window */
    int             lastflip;       /* last flip time in milliseconds */
    bool            islayersurface; /* the window works direct on the layer */
    MMSFBSurface    *saved_surface; /* copy of surface if window works direct on the layer */
} VISIBLE_WINDOWS;

class MMSFBWindowManager {
    private:
        MMSFBLayer      *layer;         /* layer on which the windows will be drawn */
        MMSFBSurface    *layer_surface; /* layer's surface */

        vector<AVAILABLE_WINDOWS> windows;  /* a list of created windows */

        vector<VISIBLE_WINDOWS> vwins;      /* a list of currently visible windows */

        MMSFBSurface    *high_freq_surface; /* surface which will flipped with high frequency */
        MMSFBSurface    *high_freq_saved_surface;
        DFBRegion       high_freq_region;   /* rectangle which will flipped with high frequency */
        int             high_freq_lastflip; /* last flip time of the high_freq_region */

        bool 			show_pointer;
        int				pointer_posx;
        int				pointer_posy;
        DFBRectangle	pointer_rect;
        DFBRegion		pointer_region;
        MMSFBSurface	*pointer_surface;
        unsigned char	pointer_opacity;
        bool			button_pressed;

        MMSMutex lock;            /* to make it thread-safe */

        MMSFBWindowManagerThread *mmsfbwinmanthread;

        void lockWM();
        void unlockWM();

        bool addWindow(MMSFBWindow *window);
        bool removeWindow(MMSFBWindow *window);

        bool raiseToTop(MMSFBWindow *window);
        bool lowerToBottom(MMSFBWindow *window);

        bool loadWindowConfig(MMSFBWindow *window, VISIBLE_WINDOWS *vwin);

        bool showWindow(MMSFBWindow *window, bool locked = false, bool refresh = true);
        bool hideWindow(MMSFBWindow *window, bool locked = false, bool refresh = true);

        bool flipSurface(MMSFBSurface *surface, DFBRegion *region = NULL,
                         bool locked = false, bool refresh = true);

        bool setWindowOpacity(MMSFBWindow *window);
        bool setWindowPosition(MMSFBWindow *window);
        bool setWindowSize(MMSFBWindow *window, int w, int h);

        bool loadPointer();
        void drawPointer(DFBRegion *region);
        unsigned char getPointerOpacity();
        void setPointerOpacity(unsigned char opacity);

    public:
        MMSFBWindowManager();
        virtual ~MMSFBWindowManager();

        bool init(MMSFBLayer *layer, bool show_pointer = false);
        bool getLayer(MMSFBLayer **layer);

        void setPointerPosition(int pointer_posx, int pointer_posy, bool pressed = false);
        bool getPointerPosition(int &pointer_posx, int &pointer_posy);

    friend class MMSFBLayer;
    friend class MMSFBSurface;
    friend class MMSFBWindow;
    friend class MMSFBWindowManagerThread;
};

/* access to global mmsfbwindowmanager */
extern MMSFBWindowManager *mmsfbwindowmanager;

#endif /*MMSFBWINDOWMANAGER_H_*/
