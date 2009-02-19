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

//! maximum number of linux input devices
#define MMSINPUTLISHANDLER_MAX_DEVICES 	16

#define MMSINPUTLISHANDLER_EVENT_BUFFER_SIZE	100

typedef int MMSINPUTLISHANDLER_DEVTYPE;

#define MMSINPUTLISHANDLER_DEVTYPE_UNKNOWN		"UNKNOWN"
#define MMSINPUTLISHANDLER_DEVTYPE_KEYBOARD		"KEYBOARD"
#define MMSINPUTLISHANDLER_DEVTYPE_REMOTE		"REMOTE"

typedef struct {
	string 	name;
	string	desc;
	string	type;
} MMSINPUTLISHANDLER_DEV;


class MMSInputLISHandler : public MMSInputHandler {
	private:
		//! available input devices
		MMSINPUTLISHANDLER_DEV devices[MMSINPUTLISHANDLER_MAX_DEVICES];

		//! number of available input devices
		int	devcnt;


		//! event ring buffer
		MMSInputEvent 	ie_buffer[MMSINPUTLISHANDLER_EVENT_BUFFER_SIZE];

		//! event ring buffer, read pos
		int ie_read_pos;

		//! event ring buffer, write pos
		int ie_write_pos;

		//! filedescriptor from which we read keyboard inputs (this should be the fd to the framebuffer console)
		int	kb_fd;


		class MMSInputLISThread	*listhread;


        //! lock
        MMSMutex lock;



        bool checkDevice();
        void getDevices();

		bool addEvent(MMSInputEvent *inputevent);

	public:
		MMSInputLISHandler(MMS_INPUT_DEVICE device);
		~MMSInputLISHandler();
		void grabEvents(MMSInputEvent *inputevent);

	// friends
	friend class MMSInputLISThread;
};

#endif /* MMSINPUTLISHANDLER_H_ */
