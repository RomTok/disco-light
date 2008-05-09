/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
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

#include "mmsconfig/mmsplugindata.h"
#include "mmsconfig/mmsimportpropertydata.h"

MMSImportPropertyData::MMSImportPropertyData() {
    this->id = -1;
    this->pluginId = 0;
    this->onStartUp = false;
    this->time = 0;
    this->interval = 0;
}        

MMSImportPropertyData::~MMSImportPropertyData() {
}

int MMSImportPropertyData::getId() {
    return this->id;
}

void MMSImportPropertyData::setId(int id) {
    this->id = id;   
}

int MMSImportPropertyData::getPluginId() {
    return this->pluginId;
}

void MMSImportPropertyData::setPluginId(int pluginId) {
    this->pluginId = pluginId;   
}

bool MMSImportPropertyData::getOnStartUp(void) {
    return this->onStartUp;
}

void MMSImportPropertyData::setOnStartUp(bool onStartUp) {
    this->onStartUp = onStartUp;
}

int MMSImportPropertyData::getTime(void) {
    return this->time;
}

void MMSImportPropertyData::setTime(int time) {
    this->time = time;
}

int MMSImportPropertyData::getInterval(void) {
    return this->interval;
}

void MMSImportPropertyData::setInterval(int interval) {
    this->interval = interval;
}
