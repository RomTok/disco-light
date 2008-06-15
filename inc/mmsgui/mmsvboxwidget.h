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

#ifndef MMSVBOXWIDGET_H_
#define MMSVBOXWIDGET_H_

#include "mmsgui/mmswidget.h"

//! With this class you can get a vertical box. 
/*!
The vbox widget is a container for other widgets.
You can add more than one child widget to a vbox. Widgets different from MMSHBoxWidget/MMSVBoxWidget can only have one child widget.
The child widgets will be vertical arranged.  
The vbox widget cannot be focused and displays nothing.
\author Jens Schneider
*/
class MMSVBoxWidget : public MMSWidget {
    private:
        void recalculateChildren();

        bool create(MMSWindow *root);

    public:
        MMSVBoxWidget(MMSWindow *root);

        MMSWidget *copyWidget();

        void add(MMSWidget *widget);
};

#endif /*MMSVBOXWIDGET_H_*/
