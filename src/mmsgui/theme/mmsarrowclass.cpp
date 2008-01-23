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

#include "theme/mmsarrowclass.h"

MMSArrowClass::MMSArrowClass() {
    unsetAll();
}

void MMSArrowClass::unsetAll() {
    this->className = "";
    unsetColor();
    unsetSelColor();
    unsetDirection();
}

void MMSArrowClass::setAttributesFromXMLNode(xmlNode* node, string prefix, string path) {
    startXMLScan
    {
        DFBColor color;
        color.a = 0;
        color.r = 0;
        color.g = 0;
        color.b = 0;

        if(attrName == "class")
            setClassName(attrValue);
        else if(attrName == prefix + "color") {
            if (isColor()) color = getColor();

            if (getColorFromString(attrValue, &color))    
                setColor(color);
        }
        else if(attrName == prefix + "color.a") {
            if (isColor()) color = getColor();
    
            color.a = atoi(attrValue.c_str());

            setColor(color);
        }
        else if(attrName == prefix + "color.r") {
            if (isColor()) color = getColor();
    
            color.r = atoi(attrValue.c_str());

            setColor(color);
        }
        else if(attrName == prefix + "color.g") {
            if (isColor()) color = getColor();
    
            color.g = atoi(attrValue.c_str());

            setColor(color);
        }
        else if(attrName == prefix + "color.b") {
            if (isColor()) color = getColor();
    
            color.b = atoi(attrValue.c_str());

            setColor(color);
        }
        else if(attrName == prefix + "selcolor") {
            if (isSelColor()) color = getSelColor();

            if (getColorFromString(attrValue, &color))    
                setSelColor(color);
        }
        else if(attrName == prefix + "selcolor.a") {
            if (isSelColor()) color = getSelColor();
    
            color.a = atoi(attrValue.c_str());

            setSelColor(color);
        }
        else if(attrName == prefix + "selcolor.r") {
            if (isSelColor()) color = getSelColor();
    
            color.r = atoi(attrValue.c_str());

            setSelColor(color);
        }
        else if(attrName == prefix + "selcolor.g") {
            if (isSelColor()) color = getSelColor();
    
            color.g = atoi(attrValue.c_str());

            setSelColor(color);
        }
        else if(attrName == prefix + "selcolor.b") {
            if (isSelColor()) color = getSelColor();
    
            color.b = atoi(attrValue.c_str());

            setSelColor(color);
        }
        else if(attrName == prefix + "direction") {
            setDirection(getDirectionFromString(attrValue));
        }
    }
    endXMLScan 
}

void MMSArrowClass::setClassName(string className) {
    this->className = className;
}

string MMSArrowClass::getClassName() {
    return this->className;
}

bool MMSArrowClass::isColor() {
    return this->iscolor;
}

void MMSArrowClass::setColor(DFBColor color) {
    this->color = color;
    this->iscolor = true;
}

void MMSArrowClass::unsetColor() {
    this->iscolor = false;
}

DFBColor MMSArrowClass::getColor() {
    return this->color;
}

bool MMSArrowClass::isSelColor() {
    return this->isselcolor;
}

void MMSArrowClass::setSelColor(DFBColor selcolor) {
    this->selcolor = selcolor;
    this->isselcolor = true;
}

void MMSArrowClass::unsetSelColor() {
    this->isselcolor = false;
}

DFBColor MMSArrowClass::getSelColor() {
    return this->selcolor;
}

bool MMSArrowClass::isDirection() {
    return this->isdirection;
}

void MMSArrowClass::setDirection(MMSDIRECTION direction) {
    this->direction = direction;
    this->isdirection = true;
}

void MMSArrowClass::unsetDirection() {
    this->isdirection = false;
}

MMSDIRECTION MMSArrowClass::getDirection() {
    return this->direction;
}

