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

void mmsfb_stretchblit_yv12_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh,
									bool antialiasing) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated stretch YV12 to YV12.\n");
		firsttime = false;
	}

	// get the first source ptr/pitch
	unsigned char *src = (unsigned char *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int src_pitch_pix 		= src_pitch;
	int src_pitch_pix_half	= src_pitch_pix >> 1;
	int dst_pitch_pix 		= dst_pitch;
	int dst_pitch_pix_half	= dst_pitch_pix >> 1;

	// check the surface range
	if ((sw <= 0)||(sh <= 0))
		return;

	// check odd/even
	bool src_odd_left 	= (sx & 0x01);
	bool src_odd_top 	= (sy & 0x01);
	bool src_odd_right 	= ((sx + sw) & 0x01);
	bool src_odd_bottom = ((sy + sh) & 0x01);
	bool dst_odd_left 	= (dx & 0x01);
	bool dst_odd_top 	= (dy & 0x01);
	bool dst_odd_right 	= ((dx + dw) & 0x01);
	bool dst_odd_bottom = ((dy + dh) & 0x01);

	// pointer to the pixel components of the first pixel
	unsigned char *src_y = src + sx + sy * src_pitch_pix;
	unsigned char *src_u;
	unsigned char *src_v;
	if ((extbuf->ptr2)&&(extbuf->ptr3)) {
		src_u = (unsigned char *)extbuf->ptr2 + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
		src_v = (unsigned char *)extbuf->ptr3 + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
	}
	else {
		src_u = src + src_pitch_pix * src_height + src_pitch_pix_half * (src_height >> 1) + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
		src_v = src + src_pitch_pix * src_height                                          + (sx >> 1) + (sy >> 1) * src_pitch_pix_half;
	}
	unsigned char *dst_y = dst + dx + dy * dst_pitch_pix;
	unsigned char *dst_u = dst + dst_pitch_pix * dst_height + dst_pitch_pix_half * (dst_height >> 1) + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;
	unsigned char *dst_v = dst + dst_pitch_pix *  dst_height                                         + (dx >> 1) + (dy >> 1) * dst_pitch_pix_half;

	// antialiasing horizontal/vertical/both
	bool h_antialiasing = false;
	bool v_antialiasing = false;
	if (antialiasing) {
		if (sw != dw)
			h_antialiasing = true;
		if (sh != dh)
			v_antialiasing = true;
	}

	// calc even positions for src...
	if (src_odd_top) {
		// odd top
		sy++;
		sh--;
		src_y+=src_pitch;
		src_u+=src_pitch >> 1;
		src_v+=src_pitch >> 1;
	}
	if (src_odd_bottom) {
		// odd bottom
		src_height--;
	}
	if (src_odd_left) {
		// odd left
		sx++;
		sw--;
		src_y++;
		src_u++;
		src_v++;
	}
	if (src_odd_right) {
		// odd right
		sw--;
	}

	// calc even positions for dst...
	if (dst_odd_top) {
		// odd top
		dy++;
		dh--;
		dst_y+=dst_pitch;
		dst_u+=dst_pitch >> 1;
		dst_v+=dst_pitch >> 1;
	}
	if (dst_odd_bottom) {
		// odd bottom
		dst_height--;
	}
	if (dst_odd_left) {
		// odd left
		dx++;
		dw--;
		dst_y++;
		dst_u++;
		dst_v++;
	}
	if (dst_odd_right) {
		// odd right
		dw--;
	}

	// check if something to do :)
	if ((sw < 2)||(sh < 2)||(dw < 2)||(dh < 2))
		return;

	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////

//printf("sw=%d,dw=%d,sh=%d,dh=%d\n", sw,dw, sh,dh);

	// calc U plane (use Y plane as temp buffer)
	// note: concerning performance we use vertical antialiasing only in combination with horizontal antialiasing
	// note: the stretch and the subsequent 2x2 matrix conversion is needed to calculate the correct arithmetic mean
	stretch_byte_buffer(h_antialiasing, h_antialiasing,
						src_u, src_pitch >> 1, src_pitch_pix >> 1, src_height >> 1, sw >> 1, sh >> 1,
						dst_y, dst_pitch, dst_pitch_pix, dst_height, dw, dh);
	compress_2x2_matrix(dst_y, dst_pitch, dst_pitch_pix, dst_height, dw, dh,
						dst_u, dst_pitch >> 1, dst_pitch_pix >> 1, dst_height >> 1, dw >> 1, dh >> 1);

	// calc V plane (use Y plane as temp buffer)
	// note: concerning performance we use vertical antialiasing only in combination with horizontal antialiasing
	// note: the stretch and the subsequent 2x2 matrix conversion is needed to calculate the correct arithmetic mean
	stretch_byte_buffer(h_antialiasing, h_antialiasing,
						src_v, src_pitch >> 1, src_pitch_pix >> 1, src_height >> 1, sw >> 1, sh >> 1,
						dst_y, dst_pitch, dst_pitch_pix, dst_height, dw, dh);
	compress_2x2_matrix(dst_y, dst_pitch, dst_pitch_pix, dst_height, dw, dh,
						dst_v, dst_pitch >> 1, dst_pitch_pix >> 1, dst_height >> 1, dw >> 1, dh >> 1);

	// calc Y plane
	stretch_byte_buffer(h_antialiasing, v_antialiasing,
						src_y, src_pitch, src_pitch_pix, src_height, sw, sh,
					    dst_y, dst_pitch, dst_pitch_pix, dst_height, dw, dh);
}

