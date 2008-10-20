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

#ifndef MMSGUITOOLS_H_
#define MMSGUITOOLS_H_

#include "mmstools/mmslogger.h"
#include "mmsgui/fb/mmsfb.h"

typedef enum {
	MMSINPUTEVENTTYPE_NONE = 0,
	MMSINPUTEVENTTYPE_KEYPRESS,
	MMSINPUTEVENTTYPE_KEYRELEASE,
	MMSINPUTEVENTTYPE_BUTTONPRESS,
	MMSINPUTEVENTTYPE_BUTTONRELEASE,
	MMSINPUTEVENTTYPE_AXISMOTION
} MMSINPUTEVENTTYPE;

typedef struct {
	MMSINPUTEVENTTYPE		type;
	DFBInputDeviceKeySymbol key;
	int		    			posx;
	int						posy;
} MMSInputEvent;



//! Translate the size given in hint into pixel value.
/*!
\param retpix      return value
       hint        size with following formats
                     <size>px          - size in pixel, example "100px"
                     <size>%           - size in percent, example "100%"
                     <size>px-<size2>  - size in pixel minus pixel, example "100px-50"
                     <size>%-<size2>   - size in percent minus pixel, example "100%-50"
                     <factor>$         - factor which will used in conjunction with secondaxis
                                           result=<factor>*secondaxis
                                         example: "1.333$"
\param maxpixel    maximum pixel for calculation and check
\param secondaxis  size of the second axis, will only used with hint <factor>$
\return true if input is correct and retpix is filled
*/
bool getPixelFromSizeHint(int *retpix, string hint, int maxpixel, int secondaxis);

bool getColorFromString(string input, DFBColor *color);

bool getScreenInfo(int *w, int *h, IDirectFBDisplayLayer **layer = NULL, IDirectFB *dfb = NULL);

bool loadImage(IDirectFBImageProvider **image, string path, string filename);

bool loadFont(IDirectFBFont **font, string path, string filename, unsigned int size);

unsigned int getFrameNum(unsigned int delay_time);

unsigned int getFrameDelay(unsigned int start_ts, unsigned int end_ts);

#endif /*MMSGUITOOLS_H_*/
