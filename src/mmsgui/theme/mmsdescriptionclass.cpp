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

#include "mmsgui/theme/mmsdescriptionclass.h"

MMSDescriptionClass::MMSDescriptionClass() {
    unsetAll();
}

void MMSDescriptionClass::unsetAll() {
    unsetAuthor();
    unsetEmail();
    unsetDesc();
}

void MMSDescriptionClass::setAttributesFromXMLNode(xmlNode* node) {
    startXMLScan
    {
        if(attrName == "author")
            setAuthor(attrValue);
        else if(attrName == "email")
            setEmail(attrValue);
        else if(attrName == "desc")
            setDesc(attrValue);
    }
    endXMLScan 
}

bool MMSDescriptionClass::isAuthor() {
    return this->isauthor;
}

void MMSDescriptionClass::setAuthor(string author) {
    this->author = author;
    this->isauthor = true;
}

void MMSDescriptionClass::unsetAuthor() {
    this->isauthor = false;
}

string MMSDescriptionClass::getAuthor() {
    return this->author;
}

bool MMSDescriptionClass::isEmail() {
    return this->isemail;
}

void MMSDescriptionClass::setEmail(string email) {
    this->email = email;
    this->isemail = true;
}

void MMSDescriptionClass::unsetEmail() {
    this->isemail = false;
}

string MMSDescriptionClass::getEmail() {
    return this->email;
}

bool MMSDescriptionClass::isDesc() {
    return this->isdesc;
}

void MMSDescriptionClass::setDesc(string desc) {
    this->desc = desc;
    this->isdesc = true;
}

void MMSDescriptionClass::unsetDesc() {
    this->isdesc = false;
}

string MMSDescriptionClass::getDesc() {
    return this->desc;
}

