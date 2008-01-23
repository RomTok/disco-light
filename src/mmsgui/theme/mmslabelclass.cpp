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

#include "theme/mmslabelclass.h"

MMSLabelClass::MMSLabelClass() {
    unsetAll();
}

void MMSLabelClass::unsetAll() {
    this->className = "";
    unsetFontPath();
    unsetFontName();
    unsetFontSize();
    unsetAlignment();
    unsetColor();
    unsetSelColor();
    unsetText();
}

void MMSLabelClass::setAttributesFromXMLNode(xmlNode* node, string prefix, string path) {
    bool class_set = false;

    startXMLScan
    {
        DFBColor color;
        color.a = 0;
        color.r = 0;
        color.g = 0;
        color.b = 0;

        if(attrName == "class") {
            setClassName(attrValue);
            class_set = true;
        }
        else if(attrName == prefix + "font.path") {
            if (attrValue != "")
                setFontPath(attrValue);
            else
                setFontPath(path);
        }
        else if(attrName == prefix + "font.name")
            setFontName(attrValue);
        else if(attrName == prefix + "font.size")
            setFontSize(atoi(attrValue.c_str()));
        else if(attrName == prefix + "alignment")
            setAlignment(getAlignmentFromString(attrValue));
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
        else if(attrName == prefix + "text")
            setText(attrValue);
    }
    endXMLScan 

    if ((!isFontPath())&&(!class_set)&&(path!=""))
        setFontPath(path);
}

void MMSLabelClass::setClassName(string className) {
    this->className = className;
}

string MMSLabelClass::getClassName() {
    return this->className;
}

bool MMSLabelClass::isFontPath() {
    return this->isfontpath;
}

void MMSLabelClass::setFontPath(string fontpath) {
    this->fontpath = fontpath;
    this->isfontpath = true;
}

void MMSLabelClass::unsetFontPath() {
    this->isfontpath = false;
}

string MMSLabelClass::getFontPath() {
    return this->fontpath;
}

bool MMSLabelClass::isFontName() {
    return this->isfontname;
}

void MMSLabelClass::setFontName(string fontname) {
    this->fontname = fontname;
    this->isfontname = true;
}

void MMSLabelClass::unsetFontName() {
    this->isfontname = false;
}

string MMSLabelClass::getFontName() {
    return this->fontname;
}

bool MMSLabelClass::isFontSize() {
    return this->isfontsize;
}

void MMSLabelClass::setFontSize(unsigned int fontsize) {
    this->fontsize = fontsize;
    this->isfontsize = true;
}

void MMSLabelClass::unsetFontSize() {
    this->isfontsize = false;
}

unsigned int MMSLabelClass::getFontSize() {
    return this->fontsize;
}

bool MMSLabelClass::isAlignment() {
    return this->isalignment;
}

void MMSLabelClass::setAlignment(MMSALIGNMENT alignment) {
    this->alignment = alignment;
    this->isalignment = true;
}

void MMSLabelClass::unsetAlignment() {
    this->isalignment = false;
}

MMSALIGNMENT MMSLabelClass::getAlignment() {
    return this->alignment;
}

bool MMSLabelClass::isColor() {
    return this->iscolor;
}

void MMSLabelClass::setColor(DFBColor color) {
    this->color = color;
    this->iscolor = true;
}

void MMSLabelClass::unsetColor() {
    this->iscolor = false;
}

DFBColor MMSLabelClass::getColor() {
    return this->color;
}

bool MMSLabelClass::isSelColor() {
    return this->isselcolor;
}

void MMSLabelClass::setSelColor(DFBColor selcolor) {
    this->selcolor = selcolor;
    this->isselcolor = true;
}

void MMSLabelClass::unsetSelColor() {
    this->isselcolor = false;
}

DFBColor MMSLabelClass::getSelColor() {
    return this->selcolor;
}

bool MMSLabelClass::isText() {
    return this->istext;
}

void MMSLabelClass::setText(string text) {
    this->text = text;
    this->istext = true;
}

void MMSLabelClass::unsetText() {
    this->istext = false;
}

string MMSLabelClass::getText() {
    return this->text;
}

