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

#ifndef MMSINPUTLISHANDLER_H_
#define MMSINPUTLISHANDLER_H_

#include "mmsgui/mmsguitools.h"
#include "mmsbase/mmsbase.h"
#include "mmsinput/mmsinputhandler.h"

#define MMSINPUTLISHANDLER_EVENT_BUFFER_SIZE	100

class MMSInputLISHandler : public MMSInputHandler {
	private:
		//! event ring buffer
		MMSInputEvent 	ie_buffer[MMSINPUTLISHANDLER_EVENT_BUFFER_SIZE];

		//! event ring buffer, read pos
		int ie_read_pos;

		//! event ring buffer, write pos
		int ie_write_pos;

		bool shift_pressed;

		//! filedescriptor from which we read keyboard inputs (this should be the fd to the framebuffer console)
		int	kb_fd;

		MMSKeySymbol getSymbol(int code, unsigned short value);
		unsigned short readValue(unsigned char table, unsigned char index);
		MMSKeySymbol getKeyFromCode(bool pressed, unsigned char code);
		void readKeyboardEvents();
	public:
		MMSInputLISHandler(MMS_INPUT_DEVICE device);
		~MMSInputLISHandler();
		void grabEvents(MMSInputEvent *inputevent);
};

#endif /* MMSINPUTLISHANDLER_H_ */
