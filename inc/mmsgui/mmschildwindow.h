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

#ifndef MMSCHILDWINDOW_H_
#define MMSCHILDWINDOW_H_

#include "mmswindow.h"

class MMSChildWindow : public MMSWindow {
    private:
        string              className;
        MMSChildWindowClass *childWindowClass;
        MMSChildWindowClass myChildWindowClass;

        bool create(string className, MMSWindow *parent,
                    string dx, string dy, string w, string h, MMSALIGNMENT alignment,
                    MMSWINDOW_FLAGS flags, MMSTheme *theme, bool *own_surface);

    public:
        MMSChildWindow(string className, MMSWindow *parent,
                       string dx, string dy, string w, string h, MMSALIGNMENT alignment = MMSALIGNMENT_CENTER,
                       MMSWINDOW_FLAGS flags = MMSW_NONE, MMSTheme *theme = NULL, bool *own_surface = NULL);
        MMSChildWindow(string className, MMSWindow *parent,
                       string w, string h, MMSALIGNMENT alignment = MMSALIGNMENT_CENTER,
                       MMSWINDOW_FLAGS flags = MMSW_NONE, MMSTheme *theme = NULL, bool *own_surface = NULL);
        virtual ~MMSChildWindow(); 
        MMSWINDOWTYPE getType() { return MMSWINDOWTYPE_CHILDWINDOW; }

    public:
        /* theme access methods */
        void updateFromThemeClass(MMSChildWindowClass *themeClass);
};

#endif /*MMSCHILDWINDOW_H_*/
