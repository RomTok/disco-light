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

#include "mmsgui/mmsprogressbar.h"

MMSProgressBar::MMSProgressBar(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget() {
    create(root, className, theme);
}

MMSProgressBar::~MMSProgressBar() {
}

bool MMSProgressBar::create(MMSWindow *root, string className, MMSTheme *theme) {
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->progressBarClass = this->theme->getProgressBarClass(className);
    this->baseWidgetClass = &(this->theme->progressBarClass.widgetClass);
    if (this->progressBarClass) this->widgetClass = &(this->progressBarClass->widgetClass); else this->widgetClass = NULL;

    return MMSWidget::create(root, true, false, false, true, true, true);
}

MMSWidget *MMSProgressBar::copyWidget() {
    /* create widget */
    MMSProgressBar *newWidget = new MMSProgressBar(this->rootwindow, className); 

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    return newWidget;
}


bool MMSProgressBar::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

    return true;
}

bool MMSProgressBar::draw(bool *backgroundFilled) {
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
    if (this->myProgressBarClass.is##x()) return myProgressBarClass.get##x(); \
    else if ((progressBarClass)&&(progressBarClass->is##x())) return progressBarClass->get##x(); \
    else return this->theme->progressBarClass.get##x();

DFBColor MMSProgressBar::getColor() {
    GETPBAR(Color);
}

DFBColor MMSProgressBar::getSelColor() {
    GETPBAR(SelColor);
}

unsigned int MMSProgressBar::getProgress() {
    GETPBAR(Progress);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/
void MMSProgressBar::setColor(DFBColor color, bool refresh) {
    myProgressBarClass.setColor(color);
    if (refresh)
        this->refresh();
}

void MMSProgressBar::setSelColor(DFBColor selcolor, bool refresh) {
    myProgressBarClass.setSelColor(selcolor);
    if (refresh)
        this->refresh();
}

void MMSProgressBar::setProgress(unsigned int progress, bool refresh) {
    if(progress>100)
        progress = 100;
    myProgressBarClass.setProgress(progress);
    if (refresh)
        this->refresh();
}


void MMSProgressBar::updateFromThemeClass(MMSProgressBarClass *themeClass) {
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
