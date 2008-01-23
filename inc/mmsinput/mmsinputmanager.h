/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
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

#include "mmstools.h"
#include "mmsinputhandler.h"
#include "mmsinputthread.h"
#include "interfaces/immswindowmanager.h"
#include "mmsinputmapper.h"

#include <vector>

class MMSInputManager  {
	private:
		MMSInputLogger logger;
		MMSMutex mutex;
		void handleInput(MMSInputEvent *inputevent);
		vector <MMSInputThread *> threads;
		IMMSWindowManager *windowmanager;
		vector <class MMSInputSubscription *> subscriptions;
		DFBInputDeviceKeySymbol lastkey;
		MMSInputMapper *mapper;
		MMSConfigData *config;

	public:
		MMSInputManager(string file, string name);				
		~MMSInputManager();
		void addDevice(DFBInputDeviceID device, int inputinterval);
		void setWindowManager(IMMSWindowManager *wm);
		void addSubscription(class MMSInputSubscription *sub);
		void startListen();
		void stopListen();
		
	friend class MMSInputThread;
};

#include "mmsinputsubscription.h"

#endif /*MMSINPUTMANAGER_*/
