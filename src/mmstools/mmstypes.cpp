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

string getMMSFBOutputTypeString(MMSFBOutputType ot) {
    if(ot == MMSFB_OT_VESAFB)
        return MMSFB_OT_VESAFB_STR;
    if(ot == MMSFB_OT_MATROXFB)
        return MMSFB_OT_MATROXFB_STR;
    if(ot == MMSFB_OT_VIAFB)
        return MMSFB_OT_VIAFB_STR;
    if(ot == MMSFB_OT_X11)
        return MMSFB_OT_X11_STR;
    if(ot == MMSFB_OT_XSHM)
        return MMSFB_OT_XSHM_STR;
    if(ot == MMSFB_OT_XVSHM)
        return MMSFB_OT_XVSHM_STR;
    return MMSFB_OT_NONE_STR;
}

MMSFBOutputType getMMSFBOutputTypeFromString(string ot) {
	if(ot == MMSFB_OT_NONE_STR)
        return MMSFB_OT_NONE;
	if(ot == MMSFB_OT_VESAFB_STR)
        return MMSFB_OT_VESAFB;
	if(ot == MMSFB_OT_MATROXFB_STR)
        return MMSFB_OT_MATROXFB;
	if(ot == MMSFB_OT_VIAFB_STR)
        return MMSFB_OT_VIAFB;
	if(ot == MMSFB_OT_X11_STR)
        return MMSFB_OT_X11;
	if(ot == MMSFB_OT_XSHM_STR)
        return MMSFB_OT_XSHM;
	if(ot == MMSFB_OT_XVSHM_STR)
        return MMSFB_OT_XVSHM;
    return MMSFB_OT_NONE;
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



string getMMSFBPixelFormatString(MMSFBSurfacePixelFormat pf) {
    if(pf == MMSFB_PF_RGB16)
        return MMSFB_PF_RGB16_STR;
    if(pf == MMSFB_PF_RGB24)
        return MMSFB_PF_RGB24_STR;
    if(pf == MMSFB_PF_RGB32)
        return MMSFB_PF_RGB32_STR;
    if(pf == MMSFB_PF_ARGB)
        return MMSFB_PF_ARGB_STR;
    if(pf == MMSFB_PF_A8)
        return MMSFB_PF_A8_STR;
    if(pf == MMSFB_PF_YUY2)
        return MMSFB_PF_YUY2_STR;
    if(pf == MMSFB_PF_UYVY)
        return MMSFB_PF_UYVY_STR;
    if(pf == MMSFB_PF_I420)
        return MMSFB_PF_I420_STR;
    if(pf == MMSFB_PF_YV12)
        return MMSFB_PF_YV12_STR;
    if(pf == MMSFB_PF_AiRGB)
        return MMSFB_PF_AiRGB_STR;
    if(pf == MMSFB_PF_A1)
        return MMSFB_PF_A1_STR;
    if(pf == MMSFB_PF_NV12)
        return MMSFB_PF_NV12_STR;
    if(pf == MMSFB_PF_NV16)
        return MMSFB_PF_NV16_STR;
    if (pf == MMSFB_PF_NV21)
        return MMSFB_PF_NV21_STR;
    if (pf == MMSFB_PF_AYUV)
        return MMSFB_PF_AYUV_STR;
    if (pf == MMSFB_PF_A4)
        return MMSFB_PF_A4_STR;
    if (pf == MMSFB_PF_ARGB1666)
        return MMSFB_PF_ARGB1666_STR;
    if (pf == MMSFB_PF_ARGB6666)
        return MMSFB_PF_ARGB6666_STR;
    if (pf == MMSFB_PF_RGB18)
        return MMSFB_PF_RGB18_STR;
    if (pf == MMSFB_PF_LUT2)
        return MMSFB_PF_LUT2_STR;
    if (pf == MMSFB_PF_RGB444)
        return MMSFB_PF_RGB444_STR;
    if (pf == MMSFB_PF_RGB555)
        return MMSFB_PF_RGB555_STR;
    if(pf == MMSFB_PF_ARGB1555)
        return MMSFB_PF_ARGB1555_STR;
    if(pf == MMSFB_PF_RGB332)
        return MMSFB_PF_RGB332_STR;
    if(pf == MMSFB_PF_ALUT44)
        return MMSFB_PF_ALUT44_STR;
    if(pf == MMSFB_PF_LUT8)
        return MMSFB_PF_LUT8_STR;
    if(pf == MMSFB_PF_ARGB2554)
        return MMSFB_PF_ARGB2554_STR;
    if(pf == MMSFB_PF_ARGB4444)
        return MMSFB_PF_ARGB4444_STR;
    return MMSFB_PF_NONE_STR;
}

MMSFBSurfacePixelFormat getMMSFBPixelFormatFromString(string pf) {
    if(pf == MMSFB_PF_NONE_STR)
        return MMSFB_PF_NONE;
    if(pf == MMSFB_PF_RGB16_STR)
        return MMSFB_PF_RGB16;
    if(pf == MMSFB_PF_RGB24_STR)
        return MMSFB_PF_RGB24;
    if(pf == MMSFB_PF_RGB32_STR)
        return MMSFB_PF_RGB32;
    if(pf == MMSFB_PF_ARGB_STR)
        return MMSFB_PF_ARGB;
    if(pf == MMSFB_PF_A8_STR)
        return MMSFB_PF_A8;
    if(pf == MMSFB_PF_YUY2_STR)
        return MMSFB_PF_YUY2;
    if(pf == MMSFB_PF_UYVY_STR)
        return MMSFB_PF_UYVY;
    if(pf == MMSFB_PF_I420_STR)
        return MMSFB_PF_I420;
    if(pf == MMSFB_PF_YV12_STR)
        return MMSFB_PF_YV12;
    if(pf == MMSFB_PF_AiRGB_STR)
        return MMSFB_PF_AiRGB;
    if(pf == MMSFB_PF_A1_STR)
        return MMSFB_PF_A1;
    if(pf == MMSFB_PF_NV12_STR)
        return MMSFB_PF_NV12;
    if(pf == MMSFB_PF_NV16_STR)
        return MMSFB_PF_NV16;
    if(pf == MMSFB_PF_NV21_STR)
        return MMSFB_PF_NV21;
    if(pf == MMSFB_PF_AYUV_STR)
        return MMSFB_PF_AYUV;
    if(pf == MMSFB_PF_A4_STR)
        return MMSFB_PF_A4;
    if(pf == MMSFB_PF_ARGB1666_STR)
        return MMSFB_PF_ARGB1666;
    if(pf == MMSFB_PF_ARGB6666_STR)
        return MMSFB_PF_ARGB6666;
    if(pf == MMSFB_PF_RGB18_STR)
        return MMSFB_PF_RGB18;
    if(pf == MMSFB_PF_LUT2_STR)
        return MMSFB_PF_LUT2;
    if(pf == MMSFB_PF_RGB444_STR)
        return MMSFB_PF_RGB444;
    if(pf == MMSFB_PF_RGB555_STR)
        return MMSFB_PF_RGB555;
    if(pf == MMSFB_PF_ARGB1555_STR)
        return MMSFB_PF_ARGB1555;
    if(pf == MMSFB_PF_RGB332_STR)
        return MMSFB_PF_RGB332;
    if(pf == MMSFB_PF_ALUT44_STR)
        return MMSFB_PF_ALUT44;
    if(pf == MMSFB_PF_LUT8_STR)
        return MMSFB_PF_LUT8;
    if(pf == MMSFB_PF_ARGB2554_STR)
        return MMSFB_PF_ARGB2554;
    if(pf == MMSFB_PF_ARGB4444_STR)
        return MMSFB_PF_ARGB4444;
    return MMSFB_PF_NONE;
}


