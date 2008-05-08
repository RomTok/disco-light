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

#ifndef MMSPOPUPWINDOW_H_
#define MMSPOPUPWINDOW_H_

#include "mmsgui/mmswindow.h"

class MMSPopupWindow : public MMSWindow {
	
	private:
        string              className;
        MMSPopupWindowClass *popupWindowClass;
        MMSPopupWindowClass myPopupWindowClass;

        unsigned int    counter;

        bool create(string className, string dx, string dy, string w, string h, MMSALIGNMENT alignment,
                    MMSWINDOW_FLAGS flags, MMSTheme *theme, bool *own_surface,
                    unsigned int duration);

	public:   
		MMSPopupWindow(string className, string dx, string dy, string w, string h, MMSALIGNMENT alignment = MMSALIGNMENT_CENTER,
                       MMSWINDOW_FLAGS flags = MMSW_NONE, MMSTheme *theme = NULL, bool *own_surface = NULL,
                       unsigned int duration = 0);
		MMSPopupWindow(string className, string w, string h, MMSALIGNMENT alignment = MMSALIGNMENT_CENTER,
                       MMSWINDOW_FLAGS flags = MMSW_NONE, MMSTheme *theme = NULL, bool *own_surface = NULL,
                       unsigned int duration = 0);
        virtual ~MMSPopupWindow(); 
		MMSWINDOWTYPE getType() { return MMSWINDOWTYPE_POPUPWINDOW; }

        virtual bool show();

		bool showAction(bool *stopaction);

    public:
        /* theme access methods */
        unsigned int getDuration();

        void setDuration(unsigned int duration);

        void updateFromThemeClass(MMSPopupWindowClass *themeClass);
};

#endif /*MMSPOPUPWINDOW_H_*/
