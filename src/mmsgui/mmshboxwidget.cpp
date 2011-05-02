/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2011 BerLinux Solutions GmbH                       *
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

#include "mmsgui/mmshboxwidget.h"

MMSHBoxWidget::MMSHBoxWidget(MMSWindow *root) : MMSWidget::MMSWidget() {
    create(root);
}

bool MMSHBoxWidget::create(MMSWindow *root) {
	this->type = MMSWIDGETTYPE_HBOX;
    return MMSWidget::create(root, false, true, false, false, true, true, false);
}

MMSWidget *MMSHBoxWidget::copyWidget() {
    // create widget
    MMSHBoxWidget *newWidget = new MMSHBoxWidget(this->rootwindow);

    // copy widget
    *newWidget = *this;

    // copy base widget
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    return newWidget;
}

void MMSHBoxWidget::add(MMSWidget *widget) {
	widget->setParent(this);
    this->children.push_back(widget);
    if (this->getRootWindow())
        this->getRootWindow()->add(widget);
    this->recalculateChildren();
}

void MMSHBoxWidget::recalculateChildren() {

    // check something
    if(this->children.empty())
        return;

    if(this->geomset == false)
        return;

    MMSFBRectangle rect;
    int safechildren  = 0;
    int lastsafechild = -1;
    int safepix       = this->geom.w;

    // first pass: get size hints
    for(unsigned int i = 0; i < this->children.size(); i++) {
        string sizehint = children.at(i)->getSizeHint();

		if (sizehint == "") {
            // have no sizehint
			safechildren++;
            lastsafechild = i;
		}
		else {
			// calculate width based on sizehint
	        getPixelFromSizeHint(&rect.w, sizehint, this->geom.w, this->geom.h);
            safepix -= rect.w;
            if ((safepix < 0)||((int)rect.w < 0)) {
                throw MMSWidgetError(0,"cannot calculate geometry (not enough free pixels)");
            }
		}
    }

    // second pass: calculate geometry of all children
    int nextx = this->geom.x;
    int safe_w = (safechildren) ? safepix / safechildren : 0;
    for(unsigned int i = 0; i < this->children.size(); i++) {
        string sizehint = children.at(i)->getSizeHint();

		if (sizehint == "") {
            // calculate width based on remaining space
            rect.w = safe_w;
            if (i == lastsafechild)
                rect.w+= safepix % safechildren;
     	}
     	else {
            // calculate width based on sizehint
	        getPixelFromSizeHint(&rect.w, sizehint, this->geom.w, this->geom.h);
     	}

        // set geometry of child widget
        rect.x = nextx;
     	rect.y = this->geom.y;
        rect.h = this->geom.h;
        this->children.at(i)->setGeometry(rect);

        // next position
        nextx+= rect.w;
    }
}
