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

#include "mmsgui/theme/mmsbuttonclass.h"

//store attribute descriptions here 
TAFF_ATTRDESC MMSGUI_BUTTON_ATTR_I[] = MMSGUI_BUTTON_ATTR_INIT;

//address attribute names
#define GETATTRNAME(aname) MMSGUI_BUTTON_ATTR_I[MMSGUI_BUTTON_ATTR::MMSGUI_BUTTON_ATTR_IDS_##aname].name
#define ISATTRNAME(aname) (strcmp(attrname, GETATTRNAME(aname))==0)


MMSButtonClass::MMSButtonClass() {
    unsetAll();
}

void MMSButtonClass::unsetAll() {
    this->className = "";
}

void MMSButtonClass::setAttributesFromTAFF(MMSTaffFile *tafff, string prefix, string path) {
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

void MMSButtonClass::setClassName(string className) {
    this->className = className;
}

string MMSButtonClass::getClassName() {
    return this->className;
}

