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

#define MMSFB_BLIT_BLEND_ARGB_TO_ARGB3565(src, dst, dst_a, alpha)	\
			SRC  = src;												\
			A    = SRC >> 24;								\
			if (A == 0xff) {								\
				if (SRC==OLDSRC) {							\
					dst = d;								\
					alpha da;								\
				}											\
				else {										\
					OLDSRC = SRC;							\
					unsigned int r = (SRC << 8) >> 27;		\
					unsigned int g = (SRC << 16) >> 26;		\
					unsigned int b = (SRC & 0xff) >> 3;		\
					d =   (r << 11)							\
						| (g << 5)							\
						| b;								\
					da = SRC >> 29;							\
					dst = d;								\
					alpha da;								\
				}											\
			}												\
			else											\
			if (A) {										\
				register unsigned short int DST = dst;		\
				if ((DST==OLDDST)&&(dst_a==OLDDSTA)&&(SRC==OLDSRC)) {			\
					dst = d;								\
					alpha da;								\
				}											\
				else  {										\
					OLDDST  = DST;							\
					OLDDSTA = dst_a;						\
					OLDSRC = SRC;							\
					register unsigned int SA= 0x100 - A;	\
					unsigned int a = dst_a << 5;			\
					unsigned int r = (DST >> 11) << 3;				\
					unsigned int g = ((DST << 5) >> 10) << 2;		\
					unsigned int b = (DST & 0x1f) << 3;			\
					a = (SA * a) >> 8;						\
					r = (SA * r) >> 8;						\
					g = (SA * g) >> 8;						\
					b = (SA * b) >> 8;						\
					a += A;									\
					r += (SRC << 8) >> 24;					\
					g += (SRC << 16) >> 24;					\
					b += SRC & 0xff;						\
					d =   ((r >> 8) ? 0xf800   	: ((r >> 3) << 11))	\
						| ((g >> 8) ? 0x07e0	: ((g >> 2) << 5))	\
						| ((b >> 8) ? 0x1f 		: (b >> 3));	\
					da = (a >> 8) ? 0x07 : a >> 5;			\
					dst = d;								\
					alpha da;								\
				}											\
			}


/*
			// load pixel from memory and check if the previous pixel is the same
			register unsigned int SRC = *src;

			// is the source alpha channel 0x00 or 0xff?
			register unsigned int A = SRC >> 24;
			if (A == 0xff) {
				// source pixel is not transparent, copy it directly to the destination
				*dst = SRC;
			}
			else
			if (A) {
				// source alpha is > 0x00 and < 0xff
				register unsigned int DST = *dst;

				if ((DST==OLDDST)&&(SRC==OLDSRC)) {
					// same pixel, use the previous value
					*dst = d;
				    dst++;
				    src++;
					continue;
				}
				OLDDST = DST;
				OLDSRC = SRC;

				register unsigned int SA= 0x100 - A;
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
			}

		    dst++;
		    src++;
*/



void mmsfb_blit_blend_argb_to_argb3565(MMSFBSurfacePlanes *src_planes, int src_height, int sx, int sy, int sw, int sh,
									   MMSFBSurfacePlanes *dst_planes, int dst_height, int dx, int dy) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated blend ARGB to ARGB3565.\n");
		firsttime = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)src_planes->ptr;
	int src_pitch = src_planes->pitch;

	// DST: point to the first plane (RGB16/RGB565 format)
	unsigned short int *dst = (unsigned short int *)dst_planes->ptr;
	int dst_pitch = dst_planes->pitch;

	// DST: point to the second plane (3 bit alpha (2 pixels per byte, 4 bit per pixel))
	unsigned char *dst_a;
	int dst_a_pitch;
	if (dst_planes->ptr2) {
		// plane pointer given
		dst_a = (unsigned char *)dst_planes->ptr2;
		dst_a_pitch = dst_planes->pitch2;
	}
	else {
		// calc plane pointer (after the first plane)
    	dst_a = ((unsigned char *)dst_planes->ptr) + dst_planes->pitch * dst_height;
    	dst_a_pitch = dst_planes->pitch / 4;
	}

	// prepare...
	int src_pitch_pix = src_pitch >> 2;
	int dst_pitch_pix = dst_pitch >> 1;
	src+= sx + sy * src_pitch_pix;
	dst+= dx + dy * dst_pitch_pix;
	dst_a+= (dx >> 1) + dy * dst_a_pitch;

	// check the surface range
	if (dst_pitch_pix - dx < sw - sx)
		sw = dst_pitch_pix - dx - sx;
	if (dst_height - dy < sh - sy)
		sh = dst_height - dy - sy;
	if ((sw <= 0)||(sh <= 0))
		return;

	// check odd/even
	bool odd_left 	= (dx & 0x01);
	bool odd_right 	= ((dx + sw) & 0x01);


	//TODO: not even...



	// calc even positions...
	if (odd_left) {
		// odd left
		dx++;
		sw--;
		src++;
		dst++;
		dst_a++;
	}

	if (odd_right) {
		// odd right
		sw--;
	}

	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
	unsigned short int OLDDST  = (*dst) + 1;
	unsigned char 	   OLDDSTA = (*dst_a) + 1;
	unsigned int OLDSRC = (*src) + 1;
	unsigned int *src_end = src + src_pitch_pix * sh;
	int src_pitch_diff = src_pitch_pix - sw;
	int dst_pitch_diff = dst_pitch_pix - sw;
	int dst_a_pitch_diff = dst_a_pitch - (sw >> 1);
	register unsigned short int d;
	register unsigned char da;


	// for all lines
	while (src < src_end) {
		// for all pixels in the line
		unsigned int *line_end = src + sw;
		while (src < line_end) {
			register unsigned int SRC;
			register unsigned int A;
			unsigned char alpha;

			// process two pixels
			MMSFB_BLIT_BLEND_ARGB_TO_ARGB3565(*src, *dst, ((*dst_a) >> 4), alpha=);
			MMSFB_BLIT_BLEND_ARGB_TO_ARGB3565(*(src+1), *(dst+1), (*dst_a)&0x0f, alpha=(alpha<<4)|);
			dst+=2;
			src+=2;

			// set alpha value
			*dst_a = alpha;
			dst_a++;
		}

		// go to the next line
		src+= src_pitch_diff;
		dst+= dst_pitch_diff;
		dst_a+= dst_a_pitch_diff;
	}
}

