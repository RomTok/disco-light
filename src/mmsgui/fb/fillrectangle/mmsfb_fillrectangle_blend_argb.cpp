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

void mmsfb_fillrectangle_blend_argb(unsigned int *dst, int dst_pitch, int dst_height,
									int dx, int dy, int dw, int dh, MMSFBColor color) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated blend rectangle to ARGB.\n");
		firsttime = false;
	}

	// return immediately if alpha channel of the color is 0x00
	if (!color.a)
		return;

	// prepare...
	int dst_pitch_pix = dst_pitch >> 2;
	dst+= dx + dy * dst_pitch_pix;

	unsigned int OLDDST = (*dst) + 1;
	unsigned int *dst_end = dst + dst_pitch_pix * dh;
	int dst_pitch_diff = dst_pitch_pix - dw;
	register unsigned int d;

	// prepare the color
	register unsigned int A = color.a;
	register unsigned int SRC;
	SRC =     (A << 24)
			| (color.r << 16)
			| (color.g << 8)
			| color.b;

	if (color.a == 0xff) {
		// source pixel is not transparent, copy it directly to the destination
		// for all lines
		while (dst < dst_end) {
			// for all pixels in the line
			unsigned int *line_end = dst + dw;
			while (dst < line_end) {
				*dst = SRC;
			    dst++;
			}

			// go to the next line
			dst+= dst_pitch_diff;
		}

	}
	else {
		// source alpha is > 0x00 and < 0xff
		// for all lines
		while (dst < dst_end) {
			// for all pixels in the line
			unsigned int *line_end = dst + dw;
			while (dst < line_end) {
				// read the destination
				register unsigned int DST = *dst;
				if (DST==OLDDST) {
					// same pixel, use the previous value
					*dst = d;
				    dst++;
					continue;
				}
				OLDDST = DST;

				register int SA= 0x100 - A;
				unsigned int a = DST >> 24;
				unsigned int r = (DST << 8) >> 24;
				unsigned int g = (DST << 16) >> 24;
				unsigned int b = DST & 0xff;

				// invert src alpha
			    a = (SA * a) >> 8;
			    r = (SA * r) >> 8;
			    g = (SA * g) >> 8;
			    b = (SA * b) >> 8;

			    // add src to dst
			    a += A;
			    r += (SRC << 8) >> 24;
			    g += (SRC << 16) >> 24;
			    b += SRC & 0xff;

			    d =   ((a >> 8) ? 0xff000000 : (a << 24))
					| ((r >> 8) ? 0xff0000   : (r << 16))
					| ((g >> 8) ? 0xff00     : (g << 8))
			    	| ((b >> 8) ? 0xff 		 :  b);

			    *dst = d;

			    dst++;
			}

			// go to the next line
			dst+= dst_pitch_diff;
		}
	}
}

