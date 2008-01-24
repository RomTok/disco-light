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

#include "mmsgui/theme/mmstextboxclass.h"

MMSTextBoxClass::MMSTextBoxClass() {
    unsetAll();
}

void MMSTextBoxClass::unsetAll() {
    this->className = "";
    unsetFontPath();
    unsetFontName();
    unsetFontSize();
    unsetAlignment();
    unsetWrap();
    unsetSplitWords();
    unsetColor();
    unsetSelColor();
    unsetText();
}

void MMSTextBoxClass::setAttributesFromXMLNode(xmlNode* node, string prefix, string path) {
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
        else if(attrName == prefix + "wrap")
            setWrap((attrValue == "true") ? true : false);
        else if(attrName == prefix + "splitwords")
            setSplitWords((attrValue == "true") ? true : false);
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

void MMSTextBoxClass::setClassName(string className) {
    this->className = className;
}

string MMSTextBoxClass::getClassName() {
    return this->className;
}

bool MMSTextBoxClass::isFontPath() {
    return this->isfontpath;
}

void MMSTextBoxClass::setFontPath(string fontpath) {
    this->fontpath = fontpath;
    this->isfontpath = true;
}

void MMSTextBoxClass::unsetFontPath() {
    this->isfontpath = false;
}

string MMSTextBoxClass::getFontPath() {
    return this->fontpath;
}

bool MMSTextBoxClass::isFontName() {
    return this->isfontname;
}

void MMSTextBoxClass::setFontName(string fontname) {
    this->fontname = fontname;
    this->isfontname = true;
}

void MMSTextBoxClass::unsetFontName() {
    this->isfontname = false;
}

string MMSTextBoxClass::getFontName() {
    return this->fontname;
}

bool MMSTextBoxClass::isFontSize() {
    return this->isfontsize;
}

void MMSTextBoxClass::setFontSize(unsigned int fontsize) {
    this->fontsize = fontsize;
    this->isfontsize = true;
}

void MMSTextBoxClass::unsetFontSize() {
    this->isfontsize = false;
}

unsigned int MMSTextBoxClass::getFontSize() {
    return this->fontsize;
}

bool MMSTextBoxClass::isAlignment() {
    return this->isalignment;
}

void MMSTextBoxClass::setAlignment(MMSALIGNMENT alignment) {
    this->alignment = alignment;
    this->isalignment = true;
}

void MMSTextBoxClass::unsetAlignment() {
    this->isalignment = false;
}

MMSALIGNMENT MMSTextBoxClass::getAlignment() {
    return this->alignment;
}

bool MMSTextBoxClass::isWrap() {
    return this->iswrap;
}

void MMSTextBoxClass::setWrap(bool wrap) {
    this->wrap = wrap;
    this->iswrap = true;
}

void MMSTextBoxClass::unsetWrap() {
    this->iswrap = false;
}

bool MMSTextBoxClass::getWrap() {
    return this->wrap;
}

bool MMSTextBoxClass::isSplitWords() {
    return this->issplitwords;
}

void MMSTextBoxClass::setSplitWords(bool splitwords) {
    this->splitwords = splitwords;
    this->issplitwords = true;
}

void MMSTextBoxClass::unsetSplitWords() {
    this->issplitwords = false;
}

bool MMSTextBoxClass::getSplitWords() {
    return this->splitwords;
}

bool MMSTextBoxClass::isColor() {
    return this->iscolor;
}

void MMSTextBoxClass::setColor(DFBColor color) {
    this->color = color;
    this->iscolor = true;
}

void MMSTextBoxClass::unsetColor() {
    this->iscolor = false;
}

DFBColor MMSTextBoxClass::getColor() {
    return this->color;
}

bool MMSTextBoxClass::isSelColor() {
    return this->isselcolor;
}

void MMSTextBoxClass::setSelColor(DFBColor selcolor) {
    this->selcolor = selcolor;
    this->isselcolor = true;
}

void MMSTextBoxClass::unsetSelColor() {
    this->isselcolor = false;
}

DFBColor MMSTextBoxClass::getSelColor() {
    return this->selcolor;
}

bool MMSTextBoxClass::isText() {
    return this->istext;
}

void MMSTextBoxClass::setText(string text) {
    this->text = text;
    this->istext = true;
}

void MMSTextBoxClass::unsetText() {
    this->istext = false;
}

string MMSTextBoxClass::getText() {
    return this->text;
}

