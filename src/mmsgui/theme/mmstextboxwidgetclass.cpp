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

#include "mmsgui/theme/mmstextboxwidgetclass.h"
#include <string.h>

//store attribute descriptions here
TAFF_ATTRDESC MMSGUI_TEXTBOXWIDGET_ATTR_I[] = MMSGUI_TEXTBOXWIDGET_ATTR_INIT;

// address attribute names
#define GETATTRNAME(aname) MMSGUI_TEXTBOXWIDGET_ATTR_I[MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_##aname].name

// address attribute types
#define GETATTRTYPE(aname) MMSGUI_TEXTBOXWIDGET_ATTR_I[MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_##aname].type

// address font attribute names
#define GETFONTATTRNAME(aname) MMSGUI_TEXTBOXWIDGET_ATTR_I[MMSGUI_FONT_ATTR::MMSGUI_FONT_ATTR_IDS_##aname].name

// address font attribute types
#define GETFONTATTRTYPE(aname) MMSGUI_TEXTBOXWIDGET_ATTR_I[MMSGUI_FONT_ATTR::MMSGUI_FONT_ATTR_IDS_##aname].type

// address shadow attribute names
#define GETSHADOWATTRNAME(aname) MMSGUI_TEXTBOXWIDGET_ATTR_I[MMSGUI_SHADOW_ATTR::MMSGUI_SHADOW_ATTR_IDS_##aname].name

// address shadow attribute types
#define GETSHADOWATTRTYPE(aname) MMSGUI_TEXTBOXWIDGET_ATTR_I[MMSGUI_SHADOW_ATTR::MMSGUI_SHADOW_ATTR_IDS_##aname].type


MMSTextBoxWidgetClass::MMSTextBoxWidgetClass() {
    unsetAll();
}

void MMSTextBoxWidgetClass::unsetAll() {
    this->className = "";
    unsetWrap();
    unsetSplitWords();
    unsetTranslate();
    unsetFilePath();
    unsetFileName();
    MMSTextBaseClass::unsetAll();
}

void MMSTextBoxWidgetClass::setAttributesFromTAFF(MMSTaffFile *tafff, string *prefix, string *path, bool reset_paths) {
    MMSFBColor color;

    if ((reset_paths)&&(path)&&(*path!="")) {
    	// unset my paths
        unsetFontPath();
        unsetFilePath();
    }

    if (!prefix) {
		startTAFFScan
		{
	        switch (attrid) {
			case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
	            setClassName(attrval_str);
				break;

			// special macro for font parameters
			SET_FONT_FROM_TAFF(MMSGUI_TEXTBOXWIDGET_ATTR)

			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_alignment:
	            setAlignment(getAlignmentFromString(attrval_str));
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_wrap:
	            setWrap((attrval_int) ? true : false);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_splitwords:
	            setSplitWords((attrval_int) ? true : false);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_color:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            if (getMMSFBColorFromString(attrval_str, &color))
	                setColor(color);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_color_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.a = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_color_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.r = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_color_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.g = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_color_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.b = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_selcolor:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            if (getMMSFBColorFromString(attrval_str, &color))
	                setSelColor(color);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_selcolor_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.a = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_selcolor_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.r = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_selcolor_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.g = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_selcolor_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.b = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_text:
	            setText(attrval_str);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_translate:
	            setTranslate((attrval_int)?true:false);
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_file_path:
	            if (*attrval_str)
	                setFilePath(attrval_str);
	            else
	                setFilePath((path)?*path:"");
	            break;
			case MMSGUI_TEXTBOXWIDGET_ATTR::MMSGUI_TEXTBOXWIDGET_ATTR_IDS_file_name:
	            setFileName(attrval_str);
	            break;

			// special macro for shadow parameters
			SET_SHADOW_FROM_TAFF(MMSGUI_TEXTBOXWIDGET_ATTR)
			}
		}
		endTAFFScan
    }
    else {
    	unsigned int pl = strlen(prefix->c_str());

    	startTAFFScan_WITHOUT_ID
    	{
    		// check if attrname has correct prefix
    		if (pl >= strlen(attrname))
        		continue;
            if (memcmp(attrname, prefix->c_str(), pl)!=0)
            	continue;
            attrname = &attrname[pl];

            // special storage for macros
			bool attrval_str_valid;
			bool int_val_set;
			bool byte_val_set;
			int  *p_int_val = &attrval_int;

    		// okay, correct prefix, check attributes now

            // special macro for font parameters
            SET_FONT_FROM_TAFF_WITH_PREFIX
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
	            if (getMMSFBColorFromString(attrval_str, &color))
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
	            if (getMMSFBColorFromString(attrval_str, &color))
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
            else
            if (ISATTRNAME(translate)) {
	            setTranslate((attrval_int)?true:false);
			}
            else
            if (ISATTRNAME(file_path)) {
	            if (*attrval_str)
	                setFilePath(attrval_str);
	            else
	                setFilePath((path)?*path:"");
            }
            else
            if (ISATTRNAME(file_name)) {
	            setFileName(attrval_str);
            }
            else
            // special macro for shadow parameters
            SET_SHADOW_FROM_TAFF_WITH_PREFIX
    	}
    	endTAFFScan_WITHOUT_ID
    }

    if ((reset_paths)&&(path)&&(*path!="")) {
    	// set my paths
	    if (!isFontPath())
	        setFontPath(*path);
	    if (!isFilePath())
	        setFilePath(*path);
    }
}

void MMSTextBoxWidgetClass::setClassName(string className) {
    this->className = className;
}

string MMSTextBoxWidgetClass::getClassName() {
    return this->className;
}


bool MMSTextBoxWidgetClass::isWrap() {
    return this->iswrap;
}

void MMSTextBoxWidgetClass::setWrap(bool wrap) {
    this->wrap = wrap;
    this->iswrap = true;
}

void MMSTextBoxWidgetClass::unsetWrap() {
    this->iswrap = false;
}

bool MMSTextBoxWidgetClass::getWrap() {
    return this->wrap;
}

bool MMSTextBoxWidgetClass::isSplitWords() {
    return this->issplitwords;
}

void MMSTextBoxWidgetClass::setSplitWords(bool splitwords) {
    this->splitwords = splitwords;
    this->issplitwords = true;
}

void MMSTextBoxWidgetClass::unsetSplitWords() {
    this->issplitwords = false;
}

bool MMSTextBoxWidgetClass::getSplitWords() {
    return this->splitwords;
}


bool MMSTextBoxWidgetClass::isTranslate() {
    return this->istranslate;
}

void MMSTextBoxWidgetClass::setTranslate(bool translate) {
    this->translate = translate;
    this->istranslate = true;
}

void MMSTextBoxWidgetClass::unsetTranslate() {
    this->istranslate = false;
}

bool MMSTextBoxWidgetClass::getTranslate() {
    return this->translate;
}

bool MMSTextBoxWidgetClass::isFilePath() {
    return this->isfilepath;
}

void MMSTextBoxWidgetClass::setFilePath(string filepath) {
    this->filepath = filepath;
    this->isfilepath = true;
}

void MMSTextBoxWidgetClass::unsetFilePath() {
    this->isfilepath = false;
}

string MMSTextBoxWidgetClass::getFilePath() {
    return this->filepath;
}

bool MMSTextBoxWidgetClass::isFileName() {
    return this->isfilename;
}

void MMSTextBoxWidgetClass::setFileName(string filename) {
    this->filename = filename;
    this->isfilename = true;
}

void MMSTextBoxWidgetClass::unsetFileName() {
    this->isfilename = false;
}

string MMSTextBoxWidgetClass::getFileName() {
    return this->filename;
}


