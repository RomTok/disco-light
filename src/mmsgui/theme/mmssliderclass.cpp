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

#include "mmsgui/theme/mmssliderclass.h"

MMSSliderClass::MMSSliderClass() {
    unsetAll();
}

void MMSSliderClass::unsetAll() {
    this->className = "";
    unsetImagePath();
    unsetImageName();
    unsetSelImagePath();
    unsetSelImageName();
    unsetImagePath_p();
    unsetImageName_p();
    unsetSelImagePath_p();
    unsetSelImageName_p();
    unsetImagePath_i();
    unsetImageName_i();
    unsetSelImagePath_i();
    unsetSelImageName_i();
    unsetPosition();
}

void MMSSliderClass::setAttributesFromXMLNode(xmlNode* node, string prefix, string path) {
    bool class_set = false;

    startXMLScan
    {
        if(attrName == "class") {
            setClassName(attrValue);
            class_set = true;
        }
        else if(attrName == prefix + "image") {
            if (attrValue != "")
                setImagePath("");
            else
                setImagePath(path);
            setImageName(attrValue);
        }
        else if(attrName == prefix + "image.path") {
            if (attrValue != "")
                setImagePath(attrValue);
            else
                setImagePath(path);
        }
        else if(attrName == prefix + "image.name")
            setImageName(attrValue);
        else if(attrName == prefix + "selimage") {
            if (attrValue != "")
                setSelImagePath("");
            else
                setSelImagePath(path);
            setSelImageName(attrValue);
        }
        else if(attrName == prefix + "selimage.path") {
            if (attrValue != "")
                setSelImagePath(attrValue);
            else
                setSelImagePath(path);
        }
        else if(attrName == prefix + "selimage.name")
            setSelImageName(attrValue);

        else if(attrName == prefix + "image_p") {
            if (attrValue != "")
                setImagePath_p("");
            else
                setImagePath_p(path);
            setImageName_p(attrValue);
        }
        else if(attrName == prefix + "image_p.path") {
            if (attrValue != "")
                setImagePath_p(attrValue);
            else
                setImagePath_p(path);
        }
        else if(attrName == prefix + "image_p.name")
            setImageName_p(attrValue);
        else if(attrName == prefix + "selimage_p") {
            if (attrValue != "")
                setSelImagePath_p("");
            else
                setSelImagePath_p(path);
            setSelImageName_p(attrValue);
        }
        else if(attrName == prefix + "selimage_p.path") {
            if (attrValue != "")
                setSelImagePath_p(attrValue);
            else
                setSelImagePath_p(path);
        }
        else if(attrName == prefix + "selimage_p.name")
            setSelImageName_p(attrValue);
        
        else if(attrName == prefix + "image_i") {
            if (attrValue != "")
                setImagePath_i("");
            else
                setImagePath_i(path);
            setImageName_i(attrValue);
        }
        else if(attrName == prefix + "image_i.path") {
            if (attrValue != "")
                setImagePath_i(attrValue);
            else
                setImagePath_i(path);
        }
        else if(attrName == prefix + "image_i.name")
            setImageName_i(attrValue);
        else if(attrName == prefix + "selimage_i") {
            if (attrValue != "")
                setSelImagePath_i("");
            else
                setSelImagePath_i(path);
            setSelImageName_i(attrValue);
        }
        else if(attrName == prefix + "selimage_i.path") {
            if (attrValue != "")
                setSelImagePath_i(attrValue);
            else
                setSelImagePath_i(path);
        }
        else if(attrName == prefix + "selimage_i.name")
            setSelImageName_i(attrValue);
        else if(attrName == "position")
            setPosition(atoi(attrValue.c_str()));
    }
    endXMLScan 

    if ((!isImagePath())&&(!class_set)&&(path!=""))
        setImagePath(path);
    if ((!isSelImagePath())&&(!class_set)&&(path!=""))
        setSelImagePath(path);
    if ((!isImagePath_p())&&(!class_set)&&(path!=""))
        setImagePath_p(path);
    if ((!isSelImagePath_p())&&(!class_set)&&(path!=""))
        setSelImagePath_p(path);
    if ((!isImagePath_i())&&(!class_set)&&(path!=""))
        setImagePath_i(path);
    if ((!isSelImagePath_i())&&(!class_set)&&(path!=""))
        setSelImagePath_i(path);
}

void MMSSliderClass::setClassName(string className) {
    this->className = className;
}

string MMSSliderClass::getClassName() {
    return this->className;
}

bool MMSSliderClass::isImagePath() {
    return this->isimagepath;
}

void MMSSliderClass::setImagePath(string imagepath) {
    this->imagepath = imagepath;
    this->isimagepath = true;
}

void MMSSliderClass::unsetImagePath() {
    this->isimagepath = false;
}

string MMSSliderClass::getImagePath() {
    return this->imagepath;
}

bool MMSSliderClass::isImageName() {
    return this->isimagename;
}

void MMSSliderClass::setImageName(string imagename) {
    this->imagename = imagename;
    this->isimagename = true;
}

void MMSSliderClass::unsetImageName() {
    this->isimagename = false;
}

string MMSSliderClass::getImageName() {
    return this->imagename;
}

bool MMSSliderClass::isSelImagePath() {
    return this->isselimagepath;
}

void MMSSliderClass::setSelImagePath(string selimagepath) {
    this->selimagepath = selimagepath;
    this->isselimagepath = true;
}

void MMSSliderClass::unsetSelImagePath() {
    this->isselimagepath = false;
}

string MMSSliderClass::getSelImagePath() {
    return this->selimagepath;
}

bool MMSSliderClass::isSelImageName() {
    return this->isselimagename;
}

void MMSSliderClass::setSelImageName(string selimagename) {
    this->selimagename = selimagename;
    this->isselimagename = true;
}

void MMSSliderClass::unsetSelImageName() {
    this->isselimagename = false;
}

string MMSSliderClass::getSelImageName() {
    return this->selimagename;
}

bool MMSSliderClass::isImagePath_p() {
    return this->isimagepath_p;
}

void MMSSliderClass::setImagePath_p(string imagepath_p) {
    this->imagepath_p = imagepath_p;
    this->isimagepath_p = true;
}

void MMSSliderClass::unsetImagePath_p() {
    this->isimagepath_p = false;
}

string MMSSliderClass::getImagePath_p() {
    return this->imagepath_p;
}

bool MMSSliderClass::isImageName_p() {
    return this->isimagename_p;
}

void MMSSliderClass::setImageName_p(string imagename_p) {
    this->imagename_p = imagename_p;
    this->isimagename_p = true;
}

void MMSSliderClass::unsetImageName_p() {
    this->isimagename_p = false;
}

string MMSSliderClass::getImageName_p() {
    return this->imagename_p;
}

bool MMSSliderClass::isSelImagePath_p() {
    return this->isselimagepath_p;
}

void MMSSliderClass::setSelImagePath_p(string selimagepath_p) {
    this->selimagepath_p = selimagepath_p;
    this->isselimagepath_p = true;
}

void MMSSliderClass::unsetSelImagePath_p() {
    this->isselimagepath_p = false;
}

string MMSSliderClass::getSelImagePath_p() {
    return this->selimagepath_p;
}

bool MMSSliderClass::isSelImageName_p() {
    return this->isselimagename_p;
}

void MMSSliderClass::setSelImageName_p(string selimagename_p) {
    this->selimagename_p = selimagename_p;
    this->isselimagename_p = true;
}

void MMSSliderClass::unsetSelImageName_p() {
    this->isselimagename_p = false;
}

string MMSSliderClass::getSelImageName_p() {
    return this->selimagename_p;
}

bool MMSSliderClass::isImagePath_i() {
    return this->isimagepath_i;
}

void MMSSliderClass::setImagePath_i(string imagepath_i) {
    this->imagepath_i = imagepath_i;
    this->isimagepath_i = true;
}

void MMSSliderClass::unsetImagePath_i() {
    this->isimagepath_i = false;
}

string MMSSliderClass::getImagePath_i() {
    return this->imagepath_i;
}

bool MMSSliderClass::isImageName_i() {
    return this->isimagename_i;
}

void MMSSliderClass::setImageName_i(string imagename_i) {
    this->imagename_i = imagename_i;
    this->isimagename_i = true;
}

void MMSSliderClass::unsetImageName_i() {
    this->isimagename_i = false;
}

string MMSSliderClass::getImageName_i() {
    return this->imagename_i;
}

bool MMSSliderClass::isSelImagePath_i() {
    return this->isselimagepath_i;
}

void MMSSliderClass::setSelImagePath_i(string selimagepath_i) {
    this->selimagepath_i = selimagepath_i;
    this->isselimagepath_i = true;
}

void MMSSliderClass::unsetSelImagePath_i() {
    this->isselimagepath_i = false;
}

string MMSSliderClass::getSelImagePath_i() {
    return this->selimagepath_i;
}

bool MMSSliderClass::isSelImageName_i() {
    return this->isselimagename_i;
}

void MMSSliderClass::setSelImageName_i(string selimagename_i) {
    this->selimagename_i = selimagename_i;
    this->isselimagename_i = true;
}

void MMSSliderClass::unsetSelImageName_i() {
    this->isselimagename_i = false;
}

string MMSSliderClass::getSelImageName_i() {
    return this->selimagename_i;
}

bool MMSSliderClass::isPosition() {
    return this->isposition;
}

void MMSSliderClass::setPosition(unsigned int position) {
    this->position = position;
    this->isposition = true;
}

void MMSSliderClass::unsetPosition() {
    this->isposition = false;
}

unsigned int MMSSliderClass::getPosition() {
    return this->position;
}

