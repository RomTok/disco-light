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

#include "mmsconfig/mmspropertydata.h"

MMSPropertyData::MMSPropertyData() {
    this->id = -1;
    this->parameter = "";
    this->max = 0;
    this->min = 0;
    this->separator = ';';
    this->value = "";
    this->issetindb = true;
}


void MMSPropertyData::setID(int id) {
    this->id = id;
}

int MMSPropertyData::getID() {
    return this->id;
}

string MMSPropertyData::getParameter() {
    return this->parameter;
}

void MMSPropertyData::setParameter(string parameter) {
    this->parameter = parameter;
}

string MMSPropertyData::getValue() {
    return this->value;
}
void MMSPropertyData::setValue(string value) {
    this->value = value;
}

string MMSPropertyData::getType() {
    return this->type;
}

void MMSPropertyData::setType(string type) {
    this->type = type;
}

int MMSPropertyData::getMax() {
    return this->max;
}

void MMSPropertyData::setMax(int max) {
    this->max = max;
}

int MMSPropertyData::getMin() {
    return this->min;
}

void MMSPropertyData::setMin(int min) {
    this->min = min;
}

vector <string> MMSPropertyData::getVallist() {
    return this->vallist;
}

void MMSPropertyData::setVallist(vector <string> vallist) {
    this->vallist = vallist;
}

char MMSPropertyData::getSeparator() {
    return this->separator;
}

void MMSPropertyData::setSeparator(char separator){
    this->separator = separator;
}

bool MMSPropertyData::isSetInDb() {
    return this->issetindb;
}
void MMSPropertyData::setisSetinDb(bool issetindb) {
    this->issetindb = issetindb;
}
