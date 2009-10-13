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

#ifndef MMSMAINWINDOW_H_
#define MMSMAINWINDOW_H_

#include "mmsgui/mmswindow.h"

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

    public:
        /* theme access methods */
        void updateFromThemeClass(MMSMainWindowClass *themeClass);
};

#endif /*MMSMAINWINDOW_H_*/
