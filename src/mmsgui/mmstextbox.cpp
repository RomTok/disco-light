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

#include "mmsgui/mmstextbox.h"

MMSTextBox::MMSTextBox(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget() {
    create(root, className, theme);
}

MMSTextBox::~MMSTextBox() {
    if (this->rootwindow) {
        this->rootwindow->fm->releaseFont(this->font);
    }
    for(vector<TEXTBOX_WORDGEOM *>::iterator it(wordgeom.begin()); it != wordgeom.end(); ++it)
        delete(*it);
}

bool MMSTextBox::create(MMSWindow *root, string className, MMSTheme *theme) {
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->textBoxClass = this->theme->getTextBoxClass(className);
    this->baseWidgetClass = &(this->theme->textBoxClass.widgetClass);
    if (this->textBoxClass) this->widgetClass = &(this->textBoxClass->widgetClass); else this->widgetClass = NULL;

    /* clear */
    this->font = NULL;
    this->lasttext = "";
    this->surfaceChanged = true;

    return MMSWidget::create(root, true, false, true, true, false, false);
}

MMSWidget *MMSTextBox::copyWidget() {
    /* create widget */
    MMSTextBox *newWidget = new MMSTextBox(this->rootwindow, className); 

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


void MMSTextBox::setSurfaceGeometry(unsigned int width, unsigned int height) {
    this->surfaceChanged = true;
   	MMSWidget::setSurfaceGeometry(width, height);
}

bool MMSTextBox::calcWordGeom(string text, unsigned int startWidth, unsigned int startHeight,
                              unsigned int *realWidth, unsigned int *realHeight,
                              unsigned int *scrollDX, unsigned int *scrollDY, unsigned int *lines, unsigned int *paragraphs,
                              bool wrap, bool splitwords, MMSALIGNMENT alignment) {
    int fontHeight, blankWidth;
    unsigned int x, y;

    /* init */
    *realWidth = startWidth;
    *realHeight = startHeight;
    *lines = 0;
    *paragraphs = 0;

    /* get font height */
    this->font->GetHeight(this->font, &fontHeight);
    *scrollDX = fontHeight;
    *scrollDY = fontHeight;

    /* has text or surface changed? */
    if ((text == this->lasttext)&&(!this->surfaceChanged)) return false;
    this->lasttext = text;
    this->surfaceChanged = false;

    /* clear wordgeom */
    for (int i = (int)(this->wordgeom.size())-1; i >= 0; i--) {
        delete this->wordgeom.at(i);
        this->wordgeom.erase(this->wordgeom.end()-1);
    }

    /* is text set? */
    if (text=="")
        /* no text, all is done */
        return true;

    /* get width of a blank character */
    this->font->GetStringWidth(this->font, " ", -1, &blankWidth);

    /* through the text and extract single words */
    do {
        int lfindex;
        int index;
        lfindex = (int)text.find("\n");
        index = (int)text.find(" ");
        if (lfindex < 0) {
            if (index < 0) index = text.size();
            if (index == 0) {
                text = text.substr(index + 1);
                continue;
            }
        }
        else {
            if ((index < 0)||(index > lfindex))
                index = lfindex;
            else
            if (index < lfindex) {
                if (index < 0) index = text.size();
                if (index == 0) {
                    text = text.substr(index + 1);
                    continue;
                }
                lfindex = -1;
            }
        }

        if (*lines == 0) {
            /* first word */
            *lines = 1;
            x = 0;
            y = 0;
        }

        /* new word */
        TEXTBOX_WORDGEOM *mywordgeom = new TEXTBOX_WORDGEOM;
        mywordgeom->geom.h=fontHeight;
        mywordgeom->word  =text.substr(0, index);


        this->font->GetStringWidth(this->font, mywordgeom->word.c_str(), -1, &mywordgeom->geom.w);

        if (x > 0)
            x += blankWidth;

        unsigned int endpos = x + mywordgeom->geom.w; 

        bool gotonext = true;

        if ((wrap)&&(splitwords)) {
            /* split words in wrap mode */
            if ((index > 1)&&(mywordgeom->geom.w > (int)*realWidth)) {
                /* recalculate index */
                while ((index > 1)&&(mywordgeom->geom.w > (int)*realWidth)) {
                    index--;
                    mywordgeom->word = text.substr(0, index);
                    this->font->GetStringWidth(this->font, mywordgeom->word.c_str(), -1, &mywordgeom->geom.w);
                    endpos = x + mywordgeom->geom.w;
                }

                text = text.substr(index);
                gotonext = false;
            }
        }

        if ((x==0)||(endpos <= *realWidth)||(wrap==false)) {
            if (endpos > *realWidth)
                if (wrap==false)
                    *realWidth = endpos;
                else {
                    mywordgeom->geom.w-= endpos - *realWidth;  
                    endpos = *realWidth;
                }

            mywordgeom->geom.x = x;
            mywordgeom->geom.y = y;

            x = endpos;
            
            mywordgeom->line = *lines;
            mywordgeom->paragraph = *paragraphs;
        }
        else
        {
            x = 0;
            y+= fontHeight;
            (*lines)++;

            mywordgeom->geom.x = x;
            mywordgeom->geom.y = y;

            x += mywordgeom->geom.w;

            mywordgeom->line = *lines;
            mywordgeom->paragraph = *paragraphs;
        }

        if ((lfindex >= 0)||(gotonext==false)) {
            x = 0;
            y+= fontHeight;
            (*lines)++;
            if (lfindex >= 0) (*paragraphs)++;
        }

        /* add to list */
        wordgeom.push_back(mywordgeom);

        if (gotonext) {
            if (index + 1 < (int)text.size())
                text = text.substr(index + 1);
            else 
                text = ""; 
        }
    } while (text != "");

    /* go through the list and calculate horizontal text alignment */
    unsigned int oldline = 1;
    unsigned int oldpos = 0;
    for (unsigned int i = 0; i < wordgeom.size(); i++) {
        if (wordgeom.at(i)->line != oldline) {
            if   ((alignment == MMSALIGNMENT_CENTER)||(alignment == MMSALIGNMENT_TOP_CENTER)||(alignment == MMSALIGNMENT_BOTTOM_CENTER)) {
                unsigned int diff = (*realWidth - wordgeom.at(i-1)->geom.x - wordgeom.at(i-1)->geom.w) / 2; 
                for (unsigned int j = oldpos; j < i; j++)
                    wordgeom.at(j)->geom.x += diff;
            }
            else
            if   ((alignment == MMSALIGNMENT_RIGHT)||(alignment == MMSALIGNMENT_TOP_RIGHT)
                ||(alignment == MMSALIGNMENT_BOTTOM_RIGHT)) {
                unsigned int diff = *realWidth - wordgeom.at(i-1)->geom.x - wordgeom.at(i-1)->geom.w; 
                for (unsigned int j = oldpos; j < i; j++)
                    wordgeom.at(j)->geom.x += diff;
            }
            else
            if  (((alignment == MMSALIGNMENT_JUSTIFY)||(alignment == MMSALIGNMENT_TOP_JUSTIFY)
                ||(alignment == MMSALIGNMENT_BOTTOM_JUSTIFY))&&(wordgeom.at(i)->paragraph == wordgeom.at(i-1)->paragraph)) {
                if (oldpos < i-1) {
                    unsigned int diff = (*realWidth - wordgeom.at(i-1)->geom.x - wordgeom.at(i-1)->geom.w) / (i-1-oldpos); 
                    for (unsigned int j = oldpos; j < i; j++) {
                        wordgeom.at(j)->geom.x += (j - oldpos) * diff;
                    }
                }
            }
            oldpos = i;
            oldline = wordgeom.at(i)->line;
        }
    }
    if   ((alignment == MMSALIGNMENT_CENTER)||(alignment == MMSALIGNMENT_TOP_CENTER)||(alignment == MMSALIGNMENT_BOTTOM_CENTER)) {
        unsigned int diff = (*realWidth - wordgeom.at(wordgeom.size()-1)->geom.x - wordgeom.at(wordgeom.size()-1)->geom.w) / 2; 
        for (unsigned int j = oldpos; j < wordgeom.size(); j++)
            wordgeom.at(j)->geom.x += diff;
    }
    else
    if   ((alignment == MMSALIGNMENT_RIGHT)||(alignment == MMSALIGNMENT_TOP_RIGHT)
        ||(alignment == MMSALIGNMENT_BOTTOM_RIGHT)) {
        unsigned int diff = *realWidth - wordgeom.at(wordgeom.size()-1)->geom.x - wordgeom.at(wordgeom.size()-1)->geom.w; 
        for (unsigned int j = oldpos; j < wordgeom.size(); j++)
            wordgeom.at(j)->geom.x += diff;
    }

    /* go through the list and calculate vertical text alignment */
    if (fontHeight * (*lines) > *realHeight)
        *realHeight = fontHeight * (*lines);
    else
    if (fontHeight * (*lines) < *realHeight) {
        if   ((alignment == MMSALIGNMENT_CENTER)||(alignment == MMSALIGNMENT_LEFT)
            ||(alignment == MMSALIGNMENT_RIGHT)||(alignment == MMSALIGNMENT_JUSTIFY)) {
            unsigned int diff = (*realHeight - fontHeight * (*lines)) / 2;
            if (diff > 0)
                for (unsigned int i = 0; i < wordgeom.size(); i++)
                    wordgeom.at(i)->geom.y += diff;
        }
        else
        if   ((alignment == MMSALIGNMENT_BOTTOM_CENTER)||(alignment == MMSALIGNMENT_BOTTOM_LEFT)
            ||(alignment == MMSALIGNMENT_BOTTOM_RIGHT)||(alignment == MMSALIGNMENT_BOTTOM_JUSTIFY)) {
            unsigned int diff = (*realHeight - fontHeight * (*lines));
            if (diff > 0)
                for (unsigned int i = 0; i < wordgeom.size(); i++)
                    wordgeom.at(i)->geom.y += diff;
        }
    }

    return true;
}


bool MMSTextBox::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

    /* load font */
    this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());

    return true;
}


bool MMSTextBox::draw(bool *backgroundFilled) {
    bool myBackgroundFilled = false;

    if (!this->initialized) {
        /* init widget (e.g. load images, fonts, ...) */
        init();
        this->initialized = true;
    }

    if (backgroundFilled) {
    	if (this->has_own_surface)
    		*backgroundFilled = false;
    }
    else
        backgroundFilled = &myBackgroundFilled;

    /* calculate text and surface size */
    if (this->font) {
        unsigned int realWidth, realHeight, scrollDX, scrollDY, lines, paragraphs;

        if (calcWordGeom(getText(), getInnerGeometry().w, getInnerGeometry().h, &realWidth, &realHeight, &scrollDX, &scrollDY,
                         &lines, &paragraphs, getWrap(), getSplitWords(), getAlignment())) {
            /* text has changed, reset something */
        	setScrollSize(scrollDX, scrollDY);
          	setSurfaceGeometry(realWidth, realHeight);
        }
    }    

    /* draw widget basics */
    if (MMSWidget::draw(backgroundFilled)) {

        /* lock */
        this->surface->lock();

        /* draw my things */
        if (this->font) {
        	DFBRectangle surfaceGeom = getSurfaceGeometry();

            DFBColor color;
            
            if (isSelected())
                color = getSelColor();
            else
                color = getColor();
            
            if (color.a) {
                /* set the font */
                this->surface->setFont(this->font);

                /* prepare for drawing */        
                this->surface->setDrawingColorAndFlagsByBrightnessAndOpacity(color, getBrightness(), getOpacity());

                /* draw single words into surface */
                for (unsigned int i = 0; i < this->wordgeom.size(); i++)
                {
					if (this->has_own_surface)
						this->surface->drawString(this->wordgeom.at(i)->word, -1,
						                          surfaceGeom.x + this->wordgeom.at(i)->geom.x,
						                          surfaceGeom.y + this->wordgeom.at(i)->geom.y);
					else
						this->surface->drawString(this->wordgeom.at(i)->word, -1,
						                          surfaceGeom.x + this->wordgeom.at(i)->geom.x - scrollPosX,
						                          surfaceGeom.y + this->wordgeom.at(i)->geom.y - scrollPosY);
                }
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

#define GETTEXTBOX(x) \
    if (this->myTextBoxClass.is##x()) return myTextBoxClass.get##x(); \
    else if ((textBoxClass)&&(textBoxClass->is##x())) return textBoxClass->get##x(); \
    else return this->theme->textBoxClass.get##x();

string MMSTextBox::getFontPath() {
    GETTEXTBOX(FontPath);
}

string MMSTextBox::getFontName() {
    GETTEXTBOX(FontName);
}

unsigned int MMSTextBox::getFontSize() {
    GETTEXTBOX(FontSize);
}

MMSALIGNMENT MMSTextBox::getAlignment() {
    GETTEXTBOX(Alignment);
}

bool MMSTextBox::getWrap() {
    GETTEXTBOX(Wrap);
}

bool MMSTextBox::getSplitWords() {
    GETTEXTBOX(SplitWords);
}

DFBColor MMSTextBox::getColor() {
    GETTEXTBOX(Color);
}

DFBColor MMSTextBox::getSelColor() {
    GETTEXTBOX(SelColor);
}

string MMSTextBox::getText() {
    GETTEXTBOX(Text);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSTextBox::setFontPath(string fontpath, bool load, bool refresh) {
    myTextBoxClass.setFontPath(fontpath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSTextBox::setFontName(string fontname, bool load, bool refresh) {
    myTextBoxClass.setFontName(fontname);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSTextBox::setFontSize(unsigned int fontsize, bool load, bool refresh) {
    myTextBoxClass.setFontSize(fontsize);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSTextBox::setFont(string fontpath, string fontname, unsigned int fontsize, bool load, bool refresh) {
    myTextBoxClass.setFontPath(fontpath);
    myTextBoxClass.setFontName(fontname);
    myTextBoxClass.setFontSize(fontsize);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->fm->releaseFont(this->font);
            this->font = this->rootwindow->fm->getFont(getFontPath(), getFontName(), getFontSize());
        }
    if (refresh)
        this->refresh();
}

void MMSTextBox::setAlignment(MMSALIGNMENT alignment, bool refresh) {
    myTextBoxClass.setAlignment(alignment);
    if (refresh)
        this->refresh();
}

void MMSTextBox::setWrap(bool wrap, bool refresh) {
    myTextBoxClass.setWrap(wrap);
    if (refresh)
        this->refresh();
}

void MMSTextBox::setSplitWords(bool splitwords, bool refresh) {
    myTextBoxClass.setSplitWords(splitwords);
    if (refresh)
        this->refresh();
}

void MMSTextBox::setColor(DFBColor color, bool refresh) {
    myTextBoxClass.setColor(color);
    if (refresh)
        this->refresh();
}

void MMSTextBox::setSelColor(DFBColor selcolor, bool refresh) {
    myTextBoxClass.setSelColor(selcolor);
    if (refresh)
        this->refresh();
}

void MMSTextBox::setText(string text, bool refresh) {
    myTextBoxClass.setText(text);
    this->scrollPosX=0;
    this->scrollPosY=0;
    if (refresh)
        this->refresh();
}

void MMSTextBox::updateFromThemeClass(MMSTextBoxClass *themeClass) {
    if (themeClass->isFontPath())
        setFontPath(themeClass->getFontPath());
    if (themeClass->isFontName())
        setFontName(themeClass->getFontName());
    if (themeClass->isFontSize())
        setFontSize(themeClass->getFontSize());
    if (themeClass->isAlignment())
        setAlignment(themeClass->getAlignment());
    if (themeClass->isWrap())
        setWrap(themeClass->getWrap());
    if (themeClass->isSplitWords())
        setSplitWords(themeClass->getSplitWords());
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
