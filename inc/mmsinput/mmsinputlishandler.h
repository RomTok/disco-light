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
#define MMSINPUTLISHANDLER_DEVTYPE_TOUCHSCREEN	"TOUCHSCREEN"

typedef struct {
	unsigned int xRes;
	unsigned int yRes;
	float 	xFactor;		/**< multiplicate the x value to get the real value (touchscreen only) */
	float 	yFactor;		/**< multiplicate the y value to get the real value (touchscreen only) */
	bool  	swapX;			/**< swap x axis */
	bool  	swapY;			/**< swap y axis */
	bool	swapXY;			/**< swap x and y axis */
} MMSINPUTLISHANDLER_DEV_TOUCH;

typedef struct {
	string 	name;
	string	desc;
	string	type;
	union {
		MMSINPUTLISHANDLER_DEV_TOUCH touch;
	};
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
