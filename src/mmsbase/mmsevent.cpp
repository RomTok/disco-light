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

#include "mmsbase/mmsevent.h"

MMSEvent::MMSEvent(string heading) {
    this->heading = heading;
}

void MMSEvent::setHeading(string heading) {
    this->heading=heading;
}

string MMSEvent::getHeading() {
    return this->heading;
}

string MMSEvent::getData(string key) {
	std::map<string, string>::iterator it;
	it = data.find(key);
	if(it!=data.end())
    	return it->second;
    else
    	return "";
}

void MMSEvent::setData(string key, string value) {
	data.insert(std::make_pair(key,value));
}

void MMSEvent::clear() {
	data.clear();
}

void MMSEvent::send() {
    this->dispatcher->raise(this);
}

void MMSEvent::sendTo(int pluginid) {
    this->dispatcher->raise(this,pluginid);
}

void MMSEvent::setDispatcher(IMMSEventDispatcher *dispatcher) {
    this->dispatcher = dispatcher;
}

IMMSEventDispatcher *MMSEvent::dispatcher = NULL;
