/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
 ***************************************************************************/

#ifndef MMSINPUTLISTHREAD_H_
#define MMSINPUTLISTHREAD_H_

#include "mmstools/mmstools.h"
#include "mmsinput/mmsinputlishandler.h"

class MMSInputLISThread : public MMSThread {
	private:
		//! access to the input handler to buffer input events there
		MMSInputLISHandler	*handler;

		//! filedescriptor from which we read keyboard inputs (this should be the fd to the framebuffer console)
		int	kb_fd;

		// name of the device != keyboard
		MMSINPUTLISHANDLER_DEV device;

		//! filedescriptor of the device
		int dv_fd;

		//! shift key pressed
		bool shift_pressed;

		//! altgr key pressed
		bool altgr_pressed;

		//! is caps lock?
		bool is_caps_lock;

		//! button pressed?
		bool button_pressed;
		
		int lastX, lastY;

		bool openDevice();
		void closeDevice();

		MMSKeySymbol getSymbol(int code, unsigned short value);
		unsigned short readValue(unsigned char table, unsigned char index);
		MMSKeySymbol getKeyFromCode(bool pressed, unsigned char code);
		void updateLED();

		MMSKeySymbol translateKey(int code);
		bool translateEvent(struct input_event *linux_evt, MMSInputEvent *inputevent);

	public:
		MMSInputLISThread(MMSInputLISHandler *handler, int kb_fd);
		MMSInputLISThread(MMSInputLISHandler *handler, MMSINPUTLISHANDLER_DEV *device);
		~MMSInputLISThread();
		void threadMain();
};

#endif /* MMSINPUTLISTHREAD_H_ */
