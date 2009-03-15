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

#ifndef MMSTYPES_H_
#define MMSTYPES_H_

#include "mmstools/base.h"

// backend types.............................................................

//! supported backends
typedef enum {
	//! none
	MMSFB_BE_NONE = 0,
	//! directfb
	MMSFB_BE_DFB,
	//! X11 backend from disko framework
	MMSFB_BE_X11,
	//! FBDEV backend from disko framework
	MMSFB_BE_FBDEV
} MMSFBBackend;

//! backend: none
#define MMSFB_BE_NONE_STR		""
//! backend: DFB
#define MMSFB_BE_DFB_STR		"DFB"
//! backend: X11
#define MMSFB_BE_X11_STR		"X11"
//! backend: FBDEV
#define MMSFB_BE_FBDEV_STR		"FBDEV"

//! list of valid backend types
#define MMSFB_BE_VALID_VALUES	"DFB, X11, FBDEV"

//! list of valid backend types for output types MMSFB_OT_xxxFB
#define MMSFB_BE_VALID_VALUES_OT_FB		"DFB, FBDEV"

//! list of valid backend types for output types MMSFB_OT_X11
#define MMSFB_BE_VALID_VALUES_OT_X11	"DFB, X11"

//! list of valid backend types for output types MMSFB_OT_X
#define MMSFB_BE_VALID_VALUES_OT_X		"X11"

// conversion routines for backend types
string getMMSFBBackendString(MMSFBBackend be);
MMSFBBackend getMMSFBBackendFromString(string be);


// output types..............................................................

//! supported output types
typedef enum {
	//! none
	MMSFB_OT_NONE = 0,
	//! VESAFB (backend: DFB and FBDEV)
	MMSFB_OT_VESAFB,
	//! MATROXFB (backend: DFB and FBDEV)
	MMSFB_OT_MATROXFB,
	//! VIAFB (backend: DFB)
	MMSFB_OT_VIAFB,
	//! X11 (backend: DFB)
	MMSFB_OT_X11,
	//! XSHM (backend: X11)
	MMSFB_OT_XSHM,
	//! XVSHM (backend: X11)
	MMSFB_OT_XVSHM,
	//! DAVINCIFB (backend: DFB and FBDEV)
	MMSFB_OT_DAVINCIFB
} MMSFBOutputType;

//! output type: none
#define MMSFB_OT_NONE_STR		""
//! output type: VESAFB (backend: DFB and FBDEV)
#define MMSFB_OT_VESAFB_STR		"VESAFB"
//! output type: MATROXFB (backend: DFB and FBDEV)
#define MMSFB_OT_MATROXFB_STR	"MATROXFB"
//! output type: VIAFB (backend: DFB)
#define MMSFB_OT_VIAFB_STR		"VIAFB"
//! output type: X11 (backend: DFB)
#define MMSFB_OT_X11_STR		"X11"
//! output type: XSHM (backend: X11)
#define MMSFB_OT_XSHM_STR		"XSHM"
//! output type: XVSHM (backend: X11)
#define MMSFB_OT_XVSHM_STR		"XVSHM"
//! output type: DAVINCIFB (backend: DFB and FBDEV)
#define MMSFB_OT_DAVINCIFB_STR	"DAVINCIFB"

//! list of valid output types
#define MMSFB_OT_VALID_VALUES			"VESAFB, MATROXFB, VIAFB, X11, XSHM, XVSHM, DAVINCIFB"

//! list of valid output types for backend MMSFB_BE_DFB
#define MMSFB_OT_VALID_VALUES_BE_DFB	"VESAFB, MATROXFB, VIAFB, X11, DAVINCIFB"

//! list of valid output types for backend MMSFB_BE_X11
#define MMSFB_OT_VALID_VALUES_BE_X11	"X11, XSHM, XVSHM"

//! list of valid output types for backend MMSFB_BE_FBDEV
#define MMSFB_OT_VALID_VALUES_BE_FBDEV	"VESAFB, MATROXFB, DAVINCIFB"

// conversion routines for output types
string getMMSFBOutputTypeString(MMSFBOutputType ot);
MMSFBOutputType getMMSFBOutputTypeFromString(string ot);


// full screen modes.........................................................

//! supported full screen modes
typedef enum {
	//! none
	MMSFB_FSM_NONE = 0,
	//! disabled
	MMSFB_FSM_FALSE,
	//! enabled
	MMSFB_FSM_TRUE,
	//! enabled, using the correct aspect ratio
	MMSFB_FSM_ASPECT_RATIO
} MMSFBFullScreenMode;

//! full screen mode: none
#define MMSFB_FSM_NONE_STR			""
//! full screen mode: disabled
#define MMSFB_FSM_FALSE_STR			"FALSE"
//! full screen mode: enabled
#define MMSFB_FSM_TRUE_STR			"TRUE"
//! full screen mode: enabled, using the correct aspect ratio
#define MMSFB_FSM_ASPECT_RATIO_STR	"ASPECT_RATIO"

//! list of valid full screen modes
#define MMSFB_FSM_VALID_VALUES		"FALSE, TRUE, ASPECT_RATIO"

// conversion routines for full screen modes
string getMMSFBFullScreenModeString(MMSFBFullScreenMode fsm);
MMSFBFullScreenMode getMMSFBFullScreenModeFromString(string fsm);


// pixelformats..............................................................

//! supported pixel formats
typedef enum {
	//! none
	MMSFB_PF_NONE = 0,
    //! 16 bit RGB (2 byte, red 5\@11, green 6\@5, blue 5\@0)
	MMSFB_PF_RGB16,
    //! 24 bit RGB (3 byte, red 8\@16, green 8\@8, blue 8\@0)
    MMSFB_PF_RGB24,
    //! 24 bit RGB (4 byte, nothing\@24, red 8\@16, green 8\@8, blue 8\@0)
    MMSFB_PF_RGB32,
    //! 32 bit ARGB (4 byte, alpha 8\@24, red 8\@16, green 8\@8, blue 8\@0)
    MMSFB_PF_ARGB,
    //! 8 bit alpha (1 byte, alpha 8\@0), e.g. anti-aliased glyphs
    MMSFB_PF_A8,
    //! 16 bit YUV (4 byte/ 2 pixel, macropixel contains CbYCrY [31:0])
    MMSFB_PF_YUY2,
    //! 16 bit YUV (4 byte/ 2 pixel, macropixel contains YCbYCr [31:0])
    MMSFB_PF_UYVY,
    //! 12 bit YUV (8 bit Y plane followed by 8 bit quarter size U/V planes)
    MMSFB_PF_I420,
    //! 12 bit YUV (8 bit Y plane followed by 8 bit quarter size V/U planes)
    MMSFB_PF_YV12,
    //! 32 bit ARGB (4 byte, inv. alpha 8\@24, red 8\@16, green 8\@8, blue 8\@0)
    MMSFB_PF_AiRGB,
    //! 1 bit alpha (1 byte/ 8 pixel, most significant bit used first)
    MMSFB_PF_A1,
    //! 12 bit YUV (8 bit Y plane followed by one 16 bit quarter size CbCr [15:0] plane)
    MMSFB_PF_NV12,
    //! 16 bit YUV (8 bit Y plane followed by one 16 bit half width CbCr [15:0] plane)
    MMSFB_PF_NV16,
    //! 12 bit YUV (8 bit Y plane followed by one 16 bit quarter size CrCb [15:0] plane)
    MMSFB_PF_NV21,
    //! 32 bit AYUV (4 byte, alpha 8\@24, Y 8\@16, Cb 8\@8, Cr 8\@0)
    MMSFB_PF_AYUV,
    //! 4 bit alpha (1 byte/ 2 pixel, more significant nibble used first)
    MMSFB_PF_A4,
    //! 1 bit alpha (3 byte/  alpha 1\@18, red 6\@16, green 6\@6, blue 6\@0)
    MMSFB_PF_ARGB1666,
    //! 6 bit alpha (3 byte/  alpha 6\@18, red 6\@16, green 6\@6, blue 6\@0)
    MMSFB_PF_ARGB6666,
    //! 6 bit RGB (3 byte/   red 6\@16, green 6\@6, blue 6\@0)
    MMSFB_PF_RGB18,
    //! 2 bit LUT (1 byte/ 4 pixel, 2 bit color and alpha lookup from palette)
    MMSFB_PF_LUT2,
    //! 16 bit RGB (2 byte, nothing \@12, red 4\@8, green 4\@4, blue 4\@0)
    MMSFB_PF_RGB444,
    //! 16 bit RGB (2 byte, nothing \@15, red 5\@10, green 5\@5, blue 5\@0)
    MMSFB_PF_RGB555,
	//! 16 bit ARGB (2 byte, alpha 1\@15, red 5\@10, green 5\@5, blue 5\@0)
	MMSFB_PF_ARGB1555,
    //! 8 bit RGB (1 byte, red 3\@5, green 3\@2, blue 2\@0)
    MMSFB_PF_RGB332,
    //! 8 bit ALUT (1 byte, alpha 4\@4, color lookup 4\@0)
    MMSFB_PF_ALUT44,
    //! 8 bit LUT (8 bit color and alpha lookup from palette)
    MMSFB_PF_LUT8,
    //! 16 bit ARGB (2 byte, alpha 2\@14, red 5\@9, green 5\@4, blue 4\@0)
    MMSFB_PF_ARGB2554,
    //! 16 bit ARGB (2 byte, alpha 4\@12, red 4\@8, green 4\@4, blue 4\@0)
    MMSFB_PF_ARGB4444,
    //! 20 bit ARGB (16 bit RGB565 plane followed by 4 bit alpha plane (highest bit unused))
    MMSFB_PF_ARGB3565
} MMSFBSurfacePixelFormat;

//! pixel format: none
#define MMSFB_PF_NONE_STR       ""
//! pixel format: 16 bit RGB (2 byte, red 5@11, green 6@5, blue 5@0)
#define MMSFB_PF_RGB16_STR      "RGB16"
//! pixel format: 24 bit RGB (3 byte, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_RGB24_STR      "RGB24"
//! pixel format: 24 bit RGB (4 byte, nothing@24, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_RGB32_STR      "RGB32"
//! pixel format: 32 bit ARGB (4 byte, alpha 8@24, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_ARGB_STR       "ARGB"
//! pixel format: 8 bit alpha (1 byte, alpha 8@0), e.g. anti-aliased glyphs
#define MMSFB_PF_A8_STR         "A8"
//! pixel format: 16 bit YUV (4 byte/ 2 pixel, macropixel contains CbYCrY [31:0])
#define MMSFB_PF_YUY2_STR       "YUY2"
//! pixel format: 16 bit YUV (4 byte/ 2 pixel, macropixel contains YCbYCr [31:0])
#define MMSFB_PF_UYVY_STR       "UYVY"
//! pixel format: 12 bit YUV (8 bit Y plane followed by 8 bit quarter size U/V planes)
#define MMSFB_PF_I420_STR       "I420"
//! pixel format: 12 bit YUV (8 bit Y plane followed by 8 bit quarter size V/U planes)
#define MMSFB_PF_YV12_STR       "YV12"
//! pixel format: 32 bit ARGB (4 byte, inv. alpha 8@24, red 8@16, green 8@8, blue 8@0)
#define MMSFB_PF_AiRGB_STR      "AiRGB"
//! pixel format: 1 bit alpha (1 byte/ 8 pixel, most significant bit used first)
#define MMSFB_PF_A1_STR         "A1"
//! pixel format: 12 bit YUV (8 bit Y plane followed by one 16 bit quarter size CbCr [15:0] plane)
#define MMSFB_PF_NV12_STR       "NV12"
//! pixel format: 16 bit YUV (8 bit Y plane followed by one 16 bit half width CbCr [15:0] plane)
#define MMSFB_PF_NV16_STR       "NV16"
//! pixel format: 12 bit YUV (8 bit Y plane followed by one 16 bit quarter size CrCb [15:0] plane)
#define MMSFB_PF_NV21_STR       "NV21"
//! pixel format: 32 bit AYUV (4 byte, alpha 8@24, Y 8@16, Cb 8@8, Cr 8@0)
#define MMSFB_PF_AYUV_STR       "AYUV"
//! pixel format: 4 bit alpha (1 byte/ 2 pixel, more significant nibble used first)
#define	MMSFB_PF_A4_STR			"A4"
//! pixel format: 1 bit alpha (3 byte/  alpha 1@18, red 6@16, green 6@6, blue 6@0)
#define	MMSFB_PF_ARGB1666_STR	"ARGB1666"
//! pixel format: 6 bit alpha (3 byte/  alpha 6@18, red 6@16, green 6@6, blue 6@0)
#define	MMSFB_PF_ARGB6666_STR	"ARGB6666"
//! pixel format: 6 bit RGB (3 byte/   red 6@16, green 6@6, blue 6@0)
#define	MMSFB_PF_RGB18_STR		"RGB18"
//! pixel format: 2 bit LUT (1 byte/ 4 pixel, 2 bit color and alpha lookup from palette)
#define	MMSFB_PF_LUT2_STR		"LUT2"
//! pixel format: 16 bit RGB (2 byte, nothing @12, red 4@8, green 4@4, blue 4@0)
#define	MMSFB_PF_RGB444_STR		"RGB444"
//! pixel format: 16 bit RGB (2 byte, nothing @15, red 5@10, green 5@5, blue 5@0)
#define	MMSFB_PF_RGB555_STR		"RGB555"
//! pixel format: 16 bit ARGB (2 byte, alpha 1@15, red 5@10, green 5@5, blue 5@0)
#define MMSFB_PF_ARGB1555_STR   "ARGB1555"
//! pixel format: 8 bit RGB (1 byte, red 3@5, green 3@2, blue 2@0)
#define MMSFB_PF_RGB332_STR     "RGB332"
//! pixel format: 8 bit ALUT (1 byte, alpha 4@4, color lookup 4@0)
#define MMSFB_PF_ALUT44_STR     "ALUT44"
//! pixel format: 8 bit LUT (8 bit color and alpha lookup from palette)
#define MMSFB_PF_LUT8_STR       "LUT8"
//! pixel format: 16 bit ARGB (2 byte, alpha 2@14, red 5@9, green 5@4, blue 4@0)
#define MMSFB_PF_ARGB2554_STR   "ARGB2554"
//! pixel format: 16 bit ARGB (2 byte, alpha 4@12, red 4@8, green 4@4, blue 4@0)
#define MMSFB_PF_ARGB4444_STR   "ARGB4444"
//! pixel format: 20 bit ARGB (16 bit RGB565 plane followed by 4 bit alpha plane (highest bit unused))
#define MMSFB_PF_ARGB3565_STR	"ARGB3565"

//! list of valid pixelformats
#define MMSFB_PF_VALID_VALUES	"RGB16, RGB24, RGB32, ARGB, A8, YUY2, UYVY, I420, YV12, AiRGB, A1, NV12, NV16, NV21, AYUV, A4, ARGB1666, ARGB6666, RGB18, LUT2, RGB444, RGB555, ARGB1555, RGB332, ALUT44, LUT8, ARGB2554, ARGB4444, ARGB3565"

//! list of valid pixelformats used for layer surfaces
#define MMSFB_PF_VALID_VALUES_LAYER	"RGB16, RGB24, RGB32, ARGB, YUY2, UYVY, I420, YV12, AiRGB, NV12, NV16, NV21, AYUV, ARGB1666, ARGB6666, RGB18, LUT2, RGB444, RGB555, ARGB1555, RGB332, LUT8, ARGB2554, ARGB4444, ARGB3565"

//! list of valid pixelformats used for windows surfaces
#define MMSFB_PF_VALID_VALUES_WINDOWS	"ARGB, AiRGB, AYUV"

//! list of valid pixelformats used for worker surfaces
#define MMSFB_PF_VALID_VALUES_SURFACES	"ARGB, AiRGB, AYUV"

//! list of valid pixelformats for XVSHM
#define MMSFB_PF_VALID_VALUES_BE_X11_OT_XVSHM	"YV12"

//! list of valid pixelformats for XSHM
#define MMSFB_PF_VALID_VALUES_BE_X11_OT_XSHM	"RGB32"

//! list of valid pixelformats for DAVINCIFB, OSD Layer
#define MMSFB_PF_VALID_VALUES_BE_FBDEV_OT_DAVINCIFB_LAYER_0	"ARGB3565"

// conversion routines for pixel formats
string getMMSFBPixelFormatString(MMSFBSurfacePixelFormat pf);
MMSFBSurfacePixelFormat getMMSFBPixelFormatFromString(string pf);



// pointer mode..............................................................

//! supported pointer modes
typedef enum {
	//! none
	MMSFB_PM_NONE = 0,
	//! disabled
	MMSFB_PM_FALSE,
	//! enabled, using internal mouse pointer
	MMSFB_PM_TRUE,
	//! enabled, using external mouse pointer e.g. from X11, if using the X11 backend
	MMSFB_PM_EXTERNAL
} MMSFBPointerMode;

//! pointer mode: none
#define MMSFB_PM_NONE_STR			""
//! pointer mode: disabled
#define MMSFB_PM_FALSE_STR			"FALSE"
//! pointer mode: enabled, using internal mouse pointer
#define MMSFB_PM_TRUE_STR			"TRUE"
//! pointer mode: enabled, using external mouse pointer e.g. from X11, if using the X11 backend
#define MMSFB_PM_EXTERNAL_STR		"EXTERNAL"

//! list of valid pointer modes
#define MMSFB_PM_VALID_VALUES		"FALSE, TRUE, EXTERNAL"

// conversion routines for pointer modes
string getMMSFBPointerModeString(MMSFBPointerMode pm);
MMSFBPointerMode getMMSFBPointerModeFromString(string pm);


#endif /* MMSTYPES_H_ */
