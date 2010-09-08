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

#ifndef MMSFBPERF_H_
#define MMSFBPERF_H_

//#define __ENABLE_MMSFBPERF__

#ifdef __ENABLE_MMSFBPERF__

#include "mmstools/mmsthread.h"
#include "mmsgui/fb/mmsfbsurface.h"

#define MMSFBPERF_START_MEASURING_FILLRECT { \
	struct timeval perf_stime; \
	mmsfbperf->startMeasuringFillRectangle(&perf_stime);

#define MMSFBPERF_STOP_MEASURING_FILLRECT \
	mmsfbperf->stopMeasuringFillRectangle( \
			&perf_stime, \
			this->config.surface_buffer->pixelformat, sw, sh); }


class MMSFBPerf: public MMSThread {
private:
	typedef struct {
		unsigned int mpixels;
		unsigned int rpixels;
		unsigned int usecs;
		unsigned int mpps;
	} MEASURING_VALS;

	MEASURING_VALS mvals[MMSFB_PF_CNT];

	void threadMain();

public:
	MMSFBPerf();
	~MMSFBPerf();

	void startMeasuringFillRectangle(struct timeval *perf_stime);

	void stopMeasuringFillRectangle(struct timeval *perf_stime,
			MMSFBSurfacePixelFormat pixelformat, int w, int h);

};

extern MMSFBPerf *mmsfbperf;

#else

#define MMSFBPERF_START_MEASURING_FILLRECT
#define MMSFBPERF_STOP_MEASURING_FILLRECT

#endif

#endif /* MMSFBPERF_H_ */
