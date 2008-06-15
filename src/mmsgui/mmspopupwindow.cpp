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

#include "mmsgui/mmspopupwindow.h"

MMSPopupWindow::MMSPopupWindow(string className, string dx, string dy, string w, string h, MMSALIGNMENT alignment,
                               MMSWINDOW_FLAGS flags, MMSTheme *theme, bool *own_surface, unsigned int duration) {
	create(className, dx, dy, w, h, alignment, flags, theme, own_surface, duration);
}

MMSPopupWindow::MMSPopupWindow(string className, string w, string h, MMSALIGNMENT alignment,
                               MMSWINDOW_FLAGS flags, MMSTheme *theme, bool *own_surface, unsigned int duration) {
	create(className, "", "", w, h, alignment, flags, theme, own_surface, duration);
}     

MMSPopupWindow::~MMSPopupWindow() {
}

bool MMSPopupWindow::create(string className, string dx, string dy, string w, string h, MMSALIGNMENT alignment,
                            MMSWINDOW_FLAGS flags, MMSTheme *theme, bool *own_surface, unsigned int duration) {
	this->type = MMSWINDOWTYPE_POPUPWINDOW;
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->popupWindowClass = this->theme->getPopupWindowClass(className);
    this->baseWindowClass = &(this->theme->popupWindowClass.windowClass);
    if (this->popupWindowClass) this->windowClass = &(this->popupWindowClass->windowClass); else this->windowClass = NULL;

    this->counter = 0;

    if (duration)
        setDuration(duration);

	return MMSWindow::create(dx, dy, w, h, alignment, flags, own_surface);
}

bool MMSPopupWindow::show() {
    this->counter = 0;
    return MMSWindow::show();
}

bool MMSPopupWindow::showAction(bool *stopaction) {
	bool	      ret;
    unsigned int  duration;

	ret = MMSWindow::showAction(stopaction);

    duration = getDuration();
	if(duration > 0) {
		if (ret) {
			while (this->counter < duration*4) {
				msleep(250);
				if (*stopaction)
					break;
                this->counter++;
			}
		}
		else
			msleep(250);
		if (!*stopaction)
			this->hideAction(stopaction);
		*stopaction=false;
	}
	
	return ret;
}


/***********************************************/
/* begin of theme access methods (get methods) */
/***********************************************/

#define GETPOPUPWINDOW(x) \
    if (this->myPopupWindowClass.is##x()) return myPopupWindowClass.get##x(); \
    else if ((popupWindowClass)&&(popupWindowClass->is##x())) return popupWindowClass->get##x(); \
    else return this->theme->popupWindowClass.get##x();

unsigned int MMSPopupWindow::getDuration() {
    GETPOPUPWINDOW(Duration);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSPopupWindow::setDuration(unsigned int duration) {
    myPopupWindowClass.setDuration(duration);
}

void MMSPopupWindow::updateFromThemeClass(MMSPopupWindowClass *themeClass) {
    if (themeClass->isDuration())
        setDuration(themeClass->getDuration());

    MMSWindow::updateFromThemeClass(&(themeClass->windowClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
