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




void mmsfb_blit_yuy2_to_yv12(MMSFBSurfacePlanes *extbuf, int src_height, int sx, int sy, int sw, int sh,
							 unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated conversion YUY2 to YV12.\n");
		firsttime = false;
	}




	// source/dest x and w must be multiple of two
	// reason: YUY2 codes U/V values separated in two horizontal pixels
	if (sx & 0x01) {
		sx++;
		sw--;
		dx++;
	}
	sw &= ~0x01;
	if (dx & 0x01) {
		dx++;
	}





	// get the first source ptr/pitch
	unsigned short int *src = (unsigned short int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int  src_pitch_pix 		= src_pitch >> 2;
	int dst_pitch_pix 		= dst_pitch;
	int dst_pitch_pix_half	= dst_pitch_pix >> 1;

	src+= sx + sy * src_pitch_pix;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

	int  src_pixels = src_pitch_pix * sh;

	// check odd/even
	bool odd_top 	= (dy & 0x01);
	bool odd_bottom = ((dy + sh) & 0x01);

	// pointer to the pixel components of the first pixel
	unsigned char *dst_y = dst + dx + dy * dst_pitch_pix;
	unsigned char *dst_u = dst + dst_pitch_pix * dst_height + dst_pitch_pix_half * (dst_height >> 1) + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	unsigned char *dst_v = dst + dst_pitch_pix * dst_height                                          + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;

	// offsets to the other three pixels
	unsigned int dst_y2_offs = 1;
	unsigned int dst_y3_offs = dst_pitch;
	unsigned int dst_y4_offs = dst_y3_offs + 1;
	unsigned int src2_offs = 1;
	unsigned int src3_offs = src_pitch_pix;
	unsigned int src4_offs = src3_offs + 1;

	// arithmetic mean
	register unsigned int d_u;
	register unsigned int d_v;

	if (odd_top) {
		// odd top line
		MMSFB_CONV_YUY2_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned short int *line_end = src + sw;

		// through the line
		while (src < line_end) {
			register unsigned short int SRC;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFB_CONV_YUY2_TO_YV12_PIXEL_2(*src, *dst_y, d_u+=);
			MMSFB_CONV_YUY2_TO_YV12_PIXEL_2(src[src2_offs], dst_y[dst_y2_offs], d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFB_CONV_YUY2_TO_YV12_POPPTR;
	}

	if (odd_bottom) {
		// odd bottom line
		MMSFB_CONV_YUY2_TO_YV12_PUSHPTR;

		// calculate start and end
		src   += src_pitch_pix * (sh-1);
		dst_y += dst_pitch_pix * (sh-1);
		if (odd_top) {
			dst_u += dst_pitch_pix_half * (sh >> 1);
			dst_v += dst_pitch_pix_half * (sh >> 1);
		}
		else {
			dst_u += dst_pitch_pix_half * ((sh-1) >> 1);
			dst_v += dst_pitch_pix_half * ((sh-1) >> 1);
		}

		unsigned short int *line_end = src + sw;

		// through the line
		while (src < line_end) {
			register unsigned short int SRC;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFB_CONV_YUY2_TO_YV12_PIXEL_2(*src, *dst_y, d_u+=);
			MMSFB_CONV_YUY2_TO_YV12_PIXEL_2(src[src2_offs], dst_y[dst_y2_offs], d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two pixels
		    src+=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// restore the pointers
		MMSFB_CONV_YUY2_TO_YV12_POPPTR;
	}

	// calc even positions...
	if (odd_top) {
		// odd top
		dy++;
		sh--;
		src+=src_pitch_pix;
		src_pixels-=src_pitch_pix;
		dst_y+=dst_pitch;
		dst_u+=dst_pitch >> 1;
		dst_v+=dst_pitch >> 1;
	}

	if (odd_bottom) {
		// odd bottom
		src_height--;
		src_pixels-=src_pitch_pix;
	}

	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
	unsigned short int *src_end = src + src_pixels;
	int src_pitch_diff = (src_pitch_pix << 1) - sw;
	int dst_pitch_diff = (dst_pitch_pix << 1) - sw;
	int dst_pitch_uvdiff = (dst_pitch_pix - sw) >> 1;

	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned short int *line_end = src + sw;

		// go through two lines in parallel (square 2x2 pixel)
		while (src < line_end) {
			register unsigned short int SRC;

		    // calculate the four pixels...
			MMSFB_CONV_YUY2_TO_YV12_PIXEL(*src, *dst_y, d_u=);
			MMSFB_CONV_YUY2_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], d_v=);
			MMSFB_CONV_YUY2_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], d_u+=);
			MMSFB_CONV_YUY2_TO_YV12_PIXEL(src[src4_offs], dst_y[dst_y4_offs], d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 1;
			*dst_v = d_v >> 1;

			// go to the next two pixels
		    src  +=2;
		    dst_y+=2;
		    dst_u++;
		    dst_v++;
		}

		// go to the next two lines
		src   += src_pitch_diff;
		dst_y += dst_pitch_diff;
		dst_u += dst_pitch_uvdiff;
		dst_v += dst_pitch_uvdiff;
	}
}



