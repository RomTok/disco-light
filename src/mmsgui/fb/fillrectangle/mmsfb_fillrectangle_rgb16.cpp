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

#include "mmsgui/fb/mmsfbconv.h"
#include "mmstools/mmstools.h"

void mmsfb_fillrectangle_rgb16(unsigned short int *dst, int dst_pitch, int dst_height,
						       int dx, int dy, int dw, int dh, MMSFBColor color) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated fill rectangle to RGB16.\n");
		firsttime = false;
	}

	// prepare...
	int dst_pitch_pix = dst_pitch >> 1;
	dst+= dx + dy * dst_pitch_pix;

	unsigned short int *dst_end = dst + dst_pitch_pix * dh;
	int dst_pitch_diff = dst_pitch_pix - dw;

	// prepare the color
	register unsigned short int SRC;
	unsigned int r = color.r >> 3;
	unsigned int g = color.g >> 2;
	unsigned int b = color.b >> 3;
	SRC =     (r << 11)
			| (g << 5)
			| b;

	// copy pixel directly to the destination
	// for all lines
	while (dst < dst_end) {
		// for all pixels in the line
		unsigned short int *line_end = dst + dw;
		while (dst < line_end) {
			*dst = SRC;
			dst++;
		}

		// go to the next line
		dst+= dst_pitch_diff;
	}
}

