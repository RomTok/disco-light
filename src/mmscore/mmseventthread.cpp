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

#include "mmscore/mmseventthread.h"

MMSEventThread::MMSEventThread(MMSOSDPluginHandler *handler, IMMSEvent _event) :
    osdHandler(handler),
    centralHandler(NULL),
    backendHandler(NULL),
    event(_event) {
}

MMSEventThread::MMSEventThread(MMSCentralPluginHandler *handler, IMMSEvent _event) :
    osdHandler(NULL),
    centralHandler(handler),
    backendHandler(NULL),
    event(_event) {
}
    
MMSEventThread::MMSEventThread(MMSBackendPluginHandler *handler, IMMSEvent _event) :
    osdHandler(NULL),
    centralHandler(NULL),
    backendHandler(handler),
    event(_event) {
}


MMSEventThread::~MMSEventThread() {
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
