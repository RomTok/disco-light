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

#include "mmstools/mmstypes.h"

string getMMSFBBackendString(MMSFBBackend be) {
    if(be == MMSFB_BE_DFB)
        return MMSFB_BE_DFB_STR;
    if(be == MMSFB_BE_X11)
        return MMSFB_BE_X11_STR;
    if(be == MMSFB_BE_FBDEV)
        return MMSFB_BE_FBDEV_STR;
    return MMSFB_BE_NONE_STR;
}

MMSFBBackend getMMSFBBackendFromString(string be) {
    if(be == MMSFB_BE_NONE_STR)
        return MMSFB_BE_NONE;
    if(be == MMSFB_BE_DFB_STR)
        return MMSFB_BE_DFB;
    if(be == MMSFB_BE_X11_STR)
        return MMSFB_BE_X11;
    if(be == MMSFB_BE_FBDEV_STR)
        return MMSFB_BE_FBDEV;
    return MMSFB_BE_NONE;
}


string getMMSFBFullScreenModeString(MMSFBFullScreenMode fsm) {
    if(fsm == MMSFB_FSM_FALSE)
        return MMSFB_FSM_FALSE_STR;
    if(fsm == MMSFB_FSM_TRUE)
        return MMSFB_FSM_TRUE_STR;
    if(fsm == MMSFB_FSM_ASPECT_RATIO)
        return MMSFB_FSM_ASPECT_RATIO_STR;
    return MMSFB_FSM_NONE_STR;
}

MMSFBFullScreenMode getMMSFBFullScreenModeFromString(string fsm) {
    if(fsm == MMSFB_FSM_NONE_STR)
        return MMSFB_FSM_NONE;
    if(fsm == MMSFB_FSM_FALSE_STR)
        return MMSFB_FSM_FALSE;
    if(fsm == MMSFB_FSM_TRUE_STR)
        return MMSFB_FSM_TRUE;
    if(fsm == MMSFB_FSM_ASPECT_RATIO_STR)
        return MMSFB_FSM_ASPECT_RATIO;
    return MMSFB_FSM_NONE;
}


