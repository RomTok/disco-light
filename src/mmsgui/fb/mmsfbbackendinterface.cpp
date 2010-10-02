/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
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

#include "mmsgui/fb/mmsfbbackendinterface.h"
#include "mmstools/tools.h"

#define BEI_SOURCE_WIDTH	((!req->source->is_sub_surface)?req->source->config.w:req->source->root_parent->config.w)
#define BEI_SOURCE_HEIGHT	((!req->source->is_sub_surface)?req->source->config.h:req->source->root_parent->config.h)

#define BEI_SURFACE_WIDTH		((!req->surface->is_sub_surface)?req->surface->config.w:req->surface->root_parent->config.w)
#define BEI_SURFACE_HEIGHT		((!req->surface->is_sub_surface)?req->surface->config.h:req->surface->root_parent->config.h)
#define BEI_SURFACE_BOTTOM		(BEI_SURFACE_HEIGHT - 1)
#define BEI_SURFACE_BOTTOM_F	(float)BEI_SURFACE_BOTTOM

#ifdef __HAVE_GL2__

#define INIT_OGL_FBO(surface) \
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surface->config.surface_buffer->ogl_fbo); \
	if (!surface->is_sub_surface) oglMatrix(surface->config.w, surface->config.h); \
	else oglMatrix(surface->root_parent->config.w, surface->root_parent->config.h); \
	glDisable(GL_SCISSOR_TEST);

#define INIT_OGL_FBOXX(surface) \
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surface->config.surface_buffer->ogl_fbo); \
	if (!surface->is_sub_surface) oglMatrixXX(surface->config.w, surface->config.h); \
	else oglMatrixXX(surface->root_parent->config.w, surface->root_parent->config.h); \
	glDisable(GL_SCISSOR_TEST);

#define INIT_OGL_DRAWING(surface) { \
			switch (surface->config.drawingflags) { \
			case MMSFB_DRAW_BLEND: \
				glEnable(GL_BLEND); \
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); \
				break; \
			default: \
				glDisable(GL_BLEND); \
				break; } \
			glColor4ub(surface->config.color.r, surface->config.color.g, surface->config.color.b, surface->config.color.a); }

#define INIT_OGL_BLITTING(surface) \
		switch (surface->config.blittingflags) { \
		case MMSFB_BLIT_BLEND_ALPHACHANNEL:	\
			glEnable(GL_BLEND); \
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); \
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); \
			break; \
		case MMSFB_BLIT_BLEND_COLORALPHA: \
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); \
			glColor4ub(255, 255, 255, surface->config.color.a); \
			break; \
		case MMSFB_BLIT_BLEND_ALPHACHANNEL + MMSFB_BLIT_BLEND_COLORALPHA: \
			glEnable(GL_BLEND); \
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); \
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); \
			glColor4ub(255, 255, 255, surface->config.color.a); \
			break; \
		case MMSFB_BLIT_COLORIZE: \
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); \
			glColor4ub(surface->config.color.r, surface->config.color.g, surface->config.color.b, 0xff); \
			break; \
		case MMSFB_BLIT_BLEND_ALPHACHANNEL + MMSFB_BLIT_COLORIZE: \
			glEnable(GL_BLEND); \
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); \
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); \
			glColor4ub(surface->config.color.r, surface->config.color.g, surface->config.color.b, 0xff); \
			break; \
		case MMSFB_BLIT_BLEND_COLORALPHA + MMSFB_BLIT_COLORIZE: \
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); \
			glColor4ub(surface->config.color.r, surface->config.color.g, surface->config.color.b, surface->config.color.a); \
			break; \
		case MMSFB_BLIT_BLEND_ALPHACHANNEL + MMSFB_BLIT_BLEND_COLORALPHA + MMSFB_BLIT_COLORIZE: \
			glEnable(GL_BLEND); \
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); \
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); \
			glColor4ub(surface->config.color.r, surface->config.color.g, surface->config.color.b, surface->config.color.a); \
			break; \
		default: \
			glDisable(GL_BLEND); \
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); \
			break; }

#define GET_OFFS(surface) \
		int xoff = 0; int yoff = 0; \
		if (surface->is_sub_surface) { \
			xoff = surface->sub_surface_xoff; \
			yoff = surface->sub_surface_yoff; }

#define GET_OFFS_SRC(surface) \
		int src_xoff = 0; int src_yoff = 0; \
		if (surface->is_sub_surface) { \
			src_xoff = surface->sub_surface_xoff; \
			src_yoff = surface->sub_surface_yoff; }

#define OGL_GLSCISSOR(X, Y, W, H) glScissor(X, BEI_SURFACE_HEIGHT - (H) - (Y), W, H)

#define OGL_DRAW_POINT(x, y) { glBegin(GL_POINTS); glVertex2f((float)(x) + 0.5, (float)(BEI_SURFACE_BOTTOM - (y)) + 0.5); glEnd(); }

#define OGL_SINGLE_POINT_FALLBACK(x1, y1, x2, y2) \
		if (x1 == x2 && y1 == y2) OGL_DRAW_POINT(x1, y1) else

//TODO: am dreieck nochwas???
#define OGL_SINGLE_POINT_FALLBACK2(x1, y1, x2, y2, x3, y3) \
		if (x1 == x2 && x1 == x3 && y1 == y2 && y1 == y3) OGL_DRAW_POINT(x1, y1) else



#define OGL_CALC_2X(v1, v2) (((v1)<(v2)) ? (float)(v1) : (float)(v1) + 0.99)
#define OGL_CALC_2Y(v1, v2) OGL_CALC_2Y_H(v1, v2, BEI_SURFACE_HEIGHT)
#define OGL_CALC_2Y_H(v1, v2, height) (((v1)<(v2)) ? (float)(height-1) - (float)(v1) + 0.99 : (float)(height-1) - (float)(v1))

#define OGL_CALC_3X(v1, v2, v3) (((v1)<=(v2) && (v1)<=(v3)) ? (float)(v1) : ((v1)>(v2) && (v1)>(v3)) ? (float)(v1) + 0.99 : (float)(v1) + 0.5)
#define OGL_CALC_3Y(v1, v2, v3) (((v1)<=(v2) && (v1)<=(v3)) ? BEI_SURFACE_BOTTOM_F - (float)(v1) + 0.99 : ((v1)>(v2) && (v1)>(v3)) ? BEI_SURFACE_BOTTOM_F - (float)(v1) : BEI_SURFACE_BOTTOM_F - (float)(v1) + 0.5)

#define OGL_CALC_2X_N(v1, v2, width)	(OGL_CALC_2X(v1, v2) / (width))
#define OGL_CALC_2Y_N(v1, v2, height)	(OGL_CALC_2Y_H(v1, v2, height) / (height))

#define OGL_FILL_RECTANGLE(x1, y1, x2, y2) \
		OGL_SINGLE_POINT_FALLBACK(x1, y1, x2, y2) \
		glRectf(OGL_CALC_2X(x1, x2), OGL_CALC_2Y(y1, y2), OGL_CALC_2X(x2, x1), OGL_CALC_2Y(y2, y1));


#define OGL_DRAW_LINE(x1, y1, x2, y2) \
		OGL_SINGLE_POINT_FALLBACK(x1, y1, x2, y2) { \
		glBegin(GL_LINES); \
			glVertex2f(OGL_CALC_2X(x1, x2), OGL_CALC_2Y(y1, y2)); \
			glVertex2f(OGL_CALC_2X(x2, x1), OGL_CALC_2Y(y2, y1)); \
		glEnd(); }

#define OGL_DRAW_RECTANGLE(x1, y1, x2, y2) \
		OGL_SINGLE_POINT_FALLBACK(x1, y1, x2, y2) { \
		glBegin(GL_LINE_STRIP); \
			glVertex2f(OGL_CALC_2X(x1, x2), OGL_CALC_2Y(y1, y2)); \
			glVertex2f(OGL_CALC_2X(x2, x1), OGL_CALC_2Y(y1, y2)); \
			glVertex2f(OGL_CALC_2X(x2, x1), OGL_CALC_2Y(y2, y1)); \
			glVertex2f(OGL_CALC_2X(x1, x2), OGL_CALC_2Y(y2, y1)); \
			glVertex2f(OGL_CALC_2X(x1, x2), OGL_CALC_2Y(y1, y2)); \
		glEnd(); }

#define OGL_FILL_TRIANGLE(x1, y1, x2, y2, x3, y3) \
		OGL_SINGLE_POINT_FALLBACK2(x1, y1, x2, y2, x3, y3) { \
		glBegin(GL_TRIANGLES); \
			glVertex2f(OGL_CALC_3X(x1, x2, x3), OGL_CALC_3Y(y1, y2, y3)); \
			glVertex2f(OGL_CALC_3X(x2, x3, x1), OGL_CALC_3Y(y2, y3, y1)); \
			glVertex2f(OGL_CALC_3X(x3, x1, x2), OGL_CALC_3Y(y3, y1, y2)); \
		glEnd(); }

#define OGL_DRAW_TRIANGLE(x1, y1, x2, y2, x3, y3) \
		OGL_SINGLE_POINT_FALLBACK2(x1, y1, x2, y2, x3, y3) { \
		glBegin(GL_LINE_STRIP); \
			glVertex2f(OGL_CALC_3X(x1, x2, x3), OGL_CALC_3Y(y1, y2, y3)); \
			glVertex2f(OGL_CALC_3X(x2, x3, x1), OGL_CALC_3Y(y2, y3, y1)); \
			glVertex2f(OGL_CALC_3X(x3, x1, x2), OGL_CALC_3Y(y3, y1, y2)); \
			glVertex2f(OGL_CALC_3X(x1, x2, x3), OGL_CALC_3Y(y1, y2, y3)); \
		glEnd(); }

#define OGL_STRETCH_BLIT(sx1, sy1, sx2, sy2, sw, sh, dx1, dy1, dx2, dy2) \
		glBegin(GL_QUADS); \
		if (dx1 != dx2 && dy1 != dy2) { \
			glTexCoord2f(OGL_CALC_2X_N(sx1, sx2, sw), OGL_CALC_2Y_N(sy1, sy2, sh)); \
				glVertex2f(OGL_CALC_2X(dx1, dx2), OGL_CALC_2Y(dy1, dy2)); \
			glTexCoord2f(OGL_CALC_2X_N(sx2, sx1, sw), OGL_CALC_2Y_N(sy1, sy2, sh)); \
				glVertex2f(OGL_CALC_2X(dx2, dx1), OGL_CALC_2Y(dy1, dy2)); \
			glTexCoord2f(OGL_CALC_2X_N(sx2, sx1, sw), OGL_CALC_2Y_N(sy2, sy1, sh)); \
				glVertex2f(OGL_CALC_2X(dx2, dx1), OGL_CALC_2Y(dy2, dy1)); \
			glTexCoord2f(OGL_CALC_2X_N(sx1, sx2, sw), OGL_CALC_2Y_N(sy2, sy1, sh)); \
				glVertex2f(OGL_CALC_2X(dx1, dx2), OGL_CALC_2Y(dy2, dy1)); \
		} else if (dy1 != dy2) { \
			glTexCoord2f(OGL_CALC_2X_N(sx1, sx2, sw), OGL_CALC_2Y_N(sy1, sy2, sh)); \
				glVertex2f((float)(dx1),		OGL_CALC_2Y(dy1, dy2)); \
			glTexCoord2f(OGL_CALC_2X_N(sx2, sx1, sw), OGL_CALC_2Y_N(sy1, sy2, sh)); \
				glVertex2f((float)(dx1) + 0.99,	OGL_CALC_2Y(dy1, dy2)); \
			glTexCoord2f(OGL_CALC_2X_N(sx2, sx1, sw), OGL_CALC_2Y_N(sy2, sy1, sh)); \
				glVertex2f((float)(dx1) + 0.99,	OGL_CALC_2Y(dy2, dy1)); \
			glTexCoord2f(OGL_CALC_2X_N(sx1, sx2, sw), OGL_CALC_2Y_N(sy2, sy1, sh)); \
				glVertex2f((float)(dx1),		OGL_CALC_2Y(dy2, dy1)); \
		} else if (dx1 != dx2) { \
			glTexCoord2f(OGL_CALC_2X_N(sx1, sx2, sw), OGL_CALC_2Y_N(sy1, sy2, sh)); \
				glVertex2f(OGL_CALC_2X(dx1, dx2), BEI_SURFACE_BOTTOM_F - (float)(dy1)); \
			glTexCoord2f(OGL_CALC_2X_N(sx2, sx1, sw), OGL_CALC_2Y_N(sy1, sy2, sh)); \
				glVertex2f(OGL_CALC_2X(dx2, dx1), BEI_SURFACE_BOTTOM_F - (float)(dy1)); \
			glTexCoord2f(OGL_CALC_2X_N(sx2, sx1, sw), OGL_CALC_2Y_N(sy2, sy1, sh)); \
				glVertex2f(OGL_CALC_2X(dx2, dx1), BEI_SURFACE_BOTTOM_F - (float)(dy1) + 0.99); \
			glTexCoord2f(OGL_CALC_2X_N(sx1, sx2, sw), OGL_CALC_2Y_N(sy2, sy1, sh)); \
				glVertex2f(OGL_CALC_2X(dx1, dx2), BEI_SURFACE_BOTTOM_F - (float)(dy1) + 0.99); \
		} else { \
			glTexCoord2f(OGL_CALC_2X_N(sx1, sx2, sw), OGL_CALC_2Y_N(sy1, sy2, sh)); \
				glVertex2f((float)(dx1),		BEI_SURFACE_BOTTOM_F - (float)(dy1)); \
			glTexCoord2f(OGL_CALC_2X_N(sx2, sx1, sw), OGL_CALC_2Y_N(sy1, sy2, sh)); \
				glVertex2f((float)(dx1) + 0.99,	BEI_SURFACE_BOTTOM_F - (float)(dy1)); \
			glTexCoord2f(OGL_CALC_2X_N(sx2, sx1, sw), OGL_CALC_2Y_N(sy2, sy1, sh)); \
				glVertex2f((float)(dx1) + 0.99,	BEI_SURFACE_BOTTOM_F - (float)(dy1) + 0.99); \
			glTexCoord2f(OGL_CALC_2X_N(sx1, sx2, sw), OGL_CALC_2Y_N(sy2, sy1, sh)); \
				glVertex2f((float)(dx1),		BEI_SURFACE_BOTTOM_F - (float)(dy1) + 0.99); \
		} \
		glEnd();

#endif

MMSFBBackEndInterface::MMSFBBackEndInterface(int queue_size) : MMSThreadServer(queue_size, "MMSFBBackEndInterface") {

}

void MMSFBBackEndInterface::processData(void *in_data, int in_data_len, void **out_data, int *out_data_len) {
	if (!in_data) return;

	BEI_REQUEST_TYPE *type = (BEI_REQUEST_TYPE *)in_data;

	switch (*type) {
	case BEI_REQUEST_TYPE_INIT:
		processInit((BEI_INIT *)in_data);
		break;
	case BEI_REQUEST_TYPE_SWAP:
		processSwap((BEI_SWAP *)in_data);
		break;
	case BEI_REQUEST_TYPE_ALLOC:
		processAlloc((BEI_ALLOC *)in_data);
		break;
	case BEI_REQUEST_TYPE_FREE:
		processFree((BEI_FREE *)in_data);
		break;
	case BEI_REQUEST_TYPE_CLEAR:
		processClear((BEI_CLEAR *)in_data);
		break;
	case BEI_REQUEST_TYPE_FILLRECTANGLE:
		processFillRectangle((BEI_FILLRECTANGLE *)in_data);
		break;
	case BEI_REQUEST_TYPE_FILLTRIANGLE:
		processFillTriangle((BEI_FILLTRIANGLE *)in_data);
		break;
	case BEI_REQUEST_TYPE_DRAWLINE:
		processDrawLine((BEI_DRAWLINE *)in_data);
		break;
	case BEI_REQUEST_TYPE_DRAWRECTANGLE:
		processDrawRectangle((BEI_DRAWRECTANGLE *)in_data);
		break;
	case BEI_REQUEST_TYPE_DRAWTRIANGLE:
		processDrawTriangle((BEI_DRAWTRIANGLE *)in_data);
		break;
	case BEI_REQUEST_TYPE_STRETCHBLIT:
		processStretchBlit((BEI_STRETCHBLIT *)in_data);
		break;
	case BEI_REQUEST_TYPE_STRETCHBLITBUFFER:
		processStretchBlitBuffer((BEI_STRETCHBLITBUFFER *)in_data);
		break;
	case BEI_REQUEST_TYPE_DRAWSTRING:
		processDrawString((BEI_DRAWSTRING *)in_data);
		break;
	case BEI_REQUEST_TYPE_CUBE:
		processCube((BEI_CUBE *)in_data);
		break;
	default:
		break;
	}
}

#ifdef  __HAVE_XLIB__

void MMSFBBackEndInterface::init(Display *x_display, int x_screen, Window x_window, MMSFBRectangle x11_win_rect) {
	// start the server thread
	start();

	// trigger the init request
	BEI_INIT req;
	req.type		= BEI_REQUEST_TYPE_INIT;
	req.x_display	= x_display;
	req.x_screen	= x_screen;
	req.x_window	= x_window;
	req.x11_win_rect= x11_win_rect;
	trigger((void*)&req, sizeof(req));
}

#else

void MMSFBBackEndInterface::init() {
	// start the server thread
	start();

	// trigger the init request
	BEI_INIT req;
	req.type = BEI_REQUEST_TYPE_INIT;
	trigger((void*)&req, sizeof(req));
}

#endif


void MMSFBBackEndInterface::processInit(BEI_INIT *req) {
#ifdef __HAVE_OPENGL__

#ifdef __HAVE_XLIB__

	mmsfbgl.init(req->x_display, req->x_screen, req->x_window, req->x11_win_rect.w, req->x11_win_rect.h);

#else

	mmsfbgl.init();

#endif

	//set the coordinate system
	this->matrix_w = 0;
	this->matrix_h = 0;
	int w, h;
	mmsfbgl.getResolution(&w, &h);
	oglMatrix(w, h);


	mmsfbgl.useShaderProgram4Drawing();

	mmsfbgl.clear(0xff, 0xff, 0x00, 0xff);

	mmsfbgl.setColor(0x00, 0xff, 0xff, 0x80);

	mmsfbgl.enableBlend();

	mmsfbgl.fillTriangle2D(0, 0, 399, 0, 199, 239);


	mmsfbgl.swap();

	sleep(10);


#endif
}


#ifdef __HAVE_OPENGL__
void MMSFBBackEndInterface::oglMatrix(GLuint w, GLuint h) {
	float ratio = (float)w / (float)h;
	if ((ratio != this->matrix_ratio) || (w > this->matrix_w) || (h > this->matrix_h)) {
		this->matrix_w = w;
		this->matrix_h = h;
		this->matrix_ratio = ratio;
		mmsfbgl.setModelViewMatrix(w, h);
	}
}
#endif

void MMSFBBackEndInterface::swap() {
	BEI_SWAP req;
	req.type	= BEI_REQUEST_TYPE_SWAP;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processSwap(BEI_SWAP *req) {
#ifdef __HAVE_OPENGL__
	// swap screen
	mmsfbgl.swap();
#endif
}

void MMSFBBackEndInterface::alloc(MMSFBSurface *surface) {
	BEI_ALLOC req;
	req.type	= BEI_REQUEST_TYPE_ALLOC;
	req.surface	= surface;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processAlloc(BEI_ALLOC *req) {
#ifdef  __HAVE_OPENGL__
	MMSFBSurfaceBuffer *sb = req->surface->config.surface_buffer;
	oglAlloc(req->surface->config.w, req->surface->config.h, &sb->ogl_fbo, &sb->ogl_tex, &sb->ogl_rb);
	return;
#endif
}

#ifdef  __HAVE_OPENGL__
void MMSFBBackEndInterface::oglAlloc(int width, int height, GLuint *ogl_fbo, GLuint *ogl_tex, GLuint *ogl_rb) {
#ifdef  __HAVE_GL2__
	// lock destination fbo and prepare it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDisable(GL_SCISSOR_TEST);

	glGenTextures(1, ogl_tex);
	glBindTexture(GL_TEXTURE_2D, *ogl_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	// ---
//remove glGenRenderbuffersEXT, glBindRenderbufferEXT, glRenderbufferStorageEXT?
	glGenRenderbuffersEXT(1, ogl_rb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, *ogl_rb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);
	// ---
	glGenFramebuffersEXT(1, ogl_fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *ogl_fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, *ogl_tex, 0);
//remove glFramebufferRenderbufferEXT?
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, *ogl_rb);
	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT) {
		// the GPU does not support current FBO configuration
		MMSFB_SetError(0, "creating OPENGL FBO with " + iToStr(width) + "x" + iToStr(height) + " failed, the GPU does not support current FBO configuration");
		glDisable(GL_SCISSOR_TEST);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
printf("TODO: fatal error while allocating new fbo\n");
		return;
	}

	// all is fine
	glDisable(GL_SCISSOR_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#endif
}
#endif

void MMSFBBackEndInterface::free(MMSFBSurface *surface) {
	BEI_FREE req;
	req.type	= BEI_REQUEST_TYPE_FREE;
	req.surface	= surface;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processFree(BEI_FREE *req) {
#ifdef  __HAVE_OPENGL__
	MMSFBSurfaceBuffer *sb = req->surface->config.surface_buffer;
	oglFree(sb->ogl_fbo, sb->ogl_rb, sb->ogl_tex);
	return;
#endif
}

#ifdef  __HAVE_OPENGL__
void MMSFBBackEndInterface::oglFree(GLuint ogl_fbo, GLuint ogl_rb, GLuint ogl_tex) {
#ifdef  __HAVE_GL2__
	glDeleteFramebuffersEXT(1, &ogl_fbo);
	glDeleteRenderbuffersEXT(1, &ogl_rb);
	glDeleteTextures(1, &ogl_tex);
#endif
}
#endif

void MMSFBBackEndInterface::clear(MMSFBSurface *surface, MMSFBColor &color) {
	BEI_CLEAR req;
	req.type	= BEI_REQUEST_TYPE_CLEAR;
	req.surface	= surface;
	req.color	= color;
	trigger((void*)&req, sizeof(req));
}


void MMSFBBackEndInterface::processClear(BEI_CLEAR *req) {
#ifdef  __HAVE_GL2__
	// lock destination fbo and prepare it
	INIT_OGL_FBO(req->surface);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(req->color.r, req->color.g, req->color.b, req->color.a);

/*	printf("%08x, %dx%d, %d,%d,%d,%d, %d,%d,%d,%d DISKO: processClear\n",
			req->surface,BEI_SURFACE_WIDTH,BEI_SURFACE_HEIGHT,0,0,BEI_SURFACE_WIDTH,BEI_SURFACE_HEIGHT,
			req->color.r,req->color.g,req->color.b,req->color.a);
*/
	// get subsurface offsets
	GET_OFFS(req->surface);

	// set the clip to ogl
	MMSFBRectangle crect;
	if (req->surface->calcClip(0 + xoff, 0 + yoff, req->surface->config.w, req->surface->config.h, &crect)) {
		// inside clipping region
		OGL_GLSCISSOR(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// fill rectangle
		OGL_FILL_RECTANGLE(0								+ xoff,
						   0								+ yoff,
						   0 + req->surface->config.w - 1	+ xoff,
						   0 + req->surface->config.h - 1	+ yoff);
	}
#endif
}

void MMSFBBackEndInterface::fillRectangle(MMSFBSurface *surface, MMSFBRectangle &rect) {
	BEI_FILLRECTANGLE req;
	req.type	= BEI_REQUEST_TYPE_FILLRECTANGLE;
	req.surface	= surface;
	req.rect	= rect;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processFillRectangle(BEI_FILLRECTANGLE *req) {
#ifdef  __HAVE_GL2__
	// lock destination fbo and prepare it
	INIT_OGL_FBO(req->surface);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

/*	printf("%08x, %dx%d, %d,%d,%d,%d, %d,%d,%d,%d DISKO: processFillRectangle\n",
			req->surface,BEI_SURFACE_WIDTH,BEI_SURFACE_HEIGHT,req->rect.x,req->rect.y,req->rect.w,req->rect.h,
			req->surface->config.color.r,req->surface->config.color.g,req->surface->config.color.b,req->surface->config.color.a);
*/
	// setup drawing
	INIT_OGL_DRAWING(req->surface);

	// get subsurface offsets
	GET_OFFS(req->surface);

	// set the clip to ogl
	MMSFBRectangle crect;
	if (req->surface->calcClip(req->rect.x + xoff, req->rect.y + yoff, req->rect.w, req->rect.h, &crect)) {
		// inside clipping region
		OGL_GLSCISSOR(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// fill rectangle
		OGL_FILL_RECTANGLE(req->rect.x						+ xoff,
						   req->rect.y						+ yoff,
						   req->rect.x + req->rect.w - 1	+ xoff,
						   req->rect.y + req->rect.h - 1	+ yoff);
	}
#endif
}

void MMSFBBackEndInterface::fillTriangle(MMSFBSurface *surface, MMSFBTriangle &triangle) {
	BEI_FILLTRIANGLE req;
	req.type	= BEI_REQUEST_TYPE_FILLTRIANGLE;
	req.surface	= surface;
	req.triangle= triangle;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processFillTriangle(BEI_FILLTRIANGLE *req) {
#ifdef  __HAVE_GL2__
	// lock destination fbo and prepare it
	INIT_OGL_FBO(req->surface);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	// setup drawing
	INIT_OGL_DRAWING(req->surface);

	// get subsurface offsets
	GET_OFFS(req->surface);

	// set the clip to ogl
	MMSFBRectangle crect;
	int x, y, w, h;
	if (req->triangle.x2 >= req->triangle.x1) {
		x = req->triangle.x1;
		w = req->triangle.x2 - req->triangle.x1 + 1;
	}
	else {
		x = req->triangle.x2;
		w = req->triangle.x1 - req->triangle.x2 + 1;
	}
	if (req->triangle.y2 >= req->triangle.y1) {
		y = req->triangle.y1;
		h = req->triangle.y2 - req->triangle.y1 + 1;
	}
	else {
		y = req->triangle.y2;
		h = req->triangle.y1 - req->triangle.y2 + 1;
	}
	if (req->triangle.x3 < x) x = req->triangle.x3;
	if (req->triangle.x3 > x + w - 1) w = req->triangle.x3 - x + 1;
	if (req->triangle.y3 < y) y = req->triangle.y3;
	if (req->triangle.y3 > y + h - 1) h = req->triangle.y3 - y + 1;

	if (req->surface->calcClip(x + xoff, y + yoff, w, h, &crect)) {
		// inside clipping region
		OGL_GLSCISSOR(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// fill triangle
		OGL_FILL_TRIANGLE(req->triangle.x1 + xoff, req->triangle.y1 + yoff,
						  req->triangle.x2 + xoff, req->triangle.y2 + yoff,
						  req->triangle.x3 + xoff, req->triangle.y3 + yoff);
	}
#endif
}

void MMSFBBackEndInterface::drawLine(MMSFBSurface *surface, MMSFBRegion &region) {
	BEI_DRAWLINE req;
	req.type	= BEI_REQUEST_TYPE_DRAWLINE;
	req.surface	= surface;
	req.region	= region;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processDrawLine(BEI_DRAWLINE *req) {
#ifdef  __HAVE_GL2__
	// lock destination fbo and prepare it
	INIT_OGL_FBO(req->surface);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

/*	printf("%08x, %dx%d, %d,%d,%d,%d DISKO: processDrawLine\n",
			req->surface,
			req->surface->config.color.r,req->surface->config.color.g,req->surface->config.color.b,req->surface->config.color.a);
*/
	// setup drawing
	INIT_OGL_DRAWING(req->surface);

	// get subsurface offsets
	GET_OFFS(req->surface);

	// set the clip to ogl
	MMSFBRectangle crect;
	int x, y, w, h;
	if (req->region.x2 >= req->region.x1) {
		x = req->region.x1;
		w = req->region.x2 - req->region.x1 + 1;
	}
	else {
		x = req->region.x2;
		w = req->region.x1 - req->region.x2 + 1;
	}
	if (req->region.y2 >= req->region.y1) {
		y = req->region.y1;
		h = req->region.y2 - req->region.y1 + 1;
	}
	else {
		y = req->region.y2;
		h = req->region.y1 - req->region.y2 + 1;
	}

	if (req->surface->calcClip(x + xoff, y + yoff, w, h, &crect)) {
		// inside clipping region
		OGL_GLSCISSOR(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// draw line
		OGL_DRAW_LINE(req->region.x1 + xoff,
					  req->region.y1 + yoff,
					  req->region.x2 + xoff,
					  req->region.y2 + yoff);
	}
#endif
}

void MMSFBBackEndInterface::drawRectangle(MMSFBSurface *surface, MMSFBRectangle &rect) {
	BEI_DRAWRECTANGLE req;
	req.type	= BEI_REQUEST_TYPE_DRAWRECTANGLE;
	req.surface	= surface;
	req.rect	= rect;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processDrawRectangle(BEI_DRAWRECTANGLE *req) {
#ifdef  __HAVE_GL2__
	// lock destination fbo and prepare it
	INIT_OGL_FBO(req->surface);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	// setup drawing
	INIT_OGL_DRAWING(req->surface);

	// get subsurface offsets
	GET_OFFS(req->surface);

	// set the clip to ogl
	MMSFBRectangle crect;
	if (req->surface->calcClip(req->rect.x + xoff, req->rect.y + yoff, req->rect.w, req->rect.h, &crect)) {
		// inside clipping region
		OGL_GLSCISSOR(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// draw rectangle
		OGL_DRAW_RECTANGLE(req->rect.x						+ xoff,
						   req->rect.y						+ yoff,
						   req->rect.x + req->rect.w - 1	+ xoff,
						   req->rect.y + req->rect.h - 1	+ yoff);
	}
#endif
}

void MMSFBBackEndInterface::drawTriangle(MMSFBSurface *surface, MMSFBTriangle &triangle) {
	BEI_DRAWTRIANGLE req;
	req.type	= BEI_REQUEST_TYPE_DRAWTRIANGLE;
	req.surface	= surface;
	req.triangle= triangle;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processDrawTriangle(BEI_DRAWTRIANGLE *req) {
#ifdef  __HAVE_GL2__
	// lock destination fbo and prepare it
	INIT_OGL_FBO(req->surface);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	// setup drawing
	INIT_OGL_DRAWING(req->surface);

	// get subsurface offsets
	GET_OFFS(req->surface);

	// set the clip to ogl
	MMSFBRectangle crect;
	int x, y, w, h;
	if (req->triangle.x2 >= req->triangle.x1) {
		x = req->triangle.x1;
		w = req->triangle.x2 - req->triangle.x1 + 1;
	}
	else {
		x = req->triangle.x2;
		w = req->triangle.x1 - req->triangle.x2 + 1;
	}
	if (req->triangle.y2 >= req->triangle.y1) {
		y = req->triangle.y1;
		h = req->triangle.y2 - req->triangle.y1 + 1;
	}
	else {
		y = req->triangle.y2;
		h = req->triangle.y1 - req->triangle.y2 + 1;
	}
	if (req->triangle.x3 < x) x = req->triangle.x3;
	if (req->triangle.x3 > x + w - 1) w = req->triangle.x3 - x + 1;
	if (req->triangle.y3 < y) y = req->triangle.y3;
	if (req->triangle.y3 > y + h - 1) h = req->triangle.y3 - y + 1;

	if (req->surface->calcClip(x + xoff, y + yoff, w, h, &crect)) {
		// inside clipping region
		OGL_GLSCISSOR(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// draw triangle
		OGL_DRAW_TRIANGLE(req->triangle.x1 + xoff, req->triangle.y1 + yoff,
						  req->triangle.x2 + xoff, req->triangle.y2 + yoff,
						  req->triangle.x3 + xoff, req->triangle.y3 + yoff);
	}
#endif
}

void MMSFBBackEndInterface::blit(MMSFBSurface *surface, MMSFBSurface *source, MMSFBRectangle &src_rect,
								 int x, int y) {
	BEI_STRETCHBLIT req;
	req.type		= BEI_REQUEST_TYPE_STRETCHBLIT;
	req.surface		= surface;
	req.source		= source;
	req.src_rect	= src_rect;
	req.dst_rect.x	= x;
	req.dst_rect.y	= y;
	req.dst_rect.w	= src_rect.w;
	req.dst_rect.h	= src_rect.h;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::stretchBlit(MMSFBSurface *surface, MMSFBSurface *source, MMSFBRectangle &src_rect,
										MMSFBRectangle &dst_rect) {
	BEI_STRETCHBLIT req;
	req.type	= BEI_REQUEST_TYPE_STRETCHBLIT;
	req.surface	= surface;
	req.source	= source;
	req.src_rect= src_rect;
	req.dst_rect= dst_rect;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processStretchBlit(BEI_STRETCHBLIT *req) {
#ifdef  __HAVE_GL2__
	// lock destination fbo and bind source texture to it
	INIT_OGL_FBO(req->surface);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, req->source->config.surface_buffer->ogl_tex);

/*	printf("%08x, src %08x DISKO: processStretchBlit\n",
			req->surface, req->source);
*/
	// setup blitting
	INIT_OGL_BLITTING(req->surface);

	// get subsurface offsets
	GET_OFFS(req->surface);
	GET_OFFS_SRC(req->source);

	// set the clip to ogl
	MMSFBRectangle crect;
	if (req->surface->calcClip(req->dst_rect.x + xoff, req->dst_rect.y + yoff, req->dst_rect.w, req->dst_rect.h, &crect)) {
		// inside clipping region
		OGL_GLSCISSOR(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// get source region
		int sx1 = req->src_rect.x + src_xoff;
		int sy1 = req->src_rect.y + src_yoff;
		int sx2 = req->src_rect.x + req->src_rect.w - 1 + src_xoff;
		int sy2 = req->src_rect.y + req->src_rect.h - 1 + src_yoff;

		// get destination region
		int dx1 = req->dst_rect.x + xoff;
		int dy1 = req->dst_rect.y + yoff;
		int dx2 = req->dst_rect.x + req->dst_rect.w - 1 + xoff;
		int dy2 = req->dst_rect.y + req->dst_rect.h - 1 + yoff;

		// blit source texture to the destination
		OGL_STRETCH_BLIT(sx1, sy1, sx2, sy2, req->source->config.w, req->source->config.h, dx1, dy1, dx2, dy2);
	}
#endif
}

void MMSFBBackEndInterface::blitBuffer(MMSFBSurface *surface, MMSFBSurfacePlanes *src_planes, MMSFBSurfacePixelFormat src_pixelformat,
									   int src_width, int src_height, MMSFBRectangle &src_rect, int x, int y) {
	BEI_STRETCHBLITBUFFER req;
	req.type			= BEI_REQUEST_TYPE_STRETCHBLITBUFFER;
	req.surface			= surface;
	req.src_planes		= src_planes;
	req.src_pixelformat	= src_pixelformat;
	req.src_width		= src_width;
	req.src_height		= src_height;
	req.src_rect		= src_rect;
	req.dst_rect.x		= x;
	req.dst_rect.y		= y;
	req.dst_rect.w		= src_rect.w;
	req.dst_rect.h		= src_rect.h;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::stretchBlitBuffer(MMSFBSurface *surface, MMSFBSurfacePlanes *src_planes, MMSFBSurfacePixelFormat src_pixelformat,
											  int src_width, int src_height,MMSFBRectangle &src_rect, MMSFBRectangle &dst_rect) {
	BEI_STRETCHBLITBUFFER req;
	req.type			= BEI_REQUEST_TYPE_STRETCHBLITBUFFER;
	req.surface			= surface;
	req.src_planes		= src_planes;
	req.src_pixelformat	= src_pixelformat;
	req.src_width		= src_width;
	req.src_height		= src_height;
	req.src_rect		= src_rect;
	req.dst_rect		= dst_rect;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processStretchBlitBuffer(BEI_STRETCHBLITBUFFER *req) {
#ifdef  __HAVE_GL2__
	// lock destination fbo and bind source texture to it
	INIT_OGL_FBO(req->surface);
	glEnable(GL_TEXTURE_2D);

/*	printf("%08x, src %08x DISKO: processStretchBlitBuffer\n",
			req->surface, req->src_planes->ptr);
*/
	// allocate a texture name
	GLuint texture;
	glGenTextures( 1, &texture );
    glBindTexture (GL_TEXTURE_2D, texture);
    // select modulate to mix texture with color for shading
    //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // the texture wraps over at the edges (repeat)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D,
	 	0,
	 	GL_RGBA,
	 	req->src_width,
	 	req->src_height,
	 	0,
	 	GL_BGRA,
	 	GL_UNSIGNED_BYTE,
	 	req->src_planes->ptr);

/*
unsigned char pix[8*8];
memset(pix, 0, sizeof(pix));
pix[0]=0xff;
pix[9]=0x90;
pix[18]=0x40;
glTexImage2D(GL_TEXTURE_2D,
	0,
	GL_ALPHA,
	8,
	8,
	0,
	GL_ALPHA,
	GL_UNSIGNED_BYTE,
	pix);
*/

	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// setup blitting
	INIT_OGL_BLITTING(req->surface);

//glDisable(GL_BLEND);
//glColor4ub(255, 255, 0, 255);

	// get subsurface offsets
	GET_OFFS(req->surface);

	// set the clip to ogl
	MMSFBRectangle crect;
	if (req->surface->calcClip(req->dst_rect.x + xoff, req->dst_rect.y + yoff, req->dst_rect.w, req->dst_rect.h, &crect)) {
		// inside clipping region
		OGL_GLSCISSOR(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// get source region
		int sx1 = req->src_rect.x;
		int sy1 = req->src_rect.y;
		int sx2 = req->src_rect.x + req->src_rect.w - 1;
		int sy2 = req->src_rect.y + req->src_rect.h - 1;

		// get destination region
		int dx1 = req->dst_rect.x + xoff;
		int dy1 = req->dst_rect.y + yoff;
		int dx2 = req->dst_rect.x + req->dst_rect.w - 1 + xoff;
		int dy2 = req->dst_rect.y + req->dst_rect.h - 1 + yoff;

		// blit source texture to the destination
		// note: the source has to be flipped vertical because the difference between 2D input and OGL
		OGL_STRETCH_BLIT(sx1, sy1, sx2, sy2, req->src_width, req->src_height, dx1, dy2, dx2, dy1);
	}

	// all is fine
	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &texture);


#endif
}

void MMSFBBackEndInterface::drawString(MMSFBSurface *surface, string &text, int len, int x, int y) {
	BEI_DRAWSTRING req;
	req.type	= BEI_REQUEST_TYPE_DRAWSTRING;
	req.surface	= surface;
	req.text	= text;
	req.len		= len;
	req.x		= x;
	req.y		= y;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processDrawString(BEI_DRAWSTRING *req) {
#ifdef  __HAVE_GL2__

	// lock destination fbo and bind source texture to it
	INIT_OGL_FBO(req->surface);
	glEnable(GL_TEXTURE_2D);

/*	printf("%08x, DISKO: processDrawString\n",
			req->surface);
*/
	// allocate a texture name
	GLuint texture;
	glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading
//    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
//    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
//    glTexEnvf( GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE );
//    glTexEnvf( GL_TEXTURE_ENV, GL_ALPHA_SCALE, 1.0 );
//    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// setup blitting
    glEnable(GL_BLEND);
	glColor4ub(req->surface->config.color.r,
			   req->surface->config.color.g,
			   req->surface->config.color.b,
			   req->surface->config.color.a);

	// get subsurface offsets
	GET_OFFS(req->surface);

	// get vertical font settings
	int DY = 0, desc = 0;
	req->surface->config.font->getHeight(&DY);
	req->surface->config.font->getDescender(&desc);
	DY -= desc + 1;

	// for all characters
	MMSFBFONT_GET_UNICODE_CHAR(req->text, req->len) {

		// load the glyph
		MMSFBSURFACE_BLIT_TEXT_LOAD_GLYPH(req->surface->config.font, character);

		// start rendering of glyph to destination
		if (glyph) {
			// calc destination position of the character
			int dx = req->x + glyph->left;
			int dy = req->y + DY - glyph->top;

			// set the clip to ogl
			MMSFBRectangle crect;
			if (req->surface->calcClip(dx + xoff, dy + yoff, src_w, src_h, &crect)) {
				// inside clipping region
				OGL_GLSCISSOR(crect.x, crect.y, crect.w, crect.h);
				glEnable(GL_SCISSOR_TEST);

				// get source region
				int sx1 = 0;
				int sy1 = 0;
				int sx2 = src_w - 1;
				int sy2 = src_h - 1;

				// get destination region
				int dx1 = dx + xoff;
				int dy1 = dy + yoff;
				int dx2 = dx + src_w - 1 + xoff;
				int dy2 = dy + src_h - 1 + yoff;

			    // specify two-dimensional glyph texture
			    // we use pitch instead of width here, because we need a width which is divisible by 4!!!
				glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_ALPHA,
					src_pitch_pix,
					src_h,
					0,
					GL_ALPHA,
					GL_UNSIGNED_BYTE,
					src);

				// blit glyph texture to the destination
				// note: the source has to be flipped vertical because the difference between 2D input and OGL
				OGL_STRETCH_BLIT(sx1, sy1, sx2, sy2, src_pitch_pix, src_h, dx1, dy2, dx2, dy1);
			}

			// prepare for next loop
			req->x+=glyph->advanceX >> 6;
		}
	}}

	// all is fine
	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &texture);

#endif
}

void MMSFBBackEndInterface::cube(MMSFBSurface *surface,
									MMSFBSurface *front, MMSFBSurface *back,
									MMSFBSurface *left, MMSFBSurface *right,
									MMSFBSurface *top, MMSFBSurface *bottom,
									float angle_x, float angle_y, float angle_z) {
	BEI_CUBE req;
	req.type		= BEI_REQUEST_TYPE_CUBE;
	req.surface		= surface;
	req.front		= front;
	req.back		= back;
	req.left		= left;
	req.right		= right;
	req.top			= top;
	req.bottom		= bottom;
	req.angle_x		= angle_x;
	req.angle_y		= angle_y;
	req.angle_z		= angle_z;
	trigger((void*)&req, sizeof(req));
}



void MMSFBBackEndInterface::processCube(BEI_CUBE *req) {
#ifdef  __HAVE_GL2__
	// lock destination fbo and bind source texture to it
	INIT_OGL_FBOXX(req->surface);


	// setup blitting
	INIT_OGL_BLITTING(req->surface);

	// get subsurface offsets
	GET_OFFS(req->surface);
//	GET_OFFS_SRC(req->source);

	MMSFBRectangle dst_rect(0,0,300,300);
//	MMSFBRectangle src_rect(0,0,req->source->config.w,req->source->config.h);

	// set the clip to ogl
	MMSFBRectangle crect;
	if (req->surface->calcClip(dst_rect.x + xoff, dst_rect.y + yoff, dst_rect.w, dst_rect.h, &crect)) {
		// inside clipping region
		OGL_GLSCISSOR(crect.x, crect.y, crect.w, crect.h);
//		glEnable(GL_SCISSOR_TEST);

		dst_rect.x+=90;
		dst_rect.y+=90;
		dst_rect.w-=180;
		dst_rect.h-=180;

		// get source region
/*		int sx1 = src_rect.x + src_xoff;
		int sy1 = src_rect.y + src_yoff;
		int sx2 = src_rect.x + src_rect.w - 1 + src_xoff;
		int sy2 = src_rect.y + src_rect.h - 1 + src_yoff;
*/
		// get destination region
		int dx1 = dst_rect.x + xoff;
		int dy1 = dst_rect.y + yoff;
		int dx2 = dst_rect.x + dst_rect.w - 1 + xoff;
		int dy2 = dst_rect.y + dst_rect.h - 1 + yoff;

		// blit source texture to the destination
//		OGL_STRETCH_BLIT(sx1, sy1, sx2, sy2, req->source->config.w, req->source->config.h, dx1, dy1, dx2, dy2);

//		int sw = req->source->config.w;
//		int sh = req->source->config.h;

glDepthMask(GL_TRUE);
//glDisable(GL_BLEND);
glClearColor(0.0, 0.0, 0.0, 0.0);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glEnable(GL_DEPTH_TEST);
glDepthRange(1,-1);

		glPushMatrix();
		//glTranslatef(-150,-150,0);
//		glTranslatef(0,0,-1);
//		glTranslatef(-0.25,-0.25,0);
//		glRotatef(req->angle, req->x,req->y,req->z);
//		glRotatef(4, req->x,req->y,req->z);
//		glTranslatef(100,100,0);

//		glRotatef(req->angle, 0,0,1);
		glRotatef(req->angle_x, 1, 0, 0);
		glRotatef(req->angle_y, 0, 1, 0);
		glRotatef(req->angle_z, 0, 0, 1);

		int ddw = 100;
		int ddh = 100;
		int ddz = 100;



		// front
		if (req->front) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, req->front->config.surface_buffer->ogl_tex);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
					glVertex3f(-ddw/2, -ddh/2, ddz/2);
				glTexCoord2f(1, 0);
					glVertex3f(ddw/2, -ddh/2, ddz/2);
				glTexCoord2f(1, 1);
					glVertex3f(ddw/2, ddh/2, ddz/2);
				glTexCoord2f(0, 1);
					glVertex3f(-ddw/2, ddh/2, ddz/2);
			glEnd();
		}

		// back
		if (req->back) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, req->back->config.surface_buffer->ogl_tex);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
					glVertex3f(ddw/2, -ddh/2, -ddz/2);
				glTexCoord2f(1, 0);
					glVertex3f(-ddw/2, -ddh/2, -ddz/2);
				glTexCoord2f(1, 1);
					glVertex3f(-ddw/2, ddh/2, -ddz/2);
				glTexCoord2f(0, 1);
					glVertex3f(ddw/2, ddh/2, -ddz/2);
			glEnd();
		}

		// left
		if (req->left) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, req->left->config.surface_buffer->ogl_tex);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
					glVertex3f(-ddw/2, -ddh/2, -ddz/2);
				glTexCoord2f(1, 0);
					glVertex3f(-ddw/2, -ddh/2, ddz/2);
				glTexCoord2f(1, 1);
					glVertex3f(-ddw/2, ddh/2, ddz/2);
				glTexCoord2f(0, 1);
					glVertex3f(-ddw/2, ddh/2, -ddz/2);
			glEnd();
		}

		// right
		if (req->right) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, req->right->config.surface_buffer->ogl_tex);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
					glVertex3f(ddw/2, -ddh/2, ddz/2);
				glTexCoord2f(1, 0);
					glVertex3f(ddw/2, -ddh/2, -ddz/2);
				glTexCoord2f(1, 1);
					glVertex3f(ddw/2, ddh/2, -ddz/2);
				glTexCoord2f(0, 1);
					glVertex3f(ddw/2, ddh/2, ddz/2);
			glEnd();
		}

		// top
		if (req->top) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, req->top->config.surface_buffer->ogl_tex);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
					glVertex3f(-ddw/2, ddh/2, ddz/2);
				glTexCoord2f(1, 0);
					glVertex3f(ddw/2, ddh/2, ddz/2);
				glTexCoord2f(1, 1);
					glVertex3f(ddw/2, ddh/2, -ddz/2);
				glTexCoord2f(0, 1);
					glVertex3f(-ddw/2, ddh/2, -ddz/2);
			glEnd();
		}

		// bottom
		if (req->bottom) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, req->bottom->config.surface_buffer->ogl_tex);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
					glVertex3f(-ddw/2, -ddh/2, -ddz/2);
				glTexCoord2f(1, 0);
					glVertex3f(ddw/2, -ddh/2, -ddz/2);
				glTexCoord2f(1, 1);
					glVertex3f(ddw/2, -ddh/2, ddz/2);
				glTexCoord2f(0, 1);
					glVertex3f(-ddw/2, -ddh/2, ddz/2);
			glEnd();
		}

		glPopMatrix();

glDepthMask(GL_FALSE);
glDisable(GL_DEPTH_TEST);

	}
#endif
}

#ifdef __HAVE_GL2__
void MMSFBBackEndInterface::oglMatrixXX(GLuint w, GLuint h) {
	float ratio = (float)w / (float)h;
	this->matrix_w = w;
	this->matrix_h = h;
	this->matrix_ratio = ratio;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	glOrtho(-(int)w/2, w/2, -(int)h/2, h/2, 600.0, -600.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	this->matrix_ratio = 0;
}
#endif

