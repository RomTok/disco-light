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

#ifndef MMSMAINWINDOW_H_
#define MMSMAINWINDOW_H_

#include "mmswindow.h"

class MMSMainWindow : public MMSWindow {
	private:
        string              className;
        MMSMainWindowClass  *mainWindowClass;
        MMSMainWindowClass  myMainWindowClass;

        bool create(string className, string dx, string dy, string w, string h, MMSALIGNMENT alignment,
                    MMSWINDOW_FLAGS flags, MMSTheme *theme, bool *own_surface);

	public:
		MMSMainWindow(string className, string dx, string dy, string w, string h, MMSALIGNMENT alignment = MMSALIGNMENT_CENTER,
                      MMSWINDOW_FLAGS flags = MMSW_NONE, MMSTheme *theme = NULL, bool *own_surface = NULL);
		MMSMainWindow(string className, string w, string h, MMSALIGNMENT alignment = MMSALIGNMENT_CENTER,
                      MMSWINDOW_FLAGS flags = MMSW_NONE, MMSTheme *theme = NULL, bool *own_surface = NULL);
        virtual ~MMSMainWindow(); 
		MMSWINDOWTYPE getType() { return MMSWINDOWTYPE_MAINWINDOW; }

    public:
        /* theme access methods */
        void updateFromThemeClass(MMSMainWindowClass *themeClass);
};

#endif /*MMSMAINWINDOW_H_*/
