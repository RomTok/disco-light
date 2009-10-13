/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
 ***************************************************************************/

#ifndef MMSBUTTONWIDGET_H_
#define MMSBUTTONWIDGET_H_

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
class MMSButtonWidget : public MMSWidget {
    private:
        string          		className;
        MMSButtonWidgetClass  	*buttonWidgetClass;
        MMSButtonWidgetClass 	myButtonWidgetClass;

        bool create(MMSWindow *root, string className, MMSTheme *theme);

    public:
        MMSButtonWidget(MMSWindow *root, string className, MMSTheme *theme = NULL);
        ~MMSButtonWidget();

        MMSWidget *copyWidget();

        bool init();
        bool draw(bool *backgroundFilled = NULL);

    public:
        /* theme access methods */
        void updateFromThemeClass(MMSButtonWidgetClass *themeClass);
};

#endif /*MMSBUTTONWIDGET_H_*/
