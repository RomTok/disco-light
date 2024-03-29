/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2013 BerLinux Solutions GmbH                       *
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

#ifdef __HAVE_PF_ARGB__
#ifdef __HAVE_PF_BGR24__

#include "mmstools/mmstools.h"

void mmsfb_blit_blend_coloralpha_argb_to_bgr24(MMSFBSurfacePlanes *src_planes, int src_height, int sx, int sy, int sw, int sh,
											   MMSFBSurfacePlanes *dst_planes, int dst_height, int dx, int dy,
											   unsigned char alpha) {
	// check for full alpha value
	if (alpha == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		mmsfb_blit_blend_argb_to_bgr24(src_planes, src_height, sx, sy, sw, sh,
									   dst_planes, dst_height, dx, dy);
		return;
	}

	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated blend coloralpha ARGB to BGR24.\n");
		firsttime = false;
	}

	// something to do?
	if (!alpha)
		// source should blitted full transparent, so leave destination as is
		return;

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)src_planes->ptr;
	int src_pitch = src_planes->pitch;

	// get the first destination ptr/pitch
	unsigned char *dst = (unsigned char *)dst_planes->ptr;
	int dst_pitch = dst_planes->pitch;

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch / 3;
	src+= sx + sy * src_pitch_pix;
	dst+= dx*3 + dy * dst_pitch;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

	unsigned int *src_end = src + src_pitch_pix * sh;
	int src_pitch_diff = src_pitch_pix - sw;
	int dst_pitch_diff = dst_pitch - sw * 3;

	register unsigned int ALPHA = alpha;
	ALPHA++;

	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;
		while (src < line_end) {
			// load pixel from memory
			register unsigned int SRC  = *src;

			// is the source alpha channel != 0x00?
			register unsigned int A = SRC >> 24;
			if (A) {
				// load source pixel and multiply it with given ALPHA
			    A = (ALPHA * A) >> 8;
				register unsigned int SA= 0x100 - A;

				unsigned int r = *dst;
				unsigned int g = *(dst+1);
				unsigned int b = *(dst+2);

				// invert src alpha
			    r = (SA * r) >> 8;
			    g = (SA * g) >> 8;
			    b = (SA * b) >> 8;

			    // add src to dst
			    r += (A*(SRC & 0xff0000)) >> 24;
			    g += (A*(SRC & 0xff00)) >> 16;
			    b += (A*(SRC & 0xff)) >> 8;
			    *dst     = (r >> 8) ? 0xff : r;
			    *(dst+1) = (g >> 8) ? 0xff : g;
			    *(dst+2) = (b >> 8) ? 0xff : b;
			}

		    dst+=3;
		    src++;
		}

		// go to the next line
		src+= src_pitch_diff;
		dst+= dst_pitch_diff;
	}
}

#endif
#endif
