/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#ifndef MMSHBOXWIDGET_H_
#define MMSHBOXWIDGET_H_

#include "mmsgui/mmswidget.h"

//! With this class you can get a horizontal box.
/*!
The hbox widget is a container for other widgets.
You can add more than one child widget to a hbox. Widgets different from MMSHBoxWidget/MMSVBoxWidget can only have one child widget.
The child widgets will be horizontal arranged.
The hbox widget cannot be focused and displays nothing.
\author Jens Schneider
*/
class MMSHBoxWidget : public MMSWidget {
    private:
        void recalculateChildren();

        bool create(MMSWindow *root);

    public:
        MMSHBoxWidget(MMSWindow *root);

        MMSWidget *copyWidget();

        void add(MMSWidget *widget);
};

#endif /*MMSHBOXWIDGET_H_*/
