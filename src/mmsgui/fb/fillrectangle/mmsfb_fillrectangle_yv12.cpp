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

#include <cstring>
#include "mmsgui/fb/mmsfbconv.h"
#include "mmstools/mmstools.h"

void mmsfb_fillrectangle_yv12(MMSFBSurfacePlanes *dst_planes, int dst_height,
						      int dx, int dy, int dw, int dh, MMSFBColor color) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated fill rectangle to YV12.\n");
		firsttime = false;
	}

	// get the first destination ptr/pitch
	unsigned char *dst = (unsigned char *)dst_planes->ptr;
	int dst_pitch = dst_planes->pitch;

	// prepare...
//	int  src_pitch_pix 		= src_pitch >> 2;
	int dst_pitch_pix 		= dst_pitch;
	int dst_pitch_pix_half	= dst_pitch_pix >> 1;

//	src+= sx + sy * src_pitch_pix;

//	unsigned int OLDSRC  = (*src) + 1;
//	unsigned int old_y;
//	unsigned int old_u;
//	unsigned int old_v;

//	int  src_pixels = src_pitch_pix * sh;

	// check odd/even
	bool odd_left 	= (dx & 0x01);
	bool odd_top 	= (dy & 0x01);
	bool odd_right 	= ((dx + dw) & 0x01);
	bool odd_bottom = ((dy + dh) & 0x01);

	// pointer to the pixel components of the first destination pixel
	unsigned char *dst_y = dst + dx + dy * dst_pitch_pix;
	unsigned char *dst_u;
	unsigned char *dst_v;
	if ((dst_planes->ptr2)&&(dst_planes->ptr3)) {
		dst_u = (unsigned char *)dst_planes->ptr2 + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
		dst_v = (unsigned char *)dst_planes->ptr3 + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	}
	else {
		dst_u = dst + dst_pitch_pix * dst_height + dst_pitch_pix_half * (dst_height >> 1) + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
		dst_v = dst + dst_pitch_pix * dst_height                                          + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	}


	// offsets to the other three pixels
//	unsigned int dst_y2_offs = 1;
//	unsigned int dst_y3_offs = dst_pitch;
//	unsigned int dst_y4_offs = dst_y3_offs + 1;
//	unsigned int src2_offs = 1;
//	unsigned int src3_offs = src_pitch_pix;
//	unsigned int src4_offs = src3_offs + 1;

	// arithmetic mean
//	register unsigned int d_u;
//	register unsigned int d_v;

	// prepare the color
	unsigned int SRC_Y = MMSFB_CONV_RGB2Y(color.r, color.g, color.b);
	unsigned int SRC_U = MMSFB_CONV_RGB2U(color.r, color.g, color.b);
	unsigned int SRC_V = MMSFB_CONV_RGB2V(color.r, color.g, color.b);

	// draw odd pixels around the even rectangle
	/*if (odd_top && odd_left) {
		// odd top-left pixel
		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;
	}*/

	/*if (odd_top && odd_right) {
		// odd top-right pixel
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;
	}*/

/*	if (odd_bottom && odd_left) {
		// odd bottom-left pixel
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
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

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;
	}*/

	/*if (odd_bottom && odd_right) {
		// odd bottom-right pixel
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PUSHPTR;

		// go to the line
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

		// go to the pixel in the current line
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}

		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;

		// restore the pointers
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;
	}*/

	/*if (odd_top) {
		// odd top line
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

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
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;
	}*/

	/*if (odd_bottom) {
		// odd bottom line
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PUSHPTR;

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

		unsigned int *line_end = src + sw;
		if (odd_left) {
			src++;
			dst_y++;
			dst_u++;
			dst_v++;
			line_end--;
		}
		if (odd_right)
			line_end--;

		// through the line
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(src[src2_offs], dst_y[dst_y2_offs], *dst_u, *dst_v, d_u+=, d_v+=);

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
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;
	}*/

	/*if (odd_left) {
		// odd left line
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;
	}*/

	/*if (odd_right) {
		// odd right line
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PUSHPTR;

		// calculate start and end
		unsigned int *src_end = src + src_pixels;
		int src_pitch_diff    = src_pitch_pix << 1;
		int dst_pitch_diff    = dst_pitch_pix << 1;
		int dst_pitch_uvdiff  = dst_pitch_pix_half;
		src   += sw - 1;
		dst_y += sw - 1;
		if (odd_left) {
			dst_u += sw >> 1;
			dst_v += sw >> 1;
		}
		else {
			dst_u += (sw - 1) >> 1;
			dst_v += (sw - 1) >> 1;
		}
		if (odd_top) {
			src     += src_pitch_pix;
			src_end -= src_pitch_pix;
			dst_y   += dst_pitch_pix;
			dst_u   += dst_pitch_pix_half;
			dst_v   += dst_pitch_pix_half;
		}
		if (odd_bottom)
			src_end -= src_pitch_pix;

		// through all lines
		while (src < src_end) {
			// for the first pixel in the line
			register unsigned int SRC;
			register unsigned int A;

			// for arithmetic mean we have to set U and V from pixels outside the current rectangle
			d_u = (*dst_u) << 1;
			d_v = (*dst_v) << 1;

		    // calculate my two pixels...
			MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);
			MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(src[src3_offs], dst_y[dst_y3_offs], *dst_u, *dst_v, d_u+=, d_v+=);

			// calulate the arithmetic mean
			*dst_u = d_u >> 2;
			*dst_v = d_v >> 2;

			// go to the next two lines
			src   += src_pitch_diff;
			dst_y += dst_pitch_diff;
			dst_u += dst_pitch_uvdiff;
			dst_v += dst_pitch_uvdiff;
		}

		// restore the pointers
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;
	}*/

	// calc even positions...
	if (odd_top) {
		// odd top
		dy++;
		dh--;
		dst_y+=dst_pitch;
		dst_u+=dst_pitch >> 1;
		dst_v+=dst_pitch >> 1;
	}

	if (odd_bottom) {
		// odd bottom
		dh--;
	}

	if (odd_left) {
		// odd left
		dx++;
		dw--;
		dst_y++;
		dst_u++;
		dst_v++;
	}

	if (odd_right) {
		// odd right
		dw--;
	}


	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
	// clear Y
	unsigned char *dst_end = dst_y + dst_pitch_pix * dh;
	int dw_half = dw >> 1;
	while (dst_y < dst_end) {
		memset(dst_y, SRC_Y, dw);
		dst_y+= dst_pitch_pix;
	}
	// clear U
	dst_end = dst_u + dst_pitch_pix_half * (dh >> 1);
	while (dst_u < dst_end) {
		memset(dst_u, SRC_U, dw_half);
		dst_u+= dst_pitch_pix_half;
	}
	// clear V
	dst_end = dst_v + dst_pitch_pix_half * (dh >> 1);
	while (dst_v < dst_end) {
		memset(dst_v, SRC_V, dw_half);
		dst_v+= dst_pitch_pix_half;
	}
}

