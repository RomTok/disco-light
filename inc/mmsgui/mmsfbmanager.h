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

#ifndef MMSFBMANAGER_H_
#define MMSFBMANAGER_H_

#include "mmsgui/fb/mmsfb.h"
#include "mmsconfig/mmsconfigdata.h"
#include "mmsgui/mmsguilogger.h"

#define MMS_OT_VESAFB       "vesafb"
#define MMS_OT_MATROXFB     "matroxfb"
#define MMS_OT_VIAFB        "viafb"
#define MMS_OT_X11FB        "x11"
#define MMS_OT_SDLFB        "sdl"

class MMSFBManager {

    private:
        MMSGuiLogger        logger;

        MMSFBLayer          *graphicslayer;
        MMSFBLayer          *videolayer;
        int                 videolayerid;
        int                 graphicslayerid;
        MMSConfigData       config;
        int                 layercount;     
        
    public:
        /* constructor */
        MMSFBManager();

        /* destructor */
        ~MMSFBManager();

        bool init(int argc, char **argv);
        void release();

        void applySettings();
        MMSFBLayer *getVideoLayer();
        MMSFBLayer *getGraphicsLayer();

        int getLayerCount();  
};

/* access to global mmsfbmanager */
extern MMSFBManager mmsfbmanager;

#endif /*MMSFBMANAGER_H_*/
