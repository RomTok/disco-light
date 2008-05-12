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

#ifndef MMSMAINWINDOWCLASS_H_
#define MMSMAINWINDOWCLASS_H_

#include "mmsgui/theme/mmswindowclass.h"

//! describe attributes for MMSMainWindow which are additional to the MMSWindowClass 
namespace MMSGUI_MAINWINDOW_ATTR {

	#define MMSGUI_MAINWINDOW_ATTR_INIT { \
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

extern TAFF_ATTRDESC MMSGUI_MAINWINDOW_ATTR_I[];


class MMSMainWindowClass {
    private:
        string className;

        //! Read and set all attributes from the given TAFF buffer.
        /*!
        \param tafff   pointer to the TAFF buffer
        \param path    optional, path needed for empty path values from the TAFF buffer
        */
        void setAttributesFromTAFF(MMSTaffFile *tafff, string *path = NULL);

    public:
        MMSWindowClass windowClass;
        //
        MMSMainWindowClass();
        //
        void unsetAll();
        
        void setClassName(string className);
        string getClassName();

    /* friends */
    friend class MMSThemeManager;
    friend class MMSDialogManager;
};

#endif /*MMSMAINWINDOWCLASS_H_*/
