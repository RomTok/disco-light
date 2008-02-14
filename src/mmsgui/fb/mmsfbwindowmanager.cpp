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

#include "mmsgui/fb/mmsfbwindowmanager.h"
#include <directfb.h>

/* initialize the mmsfbwindowmanager object */ 
MMSFBWindowManager *mmsfbwindowmanager = new MMSFBWindowManager();

#define INITCHECK  if(!this->layer){MMSFB_SetError(0,"not initialized");return false;}

MMSFBWindowManager::MMSFBWindowManager() {
    /* init me */
    this->layer = NULL;
    this->layer_surface = NULL;
    this->high_freq_surface = NULL;
    this->high_freq_saved_surface = NULL;
    this->high_freq_region.x1 = 0;
    this->high_freq_region.y1 = 0;
    this->high_freq_region.x2 = 0;
    this->high_freq_region.y2 = 0;
    this->high_freq_lastflip = 0;
    
    this->pointer_posx = -1;        
    this->pointer_posy = -1;        
    
    mmsfbwinmanthread = new MMSFBWindowManagerThread(&this->high_freq_surface,
                                                     &this->high_freq_saved_surface,
                                                     &this->high_freq_lastflip,
                                                     &this->lock);
    mmsfbwinmanthread->start();
}

MMSFBWindowManager::~MMSFBWindowManager() {
    for (unsigned int i=0; i < this->windows.size(); i++) {
        delete this->windows.at(i).window;
        if(this->windows.at(i).islayersurface)
            delete this->windows.at(i).saved_surface;
    }
}

bool MMSFBWindowManager::init(MMSFBLayer *layer) {

    /* check if already initialized */
    if (this->layer) {
        MMSFB_SetError(0, "already initialized");
        return false;
    }

    /* save layer */
    this->layer = layer;

    DEBUGMSG("MMSGUI", "MMSFBWindowManager: get layer surface");
    
    /* get the surface of the layer */
    if (!this->layer->getSurface(&this->layer_surface))
        return false;

    return true;
}

bool MMSFBWindowManager::getLayer(MMSFBLayer **layer) {

    /* check if initialized */
    INITCHECK;

    /* return the layer */    
    *layer = this->layer;

    return true;
}

void MMSFBWindowManager::lockWM() {
    /* stop parallel processing */
    lock.lock();
}

void MMSFBWindowManager::unlockWM() {
    /* unlock */
    lock.unlock();
}

bool MMSFBWindowManager::addWindow(MMSFBWindow *window) {

    /* check if initialized */
    INITCHECK;

    /* stop parallel processing */
    lock.lock();

    /* search for duplicate items */
    for (unsigned int i=0; i < this->windows.size(); i++)
        if (this->windows.at(i).window == window) {
            lock.unlock();
            return false;
        }

    /* add window */
    AVAILABLE_WINDOWS awin;
    awin.window = window;
    awin.islayersurface = false;            
    awin.saved_surface = NULL;
    MMSFBSurface *s;
    window->getSurface(&s);
    if (s->isLayerSurface()) {
        /* the window uses the layer surface, allocate a save buffer for it */
        awin.islayersurface = true;            
        int w, h;
        s->getSize(&w, &h);
        string pf;
        s->getPixelFormat(&pf);
        this->layer->createSurface(&(awin.saved_surface), w, h, pf, 0);
        if (awin.saved_surface) {
            awin.saved_surface->clear();
            awin.saved_surface->setBlittingFlags((MMSFBSurfaceBlittingFlags) DSBLIT_NOFX);
        }
    }
    this->windows.push_back(awin);

    /* unlock */
    lock.unlock();

    return true; 
}

bool MMSFBWindowManager::removeWindow(MMSFBWindow *window) {

    /* check if initialized */
    INITCHECK;

    /* stop parallel processing */
    lock.lock();

    /* search for item */
    for (unsigned int i=0; i < this->windows.size(); i++)
        if (this->windows.at(i).window == window) {

            /* hide the window before removing */
            hideWindow(window);

            /* delete the saved_surface if existent */
            if (this->windows.at(i).saved_surface)
                delete this->windows.at(i).saved_surface;

            /* remove it from list */
            this->windows.erase(this->windows.begin()+i);

            /* unlock */
            lock.unlock();
            return true;
        }

    /* not found */
    lock.unlock();
    return false;
}

bool MMSFBWindowManager::raiseToTop(MMSFBWindow *window) {

    /* check if initialized */
    INITCHECK;

    /* stop parallel processing */
    lock.lock();

    /* search for item */
    for (unsigned int i=0; i < this->windows.size(); i++)
        if (this->windows.at(i).window == window) {
            /* window found */
            if (i < this->windows.size()-1) {
                /* window is not at the top, raise it now */
                AVAILABLE_WINDOWS aw = this->windows.at(i);
                this->windows.erase(this->windows.begin()+i);
                this->windows.push_back(aw);

                /* search for item within visible list */
                for (unsigned int j=0; j < this->vwins.size(); j++)
                    if (this->vwins.at(j).window == window) {
                        /* window found */
                        if (i < this->vwins.size()-1) {
                            /* window is not at the top, raise it now */
                            VISIBLE_WINDOWS vw = this->vwins.at(j);
                            this->vwins.erase(this->vwins.begin()+j);
                            this->vwins.push_back(vw);

                            /* draw the window */
                            flipSurface(vw.surface, NULL, (MMSFBSurfaceFlipFlags)0, true);
                        }
                    }
            }

            /* unlock */
            lock.unlock();
            return true;
        }

    /* not found */
    lock.unlock();
    return false;
}

bool MMSFBWindowManager::lowerToBottom(MMSFBWindow *window) {

    /* check if initialized */
    INITCHECK;

    /* stop parallel processing */
    lock.lock();

    /* search for item */
    for (unsigned int i=0; i < this->windows.size(); i++)
        if (this->windows.at(i).window == window) {
            /* window found */
            if (i > 0) {
                /* window is not at the bottom, lower it now */
                AVAILABLE_WINDOWS aw = this->windows.at(i);
                this->windows.erase(this->windows.begin()+i);
                this->windows.insert(this->windows.begin(), aw);

                /* search for item within visible list */
                for (unsigned int j=0; j < this->vwins.size(); j++)
                    if (this->vwins.at(j).window == window) {
                        /* window found */
                        if (j > 0) {
                            /* window is not at the bottom, lower it now */
                            VISIBLE_WINDOWS vw = this->vwins.at(j);
                            this->vwins.erase(this->vwins.begin()+j);
                            this->vwins.insert(this->vwins.begin(), vw);

                            /* draw the window */
                            flipSurface(vw.surface, NULL, (MMSFBSurfaceFlipFlags)0, true);
                        }
                    }
            }

            /* unlock */
            lock.unlock();
            return true;
        }

    /* not found */
    lock.unlock();
    return false;
}


bool MMSFBWindowManager::loadWindowConfig(MMSFBWindow *window, VISIBLE_WINDOWS *vwin) {
    vwin->window = window;
    vwin->window->getSurface(&vwin->surface);
    MMSFBWindowConfig winconf;
    vwin->window->getConfiguration(&winconf);
    vwin->region.x1 = winconf.posx;
    vwin->region.y1 = winconf.posy;
    vwin->region.x2 = vwin->region.x1 + winconf.surface_config.w - 1;
    vwin->region.y2 = vwin->region.y1 + winconf.surface_config.h - 1;
    vwin->alphachannel = winconf.surface_config.alphachannel;
    vwin->opacity = winconf.opacity;
    vwin->lastflip = 0;
    vwin->islayersurface = false;
    vwin->saved_surface = NULL;
    if (vwin->surface->isLayerSurface()) {
        vwin->islayersurface = true;
        for (unsigned int i=0; i < this->windows.size(); i++)
            if (this->windows.at(i).window == window) {
                vwin->saved_surface = this->windows.at(i).saved_surface;
                break;
            }
    }
    return true;
}

bool MMSFBWindowManager::showWindow(MMSFBWindow *window, bool locked, bool refresh) {

    /* check if initialized */
    INITCHECK;

    /* stop parallel processing */
    if (!locked)
        lock.lock();

    /* search for item */
    for (unsigned int i=0; i < this->windows.size(); i++)
        if (this->windows.at(i).window == window) {
            /* search for duplicate items */
            for (unsigned int j=0; j < this->vwins.size(); j++)
                if (this->vwins.at(j).window == window) {
                    /* the window is already visible */
                    if (!locked)
                        lock.unlock();
                    return false;
                }

            /* prepare new list item */
            VISIBLE_WINDOWS vwin;
            loadWindowConfig(window, &vwin);
            
            /* add window to visible list */
            bool inserted = false;
            if (i < this->windows.size()-1) {
                /* first searching for the right position within the window stack */
                for (unsigned int j=0; j < this->vwins.size() && !inserted; j++)
                    for (unsigned int k=0; k < this->windows.size() && !inserted; k++)
                        if (this->vwins.at(j).window == this->windows.at(k).window)
                            if (k > i) {
                                /* insert the window */
                                this->vwins.insert(this->vwins.begin()+j, vwin);
                                inserted = true;
                                break;
                            }
            }
            if (!inserted)
                /* insert at the end (this is the top) */
                this->vwins.push_back(vwin);

            /* draw the window */
            flipSurface(vwin.surface, NULL, (MMSFBSurfaceFlipFlags)0, true, refresh);

            /* unlock */
            if (!locked)
                lock.unlock();

            return true;
        }

    /* not found */
    lock.unlock();
    return false;
}

bool MMSFBWindowManager::hideWindow(MMSFBWindow *window, bool locked, bool refresh) {

    /* check if initialized */
    INITCHECK;

    /* stop parallel processing */
    if (!locked)
        lock.lock();

    /* search for item */
    for (unsigned int i=0; i < this->vwins.size(); i++)
        if (this->vwins.at(i).window == window) {
            /* redraw the window with no opacity because must redrawing other windows */
            this->vwins.at(i).opacity = 0;

            flipSurface(this->vwins.at(i).surface, NULL, (MMSFBSurfaceFlipFlags)0, true, refresh);

            if (this->high_freq_surface==this->vwins.at(i).surface) {
                /* i was the high_freq_surface */
                this->high_freq_surface = NULL;
                this->high_freq_saved_surface = NULL;
                this->high_freq_lastflip = 0;
            }

            /* remove it from list */
            this->vwins.erase(this->vwins.begin()+i);

            /* unlock */
            if (!locked)
                lock.unlock();

            return true;
        }

    /* not found */
    if (!locked)
        lock.unlock();
    return false;
}

bool MMSFBWindowManager::flipSurface(MMSFBSurface *surface, DFBRegion *region,
                                     MMSFBSurfaceFlipFlags flags,
                                     bool locked, bool refresh) {
    VISIBLE_WINDOWS *vw = NULL;
    DFBRegion       ls_region;
    bool            high_freq = false;
    bool            cleared = false;

    /* check if initialized */
    INITCHECK;

    /* stop parallel processing */
    if (!locked)
        lock.lock();

    /* search for item */
    if (surface) {
        /* surface given */
        for (unsigned int i=0; i < this->vwins.size(); i++)
            if (this->vwins.at(i).surface == surface) {
                /* surface found */
                vw = &(this->vwins.at(i));
                DFBRegion *myregion = &(vw->region);
    
                /* calculate the affected region on the layer surface */
                if (region == NULL) {
                    /* complete surface */
                    ls_region = *myregion;
                }
                else {
                    /* only a region */
                    ls_region.x1 = myregion->x1 + region->x1;
                    ls_region.y1 = myregion->y1 + region->y1;
                    ls_region.x2 = myregion->x1 + region->x2;
                    ls_region.y2 = myregion->y1 + region->y2;
                }
                break;
            }

        if (!vw) {
            /* not found */
            if (!locked)
                lock.unlock();
            return false;
        }
    }
    else {
        /* no surface given, have to redraw a region? */
        if (region == NULL) {
            /* no */
            if (!locked)
                lock.unlock();
            return false;
        }

        /* take this region */
        ls_region = *region;
    }

    if (region == NULL) {
        /* this is only for whole (window) surfaces with an high flip frequency */ 
        struct  timeval tv;
        /* get the flip time */
        gettimeofday(&tv, NULL);
        int newfliptime = (((int)tv.tv_sec)%1000000)*1000+((int)tv.tv_usec)/1000;
        int diff = newfliptime - vw->lastflip;  

        if ((diff > 0)&&(diff < 50)) {
            /* more than 20 pictures per second comes from this surface */
            high_freq = true;
        }

        if (vw->saved_surface) {
            /* save the frames if window works direct on the layer surface */
            if (vw->lastflip % 1000 < 40) {
/*
logger.writeLog("AAA>");

void        *  ret_ptr_src;
int            ret_pitch_src;
void        *  ret_ptr_dst;
int            ret_pitch_dst;

vw->saved_surface->getDFBSurface()->Lock (vw->saved_surface->getDFBSurface(),
                                          DSLF_WRITE, &ret_ptr_dst, &ret_pitch_dst);
this->layer_surface->getDFBSurface()->Lock (this->layer_surface->getDFBSurface(),
                                          DSLF_WRITE, &ret_ptr_src, &ret_pitch_src);

memcpy(ret_ptr_dst, ret_ptr_src, ret_pitch_src * 576);

vw->saved_surface->getDFBSurface()->Unlock (vw->saved_surface->getDFBSurface());
this->layer_surface->getDFBSurface()->Unlock (this->layer_surface->getDFBSurface());

logger.writeLog("BBB>");
*/
            }
        }

        vw->lastflip = newfliptime;
    }

    if (high_freq) {
        /* this surface has an high flip frequency */
        if (!this->high_freq_surface) {
            /* this->high_freq_surface is not set, set it now */
            this->high_freq_region = ls_region;
            this->high_freq_lastflip = vw->lastflip;
            this->high_freq_surface = vw->surface;
            this->high_freq_saved_surface = vw->saved_surface;
        }
        else
            /* update the high_freq_lastflip */
            this->high_freq_lastflip = vw->lastflip;
    }
    else {
        bool check = (this->high_freq_surface!=NULL);
        if ((check)&&(vw))
            check = (this->high_freq_surface!=vw->surface);
        if (check) {
            /* high_freq_surface is set and i am not this surface */ 
            /* check if i am within high_freq_region */
            if ((this->high_freq_region.x1 <= ls_region.x1)
              &&(this->high_freq_region.y1 <= ls_region.y1)
              &&(this->high_freq_region.x2 >= ls_region.x2)
              &&(this->high_freq_region.y2 >= ls_region.y2)) {
                /* yes, have to flip nothing */
                if (!locked)
                    lock.unlock();
                return true;
            }
        }
        else {
            if ((this->high_freq_surface)&&(vw))
                /* update the high_freq_lastflip */
                this->high_freq_lastflip = vw->lastflip;
        }
    }

    /* set the region of the layer surface */
    this->layer_surface->setClip(&ls_region);

    /* check if i have to clear the background */
    if (!vw)
        cleared = true;
    else
        cleared = (!((vw->alphachannel==false)&&(vw->opacity==255)));

    if (cleared)
        this->layer_surface->clear();

    /* search for other affected windows and draw parts of it */
    for (unsigned int i=0; i < this->vwins.size(); i++) {
        VISIBLE_WINDOWS *aw = &(this->vwins.at(i));
        DFBRegion *myregion = &(aw->region);

        /* if the window has no opacity then continue */
        if (!aw->opacity)
            continue;

        /* check if layer surface */
        if (aw->islayersurface)
            if (!cleared)
                continue;

        if (!((myregion->x2 < ls_region.x1)||(myregion->y2 < ls_region.y1)
            ||(myregion->x1 > ls_region.x2)||(myregion->y1 > ls_region.y2))) {

            /* the window is affected */
            /* calc source and destination */
            DFBRectangle src_rect;
            int dst_x = ls_region.x1;
            int dst_y = ls_region.y1;

            src_rect.x = ls_region.x1 - myregion->x1;
            if (src_rect.x < 0) {
                dst_x-= src_rect.x;
                src_rect.x = 0;
            }

            src_rect.y = ls_region.y1 - myregion->y1;
            if (src_rect.y < 0) {
                dst_y-= src_rect.y;
                src_rect.y = 0;
            }

            src_rect.w = myregion->x2 - myregion->x1 + 1 - src_rect.x;
            if (myregion->x2 > ls_region.x2)
                src_rect.w-= myregion->x2 - ls_region.x2;

            src_rect.h = myregion->y2 - myregion->y1 + 1 - src_rect.y;
            if (myregion->y2 > ls_region.y2)
                src_rect.h-= myregion->y2 - ls_region.y2;

            /* set the blitting flags and color */
            if (aw->alphachannel)
                if (aw->opacity < 255) { 
                    this->layer_surface->setBlittingFlags((MMSFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA));
                    this->layer_surface->setColor(0, 0, 0, aw->opacity);
                }
                else {
                    this->layer_surface->setBlittingFlags((MMSFBSurfaceBlittingFlags) DSBLIT_BLEND_ALPHACHANNEL);
                }
            else
                if (aw->opacity < 255) { 
                    this->layer_surface->setBlittingFlags((MMSFBSurfaceBlittingFlags) DSBLIT_BLEND_COLORALPHA);
                    this->layer_surface->setColor(0, 0, 0, aw->opacity);
                }
                else {
                    this->layer_surface->setBlittingFlags((MMSFBSurfaceBlittingFlags) DSBLIT_NOFX);
                }
            /* check if layer surface and blit */
            if (aw->islayersurface) {
                if (aw->saved_surface) {
                    this->layer_surface->blit(aw->saved_surface, &src_rect, dst_x, dst_y);
                }
            }
            else {
                this->layer_surface->blit(aw->surface, &src_rect, dst_x, dst_y);
            }
        }
    }

    
    
    this->layer_surface->setClip(NULL);

    /* draw pointer */
    if ((this->pointer_posx>=0)&&(this->pointer_posy>=0)) {
	    this->layer_surface->setColor(255,255,255,255);
	    this->layer_surface->drawLine(this->pointer_posx-10,this->pointer_posy,this->pointer_posx+10,this->pointer_posy);
	    this->layer_surface->drawLine(this->pointer_posx,this->pointer_posy-10,this->pointer_posx,this->pointer_posy+10);
	    this->layer_surface->setColor(0,0,0,0);
    }
    
    /* make changes visible */
    if (refresh)
//        this->layer_surface->flip(&ls_region, flags);
        this->layer_surface->flip(&ls_region, (MMSFBSurfaceFlipFlags)(DSFLIP_WAITFORSYNC));
        //this->layer_surface->flip(&ls_region, (MMSFBSurfaceFlipFlags)DSFLIP_NONE);



    
    
    /* unlock */
    if (!locked)
        lock.unlock();

    return true;
}

bool MMSFBWindowManager::setWindowOpacity(MMSFBWindow *window) {

    /* check if initialized */
    INITCHECK;

    /* stop parallel processing */
    lock.lock();

    /* search for item */
    for (unsigned int i=0; i < this->vwins.size(); i++)
        if (this->vwins.at(i).window == window) {
            /* reload windows config */
            loadWindowConfig(window, &(this->vwins.at(i)));

            /* redraw the window */
            flipSurface(this->vwins.at(i).surface, NULL, (MMSFBSurfaceFlipFlags)0, true);

            /* unlock */
            lock.unlock();

            return true;
        }

    /* not found */
    lock.unlock();
    return false;
}

bool MMSFBWindowManager::setWindowPosition(MMSFBWindow *window) {

    /* check if initialized */
    INITCHECK;

    /* stop parallel processing */
    lock.lock();

    /* search for item */
    for (unsigned int i=0; i < this->vwins.size(); i++)
        if (this->vwins.at(i).window == window) {
            /* get the old config */
            VISIBLE_WINDOWS old_vwin;
            old_vwin = this->vwins.at(i);

            /* reload windows config */
            loadWindowConfig(window, &(this->vwins.at(i)));

            /* moving high_freq_surface? */
            if (this->high_freq_surface == this->vwins.at(i).surface) {
                /* yes, reset it */
                mmsfbwindowmanager->flipSurface(this->high_freq_surface, NULL,
                                                (MMSFBSurfaceFlipFlags)0, true);
                this->high_freq_surface = NULL;
                this->high_freq_saved_surface = NULL;
                this->high_freq_lastflip = 0;
            }

            /* redraw the window */
            flipSurface(this->vwins.at(i).surface, NULL, (MMSFBSurfaceFlipFlags)0, true);

            /* redraw the old rects */
            if (old_vwin.region.y1 < this->vwins.at(i).region.y1) {
                /* redraw above */
                DFBRegion region;
                region = old_vwin.region;
                if (region.y2 >= this->vwins.at(i).region.y1)
                    region.y2 = this->vwins.at(i).region.y1 - 1;
                flipSurface(NULL, &region, (MMSFBSurfaceFlipFlags)0, true);
            }
            else 
            if (old_vwin.region.y1 > this->vwins.at(i).region.y1) {
                /* redraw below */
                DFBRegion region;
                region = old_vwin.region;
                if (region.y1 <= this->vwins.at(i).region.y2)
                    region.y1 = this->vwins.at(i).region.y2 + 1;
                flipSurface(NULL, &region, (MMSFBSurfaceFlipFlags)0, true);
            } 
            if (old_vwin.region.x1 < this->vwins.at(i).region.x1) {
                /* redraw left side */
                DFBRegion region;
                region = old_vwin.region;
                if  ((region.y2 >= this->vwins.at(i).region.y1)
                   &&(region.y1 <= this->vwins.at(i).region.y2)) {
                    if (region.x2 >= this->vwins.at(i).region.x1)
                        region.x2 = this->vwins.at(i).region.x1 - 1;
                    region.y1 = this->vwins.at(i).region.y1;
                    region.y2 = this->vwins.at(i).region.y2;
                    flipSurface(NULL, &region, (MMSFBSurfaceFlipFlags)0, true);
                }
            }
            else 
            if (old_vwin.region.x1 > this->vwins.at(i).region.x1) {
                /* redraw right side */
                DFBRegion region;
                region = old_vwin.region;
                if  ((region.y2 >= this->vwins.at(i).region.y1)
                   &&(region.y1 <= this->vwins.at(i).region.y2)) {
                    if (region.x1 <= this->vwins.at(i).region.x2)
                        region.x1 = this->vwins.at(i).region.x2 + 1;
                    region.y1 = this->vwins.at(i).region.y1;
                    region.y2 = this->vwins.at(i).region.y2;
                    flipSurface(NULL, &region, (MMSFBSurfaceFlipFlags)0, true);
                }
            } 

            /* unlock */
            lock.unlock();

            return true;
        }

    /* not found */
    lock.unlock();
    return false;
}

bool MMSFBWindowManager::setWindowSize(MMSFBWindow *window, int w, int h) {

    /* check if initialized */
    INITCHECK;

    /* stop parallel processing */
    lock.lock();

    /* search for item which is visible */
    for (unsigned int i=0; i < this->vwins.size(); i++)
        if (this->vwins.at(i).window == window) {
            /* found as visible window */
            VISIBLE_WINDOWS old_vwin;
            old_vwin = this->vwins.at(i);
            int old_w = old_vwin.region.x2 - old_vwin.region.x1 + 1; 
            int old_h = old_vwin.region.y2 - old_vwin.region.y1 + 1; 

            if ((old_w != w)||(old_h != h)) { 
                /* hide the window without updating the screen */
                hideWindow(window, true, false);
        
                /* resizing surface */
                MMSFBSurface *surface;
                window->getSurface(&surface);
                surface->resize(w, h); 
        
                /* re-show it */
                if ((old_w <= w)&&(old_h <= h)) 
                    /* larger or equal */
                    showWindow(window, true, true);
                else {
                    /* new window is less than the old one */
                    showWindow(window, true, false);

                    /* flip the old region */
                    flipSurface(NULL, &old_vwin.region, (MMSFBSurfaceFlipFlags)0,
                                true, true);
                }
            }

            /* unlock */
            lock.unlock();

            return true;
        }

    /* search for item in the window list */
    for (unsigned int i=0; i < this->windows.size(); i++)
        if (this->windows.at(i).window == window) {
            /* found as window which is currently not shown */
            /* resizing surface */
            MMSFBSurface *surface;
            window->getSurface(&surface);
            surface->resize(w, h); 

            /* unlock */
            lock.unlock();

            return true;
        }

    /* not found */
    lock.unlock();
    return false;
}

void MMSFBWindowManager::setPointerPosition(int pointer_posx, int pointer_posy) {
	if ((this->pointer_posx == pointer_posx)&&(this->pointer_posy == pointer_posy))
		return;
	this->pointer_posx = pointer_posx;
	this->pointer_posy = pointer_posy;

	DFBRegion region;
	region.x1 = this->pointer_posx - 10; 
	region.y1 = this->pointer_posy - 10; 
	region.x2 = this->pointer_posx + 10; 
	region.y2 = this->pointer_posy + 10; 
	flipSurface(NULL, &region, (MMSFBSurfaceFlipFlags)0, false);
}


