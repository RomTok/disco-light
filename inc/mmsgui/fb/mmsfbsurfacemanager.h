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

#ifndef MMSFBSURFACEMANAGER_H_
#define MMSFBSURFACEMANAGER_H_

#include "mmstools/mmslogger.h"
#include "mmsgui/fb/mmsfbsurface.h"

typedef struct {
    MMSFBSurface    *surface;
    time_t          insert_time;
} MMSFBSURMANLIST;

class MMSFBSurfaceManager {
    private:
        vector<MMSFBSURMANLIST> used_surfaces;
        vector<MMSFBSURMANLIST> free_surfaces;

        MMSFBSurface *tempsuf;

        int surface_mem_cnt;

    public:
        MMSFBSurfaceManager();
        ~MMSFBSurfaceManager();
        MMSFBSurface *createSurface(int w, int h, MMSFBSurfacePixelFormat pixelformat, int backbuffer, bool systemonly);
        void releaseSurface(MMSFBSurface *surface);

        bool createTemporarySurface(int w, int h, MMSFBSurfacePixelFormat pixelformat, bool systemonly);
        MMSFBSurface *getTemporarySurface(int w, int h);
        void releaseTemporarySurface(MMSFBSurface *tempsuf);
};

/* access to global mmsfbsurfacemanager */
extern MMSFBSurfaceManager *mmsfbsurfacemanager;

#endif /*MMSFBSURFACEMANAGER_H_*/
