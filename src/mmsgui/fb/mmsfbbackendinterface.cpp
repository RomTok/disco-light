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

#ifdef __HAVE_OPENGL__

#define INIT_OGL_DRAWING(surface) \
			switch (surface->config.drawingflags) { \
			case MMSFB_DRAW_BLEND: glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break; \
			default: glDisable(GL_BLEND); break; } \
			glColor4ub(surface->config.color.r, surface->config.color.g, surface->config.color.b, surface->config.color.a);

#define INIT_OGL_BLITTING(surface) \
		switch (surface->config.blittingflags) { \
		case MMSFB_BLIT_BLEND_ALPHACHANNEL:	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glColor4ub(255, 255, 255, 255); break; \
		case MMSFB_BLIT_BLEND_ALPHACHANNEL + MMSFB_BLIT_BLEND_COLORALPHA: glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glColor4ub(255, 255, 255, surface->config.color.a); break; \
		default: glDisable(GL_BLEND); glColor4ub(255, 255, 255, 255); break; }

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
	default:
		break;
	}
}

void MMSFBBackEndInterface::init(Display *x_display, int x_screen, Window x_window, MMSFBRectangle x11_win_rect) {

	// start the server thread
	this->start();
sleep(2);


	BEI_INIT req;
	req.type		= BEI_REQUEST_TYPE_INIT;
	req.x_display	= x_display;
	req.x_screen	= x_screen;
	req.x_window	= x_window;
	req.x11_win_rect= x11_win_rect;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processInit(BEI_INIT *req) {
#ifdef __HAVE_OPENGL__
	// OGL

	this->x_display = req->x_display;
	this->x_window = req->x_window;

	int glxMajor, glxMinor;

	glXQueryVersion(x_display, &glxMajor, &glxMinor);
	printf("GLX-Version %d.%d\n", glxMajor, glxMinor);
	int attribList[] =
		{GLX_RGBA,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_DEPTH_SIZE, 16,
		GLX_DOUBLEBUFFER,
		None};
	this->xvi = glXChooseVisual(req->x_display, req->x_screen, attribList);
	if (this->xvi == NULL) {
		int attribList[] =
				{GLX_RGBA,
				GLX_RED_SIZE, 8,
				GLX_GREEN_SIZE, 8,
				GLX_BLUE_SIZE, 8,
				None};
		this->xvi = glXChooseVisual(req->x_display, req->x_screen, attribList);
		printf("singlebuffered rendering will be used, no doublebuffering available\n");
		if(this->xvi == NULL) {
			printf("shit happens.... \n");
			return;
		}
	}
	else {
		printf("doublebuffered rendering available\n");
	}

	// create a GLX context
	this->glx_context = glXCreateContext(req->x_display, this->xvi, 0, GL_TRUE);
	if (!this->glx_context) {
		printf("context generation failed...\n");
		return;
	}

	if(glXMakeCurrent(req->x_display, req->x_window, this->glx_context) != True) {
		printf("make current failed\n");
		return;
	}

	if (glXIsDirect(req->x_display, this->glx_context))
		printf("DRI enabled\n");
	else
		printf("no DRI available\n");

	XMapRaised(req->x_display, req->x_window);
	XFlush(req->x_display);

	// init extension pointers
	GLenum err=glewInit();
	if(err!=GLEW_OK) {
		//problem: glewInit failed, something is seriously wrong
		printf("Error: %s\n", glewGetErrorString(err));
		return;
	}

	//set the coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, req->x11_win_rect.w, req->x11_win_rect.h);
	glOrtho(0, req->x11_win_rect.w, 0, req->x11_win_rect.h, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#endif

}


void MMSFBBackEndInterface::swap() {
	BEI_SWAP req;
	req.type	= BEI_REQUEST_TYPE_SWAP;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processSwap(BEI_SWAP *req) {
#ifdef  __HAVE_OPENGL__
	// lock destination fbo and prepare it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDisable(GL_SCISSOR_TEST);

	glXSwapBuffers(this->x_display, this->x_window);

	// all is fine
	glDisable(GL_SCISSOR_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
	// lock destination fbo and prepare it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDisable(GL_SCISSOR_TEST);

	MMSFBSurfaceBuffer *sb = req->surface->config.surface_buffer;
	glGenTextures(1, &sb->ogl_tex);
	glBindTexture(GL_TEXTURE_2D, sb->ogl_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, req->surface->config.w, req->surface->config.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	// ---
//remove glGenRenderbuffersEXT, glBindRenderbufferEXT, glRenderbufferStorageEXT?
	glGenRenderbuffersEXT(1, &sb->ogl_rb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, sb->ogl_rb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, req->surface->config.w, req->surface->config.h);
	// ---
	glGenFramebuffersEXT(1, &sb->ogl_fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sb->ogl_fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, sb->ogl_tex, 0);
//remove glFramebufferRenderbufferEXT?
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, sb->ogl_rb);
	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT) {
		// the GPU does not support current FBO configuration
		MMSFB_SetError(0, "creating OPENGL FBO with " + iToStr(req->surface->config.w) + "x" + iToStr(req->surface->config.h) + " failed, the GPU does not support current FBO configuration");
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

void MMSFBBackEndInterface::clear(MMSFBSurface *surface, MMSFBColor &color) {
	BEI_CLEAR req;
	req.type	= BEI_REQUEST_TYPE_CLEAR;
	req.surface	= surface;
	req.color	= color;
	trigger((void*)&req, sizeof(req));
}

void MMSFBBackEndInterface::processClear(BEI_CLEAR *req) {
#ifdef  __HAVE_OPENGL__
	// lock destination fbo and prepare it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(req->color.r, req->color.g, req->color.b, req->color.a);

	// get subsurface offsets
	GET_OFFS(req->surface);

	// set the clip to ogl
	MMSFBRectangle crect;
	if (req->surface->calcClip(0 + xoff, 0 + yoff, req->surface->config.w, req->surface->config.h, &crect)) {
		// inside clipping region
		glScissor(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// fill rectangle
		glRecti(0								+ xoff,
				0								+ yoff,
				0 + req->surface->config.w - 1	+ xoff,
				0 + req->surface->config.h - 1	+ yoff);
	}

	// all is fine
	glDisable(GL_SCISSOR_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
#ifdef  __HAVE_OPENGL__
	// lock destination fbo and prepare it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
	glDisable(GL_SCISSOR_TEST);
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
		glScissor(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// fill rectangle
		glRecti(req->rect.x						+ xoff,
				req->rect.y						+ yoff,
				req->rect.x + req->rect.w - 1	+ xoff,
				req->rect.y + req->rect.h - 1	+ yoff);
	}

	// all is fine
	glDisable(GL_SCISSOR_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
#ifdef  __HAVE_OPENGL__
	// lock destination fbo and prepare it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
	glDisable(GL_SCISSOR_TEST);
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
		glScissor(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// fill triangle
		glBegin(GL_TRIANGLES);
		glVertex2i(req->triangle.x1 + xoff, req->triangle.y1 + yoff);
		glVertex2i(req->triangle.x2 + xoff, req->triangle.y2 + yoff);
		glVertex2i(req->triangle.x3 + xoff, req->triangle.y3 + yoff);
		glEnd();
	}

	// all is fine
	glDisable(GL_SCISSOR_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
#ifdef  __HAVE_OPENGL__
	// lock destination fbo and prepare it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

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
		glScissor(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// draw line
		glBegin(GL_LINES);
		glVertex2i(req->region.x1 + xoff, req->region.y1 + yoff);
		glVertex2i(req->region.x2 + xoff, req->region.y2 + yoff);
		glEnd();
	}

	// all is fine
	glDisable(GL_SCISSOR_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
#ifdef  __HAVE_OPENGL__
	// lock destination fbo and prepare it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
	glDisable(GL_SCISSOR_TEST);
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
		glScissor(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// draw rectangle
		glBegin(GL_LINE_STRIP);
		glVertex2i(req->rect.x				+ xoff, req->rect.y					+ yoff);
		glVertex2i(req->rect.x+req->rect.w-1+ xoff, req->rect.y					+ yoff);
		glVertex2i(req->rect.x+req->rect.w-1+ xoff, req->rect.y+req->rect.h-1	+ yoff);
		glVertex2i(req->rect.x				+ xoff, req->rect.y+req->rect.h-1	+ yoff);
		glVertex2i(req->rect.x				+ xoff, req->rect.y					+ yoff);
		glEnd();
	}

	// all is fine
	glDisable(GL_SCISSOR_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
#ifdef  __HAVE_OPENGL__
	// lock destination fbo and prepare it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
	glDisable(GL_SCISSOR_TEST);
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
		glScissor(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// draw triangle
		glBegin(GL_LINE_STRIP);
		glVertex2i(req->triangle.x1 + xoff, req->triangle.y1 + yoff);
		glVertex2i(req->triangle.x2 + xoff, req->triangle.y2 + yoff);
		glVertex2i(req->triangle.x3 + xoff, req->triangle.y3 + yoff);
		glVertex2i(req->triangle.x1 + xoff, req->triangle.y1 + yoff);
		glEnd();
	}

	// all is fine
	glDisable(GL_SCISSOR_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#endif
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
#ifdef  __HAVE_OPENGL__
	// lock destination fbo and bind source texture to it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, req->source->config.surface_buffer->ogl_tex);

	// setup blitting
	INIT_OGL_BLITTING(req->surface);

	// get subsurface offsets
	GET_OFFS(req->surface);
	GET_OFFS_SRC(req->source);

	// set the clip to ogl
	MMSFBRectangle crect;
	if (req->surface->calcClip(req->dst_rect.x + xoff, req->dst_rect.y + yoff, req->dst_rect.w, req->dst_rect.h, &crect)) {
		// inside clipping region
		glScissor(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// get source region
		double sx1 = req->src_rect.x + src_xoff;
		double sy1 = req->src_rect.y + src_yoff;
		double sx2 = req->src_rect.x + req->src_rect.w - 1 + src_xoff;
		double sy2 = req->src_rect.y + req->src_rect.h - 1 + src_yoff;

		// normalize source region
		sx1 = sx1 / (req->source->config.w - 1);
		sy1 = sy1 / (req->source->config.h - 1);
		sx2 = sx2 / (req->source->config.w - 1);
		sy2 = sy2 / (req->source->config.h - 1);

		// get destination region
		int dx1 = req->dst_rect.x + xoff;
		int dy1 = req->dst_rect.y + yoff;
		int dx2 = req->dst_rect.x + req->dst_rect.w - 1 + xoff;
		int dy2 = req->dst_rect.y + req->dst_rect.h - 1 + yoff;

		// blit source texture to the destination
		glBegin(GL_QUADS);
			glTexCoord2f(sx1, sy1);
			glVertex2i(dx1, dy1);

			glTexCoord2f(sx2, sy1);
			glVertex2i(dx2, dy1);

			glTexCoord2f(sx2, sy2);
			glVertex2i(dx2, dy2);

			glTexCoord2f(sx1, sy2);
			glVertex2i(dx1, dy2);
		glEnd();
	}

	// all is fine
	glDisable(GL_SCISSOR_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#endif
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
#ifdef  __HAVE_OPENGL__
	// lock destination fbo and bind source texture to it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_TEXTURE_2D);

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
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexImage2D(GL_TEXTURE_2D,
	 	0,
	 	GL_RGBA,
	 	req->src_width,
	 	req->src_height,
	 	0,
	 	GL_RGBA,
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
		glScissor(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// get source region
		double sx1 = req->src_rect.x;
		double sy1 = req->src_rect.y;
		double sx2 = req->src_rect.x + req->src_rect.w - 1;
		double sy2 = req->src_rect.y + req->src_rect.h - 1;

		// normalize source region
		sx1 = sx1 / (req->src_width - 1);
		sy1 = sy1 / (req->src_height - 1);
		sx2 = sx2 / (req->src_width - 1);
		sy2 = sy2 / (req->src_height - 1);

		// get destination region
		int dx1 = req->dst_rect.x + xoff;
		int dy1 = req->dst_rect.y + yoff;
		int dx2 = req->dst_rect.x + req->dst_rect.w - 1 + xoff;
		int dy2 = req->dst_rect.y + req->dst_rect.h - 1 + yoff;

		// blit source texture to the destination
		glBegin(GL_QUADS);
			glTexCoord2f(sx1, sy1);
			glVertex2i(dx1, dy1);

			glTexCoord2f(sx2, sy1);
			glVertex2i(dx2, dy1);

			glTexCoord2f(sx2, sy2);
			glVertex2i(dx2, dy2);

			glTexCoord2f(sx1, sy2);
			glVertex2i(dx1, dy2);
		glEnd();
	}

	// all is fine
	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &texture);
	glDisable(GL_SCISSOR_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);


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
#ifdef  __HAVE_OPENGL__



	// get the first destination ptr/pitch
	void *dst_ptr = NULL;
	int dst_pitch = 0;

	// lock font and destination surface
//	MMSFBSURFACE_BLIT_TEXT_INIT(2);
	int DY = 0;   req->surface->config.font->getHeight(&DY);
	int desc = 0; req->surface->config.font->getDescender(&desc);
	DY -= desc + 1; \
	int dst_pitch_pix = dst_pitch >> 2;

	// for all characters
	unsigned int OLDDST = 0;
	unsigned int OLDSRC = 0;
	register unsigned int d = 0;
//	register unsigned int SRCPIX = 0xff000000 | (((unsigned int)color.r) << 16) | (((unsigned int)color.g) << 8) | (unsigned int)color.b;
	MMSFBFONT_GET_UNICODE_CHAR(req->text, req->len) {

		// load the glyph
//		MMSFBSURFACE_BLIT_TEXT_LOAD_GLYPH(character);

		int			  src_pitch_pix;
		int 		  src_w;
		int 		  src_h;
		unsigned char *src;
		MMSFBFont_Glyph *glyph = req->surface->config.font->getGlyph(character);
		if (glyph) {
			src_pitch_pix = glyph->pitch;
			src_w         = glyph->width;
			src_h         = glyph->height;
			src           = glyph->buffer;
		}


		// start rendering of glyph to destination
//		MMSFBSURFACE_BLIT_TEXT_START_RENDER(unsigned int);

		if (glyph) {

			int dx = req->x + glyph->left;
			int dy = req->y + DY - glyph->top;

////////////////////////////////


			// lock destination fbo and bind source texture to it
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
			glDisable(GL_SCISSOR_TEST);
			glEnable(GL_TEXTURE_2D);

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
		    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		    // specify two-dimensional texture image
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


			 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			// setup blitting
			INIT_OGL_BLITTING(req->surface);

//		glDisable(GL_BLEND);
		glEnable(GL_BLEND);
		glColor4ub(255, 255, 255, 255);

			// get subsurface offsets
			GET_OFFS(req->surface);

			// set the clip to ogl
			MMSFBRectangle crect;
			if (req->surface->calcClip(dx + xoff, dy + yoff, src_w, src_h, &crect)) {
				// inside clipping region
				glScissor(crect.x, crect.y, crect.w, crect.h);
				glEnable(GL_SCISSOR_TEST);

				// get source region, we have to adjust the right x position because of pitch != width
				double sx1 = 0;
				double sy1 = 0;
				double sx2 = (double)src_w / (double)src_pitch_pix;
				double sy2 = 1;

				// get destination region
				int dx1 = dx + xoff;
				int dy1 = dy + yoff;
				int dx2 = dx + src_w - 1 + xoff;
				int dy2 = dy + src_h - 1 + yoff;

				// blit source texture to the destination
				glBegin(GL_QUADS);
					glTexCoord2f(sx1, sy1);
					glVertex2i(dx1, dy1);

					glTexCoord2f(sx2, sy1);
					glVertex2i(dx2, dy1);

					glTexCoord2f(sx2, sy2);
					glVertex2i(dx2, dy2);

					glTexCoord2f(sx1, sy2);
					glVertex2i(dx1, dy2);
				glEnd();
			}

			// all is fine
			glDisable(GL_TEXTURE_2D);
			glDeleteTextures(1, &texture);
			glDisable(GL_SCISSOR_TEST);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);





///////////////////////////////





#ifdef fffff
/*			if (dx < clipreg.x1) { \
				src_w -= clipreg.x1 - dx; \
				src   += clipreg.x1 - dx; \
				dx     = clipreg.x1; } \
			if (dx + src_w - 1 > clipreg.x2) src_w = clipreg.x2 - dx + 1; \
			if (dy < clipreg.y1) { \
				src_h -= clipreg.y1 - dy; \
				src   +=(clipreg.y1 - dy) * src_pitch_pix; \
				dy     = clipreg.y1; } \
			if (dy + src_h - 1 > clipreg.y2) src_h = clipreg.y2 - dy + 1; \*/
			unsigned char *src_end = src + src_h * src_pitch_pix;
			unsigned char *line_end = src + src_w;
			int src_pitch_pix_diff = src_pitch_pix - src_w;
			int dst_pitch_pix_diff = dst_pitch_pix - src_w;
//			pt *dst = ((pt *)dst_ptr) + dx + dy * dst_pitch_pix;

		// through the pixels
		while (src < src_end) {
			while (src < line_end) {
				// load pixel from memory
				register unsigned int SRC = *src;

				// is the source alpha channel 0x00 or 0xff?
				register unsigned int A = SRC;
				if (A == 0xff) {
					// source pixel is not transparent, copy it directly to the destination
//					*dst = SRCPIX;
				}
				else
				if (A) {
					// source alpha is > 0x00 and < 0xff
/*					register unsigned int DST = *dst;

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
				    A++;
				    r += (A * color.r) >> 8;
				    g += (A * color.g) >> 8;
				    b += (A * color.b) >> 8;
				    d =   ((a >> 8) ? 0xff000000 : (a << 24))
						| ((r >> 8) ? 0xff0000   : (r << 16))
						| ((g >> 8) ? 0xff00     : (g << 8))
				    	| ((b >> 8) ? 0xff 		 :  b);
					*dst = d;*/
				}

				src++;
//				dst++;
			}
			line_end+= src_pitch_pix;
			src     += src_pitch_pix_diff;
//			dst     += dst_pitch_pix_diff;
		}
#endif
		// prepare for next loop
//		MMSFBSURFACE_BLIT_TEXT_END_RENDER;
		req->x+=glyph->advanceX >> 6; }
	}}



#endif
}

