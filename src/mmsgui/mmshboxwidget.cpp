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

#include "mmsgui/mmshboxwidget.h"

MMSHBoxWidget::MMSHBoxWidget(MMSWindow *root) : MMSWidget::MMSWidget() {
    create(root);
}

bool MMSHBoxWidget::create(MMSWindow *root) {
	this->type = MMSWIDGETTYPE_HBOX;
    return MMSWidget::create(root, false, true, false, false, true, true);
}

MMSWidget *MMSHBoxWidget::copyWidget() {
    /* create widget */
    MMSHBoxWidget *newWidget = new MMSHBoxWidget(this->rootwindow);

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
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
    unsigned int nextx;
    unsigned int safechildren;
    unsigned int lastsafechild;
    unsigned int safepix;

    /* check something */
    if(this->children.empty())
        return;

    if(this->geomset == false)
        return;

    safepix = this->geom.w;
    safechildren = 0;

    /* first pass get and apply size hints */
    nextx = this->geom.x;
    for(unsigned int i = 0; i < this->children.size(); i++) {
        MMSFBRectangle rect;

        string sizehint = children.at(i)->getSizeHint();

		if (sizehint == "") {
            /* have no sizehint */
			children.at(i)->setGeomSet(false);
			safechildren++;
            lastsafechild = i;
		}
		else {
	        /* calculate max width */
	        getPixelFromSizeHint(&rect.w, sizehint, this->geom.w, this->geom.h);
            safepix -= rect.w;
            if ((safepix < 0)||((int)rect.w < 0))
                throw new MMSWidgetError(0,"cannot calculate geometry (not enough free pixels)");

            rect.h = this->geom.h;
	        rect.y = this->geom.y;
            rect.x = nextx;

            /* set childs geometry */
	        this->children.at(i)->setGeometry(rect);

            /* next position */
            nextx+= rect.w;
		}
    }

	/* every child has a geometry */
	if (safechildren == 0)
		return;
    unsigned int safe_w = safepix / safechildren;

    /* second pass calculate rest */
    nextx = this->geom.x;
    for(unsigned int i = 0; i < this->children.size(); i++) {
        MMSFBRectangle rect;

     	if(this->children.at(i)->isGeomSet()==false) {
            /* calculate complete geometry, max width */
            rect.w = safe_w;
            if (i == lastsafechild)
                rect.w+= safepix % safechildren;
            rect.h = this->geom.h;
	        rect.y = this->geom.y;
            rect.x = nextx;
     	}
     	else {
            /* adjust x position */
     		rect = children.at(i)->getGeometry();
            rect.x = nextx;
     	}

        /* set childs geometry */
        this->children.at(i)->setGeometry(rect);

        /* next position */
        nextx+= rect.w;
    }
}
