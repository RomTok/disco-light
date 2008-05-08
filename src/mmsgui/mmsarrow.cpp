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

#include "mmsgui/mmsarrow.h"

MMSArrow::MMSArrow(MMSWindow *root, string className, MMSTheme *theme) {
    create(root, className, theme);
} 

MMSArrow::~MMSArrow() {
}

bool MMSArrow::create(MMSWindow *root, string className, MMSTheme *theme) {
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->arrowClass = this->theme->getArrowClass(className);
    this->baseWidgetClass = &(this->theme->arrowClass.widgetClass);
    if (this->arrowClass) this->widgetClass = &(this->arrowClass->widgetClass); else this->widgetClass = NULL;

    return MMSWidget::create(root, true, false, false, true, true, true);
}

MMSWidget *MMSArrow::copyWidget() {
    /* create widget */
    MMSArrow *newWidget = new MMSArrow(this->rootwindow, className); 

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    return newWidget;
}

bool MMSArrow::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

    return true;
}

bool MMSArrow::draw(bool *backgroundFilled) {
    bool myBackgroundFilled = false;

    if (backgroundFilled) {
    	if (this->has_own_surface)
    		*backgroundFilled = false;
    }
    else
        backgroundFilled = &myBackgroundFilled;

    /* draw widget basics */
    if (MMSWidget::draw(backgroundFilled)) {

        /* lock */
        this->surface->lock();

        /* draw my things */
        DFBRectangle surfaceGeom = getSurfaceGeometry();

        /* get color */
        DFBColor color;
        if (isSelected())
            color = getSelColor();
        else
            color = getColor();

        if (color.a) {
            /* prepare for drawing */        
            this->surface->setDrawingColorAndFlagsByBrightnessAndOpacity(color, getBrightness(), getOpacity());

            /* draw triangle */
            switch (getDirection()) {
                case MMSDIRECTION_LEFT:
                case MMSDIRECTION_NOTSET:
                    /* draw triangle */
                    this->surface->drawTriangle(
                                    surfaceGeom.x,                     surfaceGeom.y + surfaceGeom.h/2,
                                    surfaceGeom.x + surfaceGeom.w - 1, surfaceGeom.y,
                                    surfaceGeom.x + surfaceGeom.w - 1, surfaceGeom.y + surfaceGeom.h - 1 + (surfaceGeom.h % 2 - 1));
                    /* fill triangle */
                    this->surface->fillTriangle(
                                    surfaceGeom.x,                     surfaceGeom.y + surfaceGeom.h/2,
                                    surfaceGeom.x + surfaceGeom.w - 1, surfaceGeom.y,
                                    surfaceGeom.x + surfaceGeom.w - 1, surfaceGeom.y + surfaceGeom.h - 1 + (surfaceGeom.h % 2 - 1));
                    break;
                case MMSDIRECTION_RIGHT:
                    /* draw triangle */
                    this->surface->drawTriangle(
                                    surfaceGeom.x + surfaceGeom.w - 1, surfaceGeom.y + surfaceGeom.h/2,
                                    surfaceGeom.x,                     surfaceGeom.y,
                                    surfaceGeom.x,                     surfaceGeom.y + surfaceGeom.h - 1 + (surfaceGeom.h % 2 - 1));
                    /* fill triangle */
                    this->surface->fillTriangle(
                                    surfaceGeom.x + surfaceGeom.w - 1, surfaceGeom.y + surfaceGeom.h/2,
                                    surfaceGeom.x,                     surfaceGeom.y,
                                    surfaceGeom.x,                     surfaceGeom.y + surfaceGeom.h - 1 + (surfaceGeom.h % 2 - 1));
                    break;
                case MMSDIRECTION_UP:
                    /* draw triangle */
                    this->surface->drawTriangle(
                                    surfaceGeom.x + surfaceGeom.w/2,                            surfaceGeom.y,
                                    surfaceGeom.x,                                              surfaceGeom.y + surfaceGeom.h - 1,
                                    surfaceGeom.x + surfaceGeom.w - 1 + (surfaceGeom.w % 2 - 1),surfaceGeom.y + surfaceGeom.h - 1);
                    /* fill triangle */
                    this->surface->fillTriangle(
                                    surfaceGeom.x + surfaceGeom.w/2,                            surfaceGeom.y,
                                    surfaceGeom.x,                                              surfaceGeom.y + surfaceGeom.h - 1,
                                    surfaceGeom.x + surfaceGeom.w - 1 + (surfaceGeom.w % 2 - 1),surfaceGeom.y + surfaceGeom.h - 1);
                    break;
                case MMSDIRECTION_DOWN:
                    /* draw triangle */
                    this->surface->drawTriangle(
                                    surfaceGeom.x + surfaceGeom.w/2,                            surfaceGeom.y + surfaceGeom.h - 1,
                                    surfaceGeom.x,                                              surfaceGeom.y,
                                    surfaceGeom.x + surfaceGeom.w - 1 + (surfaceGeom.w % 2 - 1),surfaceGeom.y);
                    /* fill triangle */
                    this->surface->fillTriangle(
                                    surfaceGeom.x + surfaceGeom.w/2,                            surfaceGeom.y + surfaceGeom.h - 1,
                                    surfaceGeom.x,                                              surfaceGeom.y,
                                    surfaceGeom.x + surfaceGeom.w - 1 + (surfaceGeom.w % 2 - 1),surfaceGeom.y);
                    break;
            }
        }

        /* unlock */
        this->surface->unlock();

        /* update window surface with an area of surface */
        updateWindowSurfaceWithSurface(!*backgroundFilled);
    }

    /* draw widgets debug frame */
    return MMSWidget::drawDebug();
}

/***********************************************/
/* begin of theme access methods (get methods) */
/***********************************************/

#define GETARROW(x) \
    if (this->myArrowClass.is##x()) return myArrowClass.get##x(); \
    else if ((arrowClass)&&(arrowClass->is##x())) return arrowClass->get##x(); \
    else return this->theme->arrowClass.get##x();

DFBColor MMSArrow::getColor() {
    GETARROW(Color);
}

DFBColor MMSArrow::getSelColor() {
    GETARROW(SelColor);
}

MMSDIRECTION MMSArrow::getDirection() {
    GETARROW(Direction);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSArrow::setColor(DFBColor color, bool refresh) {
    myArrowClass.setColor(color);
    if (refresh)
        this->refresh();
}

void MMSArrow::setSelColor(DFBColor selcolor, bool refresh) {
    myArrowClass.setSelColor(selcolor);
    if (refresh)
        this->refresh();
}

void MMSArrow::setDirection(MMSDIRECTION direction, bool refresh) {
    myArrowClass.setDirection(direction);
    if (refresh)
        this->refresh();
}

void MMSArrow::updateFromThemeClass(MMSArrowClass *themeClass) {
    if (themeClass->isColor())
        setColor(themeClass->getColor());
    if (themeClass->isSelColor())
        setSelColor(themeClass->getSelColor());
    if (themeClass->isDirection())
        setDirection(themeClass->getDirection());

    MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
