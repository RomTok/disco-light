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

#include "mmsconfig/mmsimportsourcedata.h"

MMSImportSourceData::MMSImportSourceData() {
    this->id = -1;
    this->pluginId = 0;
    this->name = "";
    this->source = "";
    this->lifeTime = 0;
}

MMSImportSourceData::~MMSImportSourceData() {
}

int MMSImportSourceData::getId() {
    return this->id;
}

void MMSImportSourceData::setId(int id) {
    this->id = id;
}

int MMSImportSourceData::getPluginId() {
    return this->pluginId;
}

void MMSImportSourceData::setPluginId(int pluginId) {
    this->pluginId = pluginId;
}

string MMSImportSourceData::getName() {
    return this->name;
}

void MMSImportSourceData::setName(string name) {
    this->name = name;
}

string MMSImportSourceData::getSource() {
    return this->source;
}

void MMSImportSourceData::setSource(string source) {
    this->source = source;
}

int MMSImportSourceData::getLifeTime() {
    return this->lifeTime;
}

void MMSImportSourceData::setLifeTime(int lifeTime) {
    this->lifeTime = lifeTime;
}
