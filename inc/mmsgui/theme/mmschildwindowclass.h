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

#ifndef MMSCHILDWINDOWCLASS_H_
#define MMSCHILDWINDOWCLASS_H_

#include "mmsgui/theme/mmswindowclass.h"

//describe attributes
namespace MMSGUI_CHILDWINDOW_ATTR {

	#define MMSGUI_CHILDWINDOW_ATTR_INIT { \
		MMSGUI_BASE_ATTR_ATTRDESC, \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WINDOW_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BASE_ATTR_IDS,
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WINDOW_ATTR_IDS
	} ids;
}

extern TAFF_ATTRDESC MMSGUI_CHILDWINDOW_ATTR_I[];

class MMSChildWindowClass {
    private:
        string          className;          //! name of the theme class

    public:
        MMSWindowClass windowClass;
        //
        MMSChildWindowClass();
        //
        void unsetAll();
        //
        void setAttributesFromXMLNode(MMSTaffFile *tafff, string path = "");
        //
        void setClassName(string className);
        string getClassName();
};

#endif /*MMSCHILDWINDOWCLASS_H_*/
