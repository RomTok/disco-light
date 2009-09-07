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

#include "mmsinput/mmsinputlishandler.h"
#include "mmsinput/mmsinputlisthread.h"
#include "mmsgui/fb/mmsfb.h"
#include <string.h>
#include <typeinfo>
#include <linux/keyboard.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <linux/vt.h>

#include <linux/input.h>


MMSInputLISHandler::MMSInputLISHandler(MMS_INPUT_DEVICE device) {
#ifdef __HAVE_FBDEV__
	// init dev list
	this->devcnt = 0;

	// init event buffer
	this->ie_read_pos = 0;
	this->ie_write_pos = 0;

	// get access to the framebuffer console
	this->kb_fd = -1;
	if (mmsfb->mmsfbdev)
		mmsfb->mmsfbdev->vtGetFd(&this->kb_fd);

	// start the keyboard thread
	this->listhread = new MMSInputLISThread(this, this->kb_fd);
	if (this->listhread)
		this->listhread->start();

	// get other linux input devices and start separate threads
	getDevices();
	for (int i = 0; i < this->devcnt; i++) {
		if((this->devices[i].type == MMSINPUTLISHANDLER_DEVTYPE_REMOTE) ||
		   (this->devices[i].type == MMSINPUTLISHANDLER_DEVTYPE_TOUCHSCREEN)) {
			// we start input thread only for remote controls and touchscreens
			MMSInputLISThread *lt = new MMSInputLISThread(this, &this->devices[i]);
			if (lt)
				lt->start();
		}
	}
#else
	throw new MMSError(0,(string)typeid(this).name() + " is empty. compile FBDEV support!");
#endif
}

MMSInputLISHandler::~MMSInputLISHandler() {
}


#define NUMBITS(x)          ((((x)-1)/(sizeof(long)*8))+1)
#define TSTBIT(bit, array)	((array[(bit)/(sizeof(long)*8)] >> ((bit)%(sizeof(long)*8))) & 1)

bool MMSInputLISHandler::checkDevice() {
    // point to the device
    MMSINPUTLISHANDLER_DEV *dev = &this->devices[this->devcnt];

    // open the device
    int  fd;
    if ((fd = open(dev->name.c_str(), O_RDWR)) < 0)
         return false;

	// try to grab the device
	if (ioctl(fd, EVIOCGRAB, 1)) {
		close(fd);
		return false;
	}

	// get description
	char devdesc[256];
	memset(devdesc, 0, sizeof(devdesc));
	ioctl(fd, EVIOCGNAME(sizeof(devdesc)-1), devdesc);
	dev->desc = devdesc;
	dev->type = MMSINPUTLISHANDLER_DEVTYPE_UNKNOWN;
	dev->xFactor = dev->yFactor = 1;

	// try to find out the type of input device
	unsigned int  keys = 0;
	unsigned long ev_bit[NUMBITS(EV_MAX)];
	unsigned long key_bit[NUMBITS(KEY_MAX)];
	unsigned long abs_bit[NUMBITS(ABS_MAX)];

    // get event type of device
    ioctl(fd, EVIOCGBIT(0, sizeof(ev_bit)), ev_bit);
    if(TSTBIT(EV_KEY, ev_bit)) {
		ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(key_bit)), key_bit);
		for (int i = KEY_Q; i < KEY_M; i++)
			if (TSTBIT(i, key_bit))
				keys++;
		if(keys > 20)
			dev->type = MMSINPUTLISHANDLER_DEVTYPE_KEYBOARD;
		else {
			for (int i = KEY_OK; i < KEY_MAX; i++) {
				if (TSTBIT(i, key_bit)) {
						dev->type = MMSINPUTLISHANDLER_DEVTYPE_REMOTE;
					break;
				}
			}
		}
    }

	/* check for touchscreen (only ABS events are supported) */
    /* TODO: add button events */
	if(dev->type == MMSINPUTLISHANDLER_DEVTYPE_UNKNOWN) {
		if(ioctl(fd, EVIOCGBIT(EV_ABS, sizeof (abs_bit)), abs_bit) != -1) {
			if(TSTBIT(ABS_X, abs_bit) && TSTBIT(ABS_Y, abs_bit) && TSTBIT(ABS_PRESSURE, abs_bit)) {
				dev->type = MMSINPUTLISHANDLER_DEVTYPE_TOUCHSCREEN;
				struct input_absinfo abs;
				if(ioctl(fd, EVIOCGABS(ABS_X), &abs) != -1)
					dev->xFactor = mmsfb->display_w / abs.maximum;
				if(ioctl(fd, EVIOCGABS(ABS_Y), &abs) != -1)
					dev->yFactor = mmsfb->display_h / abs.maximum;
			}
		}
	}

	printf("Found %s, type=%s (%s)\n",
						dev->name.c_str(),
						dev->type.c_str(),
						dev->desc.c_str());

	// release device
	ioctl(fd, EVIOCGRAB, 0);
	close(fd);

	return true;
}

void MMSInputLISHandler::getDevices() {
	for (int i = 0; i < MMSINPUTLISHANDLER_MAX_DEVICES; i++) {
		this->devices[this->devcnt].name = "/dev/input/event" + iToStr(i);
		if (checkDevice())
			this->devcnt++;
	}
}

bool MMSInputLISHandler::addEvent(MMSInputEvent *inputevent) {
	this->lock.lock();
	int old_ie_write_pos;
	while (1) {
		// set new write pos for the event buffer
		old_ie_write_pos = this->ie_write_pos;
		this->ie_write_pos++;
		if (this->ie_write_pos >= MMSINPUTLISHANDLER_EVENT_BUFFER_SIZE)
			this->ie_write_pos = 0;
		if (this->ie_read_pos == this->ie_write_pos) {
			// buffer is full, waiting
			this->ie_write_pos = old_ie_write_pos;
			usleep(10000);
			continue;
		}
		break;
	}
	// add event
	this->ie_buffer[old_ie_write_pos] = *inputevent;
	this->lock.unlock();
	return true;
}

void MMSInputLISHandler::grabEvents(MMSInputEvent *inputevent) {
#ifdef __HAVE_FBDEV__
	while (1) {
		// is event buffer empty?
		if (this->ie_read_pos == this->ie_write_pos) {
			usleep(10000);
			continue;
		}

		// there is at least one event in the buffer, return next event
		*inputevent = this->ie_buffer[this->ie_read_pos];
		this->ie_read_pos++;
		if (this->ie_read_pos >= MMSINPUTLISHANDLER_EVENT_BUFFER_SIZE)
			this->ie_read_pos = 0;
		return;
	}
#else
	throw new MMSError(0,(string)typeid(this).name() + " is empty. compile FBDEV support!");
#endif
}

