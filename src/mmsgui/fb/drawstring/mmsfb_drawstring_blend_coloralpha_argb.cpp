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

void mmsfb_drawstring_blend_coloralpha_argb(MMSFBFont *font, void *dst_ptr, int dst_pitch,
											MMSFBRegion &clipreg, string &text, int len, int x, int y, MMSFBColor &color) {
	// check for full alpha value
	if (color.a == 0xff) {
		// max alpha is specified, so i can ignore it and use faster routine
		mmsfb_drawstring_blend_argb(font, dst_ptr, dst_pitch, clipreg, text, len, x, y, color);
		return;
	}

	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using blend text coloralpha ARGB.\n");
		firsttime = false;
	}

	// something to do?
	if (!color.a)
		// source should blitted full transparent, so leave destination as is
		return;

	// lock font and destination surface
	MMSFBSURFACE_BLIT_TEXT_INIT(2);

	// for all characters
	unsigned int OLDDST = 0;
	unsigned int OLDSRC = 0;
	register unsigned int d = 0;
	register unsigned int ALPHA = color.a;
	ALPHA++;
	MMSFBFONT_GET_UNICODE_CHAR(text, len) {
		// load the glyph
		MMSFBSURFACE_BLIT_TEXT_LOAD_GLYPH(character);

		// start rendering of glyph to destination
		MMSFBSURFACE_BLIT_TEXT_START_RENDER(unsigned int);

		// through the pixels
		while (src < src_end) {
			while (src < line_end) {
				// load pixel from memory
				register unsigned int SRC = *src;

				// is the source alpha channel 0x00 or 0xff?
				register unsigned int A = SRC;
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

				    A = (ALPHA * A) >> 8;
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
				    A++;
				    r += (A * color.r) >> 8;
				    g += (A * color.g) >> 8;
				    b += (A * color.b) >> 8;
				    d =   ((a >> 8) ? 0xff000000 : (a << 24))
						| ((r >> 8) ? 0xff0000   : (r << 16))
						| ((g >> 8) ? 0xff00     : (g << 8))
				    	| ((b >> 8) ? 0xff 		 :  b);
					*dst = d;
				}

				src++;
				dst++;
			}
			line_end+= src_pitch_pix;
			src     += src_pitch_pix_diff;
			dst     += dst_pitch_pix_diff;
		}

		// prepare for next loop
		MMSFBSURFACE_BLIT_TEXT_END_RENDER;
	}}
}

