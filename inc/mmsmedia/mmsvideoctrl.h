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

#ifdef  __HAVE_DIRECTFB__

#ifndef MMSVIDEOCTRL_H_
#define MMSVIDEOCTRL_H_

#include <directfb.h>

#include "mmstools/mmstools.h"

/**
 * @brief   Controls the video device.
 *
 * @ingroup     mmsmedia mmslibs
 *
 * @author      Jens Schneider (pupeider@morphine.tv)
 * @version     1.0.3
 * @date        11/07/2007
 *
 * This class controls global video output settings.
 */
class MMSVideoCtrl {
    private:
        IDirectFB                           *dfb;
        IDirectFBDisplayLayer               *layer;
        static DFBDisplayLayerDescription   desc;
        static DFBColorAdjustment           cadj;

    public:
        /* constructor */
        MMSVideoCtrl();

        /* destructor */
        ~MMSVideoCtrl();

        /* get the description and capabilities of the display layer */
        DFBDisplayLayerDescription getDisplayLayerDescription();

        /* get the current color adjustment of the display layer */
        DFBColorAdjustment getColorAdjustment();

        /* adjust brightness */
        void adjustBrightness(unsigned val = 0x8000);

        /* adjust contrast */
        void adjustContrast(unsigned val = 0x8000);

        /* adjust hue */
        void adjustHue(unsigned val = 0x8000);

        /* adjust saturation */
        void adjustSaturation(unsigned val = 0x8000);
};

#endif /*MMSVIDEOCTRL_H_*/

#endif
