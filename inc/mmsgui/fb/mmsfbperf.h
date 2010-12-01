/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2010 BerLinux Solutions GmbH                       *
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

#ifndef MMSFBPERF_H_
#define MMSFBPERF_H_

#include "mmstools/mmsthread.h"
#include "mmsgui/fb/mmsfbsurface.h"

#ifdef __ENABLE_PERFMON__

#define MMSFBPERF_START_MEASURING { \
	struct timeval perf_stime; \
	if (mmsfbperf) mmsfbperf->startMeasuring(&perf_stime);

#define MMSFBPERF_STOP_MEASURING_FILLRECT(surface, dw, dh) \
	if (mmsfbperf) mmsfbperf->stopMeasuringFillRectangle( \
					&perf_stime, surface, dw, dh); }

#define MMSFBPERF_STOP_MEASURING_DRAWLINE(surface, x1, y1, x2, y2) { \
	int w=x2-x1;if(w<0)w=-w;w++; \
	int h=y2-y1;if(h<0)h=-h;h++; \
	if (mmsfbperf) mmsfbperf->stopMeasuringDrawLine( \
					&perf_stime, surface, (w>=h)?w:h); }}

#define MMSFBPERF_STOP_MEASURING_DRAWSTRING(surface, clipreg, text, len, x, y) { \
	int w, h; \
	surface->config.font->getStringWidth(text, len, &w); \
	surface->config.font->getHeight(&h); \
	MMSFBRegion myreg = clipreg; \
	if (x+w-1 < myreg.x2) myreg.x2 = x+w-1; \
	if (y+h-1 < myreg.y2) myreg.y2 = y+h-1; \
	if (x > myreg.x1) myreg.x1 = x; \
	if (y > myreg.y1) myreg.y1 = y; \
	if ((myreg.x1 <= myreg.x2) && (myreg.y1 <= myreg.y2)) \
	if (mmsfbperf) mmsfbperf->stopMeasuringDrawString( \
					&perf_stime, surface, myreg.x2 - myreg.x1 + 1, myreg.y2 - myreg.y1 + 1); }}

#define MMSFBPERF_STOP_MEASURING_BLIT(surface, src_pixelformat, sw, sh) \
	if (mmsfbperf) mmsfbperf->stopMeasuringBlit( \
					&perf_stime, surface, src_pixelformat, sw, sh); }

#define MMSFBPERF_STOP_MEASURING_STRETCHBLIT(surface, source, sw, sh, dw, dh) \
	if (mmsfbperf) mmsfbperf->stopMeasuringStretchBlit( \
					&perf_stime, surface, src_pixelformat, sw, sh, dw, dh); }

#else

#define MMSFBPERF_START_MEASURING
#define MMSFBPERF_STOP_MEASURING_FILLRECT(surface, dw, dh)
#define MMSFBPERF_STOP_MEASURING_DRAWLINE(surface, x1, y1, x2, y2)
#define MMSFBPERF_STOP_MEASURING_DRAWSTRING(surface, clipreg, text, len, x, y)
#define MMSFBPERF_STOP_MEASURING_BLIT(surface, src_pixelformat, sw, sh)
#define MMSFBPERF_STOP_MEASURING_STRETCHBLIT(surface, source, sw, sh, dw, dh)

#endif

#if MMSFB_BLIT_CNT > MMSFB_DRAW_CNT
	#define MMSFBPERF_MAXFLAGS	MMSFB_BLIT_CNT
#else
	#define MMSFBPERF_MAXFLAGS	MMSFB_DRAW_CNT
#endif

typedef struct {
	unsigned int calls;
	unsigned int mpixels;
	unsigned int rpixels;
	unsigned int usecs;
	unsigned int mpps;
} MMSFBPERF_MEASURING_VALS;

typedef MMSFBPERF_MEASURING_VALS MMSFBPERF_MEASURING_LIST[MMSFB_PF_CNT][MMSFB_PF_CNT][MMSFBPERF_MAXFLAGS];

class MMSFBPerf {
private:

	//! already initialized?
	static bool initialized;

	//! statistic for fill rectangle routines
	static MMSFBPERF_MEASURING_LIST fillrect;

	//! statistic for draw line routines
	static MMSFBPERF_MEASURING_LIST drawline;

	//! statistic for draw string routines
	static MMSFBPERF_MEASURING_LIST drawstring;

	//! statistic for blitting routines
	static MMSFBPERF_MEASURING_LIST blit;

	//! statistic for stretchblit routines
	static MMSFBPERF_MEASURING_LIST stretchblit;

	int getPerfVals(MMSFBPERF_MEASURING_LIST *mlist, const char *prefix, char *retbuf, int retbuf_size);

	void stopMeasuring(struct timeval *perf_stime, MMSFBPERF_MEASURING_VALS *mvals,
					   int sw, int sh, int dw = 0, int dh = 0);

public:
	MMSFBPerf();
	~MMSFBPerf();

	void startMeasuring(struct timeval *perf_stime);

	void stopMeasuringFillRectangle(struct timeval *perf_stime,
									MMSFBSurface *surface, int w, int h);

	void stopMeasuringDrawLine(struct timeval *perf_stime,
							   MMSFBSurface *surface, int pixels);

	void stopMeasuringDrawString(struct timeval *perf_stime,
								 MMSFBSurface *surface, int w, int h);

	void stopMeasuringBlit(struct timeval *perf_stime,
						   MMSFBSurface *surface,
						   MMSFBSurfacePixelFormat src_pixelformat,
						   int sw, int sh);

	void stopMeasuringStretchBlit(struct timeval *perf_stime,
								  MMSFBSurface *surface,
								  MMSFBSurfacePixelFormat src_pixelformat,
								  int sw, int sh, int dw, int dh);

    friend class MMSFBPerfInterface;
};

#endif /* MMSFBPERF_H_ */
