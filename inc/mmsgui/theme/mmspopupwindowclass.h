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

#ifndef MMSPOPUPWINDOWCLASS_H_
#define MMSPOPUPWINDOWCLASS_H_

#include "mmsgui/theme/mmswindowclass.h"

//! describe attributes for MMSPopupWindow which are additional to the MMSWindowClass
namespace MMSGUI_POPUPWINDOW_ATTR {

	#define MMSGUI_POPUPWINDOW_ATTR_ATTRDESC \
		{ "duration", TAFF_ATTRTYPE_UCHAR }

	#define MMSGUI_POPUPWINDOW_ATTR_IDS \
		MMSGUI_POPUPWINDOW_ATTR_IDS_duration

	#define MMSGUI_POPUPWINDOW_ATTR_INIT { \
		MMSGUI_BASE_ATTR_ATTRDESC, \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WINDOW_ATTR_ATTRDESC, \
		MMSGUI_POPUPWINDOW_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BASE_ATTR_IDS,
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WINDOW_ATTR_IDS,
		MMSGUI_POPUPWINDOW_ATTR_IDS
	} ids;
}

extern TAFF_ATTRDESC MMSGUI_POPUPWINDOW_ATTR_I[];


class MMSPopupWindowClass {
    private:
        string       className;
        bool         isduration;
        unsigned int duration;

        //! Read and set all attributes from the given TAFF buffer.
        /*!
        \param tafff   pointer to the TAFF buffer
        \param path    optional, path needed for empty path values from the TAFF buffer
        */
        void setAttributesFromTAFF(MMSTaffFile *tafff, string *path = NULL);

    public:
        MMSWindowClass windowClass;

        MMSPopupWindowClass();
        //
        void unsetAll();

        void setClassName(string className);
        string getClassName();
        //
        bool isDuration();
        void setDuration(unsigned int duration);
        void unsetDuration();
        unsigned int getDuration();

    /* friends */
    friend class MMSThemeManager;
    friend class MMSDialogManager;
};

#endif /*MMSPOPUPWINDOWCLASS_H_*/
