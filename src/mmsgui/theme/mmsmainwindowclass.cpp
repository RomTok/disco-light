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

#include "mmsgui/theme/mmsmainwindowclass.h"

//store attribute descriptions here 
TAFF_ATTRDESC MMSGUI_MAINWINDOW_ATTR_I[] = MMSGUI_MAINWINDOW_ATTR_INIT;


MMSMainWindowClass::MMSMainWindowClass() {
    unsetAll();
}

void MMSMainWindowClass::unsetAll() {
    this->className = "";
}

void MMSMainWindowClass::setAttributesFromXMLNode(MMSTaffFile *tafff, string path) {
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

void MMSMainWindowClass::setClassName(string className) {
    this->className = className;
}

string MMSMainWindowClass::getClassName() {
    return this->className;
}

