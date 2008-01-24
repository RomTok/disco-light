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

#include "mmsgui/theme/mmspopupwindowclass.h"

MMSPopupWindowClass::MMSPopupWindowClass() {
    unsetAll();
}

void MMSPopupWindowClass::unsetAll() {
    this->className  = "";
    unsetDuration();
}

void MMSPopupWindowClass::setAttributesFromXMLNode(xmlNode* node, string path) {
    startXMLScan
    {
        if(attrName == "class")
            setClassName(attrValue);
        else if(attrName == "duration")
            setDuration(atoi(attrValue.c_str()));
    }
    endXMLScan 
}

void MMSPopupWindowClass::setClassName(string className) {
    this->className = className;
}

string MMSPopupWindowClass::getClassName() {
    return this->className;
}

bool MMSPopupWindowClass::isDuration() {
    return this->isduration;
}

void MMSPopupWindowClass::setDuration(unsigned int duration) {
    this->duration = duration;
    this->isduration = true;
}

void MMSPopupWindowClass::unsetDuration() {
    this->isduration = false;
}

unsigned int MMSPopupWindowClass::getDuration() {
    return this->duration;
}

