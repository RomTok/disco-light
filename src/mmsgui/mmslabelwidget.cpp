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

#include "mmsgui/mmslabelwidget.h"


MMSLabelWidget::MMSLabelWidget(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget() {
    create(root, className, theme);
}

MMSLabelWidget::~MMSLabelWidget() {
    if (labelThread) {
        labelThread->stop();
        labelThread=NULL;
    }

    if (this->rootwindow) {
        this->rootwindow->fm->releaseFont(this->font);
    }
}

bool MMSLabelWidget::create(MMSWindow *root, string className, MMSTheme *theme) {
	this->type = MMSWIDGETTYPE_LABEL;
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->labelWidgetClass = this->theme->getLabelWidgetClass(className);
    this->baseWidgetClass = &(this->theme->labelWidgetClass.widgetClass);
    if (this->labelWidgetClass) this->widgetClass = &(this->labelWidgetClass->widgetClass); else this->widgetClass = NULL;

    /* clear */
    this->font = NULL;
    this->slid_width = 0;
    this->slid_offset = 0;
    this->labelThread = NULL;

    return MMSWidget::create(root, true, false, false, true, false, false);
}

MMSWidget *MMSLabelWidget::copyWidget() {
    /* create widget */
    MMSLabelWidget *newWidget = new MMSLabelWidget(this->rootwindow, className); 

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    /* reload my font */    
    newWidget->font = NULL;
    newWidget->labelThread = NULL;
    if (this->rootwindow) {
        newWidget->font = this->rootwindow->fm->getFont(newWidget->getFontPath(), newWidget->getFontName(), newWidget->getFontSize());

        // first time the label thread has to be started
        if (newWidget->getSlidable()) {
			newWidget->setSlidable(true);
        }
    }

    return newWidget;
}

bool MMSLabelWidget::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

    /* load font */
    this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());

    // first time the label thread has to be started
    if (getSlidable()) {
    	setSlidable(true);
    }

    return true;
}

bool MMSLabelWidget::draw(bool *backgroundFilled) {
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
    
            // save the width of the text
            this->slid_width = width;
            
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
                this->surface->drawString(text, -1, x - this->slid_offset, y);
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

#define GETLABEL(x) \
    if (this->myLabelWidgetClass.is##x()) return myLabelWidgetClass.get##x(); \
    else if ((labelWidgetClass)&&(labelWidgetClass->is##x())) return labelWidgetClass->get##x(); \
    else return this->theme->labelWidgetClass.get##x();

string MMSLabelWidget::getFontPath() {
    GETLABEL(FontPath);
}

string MMSLabelWidget::getFontName() {
    GETLABEL(FontName);
}

unsigned int MMSLabelWidget::getFontSize() {
    GETLABEL(FontSize);
}

MMSALIGNMENT MMSLabelWidget::getAlignment() {
    GETLABEL(Alignment);
}

DFBColor MMSLabelWidget::getColor() {
    GETLABEL(Color);
}

DFBColor MMSLabelWidget::getSelColor() {
    GETLABEL(SelColor);
}

string MMSLabelWidget::getText() {
    GETLABEL(Text);
}

bool MMSLabelWidget::getSlidable() {
    GETLABEL(Slidable);
}

unsigned int MMSLabelWidget::getSlideDelay() {
    GETLABEL(SlideDelay);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSLabelWidget::setFontPath(string fontpath, bool load, bool refresh) {
    myLabelWidgetClass.setFontPath(fontpath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setFontName(string fontname, bool load, bool refresh) {
    myLabelWidgetClass.setFontName(fontname);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setFontSize(unsigned int fontsize, bool load, bool refresh) {
    myLabelWidgetClass.setFontSize(fontsize);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setFont(string fontpath, string fontname, unsigned int fontsize, bool load, bool refresh) {
    myLabelWidgetClass.setFontPath(fontpath);
    myLabelWidgetClass.setFontName(fontname);
    myLabelWidgetClass.setFontSize(fontsize);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setAlignment(MMSALIGNMENT alignment, bool refresh) {
    myLabelWidgetClass.setAlignment(alignment);
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setColor(DFBColor color, bool refresh) {
    myLabelWidgetClass.setColor(color);
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setSelColor(DFBColor selcolor, bool refresh) {
    myLabelWidgetClass.setSelColor(selcolor);
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setText(string text, bool refresh) {
    myLabelWidgetClass.setText(text);
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setSlidable(bool slidable) {
    myLabelWidgetClass.setSlidable(slidable);
    if (slidable) {
    	// text should slide
    	this->slid_offset = 0;

        if (this->labelThread) {
            // toggle pause off
            this->labelThread->pause(false);
        }
        else {
            // start a thread
            this->labelThread = new MMSLabelWidgetThread(this);
            this->labelThread->start();
        }
    }
    else {
    	// static text
        if (labelThread)
            labelThread->stop();
    	this->slid_offset = 0;
    	this->refresh();
    }
}

void MMSLabelWidget::setSlideDelay(unsigned int slidedelay) {
    myLabelWidgetClass.setSlideDelay(slidedelay);
}

void MMSLabelWidget::updateFromThemeClass(MMSLabelWidgetClass *themeClass) {
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
    if (themeClass->isSlidable())
        setSlidable(themeClass->getSlidable());
    if (themeClass->isSlideDelay())
        setSlideDelay(themeClass->getSlideDelay());

    MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
