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

#ifndef MMSFBBASE_H_
#define MMSFBBASE_H_

#include "mmstools/base.h"

#ifdef __HAVE_XLIB__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xvlib.h>
#endif

extern "C" {
#include <direct/debug.h>
#include <direct/trace.h>

#include <directfb.h>
#include <directfb_keynames.h>
#include <directfb_strings.h>
#include <directfb_util.h>
}

#ifdef MMSFB_TRACE_ENABLE
#define MMSFB_TRACE()    direct_trace_print_stack(NULL)
#else
#define MMSFB_TRACE()    do {} while (0)
#endif

#ifdef MMSFB_BREAK_ENABLE
#define MMSFB_BREAK()    D_BREAK("bad function")
#else
#define MMSFB_BREAK()    do {} while (0)
#endif

/* switch between window managers */
//#define USE_DFB_WINMAN
#define USE_MMSFB_WINMAN

/* access to the last error text */
extern string MMSFB_LastErrorString;

/* describes a color with alpha */
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} MMSFBColor;

/* supported pixel formats */
#define MMSFB_PF_NONE       ""          // none
#define MMSFB_PF_ARGB1555   "ARGB1555"  // 16 bit ARGB (2 byte, alpha 1@15, red 5@10, green 5@5, blue 5@0)
#define MMSFB_PF_RGB16      "RGB16"     // 16 bit RGB (2 byte, red 5@11, green 6@5, blue 5@0)
#define MMSFB_PF_RGB24      "RGB24"     // 24 bit RGB (3 byte, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_RGB32      "RGB32"     // 24 bit RGB (4 byte, nothing@24, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_ARGB       "ARGB"      // 32 bit ARGB (4 byte, alpha 8@24, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_A8         "A8"        // 8 bit alpha (1 byte, alpha 8@0), e.g. anti-aliased glyphs
#define MMSFB_PF_YUY2       "YUY2"      // 16 bit YUV (4 byte/ 2 pixel, macropixel contains CbYCrY [31:0])
#define MMSFB_PF_RGB332     "RGB332"    // 8 bit RGB (1 byte, red 3@5, green 3@2, blue 2@0)
#define MMSFB_PF_UYVY       "UYVY"      // 16 bit YUV (4 byte/ 2 pixel, macropixel contains YCbYCr [31:0])
#define MMSFB_PF_I420       "I420"      // 12 bit YUV (8 bit Y plane followed by 8 bit quarter size U/V planes)
#define MMSFB_PF_YV12       "YV12"      // 12 bit YUV (8 bit Y plane followed by 8 bit quarter size V/U planes)
#define MMSFB_PF_LUT8       "LUT8"      // 8 bit LUT (8 bit color and alpha lookup from palette)
#define MMSFB_PF_ALUT44     "ALUT44"    // 8 bit ALUT (1 byte, alpha 4@4, color lookup 4@0)
#define MMSFB_PF_AiRGB      "AiRGB"     // 32 bit ARGB (4 byte, inv. alpha 8@24, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_A1         "A1"        // 1 bit alpha (1 byte/ 8 pixel, most significant bit used first)
#define MMSFB_PF_NV12       "NV12"      // 12 bit YUV (8 bit Y plane followed by one 16 bit quarter size CbCr [15:0] plane)
#define MMSFB_PF_NV16       "NV16"      // 16 bit YUV (8 bit Y plane followed by one 16 bit half width CbCr [15:0] plane)
#define MMSFB_PF_ARGB2554   "ARGB2554"  // 16 bit ARGB (2 byte, alpha 2@14, red 5@9, green 5@4, blue 4@0)
#define MMSFB_PF_ARGB4444   "ARGB4444"  // 16 bit ARGB (2 byte, alpha 4@12, red 4@8, green 4@4, blue 4@0)
#define MMSFB_PF_NV21       "NV21"      // 12 bit YUV (8 bit Y plane followed by one 16 bit quarter size CrCb [15:0] plane)
#define MMSFB_PF_AYUV       "AYUV"      // 32 bit AYUV (4 byte, alpha 8@24, Y 8@16, Cb 8@8, Cr 8@0)

/* supported buffer modes */
#define MMSFB_BM_NONE       ""          // none
#define MMSFB_BM_FRONTONLY  "FRONTONLY" // no backbuffer
#define MMSFB_BM_BACKVIDEO  "BACKVIDEO" // backbuffer in video memory
#define MMSFB_BM_BACKSYSTEM "BACKSYSTEM"// backbuffer in system memory
#define MMSFB_BM_TRIPLE     "TRIPLE"    // triple buffering
#define MMSFB_BM_WINDOWS    "WINDOWS"   // no layer buffers at all, using buffer of each window


/* supported layer options */
#define MMSFB_LO_NONE               ""                  // none
#define MMSFB_LO_ALPHACHANNEL       "ALPHACHANNEL"      // Make usage of alpha channel for blending on a pixel per pixel basis.
#define MMSFB_LO_FLICKER_FILTERING  "FLICKER_FILTERING" // Enable flicker filtering.
#define MMSFB_LO_DEINTERLACING      "DEINTERLACING"     // Enable deinterlacing of an interlaced (video) source.
#define MMSFB_LO_SRC_COLORKEY       "SRC_COLORKEY"      // Enable source color key.
#define MMSFB_LO_DST_COLORKEY       "DST_COLORKEY"      // Enable dest. color key.
#define MMSFB_LO_OPACITY            "OPACITY"           // Make usage of the global alpha factor set by SetOpacity.
#define MMSFB_LO_FIELD_PARITY       "FIELD_PARITY"      // Set field parity


/* error logging routines */
string MMSFB_ErrorString(const int rc, const string msg);
void MMSFB_SetError(const int rc, const string msg);

/* conversion routines for pixel formats */
string getDFBPixelFormatString(DFBSurfacePixelFormat pf);
DFBSurfacePixelFormat getDFBPixelFormatFromString(string pf);
bool isAlphaPixelFormat(string pf);
bool isIndexedPixelFormat(string pf);
bool isRGBPixelFormat(string pf);

/* conversion routines for layer buffer modes */
string getDFBLayerBufferModeString(DFBDisplayLayerBufferMode bm);
DFBDisplayLayerBufferMode getDFBLayerBufferModeFromString(string bm);

/* conversion routines for layer options */
string getDFBLayerOptionsString(DFBDisplayLayerOptions opts);
DFBDisplayLayerOptions getDFBLayerOptionsFromString(string opts);

#endif /*MMSFBBASE_H_*/
