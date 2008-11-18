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
#include "mmsgui/mmsinputwidgetthread.h"


MMSInputWidget::MMSInputWidget(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget() {
    create(root, className, theme);
}

MMSInputWidget::~MMSInputWidget() {
	if (this->iwt)
		delete this->iwt;
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

    // clear
    this->font = NULL;
    this->cursor_pos = 0;
	this->cursor_on = true;
	this->scroll_x = 0;

    // create thread
    this->iwt = new MMSInputWidgetThread(this);
	if (this->iwt)
		this->iwt->start();

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
        int width, height, x, y;
        int cursor_x=0, cursor_w = 4;

        /* draw my things */
        if (this->font) {
            MMSFBRectangle surfaceGeom = getSurfaceGeometry();

            this->surface->setFont(this->font);

            string text = getText();

            // get width and height of the string to be drawn
            this->font->getStringWidth(text, -1, &width);
            this->font->getHeight(&height);

            // calc cursor position
            if (text.size() < (unsigned int)this->cursor_pos)
            	cursor_x = width;
            else
            	this->font->getStringWidth(text.substr(0,this->cursor_pos), -1, &cursor_x);

            MMSALIGNMENT alignment = getAlignment();
            switch (alignment) {
                case MMSALIGNMENT_LEFT:
                case MMSALIGNMENT_TOP_LEFT:
                case MMSALIGNMENT_BOTTOM_LEFT:
                default:
                    if (cursor_x + cursor_w + scroll_x > surfaceGeom.w)
                    	// move text to the left
                    	scroll_x-= cursor_x + cursor_w + scroll_x - surfaceGeom.w;
                    else
                    if (scroll_x < 0) {
                    	// move text to the right
                    	scroll_x+= surfaceGeom.w - (cursor_x + cursor_w + scroll_x);
                    	if (scroll_x > 0)
                    		scroll_x = 0;
                    }
                    x = surfaceGeom.x + scroll_x;
                    cursor_x+=scroll_x;
                	switch (alignment) {
                		case MMSALIGNMENT_LEFT:
                		default:
                			y = ((surfaceGeom.h - height) / 2) + surfaceGeom.y;
                			break;
                        case MMSALIGNMENT_TOP_LEFT:
                            y = surfaceGeom.y;
                            break;
                        case MMSALIGNMENT_BOTTOM_LEFT:
                            y = surfaceGeom.y + surfaceGeom.h - height;
                            break;
                	}
                    break;
                case MMSALIGNMENT_RIGHT:
                case MMSALIGNMENT_TOP_RIGHT:
                case MMSALIGNMENT_BOTTOM_RIGHT:
                    if (cursor_x + scroll_x > surfaceGeom.w)
                    	// move text to the left
                    	scroll_x-= cursor_x + scroll_x - surfaceGeom.w;
                    else
                    if (scroll_x < 0) {
                    	// move text to the right
                    	scroll_x+= surfaceGeom.w - (cursor_x + cursor_w + scroll_x);
                    	if (scroll_x > 0)
                    		scroll_x = 0;
                    }
                    x = surfaceGeom.x + surfaceGeom.w - width - cursor_w;
                    cursor_x+=surfaceGeom.w - width - cursor_w;
                    if (cursor_x < 0) {
                    	x-=cursor_x;
                    	cursor_x = 0;
                    }
                	switch (alignment) {
                		case MMSALIGNMENT_RIGHT:
                			y = ((surfaceGeom.h - height) / 2) + surfaceGeom.y;
                			break;
                        case MMSALIGNMENT_TOP_RIGHT:
                            y = surfaceGeom.y;
                            break;
                        case MMSALIGNMENT_BOTTOM_RIGHT:
                            y = surfaceGeom.y + surfaceGeom.h - height;
                            break;
                        default:
                        	break;
                	}
                    break;
                case MMSALIGNMENT_CENTER:
                case MMSALIGNMENT_TOP_CENTER:
                case MMSALIGNMENT_BOTTOM_CENTER:
                    x = surfaceGeom.x + ((surfaceGeom.w - width) / 2) - cursor_w;
                    cursor_x+=((surfaceGeom.w - width) / 2) - cursor_w;
                    if (cursor_x < 0) {
                    	x-=cursor_x;
                    	cursor_x = 0;
                    }
                	switch (alignment) {
                		case MMSALIGNMENT_CENTER:
                			y = ((surfaceGeom.h - height) / 2) + surfaceGeom.y;
                			break;
                        case MMSALIGNMENT_TOP_CENTER:
                            y = surfaceGeom.y;
                            break;
                        case MMSALIGNMENT_BOTTOM_CENTER:
                            y = surfaceGeom.y + surfaceGeom.h - height;
                            break;
                        default:
                        	break;
                	}
                    break;
            }

            MMSFBColor color;

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

            // draw the cursor
        	if (this->cursor_on)
        		if (isFocused())
        			this->surface->drawRectangle(cursor_x, y, cursor_w, height);
        }

        /* update window surface with an area of surface */
        updateWindowSurfaceWithSurface(!*backgroundFilled);
    }

    /* unlock */
    this->surface->unlock();

    /* draw widgets debug frame */
    return MMSWidget::drawDebug();
}

void MMSInputWidget::drawCursor(bool cursor_on) {
	this->cursor_on = cursor_on;
	refresh();
}


void MMSInputWidget::setCursorPos(int cursor_pos, bool refresh) {
	if (cursor_pos < 0) {
		// out of range
		this->cursor_pos = 0;
		return;
	}

	string text = getText();
	if (text.size() >= (unsigned int)cursor_pos)
		// okay, set it
		this->cursor_pos = cursor_pos;
	else
		// out of range
		this->cursor_pos = text.size();

	if (refresh)
		this->refresh();
}

void MMSInputWidget::addTextAfterCursorPos(string text, bool refresh) {
	// something to add?
	if (text=="") return;
	int textlen = text.size();

	string oldtext = getText();
	if (oldtext.size() < (unsigned int)this->cursor_pos)
		this->cursor_pos = oldtext.size();

	// insert the text and change the cursor position
	this->cursor_pos+=textlen;
	setText(oldtext.substr(0,this->cursor_pos-textlen) + text + oldtext.substr(this->cursor_pos-textlen), refresh, false);
}

void MMSInputWidget::removeTextBeforeCursorPos(int textlen, bool refresh) {
	// something to remove?
	if (textlen<=0) return;
	if (this->cursor_pos<=0) return;

	string text = getText();
	if (text.size() < (unsigned int)this->cursor_pos)
		this->cursor_pos = text.size();

	if (this->cursor_pos <= textlen)
		textlen = this->cursor_pos;

	//remove a part of the text
	this->cursor_pos-=textlen;
	setText(text.substr(0,this->cursor_pos) + text.substr(this->cursor_pos+textlen), refresh, false);
}

void MMSInputWidget::handleInput(MMSInputEvent *inputevent) {
	bool processed = false;

	if (inputevent->type == MMSINPUTEVENTTYPE_KEYPRESS) {
		// keyboard inputs

		// save last inputevent
		last_inputevent = *inputevent;

		processed = true;
		switch (inputevent->key) {
			case MMSKEY_CURSOR_RIGHT:
				setCursorPos(this->cursor_pos+1);
		        break;
			case MMSKEY_CURSOR_LEFT:
				setCursorPos(this->cursor_pos-1);
				break;
			case MMSKEY_BACKSPACE:
				removeTextBeforeCursorPos(1);
				break;
			case MMSKEY_HOME:
		    	setCursorPos(0);
				break;
			case MMSKEY_END:
		    	setCursorPos(0xffff);
				break;
			case MMSKEY_SPACE:
				addTextAfterCursorPos(" ");
				break;
			case MMSKEY_PLUS_SIGN:
				addTextAfterCursorPos("+");
				break;
			case MMSKEY_MINUS_SIGN:
				addTextAfterCursorPos("-");
				break;
			case MMSKEY_PERIOD:
				addTextAfterCursorPos(".");
				break;
			case MMSKEY_SLASH:
				addTextAfterCursorPos("/");
				break;
			case MMSKEY_UNDERSCORE:
				addTextAfterCursorPos("_");
				break;
			case MMSKEY_0:
				addTextAfterCursorPos("0");
				break;
			case MMSKEY_1:
				addTextAfterCursorPos("1");
				break;
			case MMSKEY_2:
				addTextAfterCursorPos("2");
				break;
			case MMSKEY_3:
				addTextAfterCursorPos("3");
				break;
			case MMSKEY_4:
				addTextAfterCursorPos("4");
				break;
			case MMSKEY_5:
				addTextAfterCursorPos("5");
				break;
			case MMSKEY_6:
				addTextAfterCursorPos("6");
				break;
			case MMSKEY_7:
				addTextAfterCursorPos("7");
				break;
			case MMSKEY_8:
				addTextAfterCursorPos("8");
				break;
			case MMSKEY_9:
				addTextAfterCursorPos("9");
				break;
			case MMSKEY_CAPITAL_A:
				addTextAfterCursorPos("A");
				break;
			case MMSKEY_CAPITAL_B:
				addTextAfterCursorPos("B");
				break;
			case MMSKEY_CAPITAL_C:
				addTextAfterCursorPos("C");
				break;
			case MMSKEY_CAPITAL_D:
				addTextAfterCursorPos("D");
				break;
			case MMSKEY_CAPITAL_E:
				addTextAfterCursorPos("E");
				break;
			case MMSKEY_CAPITAL_F:
				addTextAfterCursorPos("F");
				break;
			case MMSKEY_CAPITAL_G:
				addTextAfterCursorPos("G");
				break;
			case MMSKEY_CAPITAL_H:
				addTextAfterCursorPos("H");
				break;
			case MMSKEY_CAPITAL_I:
				addTextAfterCursorPos("I");
				break;
			case MMSKEY_CAPITAL_J:
				addTextAfterCursorPos("J");
				break;
			case MMSKEY_CAPITAL_K:
				addTextAfterCursorPos("K");
				break;
			case MMSKEY_CAPITAL_L:
				addTextAfterCursorPos("L");
				break;
			case MMSKEY_CAPITAL_M:
				addTextAfterCursorPos("M");
				break;
			case MMSKEY_CAPITAL_N:
				addTextAfterCursorPos("N");
				break;
			case MMSKEY_CAPITAL_O:
				addTextAfterCursorPos("O");
				break;
			case MMSKEY_CAPITAL_P:
				addTextAfterCursorPos("P");
				break;
			case MMSKEY_CAPITAL_Q:
				addTextAfterCursorPos("Q");
				break;
			case MMSKEY_CAPITAL_R:
				addTextAfterCursorPos("R");
				break;
			case MMSKEY_CAPITAL_S:
				addTextAfterCursorPos("S");
				break;
			case MMSKEY_CAPITAL_T:
				addTextAfterCursorPos("T");
				break;
			case MMSKEY_CAPITAL_U:
				addTextAfterCursorPos("U");
				break;
			case MMSKEY_CAPITAL_V:
				addTextAfterCursorPos("V");
				break;
			case MMSKEY_CAPITAL_W:
				addTextAfterCursorPos("W");
				break;
			case MMSKEY_CAPITAL_X:
				addTextAfterCursorPos("X");
				break;
			case MMSKEY_CAPITAL_Y:
				addTextAfterCursorPos("Y");
				break;
			case MMSKEY_CAPITAL_Z:
				addTextAfterCursorPos("Z");
				break;
			case MMSKEY_SMALL_A:
				addTextAfterCursorPos("a");
				break;
			case MMSKEY_SMALL_B:
				addTextAfterCursorPos("b");
				break;
			case MMSKEY_SMALL_C:
				addTextAfterCursorPos("c");
				break;
			case MMSKEY_SMALL_D:
				addTextAfterCursorPos("d");
				break;
			case MMSKEY_SMALL_E:
				addTextAfterCursorPos("e");
				break;
			case MMSKEY_SMALL_F:
				addTextAfterCursorPos("f");
				break;
			case MMSKEY_SMALL_G:
				addTextAfterCursorPos("g");
				break;
			case MMSKEY_SMALL_H:
				addTextAfterCursorPos("h");
				break;
			case MMSKEY_SMALL_I:
				addTextAfterCursorPos("i");
				break;
			case MMSKEY_SMALL_J:
				addTextAfterCursorPos("j");
				break;
			case MMSKEY_SMALL_K:
				addTextAfterCursorPos("k");
				break;
			case MMSKEY_SMALL_L:
				addTextAfterCursorPos("l");
				break;
			case MMSKEY_SMALL_M:
				addTextAfterCursorPos("m");
				break;
			case MMSKEY_SMALL_N:
				addTextAfterCursorPos("n");
				break;
			case MMSKEY_SMALL_O:
				addTextAfterCursorPos("o");
				break;
			case MMSKEY_SMALL_P:
				addTextAfterCursorPos("p");
				break;
			case MMSKEY_SMALL_Q:
				addTextAfterCursorPos("q");
				break;
			case MMSKEY_SMALL_R:
				addTextAfterCursorPos("r");
				break;
			case MMSKEY_SMALL_S:
				addTextAfterCursorPos("s");
				break;
			case MMSKEY_SMALL_T:
				addTextAfterCursorPos("t");
				break;
			case MMSKEY_SMALL_U:
				addTextAfterCursorPos("u");
				break;
			case MMSKEY_SMALL_V:
				addTextAfterCursorPos("v");
				break;
			case MMSKEY_SMALL_W:
				addTextAfterCursorPos("w");
				break;
			case MMSKEY_SMALL_X:
				addTextAfterCursorPos("x");
				break;
			case MMSKEY_SMALL_Y:
				addTextAfterCursorPos("y");
				break;
			case MMSKEY_SMALL_Z:
				addTextAfterCursorPos("z");
				break;

			default:
				processed = false;
		}
	}

	// not processed, call base widget
	if (!processed)
		MMSWidget::handleInput(inputevent);
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

MMSFBColor MMSInputWidget::getColor() {
    GETINPUT(Color);
}

MMSFBColor MMSInputWidget::getSelColor() {
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

void MMSInputWidget::setColor(MMSFBColor color, bool refresh) {
    myInputWidgetClass.setColor(color);
    if (refresh)
        this->refresh();
}

void MMSInputWidget::setSelColor(MMSFBColor selcolor, bool refresh) {
    myInputWidgetClass.setSelColor(selcolor);
    if (refresh)
        this->refresh();
}

void MMSInputWidget::setText(string text, bool refresh, bool reset_cursor) {
    myInputWidgetClass.setText(text);

    if (reset_cursor)
    	// set cursor to the end
    	setCursorPos(0xffff, refresh);
    else
    	// try to leave cursor at its position
    	setCursorPos(this->cursor_pos, refresh);

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
