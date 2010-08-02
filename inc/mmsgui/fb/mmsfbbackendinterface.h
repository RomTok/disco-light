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

#ifndef MMSFBBACKENDINTERFACE_H_
#define MMSFBBACKENDINTERFACE_H_

#include "mmstools/mmsthreadserver.h"
#include "mmsgui/fb/mmsfbsurface.h"

class MMSFBBackEndInterface : public MMSThreadServer {
private:
	typedef enum {
		BEI_REQUEST_TYPE_CLEAR = 0,
		BEI_REQUEST_TYPE_FILLRECTANGLE,
		BEI_REQUEST_TYPE_FILLTRIANGLE,
		BEI_REQUEST_TYPE_DRAWLINE,
		BEI_REQUEST_TYPE_DRAWRECTANGLE,
		BEI_REQUEST_TYPE_DRAWTRIANGLE,
		BEI_REQUEST_TYPE_STRETCHBLIT
	} BEI_REQUEST_TYPE;

	typedef struct {
		BEI_REQUEST_TYPE	type;
		MMSFBSurface		*surface;
		MMSFBColor			color;
	} BEI_CLEAR;

	typedef struct {
		BEI_REQUEST_TYPE	type;
		MMSFBSurface		*surface;
		MMSFBRectangle		rect;
	} BEI_FILLRECTANGLE;

	typedef struct {
		BEI_REQUEST_TYPE	type;
		MMSFBSurface		*surface;
		MMSFBTriangle		triangle;
	} BEI_FILLTRIANGLE;

	typedef struct {
		BEI_REQUEST_TYPE	type;
		MMSFBSurface		*surface;
		MMSFBRegion			region;
	} BEI_DRAWLINE;

	typedef struct {
		BEI_REQUEST_TYPE	type;
		MMSFBSurface		*surface;
		MMSFBRectangle		rect;
	} BEI_DRAWRECTANGLE;

	typedef struct {
		BEI_REQUEST_TYPE	type;
		MMSFBSurface		*surface;
		MMSFBTriangle		triangle;
	} BEI_DRAWTRIANGLE;

	typedef struct {
		BEI_REQUEST_TYPE	type;
		MMSFBSurface		*surface;
		MMSFBSurface		*source;
		MMSFBRectangle		src_rect;
		MMSFBRectangle		dst_rect;
	} BEI_STRETCHBLIT;

	void processData(void *in_data, int in_data_len, void **out_data, int *out_data_len);
	void processClear(BEI_CLEAR *req);
	void processFillRectangle(BEI_FILLRECTANGLE *req);
	void processFillTriangle(BEI_FILLTRIANGLE *req);
	void processDrawLine(BEI_DRAWLINE *req);
	void processDrawRectangle(BEI_DRAWRECTANGLE *req);
	void processDrawTriangle(BEI_DRAWTRIANGLE *req);
	void processStretchBlit(BEI_STRETCHBLIT *req);

public:
	MMSFBBackEndInterface(int queue_size = 1000);

	void clear(MMSFBSurface *surface, MMSFBColor &color);
	void fillRectangle(MMSFBSurface *surface, MMSFBRectangle &rect);
	void fillTriangle(MMSFBSurface *surface, MMSFBTriangle &triangle);
	void drawLine(MMSFBSurface *surface, MMSFBRegion &region);
	void drawRectangle(MMSFBSurface *surface, MMSFBRectangle &rect);
	void drawTriangle(MMSFBSurface *surface, MMSFBTriangle &triangle);
	void stretchBlit(MMSFBSurface *surface, MMSFBSurface *source, MMSFBRectangle &src_rect, MMSFBRectangle &dst_rect);
};

#endif /* MMSFBBACKENDINTERFACE_H_ */