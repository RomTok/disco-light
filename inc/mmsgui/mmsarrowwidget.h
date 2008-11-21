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

#ifndef MMSARROWWIDGET_H_
#define MMSARROWWIDGET_H_

#include "mmsgui/mmswidget.h"

//! With this class you can draw an arrow.
/*!
The arrow here is nothing more than a triangle with a specified direction.
The arrow widget cannot be focused.
But if you click on it (e.g. mouse or touch screen), the arrow widget submits an input
event (MMSKEY_CURSOR_LEFT, MMSKEY_CURSOR_RIGHT, MMSKEY_CURSOR_UP, MMSKEY_CURSOR_DOWN)
to the toplevel parent window according to the direction parameter (see setDirection()).
\author Jens Schneider
*/
class MMSArrowWidget : public MMSWidget {
    private:
        string         		className;
        MMSArrowWidgetClass *arrowWidgetClass;
        MMSArrowWidgetClass myArrowWidgetClass;

        bool last_pressed;

        bool create(MMSWindow *root, string className, MMSTheme *theme);

        void handleInput(MMSInputEvent *inputevent);

    public:
        MMSArrowWidget(MMSWindow *root, string className, MMSTheme *theme = NULL);
        ~MMSArrowWidget();

        MMSWidget *copyWidget();

        bool init();
        bool draw(bool *backgroundFilled = NULL);

    public:
        /* theme access methods */
        MMSFBColor getColor();
        MMSFBColor getSelColor();
        MMSDIRECTION getDirection();

        void setColor(MMSFBColor color, bool refresh = true);
        void setSelColor(MMSFBColor selcolor, bool refresh = true);
        void setDirection(MMSDIRECTION direction, bool refresh = true);

        void updateFromThemeClass(MMSArrowWidgetClass *themeClass);
};

#endif /*MMSARROWWIDGET_H_*/
