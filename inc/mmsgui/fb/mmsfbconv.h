/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
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

#ifndef MMSFBCONV_H_
#define MMSFBCONV_H_

//! Describes up to 3 planes of an external surface buffer.
typedef struct {
	void	*ptr;
	int 	pitch;
	void	*ptr2;
	int 	pitch2;
	void	*ptr3;
	int 	pitch3;
} MMSFBExternalSurfaceBuffer;

#define MMSFB_CONV_RGB2Y(r,g,b) ((((66*r+129*g+25*b+128)>>8)+16) & 0xff)
#define MMSFB_CONV_RGB2U(r,g,b) ((((-38*r-74*g+112*b+128)>>8)+128) & 0xff)
#define MMSFB_CONV_RGB2V(r,g,b) ((((112*r-94*g-18*b+128)>>8)+128) & 0xff)

#define MMSFB_CONV_PREPARE_YUV2RGB(y,u,v)  y=(int)y-16;u=(int)u-128;v=(int)v-128;
#define MMSFB_CONV_PREPARE_YUVBLEND(y,u,v) y=(int)y-16;u=(int)u-128;v=(int)v-128;
#define MMSFB_CONV_RESET_YUVBLEND(y,u,v) y=(int)y+16;u=(int)u+128;v=(int)v+128;

#define MMSFB_CONV_YUV2R(y,u,v,r) if ((r=(298*(int)y+409*(int)v+128+0x200)>>8)>>8) r=0xff;
#define MMSFB_CONV_YUV2G(y,u,v,g) if ((g=(298*(int)y-100*(int)u-208*(int)v+128+0x200)>>8)>>8) g=0xff;
#define MMSFB_CONV_YUV2B(y,u,v,b) if ((b=(298*(int)y+516*(int)u+128+0x200)>>8)>>8) b=0xff;
#define MMSFB_CONV_YUV2RX(y,u,v,r) if ((r=298*(int)y+409*(int)v+128+0x200)>>16) r=0xff00;
#define MMSFB_CONV_YUV2GX(y,u,v,g) if ((g=298*(int)y-100*(int)u-208*(int)v+128+0x200)>>16) g=0xff00;
#define MMSFB_CONV_YUV2BX(y,u,v,b) if ((b=298*(int)y+516*(int)u+128+0x200)>>16) b=0xff00;


#define MMSFB_CONV_RGB24_TO_YV12_PUSHPTR \
	unsigned char *saved_src   = src;   \
	unsigned char *saved_dst_y = dst_y; \
	unsigned char *saved_dst_u = dst_u; \
	unsigned char *saved_dst_v = dst_v;

#define MMSFB_CONV_RGB24_TO_YV12_POPPTR \
	src   = saved_src;   \
	dst_y = saved_dst_y; \
	dst_u = saved_dst_u; \
	dst_v = saved_dst_v;

#define MMSFB_CONV_RGB24_TO_YV12_PIXEL(src, dst_y, dst_u, dst_v, d_u, d_v) \
	{	register int r = *(src);				\
		register int g = *(src+1);				\
		register int b = *(src+2);				\
		dst_y = MMSFB_CONV_RGB2Y(r,g,b);		\
		d_u     MMSFB_CONV_RGB2U(r,g,b);		\
		d_v     MMSFB_CONV_RGB2V(r,g,b); }



#define MMSFB_CONV_ARGB_TO_YV12_PUSHPTR \
	unsigned int  *saved_src   = src;   \
	unsigned char *saved_dst_y = dst_y; \
	unsigned char *saved_dst_u = dst_u; \
	unsigned char *saved_dst_v = dst_v;

#define MMSFB_CONV_ARGB_TO_YV12_POPPTR \
	src   = saved_src;   \
	dst_y = saved_dst_y; \
	dst_u = saved_dst_u; \
	dst_v = saved_dst_v;

#define MMSFB_CONV_ARGB_TO_YV12_PIXEL(src, dst_y, dst_u, dst_v, d_u, d_v) \
	SRC = src;													\
	A = SRC >> 24;												\
																\
	if (A == 0xff) {											\
		if (SRC!=OLDSRC) {										\
			int r = (SRC >> 16) & 0xff;							\
			int g = (SRC >> 8) & 0xff;							\
			int b = SRC  & 0xff;								\
			old_y = MMSFB_CONV_RGB2Y(r,g,b);					\
			old_u = MMSFB_CONV_RGB2U(r,g,b);					\
			old_v = MMSFB_CONV_RGB2V(r,g,b);					\
			OLDSRC = SRC;										\
		}														\
		dst_y = old_y;											\
		d_u     old_u;											\
		d_v     old_v;											\
	}															\
	else														\
	if (!A) {													\
		dst_y = 0;												\
		d_u 128;												\
		d_v 128;												\
	}															\
	else														\
	{															\
		if (SRC!=OLDSRC) {										\
			int r = (SRC >> 16) & 0xff;							\
			int g = (SRC >> 8) & 0xff;							\
			int b = SRC  & 0xff;								\
			old_y = MMSFB_CONV_RGB2Y(r,g,b);					\
			old_u = MMSFB_CONV_RGB2U(r,g,b);					\
			old_v = MMSFB_CONV_RGB2V(r,g,b);					\
			MMSFB_CONV_PREPARE_YUVBLEND(old_y, old_u, old_v); 	\
			old_y = (A * old_y) >> 8;  							\
			old_u = (A * old_u) >> 8; 							\
			old_v = (A * old_v) >> 8; 							\
			MMSFB_CONV_RESET_YUVBLEND(old_y, old_u, old_v); 	\
			OLDSRC = SRC;										\
		}														\
																\
	    dst_y = old_y;											\
		d_u     old_u;											\
		d_v     old_v;											\
	}



#define MMSFB_CONV_BLEND_ARGB_TO_YV12_PUSHPTR \
	unsigned int  *saved_src   = src;   \
	unsigned char *saved_dst_y = dst_y; \
	unsigned char *saved_dst_u = dst_u; \
	unsigned char *saved_dst_v = dst_v;

#define MMSFB_CONV_BLEND_ARGB_TO_YV12_POPPTR \
	src   = saved_src;   \
	dst_y = saved_dst_y; \
	dst_u = saved_dst_u; \
	dst_v = saved_dst_v;








#define MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL(src, dst_y, dst_u, dst_v, d_u, d_v) \
	SRC = src;											\
	A = SRC >> 24;										\
														\
	if (A == 0xff) {									\
		if (SRC!=OLDSRC) {								\
			int r = (SRC >> 16) & 0xff;					\
			int g = (SRC >> 8) & 0xff;					\
			int b = SRC  & 0xff;						\
			old_y = MMSFB_CONV_RGB2Y(r,g,b);			\
			old_u = MMSFB_CONV_RGB2U(r,g,b);			\
			old_v = MMSFB_CONV_RGB2V(r,g,b);			\
			OLDSRC = SRC;								\
		}												\
		dst_y = old_y;									\
		d_u     old_u;									\
		d_v     old_v;									\
	}													\
	else												\
	if (!A) {											\
		d_u dst_u;										\
		d_v dst_v;										\
	}													\
	else												\
	{													\
		if (SRC!=OLDSRC) {								\
			int r = (SRC >> 16) & 0xff;					\
			int g = (SRC >> 8) & 0xff;					\
			int b = SRC  & 0xff;						\
			old_y = A * MMSFB_CONV_RGB2Y(r,g,b);		\
			old_u = A * MMSFB_CONV_RGB2U(r,g,b);		\
			old_v = A * MMSFB_CONV_RGB2V(r,g,b);		\
			OLDSRC = SRC;								\
		}												\
														\
		register unsigned int SA = 0x100 - A;			\
		unsigned int y = dst_y;							\
		unsigned int u = dst_u;							\
		unsigned int v = dst_v;							\
														\
	    y = SA * y + old_y;								\
	    u = SA * u + old_u;								\
	    v = SA * v + old_v;								\
	    												\
	    dst_y = y >> 8;									\
		d_u     u >> 8;									\
		d_v     v >> 8;									\
	}



#define MMSFB_CONV_BLEND_SRCALPHA_ARGB_TO_YV12_PIXEL(src, dst_y, dst_u, dst_v, d_u, d_v) \
	SRC = src;											\
	A = SRC >> 24;										\
														\
	if (!A) {											\
		d_u dst_u;										\
		d_v dst_v;										\
	}													\
	else												\
	{													\
		if (SRC!=OLDSRC) {								\
			int r = (ALPHA * (SRC & 0xff0000)) >> 24;	\
			int g = (ALPHA * (SRC & 0xff00)) >> 16;		\
			int b = (ALPHA * (SRC & 0xff)) >> 8;		\
			old_y = A * MMSFB_CONV_RGB2Y(r,g,b);		\
			A = (ALPHA * A) >> 8;						\
			old_u = A * MMSFB_CONV_RGB2U(r,g,b);		\
			old_v = A * MMSFB_CONV_RGB2V(r,g,b);		\
			OLDSRC = SRC;								\
		}												\
		else											\
			A = (ALPHA * A) >> 8;						\
														\
		register unsigned int SA = 0x100 - A;			\
		unsigned int y = dst_y;							\
		unsigned int u = dst_u;							\
		unsigned int v = dst_v;							\
														\
	    y = SA * y + old_y;								\
	    u = SA * u + old_u;								\
	    v = SA * v + old_v;								\
	    												\
	    dst_y = y >> 8;									\
		d_u     u >> 8;									\
		d_v     v >> 8;									\
	}



//! Blit RGB24 to YV12.
/*!
\author Jens Schneider
*/
void mmsfb_blit_rgb24_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
							  unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy);


//! Blit ARGB to YV12.
/*!
\author Jens Schneider
*/
void mmsfb_blit_argb_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
							 unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy);


//! Blit with alpha blending ARGB to YV12.
/*!
\author Jens Schneider
*/
void mmsfb_blit_blend_argb_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
								   unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy);


//! Stretch blit YV12 to YV12 with antialiasing.
/*!
\author Jens Schneider
*/
void mmsfb_stretchblit_yv12_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh);



#endif /* MMSFBCONV_H_ */
