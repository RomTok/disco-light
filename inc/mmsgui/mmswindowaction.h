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

#ifndef MMSWINDOWACTION_H_
#define MMSWINDOWACTION_H_

#include <sigc++/sigc++.h>
#include "mmstools/mmsidfactory.h"
#include "mmstools/mmsthread.h"

typedef enum {
	MMSWACTION_NONE = 0,
	MMSWACTION_SHOW = 1,
	MMSWACTION_HIDE = 2
} MMSWACTION;

class MMSWindowAction  : public MMSThread {

	private:
		class MMSWindow *window;
		MMSIdFactory	idFactory;
		int 			id;
		MMSWACTION 		action;
		bool			stopaction;

	public:
		MMSWindowAction(class MMSWindow* destination);
		virtual ~MMSWindowAction();
		void threadMain();
		void setAction(MMSWACTION action);
		MMSWACTION getAction();
		void cancelCleanup();
		void onCancelBroadcast(int windowtype);
/*		void onShow();
		void onHide();*/

		/* broadcast windowaction cancel */
		static sigc::signal<void,int> cancelBroadcast;
};


#include "mmswindow.h"

#endif /*MMSWINDOWACTION_H_*/
