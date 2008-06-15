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

#include "mmsgui/mmsinputwidget.h"


MMSInputWidget::MMSInputWidget(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget() {
    create(root, className, theme);
}

MMSInputWidget::~MMSInputWidget() {
    if (this->rootwindow) {
        this->rootwindow->fm->releaseFont(this->font);
    }
}

bool MMSInputWidget::create(MMSWindow *root, string className, MMSTheme *theme) {
	this->type = MMSWIDGETTYPE_INPUT;
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->inputWidgetClass = this->theme->getInputWidgetClass(className);
    this->baseWidgetClass = &(this->theme->inputWidgetClass.widgetClass);
    if (this->inputWidgetClass) this->widgetClass = &(this->inputWidgetClass->widgetClass); else this->widgetClass = NULL;

    /* clear */
    this->font = NULL;

    return MMSWidget::create(root, true, false, true, true, false, false);
}

MMSWidget *MMSInputWidget::copyWidget() {
    /* create widget */
    MMSInputWidget *newWidget = new MMSInputWidget(this->rootwindow, className); 

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    /* reload my font */    
    newWidget->font = NULL;
    if (this->rootwindow) {
        newWidget->font = this->rootwindow->fm->getFont(newWidget->getFontPath(), newWidget->getFontName(), newWidget->getFontSize());
    }

    return newWidget;
}

bool MMSInputWidget::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

    /* load font */
    this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());

    return true;
}

bool MMSInputWidget::draw(bool *backgroundFilled) {
    int width, height, x, y;
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

        /* draw my things */
        if (this->font) {
            DFBRectangle surfaceGeom = getSurfaceGeometry();

            this->surface->setFont(this->font);

            string text = getText();

            /* get width and height of the string to be drawn */            
            this->font->GetStringWidth(this->font, text.c_str(), -1, &width);
            this->font->GetHeight(this->font, &height);
    
            switch (getAlignment()) {
                case MMSALIGNMENT_LEFT:
                    x = surfaceGeom.x;
                    y = ((surfaceGeom.h - height) / 2) + surfaceGeom.y;
                    break;
                case MMSALIGNMENT_RIGHT:
                    x = surfaceGeom.x + surfaceGeom.w - width;
                    y = ((surfaceGeom.h - height) / 2) + surfaceGeom.y;
                    break;
                case MMSALIGNMENT_CENTER:
                    x = ((surfaceGeom.w - width) / 2) + surfaceGeom.x;
                    y = ((surfaceGeom.h - height) / 2) + surfaceGeom.y;
                    break;
                case MMSALIGNMENT_TOP_CENTER:
                    x = ((surfaceGeom.w - width) / 2) + surfaceGeom.x;
                    y = surfaceGeom.y;
                    break;
                case MMSALIGNMENT_TOP_LEFT:
                    x = surfaceGeom.x;
                    y = surfaceGeom.y;
                    break;
                case MMSALIGNMENT_TOP_RIGHT:
                    x = surfaceGeom.x + surfaceGeom.w - width;
                    y = surfaceGeom.y;
                    break;
                case MMSALIGNMENT_BOTTOM_CENTER:
                    x = ((surfaceGeom.w - width) / 2) + surfaceGeom.x;
                    y = surfaceGeom.y + surfaceGeom.h - height;
                    break;
                case MMSALIGNMENT_BOTTOM_LEFT:
                    x = surfaceGeom.x;
                    y = surfaceGeom.y + surfaceGeom.h - height;
                    break;
                case MMSALIGNMENT_BOTTOM_RIGHT:
                    x = surfaceGeom.x + surfaceGeom.w - width;
                    y = surfaceGeom.y + surfaceGeom.h - height;
                    break;
                default:
                    x = ((surfaceGeom.w - width) / 2) + surfaceGeom.x;
                    y = ((surfaceGeom.h - height) / 2) + surfaceGeom.y;
                    break;
            }
            
            DFBColor color;
            
            if (isSelected())
                color = getSelColor();
            else
                color = getColor();
            
            if (color.a) {
                /* prepare for drawing */        
                this->surface->setDrawingColorAndFlagsByBrightnessAndOpacity(color, getBrightness(), getOpacity());

                /* draw the text */
                this->surface->drawString(text, -1, x, y);
            }
        }

        /* update window surface with an area of surface */
        updateWindowSurfaceWithSurface(!*backgroundFilled);
    }

    /* unlock */
    this->surface->unlock();

    /* draw widgets debug frame */
    return MMSWidget::drawDebug();
}

/***********************************************/
/* begin of theme access methods (get methods) */
/***********************************************/

#define GETINPUT(x) \
    if (this->myInputWidgetClass.is##x()) return myInputWidgetClass.get##x(); \
    else if ((inputWidgetClass)&&(inputWidgetClass->is##x())) return inputWidgetClass->get##x(); \
    else return this->theme->inputWidgetClass.get##x();

string MMSInputWidget::getFontPath() {
    GETINPUT(FontPath);
}

string MMSInputWidget::getFontName() {
    GETINPUT(FontName);
}

unsigned int MMSInputWidget::getFontSize() {
    GETINPUT(FontSize);
}

MMSALIGNMENT MMSInputWidget::getAlignment() {
    GETINPUT(Alignment);
}

DFBColor MMSInputWidget::getColor() {
    GETINPUT(Color);
}

DFBColor MMSInputWidget::getSelColor() {
    GETINPUT(SelColor);
}

string MMSInputWidget::getText() {
    GETINPUT(Text);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSInputWidget::setFontPath(string fontpath, bool load, bool refresh) {
    myInputWidgetClass.setFontPath(fontpath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSInputWidget::setFontName(string fontname, bool load, bool refresh) {
    myInputWidgetClass.setFontName(fontname);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSInputWidget::setFontSize(unsigned int fontsize, bool load, bool refresh) {
    myInputWidgetClass.setFontSize(fontsize);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSInputWidget::setFont(string fontpath, string fontname, unsigned int fontsize, bool load, bool refresh) {
    myInputWidgetClass.setFontPath(fontpath);
    myInputWidgetClass.setFontName(fontname);
    myInputWidgetClass.setFontSize(fontsize);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSInputWidget::setAlignment(MMSALIGNMENT alignment, bool refresh) {
    myInputWidgetClass.setAlignment(alignment);
    if (refresh)
        this->refresh();
}

void MMSInputWidget::setColor(DFBColor color, bool refresh) {
    myInputWidgetClass.setColor(color);
    if (refresh)
        this->refresh();
}

void MMSInputWidget::setSelColor(DFBColor selcolor, bool refresh) {
    myInputWidgetClass.setSelColor(selcolor);
    if (refresh)
        this->refresh();
}

void MMSInputWidget::setText(string text, bool refresh) {
    myInputWidgetClass.setText(text);
    if (refresh)
        this->refresh();
}

void MMSInputWidget::updateFromThemeClass(MMSInputWidgetClass *themeClass) {
    if (themeClass->isFontPath())
        setFontPath(themeClass->getFontPath());
    if (themeClass->isFontName())
        setFontName(themeClass->getFontName());
    if (themeClass->isFontSize())
        setFontSize(themeClass->getFontSize());
    if (themeClass->isAlignment())
        setAlignment(themeClass->getAlignment());
    if (themeClass->isColor())
        setColor(themeClass->getColor());
    if (themeClass->isSelColor())
        setSelColor(themeClass->getSelColor());
    if (themeClass->isText())
        setText(themeClass->getText());

    MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
