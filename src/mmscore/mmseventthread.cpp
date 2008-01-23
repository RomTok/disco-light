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

#include "mmseventthread.h"

MMSEventThread::MMSEventThread(MMSOSDPluginHandler *handler, IMMSEvent *event) {
    this->osdHandler = handler;
    this->centralHandler = NULL;
    this->backendHandler = NULL;
    this->event = event;
}

MMSEventThread::MMSEventThread(MMSCentralPluginHandler *handler, IMMSEvent *event) {
    this->osdHandler = NULL;
    this->backendHandler = NULL;
    this->centralHandler = handler;
    this->event = event;
}
MMSEventThread::MMSEventThread(MMSBackendPluginHandler *handler, IMMSEvent *event) {
    this->osdHandler = NULL;
    this->centralHandler = NULL;
    this->backendHandler = handler;
    this->event = event;
}


MMSEventThread::~MMSEventThread() {
	delete this->event;
}

void MMSEventThread::threadMain() {
    if (this->osdHandler)
        this->osdHandler->invokeOnEvent(this->event);
    else if(this->centralHandler)
        this->centralHandler->invokeOnEvent(this->event);
    else if(this->backendHandler)
        this->backendHandler->invokeOnEvent(this->event);
    delete this;
}

IMMSEvent *MMSEventThread::getEvent() {
    return this->event;
}

