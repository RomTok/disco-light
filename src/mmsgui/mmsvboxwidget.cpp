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

#include "mmsgui/mmsvboxwidget.h"

MMSVBoxWidget::MMSVBoxWidget(MMSWindow *root) : MMSWidget::MMSWidget() {
    create(root);
}

bool MMSVBoxWidget::create(MMSWindow *root) {
	this->type = MMSWIDGETTYPE_VBOX;
    return MMSWidget::create(root, false, true, false, false, true, true, false);
}

MMSWidget *MMSVBoxWidget::copyWidget() {
    // create widget
    MMSVBoxWidget *newWidget = new MMSVBoxWidget(this->rootwindow);

    // copy widget
    *newWidget = *this;

    // copy base widget
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    return newWidget;
}

void MMSVBoxWidget::add(MMSWidget *widget) {
	widget->setParent(this);
    this->children.push_back(widget);
    if (this->getRootWindow())
        this->getRootWindow()->add(widget);
    this->recalculateChildren();
}

void MMSVBoxWidget::recalculateChildren() {

    // check something
    if(this->children.empty())
        return;

    if(this->geomset == false)
        return;

    MMSFBRectangle rect;
    int safechildren  = 0;
    int lastsafechild = -1;
    int safepix       = this->geom.h;

    // first pass: determine safepix which will divided between widgets with no content size and no size hint
    for(unsigned int i = 0; i < this->children.size(); i++) {
    	int content_width;
    	int content_height;
    	if (!children.at(i)->getContentSize(&content_width, &content_height)) {
    		// size of content not set, use sizehint
			string sizehint = children.at(i)->getSizeHint();

			if (sizehint == "") {
				// have no sizehint
				safechildren++;
				lastsafechild = i;
			}
			else {
				// calculate width based on sizehint
				getPixelFromSizeHint(&rect.h, sizehint, this->geom.h, this->geom.w);
				safepix -= rect.h;
				if ((safepix < 0)||((int)rect.h < 0)) {
					throw MMSWidgetError(0,"cannot calculate geometry (not enough free pixels)");
				}
			}
    	}
    	else {
    		// use content height
			safepix-= content_height;
			if (safepix < 0) {
				throw MMSWidgetError(0,"cannot calculate geometry (not enough free pixels)");
			}
    	}
    }

    // second pass: calculate geometry of all children
    int nexty = this->geom.y;
    int safe_h = (safechildren) ? safepix / safechildren : 0;
    for(unsigned int i = 0; i < this->children.size(); i++) {
    	int content_width;
    	int content_height;
    	if (!children.at(i)->getContentSize(&content_width, &content_height)) {
    		// size of content not set, use sizehint
			string sizehint = children.at(i)->getSizeHint();

			if (sizehint == "") {
				// calculate height based on remaining space
				rect.h = safe_h;
				if (i == lastsafechild)
					rect.h+= safepix % safechildren;
			}
			else {
				// calculate height based on sizehint
				getPixelFromSizeHint(&rect.h, sizehint, this->geom.h, this->geom.w);
			}
    	}
    	else {
    		// use content height
    		rect.h = content_height;
    	}

        // set geometry of child widget
        rect.y = nexty;
     	rect.x = this->geom.x;
        rect.w = this->geom.w;
        this->children.at(i)->setGeometry(rect);

        // next position
        nexty+= rect.h;
    }
}

void MMSVBoxWidget::setContentSizeFromChild() {
	if (!this->minmax_set) {
		return;
	}

	if (!this->parent)
		return;

	int mycw = 0;
	int mych = 0;

    for(unsigned int i = 0; i < this->children.size(); i++) {

    	int content_width;
    	int content_height;
    	if (!children.at(i)->getContentSize(&content_width, &content_height)) {
    		// size of content not set, use sizehint
			string sizehint = children.at(i)->getSizeHint();

			if (sizehint == "") {
				// have no sizehint
				content_height = 1;
			}
			else {
				// calculate height based on sizehint
				getPixelFromSizeHint(&content_height, sizehint, this->geom.h, this->geom.w);
			}
    	}
    	else {
    		if (mycw < content_width)
    			mycw = content_width;
    	}

    	mych+= content_height;
    }

    if (mycw > 0 && mych > 0) {
		this->content_width_child = mycw;
		this->content_height_child = mych;
		this->parent->setContentSizeFromChild();
    }
}

