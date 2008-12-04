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

#ifndef MMSTEXTBOXWIDGET_H_
#define MMSTEXTBOXWIDGET_H_

#include "mmsgui/mmswidget.h"


//! With this class you can display text with more than one line.
/*!
The textbox is focusable. So the user can scroll in it.
Line breaks will be done with the normal linefeed (\n (0x0a)).
If you want to display only one line of static text, you should use the MMSLabel widget.
\author Jens Schneider
*/
class MMSTextBoxWidget : public MMSWidget {
    private:
        typedef struct {
            MMSFBRectangle geom;
            string       word;
            unsigned int line;
            unsigned int paragraph;
        } TEXTBOX_WORDGEOM;

        string          		className;
        MMSTextBoxWidgetClass 	*textBoxWidgetClass;
        MMSTextBoxWidgetClass 	myTextBoxWidgetClass;

        MMSFBFont *font;

        vector<TEXTBOX_WORDGEOM *> wordgeom;

        string  lasttext;
        bool    surfaceChanged;

        bool create(MMSWindow *root, string className, MMSTheme *theme);

        void setSurfaceGeometry(unsigned int width = 0, unsigned int height = 0);

    public:
        MMSTextBoxWidget(MMSWindow *root, string className, MMSTheme *theme = NULL);
        ~MMSTextBoxWidget();

        MMSWidget *copyWidget();

        bool calcWordGeom(string text, unsigned int startWidth, unsigned int startHeight,
                          unsigned int *realWidth, unsigned int *realHeight,
                          unsigned int *scrollDX, unsigned int *scrollDY, unsigned int *lines, unsigned int *paragraphs,
                          bool wrap = true, bool splitwords = true, MMSALIGNMENT alignment = MMSALIGNMENT_CENTER);

        bool init();
        bool draw(bool *backgroundFilled = NULL);

    public:
        /* theme access methods */
        string getFontPath();
        string getFontName();
        unsigned int getFontSize();
        MMSALIGNMENT getAlignment();
        bool getWrap();
        bool getSplitWords();
        MMSFBColor getColor();
        MMSFBColor getSelColor();
        string getText();

        void setFontPath(string fontpath, bool load = true, bool refresh = true);
        void setFontName(string fontname, bool load = true, bool refresh = true);
        void setFontSize(unsigned int  fontsize, bool load = true, bool refresh = true);
        void setFont(string fontpath, string fontname, unsigned int fontsize, bool load = true, bool refresh = true);
        void setAlignment(MMSALIGNMENT alignment, bool refresh = true);
        void setWrap(bool wrap, bool refresh = true);
        void setSplitWords(bool splitwords, bool refresh = true);
        void setColor(MMSFBColor color, bool refresh = true);
        void setSelColor(MMSFBColor selcolor, bool refresh = true);
        void setText(string text, bool refresh = true);

        void updateFromThemeClass(MMSTextBoxWidgetClass *themeClass);
};

#endif /*MMSTEXTBOXWIDGET_H_*/
