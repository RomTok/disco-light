/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#include "mmsgui/theme/mmsbuttonwidgetclass.h"

//store attribute descriptions here
TAFF_ATTRDESC MMSGUI_BUTTONWIDGET_ATTR_I[] = MMSGUI_BUTTONWIDGET_ATTR_INIT;

//address attribute names
#define GETATTRNAME(aname) MMSGUI_BUTTONWIDGET_ATTR_I[MMSGUI_BUTTONWIDGET_ATTR::MMSGUI_BUTTONWIDGET_ATTR_IDS_##aname].name
#define ISATTRNAME(aname) (strcmp(attrname, GETATTRNAME(aname))==0)


MMSButtonWidgetClass::MMSButtonWidgetClass() {
    unsetAll();
}

void MMSButtonWidgetClass::unsetAll() {
    this->className = "";
}

void MMSButtonWidgetClass::setAttributesFromTAFF(MMSTaffFile *tafff, string *prefix, string *path) {
	startTAFFScan
	{
        switch (attrid) {
		case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
            setClassName(attrval_str);
			break;
		}
	}
	endTAFFScan
}

void MMSButtonWidgetClass::setClassName(string className) {
    this->className = className;
}

string MMSButtonWidgetClass::getClassName() {
    return this->className;
}

