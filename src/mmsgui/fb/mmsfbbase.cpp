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

#include "mmsgui/fb/mmsfbbase.h"
#include "mmstools/mmstools.h"
#include <string.h>

/* stores the last error text */
string MMSFB_LastErrorString;

string MMSFB_ErrorString(const int rc, const string msg) {
    if (rc)
    {
        string s1 = msg;
        string s2 = DirectFBErrorString((DFBResult)rc);
        return s1 + " [" + s2 + "]";
    }
    else
        return msg;
}

void MMSFB_SetError(const int rc, const string msg) {
    MMSFB_LastErrorString = MMSFB_ErrorString(rc, msg);
    DEBUGMSG("MMSGUI", MMSFB_LastErrorString);
}

string getDFBPixelFormatString(DFBSurfacePixelFormat pf) {
    if(pf == DSPF_ARGB1555)
        return MMSFB_PF_ARGB1555; 
    if(pf == DSPF_RGB16)
        return MMSFB_PF_RGB16;
    if(pf == DSPF_RGB24)
        return MMSFB_PF_RGB24;
    if(pf == DSPF_RGB32)
        return MMSFB_PF_RGB32;
    if(pf == DSPF_ARGB)
        return MMSFB_PF_ARGB;
    if(pf == DSPF_A8)
        return MMSFB_PF_A8;
    if(pf == DSPF_YUY2)
        return MMSFB_PF_YUY2;
    if(pf == DSPF_RGB332)
        return MMSFB_PF_RGB332; 
    if(pf == DSPF_UYVY)
        return MMSFB_PF_UYVY;
    if(pf == DSPF_I420)
        return MMSFB_PF_I420;
    if(pf == DSPF_YV12)
        return MMSFB_PF_YV12;
    if(pf == DSPF_LUT8)
        return MMSFB_PF_LUT8;
    if(pf == DSPF_ALUT44)
        return MMSFB_PF_ALUT44;
    if(pf == DSPF_AiRGB)
        return MMSFB_PF_AiRGB;
    if(pf == DSPF_A1)
        return MMSFB_PF_A1;
    if(pf == DSPF_NV12)
        return MMSFB_PF_NV12;
    if(pf == DSPF_NV16)
        return MMSFB_PF_NV16;
    if(pf == DSPF_ARGB2554)
        return MMSFB_PF_ARGB2554;
    if(pf == DSPF_ARGB4444)
        return MMSFB_PF_ARGB4444;
    if (pf == DSPF_NV21)
        return MMSFB_PF_NV21;
    if (pf == DSPF_AYUV)
        return MMSFB_PF_AYUV;
    return MMSFB_PF_NONE;
}

DFBSurfacePixelFormat getDFBPixelFormatFromString(string pf) {
    if(pf == MMSFB_PF_NONE)
        return DSPF_UNKNOWN;
    if(pf == MMSFB_PF_ARGB1555)
        return DSPF_ARGB1555;
    if(pf == MMSFB_PF_RGB16)
        return DSPF_RGB16; 
    if(pf == MMSFB_PF_RGB24)
        return DSPF_RGB24;
    if(pf == MMSFB_PF_RGB32)
        return DSPF_RGB32;
    if(pf == MMSFB_PF_ARGB)
        return DSPF_ARGB; 
    if(pf == MMSFB_PF_A8)
        return DSPF_A8;
    if(pf == MMSFB_PF_YUY2)
        return DSPF_YUY2;
    if(pf == MMSFB_PF_RGB332)
        return DSPF_RGB332;
    if(pf == MMSFB_PF_UYVY)
        return DSPF_UYVY;
    if(pf == MMSFB_PF_I420)
        return DSPF_I420;
    if(pf == MMSFB_PF_YV12)
        return DSPF_YV12;
    if(pf == MMSFB_PF_LUT8)
        return DSPF_LUT8;
    if(pf == MMSFB_PF_ALUT44)
        return DSPF_ALUT44;
    if(pf == MMSFB_PF_AiRGB)
        return DSPF_AiRGB;
    if(pf == MMSFB_PF_A1)
        return DSPF_A1;
    if(pf == MMSFB_PF_NV12)
        return DSPF_NV12;
    if(pf == MMSFB_PF_NV16)
        return DSPF_NV16;
    if(pf == MMSFB_PF_ARGB2554)
        return DSPF_ARGB2554;
    if(pf == MMSFB_PF_ARGB4444)
        return DSPF_ARGB4444;
    if(pf == MMSFB_PF_NV21)
        return DSPF_NV21;
    if(pf == MMSFB_PF_AYUV)
        return DSPF_AYUV;
    return DSPF_UNKNOWN;
}

bool isAlphaPixelFormat(string pf) {
    if   ((pf == MMSFB_PF_RGB16)
        ||(pf == MMSFB_PF_RGB24)
        ||(pf == MMSFB_PF_RGB32)
		||(pf == MMSFB_PF_YV12)
		||(pf == MMSFB_PF_YUY2)
		||(pf == MMSFB_PF_UYVY)
		||(pf == MMSFB_PF_LUT8)
		||(pf == MMSFB_PF_NV12)
		||(pf == MMSFB_PF_NV16)
		||(pf == MMSFB_PF_NV21)
        ||(pf == MMSFB_PF_I420))
        return false;
    return true;
}

bool isIndexedPixelFormat(string pf) {
    if (pf == MMSFB_PF_ALUT44)
        return true;
    return false;
}

bool isRGBPixelFormat(string pf) {
    if   ((pf == MMSFB_PF_YUY2)
        ||(pf == MMSFB_PF_UYVY)
        ||(pf == MMSFB_PF_I420)
		||(pf == MMSFB_PF_YV12)
		||(pf == MMSFB_PF_NV12)
		||(pf == MMSFB_PF_NV16)
		||(pf == MMSFB_PF_NV21)
        ||(pf == MMSFB_PF_AYUV))
        return false;
    return true;
}

string getDFBLayerBufferModeString(DFBDisplayLayerBufferMode bm) {
    string ret = MMSFB_BM_NONE;

    if(bm & DLBM_FRONTONLY)
        ret = ret + "|" + MMSFB_BM_FRONTONLY;

    if(bm & DLBM_BACKVIDEO)
        ret = ret + "|" + MMSFB_BM_BACKVIDEO;
    if(bm & DLBM_BACKSYSTEM)
        ret = ret + "|" + MMSFB_BM_BACKSYSTEM;
    if(bm & DLBM_TRIPLE)
        ret = ret + "|" + MMSFB_BM_TRIPLE;
    if(bm & DLBM_WINDOWS)
        ret = ret + "|" + MMSFB_BM_WINDOWS;

    if (ret!=MMSFB_BM_NONE)
        return ret.substr(1);
    else
        return MMSFB_BM_NONE;
}

DFBDisplayLayerBufferMode getDFBLayerBufferModeFromString(string bm) {
    DFBDisplayLayerBufferMode b = DLBM_UNKNOWN;

    if (bm == MMSFB_BM_NONE)
        return b;
    if(strstr(bm.c_str(),MMSFB_BM_FRONTONLY))
        b = (DFBDisplayLayerBufferMode)(b | DLBM_FRONTONLY);

    if(strstr(bm.c_str(),MMSFB_BM_BACKVIDEO))
        b = (DFBDisplayLayerBufferMode)(b | DLBM_BACKVIDEO);
    if(strstr(bm.c_str(),MMSFB_BM_BACKSYSTEM))
        b = (DFBDisplayLayerBufferMode)(b | DLBM_BACKSYSTEM);
    if(strstr(bm.c_str(),MMSFB_BM_TRIPLE))
        b = (DFBDisplayLayerBufferMode)(b | DLBM_TRIPLE);
    if(strstr(bm.c_str(),MMSFB_BM_WINDOWS))
        b = (DFBDisplayLayerBufferMode)(b | DLBM_WINDOWS);
    return b;
}

string getDFBLayerOptionsString(DFBDisplayLayerOptions opts) {
    string ret = MMSFB_LO_NONE;

    if(opts & DLOP_ALPHACHANNEL)
        ret = ret + "|" + MMSFB_LO_ALPHACHANNEL;
    if(opts & DLOP_FLICKER_FILTERING)
        ret = ret + "|" + MMSFB_LO_FLICKER_FILTERING;
    if(opts & DLOP_DEINTERLACING)
        ret = ret + "|" + MMSFB_LO_DEINTERLACING;
    if(opts & DLOP_SRC_COLORKEY)   
        ret = ret + "|" + MMSFB_LO_SRC_COLORKEY;
    if(opts & DLOP_DST_COLORKEY)
        ret = ret + "|" + MMSFB_LO_DST_COLORKEY;
    if(opts & DLOP_OPACITY)
        ret = ret + "|" + MMSFB_LO_OPACITY;
    if(opts & DLOP_FIELD_PARITY)
        ret = ret + "|" + MMSFB_LO_FIELD_PARITY;
    
    if (ret!=MMSFB_LO_NONE)
        return ret.substr(1);
    else
        return MMSFB_LO_NONE;
}  

DFBDisplayLayerOptions getDFBLayerOptionsFromString(string opts) {
    DFBDisplayLayerOptions  o = DLOP_NONE;

    if (opts == MMSFB_LO_NONE)
        return o;
    if(strstr(opts.c_str(),MMSFB_LO_ALPHACHANNEL))
        o = (DFBDisplayLayerOptions)(o | DLOP_ALPHACHANNEL);
    if(strstr(opts.c_str(),MMSFB_LO_FLICKER_FILTERING))
        o = (DFBDisplayLayerOptions)(o | DLOP_FLICKER_FILTERING);
    if(strstr(opts.c_str(),MMSFB_LO_DEINTERLACING))
        o = (DFBDisplayLayerOptions)(o | DLOP_DEINTERLACING);
    if(strstr(opts.c_str(),MMSFB_LO_SRC_COLORKEY))
        o = (DFBDisplayLayerOptions)(o | DLOP_SRC_COLORKEY);
    if(strstr(opts.c_str(),MMSFB_LO_DST_COLORKEY))
        o = (DFBDisplayLayerOptions)(o | DLOP_DST_COLORKEY);
    if(strstr(opts.c_str(),MMSFB_LO_OPACITY))
        o = (DFBDisplayLayerOptions)(o | DLOP_OPACITY);
    if(strstr(opts.c_str(),MMSFB_LO_FIELD_PARITY))
        o = (DFBDisplayLayerOptions)(o | DLOP_FIELD_PARITY);
    return o;
}


