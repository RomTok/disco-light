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

#include "mmsgui/theme/mmslabelclass.h"

//store attribute descriptions here 
TAFF_ATTRDESC MMSGUI_LABEL_ATTR_I[] = MMSGUI_LABEL_ATTR_INIT;

//address attribute names
#define GETATTRNAME(aname) MMSGUI_LABEL_ATTR_I[MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_##aname].name
#define ISATTRNAME(aname) (strcmp(attrname, GETATTRNAME(aname))==0)


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

void MMSLabelClass::setAttributesFromTAFF(MMSTaffFile *tafff, string *prefix, string *path) {
    DFBColor color;
    bool class_set = false;

    if (!prefix) {
		startTAFFScan
		{
	        switch (attrid) {
			case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
	            setClassName(attrval_str);
	            class_set = true;
				break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_font_path:
	            if (*attrval_str)
	                setFontPath(attrval_str);
	            else
	                setFontPath((path)?*path:"");
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_font_name:
	            setFontName(attrval_str);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_font_size:
	            setFontSize(attrval_int);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_alignment:
	            setAlignment(getAlignmentFromString(attrval_str));
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_color:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            if (getColorFromString(attrval_str, &color))    
	                setColor(color);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_color_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.a = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_color_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.r = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_color_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.g = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_color_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.b = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_selcolor:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            if (getColorFromString(attrval_str, &color))    
	                setSelColor(color);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_selcolor_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.a = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_selcolor_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.r = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_selcolor_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.g = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_selcolor_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.b = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_LABEL_ATTR::MMSGUI_LABEL_ATTR_IDS_text:
	            setText(attrval_str);
	            break;
			}
		}
		endTAFFScan
    }
    else {
    	unsigned int pl = strlen(prefix->c_str());
    	
    	startTAFFScan_WITHOUT_ID
    	{
    		/* check if attrname has correct prefix */
    		if (pl >= strlen(attrname))
        		continue;
            if (memcmp(attrname, prefix->c_str(), pl)!=0)
            	continue;
            attrname = &attrname[pl];

    		/* okay, correct prefix, check attributes now */
            if (ISATTRNAME(font_path)) { 
	            if (*attrval_str)
	                setFontPath(attrval_str);
	            else
	                setFontPath((path)?*path:"");
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

    if ((!class_set)&&(path)&&(*path!="")) {
	    if (!isFontPath())
	        setFontPath(*path);
    }
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

