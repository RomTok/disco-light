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

//#define MMSFB_CONV_RGB2Y(r,g,b) ((((66*r+129*g+25*b+128)>>8)+16) & 0xff)
//#define MMSFB_CONV_RGB2U(r,g,b) ((((-38*r-74*g+112*b+128)>>8)+128) & 0xff)
//#define MMSFB_CONV_RGB2V(r,g,b) ((((112*r-94*g-18*b+128)>>8)+128) & 0xff)
#define MMSFB_CONV_RGB2Y(r,g,b) (((66*r+129*g+25*b+128)>>8)+16)
#define MMSFB_CONV_RGB2U(r,g,b) (((-38*r-74*g+112*b+128)>>8)+128)
#define MMSFB_CONV_RGB2V(r,g,b) (((112*r-94*g-18*b+128)>>8)+128)

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

/*
typedef int v4si __attribute__ ((vector_size (8)));
typedef long long unsigned int v4sia __attribute__ ((vector_size (8)));
typedef short int v4six __attribute__ ((vector_size (8)));
typedef signed char v4siy __attribute__ ((vector_size (8)));

typedef union
{
v4si v;
v4sia va;
v4six vv;
v4siy vvv;
short s[4];
int i[2];
unsigned char c[8];
} _v4si;
*/

#ifdef __HAVE_SSE__

typedef int v4si[2];
typedef short int v4six[4];
typedef signed char v4siy[8];

typedef union
{
v4si v;
v4six vv;
v4siy vvv;
short s[4];
int i[2];
unsigned char c[8];
} _v4si;

#endif

//#ifdef __SSE2__
//#ifdef __MMX__

/*AA = __builtin_ia32_psadbw(A, TTT); \
if (!AA) { \
*/
/*
#define MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL_MMX(src, dst_y1, dst_y2, dst_u, dst_v, d_u, d_v) \
	if ((src->c[3]==0xff)&&(src->c[7]==0xff)) { \
		if ((src->i[0] != OLDSRC_MMX.i[0])&&(src->i[1] != OLDSRC_MMX.i[1])) { \
			x1 = __builtin_ia32_pand(src->v, X1); \
			x2 = __builtin_ia32_psrlw(src->v, 8); \
			_v4si yy; \
			yy.vv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
			yy.vv = __builtin_ia32_paddd(__builtin_ia32_psrld(yy.vv, 8), YY); \
			old_y1 = yy.c[0]; \
			old_y2 = yy.c[4]; \
			ou = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
			ou = __builtin_ia32_paddd(__builtin_ia32_psrld(ou, 8), UV); \
			old_u = __builtin_ia32_psadbw(ou, TTT); \
			ov = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
			ov = __builtin_ia32_paddd(__builtin_ia32_psrld(ov, 8), UV); \
			old_v = __builtin_ia32_psadbw(ov, TTT); \
			OLDSRC_MMX = *src;								\
		} \
		dst_y1 = old_y1; \
		dst_y2 = old_y2; \
		d_u old_u; \
		d_v old_v; \
	} \
	else \
	if ((!src->c[3])&&(!src->c[7])) { \
		d_u (dst_u << 1);										\
		d_v (dst_v << 1);										\
	} \
	else { \
		if ((src->i[0] != OLDSRC_MMX.i[0])&&(src->i[1] != OLDSRC_MMX.i[1])) { \
			x1 = __builtin_ia32_pand(src->v, X1); \
			x2 = __builtin_ia32_psrlw(src->v, 8); \
			A  = __builtin_ia32_psrld(x2, 16); \
			oy = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
			oy = __builtin_ia32_paddd(__builtin_ia32_psrld(oy, 8), YY); \
			oy = __builtin_ia32_pmullw(oy, A); \
			ou = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
			ou = __builtin_ia32_paddd(__builtin_ia32_psrld(ou, 8), UV); \
			ou = __builtin_ia32_pmullw(ou, A); \
			ov = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
			ov = __builtin_ia32_paddd(__builtin_ia32_psrld(ov, 8), UV); \
			ov = __builtin_ia32_pmullw(ov, A); \
			A = __builtin_ia32_psubd(TTTT, A); \
			OLDSRC_MMX = *src;								\
		} \
		{ \
		_v4si yy; yy.i[0] = dst_y1; yy.i[1] = dst_y2; \
		_v4si uu; uu.i[0] = uu.i[1] = dst_u; \
		_v4si vv; vv.i[0] = vv.i[1] = dst_v; \
		\
		v4six qq; \
		qq = __builtin_ia32_pmullw(yy.vv, A); \
		qq = __builtin_ia32_paddw(qq, oy); \
		yy.vv = __builtin_ia32_psrlw(qq, 8); \
		dst_y1 = yy.c[0]; dst_y2 = yy.c[4]; \
		\
		qq = __builtin_ia32_pmullw(uu.vv, A); \
		qq = __builtin_ia32_paddw(qq, ou); \
		qq = __builtin_ia32_psrlw(qq, 8); \
		d_u  __builtin_ia32_psadbw(qq, TTT); \
		\
		qq = __builtin_ia32_pmullw(vv.vv, A); \
		qq = __builtin_ia32_paddw(qq, ov); \
		qq = __builtin_ia32_psrlw(qq, 8); \
		d_v  __builtin_ia32_psadbw(qq, TTT); \
		} \
	}

*/
//	     __asm__ __volatile__ ( "mov %%eax,%%ebx" : [x] "=m" (x));

/*
#define MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL_MMX(src, dst_y1, dst_y2, dst_u, dst_v, d_u, d_v) \
	if ((src->c[3]==0xff)&&(src->c[7]==0xff)) { \
		if ((src->i[0] != OLDSRC_MMX.i[0])&&(src->i[1] != OLDSRC_MMX.i[1])) { \
			x1 = __builtin_ia32_pand(src->v, X1); \
			x2 = __builtin_ia32_psrlw(src->v, 8); \
			_v4si yy; \
			yy.vv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
			yy.vv = __builtin_ia32_paddd(__builtin_ia32_psrld(yy.vv, 8), YY); \
			old_y1 = yy.c[0]; \
			old_y2 = yy.c[4]; \
			ou = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
			ou = __builtin_ia32_paddd(__builtin_ia32_psrld(ou, 8), UV); \
			old_u = __builtin_ia32_psadbw(ou, TTT); \
			ov = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
			ov = __builtin_ia32_paddd(__builtin_ia32_psrld(ov, 8), UV); \
			old_v = __builtin_ia32_psadbw(ov, TTT); \
			OLDSRC_MMX = *src;								\
		} \
		dst_y1 = old_y1; \
		dst_y2 = old_y2; \
		d_u old_u; \
		d_v old_v; \
	} \
	else \
	if ((!src->c[3])&&(!src->c[7])) { \
		d_u (dst_u << 1);										\
		d_v (dst_v << 1);										\
	} \
	else { \
		if ((src->i[0] != OLDSRC_MMX.i[0])&&(src->i[1] != OLDSRC_MMX.i[1])) { \
			x1 = __builtin_ia32_pand(src->v, X1); \
			x2 = __builtin_ia32_psrlw(src->v, 8); \
			A  = __builtin_ia32_psrld(x2, 16); \
			oy = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
			oy = __builtin_ia32_paddd(__builtin_ia32_psrld(oy, 8), YY); \
			oy = __builtin_ia32_pmullw(oy, A); \
			ou = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
			ou = __builtin_ia32_paddd(__builtin_ia32_psrld(ou, 8), UV); \
			ou = __builtin_ia32_pmullw(ou, A); \
			ov = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
			ov = __builtin_ia32_paddd(__builtin_ia32_psrld(ov, 8), UV); \
			ov = __builtin_ia32_pmullw(ov, A); \
			A = __builtin_ia32_psubd(TTTT, A); \
			OLDSRC_MMX = *src;								\
		} \
		{ \
		_v4si yy; yy.i[0] = dst_y1; yy.i[1] = dst_y2; \
		_v4si uu; uu.i[0] = uu.i[1] = dst_u; \
		_v4si vv; vv.i[0] = vv.i[1] = dst_v; \
		\
		v4six qq; \
		qq = __builtin_ia32_pmullw(yy.vv, A); \
		qq = __builtin_ia32_paddw(qq, oy); \
		yy.vv = __builtin_ia32_psrlw(qq, 8); \
		dst_y1 = yy.c[0]; dst_y2 = yy.c[4]; \
		\
		qq = __builtin_ia32_pmullw(uu.vv, A); \
		qq = __builtin_ia32_paddw(qq, ou); \
		qq = __builtin_ia32_psrlw(qq, 8); \
		d_u  __builtin_ia32_psadbw(qq, TTT); \
		\
		qq = __builtin_ia32_pmullw(vv.vv, A); \
		qq = __builtin_ia32_paddw(qq, ov); \
		qq = __builtin_ia32_psrlw(qq, 8); \
		d_v  __builtin_ia32_psadbw(qq, TTT); \
		} \
	}
*/
/*
#define MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL_MMX(src, dst_y1, dst_y2, dst_u, dst_v, d_u, d_v) \
	if ((src->c[3]==0xff)&&(src->c[7]==0xff)) { \
		if ((src->i[0] != OLDSRC_MMX.i[0])&&(src->i[1] != OLDSRC_MMX.i[1])) { \
			x1 = __builtin_ia32_pand(src->v, X1); \
			x2 = __builtin_ia32_psrlw(src->v, 8); \
			_v4si yy; \
			yy.vv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
			yy.vv = __builtin_ia32_paddd(__builtin_ia32_psrld(yy.vv, 8), YY); \
			old_y1 = yy.c[0]; \
			old_y2 = yy.c[4]; \
			ou = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
			ou = __builtin_ia32_paddd(__builtin_ia32_psrld(ou, 8), UV); \
			old_u = __builtin_ia32_psadbw(ou, TTT); \
			ov = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
			ov = __builtin_ia32_paddd(__builtin_ia32_psrld(ov, 8), UV); \
			old_v = __builtin_ia32_psadbw(ov, TTT); \
			OLDSRC_MMX = *src;								\
		} \
		dst_y1 = old_y1; \
		dst_y2 = old_y2; \
		d_u old_u; \
		d_v old_v; \
	} \
	else \
	if ((!src->c[3])&&(!src->c[7])) { \
		d_u (dst_u << 1);										\
		d_v (dst_v << 1);										\
	} \
	else { \
			x1 = __builtin_ia32_pand(src->v, X1); \
			x2 = __builtin_ia32_psrlw(src->v, 8); \
			A  = __builtin_ia32_psrld(x2, 16); \
			oy = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
			oy = __builtin_ia32_paddd(__builtin_ia32_psrld(oy, 8), YY); \
			oy = __builtin_ia32_pmullw(oy, A); \
			ou = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
			ou = __builtin_ia32_paddd(__builtin_ia32_psrld(ou, 8), UV); \
			ou = __builtin_ia32_pmullw(ou, A); \
			ov = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
			ov = __builtin_ia32_paddd(__builtin_ia32_psrld(ov, 8), UV); \
			ov = __builtin_ia32_pmullw(ov, A); \
			A = __builtin_ia32_psubd(TTTT, A); \
		{ \
		_v4si yy; yy.i[0] = dst_y1; yy.i[1] = dst_y2; \
		_v4si uu; uu.i[0] = dst_u; \
		_v4si vv; vv.i[0] = dst_v; \
		\
		yy.vv = __builtin_ia32_psrlw(__builtin_ia32_paddw(__builtin_ia32_pmullw(yy.vv, A), oy), 8); \
		dst_y1 = yy.c[0]; dst_y2 = yy.c[4]; \
		\
		d_u  __builtin_ia32_psadbw(__builtin_ia32_psrlw(__builtin_ia32_paddw(__builtin_ia32_pmullw(__builtin_ia32_pshufw(uu.vv, 68), A), ou), 8), TTT); \
		d_v  __builtin_ia32_psadbw(__builtin_ia32_psrlw(__builtin_ia32_paddw(__builtin_ia32_pmullw(__builtin_ia32_pshufw(vv.vv, 68), A), ov), 8), TTT); \
		} \
	}
*/

/*

#define MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL_MMX(src, dst_y1, dst_y2, dst_u, dst_v, d_u, d_v) \
	if ((src->c[3]==0xff)&&(src->c[7]==0xff)) { \
		SRC = src->v; \
		if (SRC!=OLDSRC_MMX) {								\
			x1 = __builtin_ia32_pand(SRC, X1); \
			x2 = __builtin_ia32_psrlw(SRC, 8); \
			_v4si yy; \
			yy.vv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
			yy.vv = __builtin_ia32_paddd(__builtin_ia32_psrld(yy.vv, 8), YY); \
			old_y1 = yy.c[0]; \
			old_y2 = yy.c[4]; \
			u = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
			u = __builtin_ia32_paddd(__builtin_ia32_psrld(u, 8), UV); \
			old_u = __builtin_ia32_psadbw(u, TTT); \
			v = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
			v = __builtin_ia32_paddd(__builtin_ia32_psrld(v, 8), UV); \
			old_v = d_v __builtin_ia32_psadbw(v, TTT); \
			OLDSRC_MMX = SRC;								\
		} \
		dst_y1 = old_y1; \
		dst_y2 = old_y2; \
		d_u old_u; \
		d_v old_v; \
	} \
	else \
	if ((!src->c[3])&&(!src->c[7])) { \
		d_u (dst_u << 1);										\
		d_v (dst_v << 1);										\
	} \
	else { \
		SRC = src->v; \
		x1 = __builtin_ia32_pand(SRC, X1); \
		x2 = __builtin_ia32_psrlw(SRC, 8); \
		A  = __builtin_ia32_psrld(x2, 16); \
		y = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
		y = __builtin_ia32_paddd(__builtin_ia32_psrld(y, 8), YY); \
		y = __builtin_ia32_pmullw(y, A); \
		u = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
		u = __builtin_ia32_paddd(__builtin_ia32_psrld(u, 8), UV); \
		u = __builtin_ia32_pmullw(u, A); \
		v = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
		v = __builtin_ia32_paddd(__builtin_ia32_psrld(v, 8), UV); \
		v = __builtin_ia32_pmullw(v, A); \
		{ \
		_v4si yy; yy.i[0] = dst_y1; yy.i[1] = dst_y2; \
		_v4si uu; uu.i[0] = dst_u; \
		_v4si vv; vv.i[0] = dst_v; \
		\
		A = __builtin_ia32_psubd(TTTT, A); \
		yy.vv = __builtin_ia32_psrlw(__builtin_ia32_paddw(__builtin_ia32_pmullw(yy.vv, A), y), 8); \
		dst_y1 = yy.c[0]; dst_y2 = yy.c[4]; \
		\
		d_u  __builtin_ia32_psadbw(__builtin_ia32_psrlw(__builtin_ia32_paddw(__builtin_ia32_pmullw(__builtin_ia32_pshufw(uu.vv, 68), A), u), 8), TTT); \
		d_v  __builtin_ia32_psadbw(__builtin_ia32_psrlw(__builtin_ia32_paddw(__builtin_ia32_pmullw(__builtin_ia32_pshufw(vv.vv, 68), A), v), 8), TTT); \
		} \
	}
*/

/*
#define MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL_MMX(src, dst_y1, dst_y2, dst_u, dst_v, d_u, d_v) \
	SRC = src->v; \
	x1 = __builtin_ia32_pand(SRC, X1); \
	x2 = __builtin_ia32_psrlw(SRC, 8); \
	A  = __builtin_ia32_psrld(x2, 16); \
	y.vv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
	y.vv = __builtin_ia32_paddd(__builtin_ia32_psrld(y.vv, 8), YY); \
	y.vv = __builtin_ia32_pmullw(y.vv, A); \
	u = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
	u = __builtin_ia32_paddd(__builtin_ia32_psrld(u, 8), UV); \
	u = __builtin_ia32_pmullw(u, A); \
	v = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
	v = __builtin_ia32_paddd(__builtin_ia32_psrld(v, 8), UV); \
	v = __builtin_ia32_pmullw(v, A); \
	{ \
	_v4si yy; \
	yy.i[0] = dst_y1; \
	yy.i[1] = dst_y2; \
	_v4si uu; \
	uu.i[0] = dst_u; \
	_v4si vv; \
	vv.i[0] = dst_v; \
	\
	A = __builtin_ia32_psubd(TTTT, A); \
	yy.vv = __builtin_ia32_psrlw(__builtin_ia32_paddw(__builtin_ia32_pmullw(yy.vv, A), y.vv), 8); \
	dst_y1 = yy.c[0]; \
	dst_y2 = yy.c[4]; \
	\
	d_u  __builtin_ia32_psadbw(__builtin_ia32_psrlw(__builtin_ia32_paddw(__builtin_ia32_pmullw(__builtin_ia32_pshufw(uu.vv, 68), A), u), 8), TTT); \
	d_v  __builtin_ia32_psadbw(__builtin_ia32_psrlw(__builtin_ia32_paddw(__builtin_ia32_pmullw(__builtin_ia32_pshufw(vv.vv, 68), A), v), 8), TTT); \
    }
*/


/*
#define MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL_MMX(src, dst_y1, dst_y2, dst_u, dst_v, d_u, d_v) \
	SRC = src->v; \
	x1 = __builtin_ia32_pand(SRC, X1); \
	x2 = __builtin_ia32_psrlw(SRC, 8); \
	A.vv = __builtin_ia32_psrld(x2, 16); \
	y.vv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
	y.vv = __builtin_ia32_paddd(__builtin_ia32_psrld(y.vv, 8), YY); \
	y.vv = __builtin_ia32_pmullw(y.vv, A.vv); \
	u = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
	u = __builtin_ia32_paddd(__builtin_ia32_psrld(u, 8), UV); \
	u = __builtin_ia32_pmullw(u, A.vv); \
	v = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
	v = __builtin_ia32_paddd(__builtin_ia32_psrld(v, 8), UV); \
	v = __builtin_ia32_pmullw(v, A.vv); \
	{ \
	_v4si yy; \
	yy.i[0] = dst_y1; \
	yy.i[1] = dst_y2; \
	_v4si uu; \
	uu.i[0] = dst_u; \
	uu.i[1] = dst_u; \
	_v4si vv; \
	vv.i[0] = dst_v; \
	vv.i[1] = dst_v; \
	_v4si SA; \
	SA.vv = __builtin_ia32_psubd(TTTT, A.vv); \
	yy.vv = __builtin_ia32_pmullw(yy.vv, SA.vv); \
	uu.vv = __builtin_ia32_pmullw(uu.vv, SA.vv); \
	vv.vv = __builtin_ia32_pmullw(vv.vv, SA.vv); \
	yy.vv = __builtin_ia32_paddw(yy.vv, y.vv); \
	uu.vv = __builtin_ia32_paddw(uu.vv, u); \
	vv.vv = __builtin_ia32_paddw(vv.vv, v); \
	yy.vv = __builtin_ia32_psrlw(yy.vv, 8); \
	uu.vv = __builtin_ia32_psrlw(uu.vv, 8); \
	vv.vv = __builtin_ia32_psrlw(vv.vv, 8); \
	dst_y1 = yy.c[0]; \
	dst_y2 = yy.c[4]; \
	d_u  __builtin_ia32_psadbw(uu.vv, TTT); \
	d_v  __builtin_ia32_psadbw(vv.vv, TTT); \
    }
*/

/*
#define MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL_MMX(src, dst_y1, dst_y2, dst_u, dst_v, d_u, d_v) \
	SRC = src->v; \
	x1 = __builtin_ia32_pand(SRC, X1); \
	x2 = __builtin_ia32_psrlw(SRC, 8); \
	y.vv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
	y.vv = __builtin_ia32_paddd(__builtin_ia32_psrld(y.vv, 8), YY); \
	dst_y1 = y.c[0]; \
	dst_y2 = y.c[4]; \
	uv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
	uv = __builtin_ia32_paddd(__builtin_ia32_psrld(uv, 8), UV); \
	d_u __builtin_ia32_psadbw(uv, TTT); \
	uv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
	uv = __builtin_ia32_paddd(__builtin_ia32_psrld(uv, 8), UV); \
	d_v __builtin_ia32_psadbw(uv, TTT);

 */


/*
#define MMSFB_CONV_BLEND_ARGB_TO_YV12_PIXEL_MMX(src, dst_y1, dst_y2, dst_u, dst_v, d_u, d_v) \
 { \
	register v4si t = src->v; \
	register v4six x1, x2; \
	x1 = __builtin_ia32_pand(t, X1); \
	x2 = __builtin_ia32_psrlw(t, 8); \
	_v4si y; \
	y.vv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, Y_RBRB), __builtin_ia32_pmaddwd(x2, Y_GG)); \
	y.vv = __builtin_ia32_pand(__builtin_ia32_paddd(__builtin_ia32_psrld(y.vv, 8), YY), X1); \
	dst_y1 = y.c[0]; \
	dst_y2 = y.c[4]; \
	_v4si u; \
	u.vv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, U_RBRB), __builtin_ia32_pmaddwd(x2, U_GG)); \
	u.vv = __builtin_ia32_pand(__builtin_ia32_paddd(__builtin_ia32_psrld(u.vv, 8), UV), X1); \
	d_u (u.s[0] + u.s[2]); \
	_v4si v; \
	v.vv = __builtin_ia32_paddd(__builtin_ia32_pmaddwd(x1, V_RBRB), __builtin_ia32_pmaddwd(x2, V_GG)); \
	v.vv = __builtin_ia32_pand(__builtin_ia32_paddd(__builtin_ia32_psrld(v.vv, 8), UV), X1); \
	d_v (v.s[0] + v.s[2]); }
*/

/*

 \
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
*/


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
