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

#include "mmsgui/mmsanimationthread.h"

MMSAnimationThread::MMSAnimationThread() : MMSThread("MMSAnimationThread")  {
	// animation is not running
	this->animRunning = false;

	// set attributes
	setStepsPerSecond(25);
	setMaxCPUUsage(75);
	setMaxFrameRate(25);
	setDuration(0);

	// reset all other values
	reset();
}

MMSAnimationThread::~MMSAnimationThread() {
}

void MMSAnimationThread::reset() {
	// reset all values
	this->recalc_requested	= true;
	this->recalc_cnt		= 0;
	this->recalc_interval	= 2;
	this->step_len			= 0;
	this->offset			= 0;
	this->process_time		= 0;
	this->frame_delay		= 0;
	this->frame_rate		= 0;
	this->frames			= 0;
	this->times_buf_pos		= 0;
	this->times_buf_cnt		= 0;
	this->real_duration		= 0;
}

void MMSAnimationThread::threadMain() {
	// reset all values
	reset();

	// call connected onBeforeAnimation callbacks
    if (!this->onBeforeAnimation.emit(this)) {
    	// one of the connected callbacks has failed, do not start the animation
    	return;
    }

	// get start timestamp
	this->anim_start = getMTimeStamp();

	while (1) {
		// start and end timestamps for CPU average calculation
		unsigned int start_ts;
		unsigned int end_ts;

		// get start timestamp if needed
		if (this->recalc_requested) {
			start_ts = getMTimeStamp();
		}

		// call connected onAnimation callbacks
	    if (!this->onAnimation.emit(this)) {
	    	// one of the connected callbacks has "failed", stop the animation
		    break;
	    }

		// recalc speed parameters?
		if (this->recalc_requested) {
			// get end timestamp
			end_ts = getMTimeStamp();

			// get process time
			// we collect up to MMSANIMATIONTHREAD_TIMES_BUF_SIZE times in a ring buffer
			// and calculate the CPU average
			unsigned int times = 0;
			unsigned int diff = getMDiff(start_ts, end_ts);
			this->times_buf[times_buf_pos++] = diff;
			if (this->times_buf_pos >= MMSANIMATIONTHREAD_TIMES_BUF_SIZE) this->times_buf_pos = 0;
			if (this->times_buf_cnt < MMSANIMATIONTHREAD_TIMES_BUF_SIZE) this->times_buf_cnt++;
			for (unsigned int i = 0; i < this->times_buf_cnt; i++) times+= this->times_buf[i];
			this->process_time = (times + diff / 2) / this->times_buf_cnt;

			// calculate step length and frame delay
			int max_duration = 1000 / this->steps_per_second;
			int total_time = (this->process_time * 100) / this->max_cpu_usage;
			this->step_len = (total_time + max_duration - 1) / max_duration;
			this->frame_delay = max_duration * this->step_len - this->process_time;
			if (this->frame_delay <= 0) this->frame_delay = 1;
			int frame_duration = this->frame_delay + this->process_time;
			this->frame_rate = 1000 / frame_duration;

			if (this->frame_rate > this->max_frame_rate) {
				// calculated frame rate is greater than max frame rate
				// so we have to calculate back the step length and the frame delay
				this->frame_delay = 1000 / this->max_frame_rate - this->process_time;
				frame_duration = this->frame_delay + this->process_time;
				this->step_len = (frame_duration + max_duration - 1) / max_duration;
				this->frame_delay = max_duration * this->step_len - this->process_time;
				if (this->frame_delay <= 0) this->frame_delay = 1;
				frame_duration = this->frame_delay + this->process_time;
				this->frame_rate = 1000 / frame_duration;
			}

			// mark as calculated
			this->recalc_cnt = 1;
			this->recalc_requested = false;
		}
		else {
			// recalc not needed
			this->recalc_cnt++;
			if (this->recalc_cnt >= this->recalc_interval) {
				// after recalc_interval times, recalc requested
				this->recalc_cnt = 0;
				this->recalc_requested = true;
			}
		}

		// increase the frame counter
		this->frames++;

		// increase offset with step length
		this->offset+= this->step_len;

		// sleeping...
        usleep((this->frame_delay>0)?this->frame_delay*1000:1000);

    	// get real animation duration
    	this->anim_end = getMTimeStamp();
    	this->real_duration = getMDiff(this->anim_start, this->anim_end);

    	if ((this->duration) && (this->real_duration > this->duration)) {
    		// requested duration reached, stop the animation
    		break;
    	}
	}

	// call connected onAfterAnimation callbacks
    this->onAfterAnimation.emit(this);
}

void MMSAnimationThread::start(bool separate_thread) {
	if (!isRunning()) {
		if (separate_thread) {
			// start animation in a separate thread context
			MMSThread::start();
		}
		else {
			// start animation in the current thread context
			this->animRunning = true;
			threadMain();
			this->animRunning = false;
		}
	}
}

bool MMSAnimationThread::isRunning() {
	if (MMSThread::isRunning()) {
		// separate thread is running
		return true;
	}
	else {
		// check if running without separate thread
		return this->animRunning;
	}
}

bool MMSAnimationThread::setStepsPerSecond(int steps_per_second) {
	// check & set
	if (steps_per_second <= 0) return false;
	if (steps_per_second > 255) return false;
	this->steps_per_second = steps_per_second;

	// recalculation requested
	recalc_cnt = 0;
	recalc_requested = true;

	return true;
}

int MMSAnimationThread::getStepsPerSecond() {
	return this->steps_per_second;
}

bool MMSAnimationThread::setMaxCPUUsage(int max_cpu_usage) {
	// check & set
	if (max_cpu_usage < 10) return false;
	if (max_cpu_usage > 100) return false;
	this->max_cpu_usage = max_cpu_usage;

	// recalculation requested
	recalc_cnt = 0;
	recalc_requested = true;

	return true;
}

int MMSAnimationThread::getMaxCPUUsage() {
	return this->max_cpu_usage;
}

bool MMSAnimationThread::setMaxFrameRate(int max_frame_rate) {
	// check & set
	if (max_frame_rate < 10) return false;
	if (max_frame_rate > 100) return false;
	this->max_frame_rate = max_frame_rate;

	// recalculation requested
	recalc_cnt = 0;
	recalc_requested = true;

	return true;
}

int MMSAnimationThread::getMaxFrameRate() {
	return this->max_frame_rate;
}

int MMSAnimationThread::getFrameRate() {
	return this->frame_rate;
}

int MMSAnimationThread::getFrameDelay() {
	return this->frame_delay;
}

unsigned int MMSAnimationThread::getFrames() {
	return this->frames;
}

int MMSAnimationThread::getStepLength() {
	return this->step_len;
}

int MMSAnimationThread::getOffset() {
	return this->offset;
}

bool MMSAnimationThread::setDuration(unsigned int duration) {
	this->duration = duration;
	return true;
}

unsigned int MMSAnimationThread::getDuration() {
	return this->duration;
}

unsigned int MMSAnimationThread::getRealDuration() {
	return this->real_duration;
}
