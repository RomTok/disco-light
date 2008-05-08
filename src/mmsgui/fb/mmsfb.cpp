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

#include "mmsgui/fb/mmsfb.h"
#include "mmsgui/fb/mmsfbsurfacemanager.h"

/* initialize the mmsfb object */ 
MMSFB *mmsfb = new MMSFB();

#define INITCHECK  if(!this->dfb){MMSFB_SetError(0,"not initialized");return false;}

MMSFB::MMSFB() {
    /* init me */
    this->argc = 0;
    this->argv = NULL;
    this->dfb = NULL;
}

MMSFB::~MMSFB() {
}

bool MMSFB::init(int argc, char **argv) {
    DFBResult dfbres;

    /* check if already initialized */
    if (this->dfb) {
        MMSFB_SetError(0, "already initialized");
        return false;
    }

    /* save arguments */
    this->argc = argc;
    this->argv = argv;

    /* init dfb */
    DirectFBInit(&this->argc,&this->argv);

    /* get interface to dfb */
    if ((dfbres = DirectFBCreate(&this->dfb)) != DFB_OK) {
        MMSFB_SetError(dfbres, "DirectFBCreate() failed");
        return false;
    }

    return true;
}

bool MMSFB::release() {
    if (this->dfb) {
        this->dfb->Release(this->dfb);
        this->dfb = NULL;
    }
    return true;
}

bool MMSFB::isInitialized() {
    return (this->dfb != NULL);
}

bool MMSFB::getLayer(int id, MMSFBLayer **layer) {
    DFBResult               dfbres;
    IDirectFBDisplayLayer   *dfblayer;

    /* check if initialized */
    INITCHECK;

    /* get the layer */
    if ((dfbres = this->dfb->GetDisplayLayer(this->dfb, id, &dfblayer)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFB::GetDisplayLayer(" + iToStr(id) + ") failed");
        return false;
    }

    /* create a new layer instance */
    *layer = new MMSFBLayer(this->dfb, dfblayer);
    if (!*layer) {
        dfblayer->Release(dfblayer);
        MMSFB_SetError(0, "cannot create new instance of MMSFBLayer");
        return false;
    }
    
    return true;
}

bool MMSFB::createSurface(MMSFBSurface **surface, int w, int h, string pixelformat, int backbuffer, bool systemonly) {
    /* check if initialized */
    INITCHECK;

    /* create or reuse a surface */
    *surface = mmsfbsurfacemanager->createSurface(w, h, pixelformat, backbuffer, systemonly);

    if (*surface)
        return true;
    else
        return false;
}

bool MMSFB::createImageProvider(IDirectFBImageProvider **provider, string filename) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* create the provider */
    if ((dfbres=this->dfb->CreateImageProvider(this->dfb, filename.c_str(), provider)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFB::CreateImageProvider(" + filename + ") failed");
        return false;
    }
    
    return true;
}

bool MMSFB::createFont(IDirectFBFont **font, string filename, DFBFontDescription *desc) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* create the font */
    if ((dfbres=this->dfb->CreateFont(this->dfb, filename.c_str(), desc, font)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFB::CreateFont(" + filename + ") failed");
        return false;
    }
    
    return true;
}

