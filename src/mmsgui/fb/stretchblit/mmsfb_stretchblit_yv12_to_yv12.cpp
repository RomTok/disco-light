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
									bool h_antialiasing, bool v_antialiasing) {
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

//	src+= sx + sy * src_pitch_pix;

	// check the surface range
	if ((sw <= 0)||(sh <= 0))
		return;

//	unsigned int OLDSRC  = (*src) + 1;
//	unsigned int old_y;
//	unsigned int old_u;
//	unsigned int old_v;

	int  src_pixels = src_pitch_pix * sh;

	// check odd/even
	bool odd_left 	= (dx & 0x01);
	bool odd_top 	= (dy & 0x01);
	bool odd_right 	= ((dx + sw) & 0x01);
	bool odd_bottom = ((dy + sh) & 0x01);

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

	// offsets to the other three pixels
	unsigned int src_y2_offs = 1;
	unsigned int src_y3_offs = src_pitch_pix;
	unsigned int src_y4_offs = src_y3_offs + 1;
	unsigned int dst_y2_offs = 1;
	unsigned int dst_y3_offs = dst_pitch_pix;
	unsigned int dst_y4_offs = dst_y3_offs + 1;

	// arithmetic mean
	register unsigned int d_u;
	register unsigned int d_v;

	// draw odd pixels around the even rectangle
	if (odd_top && odd_left) {
/*		// odd top-left pixel
		register unsigned int SRC;
		register unsigned int A;

		// for arithmetic mean we have to set U and V from pixels outside the current rectangle
		d_u = (*dst_u) * 3;
		d_v = (*dst_v) * 3;

	    // calculate my pixel...
		MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(*src, *dst_y, *dst_u, *dst_v, d_u+=, d_v+=);

		// calulate the arithmetic mean
		*dst_u = d_u >> 2;
		*dst_v = d_v >> 2;*/
	}

	if (odd_top && odd_right) {
/*		// odd top-right pixel
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
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom && odd_left) {
/*		// odd bottom-left pixel
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
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom && odd_right) {
/*		// odd bottom-right pixel
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
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_top) {
/*		// odd top line
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
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_bottom) {
/*		// odd bottom line
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
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_left) {
/*		// odd left line
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
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	if (odd_right) {
/*		// odd right line
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
		MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR;*/
	}

	// calc even positions...
	if (odd_top) {
		// odd top
		dy++;
		sh--;
		src_y+=src_pitch;
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

	if (odd_left) {
		// odd left
		dx++;
		sw--;
		src_y++;
		dst_y++;
		dst_u++;
		dst_v++;
	}

	if (odd_right) {
		// odd right
		sw--;
	}

	// now we are even aligned and can go through a optimized loop
	////////////////////////////////////////////////////////////////////////
	int horifact = (dw<<16)/sw;
	int vertfact = (dh<<16)/sh;
	int vertcnt;
	unsigned char *src_end;
	unsigned char *dst_end;

	// working for Y plane......................................................
	src = src_y;
	dst = dst_y;
	src_end = src + sx + src_pitch_pix * (sy + sh);
	if (src_end > src + src_pitch_pix * src_height)
		src_end = src + src_pitch_pix * src_height;
	dst_end = dst + dst_pitch_pix * dst_height;

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


	// working for U/V planes...................................................
	sw = sw >> 1;
	dw = dw >> 1;
	sh = sh >> 1;
	dh = dh >> 1;
	for (int cnt = 0; cnt < 2; cnt++) {
		if (cnt==0) {
			// U plane
			src = src_u;
			dst = dst_u;
			src_end = src + sx + src_pitch_pix_half * (sy + sh);
			if (src_end > src + src_pitch_pix_half * src_height)
				src_end = src + src_pitch_pix_half * src_height;
			dst_end = dst + dst_pitch_pix_half * dst_height;
		}
		else {
			// V plane
			src = src_v;
			dst = dst_v;
			src_end = src + sx + src_pitch_pix_half * (sy + sh);
			if (src_end > src + src_pitch_pix_half * src_height)
				src_end = src + src_pitch_pix_half * src_height;
			dst_end = dst + dst_pitch_pix_half * dst_height;
		}

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
							// load pixel from memory and check if the previous pixel is the same

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
						dst = old_dst +  dst_pitch_pix_half;
						old_dst = dst;
					} while (vertcnt & 0xffff0000);
				}

				// next line
				src+=src_pitch_pix_half;
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
							dst = old_dst + dst_pitch_pix_half;
							old_dst = dst;
						} while (vertcnt & 0xffff0000);
					}

					// next line
					src+=src_pitch_pix_half;
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
								dst = old_dst + dst_pitch_pix_half;
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
										*(dst-dst_pitch_pix_half) = (*(dst-dst_pitch_pix_half) + SRC) >> 1;
										dst++;
										horicnt-=0x10000;

										// have to put further?
										if ((haa=(horicnt & 0xffff0000))) {
											do {
												*dst = SRC;
												*(dst-dst_pitch_pix_half) = (*(dst-dst_pitch_pix_half) + SRC) >> 1;
												dst++;
												horicnt-=0x10000;
											} while (horicnt & 0xffff0000);
										}
									}

									src++;
								}
								src-=sw;
								vertcnt-=0x10000;
								dst = old_dst + dst_pitch_pix_half;
								old_dst = dst;
								vcnt++;
							} while (vertcnt & 0xffff0000);
						}
					}

					// next line
					src+=src_pitch_pix_half;
				}
			}
		}
	}
}
