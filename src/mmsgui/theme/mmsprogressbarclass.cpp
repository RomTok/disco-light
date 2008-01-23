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

#include "theme/mmsprogressbarclass.h"

MMSProgressBarClass::MMSProgressBarClass() {
    unsetAll();
}

void MMSProgressBarClass::unsetAll() {
    this->className = "";
    unsetColor();
    unsetSelColor();
    unsetProgress();
}

void MMSProgressBarClass::setAttributesFromXMLNode(xmlNode* node, string prefix, string path) {
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
        else if(attrName == prefix + "progress")
            setProgress(atoi(attrValue.c_str()));
    }
    endXMLScan 
}

void MMSProgressBarClass::setClassName(string className) {
    this->className = className;
}

string MMSProgressBarClass::getClassName() {
    return this->className;
}

bool MMSProgressBarClass::isColor() {
    return this->iscolor;
}

void MMSProgressBarClass::setColor(DFBColor color) {
    this->color = color;
    this->iscolor = true;
}

void MMSProgressBarClass::unsetColor() {
    this->iscolor = false;
}

DFBColor MMSProgressBarClass::getColor() {
    return this->color;
}

bool MMSProgressBarClass::isSelColor() {
    return this->isselcolor;
}

void MMSProgressBarClass::setSelColor(DFBColor selcolor) {
    this->selcolor = selcolor;
    this->isselcolor = true;
}

void MMSProgressBarClass::unsetSelColor() {
    this->isselcolor = false;
}

DFBColor MMSProgressBarClass::getSelColor() {
    return this->selcolor;
}

bool MMSProgressBarClass::isProgress() {
    return this->isprogress;
}

void MMSProgressBarClass::setProgress(unsigned int progress) {
    this->progress = progress;
    this->isprogress = true;
}

void MMSProgressBarClass::unsetProgress() {
    this->isprogress = false;
}

unsigned int MMSProgressBarClass::getProgress() {
    return this->progress;
}
