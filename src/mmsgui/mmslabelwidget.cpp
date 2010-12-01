/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2010 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mmsgui/mmslabelwidget.h"


MMSLabelWidget::MMSLabelWidget(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget() {
    create(root, className, theme);
}

MMSLabelWidget::~MMSLabelWidget() {
    if (labelThread) {
        labelThread->stop();
        labelThread=NULL;
    }
}

bool MMSLabelWidget::create(MMSWindow *root, string className, MMSTheme *theme) {
	this->type = MMSWIDGETTYPE_LABEL;
    this->className = className;

    // init attributes for drawable widgets
	this->da = new MMSWIDGET_DRAWABLE_ATTRIBUTES;
    if (theme) this->da->theme = theme; else this->da->theme = globalTheme;
    this->labelWidgetClass = this->da->theme->getLabelWidgetClass(className);
    this->da->baseWidgetClass = &(this->da->theme->labelWidgetClass.widgetClass);
    if (this->labelWidgetClass) this->da->widgetClass = &(this->labelWidgetClass->widgetClass); else this->da->widgetClass = NULL;

    // clear
    initLanguage();
	this->fontpath = "";
	this->fontname = "";
	this->fontsize = 0;
    this->font = NULL;
    this->load_font = true;
    this->slide_width = 0;
    this->slide_offset = 0;
    this->frame_delay = 100;
    this->frame_delay_set = false;
    this->labelThread = NULL;
    this->translated = false;

    return MMSWidget::create(root, true, false, false, true, false, false, false);
}

MMSWidget *MMSLabelWidget::copyWidget() {
    // create widget
    MMSLabelWidget *newWidget = new MMSLabelWidget(this->rootwindow, className);

    // copy widget
    *newWidget = *this;

    // copy base widget
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    // reload my font
    initLanguage(newWidget);
    newWidget->fontpath = "";
	newWidget->fontname = "";
	newWidget->fontsize = 0;
    newWidget->font = NULL;
    newWidget->load_font = true;
    newWidget->labelThread = NULL;
    if (this->rootwindow) {
    	// load font
        loadFont(newWidget);

        // first time the label thread has to be started
        if (newWidget->getSlidable()) {
			newWidget->setSlidable(true);
        }
    }

    return newWidget;
}


void MMSLabelWidget::initLanguage(MMSLabelWidget *widget) {
	if (!widget) widget = this;

	widget->lang = (!this->rootwindow)?MMSLANG_NONE:this->rootwindow->windowmanager->getTranslator()->getTargetLang();
}

void MMSLabelWidget::loadFont(MMSLabelWidget *widget) {
	if (!this->load_font) return;
	if (!widget) widget = this;

	if (this->rootwindow) {
		// get font parameter
		widget->lang = this->rootwindow->windowmanager->getTranslator()->getTargetLang();
    	string fontpath = widget->getFontPath();
    	string fontname = widget->getFontName(widget->lang);
    	unsigned int fontsize = widget->getFontSize();

    	if (fontpath != widget->fontpath || fontname != widget->fontname || fontsize != widget->fontsize || !widget->font) {
    		// font parameter changed, (re)load it
			if (widget->font)
				this->rootwindow->fm->releaseFont(widget->font);
			widget->fontpath = fontpath;
			widget->fontname = fontname;
			widget->fontsize = fontsize;
			widget->font = this->rootwindow->fm->getFont(widget->fontpath, widget->fontname, widget->fontsize);
			if (this->font) this->load_font = false;
    	}
    	else {
    		// font parameter not changed, so we do not reload it
            this->load_font = false;
    	}
    }
}

bool MMSLabelWidget::init() {
    // init widget basics
    if (!MMSWidget::init())
        return false;

	// init language
    initLanguage();

    // load font
	loadFont();

    // first time the label thread has to be started
    if (getSlidable()) {
    	setSlidable(true);
    }

    return true;
}

bool MMSLabelWidget::release() {
    // release widget basics
    if (!MMSWidget::release())
        return false;

    // release my font
    this->rootwindow->fm->releaseFont(this->font);
    this->fontpath = "";
    this->fontname = "";
    this->fontsize = 0;
    this->font = NULL;
    this->load_font = true;

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

    // lock
    this->surface->lock();

    // draw widget basics
    if (MMSWidget::draw(backgroundFilled)) {
   		// check if we have to (re)load the font
   	    loadFont();

		// draw my things
        if (this->font) {
            MMSFBRectangle surfaceGeom = getSurfaceGeometry();

            this->surface->setFont(this->font);

            if (!this->translated) {
            	if ((this->rootwindow)&&(this->rootwindow->windowmanager)&&(getTranslate())) {
					// translate the text
            		string source;
            		getText(source);
            		this->rootwindow->windowmanager->getTranslator()->translate(source, this->translated_text);
            	}
            	else {
            		// text can not or should not translated
					getText(this->translated_text);
            	}

            	// mark as translated
            	this->translated = true;
            }

            // get width and height of the string to be drawn
            this->font->getStringWidth(this->translated_text, -1, &width);
            this->font->getHeight(&height);

            // save the width of the text
            this->slide_width = width;

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

            MMSFBColor color;

            if (isSelected())
                color = getSelColor();
            else
                color = getColor();

            if (color.a) {
                // prepare for drawing
                this->surface->setDrawingColorAndFlagsByBrightnessAndOpacity(color, getBrightness(), getOpacity());

                // draw the text
                this->surface->drawString(this->translated_text, -1, x - this->slide_offset, y);
            }
        }

        // update window surface with an area of surface
        updateWindowSurfaceWithSurface(!*backgroundFilled);
    }

    // unlock
    this->surface->unlock();

    // draw widgets debug frame
    return MMSWidget::drawDebug();
}

void MMSLabelWidget::targetLangChanged(MMSLanguage lang) {
    this->translated = false;
    this->load_font = true;
}

/***********************************************/
/* begin of theme access methods (get methods) */
/***********************************************/

#define GETLABEL(x) \
    if (this->myLabelWidgetClass.is##x()) return myLabelWidgetClass.get##x(); \
    else if ((labelWidgetClass)&&(labelWidgetClass->is##x())) return labelWidgetClass->get##x(); \
    else return this->da->theme->labelWidgetClass.get##x();

#define GETLABEL2(x,y) \
    if (this->myLabelWidgetClass.is##x()) y=myLabelWidgetClass.get##x(); \
    else if ((labelWidgetClass)&&(labelWidgetClass->is##x())) y=labelWidgetClass->get##x(); \
    else y=this->da->theme->labelWidgetClass.get##x();

#define GETLABELFONT(lang) \
    if (this->myLabelWidgetClass.isFontName(lang)) return myLabelWidgetClass.getFontName(lang); \
    else if (this->myLabelWidgetClass.isFontName(MMSLANG_NONE)) return myLabelWidgetClass.getFontName(MMSLANG_NONE); \
    else if ((labelWidgetClass)&&(labelWidgetClass->isFontName(lang))) return labelWidgetClass->getFontName(lang); \
    else if ((labelWidgetClass)&&(labelWidgetClass->isFontName(MMSLANG_NONE))) return labelWidgetClass->getFontName(MMSLANG_NONE); \
    else return this->da->theme->labelWidgetClass.getFontName();

string MMSLabelWidget::getFontPath() {
    GETLABEL(FontPath);
}

string MMSLabelWidget::getFontName(MMSLanguage lang) {
    GETLABELFONT(lang);
}

unsigned int MMSLabelWidget::getFontSize() {
    GETLABEL(FontSize);
}

MMSALIGNMENT MMSLabelWidget::getAlignment() {
    GETLABEL(Alignment);
}

MMSFBColor MMSLabelWidget::getColor() {
    GETLABEL(Color);
}

MMSFBColor MMSLabelWidget::getSelColor() {
    GETLABEL(SelColor);
}

string MMSLabelWidget::getText() {
    GETLABEL(Text);
}

void MMSLabelWidget::getText(string &text) {
    GETLABEL2(Text, text);
}

bool MMSLabelWidget::getSlidable() {
    GETLABEL(Slidable);
}

unsigned char MMSLabelWidget::getSlideSpeed() {
    GETLABEL(SlideSpeed);
}

bool MMSLabelWidget::getTranslate() {
    GETLABEL(Translate);
}


/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSLabelWidget::setFontPath(string fontpath, bool load, bool refresh) {
    myLabelWidgetClass.setFontPath(fontpath);
    if (load) {
        this->load_font = true;
    	loadFont();
    }
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setFontName(MMSLanguage lang, string fontname, bool load, bool refresh) {
    myLabelWidgetClass.setFontName(fontname, lang);
    if (load) {
        this->load_font = true;
    	loadFont();
    }
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setFontName(string fontname, bool load, bool refresh) {
	setFontName(MMSLANG_NONE, fontname, load, refresh);
}

void MMSLabelWidget::setFontSize(unsigned int fontsize, bool load, bool refresh) {
    myLabelWidgetClass.setFontSize(fontsize);
    if (load) {
        this->load_font = true;
    	loadFont();
    }
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setFont(MMSLanguage lang, string fontpath, string fontname, unsigned int fontsize, bool load, bool refresh) {
    myLabelWidgetClass.setFontPath(fontpath);
    myLabelWidgetClass.setFontName(fontname, lang);
    myLabelWidgetClass.setFontSize(fontsize);
    if (load) {
        this->load_font = true;
    	loadFont();
    }
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setFont(string fontpath, string fontname, unsigned int fontsize, bool load, bool refresh) {
	setFont(MMSLANG_NONE, fontpath, fontname, fontsize, load, refresh);
}

void MMSLabelWidget::setAlignment(MMSALIGNMENT alignment, bool refresh) {
    myLabelWidgetClass.setAlignment(alignment);
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setColor(MMSFBColor color, bool refresh) {
    myLabelWidgetClass.setColor(color);
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setSelColor(MMSFBColor selcolor, bool refresh) {
    myLabelWidgetClass.setSelColor(selcolor);
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setText(string text, bool refresh) {
    myLabelWidgetClass.setText(text);
    this->translated = false;
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::setSlidable(bool slidable) {
    myLabelWidgetClass.setSlidable(slidable);
    if (slidable) {
    	// text should slide
    	this->slide_offset = 0;

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
    	this->slide_offset = 0;
    	this->refresh();
    }
}

void MMSLabelWidget::setSlideSpeed(unsigned char slidespeed) {
    myLabelWidgetClass.setSlideSpeed(slidespeed);
    this->frame_delay = 100;
    this->frame_delay_set = false;
}

void MMSLabelWidget::setTranslate(bool translate, bool refresh) {
    myLabelWidgetClass.setTranslate(translate);
    this->translated = false;
    if (refresh)
        this->refresh();
}

void MMSLabelWidget::updateFromThemeClass(MMSLabelWidgetClass *themeClass) {
    if (themeClass->isFontPath())
        setFontPath(themeClass->getFontPath());
    for (unsigned int i = MMSLANG_NONE; i < MMSLANG_SIZE; i++) {
    	if (themeClass->isFontName((MMSLanguage)i))
    		setFontName(themeClass->getFontName((MMSLanguage)i), (MMSLanguage)i);
    }
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
    if (themeClass->isSlideSpeed())
        setSlideSpeed(themeClass->getSlideSpeed());
    if (themeClass->isTranslate())
        setTranslate(themeClass->getTranslate());

    MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
