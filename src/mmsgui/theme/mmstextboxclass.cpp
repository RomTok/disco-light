/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
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

//store attribute descriptions here 
TAFF_ATTRDESC MMSGUI_TEXTBOX_ATTR_I[] = MMSGUI_TEXTBOX_ATTR_INIT;

//address attribute names
#define GETATTRNAME(aname) MMSGUI_TEXTBOX_ATTR_I[MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_##aname].name
#define ISATTRNAME(aname) (strcmp(attrname, GETATTRNAME(aname))==0)


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

void MMSTextBoxClass::setAttributesFromXMLNode(MMSTaffFile *tafff, string prefix, string path) {
    DFBColor color;
    bool class_set = false;

    if (prefix == "") {
		startTAFFScan
		{
	        switch (attrid) {
			case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
	            setClassName(attrval_str);
	            class_set = true;
				break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_font_path:
	            if (*attrval_str)
	                setFontPath(attrval_str);
	            else
	                setFontPath(path);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_font_name:
	            setFontName(attrval_str);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_font_size:
	            setFontSize(attrval_int);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_alignment:
	            setAlignment(getAlignmentFromString(attrval_str));
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_wrap:
	            setWrap((attrval_int) ? true : false);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_splitwords:
	            setSplitWords((attrval_int) ? true : false);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_color:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            if (getColorFromString(attrval_str, &color))    
	                setColor(color);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_color_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.a = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_color_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.r = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_color_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.g = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_color_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.b = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_selcolor:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            if (getColorFromString(attrval_str, &color))    
	                setSelColor(color);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_selcolor_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.a = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_selcolor_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.r = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_selcolor_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.g = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_selcolor_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.b = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_TEXTBOX_ATTR::MMSGUI_TEXTBOX_ATTR_IDS_text:
	            setText(attrval_str);
	            break;
			}
		}
		endTAFFScan
    }
    else {
    	unsigned int pl = strlen(prefix.c_str());
    	
    	startTAFFScan_WITHOUT_ID
    	{
    		/* check if attrname has correct prefix */
    		if (pl >= strlen(attrname))
        		continue;
            if (memcmp(attrname, prefix.c_str(), pl)!=0)
            	continue;
            attrname = &attrname[pl];

    		/* okay, correct prefix, check attributes now */
            if (ISATTRNAME(font_path)) { 
	            if (*attrval_str)
	                setFontPath(attrval_str);
	            else
	                setFontPath(path);
            }
            else
            if (ISATTRNAME(font_name)) { 
	            setFontName(attrval_str);
            }
            else
            if (ISATTRNAME(font_size)) { 
	            setFontSize(attrval_int);
            }
            else
            if (ISATTRNAME(alignment)) { 
	            setAlignment(getAlignmentFromString(attrval_str));
            }
            else
            if (ISATTRNAME(wrap)) { 
	            setWrap((attrval_int) ? true : false);
            }
            else
            if (ISATTRNAME(splitwords)) { 
	            setSplitWords((attrval_int) ? true : false);
            }
            else
            if (ISATTRNAME(color)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            if (getColorFromString(attrval_str, &color))    
	                setColor(color);
            }
            else
            if (ISATTRNAME(color_a)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.a = attrval_int;
	            setColor(color);
            }
            else
            if (ISATTRNAME(color_r)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.r = attrval_int;
	            setColor(color);
            }
            else
            if (ISATTRNAME(color_g)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.g = attrval_int;
	            setColor(color);
            }
            else
            if (ISATTRNAME(color_b)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.b = attrval_int;
	            setColor(color);
            }
            else
            if (ISATTRNAME(selcolor)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            if (getColorFromString(attrval_str, &color))    
	                setSelColor(color);
            }
            else
            if (ISATTRNAME(selcolor_a)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.a = attrval_int;
	            setSelColor(color);
            }
            else
            if (ISATTRNAME(selcolor_r)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.r = attrval_int;
	            setSelColor(color);
            }
            else
            if (ISATTRNAME(selcolor_g)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.g = attrval_int;
	            setSelColor(color);
            }
            else
            if (ISATTRNAME(selcolor_b)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.b = attrval_int;
	            setSelColor(color);
            }
            else
            if (ISATTRNAME(text)) { 
	            setText(attrval_str);
			}
    	}
    	endTAFFScan_WITHOUT_ID
    }

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

