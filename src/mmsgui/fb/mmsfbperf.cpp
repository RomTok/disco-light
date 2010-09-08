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

#include "mmsgui/fb/mmsfbperf.h"

#ifdef __ENABLE_MMSFBPERF__

MMSFBPerf::MMSFBPerf() {
	memset(this->mvals, 0, sizeof(this->mvals));
	start();
}

MMSFBPerf::~MMSFBPerf() {
}

void MMSFBPerf::startMeasuringFillRectangle(struct timeval *perf_stime) {
	// get start time
	gettimeofday(perf_stime, NULL);
}

void MMSFBPerf::stopMeasuringFillRectangle(struct timeval *perf_stime,
					MMSFBSurfacePixelFormat pixelformat, int w, int h) {

	// get stop time
	struct timeval perf_etime;
	gettimeofday(&perf_etime, NULL);

	// access the mvals for specified pixelformat
	MEASURING_VALS *mvals = &this->mvals[pixelformat];

	// calculate pixels
	mvals->rpixels+= w * h;
	if (mvals->rpixels > 1000000) {
		mvals->mpixels+= mvals->rpixels / 1000000;
		mvals->rpixels%= 1000000;
	}

	// calculate time
	mvals->usecs+= (perf_etime.tv_sec - perf_stime->tv_sec) * 1000000;
	if (perf_etime.tv_usec >= perf_stime->tv_usec)
		mvals->usecs+= perf_etime.tv_usec - perf_stime->tv_usec;
	else
		mvals->usecs+= 1000000 - perf_stime->tv_usec + perf_etime.tv_usec;
	if (mvals->usecs == 0) mvals->usecs = 1;

	// calculate mpps (mega pixel per second)
	mvals->mpps= mvals->mpixels * 1000000 / mvals->usecs;
}

void MMSFBPerf::threadMain() {
	while (1) {
		for (unsigned int i = 0; i < MMSFB_PF_CNT; i++) {
			MEASURING_VALS *mvals = &this->mvals[i];
			if (!mvals->usecs)
				continue;

			printf(">>>>>>>>>> %d.%03d mp, %d us, %d mpps\n",
					mvals->mpixels,
					mvals->rpixels / 1000,
					mvals->usecs,
					mvals->mpps);
		}
		fflush(stdout);
		sleep(1);
	}
}

MMSFBPerf *mmsfbperf = new MMSFBPerf();

#endif
