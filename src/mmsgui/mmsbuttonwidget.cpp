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

#include "mmsgui/mmsbuttonwidget.h"

MMSButtonWidget::MMSButtonWidget(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget() {
    create(root, className, theme);
}

MMSButtonWidget::~MMSButtonWidget() {
}

bool MMSButtonWidget::create(MMSWindow *root, string className, MMSTheme *theme) {
	this->type = MMSWIDGETTYPE_BUTTON;
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->buttonWidgetClass = this->theme->getButtonWidgetClass(className);
    this->baseWidgetClass = &(this->theme->buttonWidgetClass.widgetClass);
    if (this->buttonWidgetClass) this->widgetClass = &(this->buttonWidgetClass->widgetClass); else this->widgetClass = NULL;

    return MMSWidget::create(root, true, false, true, true, true, true, true);
}

MMSWidget *MMSButtonWidget::copyWidget() {
    /* create widget */
    MMSButtonWidget *newWidget = new MMSButtonWidget(this->rootwindow, className);

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    return newWidget;
}

bool MMSButtonWidget::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

    return true;
}

bool MMSButtonWidget::draw(bool *backgroundFilled) {
    bool myBackgroundFilled = false;

    if (backgroundFilled) {
    	if (this->has_own_surface)
    		*backgroundFilled = false;
    }
    else
        backgroundFilled = &myBackgroundFilled;

    /* lock */
    this->surface->lock();

    /* draw widget basics */
    if (MMSWidget::draw(backgroundFilled)) {
        /* update window surface with an area of surface */
        updateWindowSurfaceWithSurface(!*backgroundFilled);
    }

    /* unlock */
    this->surface->unlock();

    /* draw widgets debug frame */
    return MMSWidget::drawDebug();
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSButtonWidget::updateFromThemeClass(MMSButtonWidgetClass *themeClass) {
    MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
