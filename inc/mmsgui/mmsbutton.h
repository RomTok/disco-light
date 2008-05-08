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

#ifndef MMSBUTTON_H_
#define MMSBUTTON_H_

#include "mmsgui/mmswidget.h"

//! With this class you can get a focusable button. 
/*!
In most cases the button widget works as a container for other widgets.
For example:
You want to have a focusable MMSArrow widget. But the arrow widget itself cannot be focused.
So you create a button widget and add the arrow widget to the button.
As result the arrow widget is the child widget of the button.
\author Jens Schneider
*/
class MMSButton : public MMSWidget {
    private:
        string          className;
        MMSButtonClass  *buttonClass;
        MMSButtonClass  myButtonClass;

        bool create(MMSWindow *root, string className, MMSTheme *theme);

    public:
        MMSButton(MMSWindow *root, string className, MMSTheme *theme = NULL);
        ~MMSButton();
        MMSWIDGETTYPE getType() { return MMSWIDGETTYPE_BUTTON; }

        MMSWidget *copyWidget();

        bool init();
        bool draw(bool *backgroundFilled = NULL);

    public:
        /* theme access methods */
        void updateFromThemeClass(MMSButtonClass *themeClass);
};

#endif /*MMSBUTTON_H_*/
