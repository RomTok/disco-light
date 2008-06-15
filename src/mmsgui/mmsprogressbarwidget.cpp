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

#include "mmsgui/mmsprogressbarwidget.h"

MMSProgressBarWidget::MMSProgressBarWidget(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget() {
    create(root, className, theme);
}

MMSProgressBarWidget::~MMSProgressBarWidget() {
}

bool MMSProgressBarWidget::create(MMSWindow *root, string className, MMSTheme *theme) {
	this->type = MMSWIDGETTYPE_PROGRESSBAR;
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->progressBarWidgetClass = this->theme->getProgressBarWidgetClass(className);
    this->baseWidgetClass = &(this->theme->progressBarWidgetClass.widgetClass);
    if (this->progressBarWidgetClass) this->widgetClass = &(this->progressBarWidgetClass->widgetClass); else this->widgetClass = NULL;

    return MMSWidget::create(root, true, false, false, true, true, true);
}

MMSWidget *MMSProgressBarWidget::copyWidget() {
    /* create widget */
    MMSProgressBarWidget *newWidget = new MMSProgressBarWidget(this->rootwindow, className); 

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    return newWidget;
}


bool MMSProgressBarWidget::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

    return true;
}

bool MMSProgressBarWidget::draw(bool *backgroundFilled) {
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
    
        DFBColor color;
    
        if (isSelected())
            color = getSelColor();
        else
            color = getColor();
    
        if (color.a) {
            /* prepare for drawing */        
            this->surface->setDrawingColorAndFlagsByBrightnessAndOpacity(color, getBrightness(), getOpacity());

            /* fill the rectangle */
            this->surface->fillRectangle(surfaceGeom.x, surfaceGeom.y,
                                        (int)((double)getProgress() / (double)100 * (double)surfaceGeom.w), surfaceGeom.h);
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

#define GETPBAR(x) \
    if (this->myProgressBarWidgetClass.is##x()) return myProgressBarWidgetClass.get##x(); \
    else if ((progressBarWidgetClass)&&(progressBarWidgetClass->is##x())) return progressBarWidgetClass->get##x(); \
    else return this->theme->progressBarWidgetClass.get##x();

DFBColor MMSProgressBarWidget::getColor() {
    GETPBAR(Color);
}

DFBColor MMSProgressBarWidget::getSelColor() {
    GETPBAR(SelColor);
}

unsigned int MMSProgressBarWidget::getProgress() {
    GETPBAR(Progress);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/
void MMSProgressBarWidget::setColor(DFBColor color, bool refresh) {
    myProgressBarWidgetClass.setColor(color);
    if (refresh)
        this->refresh();
}

void MMSProgressBarWidget::setSelColor(DFBColor selcolor, bool refresh) {
    myProgressBarWidgetClass.setSelColor(selcolor);
    if (refresh)
        this->refresh();
}

void MMSProgressBarWidget::setProgress(unsigned int progress, bool refresh) {
    if(progress>100)
        progress = 100;
    myProgressBarWidgetClass.setProgress(progress);
    if (refresh)
        this->refresh();
}


void MMSProgressBarWidget::updateFromThemeClass(MMSProgressBarWidgetClass *themeClass) {
    if (themeClass->isColor())
        setColor(themeClass->getColor());
    if (themeClass->isSelColor())
        setSelColor(themeClass->getSelColor());
    if (themeClass->isProgress())
        setProgress(themeClass->getProgress());

    MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
