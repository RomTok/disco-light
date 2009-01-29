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

#include "mmsgui/mmschildwindow.h"

MMSChildWindow::MMSChildWindow(string className, MMSWindow *parent,
                               string dx, string dy, string w, string h, MMSALIGNMENT alignment,
                               MMSWINDOW_FLAGS flags, MMSTheme *theme, bool *own_surface) {
    create(className, parent, dx, dy, w, h, alignment, flags, theme, own_surface);
}

MMSChildWindow::MMSChildWindow(string className, MMSWindow *parent, string w, string h, MMSALIGNMENT alignment,
                               MMSWINDOW_FLAGS flags, MMSTheme *theme, bool *own_surface) {
    create(className, parent, "", "", w, h, alignment, flags, theme, own_surface);
}

MMSChildWindow::~MMSChildWindow() {
}

bool MMSChildWindow::create(string className, MMSWindow *parent,
                            string dx, string dy, string w, string h, MMSALIGNMENT alignment,
                            MMSWINDOW_FLAGS flags, MMSTheme *theme, bool *own_surface) {
	this->type = MMSWINDOWTYPE_CHILDWINDOW;
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->childWindowClass = this->theme->getChildWindowClass(className);
    this->baseWindowClass = &(this->theme->childWindowClass.windowClass);
    if (this->childWindowClass) this->windowClass = &(this->childWindowClass->windowClass); else this->windowClass = NULL;

printf("create child window %x with parent = %x\n", this, parent);
    this->parent = parent;

    return MMSWindow::create(dx, dy, w, h, alignment, flags, own_surface);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

#define GETCHILDWINDOW(x,y) \
    if (this->myChildWindowClass.is##x()) return myChildWindowClass.get##x(y); \
    else if ((childWindowClass)&&(childWindowClass->is##x())) return childWindowClass->get##x(y); \
    else return this->theme->childWindowClass.get##x(y);

bool MMSChildWindow::getModal(bool &modal) {
    GETCHILDWINDOW(Modal, modal);
}

void MMSChildWindow::setModal(bool modal) {
    myChildWindowClass.setModal(modal);
}

void MMSChildWindow::updateFromThemeClass(MMSChildWindowClass *themeClass) {
	bool b;

	if (themeClass->getModal(b))
        setModal(b);

    MMSWindow::updateFromThemeClass(&(themeClass->windowClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
