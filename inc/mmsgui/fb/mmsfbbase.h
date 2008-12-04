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
#include "mmstools/mmsmutex.h"
#include "mmstools/mmstafffile.h"

#ifdef __HAVE_XLIB__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/xf86vmode.h>
#endif

#ifdef  __HAVE_DIRECTFB__
extern "C" {
#include <direct/debug.h>
#include <direct/trace.h>

#include <directfb.h>
#include <directfb_keynames.h>
#include <directfb_strings.h>
#include <directfb_util.h>
}
#endif

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

// output types
#define MMS_OT_VESAFB       "vesafb"
#define MMS_OT_MATROXFB     "matroxfb"
#define MMS_OT_VIAFB        "viafb"
#define MMS_OT_X11FB        "x11"
#define MMS_OT_SDLFB        "sdl"

typedef enum {
	MMSFB_BACKEND_DFB = 0,
	MMSFB_BACKEND_X11
} MMSFB_BACKEND;


/* switch between window managers */
//#define USE_DFB_WINMAN
#define USE_MMSFB_WINMAN

/* access to the last error text */
extern string MMSFB_LastErrorString;

//! describes a color with alpha
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} MMSFBColor;

//! describes a rectangle
typedef struct {
	int	x;
	int	y;
	int	w;
	int	h;
} MMSFBRectangle;

//! describes a region
typedef struct {
	int	x1;
	int	y1;
	int	x2;
	int	y2;
} MMSFBRegion;


//! surface blitting flags
typedef unsigned int MMSFBBlittingFlags;

//! surface blitting flag: no effects
#define MMSFB_BLIT_NOFX					0x00000000
//! surface blitting flag: use alphachannel from source
#define MMSFB_BLIT_BLEND_ALPHACHANNEL	0x00000001
//! surface blitting flag: use alphachannel from color
#define MMSFB_BLIT_BLEND_COLORALPHA		0x00000002
//! surface blitting flag: modulates source pixel with color
#define MMSFB_BLIT_COLORIZE				0x00000004
//! surface blitting flag: modulates the source color with the source alpha
#define MMSFB_BLIT_SRC_PREMULTIPLY		0x00000008
//! surface blitting flag: modulates the source color with the color alpha
#define MMSFB_BLIT_SRC_PREMULTCOLOR		0x00000010


//! surface drawing flags
typedef unsigned int MMSFBDrawingFlags;

//! surface drawing flag: no effects
#define MMSFB_DRAW_NOFX					0x00000000
//! surface drawing flag: alpha from surface color
#define MMSFB_DRAW_BLEND				0x00000001
//! surface drawing flag: multiplies the color with the alpha channel
#define MMSFB_DRAW_SRC_PREMULTIPLY		0x00000002


//! surface flip flags
typedef unsigned int MMSFBFlipFlags;

//! surface flip flag: none
#define MMSFB_FLIP_NONE					0x00000000
//! surface flip flag: returns upon vertical sync
#define MMSFB_FLIP_WAIT					0x00000001
//! surface flip flag: flip upon the next vertical sync
#define MMSFB_FLIP_ONSYNC				0x00000002
//! surface flip flag: WAIT and ONSYNC
#define MMSFB_FLIP_WAITFORSYNC			0x00000004


//! surface lock flags
typedef unsigned int MMSFBLockFlags;

//! surface lock flag: none
#define MMSFB_LOCK_NONE					0x00000000
//! surface lock flag: read access
#define MMSFB_LOCK_READ					0x00000001
//! surface lock flag: write access
#define MMSFB_LOCK_WRITE				0x00000002


// supported pixel format strings
#define MMSFB_PF_NONE_STR       ""
// 16 bit RGB (2 byte, red 5@11, green 6@5, blue 5@0)
#define MMSFB_PF_RGB16_STR      "RGB16"
// 24 bit RGB (3 byte, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_RGB24_STR      "RGB24"
// 24 bit RGB (4 byte, nothing@24, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_RGB32_STR      "RGB32"
// 32 bit ARGB (4 byte, alpha 8@24, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_ARGB_STR       "ARGB"
// 8 bit alpha (1 byte, alpha 8@0), e.g. anti-aliased glyphs
#define MMSFB_PF_A8_STR         "A8"
// 16 bit YUV (4 byte/ 2 pixel, macropixel contains CbYCrY [31:0])
#define MMSFB_PF_YUY2_STR       "YUY2"
// 16 bit YUV (4 byte/ 2 pixel, macropixel contains YCbYCr [31:0])
#define MMSFB_PF_UYVY_STR       "UYVY"
// 12 bit YUV (8 bit Y plane followed by 8 bit quarter size U/V planes)
#define MMSFB_PF_I420_STR       "I420"
// 12 bit YUV (8 bit Y plane followed by 8 bit quarter size V/U planes)
#define MMSFB_PF_YV12_STR       "YV12"
// 32 bit ARGB (4 byte, inv. alpha 8@24, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_AiRGB_STR      "AiRGB"
// 1 bit alpha (1 byte/ 8 pixel, most significant bit used first)
#define MMSFB_PF_A1_STR         "A1"
// 12 bit YUV (8 bit Y plane followed by one 16 bit quarter size CbCr [15:0] plane)
#define MMSFB_PF_NV12_STR       "NV12"
// 16 bit YUV (8 bit Y plane followed by one 16 bit half width CbCr [15:0] plane)
#define MMSFB_PF_NV16_STR       "NV16"
// 12 bit YUV (8 bit Y plane followed by one 16 bit quarter size CrCb [15:0] plane)
#define MMSFB_PF_NV21_STR       "NV21"
// 32 bit AYUV (4 byte, alpha 8@24, Y 8@16, Cb 8@8, Cr 8@0)
#define MMSFB_PF_AYUV_STR       "AYUV"
// 4 bit alpha (1 byte/ 2 pixel, more significant nibble used first)
#define	MMSFB_PF_A4_STR			"A4"
// 1 bit alpha (3 byte/  alpha 1@18, red 6@16, green 6@6, blue 6@0)
#define	MMSFB_PF_ARGB1666_STR	"ARGB1666"
// 6 bit alpha (3 byte/  alpha 6@18, red 6@16, green 6@6, blue 6@0)
#define	MMSFB_PF_ARGB6666_STR	"ARGB6666"
// 6 bit RGB (3 byte/   red 6@16, green 6@6, blue 6@0)
#define	MMSFB_PF_RGB18_STR		"RGB18"
// 2 bit LUT (1 byte/ 4 pixel, 2 bit color and alpha lookup from palette)
#define	MMSFB_PF_LUT2_STR		"LUT2"
// 16 bit RGB (2 byte, nothing @12, red 4@8, green 4@4, blue 4@0)
#define	MMSFB_PF_RGB444_STR		"RGB444"
// 16 bit RGB (2 byte, nothing @15, red 5@10, green 5@5, blue 5@0)
#define	MMSFB_PF_RGB555_STR		"RGB555"
// 16 bit ARGB (2 byte, alpha 1@15, red 5@10, green 5@5, blue 5@0)
#define MMSFB_PF_ARGB1555_STR   "ARGB1555"
// 8 bit RGB (1 byte, red 3@5, green 3@2, blue 2@0)
#define MMSFB_PF_RGB332_STR     "RGB332"
// 8 bit ALUT (1 byte, alpha 4@4, color lookup 4@0)
#define MMSFB_PF_ALUT44_STR     "ALUT44"
// 8 bit LUT (8 bit color and alpha lookup from palette)
#define MMSFB_PF_LUT8_STR       "LUT8"
// 16 bit ARGB (2 byte, alpha 2@14, red 5@9, green 5@4, blue 4@0)
#define MMSFB_PF_ARGB2554_STR   "ARGB2554"
// 16 bit ARGB (2 byte, alpha 4@12, red 4@8, green 4@4, blue 4@0)
#define MMSFB_PF_ARGB4444_STR   "ARGB4444"


// supported pixel formats
typedef enum {
	// none
	MMSFB_PF_NONE = 0,
    // 16 bit RGB (2 byte, red 5@11, green 6@5, blue 5@0)
	MMSFB_PF_RGB16,
    // 24 bit RGB (3 byte, red 8@16, green 8@8, blue 8@0)
    MMSFB_PF_RGB24,
    // 24 bit RGB (4 byte, nothing@24, red 8@16, green 8@8, blue 8@0)
    MMSFB_PF_RGB32,
    // 32 bit ARGB (4 byte, alpha 8@24, red 8@16, green 8@8, blue 8@0)
    MMSFB_PF_ARGB,
    // 8 bit alpha (1 byte, alpha 8@0), e.g. anti-aliased glyphs
    MMSFB_PF_A8,
    // 16 bit YUV (4 byte/ 2 pixel, macropixel contains CbYCrY [31:0])
    MMSFB_PF_YUY2,
    // 16 bit YUV (4 byte/ 2 pixel, macropixel contains YCbYCr [31:0])
    MMSFB_PF_UYVY,
    // 12 bit YUV (8 bit Y plane followed by 8 bit quarter size U/V planes)
    MMSFB_PF_I420,
    // 12 bit YUV (8 bit Y plane followed by 8 bit quarter size V/U planes)
    MMSFB_PF_YV12,
    // 32 bit ARGB (4 byte, inv. alpha 8@24, red 8@16, green 8@8, blue 8@0)
    MMSFB_PF_AiRGB,
    // 1 bit alpha (1 byte/ 8 pixel, most significant bit used first)
    MMSFB_PF_A1,
    // 12 bit YUV (8 bit Y plane followed by one 16 bit quarter size CbCr [15:0] plane)
    MMSFB_PF_NV12,
    // 16 bit YUV (8 bit Y plane followed by one 16 bit half width CbCr [15:0] plane)
    MMSFB_PF_NV16,
    // 12 bit YUV (8 bit Y plane followed by one 16 bit quarter size CrCb [15:0] plane)
    MMSFB_PF_NV21,
    // 32 bit AYUV (4 byte, alpha 8@24, Y 8@16, Cb 8@8, Cr 8@0)
    MMSFB_PF_AYUV,
    // 4 bit alpha (1 byte/ 2 pixel, more significant nibble used first)
    MMSFB_PF_A4,
    // 1 bit alpha (3 byte/  alpha 1@18, red 6@16, green 6@6, blue 6@0)
    MMSFB_PF_ARGB1666,
    // 6 bit alpha (3 byte/  alpha 6@18, red 6@16, green 6@6, blue 6@0)
    MMSFB_PF_ARGB6666,
    // 6 bit RGB (3 byte/   red 6@16, green 6@6, blue 6@0)
    MMSFB_PF_RGB18,
    // 2 bit LUT (1 byte/ 4 pixel, 2 bit color and alpha lookup from palette)
    MMSFB_PF_LUT2,
    // 16 bit RGB (2 byte, nothing @12, red 4@8, green 4@4, blue 4@0)
    MMSFB_PF_RGB444,
    // 16 bit RGB (2 byte, nothing @15, red 5@10, green 5@5, blue 5@0)
    MMSFB_PF_RGB555,
	// 16 bit ARGB (2 byte, alpha 1@15, red 5@10, green 5@5, blue 5@0)
	MMSFB_PF_ARGB1555,
    // 8 bit RGB (1 byte, red 3@5, green 3@2, blue 2@0)
    MMSFB_PF_RGB332,
    // 8 bit ALUT (1 byte, alpha 4@4, color lookup 4@0)
    MMSFB_PF_ALUT44,
    // 8 bit LUT (8 bit color and alpha lookup from palette)
    MMSFB_PF_LUT8,
    // 16 bit ARGB (2 byte, alpha 2@14, red 5@9, green 5@4, blue 4@0)
    MMSFB_PF_ARGB2554,
    // 16 bit ARGB (2 byte, alpha 4@12, red 4@8, green 4@4, blue 4@0)
    MMSFB_PF_ARGB4444
} MMSFBSurfacePixelFormat;


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

// conversion routines for pixel formats
string getMMSFBPixelFormatString(MMSFBSurfacePixelFormat pf);
MMSFBSurfacePixelFormat getMMSFBPixelFormatFromString(string pf);
bool isAlphaPixelFormat(MMSFBSurfacePixelFormat pf);
bool isIndexedPixelFormat(MMSFBSurfacePixelFormat pf);
bool isRGBPixelFormat(MMSFBSurfacePixelFormat pf);

#ifdef  __HAVE_DIRECTFB__
// dfb specific routines

// conversion routines for pixel formats
MMSFBSurfacePixelFormat getMMSFBPixelFormatFromDFBPixelFormat(DFBSurfacePixelFormat pf);
DFBSurfacePixelFormat getDFBPixelFormatFromMMSFBPixelFormat(MMSFBSurfacePixelFormat pf);

// conversion routines for layer buffer modes
string getDFBLayerBufferModeString(DFBDisplayLayerBufferMode bm);
DFBDisplayLayerBufferMode getDFBLayerBufferModeFromString(string bm);

// conversion routines for layer options
string getDFBLayerOptionsString(DFBDisplayLayerOptions opts);
DFBDisplayLayerOptions getDFBLayerOptionsFromString(string opts);


DFBSurfaceBlittingFlags getDFBSurfaceBlittingFlagsFromMMSFBBlittingFlags(MMSFBBlittingFlags flags);
DFBSurfaceDrawingFlags getDFBSurfaceDrawingFlagsFromMMSFBDrawingFlags(MMSFBDrawingFlags flags);
DFBSurfaceFlipFlags getDFBSurfaceFlipFlagsFromMMSFBFlipFlags(MMSFBFlipFlags flags);
DFBSurfaceLockFlags getDFBSurfaceLockFlagsFromMMSFBLockFlags(MMSFBLockFlags flags);
#endif

#endif /*MMSFBBASE_H_*/
