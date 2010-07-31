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

#endif

MMSFBBackEndInterface::MMSFBBackEndInterface(int queue_size) : MMSThreadServer(queue_size, "MMSFBBackEndInterface") {

}

void MMSFBBackEndInterface::processData(void *in_data, int in_data_len, void **out_data, int *out_data_len) {
	if (!in_data) return;

	BEI_REQUEST_TYPE *type = (BEI_REQUEST_TYPE *)in_data;

	switch (*type) {
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
	default:
		break;
	}
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

	// set the clip to ogl
	MMSFBRectangle crect;
	if (req->surface->calcClip(0, 0, req->surface->config.w, req->surface->config.h, &crect)) {
		// inside clipping region
		glScissor(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// fill rectangle
		glRecti(0, 0, req->surface->config.w, req->surface->config.h);
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
	if (!req->surface->is_sub_surface) {
		// lock destination fbo and prepare it
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		// setup drawing
		INIT_OGL_DRAWING(req->surface);

		// set the clip to ogl
		MMSFBRectangle crect;
		if (req->surface->calcClip(req->rect.x, req->rect.y, req->rect.w, req->rect.h, &crect)) {
			// inside clipping region
			glScissor(crect.x, crect.y, crect.w, crect.h);
			glEnable(GL_SCISSOR_TEST);

			// fill rectangle
			glRecti(req->rect.x, req->rect.y, req->rect.x + req->rect.w - 1, req->rect.y + req->rect.h - 1);
		}

		// all is fine
		glDisable(GL_SCISSOR_TEST);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
#ifdef  __HAVE_OPENGL__
	if (!req->surface->is_sub_surface) {
		// lock destination fbo and prepare it
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		// setup drawing
		INIT_OGL_DRAWING(req->surface);

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

		if (req->surface->calcClip(x, y, w, h, &crect)) {
			// inside clipping region
			glScissor(crect.x, crect.y, crect.w, crect.h);
			glEnable(GL_SCISSOR_TEST);

			// fill triangle
			glBegin(GL_TRIANGLES);
			glVertex2i(req->triangle.x1, req->triangle.y1);
			glVertex2i(req->triangle.x2, req->triangle.y2);
			glVertex2i(req->triangle.x3, req->triangle.y3);
			glEnd();
		}

		// all is fine
		glDisable(GL_SCISSOR_TEST);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
#ifdef  __HAVE_OPENGL__
	if (!req->surface->is_sub_surface) {
		// lock destination fbo and prepare it
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		// setup drawing
		INIT_OGL_DRAWING(req->surface);

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

		if (req->surface->calcClip(x, y, w, h, &crect)) {
			// inside clipping region
			glScissor(crect.x, crect.y, crect.w, crect.h);
			glEnable(GL_SCISSOR_TEST);

			// draw line
			glBegin(GL_LINES);
			glVertex2i(req->region.x1, req->region.y1);
			glVertex2i(req->region.x2, req->region.y2);
			glEnd();
		}

		// all is fine
		glDisable(GL_SCISSOR_TEST);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
#ifdef  __HAVE_OPENGL__
	if (!req->surface->is_sub_surface) {
		// lock destination fbo and prepare it
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		// setup drawing
		INIT_OGL_DRAWING(req->surface);

		// set the clip to ogl
		MMSFBRectangle crect;
		if (req->surface->calcClip(req->rect.x, req->rect.y, req->rect.w, req->rect.h, &crect)) {
			// inside clipping region
			glScissor(crect.x, crect.y, crect.w, crect.h);
			glEnable(GL_SCISSOR_TEST);

			// draw rectangle
			glBegin(GL_LINE_STRIP);
			glVertex2i(req->rect.x, req->rect.y);
			glVertex2i(req->rect.x+req->rect.w-1, req->rect.y);
			glVertex2i(req->rect.x+req->rect.w-1, req->rect.y+req->rect.h-1);
			glVertex2i(req->rect.x, req->rect.y+req->rect.h-1);
			glVertex2i(req->rect.x, req->rect.y);
			glEnd();
		}

		// all is fine
		glDisable(GL_SCISSOR_TEST);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
#ifdef  __HAVE_OPENGL__
	if (!req->surface->is_sub_surface) {
		// lock destination fbo and prepare it
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, req->surface->config.surface_buffer->ogl_fbo);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		// setup drawing
		INIT_OGL_DRAWING(req->surface);

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

		if (req->surface->calcClip(x, y, w, h, &crect)) {
			// inside clipping region
			glScissor(crect.x, crect.y, crect.w, crect.h);
			glEnable(GL_SCISSOR_TEST);

			// draw triangle
			glBegin(GL_LINE_STRIP);
			glVertex2i(req->triangle.x1, req->triangle.y1);
			glVertex2i(req->triangle.x2, req->triangle.y2);
			glVertex2i(req->triangle.x3, req->triangle.y3);
			glVertex2i(req->triangle.x1, req->triangle.y1);
			glEnd();
		}

		// all is fine
		glDisable(GL_SCISSOR_TEST);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
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

	// set the clip to ogl
	MMSFBRectangle crect;
	if (req->surface->calcClip(req->dst_rect.x, req->dst_rect.y, req->dst_rect.w, req->dst_rect.h, &crect)) {
		// inside clipping region
		glScissor(crect.x, crect.y, crect.w, crect.h);
		glEnable(GL_SCISSOR_TEST);

		// get source region
		double sx1 = req->src_rect.x;
		double sy1 = req->src_rect.y;
		double sx2 = req->src_rect.w - req->src_rect.x - 1;
		double sy2 = req->src_rect.h - req->src_rect.y - 1;

		// normalize source region
		sx1 = sx1 / (req->source->config.w - 1);
		sy1 = sy1 / (req->source->config.h - 1);
		sx2 = sx2 / (req->source->config.w - 1);
		sy2 = sy2 / (req->source->config.h - 1);

		// get destination region
		int dx1 = req->dst_rect.x;
		int dy1 = req->dst_rect.y;
		int dx2 = req->dst_rect.x + req->dst_rect.w - 1;
		int dy2 = req->dst_rect.y + req->dst_rect.h - 1;

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

