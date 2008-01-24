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

#include "mmsgui/theme/mmsborderclass.h"

MMSBorderClass::MMSBorderClass() {
    initColor();
    initSelColor();

    initImagePath();
    initImageNames();
    initSelImagePath();
    initSelImageNames();

    initThickness();
    initMargin();
    initRCorners();
}

MMSBorderClass::~MMSBorderClass() {
    freeColor();
    freeSelColor();

    freeImagePath();
    freeImageNames();
    freeSelImagePath();
    freeSelImageNames();

    freeThickness();
    freeMargin();
    freeRCorners();
}

MMSBorderClass &MMSBorderClass::operator=(const MMSBorderClass &c) { 
	if (this != &c) {
		/* copy internal fix data area */
		this->id = c.id;

		/* copy external data */
		memset(&(this->ed), 0, sizeof(this->ed));
		if (c.id.isimagepath)
			this->ed.imagepath = new string(*c.ed.imagepath);
		if (c.id.isselimagepath)
			this->ed.selimagepath = new string(*c.ed.selimagepath);
		if (c.id.isimagenames)
			for (int i = 0; i < 8; i++)
				this->ed.imagenames[i] = new string(*c.ed.imagenames[i]);
		if (c.id.isselimagenames)
			for (int i = 0; i < 8; i++)
				this->ed.selimagenames[i] = new string(*c.ed.selimagenames[i]);
	}
	return *this;
}

void MMSBorderClass::unsetAll() {
    unsetColor();
    unsetSelColor();

    unsetImagePath();
    unsetImageNames();
    unsetSelImagePath();
    unsetSelImageNames();

    unsetThickness();
    unsetMargin();
    unsetRCorners();
}

void MMSBorderClass::setAttributesFromXMLNode(xmlNode* node, string prefix, string path) {
    bool class_set = false;

    startXMLScan
    {
        DFBColor color;
        color.a = 0;
        color.r = 0;
        color.g = 0;
        color.b = 0;

        if(attrName == "class") {
            class_set = true;
        }
        else if(attrName == prefix + "border.color") {
            if (isColor()) getColor(color);

            if (getColorFromString(attrValue, &color))    
                setColor(color);
        }
        else if(attrName == prefix + "border.color.a") {
            if (isColor()) getColor(color);
    
            color.a = atoi(attrValue.c_str());

            setColor(color);
        }
        else if(attrName == prefix + "border.color.r") {
            if (isColor()) getColor(color);
    
            color.r = atoi(attrValue.c_str());

            setColor(color);
        }
        else if(attrName == prefix + "border.color.g") {
            if (isColor()) getColor(color);
    
            color.g = atoi(attrValue.c_str());

            setColor(color);
        }
        else if(attrName == prefix + "border.color.b") {
            if (isColor()) getColor(color);
    
            color.b = atoi(attrValue.c_str());

            setColor(color);
        }
        else if(attrName == prefix + "border.selcolor") {
            if (isSelColor()) getSelColor(color);

            if (getColorFromString(attrValue, &color))    
                setSelColor(color);
        }
        else if(attrName == prefix + "border.selcolor.a") {
            if (isSelColor()) getSelColor(color);
    
            color.a = atoi(attrValue.c_str());

            setSelColor(color);
        }
        else if(attrName == prefix + "border.selcolor.r") {
            if (isSelColor()) getSelColor(color);
    
            color.r = atoi(attrValue.c_str());

            setSelColor(color);
        }
        else if(attrName == prefix + "border.selcolor.g") {
            if (isSelColor()) getSelColor(color);
    
            color.g = atoi(attrValue.c_str());

            setSelColor(color);
        }
        else if(attrName == prefix + "border.selcolor.b") {
            if (isSelColor()) getSelColor(color);
    
            color.b = atoi(attrValue.c_str());

            setSelColor(color);
        }
        else if(attrName == prefix + "border.image.path") {
            if (attrValue != "")
                setImagePath(attrValue);
            else
                setImagePath(path);
        }
        else if(attrName == prefix + "border.image.top-left")
            setImageNames(0, attrValue);
        else if(attrName == prefix + "border.image.top")
            setImageNames(1, attrValue);
        else if(attrName == prefix + "border.image.top-right")
            setImageNames(2, attrValue);
        else if(attrName == prefix + "border.image.right")
            setImageNames(3, attrValue);
        else if(attrName == prefix + "border.image.bottom-right")
            setImageNames(4, attrValue);
        else if(attrName == prefix + "border.image.bottom")
            setImageNames(5, attrValue);
        else if(attrName == prefix + "border.image.bottom-left")
            setImageNames(6, attrValue);
        else if(attrName == prefix + "border.image.left")
            setImageNames(7, attrValue);
        else if(attrName == prefix + "border.selimage.path") {
            if (attrValue != "")
                setSelImagePath(attrValue);
            else
                setSelImagePath(path);
        }
        else if(attrName == prefix + "border.selimage.top-left")
            setSelImageNames(0, attrValue);
        else if(attrName == prefix + "border.selimage.top")
            setSelImageNames(1, attrValue);
        else if(attrName == prefix + "border.selimage.top-right")
            setSelImageNames(2, attrValue);
        else if(attrName == prefix + "border.selimage.right")
            setSelImageNames(3, attrValue);
        else if(attrName == prefix + "border.selimage.bottom-right")
            setSelImageNames(4, attrValue);
        else if(attrName == prefix + "border.selimage.bottom")
            setSelImageNames(5, attrValue);
        else if(attrName == prefix + "border.selimage.bottom-left")
            setSelImageNames(6, attrValue);
        else if(attrName == prefix + "border.selimage.left")
            setSelImageNames(7, attrValue);
        else if(attrName == prefix + "border.thickness")
            setThickness(atoi(attrValue.c_str()));
        else if(attrName == prefix + "border.margin")
            setMargin(atoi(attrValue.c_str()));
        else if(attrName == prefix + "border.rcorners")
            setRCorners((attrValue == "true") ? true : false);
    }
    endXMLScan 

    if ((!isImagePath())&&(!class_set)&&(path!=""))
        setImagePath(path);
    if ((!isSelImagePath())&&(!class_set)&&(path!=""))
        setSelImagePath(path);
}


void MMSBorderClass::initColor() {
	MMSTHEMECLASS_INIT_BASIC(color);
}

void MMSBorderClass::freeColor() {
	MMSTHEMECLASS_FREE_BASIC(color);
}

bool MMSBorderClass::isColor() {
	MMSTHEMECLASS_ISSET(color);
}

void MMSBorderClass::unsetColor() {
	MMSTHEMECLASS_UNSET(color);
}

void MMSBorderClass::setColor(const DFBColor &color) {
	MMSTHEMECLASS_SET_BASIC(color);
}

bool MMSBorderClass::getColor(DFBColor &color) {
	MMSTHEMECLASS_GET_BASIC(color);
}


void MMSBorderClass::initSelColor() {
	MMSTHEMECLASS_INIT_BASIC(selcolor);
}

void MMSBorderClass::freeSelColor() {
	MMSTHEMECLASS_FREE_BASIC(selcolor);
}

bool MMSBorderClass::isSelColor() {
	MMSTHEMECLASS_ISSET(selcolor);
}

void MMSBorderClass::unsetSelColor() {
	MMSTHEMECLASS_UNSET(selcolor);
}

void MMSBorderClass::setSelColor(const DFBColor &selcolor) {
	MMSTHEMECLASS_SET_BASIC(selcolor);
}

bool MMSBorderClass::getSelColor(DFBColor &selcolor) {
	MMSTHEMECLASS_GET_BASIC(selcolor);
}



void MMSBorderClass::initImagePath() {
	MMSTHEMECLASS_INIT_STRING(imagepath);
}

void MMSBorderClass::freeImagePath() {
	MMSTHEMECLASS_FREE_STRING(imagepath);
}

bool MMSBorderClass::isImagePath() {
	MMSTHEMECLASS_ISSET(imagepath);
}

void MMSBorderClass::unsetImagePath() {
	MMSTHEMECLASS_UNSET(imagepath);
}

void MMSBorderClass::setImagePath(const string &imagepath) {
	MMSTHEMECLASS_SET_STRING(imagepath);
}

bool MMSBorderClass::getImagePath(string &imagepath) {
	MMSTHEMECLASS_GET_STRING(imagepath);
}




void MMSBorderClass::initImageNames() {
	MMSTHEMECLASS_INIT_STRINGS(imagenames,8);
}

void MMSBorderClass::freeImageNames() {
	MMSTHEMECLASS_FREE_STRINGS(imagenames,8);
}

bool MMSBorderClass::isImageNames() {
	MMSTHEMECLASS_ISSET(imagenames);
}

void MMSBorderClass::unsetImageNames() {
	MMSTHEMECLASS_UNSET_STRINGS(imagenames,8);
}

void MMSBorderClass::setImageNames(const string &imagename_1, const string &imagename_2, const string &imagename_3, const string &imagename_4,
								   const string &imagename_5, const string &imagename_6, const string &imagename_7, const string &imagename_8) {
	MMSTHEMECLASS_SET_STRINGS(imagenames,0,imagename_1);
	MMSTHEMECLASS_SET_STRINGS(imagenames,1,imagename_2);
	MMSTHEMECLASS_SET_STRINGS(imagenames,2,imagename_3);
	MMSTHEMECLASS_SET_STRINGS(imagenames,3,imagename_4);
	MMSTHEMECLASS_SET_STRINGS(imagenames,4,imagename_5);
	MMSTHEMECLASS_SET_STRINGS(imagenames,5,imagename_6);
	MMSTHEMECLASS_SET_STRINGS(imagenames,6,imagename_7);
	MMSTHEMECLASS_SET_STRINGS(imagenames,7,imagename_8);
}

void MMSBorderClass::setImageNames(unsigned int num, const string &imagename) {
	MMSTHEMECLASS_SET_STRINGS(imagenames,num,imagename);
}

bool MMSBorderClass::getImageNames(unsigned int num, string &imagename) {
	MMSTHEMECLASS_GET_STRINGS(imagenames,num,imagename);
}


void MMSBorderClass::initSelImagePath() {
	MMSTHEMECLASS_INIT_STRING(selimagepath);
}

void MMSBorderClass::freeSelImagePath() {
	MMSTHEMECLASS_FREE_STRING(selimagepath);
}

bool MMSBorderClass::isSelImagePath() {
	MMSTHEMECLASS_ISSET(selimagepath);
}

void MMSBorderClass::unsetSelImagePath() {
	MMSTHEMECLASS_UNSET(selimagepath);
}

void MMSBorderClass::setSelImagePath(const string &selimagepath) {
	MMSTHEMECLASS_SET_STRING(selimagepath);
}

bool MMSBorderClass::getSelImagePath(string &selimagepath) {
	MMSTHEMECLASS_GET_STRING(selimagepath);
}



void MMSBorderClass::initSelImageNames() {
	MMSTHEMECLASS_INIT_STRINGS(selimagenames,8);
}

void MMSBorderClass::freeSelImageNames() {
	MMSTHEMECLASS_FREE_STRINGS(selimagenames,8);
}

bool MMSBorderClass::isSelImageNames() {
	MMSTHEMECLASS_ISSET(selimagenames);
}

void MMSBorderClass::unsetSelImageNames() {
	MMSTHEMECLASS_UNSET_STRINGS(selimagenames,8);
}

void MMSBorderClass::setSelImageNames(const string &selimagename_1, const string &selimagename_2, const string &selimagename_3, const string &selimagename_4,
								      const string &selimagename_5, const string &selimagename_6, const string &selimagename_7, const string &selimagename_8) {
	MMSTHEMECLASS_SET_STRINGS(selimagenames,0,selimagename_1);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,1,selimagename_2);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,2,selimagename_3);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,3,selimagename_4);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,4,selimagename_5);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,5,selimagename_6);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,6,selimagename_7);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,7,selimagename_8);
}

void MMSBorderClass::setSelImageNames(unsigned int num, const string &selimagename) {
	MMSTHEMECLASS_SET_STRINGS(selimagenames,num,selimagename);
}

bool MMSBorderClass::getSelImageNames(unsigned int num, string &selimagename) {
	MMSTHEMECLASS_GET_STRINGS(selimagenames,num,selimagename);
}


void MMSBorderClass::initThickness() {
	MMSTHEMECLASS_INIT_BASIC(thickness);
}

void MMSBorderClass::freeThickness() {
	MMSTHEMECLASS_FREE_BASIC(thickness);
}

bool MMSBorderClass::isThickness() {
	MMSTHEMECLASS_ISSET(thickness);
}

void MMSBorderClass::unsetThickness() {
	MMSTHEMECLASS_UNSET(thickness);
}

void MMSBorderClass::setThickness(unsigned int thickness) {
	MMSTHEMECLASS_SET_BASIC(thickness);
}

bool MMSBorderClass::getThickness(unsigned int &thickness) {
	MMSTHEMECLASS_GET_BASIC(thickness);
}

void MMSBorderClass::initMargin() {
	MMSTHEMECLASS_INIT_BASIC(margin);
}

void MMSBorderClass::freeMargin() {
	MMSTHEMECLASS_FREE_BASIC(margin);
}

bool MMSBorderClass::isMargin() {
	MMSTHEMECLASS_ISSET(margin);
}

void MMSBorderClass::unsetMargin() {
	MMSTHEMECLASS_UNSET(margin);
}

void MMSBorderClass::setMargin(unsigned int margin) {
	MMSTHEMECLASS_SET_BASIC(margin);
}

bool MMSBorderClass::getMargin(unsigned int &margin) {
	MMSTHEMECLASS_GET_BASIC(margin);
}

void MMSBorderClass::initRCorners() {
	MMSTHEMECLASS_INIT_BASIC(rcorners);
}

void MMSBorderClass::freeRCorners() {
	MMSTHEMECLASS_FREE_BASIC(rcorners);
}

bool MMSBorderClass::isRCorners() {
	MMSTHEMECLASS_ISSET(rcorners);
}

void MMSBorderClass::unsetRCorners() {
	MMSTHEMECLASS_UNSET(rcorners);
}

void MMSBorderClass::setRCorners(bool rcorners) {
	MMSTHEMECLASS_SET_BASIC(rcorners);
}

bool MMSBorderClass::getRCorners(bool &rcorners) {
	MMSTHEMECLASS_GET_BASIC(rcorners);
}

