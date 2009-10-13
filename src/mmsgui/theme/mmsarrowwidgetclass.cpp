/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
 ***************************************************************************/

#include "mmsgui/theme/mmsarrowwidgetclass.h"
#include <string.h>

//store attribute descriptions here
TAFF_ATTRDESC MMSGUI_ARROWWIDGET_ATTR_I[] = MMSGUI_ARROWWIDGET_ATTR_INIT;

//address attribute names
#define GETATTRNAME(aname) MMSGUI_ARROWWIDGET_ATTR_I[MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_##aname].name
#define ISATTRNAME(aname) (strcmp(attrname, GETATTRNAME(aname))==0)


MMSArrowWidgetClass::MMSArrowWidgetClass() {
    unsetAll();
}

void MMSArrowWidgetClass::unsetAll() {
    this->className = "";
    unsetColor();
    unsetSelColor();
    unsetDirection();
    unsetCheckSelected();
}

void MMSArrowWidgetClass::setAttributesFromTAFF(MMSTaffFile *tafff, string *prefix, string *path) {
    MMSFBColor color;

    if (!prefix) {
		startTAFFScan
		{
	        switch (attrid) {
			case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
	            setClassName(attrval_str);
				break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_color:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            if (getColorFromString(attrval_str, &color))
	                setColor(color);
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_color_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.a = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_color_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.r = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_color_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.g = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_color_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.b = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_selcolor:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            if (getColorFromString(attrval_str, &color))
	                setSelColor(color);
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_selcolor_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.a = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_selcolor_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.r = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_selcolor_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.g = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_selcolor_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.b = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_direction:
	            setDirection(getDirectionFromString(attrval_str));
	            break;
			case MMSGUI_ARROWWIDGET_ATTR::MMSGUI_ARROWWIDGET_ATTR_IDS_check_selected:
	            setCheckSelected((attrval_int) ? true : false);
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
            else
            if (ISATTRNAME(check_selected)) {
	            setCheckSelected((attrval_int) ? true : false);
			}
    	}
    	endTAFFScan_WITHOUT_ID
    }
}

void MMSArrowWidgetClass::setClassName(string className) {
    this->className = className;
}

string MMSArrowWidgetClass::getClassName() {
    return this->className;
}

bool MMSArrowWidgetClass::isColor() {
    return this->iscolor;
}

void MMSArrowWidgetClass::setColor(MMSFBColor color) {
    this->color = color;
    this->iscolor = true;
}

void MMSArrowWidgetClass::unsetColor() {
    this->iscolor = false;
}

MMSFBColor MMSArrowWidgetClass::getColor() {
    return this->color;
}

bool MMSArrowWidgetClass::isSelColor() {
    return this->isselcolor;
}

void MMSArrowWidgetClass::setSelColor(MMSFBColor selcolor) {
    this->selcolor = selcolor;
    this->isselcolor = true;
}

void MMSArrowWidgetClass::unsetSelColor() {
    this->isselcolor = false;
}

MMSFBColor MMSArrowWidgetClass::getSelColor() {
    return this->selcolor;
}

bool MMSArrowWidgetClass::isDirection() {
    return this->isdirection;
}

void MMSArrowWidgetClass::setDirection(MMSDIRECTION direction) {
    this->direction = direction;
    this->isdirection = true;
}

void MMSArrowWidgetClass::unsetDirection() {
    this->isdirection = false;
}

MMSDIRECTION MMSArrowWidgetClass::getDirection() {
    return this->direction;
}

bool MMSArrowWidgetClass::isCheckSelected() {
    return this->ischeckselected;
}

void MMSArrowWidgetClass::setCheckSelected(bool checkselected) {
    this->checkselected = checkselected;
    this->ischeckselected = true;
}

void MMSArrowWidgetClass::unsetCheckSelected() {
    this->ischeckselected = false;
}

bool MMSArrowWidgetClass::getCheckSelected() {
    return this->checkselected;
}

