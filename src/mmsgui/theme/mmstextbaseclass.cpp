/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2010 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mmsgui/theme/mmstextbaseclass.h"
#include <string.h>

MMSTextBaseClass::MMSTextBaseClass() {
    unsetAll();
}

void MMSTextBaseClass::unsetAll() {
    unsetFontPath();
    unsetFontSize();
    unsetFontNames();
    unsetAlignment();
    unsetColor();
    unsetSelColor();
    unsetText();
    shadows.unsetShadows();
}


bool MMSTextBaseClass::isFontPath() {
    return this->isfontpath;
}

void MMSTextBaseClass::setFontPath(string fontpath) {
    this->fontpath = fontpath;
    this->isfontpath = true;
}

void MMSTextBaseClass::unsetFontPath() {
    this->isfontpath = false;
}

string MMSTextBaseClass::getFontPath() {
    return this->fontpath;
}

bool MMSTextBaseClass::isFontSize() {
    return this->isfontsize;
}

void MMSTextBaseClass::setFontSize(unsigned int fontsize) {
    this->fontsize = fontsize;
    this->isfontsize = true;
}

void MMSTextBaseClass::unsetFontSize() {
    this->isfontsize = false;
}

unsigned int MMSTextBaseClass::getFontSize() {
    return this->fontsize;
}


bool MMSTextBaseClass::isFontName(MMSLanguage lang) {
	return this->fonts.isFontName(lang);
}

void MMSTextBaseClass::setFontName(string fontname, MMSLanguage lang) {
	this->fonts.setFontName(fontname, lang);
}

void MMSTextBaseClass::unsetFontName(MMSLanguage lang) {
	this->fonts.unsetFontName(lang);
}

void MMSTextBaseClass::unsetFontNames() {
	this->fonts.unsetFontNames();
}

string MMSTextBaseClass::getFontName(MMSLanguage lang) {
	return this->fonts.getFontName(lang);
}


bool MMSTextBaseClass::isAlignment() {
    return this->isalignment;
}

void MMSTextBaseClass::setAlignment(MMSALIGNMENT alignment) {
    this->alignment = alignment;
    this->isalignment = true;
}

void MMSTextBaseClass::unsetAlignment() {
    this->isalignment = false;
}

MMSALIGNMENT MMSTextBaseClass::getAlignment() {
    return this->alignment;
}

bool MMSTextBaseClass::isColor() {
    return this->iscolor;
}

void MMSTextBaseClass::setColor(MMSFBColor color) {
    this->color = color;
    this->iscolor = true;
}

void MMSTextBaseClass::unsetColor() {
    this->iscolor = false;
}

MMSFBColor MMSTextBaseClass::getColor() {
    return this->color;
}

bool MMSTextBaseClass::isSelColor() {
    return this->isselcolor;
}

void MMSTextBaseClass::setSelColor(MMSFBColor selcolor) {
    this->selcolor = selcolor;
    this->isselcolor = true;
}

void MMSTextBaseClass::unsetSelColor() {
    this->isselcolor = false;
}

MMSFBColor MMSTextBaseClass::getSelColor() {
    return this->selcolor;
}

bool MMSTextBaseClass::isText() {
    return this->istext;
}

void MMSTextBaseClass::setText(string *text) {
    this->text = *text;
    this->istext = true;
}

void MMSTextBaseClass::setText(string text) {
    setText(&text);
}

void MMSTextBaseClass::unsetText() {
    this->istext = false;
}

string MMSTextBaseClass::getText() {
    return this->text;
}



bool MMSTextBaseClass::isShadowColor(MMSPOSITION position) {
    return this->shadows.shadow[position].iscolor;
}

void MMSTextBaseClass::setShadowColor(MMSPOSITION position, MMSFBColor color) {
	this->shadows.shadow[position].color = color;
	this->shadows.shadow[position].iscolor = true;
}

void MMSTextBaseClass::unsetShadowColor(MMSPOSITION position) {
	this->shadows.shadow[position].iscolor = false;
}

MMSFBColor MMSTextBaseClass::getShadowColor(MMSPOSITION position) {
    return this->shadows.shadow[position].color;
}

bool MMSTextBaseClass::isSelShadowColor(MMSPOSITION position) {
    return this->shadows.shadow[position].isselcolor;
}

void MMSTextBaseClass::setSelShadowColor(MMSPOSITION position, MMSFBColor selcolor) {
	this->shadows.shadow[position].selcolor = selcolor;
	this->shadows.shadow[position].isselcolor = true;
}

void MMSTextBaseClass::unsetSelShadowColor(MMSPOSITION position) {
	this->shadows.shadow[position].isselcolor = false;
}

MMSFBColor MMSTextBaseClass::getSelShadowColor(MMSPOSITION position) {
    return this->shadows.shadow[position].selcolor;
}



