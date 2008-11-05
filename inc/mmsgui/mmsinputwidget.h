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

#ifndef MMSINPUTWIDGET_H_
#define MMSINPUTWIDGET_H_

#include "mmsgui/mmswidget.h"

//! With this class you can display and edit one line text.
/*!
You can display and edit one line of text.
\author Jens Schneider
*/
class MMSInputWidget : public MMSWidget {

    private:
        string         		className;
        MMSInputWidgetClass *inputWidgetClass;
        MMSInputWidgetClass myInputWidgetClass;

        MMSFBFont 		*font;
        int				cursor_pos;
        bool			cursor_on;
        int 			scroll_x;

        class MMSInputWidgetThread	*iwt;

        bool create(MMSWindow *root, string className, MMSTheme *theme);

        void handleInput(MMSInputEvent *inputevent);

        bool init();
        bool draw(bool *backgroundFilled = NULL);
        void drawCursor(bool cursor_on);

    public:
        MMSInputWidget(MMSWindow *root, string className, MMSTheme *theme = NULL);
        ~MMSInputWidget();

        MMSWidget *copyWidget();

        void setCursorPos(int cursor_pos, bool refresh = true);
        void addTextAfterCursorPos(string text, bool refresh = true);
        void removeTextBeforeCursorPos(int textlen, bool refresh = true);

    public:
    	/* theme access methods */
        string getFontPath();
        string getFontName();
        unsigned int getFontSize();
        MMSALIGNMENT getAlignment();
        DFBColor getColor();
        DFBColor getSelColor();
        string getText();

        void setFontPath(string fontpath, bool load = true, bool refresh = true);
        void setFontName(string fontname, bool load = true, bool refresh = true);
        void setFontSize(unsigned int  fontsize, bool load = true, bool refresh = true);
        void setFont(string fontpath, string fontname, unsigned int fontsize, bool load = true, bool refresh = true);
        void setAlignment(MMSALIGNMENT alignment, bool refresh = true);
        void setColor(DFBColor color, bool refresh = true);
        void setSelColor(DFBColor selcolor, bool refresh = true);
        void setText(string text, bool refresh = true, bool reset_cursor = true);

        void updateFromThemeClass(MMSInputWidgetClass *themeClass);

    friend class MMSInputWidgetThread;
};

#endif /*MMSINPUTWIDGET_H_*/
