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

#include "mmsgui/mmsvboxwidget.h"

MMSVBoxWidget::MMSVBoxWidget(MMSWindow *root) : MMSWidget::MMSWidget() {
    create(root);
}

bool MMSVBoxWidget::create(MMSWindow *root) {
	this->type = MMSWIDGETTYPE_VBOX;
    return MMSWidget::create(root, false, true, false, false, true, true, false);
}

MMSWidget *MMSVBoxWidget::copyWidget() {
    /* create widget */
    MMSVBoxWidget *newWidget = new MMSVBoxWidget(this->rootwindow);

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
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
    unsigned int nexty;
    unsigned int safechildren;
    unsigned int lastsafechild;
    unsigned int safepix;

    /* check something */
    if(this->children.empty())
        return;

    if(this->geomset == false)
        return;

	safepix = this->geom.h;
	safechildren = 0;

	/* first pass get and apply size hints */
    nexty = this->geom.y;
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
		    /* calculate max heigth */
		    getPixelFromSizeHint(&rect.h, sizehint, this->geom.h, this->geom.w);
		    safepix -= rect.h;
            if ((safepix < 0)||((int)rect.h < 0))
                throw new MMSWidgetError(0,"cannot calculate geometry (not enough free pixels), sizehint "+ sizehint);

		    rect.w = this->geom.w;
		    rect.x = this->geom.x;
            rect.y = nexty;

            /* set childs geometry */
		    this->children.at(i)->setGeometry(rect);

            /* next position */
            nexty+= rect.h;
		}
    }

	/* every child has a geometry */
	if (safechildren == 0)
		return;
    unsigned int safe_h = safepix / safechildren;

	/* second pass calculate rest */
    nexty = this->geom.y;
    for(unsigned int i = 0; i < this->children.size(); i++) {
        MMSFBRectangle rect;

		if(this->children.at(i)->isGeomSet()==false) {
			/* calculate complete geometry, max heigth */
	        rect.h = safe_h;
            if (i == lastsafechild)
                rect.h+= safepix % safechildren;
	        rect.w = this->geom.w;
	        rect.x = this->geom.x;
            rect.y = nexty;
		}
		else {
			/* adjust y position */
            rect = children.at(i)->getGeometry();
            rect.y = nexty;
		}

        /* set childs geometry */
        this->children.at(i)->setGeometry(rect);

        /* next position */
        nexty+= rect.h;
    }
}
