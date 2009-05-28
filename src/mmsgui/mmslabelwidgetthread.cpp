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

#include "mmsgui/mmslabelwidgetthread.h"

MMSLabelWidgetThread::MMSLabelWidgetThread(MMSLabelWidget *label) {
    this->label = label;
    this->inWait = false;
    this->stopThread = false;
    this->pauseThread = false;
}

void MMSLabelWidgetThread::wait(unsigned int delaytime) {
    this->inWait = true;
    usleep(delaytime);
    while (this->pauseThread)
        usleep(1000);
    this->inWait = false;
}

void MMSLabelWidgetThread::stop() {
    while (!this->inWait)
        usleep(1000);
    this->stopThread = true;
    while (isRunning())
        usleep(1000);
}

void MMSLabelWidgetThread::pause(bool pauseThread) {
    if (pauseThread) {
        while (!this->inWait)
            usleep(1000);
        this->pauseThread = true;
    }
    else {
        this->pauseThread = false;
    }
}

void MMSLabelWidgetThread::doIt() {

    // init
    this->inWait = false;
    this->stopThread = false;
    this->pauseThread = false;
    int refresh_frame_delay = 0;
	int pixel_shift = 1;
	int refresh_time = 0;

    // maximum cpy usage in percent
	int max_cpu_usage = 33;

    while (1) {
        /* first check if the window is shown */
        MMSWindow *win = this->label->getRootWindow();
        if (!win) {
            wait(1000000);
            if (this->stopThread)
                return;
            else
                continue;
        }
        if (!win->isShown(true)) {
            wait(1000000);
            if (this->stopThread)
                return;
            else
                continue;
        }

        bool changed = false;
        if (label->getSlidable()) {
            if (label->slide_width > 0) {
            	MMSFBRectangle surfaceGeom = label->getSurfaceGeometry();
                if (label->slide_width > surfaceGeom.w) {
                	// we should slide the label text
                	if (label->slide_offset >= label->slide_width)
                		// from the beginning
                		label->slide_offset = -surfaceGeom.w;
                	else
                		// increase offset
                		label->slide_offset+=pixel_shift;
                	changed = true;
                }
            }
        }

        /* refresh the widget */
        if (changed) {
            unsigned int start_ts;
            unsigned int end_ts;

            // get start timestamp if needed
            if (!this->label->frame_delay_set)
            	start_ts = getMTimeStamp();

            // update screen
            this->label->refresh();

	        // recalc speed parameters?
	        if (!this->label->frame_delay_set) {
	        	// get refresh duration
	        	end_ts = getMTimeStamp();
				if (!refresh_time)
					refresh_time = getMDiff(start_ts, end_ts);
				else
					refresh_time = (refresh_time * 2 + getMDiff(start_ts, end_ts)) / 3;

				// calc shift and frame delay
				int slide_speed = label->getSlideSpeed();
				if (slide_speed <= 0) slide_speed = 1;
				if (slide_speed > 255) slide_speed = 255;
				int max_sleep_time = 1000 / slide_speed;
	        	int total_time = (refresh_time * 100) / max_cpu_usage;
	        	pixel_shift = total_time / max_sleep_time;
	        	if (total_time % max_sleep_time) pixel_shift++;
	        	this->label->frame_delay = max_sleep_time * pixel_shift - refresh_time - pixel_shift;
	        	if (this->label->frame_delay <= 0) this->label->frame_delay = 1;

	        	// mark as calculated
	            this->label->frame_delay_set = true;
	        }
	        else {
	        	// recalc not needed
	        	refresh_frame_delay++;
	        	if (refresh_frame_delay >= 50) {
	        		// after 50 times, recalc requested
	        		refresh_frame_delay = 0;
	        		this->label->frame_delay_set = false;
	        	}
	        }
        }

        // sleep
        if (this->label->frame_delay > 0)
            wait(this->label->frame_delay*1000);
        else
            wait(1000000);
        if (this->stopThread)
            return;
    }
}

void MMSLabelWidgetThread::threadMain() {
    doIt();
    delete this;
}

