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

void stretch_byte_buffer(bool h_antialiasing, bool v_antialiasing,
						 unsigned char *src, int src_pitch, int src_pitch_pix, int src_height, int sw, int sh,
					     unsigned char *dst, int dst_pitch, int dst_pitch_pix, int dst_height, int dw, int dh) {
	// please note that the src and dst have to point to the first pixel which is to process
	int horifact = (dw<<16)/sw;
	int vertfact = (dh<<16)/sh;
	int vertcnt;
	unsigned char *src_end = src + src_pitch_pix * sh;
	if (src_end > src + src_pitch_pix * src_height)
		src_end = src + src_pitch_pix * src_height;
	unsigned char *dst_end = dst + dst_pitch_pix * dst_height;

	if (!h_antialiasing) {
		// no antialiasing
		// for all lines
		vertcnt = 0x8000;
		while ((src < src_end)&&(dst < dst_end)) {
			// for all pixels in the line
			vertcnt+=vertfact;
			if (vertcnt & 0xffff0000) {
				unsigned char *line_end = src + sw;
				unsigned char *old_dst = dst;

				do {
					int horicnt = 0x8000;
					while (src < line_end) {
						horicnt+=horifact;
						if (horicnt & 0xffff0000) {
							register unsigned char SRC  = *src;

							do {
								*dst = SRC;
								dst++;
								horicnt-=0x10000;
							} while (horicnt & 0xffff0000);
						}

						src++;
					}
					src-=sw;
					vertcnt-=0x10000;
					dst = old_dst +  dst_pitch;
					old_dst = dst;
				} while (vertcnt & 0xffff0000);
			}

			// next line
			src+=src_pitch;
		}
	}
	else {
		if (!v_antialiasing) {
			// horizontal antialiasing
			// for all lines
			vertcnt = 0x8000;
			while ((src < src_end)&&(dst < dst_end)) {
				// for all pixels in the line
				vertcnt+=vertfact;
				if (vertcnt & 0xffff0000) {
					unsigned char *line_end = src + sw;
					unsigned char *old_dst = dst;

					do {
						int horicnt = 0x8000;
						register unsigned int SRC;
						register bool haa = false;
						while (src < line_end) {
							horicnt+=horifact;
							if (horicnt & 0xffff0000) {
								// check for antialiasing
								if (haa) {
									*(dst-1) = (SRC + *src) >> 1;
									haa = false;
								}

								// load pixel
								SRC = *src;

								// put first pixel
								*dst = SRC;
								dst++;
								horicnt-=0x10000;

								// have to put further?
								if ((haa=(horicnt & 0xffff0000))) {
									do {
										*dst = SRC;
										dst++;
										horicnt-=0x10000;
									} while (horicnt & 0xffff0000);
								}
							}

							src++;
						}
						src-=sw;
						vertcnt-=0x10000;
						dst = old_dst + dst_pitch;
						old_dst = dst;
					} while (vertcnt & 0xffff0000);
				}

				// next line
				src+=src_pitch;
			}
		}
		else {
			// horizontal and vertical antialiasing
			// for all lines
			vertcnt = 0x8000;
			register unsigned char vcnt = 0;
			while ((src < src_end)&&(dst < dst_end)) {
				// for all pixels in the line
				vertcnt+=vertfact;
				if (vertcnt & 0xffff0000) {
					unsigned char *line_end = src + sw;
					unsigned char *old_dst = dst;

					bool vaa = (vcnt > 1);
					vcnt = 0;
					if (!vaa) {
						do {
							int horicnt = 0x8000;
							register unsigned int SRC;
							register bool haa = false;
							while (src < line_end) {
								horicnt+=horifact;
								if (horicnt & 0xffff0000) {
									// check for antialiasing
									if (haa) {
										*(dst-1) = (SRC + *src) >> 1;
										haa = false;
									}

									// load pixel
									SRC = *src;

									// put first pixel
									*dst = SRC;
									dst++;
									horicnt-=0x10000;

									// have to put further?
									if ((haa=(horicnt & 0xffff0000))) {
										do {
											*dst = SRC;
											dst++;
											horicnt-=0x10000;
										} while (horicnt & 0xffff0000);
									}
								}

								src++;
							}
							src-=sw;
							vertcnt-=0x10000;
							dst = old_dst + dst_pitch;
							old_dst = dst;
							vcnt++;
						} while (vertcnt & 0xffff0000);
					}
					else {
						do {
							int horicnt = 0x8000;
							register unsigned int SRC;
							register bool haa = false;
							while (src < line_end) {
								horicnt+=horifact;
								if (horicnt & 0xffff0000) {
									// check for antialiasing
									if (haa) {
										*(dst-1) = (SRC + *src) >> 1;
										haa = false;
									}

									// load pixel
									SRC = *src;

									// put first pixel
									*dst = SRC;
									*(dst-dst_pitch) = (*(dst-dst_pitch) + SRC) >> 1;
									dst++;
									horicnt-=0x10000;

									// have to put further?
									if ((haa=(horicnt & 0xffff0000))) {
										do {
											*dst = SRC;
											*(dst-dst_pitch) = (*(dst-dst_pitch) + SRC) >> 1;
											dst++;
											horicnt-=0x10000;
										} while (horicnt & 0xffff0000);
									}
								}

								src++;
							}
							src-=sw;
							vertcnt-=0x10000;
							dst = old_dst + dst_pitch;
							old_dst = dst;
							vcnt++;
						} while (vertcnt & 0xffff0000);
					}
				}

				// next line
				src+=src_pitch;
			}
		}
	}
}


void compress_2x2_matrix(unsigned char *src, int src_pitch, int src_pitch_pix, int src_height, int sw, int sh,
						 unsigned char *dst, int dst_pitch, int dst_pitch_pix, int dst_height, int dw, int dh) {
	// please note that the src and dst have to point to the first pixel which is to process
	unsigned char *src_end = src + src_pitch_pix * sh;
	if (src_end > src + src_pitch_pix * src_height)
		src_end = src + src_pitch_pix * src_height;
	unsigned char *dst_end = dst + dst_pitch_pix * dst_height;

	// prepare (have to work with even width/height)
	sw &= 0xfffffffe;
	sh &= 0xfffffffe;
	int src_pitch_diff = (src_pitch_pix << 1) - sw;
	int dst_pitch_diff = dst_pitch_pix - (sw >> 1);

	// calculate the arithmetic mean
	while ((src < src_end)&&(dst < dst_end)) {
		// go through two lines in parallel (square 2x2 pixel)
		unsigned char *line_end = src + sw;
		while (src < line_end) {
			int d = (int)*src + (int)src[1] + (int)src[src_pitch_pix] + (int)src[src_pitch_pix+1];
			*dst = d >> 2;
			src+=2;
			dst++;
		}

		// go to the next two lines
		src += src_pitch_diff;
		dst += dst_pitch_diff;
	}
}


void stretch_uint_buffer(bool h_antialiasing, bool v_antialiasing,
						 unsigned int *src, int src_pitch, int src_pitch_pix, int src_height, int sw, int sh,
					     unsigned int *dst, int dst_pitch, int dst_pitch_pix, int dst_height, int dw, int dh) {
	// please note that the src and dst have to point to the first pixel which is to process
	int horifact = (dw<<16)/sw;
	int vertfact = (dh<<16)/sh;
	int vertcnt;
	unsigned int *src_end = src + src_pitch_pix * sh;
	if (src_end > src + src_pitch_pix * src_height)
		src_end = src + src_pitch_pix * src_height;
	unsigned int *dst_end = dst + dst_pitch_pix * dst_height;

	// for all lines
	vertcnt = 0x8000;
	while ((src < src_end)&&(dst < dst_end)) {
		// for all pixels in the line
		vertcnt+=vertfact;
		if (vertcnt & 0xffff0000) {
			unsigned int *line_end = src + sw;
			unsigned int *old_dst = dst;

			do {
				int horicnt = 0x8000;
				while (src < line_end) {
					horicnt+=horifact;
					if (horicnt & 0xffff0000) {
						register unsigned int SRC  = *src;

						do {
							*dst = SRC;
							dst++;
							horicnt-=0x10000;
						} while (horicnt & 0xffff0000);
					}

					src++;
				}
				src-=sw;
				vertcnt-=0x10000;
				dst = old_dst + dst_pitch_pix;
				old_dst = dst;
			} while (vertcnt & 0xffff0000);
		}

		// next line
		src+=src_pitch_pix;
	}
}

