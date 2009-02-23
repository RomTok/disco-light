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

//! list of valid output types
#define MMSFB_OT_VALID_VALUES			"VESAFB, MATROXFB, VIAFB, X11, XSHM, XVSHM"

//! list of valid output types for backend MMSFB_BE_DFB
#define MMSFB_OT_VALID_VALUES_BE_DFB	"VESAFB, MATROXFB, VIAFB, X11"

//! list of valid output types for backend MMSFB_BE_X11
#define MMSFB_OT_VALID_VALUES_BE_X11	"X11, XSHM, XVSHM"

//! list of valid output types for backend MMSFB_BE_FBDEV
#define MMSFB_OT_VALID_VALUES_BE_FBDEV	"VESAFB, MATROXFB"

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


#endif /* MMSTYPES_H_ */
