/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2012 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <jens.schneider@diskohq.org>,                   *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mmsgui/fb/mmsfbconv.h"

#ifdef __HAVE_PF_ARGB4444__

#include "mmstools/mmstools.h"

void mmsfb_fillrectangle_blend_argb4444(MMSFBSurfacePlanes *dst_planes, int dst_height,
										int dx, int dy, int dw, int dh, MMSFBColor color) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated blend rectangle to ARGB4444.\n");
		firsttime = false;
	}

	// return immediately if alpha channel of the color is 0x00
	if (!color.a)
		return;

	// get the first destination ptr/pitch
	unsigned short int *dst = (unsigned short int *)dst_planes->ptr;
	int dst_pitch = dst_planes->pitch;

	// prepare...
	int dst_pitch_pix = dst_pitch >> 1;
	dst+= dx + dy * dst_pitch_pix;

	unsigned short int OLDDST = (*dst) + 1;
	unsigned short int *dst_end = dst + dst_pitch_pix * dh;
	int dst_pitch_diff = dst_pitch_pix - dw;
	register unsigned short int d;

	// prepare the color
	register unsigned short int A = color.a;
	register unsigned short int SRC;
	SRC =     ((A >> 4) << 12)
			| ((color.r >> 4) << 8)
			| ((color.g >> 4) << 4)
			|  (color.b >> 4);

	if (color.a == 0xff) {
		// source pixel is not transparent, copy it directly to the destination
		// for all lines
		while (dst < dst_end) {
			// for all pixels in the line
#ifdef __HAVE_SSE__
			// fill memory 2-byte-wise (much faster than loop see below)
//			__asm__ __volatile__ ( "\trep stosw\n" : : "D" (dst), "a" (SRC), "c" (dw));
			short d0, d1, d2;
			__asm__ __volatile__ ( "\tcld\n\trep stosw" \
					: "=&D" (d0), "=&a" (d1), "=&c" (d2) \
					: "0" (dst), "1" (SRC), "2" (dw) \
					: "memory", "cc");

			// go to the next line
			dst+= dst_pitch_pix;
#else
			unsigned short int *line_end = dst + dw;
			while (dst < line_end) {
				*dst = SRC;
			    dst++;
			}

			// go to the next line
			dst+= dst_pitch_diff;
#endif
		}

	}
	else {
		// source alpha is > 0x00 and < 0xff
		// for all lines
		while (dst < dst_end) {
			// for all pixels in the line
			unsigned short int *line_end = dst + dw;
			while (dst < line_end) {
				// read the destination
				register unsigned short int DST = *dst;
				if (DST==OLDDST) {
					// same pixel, use the previous value
					*dst = d;
				    dst++;
					continue;
				}
				OLDDST = DST;

				register int SA= 0x100 - A;
				unsigned int a = DST >> 12;
				unsigned int r = DST & 0x0f00;
				unsigned int g = DST & 0x00f0;
				unsigned int b = DST & 0x000f;

				// invert src alpha
			    a = (SA * a) >> 4;
			    r = (SA * r) >> 12;
			    g = (SA * g) >> 8;
			    b = (SA * b) >> 4;

			    // add src to dst
			    a += A;
			    r += (SRC & 0x0f00) >> 4;
			    g +=  SRC & 0x00f0;
			    b += (SRC & 0x000f) << 4;
			    d =   ((a >> 8) ? 0xf000 : ((a >> 4) << 12))
					| ((r >> 8) ? 0x0f00 : ((r >> 4) << 8))
					| ((g >> 8) ? 0xf0   : ((g >> 4) << 4))
			    	| ((b >> 8) ? 0x0f   :  (b >> 4));
				*dst = d;

			    dst++;
			}

			// go to the next line
			dst+= dst_pitch_diff;
		}
	}
}

#endif
