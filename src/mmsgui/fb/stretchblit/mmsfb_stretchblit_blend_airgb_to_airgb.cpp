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

#ifdef __HAVE_PF_AiRGB__

#include "mmstools/mmstools.h"

void mmsfb_stretchblit_blend_airgb_to_airgb(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
										    unsigned int *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated stretch & blend AiRGB to AiRGB.\n");
		firsttime = false;
	}

	// get the first source ptr/pitch
	unsigned int *src = (unsigned int *)extbuf->ptr;
	int src_pitch = extbuf->pitch;

	// prepare...
	int  src_pitch_pix = src_pitch >> 2;
	int  dst_pitch_pix = dst_pitch >> 2;
	unsigned int *src_end = src + sx + src_pitch_pix * (sy + sh);
	if (src_end > src + src_pitch_pix * src_height)
		src_end = src + src_pitch_pix * src_height;
	unsigned int *dst_end = dst + dst_pitch_pix * dst_height;
	src+=sx + sy * src_pitch_pix;
	dst+=dx + dy * dst_pitch_pix;


//	printf("sw=%d,sh=%d\n", sw,sh);

	int horifact = (dw<<16)/sw;
	int vertfact = (dh<<16)/sh;



	// for all lines
	int vertcnt = 0x8000;
	while ((src < src_end)&&(dst < dst_end)) {
		// for all pixels in the line
		vertcnt+=vertfact;
		if (vertcnt & 0xffff0000) {
			unsigned int *line_end = src + sw;
			unsigned int *old_dst = dst;

			do {
				int horicnt = 0x8000;
				while (src < line_end) {
					// load pixel from memory and check if the previous pixel is the same

					horicnt+=horifact;

					if (horicnt & 0xffff0000) {
						register unsigned int SRC  = *src;
						register unsigned int A = SRC >> 24;

						if (!A) {
							// source pixel is not transparent, copy it directly to the destination
							do {
								*dst = SRC;
								dst++;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}
						else
						if (A == 0xff) {
							// source pixel is full transparent, do not change the destination
							do {
								dst++;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}
						else
						{
							// source alpha is > 0x00 and < 0xff
							register unsigned int DST = *dst;
							unsigned int OLDDST = DST + 1;
							register unsigned int d;

							do {
								if (DST==OLDDST) {
									// same pixel, use the previous value
									if (A < 0xff) {
										// source has an alpha
										*dst = d;
									}
								    dst++;
								    DST = *dst;
									horicnt-=0x10000;
									continue;
								}
								OLDDST = DST;

								// extract destination
								unsigned int a = DST >> 24;
								unsigned int r = (DST << 8) >> 24;
								unsigned int g = (DST << 16) >> 24;
								unsigned int b = DST & 0xff;

								// invert src alpha
							    a = (A * (0x100 - a)) >> 8;
							    r = (A * r) >> 8;
							    g = (A * g) >> 8;
							    b = (A * b) >> 8;

							    // add src to dst
							    a += 0x100 - A;
							    r += (SRC << 8) >> 24;
							    g += (SRC << 16) >> 24;
							    b += SRC & 0xff;
								d =   ((r >> 8) ? 0xff0000   : (r << 16))
									| ((g >> 8) ? 0xff00     : (g << 8))
							    	| ((b >> 8) ? 0xff 		 :  b);
							    if (!(a >> 8)) d |= (0x100 - a) << 24;
								*dst = d;
								dst++;
								DST = *dst;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}
					}

				    src++;
				}
				src-=sw;
				vertcnt-=0x10000;
				dst = old_dst +  dst_pitch/4;
				old_dst = dst;
			} while (vertcnt & 0xffff0000);
		}

		// next line
		src+=src_pitch/4;
	}
}

#endif
