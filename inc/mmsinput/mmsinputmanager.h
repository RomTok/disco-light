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

#ifndef MMSINPUTMANAGER_
#define MMSINPUTMANAGER_

#include "mmstools/mmstools.h"
#include "mmsinput/mmsinputhandler.h"
#include "mmsinput/mmsinputthread.h"
#include "mmsgui/interfaces/immswindowmanager.h"
#include "mmsinput/mmsinputmapper.h"

#include <vector>

class MMSInputManager  {
	private:
		MMSMutex mutex;
		void handleInput(MMSInputEvent *inputevent);
		vector <MMSInputThread *> threads;
		IMMSWindowManager *windowmanager;
		vector <class MMSInputSubscription *> subscriptions;
		MMSKeySymbol lastkey;
		MMSInputMapper *mapper;
		MMSConfigData *config;

		//! store the window on which the button was pressed
		MMSWindow 	*buttonpress_window;
		bool		button_pressed;

	public:
		MMSInputManager(string file, string name);
		~MMSInputManager();
		void addDevice(MMS_INPUT_DEVICE device, int inputinterval);
		void setWindowManager(IMMSWindowManager *wm);
		void addSubscription(class MMSInputSubscription *sub);
		void startListen();
		void stopListen();

	friend class MMSInputThread;
};

#include "mmsinputsubscription.h"

#endif /*MMSINPUTMANAGER_*/
