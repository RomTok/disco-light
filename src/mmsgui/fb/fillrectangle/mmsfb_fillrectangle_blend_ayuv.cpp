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

void mmsfb_fillrectangle_blend_ayuv(unsigned int *dst, int dst_pitch, int dst_height,
									int dx, int dy, int dw, int dh, MMSFBColor color) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated blend rectangle to AYUV.\n");
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
			| ((MMSFB_CONV_RGB2Y(color.r, color.g, color.b)) << 16)
			| ((MMSFB_CONV_RGB2U(color.r, color.g, color.b)) << 8)
			| MMSFB_CONV_RGB2V(color.r, color.g, color.b);

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
				int y = (DST << 8) >> 24;
				int u = (DST << 16) >> 24;
				int v = DST & 0xff;

				// we have to move the 0 point of the coordinate system
				// this make it a little slower than ARGB to ARGB blending
				MMSFB_CONV_PREPARE_YUVBLEND(y,u,v);

				// invert src alpha
			    a = (SA * a) >> 8;
			    y = (SA * y) >> 8;
			    u = (SA * u) >> 8;
			    v = (SA * v) >> 8;

			    // add src to dst
			    a += A;
			    y += (SRC << 8) >> 24;
			    u += (SRC << 16) >> 24;
			    v += SRC & 0xff;

			    // build destination pixel, have to check for negative values
				// this make it a little slower than ARGB to ARGB blending
			    d = ((a >> 8) ? 0xff000000 : (a << 24));
			    if (y > 0)
			    	d |= ((y >> 8) ? 0xff0000 : (y << 16));
			    if (u > 0)
			    	d |= ((u >> 8) ? 0xff00 : (u << 8));
			    if (v > 0)
			    	d |= ((v >> 8) ? 0xff : v);

			    *dst = d;

			    dst++;
			}

			// go to the next line
			dst+= dst_pitch_diff;
		}
	}
}

