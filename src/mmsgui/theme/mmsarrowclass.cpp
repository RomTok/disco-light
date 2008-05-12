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

#include "mmsgui/theme/mmsarrowclass.h"

//store attribute descriptions here 
TAFF_ATTRDESC MMSGUI_ARROW_ATTR_I[] = MMSGUI_ARROW_ATTR_INIT;

//address attribute names
#define GETATTRNAME(aname) MMSGUI_ARROW_ATTR_I[MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_##aname].name
#define ISATTRNAME(aname) (strcmp(attrname, GETATTRNAME(aname))==0)


MMSArrowClass::MMSArrowClass() {
    unsetAll();
}

void MMSArrowClass::unsetAll() {
    this->className = "";
    unsetColor();
    unsetSelColor();
    unsetDirection();
}

void MMSArrowClass::setAttributesFromTAFF(MMSTaffFile *tafff, string *prefix, string *path) {
    DFBColor color;

    if (!prefix) {
		startTAFFScan
		{
	        switch (attrid) {
			case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
	            setClassName(attrval_str);
				break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_color:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            if (getColorFromString(attrval_str, &color))    
	                setColor(color);
	            break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_color_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.a = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_color_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.r = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_color_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.g = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_color_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.b = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_selcolor:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            if (getColorFromString(attrval_str, &color))    
	                setSelColor(color);
	            break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_selcolor_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.a = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_selcolor_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.r = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_selcolor_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.g = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_selcolor_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.b = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_ARROW_ATTR::MMSGUI_ARROW_ATTR_IDS_direction:
	            setDirection(getDirectionFromString(attrval_str));
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
            if (ISATTRNAME(direction)) { 
	            setDirection(getDirectionFromString(attrval_str));
			}
    	}
    	endTAFFScan_WITHOUT_ID
    }
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

