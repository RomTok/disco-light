/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
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

#include "mmsgui/fb/mmsfbsurfacemanager.h"
#include "mmsgui/fb/mmsfb.h"

/* initialize the mmsfbsurfacemanager object */ 
MMSFBSurfaceManager *mmsfbsurfacemanager = new MMSFBSurfaceManager();

MMSFBSurfaceManager::MMSFBSurfaceManager() {
    this->tempsuf = NULL;
    this->surface_mem_cnt = 0;
}

MMSFBSurfaceManager::~MMSFBSurfaceManager() {
}

MMSFBSurface *MMSFBSurfaceManager::createSurface(int w, int h, string pixelformat, int backbuffer, bool systemonly) {
    DFBResult               dfbres;
    IDirectFBSurface        *dfbsurface;
    DFBSurfaceDescription   surface_desc;
    MMSFBSurface            *surface;

    /* searching for free surface */
    for (unsigned int i = 0; i < this->free_surfaces.size(); i++) {
        surface = free_surfaces.at(i).surface;
        MMSFBSurfaceConfig *sc = &surface->config; 
        if   ((sc->w == w) && (sc->h == h)
            &&(sc->pixelformat == pixelformat) && (sc->backbuffer == backbuffer)) {
            /* found, return it */
            this->free_surfaces.erase(this->free_surfaces.begin()+i);
/*TRACE
            this->used_surfaces.push_back(surface);
*/

//printf("reuse surface=%d,%d\n", w,h);

            return surface;
        }
        else {
            /* this surface is not the right one, check the timestamp */
            if (free_surfaces.at(i).insert_time < time(NULL) - 30) {
                /* the surface is longer than 30 seconds in the free_surfaces list, remove it */

//printf("remove surface=%2,%d\n", w,h);

                if (surface->dfbsurface) {
                    surface->dfbsurface->Release(surface->dfbsurface);
                    surface->dfbsurface = NULL;
                }
                delete surface;
                this->free_surfaces.erase(this->free_surfaces.begin()+i);
            }
        }
    }

    /* create surface description */
    surface_desc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT);
    surface_desc.width = w;
    surface_desc.height = h;
    surface_desc.pixelformat = getDFBPixelFormatFromString(pixelformat);

    if (surface_desc.pixelformat==DSPF_UNKNOWN)
        surface_desc.flags = (DFBSurfaceDescriptionFlags)(surface_desc.flags & ~DSDESC_PIXELFORMAT); 

    /* we use premultiplied surfaces because of alphachannel blitting with better performance */
    surface_desc.flags = (DFBSurfaceDescriptionFlags)(surface_desc.flags | DSDESC_CAPS);
    surface_desc.caps = DSCAPS_PREMULTIPLIED;
    
    switch (backbuffer) {
        case 1: /* front + one back buffer (double) */
            surface_desc.caps = (DFBSurfaceCapabilities)(surface_desc.caps | DSCAPS_DOUBLE);
            break;
        case 2: /* front + two back buffer (triple) */
            surface_desc.caps = (DFBSurfaceCapabilities)(surface_desc.caps | DSCAPS_TRIPLE);
            break;
    }

    /* surface should stored in system memory only? */
    if (systemonly)
    	surface_desc.caps = (DFBSurfaceCapabilities)(surface_desc.caps | DSCAPS_SYSTEMONLY);
    
    /* create the surface */
    if ((dfbres=mmsfb->dfb->CreateSurface(mmsfb->dfb, &surface_desc, &dfbsurface)) != DFB_OK) {
    	DEBUGMSG("MMSGUI", "ERROR");
        MMSFB_SetError(dfbres, "IDirectFB::CreateSurface(" + iToStr(w) + "x" + iToStr(h) + ") failed");
        return NULL;
    }

    /* create a new surface instance */
    surface = new MMSFBSurface(dfbsurface);
    if (!surface) {
        dfbsurface->Release(dfbsurface);
        MMSFB_SetError(0, "cannot create new instance of MMSFBSurface");
        return NULL;
    }

    /* add size of the surface to my global counter */
    int size;
    surface->getMemSize(&size);
    this->surface_mem_cnt+=size;
    DEBUGMSG("MMSGUI", "Allocated surface memory: " + iToStr(this->surface_mem_cnt) + " Byte");
    
    /* add to used surfaces */
/* TRACE
    this->used_surfaces.push_back(surface);
*/

    return surface;
}

void MMSFBSurfaceManager::releaseSurface(MMSFBSurface *surface) {

       /* surface->dfbsurface->Release(surface->dfbsurface);
return;*/



    MMSFBSurface        *new_surface;
    MMSFBSURMANLIST     sml;

    if (!surface)
        return;

    if (surface->config.islayersurface)
        return;

	if (surface->is_sub_surface)
        return;

    /* create a new surface instance */
    new_surface = new MMSFBSurface(NULL);
    if (!new_surface) {
        surface->dfbsurface->Release(surface->dfbsurface);
        return;
    }

    /* set values to new surface */
    new_surface->dfbsurface = surface->dfbsurface;
    new_surface->config = surface->config;

    /* add to free surfaces */
    sml.surface = new_surface;
    sml.insert_time = time(NULL);
    this->free_surfaces.push_back(sml);

    /* remove from used surfaces */
/*TRACE

    for (unsigned int i = 0; i < this->used_surfaces.size(); i++) {
        if (used_surfaces.at(i) == surface) {
            this->used_surfaces.erase(this->used_surfaces.begin()+i);
            break;
        }
    }
*/
}



bool MMSFBSurfaceManager::createTemporarySurface(int w, int h, string pixelformat, bool systemonly) {
	if (!this->tempsuf)
		mmsfb->createSurface(&this->tempsuf, w, h, pixelformat, 0, systemonly);
	if (!this->tempsuf)
		return false;
	return true;
}

MMSFBSurface *MMSFBSurfaceManager::getTemporarySurface(int w, int h) {
	if (!this->tempsuf)
		return NULL;
	this->tempsuf->lock();
	int ww, hh;
	this->tempsuf->getSize(&ww, &hh);
	if ((ww>=w)&&(hh>=h))
		return this->tempsuf;

	DEBUGMSG("MMSGUI", "the temporary surface " + iToStr(ww) + "x" + iToStr(hh) + " is to small - requested size is "
											 + iToStr(w) + "x" + iToStr(h));
	this->tempsuf->unlock();
	return NULL;
}

void MMSFBSurfaceManager::releaseTemporarySurface(MMSFBSurface *tempsuf) {
	if (tempsuf!=this->tempsuf)
		return;
	this->tempsuf->unlock();
}

