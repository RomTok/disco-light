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

#include "mmsgui/mmswidget.h"
#include "mmsgui/mmsborder.h"
#include "mmsgui/mmsmenuwidget.h"
#include "mmsgui/mmssliderwidget.h"
#include <string.h>

MMSWidget::MMSWidget() {
    this->baseWidgetClass = NULL;
    this->widgetClass = NULL;
    this->initialized = false;
    this->bgimage = NULL;
    this->selbgimage = NULL;
    this->bgimage_p = NULL;
    this->selbgimage_p = NULL;
    this->bgimage_i = NULL;
    this->selbgimage_i = NULL;
    for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++)
        this->borderimages[i] = NULL;
    bordergeomset = false;
    for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++)
        this->borderselimages[i] = NULL;
    borderselgeomset = false;
    this->rootwindow = NULL;
    this->parent_rootwindow = NULL;
//    this->dfb = NULL;
    this->windowSurface = NULL;
    this->parent = NULL;
    this->focused = false;
    this->selected = false;
    this->activated = true;
    this->pressed = false;

    this->brightness = 255;
    this->opacity = 255;

    this->name = "";
    MMSIdFactory factory;
    this->id = factory.getId();

    this->toRedraw = false;
    this->redrawChildren = false;
    this->visible = true;


    this->surface = NULL;

    memset(&(this->geom), 0, sizeof(this->geom));
    memset(&(this->innerGeom), 0, sizeof(this->innerGeom));
    memset(&(this->surfaceGeom), 0, sizeof(this->surfaceGeom));

    this->scrollPosX = 0;
    this->scrollPosY = 0;
    this->scrollDX = 8;
    this->scrollDY = 8;

    /* initialize the callbacks */
    onSelect = new sigc::signal<void, MMSWidget*>;
    onFocus  = new sigc::signal<void, MMSWidget*, bool>;
    onReturn = new sigc::signal<void, MMSWidget*>;

//TODO: textbox widget should have its one surface
this->has_own_surface = false;
}

MMSWidget::~MMSWidget() {

	// delete the callbacks
    if (onSelect) delete onSelect;
    if (onFocus)  delete onFocus;
    if (onReturn) delete onReturn;

    // delete images
    if (this->rootwindow) {
        this->rootwindow->im->releaseImage(this->bgimage);
        this->rootwindow->im->releaseImage(this->selbgimage);
        this->rootwindow->im->releaseImage(this->bgimage_p);
        this->rootwindow->im->releaseImage(this->selbgimage_p);
        this->rootwindow->im->releaseImage(this->bgimage_i);
        this->rootwindow->im->releaseImage(this->selbgimage_i);
        for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++)
            this->rootwindow->im->releaseImage(this->borderimages[i]);
        for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++)
            this->rootwindow->im->releaseImage(this->borderselimages[i]);
    }

    // delete children
    for (unsigned int i = 0; i < children.size(); i++)
        delete children.at(i);

    // remove me from root window list
    if (this->rootwindow)
        this->rootwindow->remove(this);

    if(this->surface)
        delete this->surface;
}

MMSWIDGETTYPE MMSWidget::getType() {
	return this->type;
}

bool MMSWidget::create(MMSWindow *root, bool drawable, bool needsparentdraw, bool focusable, bool selectable,
                       bool canhavechildren, bool canselectchildren) {
    bool		b;

//    logger.writeLog("Create MMSWidget");

    this->drawable = drawable;
    this->needsparentdraw = needsparentdraw;
    this->focusable_initial = focusable;
    if (!this->focusable_initial)
        if (getFocusable(b))
			if (b)
				setFocusable(false, false);
    this->selectable_initial = selectable;
    if (!this->selectable_initial)
        if (getSelectable(b))
			if (b)
				setSelectable(false, false);
    this->canhavechildren = canhavechildren;
    this->canselectchildren = canselectchildren;

    setRootWindow(root);
    if (this->rootwindow) {
//        this->dfb = this->rootwindow->getDFB();
        this->windowSurface = this->rootwindow->getSurface();
    }
    this->sizehint.clear();
    this->geomset=false;

    this->upArrowWidget    = NULL;
    this->downArrowWidget  = NULL;
    this->leftArrowWidget  = NULL;
    this->rightArrowWidget = NULL;

    this->initialArrowsDrawn = false;

    this->navigateUpWidget    = NULL;
    this->navigateDownWidget  = NULL;
    this->navigateLeftWidget  = NULL;
    this->navigateRightWidget = NULL;

    this->vSliderWidget = NULL;
    this->hSliderWidget = NULL;

//    logger.writeLog("MMSWidget created");

    return true;
}

void MMSWidget::copyWidget(MMSWidget *newWidget) {
    /* get new id */
    MMSIdFactory factory;
    newWidget->id = factory.getId();

    /* copy my children */
    for (unsigned int i = 0; i < children.size(); i++)
        newWidget->children.at(i) = children.at(i)->copyWidget();

    /* initialize the callbacks */
    onSelect = new sigc::signal<void, MMSWidget*>;
    onFocus  = new sigc::signal<void, MMSWidget*, bool>;
    onReturn = new sigc::signal<void, MMSWidget*>;

    /* reload my images */
    newWidget->bgimage = NULL;
    newWidget->selbgimage = NULL;
    newWidget->bgimage_p = NULL;
    newWidget->selbgimage_p = NULL;
    newWidget->bgimage_i = NULL;
    newWidget->selbgimage_i = NULL;
    for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++)
        newWidget->borderimages[i] = NULL;
    for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++)
        newWidget->borderselimages[i] = NULL;

    if (drawable) {
        if (this->rootwindow) {
            string path, name;

            if (!newWidget->getBgImagePath(path)) path = "";
            if (!newWidget->getBgImageName(name)) name = "";
            newWidget->bgimage = this->rootwindow->im->getImage(path, name);

            if (!newWidget->getSelBgImagePath(path)) path = "";
            if (!newWidget->getSelBgImageName(name)) name = "";
            newWidget->selbgimage = this->rootwindow->im->getImage(path, name);

            if (!newWidget->getBgImagePath_p(path)) path = "";
            if (!newWidget->getBgImageName_p(name)) name = "";
            newWidget->bgimage_p = this->rootwindow->im->getImage(path, name);

            if (!newWidget->getSelBgImagePath_p(path)) path = "";
            if (!newWidget->getSelBgImageName_p(name)) name = "";
            newWidget->selbgimage_p = this->rootwindow->im->getImage(path, name);

            if (!newWidget->getBgImagePath_i(path)) path = "";
            if (!newWidget->getBgImageName_i(name)) name = "";
            newWidget->bgimage_i = this->rootwindow->im->getImage(path, name);

            if (!newWidget->getSelBgImagePath_i(path)) path = "";
            if (!newWidget->getSelBgImageName_i(name)) name = "";
            newWidget->selbgimage_i = this->rootwindow->im->getImage(path, name);

            if (!newWidget->getBorderImagePath(path)) path = "";
            for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++) {
	            if (!newWidget->getBorderImageNames((MMSBORDER_IMAGE_NUM)i, name)) name = "";
	            newWidget->borderimages[i] = this->rootwindow->im->getImage(path, name);
            }

            if (!newWidget->getBorderSelImagePath(path)) path = "";
            for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++) {
	            if (!newWidget->getBorderSelImageNames((MMSBORDER_IMAGE_NUM)i, name)) name = "";
	            newWidget->borderselimages[i] = this->rootwindow->im->getImage(path, name);
            }
        }
    }
}

MMSWidget* MMSWidget::getChild(unsigned int atpos) {
    if (atpos < children.size())
        return children.at(atpos);
    else
        return NULL;
}

MMSWidget* MMSWidget::disconnectChild(unsigned int atpos) {
    if (atpos < children.size()) {
        MMSWidget *widget = children.at(atpos);
        children.erase(children.begin()+atpos);
        return  widget;
    }
    else
        return NULL;
}

MMSWidget* MMSWidget::searchForWidget(string name) {
    MMSWidget *widget;

    if (name=="")
        return NULL;

    /* first, my own children */
    for (unsigned int i = 0; i < children.size(); i++)
        if (children.at(i)->getName() == name)
            return children.at(i);

    /* second, call search method of my children */
    for (unsigned int i = 0; i < children.size(); i++)
        if ((widget = children.at(i)->searchForWidget(name)))
            return widget;

    return NULL;
}

MMSWidget* MMSWidget::searchForWidgetType(MMSWIDGETTYPE type) {
    MMSWidget *widget;

    /* first, my own children */
    for (unsigned int i = 0; i < children.size(); i++)
        if (children.at(i)->getType() == type)
            return children.at(i);

    /* second, call search method of my children */
    for (unsigned int i = 0; i < children.size(); i++)
        if ((widget = children.at(i)->searchForWidgetType(type)))
            return widget;

    return NULL;
}

MMSWidget* MMSWidget::operator[](string name) {
    MMSWidget *widget;

    if ((widget = searchForWidget(name)))
        return widget;
    throw new MMSWidgetError(1, "widget " + name + " not found");
}



void MMSWidget::setSurfaceGeometry(unsigned int width, unsigned int height) {
    DFBRectangle mygeom;

    if (!drawable)
        return;

    mygeom.x = 0;
    mygeom.y = 0;
    if ((int)width > this->innerGeom.w)
        mygeom.w = width;
    else
        mygeom.w = this->innerGeom.w;
    if ((int)height > this->innerGeom.h)
        mygeom.h = height;
    else
        mygeom.h = this->innerGeom.h;

    if ((this->surfaceGeom.w != mygeom.w)||(this->surfaceGeom.h != mygeom.h)) {

        /* surface geom has changed */
        this->surfaceGeom = mygeom;

        /* create or change my surface */
        if (this->surface) {
            delete this->surface;
            this->surface = NULL;
        }

        if (this->has_own_surface) {
        	/* has own surface, create it */
        	this->windowSurface->createCopy(&(this->surface), this->surfaceGeom.w, this->surfaceGeom.h);
        }
        else {
        	/* get sub surface */
        	this->surface = this->windowSurface->getSubSurface(&(this->innerGeom));
        }
    }
    else {
        if (!this->has_own_surface)
        	if (this->surface) {
    	    	/* position of sub surface has changed */
    		    this->surfaceGeom = mygeom;

    		    /* move the sub surface */
    		    this->surface->moveTo(this->innerGeom.x, this->innerGeom.y);
        	}
    }
}

DFBRectangle MMSWidget::getSurfaceGeometry() {
    return this->surfaceGeom;
}

void MMSWidget::setInnerGeometry() {
    DFBRectangle mygeom;
    unsigned int diff = 0;

    /* check something */
    if (!isGeomSet())
        return;

    /* calculate geometry */
    mygeom = this->geom;


    if (drawable) {
    	unsigned int margin, borderthickness, bordermargin;
    	if (!getMargin(margin))
    		margin = 0;
    	if (!getBorderThickness(borderthickness))
    		borderthickness = 0;
    	if (!getBorderMargin(bordermargin))
    		bordermargin = 0;
        diff = margin + borderthickness + bordermargin;
    }

    if ((int)(2*diff) >= mygeom.w) {
        diff = mygeom.w / 2 - 1;
    }

    if ((int)(2*diff) >= mygeom.h) {
        diff = mygeom.h / 2 - 1;
    }

    mygeom.x+= diff;
    mygeom.y+= diff;
    mygeom.w-= 2*diff;
    mygeom.h-= 2*diff;

    if (memcmp(&(this->innerGeom), &mygeom, sizeof(mygeom))) {
        /* inner geom has changed */
        this->innerGeom = mygeom;

        /* set surface geometry */
        setSurfaceGeometry();
    }
}

DFBRectangle MMSWidget::getInnerGeometry() {
    return this->innerGeom;
}


void MMSWidget::setGeometry(DFBRectangle geom) {
    DFBRectangle oldgeom;
    bool dimChanged = true;


    if (this->geomset) {
        /* dimension has changed? */
        dimChanged = ((this->geom.w!=geom.w)||(this->geom.h!=geom.h));

        /* recalculate widgets border */
        if (dimChanged) {
            /* the dimension has changed */
            bordergeomset=false;
            borderselgeomset=false;
        }
        else {
            if (this->geom.x!=geom.x) {
                /* x pos has changed */
                if ((bordergeomset)||(borderselgeomset)) {
                    int diff = this->geom.x - geom.x;
                    for (int i=0;i<8;i++) {
                        bordergeom[i].x -= diff;
                        borderselgeom[i].x -= diff;
                    }
                }
            }
            if (this->geom.y!=geom.y) {
                /* y pos has changed */
                if ((bordergeomset)||(borderselgeomset)) {
                    int diff = this->geom.y - geom.y;
                    for (int i=0;i<8;i++) {
                        bordergeom[i].y -= diff;
                        borderselgeom[i].y -= diff;
                    }
                }
            }
        }
    }

    /* set new geom */
    this->geomset = true;
    oldgeom = this->geom;
    this->geom = geom;



    if (this->has_own_surface) {
	    if (dimChanged) {
	        /* calculate complete inner geometry */
	        setInnerGeometry();
	    }
	    else {
	        /* change only x and y values for inner geometry */
	        this->innerGeom.x+= this->geom.x - oldgeom.x;
	        this->innerGeom.y+= this->geom.y - oldgeom.y;
	    }
    }
	else {
        /* have only a subsurface, re-calculate inner geometry (e.g. move) */
        setInnerGeometry();
	}

    /* calculate my children */
    this->recalculateChildren();

}

DFBRectangle MMSWidget::getGeometry() {
    return this->geom;
}

DFBRectangle MMSWidget::getRealGeometry() {
	DFBRectangle r1,r2;

	/* have a parent widget? */
	if (!this->parent) {
		/* no, go to my window? */
		if (!this->rootwindow)
		    return this->geom;

		/* yes */
		if (!isGeomSet()) {
			this->rootwindow->recalculateChildren();
		}
		r1 = this->geom;
		r2 = this->rootwindow->getRealGeometry();
		r1.x+=r2.x;
		r1.y+=r2.y;
		return r1;
	}

	/* yes */
	r1 = this->geom;
	r2 = this->parent->getRealGeometry();
	r1.x+=r2.x;
	r1.y+=r2.y;
	return r1;
}



void MMSWidget::loadArrowWidgets() {
	bool  	b;
	string 	s;

	/* connect arrow widgets */
    if (!this->upArrowWidget)
    	if (getUpArrow(s))
    		if (s != "")
		        if (this->rootwindow)
		            if ((this->upArrowWidget = this->rootwindow->searchForWidget(s)))
		                if (!this->upArrowWidget->getSelectable(b))
		                    this->upArrowWidget = NULL;
		                else
			                if (!b)
			                    this->upArrowWidget = NULL;

    if (!this->downArrowWidget)
    	if (getDownArrow(s))
    		if (s != "")
		        if (this->rootwindow)
		            if ((this->downArrowWidget = this->rootwindow->searchForWidget(s)))
		                if (!this->downArrowWidget->getSelectable(b))
		                    this->downArrowWidget = NULL;
		                else
			                if (!b)
			                    this->downArrowWidget = NULL;

    if (!this->leftArrowWidget)
    	if (getLeftArrow(s))
    		if (s != "")
		        if (this->rootwindow)
		            if ((this->leftArrowWidget = this->rootwindow->searchForWidget(s)))
		                if (!this->leftArrowWidget->getSelectable(b))
		                    this->leftArrowWidget = NULL;
		                else
			                if (!b)
			                    this->leftArrowWidget = NULL;

    if (!this->rightArrowWidget)
    	if (getRightArrow(s))
    		if (s != "")
		        if (this->rootwindow)
		            if ((this->rightArrowWidget = this->rootwindow->searchForWidget(s)))
		                if (!this->rightArrowWidget->getSelectable(b))
		                    this->rightArrowWidget = NULL;
		                else
			                if (!b)
			                    this->rightArrowWidget = NULL;
}

void MMSWidget::switchArrowWidgets() {
    /* connect arrow widgets */
    loadArrowWidgets();

    /* switch arrow widgets */
    if (this->upArrowWidget)
        if (this->scrollPosY == 0)
            this->upArrowWidget->setSelected(false);
        else
            this->upArrowWidget->setSelected(true);

    if (this->downArrowWidget)
        if (this->surfaceGeom.h - this->surfaceGeom.y - (int)this->scrollPosY > this->innerGeom.h)
            this->downArrowWidget->setSelected(true);
        else
            this->downArrowWidget->setSelected(false);

    if (this->leftArrowWidget)
        if (this->scrollPosX == 0)
            this->leftArrowWidget->setSelected(false);
        else
            this->leftArrowWidget->setSelected(true);

    if (this->rightArrowWidget)
        if (this->surfaceGeom.w - this->surfaceGeom.x - (int)this->scrollPosX > this->innerGeom.w)
            this->rightArrowWidget->setSelected(true);
        else
            this->rightArrowWidget->setSelected(false);
}

bool MMSWidget::setScrollSize(unsigned int dX, unsigned int dY) {
    this->scrollDX = dX;
    this->scrollDY = dY;

    return true;
}

bool MMSWidget::setScrollPos(int posX, int posY, bool refresh, bool test) {
    if (!isGeomSet()) {
        /* i must have my geometry */
        MMSWindow *root = getRootWindow();
        if (root) {
            root->recalculateChildren();
        }
        else
            return false;
    }

    if (!this->surface) return false;

    if (posX < 0)
        if (this->scrollPosX > 0)
            posX = 0;
        else
            return false;

    if (posX + innerGeom.w > surfaceGeom.w)
        if ((int)this->scrollPosX + innerGeom.w < surfaceGeom.w)
            posX = surfaceGeom.w - innerGeom.w;
        else
            return false;

    if (posY < 0)
        if (this->scrollPosY > 0)
            posY = 0;
        else
            return false;

    if (posY + innerGeom.h > surfaceGeom.h)
        if ((int)this->scrollPosY + innerGeom.h < surfaceGeom.h)
            posY = surfaceGeom.h - innerGeom.h;
        else
            return false;

    if (!test) {
        this->scrollPosX = posX;
        this->scrollPosY = posY;

        if (refresh)
            this->refresh();
    }

    return true;
}


bool MMSWidget::scrollDown(unsigned int count, bool refresh, bool test, bool leave_selection) {
    if (setScrollPos((int)this->scrollPosX, (int)this->scrollPosY + count*(int)this->scrollDY, refresh, test)) {
        if (!test)
            switchArrowWidgets();
        return true;
    }
    return false;
}

bool MMSWidget::scrollUp(unsigned int count, bool refresh, bool test, bool leave_selection) {
    if (setScrollPos((int)this->scrollPosX, (int)this->scrollPosY - count*(int)this->scrollDY, refresh, test)) {
        if (!test)
            switchArrowWidgets();
        return true;
    }
    return false;
}

bool MMSWidget::scrollRight(unsigned int count, bool refresh, bool test, bool leave_selection) {
    if (setScrollPos((int)this->scrollPosX + count*(int)this->scrollDX, (int)this->scrollPosY, refresh, test)) {
        if (!test)
            switchArrowWidgets();
        return true;
    }
    return false;
}

bool MMSWidget::scrollLeft(unsigned int count, bool refresh, bool test, bool leave_selection) {
    if (setScrollPos((int)this->scrollPosX - count*(int)this->scrollDX, (int)this->scrollPosY, refresh, test)) {
        if (!test)
            switchArrowWidgets();
        return true;
    }
    return false;
}

bool MMSWidget::scrollTo(int posx, int posy, bool refresh) {
	return false;
}

DFBRectangle MMSWidget::getVisibleSurfaceArea() {
    DFBRectangle area;

    area.x = surfaceGeom.x + scrollPosX;
    area.y = surfaceGeom.y + scrollPosY;
    area.w = innerGeom.w;
    area.h = innerGeom.h;

    return area;
}

void MMSWidget::updateWindowSurfaceWithSurface(bool useAlphaChannel) {

	if (this->has_own_surface) {
		/* have own surface */
		DFBRectangle area = getVisibleSurfaceArea();

	    /* lock */
	    this->windowSurface->lock();

	    this->windowSurface->setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_NOFX));
	    this->windowSurface->blit(this->surface, &area, innerGeom.x, innerGeom.y);

	    /* unlock */
	    this->windowSurface->unlock();
	}
}

bool MMSWidget::init() {
    if (drawable) {
        if (!this->rootwindow)
            return false;

        string path, name;

        if (!getBgImagePath(path)) path = "";
        if (!getBgImageName(name)) name = "";
        this->bgimage = this->rootwindow->im->getImage(path, name);

        if (!getSelBgImagePath(path)) path = "";
        if (!getSelBgImageName(name)) name = "";
        this->selbgimage = this->rootwindow->im->getImage(path, name);

        if (!getBgImagePath_p(path)) path = "";
        if (!getBgImageName_p(name)) name = "";
        this->bgimage_p = this->rootwindow->im->getImage(path, name);

        if (!getSelBgImagePath_p(path)) path = "";
        if (!getSelBgImageName_p(name)) name = "";
        this->selbgimage_p = this->rootwindow->im->getImage(path, name);

        if (!getBgImagePath_i(path)) path = "";
        if (!getBgImageName_i(name)) name = "";
        this->bgimage_i = this->rootwindow->im->getImage(path, name);

        if (!getSelBgImagePath_i(path)) path = "";
        if (!getSelBgImageName_i(name)) name = "";
        this->selbgimage_i = this->rootwindow->im->getImage(path, name);

        if (!getBorderImagePath(path)) path = "";
        for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++) {
            if (!getBorderImageNames((MMSBORDER_IMAGE_NUM)i, name)) name = "";
            this->borderimages[i] = this->rootwindow->im->getImage(path, name);
        }

        if (!getBorderSelImagePath(path)) path = "";
        for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++) {
            if (!getBorderSelImageNames((MMSBORDER_IMAGE_NUM)i, name)) name = "";
            this->borderselimages[i] = this->rootwindow->im->getImage(path, name);
        }

        /* get my four widgets to which I have to navigate */
        if (!getNavigateUp(name)) name = "";
        this->navigateUpWidget = this->rootwindow->searchForWidget(name);

        if (!getNavigateDown(name)) name = "";
        this->navigateDownWidget = this->rootwindow->searchForWidget(name);

        if (!getNavigateLeft(name)) name = "";
        this->navigateLeftWidget = this->rootwindow->searchForWidget(name);

        if (!getNavigateRight(name)) name = "";
        this->navigateRightWidget = this->rootwindow->searchForWidget(name);

        /* get my two widgets which represents the sliders */
        if (!getVSlider(name)) name = "";
        this->vSliderWidget = this->rootwindow->searchForWidget(name);

        if (!getHSlider(name)) name = "";
        this->hSliderWidget = this->rootwindow->searchForWidget(name);
    }
    return true;
}


#ifdef OLDDRAW
bool MMSWidget(bool *backgroundFilled) {
    bool         myBackgroundFilled = false;
    bool         retry = false;

    if (!this->initialized) {
        /* init widget (e.g. load images, fonts, ...) */
        init();
        this->initialized = true;
    }

    if (backgroundFilled)
        *backgroundFilled = false;
    else
        backgroundFilled = &myBackgroundFilled;

    if ((!this->geomset)||(!this->visible))
        return false;

#ifdef __PUPTRACE__
    DEBUGOUT("draw widget = %s ***************************\n", getName().c_str());
#endif

    /* lock */
    if (this->surface) this->surface->lock();
    this->windowSurface->lock();
/*
if (!this->has_own_surface) {
//	if (this->surface) this->surface->clear(0xff,0,0,0xff);
	*backgroundFilled = true;
	if (this->surface) this->surface->unlock();
	this->windowSurface->unlock();
	return true;
}
*/
    /* draw background */
    do {
        /* searching for the background color or image */
        MMSFBColor col;
        MMSFBSurface *suf = NULL;
        col.a = 0;

        if (this->drawable) {
            if (isActivated()) {
                if (isSelected()) {
                    getSelBgColor(col);
                    suf = this->selbgimage;
                }
                else {
                    getBgColor(col);
                    suf = this->bgimage;
                }
            }
            else {
                if (isSelected()) {
                    getSelBgColor_i(col);
                    suf = this->selbgimage_i;
                }
                else {
                    getBgColor_i(col);
                    suf = this->bgimage_i;
                }
            }
        }

        if (suf) {
            if ((*backgroundFilled)||(retry)||(!this->has_own_surface)) {
                /* prepare for blitting */
                this->surface->setBlittingFlagsByBrightnessAlphaAndOpacity(brightness, (col.a)?col.a:255, opacity);

                /* fill background */
                surface->stretchBlit(suf, NULL, &surfaceGeom);
                *backgroundFilled = true;

                /* go out of the loop */
                break;
            }
            else
                /* the color has an alpha value and I need a background before drawing the image */
                retry = true;
        }
        else
        if (col.a) {
            if ((*backgroundFilled)||(retry)||(!this->has_own_surface)||((col.a==255)&&(opacity==255))) {
                /* prepare for drawing */
                this->surface->setDrawingColorAndFlagsByBrightnessAndOpacity(col, brightness, opacity);

                /* fill background */
                this->surface->fillRectangle(surfaceGeom.x, surfaceGeom.y, surfaceGeom.w, surfaceGeom.h);
                *backgroundFilled = true;

                /* go out of the loop */
                break;
            }
            else
                /* the color has an alpha value and I need a background for it */
                retry = true;
        }
        else {
            if ((*backgroundFilled)||(!this->has_own_surface))
                /* go out of the loop */
                break;
            else
                /* no color, no image, I need to search for a background */
                retry = true;
        }

        /* if not filled then */
        if (!*backgroundFilled) {
            /* searching for the next parent widget */
            MMSWidget *widget = NULL;
            vector<MMSWidget*> wlist;
            if (this->parent)
                widget = this->parent->getDrawableParent(false, false, false, &wlist);

            /* the widget found can be to far away from this widget */
            /* if wlist is filled, i can search for a better parent which has already a own surface */
            for (int i=0; i < wlist.size(); i++) {
                MMSWidget *w = wlist.at(i);
                if ((w->drawable)&&(w->geomset)&&(w->visible)) {
                    widget = w;
                    break;
                }
            }

            /* clear it (complete transparent) */
            if (this->drawable) {
                /* my own surface */
                this->surface->clear();
            }
            else {
                /* working direct on the window surface */
                DFBRegion clip;
                clip.x1 = innerGeom.x;
                clip.y1 = innerGeom.y;
                clip.x2 = innerGeom.x + innerGeom.w - 1;
                clip.y2 = innerGeom.y + innerGeom.h - 1;

#ifdef MMSGUI_STDOUT_TRACE
                DEBUGOUT("-----%u: widget setclip (%x)\n", pthread_self(), this->windowSurface);
#endif

                this->windowSurface->setClip(&clip);
                this->windowSurface->clear();
            }

            if (widget) {
                /* drawable parent found, calculate rectangle to copy */
                DFBRectangle srcrect = widget->getVisibleSurfaceArea();
                srcrect.x+= this->innerGeom.x - widget->innerGeom.x;
                srcrect.y+= this->innerGeom.y - widget->innerGeom.y;
                srcrect.w = this->innerGeom.w;
                srcrect.h = this->innerGeom.h;

                if (this->drawable) {
                    /* copy background from parent */
#ifdef __PUPTRACE__
                	DEBUGOUT("copy from widget, w = %s -> w = %s\n", widget->getName().c_str(), this->getName().c_str());
#endif
                	this->surface->setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_NOFX));
                    this->surface->blit(widget->surface, &srcrect, 0, 0);
                }
                else {
                    /* this is for example <hbox> or <vbox> which has no own drawing */
                    this->windowSurface->setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_NOFX));
                    this->windowSurface->blit(widget->surface, &srcrect, innerGeom.x, innerGeom.y);
                }
            }
            else {
                /* no parent found, use background from window */
                if (this->rootwindow) {
                    MMSFBColor bgcolor;
                    this->rootwindow->getBgColor(bgcolor);
                    if (!this->rootwindow->bgimage) {
                        /* draw background with window bgcolor */
                        if (bgcolor.a) {
                            if (this->drawable) {
                                /* clear surface */
                                this->surface->clear(bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
                            }
                            else {
                                /* this is for example <hbox> or <vbox> which has no own drawing */
                                this->windowSurface->clear(bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
                            }
                        }
                    }
                    else {
                        /* draw background with a part of window bgimage */
                        DFBRectangle src, dst;
                        int sw, sh;
#ifdef __PUPTRACE__
                        DEBUGOUT("copy from window, win=%s -> w=%s\n", this->rootwindow->getName().c_str(), this->getName().c_str());
#endif

                        /* get width and height of windows background image */
                        this->rootwindow->bgimage->getSize(&sw, &sh);

                        /* calculate with window width and height */
                        int f1 = (this->rootwindow->innerGeom.w * 10000) / sw;
                        int f2 = (this->rootwindow->innerGeom.h * 10000) / sh;

                        /* calculate the source rectangle */
                        src.x = (5000 + 10000 *(this->innerGeom.x - this->rootwindow->innerGeom.x)) / f1;
                        src.w = (5000 + 10000 * this->innerGeom.w) / f1;
                        src.y = (5000 + 10000 *(this->innerGeom.y - this->rootwindow->innerGeom.y)) / f2;
                        src.h = (5000 + 10000 * this->innerGeom.h) / f2;

                        if (this->drawable) {
                            /* the destination rectangle */
                            dst.x = 0;
                            dst.w = this->innerGeom.w;
                            dst.y = 0;
                            dst.h = this->innerGeom.h;

                            /* copy background from window's bgimage */
                            this->surface->setBlittingFlagsByBrightnessAlphaAndOpacity(255, (bgcolor.a)?bgcolor.a:255, 255);
                            this->surface->stretchBlit(this->rootwindow->bgimage, &src, &dst);
                        }
                        else {
                            /* the destination rectangle */
                            dst.x = this->innerGeom.x;
                            dst.w = this->innerGeom.w;
                            dst.y = this->innerGeom.y;
                            dst.h = this->innerGeom.h;

                            /* this is for example <hbox> or <vbox> which has no own drawing */
                            this->windowSurface->setBlittingFlagsByBrightnessAlphaAndOpacity(255, (bgcolor.a)?bgcolor.a:255, 255);
                            this->windowSurface->stretchBlit(this->rootwindow->bgimage, &src, &dst);
                        }
                    }
                }
            }

            if (!this->drawable) {
                /* reset the clip */
                this->windowSurface->setClip(NULL);
            }

            *backgroundFilled = true;
        }
    } while (retry);

    /* unlock */
    if (this->surface) this->surface->unlock();
    this->windowSurface->unlock();

    return true;
}
#endif

bool MMSWidget::draw(bool *backgroundFilled) {
    bool         myBackgroundFilled = false;
    bool         retry = false;

    if (!this->initialized) {
        /* init widget (e.g. load images, fonts, ...) */
        init();
        this->initialized = true;
    }

    if (backgroundFilled) {
    	if (this->has_own_surface)
    		*backgroundFilled = false;
    }
    else
        backgroundFilled = &myBackgroundFilled;

    if ((!this->geomset)||(!this->visible))
        return false;

    /* lock */
    if (this->surface) this->surface->lock();
    this->windowSurface->lock();

    /* draw background */
    do {
        /* searching for the background color or image */
        MMSFBColor col;
        MMSFBSurface *suf = NULL;
        col.a = 0;

        if (this->drawable) {
            if (isActivated()) {
                if (isSelected()) {
                    getSelBgColor(col);
                    suf = this->selbgimage;
                }
                else {
                    getBgColor(col);
                    suf = this->bgimage;
                }
            	if (isPressed()) {
                    MMSFBColor mycol;
                    if (isSelected()) {
                        getSelBgColor_p(mycol);
                        if (mycol.a>0) col=mycol;
                        if (this->selbgimage_p)
                        	suf = this->selbgimage_p;
                    }
                    else {
                        getBgColor_p(mycol);
                        if (mycol.a>0) col=mycol;
                        if (this->bgimage_p)
                        	suf = this->bgimage_p;
                    }
            	}
            }
            else {
                if (isSelected()) {
                    getSelBgColor_i(col);
                    suf = this->selbgimage_i;
                }
                else {
                    getBgColor_i(col);
                    suf = this->bgimage_i;
                }
            }
        }

        if (suf) {
            if ((*backgroundFilled)||(retry)||(!this->has_own_surface)) {
                /* prepare for blitting */
                this->surface->setBlittingFlagsByBrightnessAlphaAndOpacity(brightness, (col.a)?col.a:255, opacity);

                /* fill background */
                surface->stretchBlit(suf, NULL, &surfaceGeom);
                *backgroundFilled = true;

                /* go out of the loop */
                break;
            }
            else
                /* the color has an alpha value and I need a background before drawing the image */
                retry = true;
        }
        else
        if (col.a) {
            if ((*backgroundFilled)||(retry)||(!this->has_own_surface)||((col.a==255)&&(opacity==255))) {
                /* prepare for drawing */
                this->surface->setDrawingColorAndFlagsByBrightnessAndOpacity(col, brightness, opacity);

                /* fill background */
                this->surface->fillRectangle(surfaceGeom.x, surfaceGeom.y, surfaceGeom.w, surfaceGeom.h);
                *backgroundFilled = true;

                /* go out of the loop */
                break;
            }
            else
                /* the color has an alpha value and I need a background for it */
                retry = true;
        }
        else {
            if ((*backgroundFilled)||(!this->has_own_surface)) {
                /* go out of the loop */
            	if (!*backgroundFilled) {
            		if (this->surface) {
            			/* have no background, clear it */
            			/* this is in case of if I have no own surface */
            			this->surface->clear();
            			*backgroundFilled = true;
            		}
            	}
            	break;}
            else
                /* no color, no image, I need to search for a background */
                retry = true;
        }

        /* if not filled then */
        if (!*backgroundFilled) {
        	/* searching for the next parent widget */
            MMSWidget *widget = NULL;
            vector<MMSWidget*> wlist;
            if (this->parent)
                widget = this->parent->getDrawableParent(false, false, false, &wlist);

            /* the widget found can be to far away from this widget */
            /* if wlist is filled, i can search for a better parent which has already a own surface */
            for (unsigned int i=0; i < wlist.size(); i++) {
                MMSWidget *w = wlist.at(i);
                if ((w->drawable)&&(w->geomset)&&(w->visible)) {
                    widget = w;
                    break;
                }
            }

            /* clear it (complete transparent) */
            if (this->drawable) {
                /* my own surface */
                this->surface->clear();
            }
            else {
                /* working direct on the window surface */
                DFBRegion clip;
                clip.x1 = innerGeom.x;
                clip.y1 = innerGeom.y;
                clip.x2 = innerGeom.x + innerGeom.w - 1;
                clip.y2 = innerGeom.y + innerGeom.h - 1;

                this->windowSurface->setClip(&clip);
                this->windowSurface->clear();
            }

            if (widget) {
                /* drawable parent found, calculate rectangle to copy */
                DFBRectangle srcrect = widget->getVisibleSurfaceArea();
                srcrect.x+= this->innerGeom.x - widget->innerGeom.x;
                srcrect.y+= this->innerGeom.y - widget->innerGeom.y;
                srcrect.w = this->innerGeom.w;
                srcrect.h = this->innerGeom.h;

                if (this->drawable) {
                    /* copy background from parent */
                	this->surface->setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_NOFX));
                    this->surface->blit(widget->surface, &srcrect, 0, 0);
                }
                else {
                    /* this is for example <hbox> or <vbox> which has no own drawing */
                    this->windowSurface->setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_NOFX));
                    this->windowSurface->blit(widget->surface, &srcrect, innerGeom.x, innerGeom.y);
                }
            }
            else {
                /* no parent found, use background from window */
                if (this->rootwindow) {
                    MMSFBColor bgcolor;
                    this->rootwindow->getBgColor(bgcolor);
                    if (!this->rootwindow->bgimage) {
                        /* draw background with window bgcolor */
                        if (bgcolor.a) {
                            if (this->drawable) {
                                /* clear surface */
                                this->surface->clear(bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
                            }
                            else {
                                /* this is for example <hbox> or <vbox> which has no own drawing */
                                this->windowSurface->clear(bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
                            }
                        }
                    }
                    else {
                        /* draw background with a part of window bgimage */
                        DFBRectangle src, dst;
                        int sw, sh;

                        /* get width and height of windows background image */
                        this->rootwindow->bgimage->getSize(&sw, &sh);

                        /* calculate with window width and height */
                        int f1 = (this->rootwindow->innerGeom.w * 10000) / sw;
                        int f2 = (this->rootwindow->innerGeom.h * 10000) / sh;

                        /* calculate the source rectangle */
                        src.x = (5000 + 10000 *(this->innerGeom.x - this->rootwindow->innerGeom.x)) / f1;
                        src.w = (5000 + 10000 * this->innerGeom.w) / f1;
                        src.y = (5000 + 10000 *(this->innerGeom.y - this->rootwindow->innerGeom.y)) / f2;
                        src.h = (5000 + 10000 * this->innerGeom.h) / f2;

                        if (this->drawable) {
                            /* the destination rectangle */
                            dst.x = 0;
                            dst.w = this->innerGeom.w;
                            dst.y = 0;
                            dst.h = this->innerGeom.h;

                            /* copy background from window's bgimage */
                            this->surface->setBlittingFlagsByBrightnessAlphaAndOpacity(255, (bgcolor.a)?bgcolor.a:255, 255);
                            this->surface->stretchBlit(this->rootwindow->bgimage, &src, &dst);
                        }
                        else {
                            /* the destination rectangle */
                            dst.x = this->innerGeom.x;
                            dst.w = this->innerGeom.w;
                            dst.y = this->innerGeom.y;
                            dst.h = this->innerGeom.h;

                            /* this is for example <hbox> or <vbox> which has no own drawing */
                            this->windowSurface->setBlittingFlagsByBrightnessAlphaAndOpacity(255, (bgcolor.a)?bgcolor.a:255, 255);
                            this->windowSurface->stretchBlit(this->rootwindow->bgimage, &src, &dst);
                        }
                    }
                }
            }

            if (!this->drawable) {
                /* reset the clip */
                this->windowSurface->setClip(NULL);
            }

            *backgroundFilled = true;
        }
    } while (retry);

    /* unlock */
    if (this->surface) this->surface->unlock();
    this->windowSurface->unlock();

    return true;
}



void MMSWidget::drawMyBorder() {

    if ((!this->drawable)||(!this->geomset)||(!this->visible))
        return;

    unsigned int margin;
    if (!getMargin(margin))
    	margin = 0;

    DFBRectangle mygeom;
    mygeom = this->geom;
    mygeom.x+= margin;
    mygeom.y+= margin;
    mygeom.w-= 2*margin;
    mygeom.h-= 2*margin;

    unsigned int borderthickness;
    if (!getBorderThickness(borderthickness))
    	borderthickness = 0;

    bool borderrcorners;
    if (!getBorderRCorners(borderrcorners))
    	borderrcorners = false;

    if (isSelected()) {
        MMSFBColor c;
        getBorderSelColor(c);
        drawBorder(borderthickness, borderrcorners, this->borderselimages,
                   this->borderselgeom, &(this->borderselgeomset), this->windowSurface,
                   mygeom.x, mygeom.y, mygeom.w, mygeom.h, c, this->rootwindow->im,
                   getBrightness(), getOpacity());
    }
    else {
        MMSFBColor c;
        getBorderColor(c);
        drawBorder(borderthickness, borderrcorners, this->borderimages,
                   this->bordergeom, &(this->bordergeomset), this->windowSurface,
                   mygeom.x, mygeom.y, mygeom.w, mygeom.h, c, this->rootwindow->im,
                   getBrightness(), getOpacity());
    }
}



bool MMSWidget::drawDebug() {
    if ((!this->geomset)||(!this->visible))
        return false;

    bool debug;
    if (!this->getRootWindow()->getDebug(debug)) debug = false;
    if (debug) {
        this->surface->setDrawingFlagsByAlpha(255);
        this->windowSurface->setColor(255, 255, 255, 255);
        this->windowSurface->drawRectangle(this->geom.x, this->geom.y, this->geom.w, this->geom.h);
        if (this->innerGeom.x != this->geom.x) {
            this->windowSurface->setColor(200, 200, 200, 255);
            this->windowSurface->drawRectangle(this->innerGeom.x, this->innerGeom.y,
                                               this->innerGeom.w, this->innerGeom.h);
        }
    }

    if (this->initialArrowsDrawn == false) {
        switchArrowWidgets();
        this->initialArrowsDrawn = true;
    }

    return true;
}

void MMSWidget::drawchildren(bool toRedrawOnly, bool *backgroundFilled) {

    if ((toRedrawOnly) && (this->toRedraw==false) && (this->redrawChildren==false))
        return;

    if (!this->visible)
        return;

    bool myBackgroundFilled = false;
    if (!backgroundFilled)
        backgroundFilled = &myBackgroundFilled;

    if ((!toRedrawOnly)||(this->toRedraw))
    	this->draw(backgroundFilled);

    if ((!toRedrawOnly)||(this->toRedraw)||(this->redrawChildren)) {
	    for(unsigned int i=0;i<this->children.size();i++)
	        this->children.at(i)->drawchildren(toRedrawOnly, backgroundFilled);

    	drawMyBorder();
    }

    this->toRedraw = this->redrawChildren = false;

}

void MMSWidget::add(MMSWidget *widget) {
    if (canHaveChildren())
        if(this->children.size() < 1) {

            this->children.push_back(widget);

            children.at(0)->setParent(this);
            if (this->rootwindow)
                this->rootwindow->add(widget);
        }
        else
            throw new MMSWidgetError(0,"this widget does only support one child");
    else
        throw new MMSWidgetError(0,"this widget does not support children");
}

void MMSWidget::markChildren2Redraw() {
	this->toRedraw = true;
	this->redrawChildren = true;
    for(unsigned int i=0;i<this->children.size();i++) {
        if (this->children.at(i)->isVisible())
            this->children.at(i)->markChildren2Redraw();
    }
}

MMSWidget *MMSWidget::getDrawableParent(bool mark2Redraw, bool markChildren2Redraw, bool checkborder,
                                        vector<MMSWidget*> *wlist, bool followpath) {
    if (mark2Redraw) {
        this->toRedraw = true;

		if (markChildren2Redraw) {
		    this->redrawChildren = true;

		    for(unsigned int i=0;i<this->children.size();i++)
		        if (this->children.at(i)->isVisible())
		            this->children.at(i)->markChildren2Redraw();
		}
    }

    if (followpath)
    	this->redrawChildren = true;

    if (this->needsParentDraw(checkborder)==false) {
        if (wlist) wlist->push_back(this->parent);
        return this->parent->getDrawableParent(false, false, checkborder, wlist, true);
    }
    else
    if (this->parent) {
        if (wlist) wlist->push_back(this->parent);
        return this->parent->getDrawableParent(mark2Redraw, false, checkborder, wlist, followpath);
    }

    return NULL;
}

void MMSWidget::refresh() {
    DFBRectangle tobeupdated;
    unsigned int margin = 0;
    MMSWindow *myroot = this->rootwindow;

    if (!this->geomset) {
    	return;
    }

    if (!this->visible) {
    	return;
    }

    if (!myroot)
   		return;

    // it makes sense that we skip all drawing requests here, if this window OR one of its parents are not shown
    if (!myroot->isShown(true)) {
    	DEBUGMSG("MMSGUI", "MMSWidget->refresh() skipped because window is not shown");
        return;
    }

    /* lock the window because only one thread can do this at the same time */
//    myroot->lock();
    this->parent_rootwindow->lock();

    if (this->drawable)
        getMargin(margin);

    tobeupdated.x = this->geom.x + margin;
    tobeupdated.y = this->geom.y + margin;
    tobeupdated.w = this->geom.w - 2*margin;
    tobeupdated.h = this->geom.h - 2*margin;


    /* e.g. for smooth scrolling menus we must recalculate children here */
    /* so if the widget is a menu and smooth scrolling is enabled, we do the recalculation */
    if (this->type == MMSWIDGETTYPE_MENU)
    	if (((MMSMenuWidget *)this)->getSmoothScrolling())
    		recalculateChildren();

   	myroot->refreshFromChild(this->getDrawableParent(true, true), &tobeupdated, false);

    switchArrowWidgets();

    /* unlock the window */
//    myroot->unlock();
    this->parent_rootwindow->unlock();
}

bool MMSWidget::isDrawable() {
    return this->drawable;
}

bool MMSWidget::needsParentDraw(bool checkborder) {

	//NEW: return true in any case, because we cannot decide it
	return true;


	//OLD code, has to be deleted...

	MMSFBColor c;

	if (this->needsparentdraw)
        return true;

    if (checkborder) {
        unsigned int borderthickness;
        if (!getBorderThickness(borderthickness))
        	borderthickness = 0;

        if (borderthickness>0) {
            bool borderrcorners;
            if (!getBorderRCorners(borderrcorners))
            	borderrcorners = false;

            if ((borderrcorners)||(getOpacity()!=255))
                return true;
            else
            if (this->selected) {
                MMSFBColor c;
                getBorderSelColor(c);
                if (c.a!=255)
                    return true;
            }
            else {
                MMSFBColor c;
                getBorderColor(c);
                if (c.a!=255)
                    return true;
            }
        }
    }

    if (this->activated) {
        if (!this->pressed) {
	        if (this->selected) {
	            getSelBgColor(c);
	            if (c.a==255)
	                return false;
	        }
	        else {
	        	getBgColor(c);
	            if (c.a==255)
	                return false;
	        }
        }
        else {
            if (this->selected) {
            	getSelBgColor_p(c);
                if (c.a==255)
                    return false;
            }
            else {
            	getBgColor_p(c);
                if (c.a==255)
                    return false;
            }
        }
    }
    else {
        if (this->selected) {
        	getSelBgColor_i(c);
            if (c.a==255)
                return false;
        }
        else {
        	getBgColor_i(c);
            if (c.a==255)
                return false;
        }
    }

    return true;
}


 bool MMSWidget::canHaveChildren() {
    return this->canhavechildren;
}

bool MMSWidget::canSelectChildren() {
    return this->canselectchildren;
}


void MMSWidget::setParent(MMSWidget *parent) {
    this->parent = parent;
    for (unsigned int i=0; i < children.size(); i++)
        children.at(i)->setParent(this);
}

MMSWidget *MMSWidget::getParent() {
    return this->parent;
}

void MMSWidget::setRootWindow(MMSWindow *root, MMSWindow *parentroot) {
	// set window on which the widget is connected
    this->rootwindow = root;

    // set the toplevel parent window
    this->parent_rootwindow = parentroot;

    if (this->rootwindow) {
    	// searching the right toplevel parent
        if (!this->parent_rootwindow)
        	if (!this->rootwindow->parent)
        		this->parent_rootwindow = this->rootwindow;
        	else
        		this->parent_rootwindow = this->rootwindow->getParent(true);

        this->windowSurface = this->rootwindow->getSurface();
        rootwindow->add(this);
    }

    // set root window to all children
    for (unsigned int i=0; i < children.size(); i++)
        children.at(i)->setRootWindow(this->rootwindow, this->parent_rootwindow);
}

void MMSWidget::recalculateChildren() {

    if(this->children.size() == 1) {
        children.at(0)->setGeometry(innerGeom);
    }

}

MMSWindow *MMSWidget::getRootWindow(MMSWindow **parentroot) {
	if (parentroot)
		*parentroot = this->parent_rootwindow;
    return this->rootwindow;
}

int MMSWidget::getId() {
    return this->id;
}

string MMSWidget::getName() {
    return this->name;
}

void MMSWidget::setName(string name) {
    this->name = name;
}

void MMSWidget::setFocus(bool set, bool refresh, MMSInputEvent *inputevent) {
    /* switch focused on/off if possible */
	bool b;
    if (!getFocusable(b))
        return;
    if (!b)
    	return;

    /* the focused flag MUST BE set before all other calls (because of dim and trans functions) */
    this->focused = set;

    setSelected(set, refresh);

    if (this->rootwindow)
        this->rootwindow->setFocusedWidget(this, set);

    this->onFocus->emit(this, set);

    // check if we have to navigate
	if (inputevent) {
		bool scrollonfocus;
		if (getScrollOnFocus(scrollonfocus)) {
			if (scrollonfocus) {
				if (inputevent->type == MMSINPUTEVENTTYPE_KEYPRESS) {
					switch (inputevent->key) {
						case MMSKEY_CURSOR_DOWN:
							scrollDown();
						    break;
						case MMSKEY_CURSOR_UP:
							scrollUp();
						    break;
						case MMSKEY_CURSOR_RIGHT:
							scrollRight();
						    break;
						case MMSKEY_CURSOR_LEFT:
							scrollLeft();
					        break;
						default:
							break;
					}
				}
			}
		}
	}
}

bool MMSWidget::isFocused() {
    return this->focused;
}

bool MMSWidget::setSelected(bool set, bool refresh) {

    /* check if selected status already set */
    if (this->selected == set) {
        /* refresh my children */
    	if (canSelectChildren()) {
        	bool rf = false;
            for (unsigned int i=0; i < children.size(); i++)
                if (children.at(i)->setSelected(set, false))
                	rf = true;

            if (refresh && rf)
            	this->refresh();
        }
        return false;
    }

    /* get flags */
    bool selectable;
    if (!getSelectable(selectable))
    	selectable = false;
    bool canselchildren = canSelectChildren();

    /* switch selected on/off if possible */
    if (selectable)
        this->selected=set;

    /* refresh my children */
    if (canselchildren)
        for (unsigned int i=0; i < children.size(); i++)
            children.at(i)->setSelected(set, false);

    /* refresh widget */
    if (((selectable)||(canselchildren))&&(refresh))
    	this->refresh();

    /* emit select signal */
    if (selectable)
        if (set)
            this->onSelect->emit(this);

    return true;
}

bool MMSWidget::isSelected() {
    return this->selected;
}

void MMSWidget::unsetFocusableForAllChildren(bool refresh) {
    for (unsigned int i = 0; i < children.size(); i++) {
        children.at(i)->setFocusable(false, refresh);
        children.at(i)->unsetFocusableForAllChildren(refresh);
    }
}

void MMSWidget::setActivated(bool set, bool refresh) {

    /* check if activated status already set */
    if (this->activated == set) {
        /* refresh my children */
        for (unsigned int i=0; i < children.size(); i++)
            children.at(i)->setActivated(set, false);
        if (refresh)
            this->refresh();
        return;
    }

    /* switch activated on/off */
    this->activated = set;

    /* refresh my children */
    for (unsigned int i=0; i < children.size(); i++)
        children.at(i)->setActivated(set, false);

    /* refresh widget */
    if (refresh)
        this->refresh();
}

bool MMSWidget::isActivated() {
    return this->activated;
}

void MMSWidget::setPressed(bool set, bool refresh) {

    /* check if pressed status already set */
    if (this->pressed == set) {
        /* refresh my children */
        for (unsigned int i=0; i < children.size(); i++)
            children.at(i)->setPressed(set, false);
        if (refresh)
            this->refresh();
        return;
    }

    /* switch pressed on/off */
    this->pressed = set;

    /* refresh my children */
    for (unsigned int i=0; i < children.size(); i++)
        children.at(i)->setPressed(set, false);

    /* refresh widget */
    if (refresh)
        this->refresh();
}

bool MMSWidget::isPressed() {
    return this->pressed;
}

void MMSWidget::setASelected(bool set, bool refresh) {
    setActivated(true, false);
    setSelected(set, refresh);
}

void MMSWidget::setPSelected(bool set, bool refresh) {
    setPressed(true, false);
    setSelected(set, refresh);
}

void MMSWidget::setISelected(bool set, bool refresh) {
    setActivated(false, false);
    setSelected(set, refresh);
}

#ifdef sdsds
void MMSWidget::handleNavigation(DFBInputDeviceKeySymbol key, MMSWidget *requestingchild) {
    /* give the navigation to my parent */
    this->parent->handleNavigation(key,this);
}
#endif

void MMSWidget::handleInput(MMSInputEvent *inputevent) {
	bool b;

	if (inputevent->type == MMSINPUTEVENTTYPE_KEYPRESS) {
		/* keyboard inputs */

		/* save last inputevent */
		last_inputevent = *inputevent;

		switch (inputevent->key) {
			case MMSKEY_CURSOR_DOWN:
/*PERFORMANCE TEST
				for (int ii=0; ii< 15;ii++) scrollDown();
				for (int ii=0; ii< 15;ii++) scrollUp();
				for (int ii=0; ii< 15;ii++) scrollDown();
				for (int ii=0; ii< 15;ii++) scrollUp();
				for (int ii=0; ii< 15;ii++) scrollDown();
				for (int ii=0; ii< 15;ii++) scrollUp();
				for (int ii=0; ii< 15;ii++) scrollDown();
*/

				if (scrollDown())
		            return;
		        break;
			case MMSKEY_CURSOR_UP:
		        if (scrollUp())
		            return;
		        break;
			case MMSKEY_CURSOR_RIGHT:
		        if (scrollRight())
		            return;
		        break;
			case MMSKEY_CURSOR_LEFT:
		        if (scrollLeft())
		            return;
		        break;
			case MMSKEY_RETURN:
			case MMSKEY_ZOOM:
		        if (getFocusable(b))
		        	if (b) {
		        		if (callOnReturn()) this->onReturn->emit(this);
			            return;
			        }
		        break;
                default:
                        break;
		}
	}
	else
	if (inputevent->type == MMSINPUTEVENTTYPE_BUTTONPRESS) {
		/* button pressed */
		if (getFocusable(b, false))
			if (b) {
				/* save last inputevent */
				last_inputevent = *inputevent;

				/* set the pressed status */
				if (!isPressed())
					setPressed(true);

				return;
	        }
	}
	else
	if (inputevent->type == MMSINPUTEVENTTYPE_BUTTONRELEASE) {
		/* button released */
        if (getFocusable(b, false))
        	if (b) {
	    		if (last_inputevent.type == MMSINPUTEVENTTYPE_BUTTONPRESS) {

	    			/* reset the pressed status */
    				if (isPressed())
    					setPressed(false);

					/* check if the pointer is within widget */
	    			if   ((inputevent->posx >= this->geom.x)&&(inputevent->posy >= this->geom.y)
	    				&&(inputevent->posx < this->geom.x + this->geom.w)&&(inputevent->posy < this->geom.y + this->geom.h)) {
	    				/* yes, scroll to the position if possible */
	    				scrollTo(inputevent->posx, inputevent->posy);

	    				/* emit the onReturn */
	    				if (callOnReturn()) this->onReturn->emit(this);
	    			}
	    		}

	    		/* save last inputevent */
	    		last_inputevent = *inputevent;
	    		return;
			}
	}
	else
	if (inputevent->type == MMSINPUTEVENTTYPE_AXISMOTION) {
		/* axis motion */
        if (getFocusable(b, false))
        	if (b) {
	    		if (last_inputevent.type == MMSINPUTEVENTTYPE_BUTTONPRESS) {
					/* check if the pointer is within widget */
	    			if   ((inputevent->posx >= this->geom.x)&&(inputevent->posy >= this->geom.y)
	    				&&(inputevent->posx < this->geom.x + this->geom.w)&&(inputevent->posy < this->geom.y + this->geom.h)) {
	    				/* yes, set the pressed status */
	    				if (!isPressed())
	    					setPressed(true);
	    			}
	    			else {
		    			/* no, reset the pressed status */
	    				if (isPressed())
	    					setPressed(false);
	    			}
	    		}

	    		return;
			}
	}

    throw new MMSWidgetError(1,"input not handled");
}


/*void MMSWidget::registerInput(DFBInputDeviceKeySymbol key, GUIINPUTCALLBACK cb) {
    INPUT_CB *input = new INPUT_CB;

    input->key = key;
    input->cb = cb;

    this->inputs.push_back(input);
}*/

void MMSWidget::setBinData(void *data) {
	this->bindata = data;
}

void *MMSWidget::getBinData() {
	return this->bindata;

}

string MMSWidget::getSizeHint() {
    return this->sizehint;
}

bool MMSWidget::setSizeHint(string &hint) {
    if (getPixelFromSizeHint(NULL, hint, 10000, 0)) {
        this->sizehint = hint;
        return true;
    }
    else
        return false;
}

bool MMSWidget::isGeomSet() {
    return this->geomset;
}

void MMSWidget::setGeomSet(bool set) {
    this->geomset = set;
}


bool MMSWidget::isVisible() {
    return this->visible;
}

void MMSWidget::setVisible(bool visible, bool refresh) {

    if (this->geomset) {
        if (visible) {
            if (!this->visible) {
                if ((!this->surface)&&(surfaceGeom.w!=0)&&(surfaceGeom.h!=0)) {
                    unsigned int w,h;
                    w=surfaceGeom.w;
                    h=surfaceGeom.h;
                    surfaceGeom.w=0;
                    surfaceGeom.h=0;
                    setSurfaceGeometry(w,h);
                }
            }
        }
        else {
            if (this->visible) {
                if (this->surface) {
                    delete this->surface;
                    this->surface = NULL;
                }
            }
        }
    }


//TODO: bgimages handling like MMSImage!!!



    this->visible = visible;
    for (unsigned int i = 0; i < children.size(); i++)
        children.at(i)->setVisible(this->visible, false);
    if (refresh)
        this->refresh();
}

unsigned char MMSWidget::getBrightness() {
    return this->brightness;
}

void MMSWidget::setBrightness(unsigned char brightness, bool refresh) {
    this->brightness = brightness;
    for (unsigned int i = 0; i < children.size(); i++)
        children.at(i)->setBrightness(this->brightness, false);
    if (refresh)
        this->refresh();
}

unsigned char MMSWidget::getOpacity() {
    return this->opacity;
}

void MMSWidget::setOpacity(unsigned char opacity, bool refresh) {
    this->opacity = opacity;
    for (unsigned int i = 0; i < children.size(); i++)
        children.at(i)->setOpacity(this->opacity, false);
    if (refresh)
        this->refresh();
}


MMSWidget *MMSWidget::getNavigateUpWidget() {
    return navigateUpWidget;
}

MMSWidget *MMSWidget::getNavigateDownWidget() {
    return navigateDownWidget;
}

MMSWidget *MMSWidget::getNavigateLeftWidget() {
    return navigateLeftWidget;
}

MMSWidget *MMSWidget::getNavigateRightWidget() {
    return navigateRightWidget;
}

void MMSWidget::setNavigateUpWidget(MMSWidget *upwidget) {
    navigateUpWidget = upwidget;
}

void MMSWidget::setNavigateDownWidget(MMSWidget *downwidget) {
    navigateDownWidget = downwidget;
}

void MMSWidget::setNavigateRightWidget(MMSWidget *rightwidget) {
    navigateRightWidget = rightwidget;
}

void MMSWidget::setNavigateLeftWidget(MMSWidget *leftwidget) {
    navigateLeftWidget = leftwidget;
}

bool MMSWidget::canNavigateUp() {
    if (navigateUpWidget)
        return true;
    else
        return scrollUp(1, false, true);
}

bool MMSWidget::canNavigateDown() {
    if (navigateDownWidget)
        return true;
    else
        return scrollDown(1, false, true);
}

bool MMSWidget::canNavigateLeft() {
	if (navigateLeftWidget)
        return true;
    else
        return scrollLeft(1, false, true);
}

bool MMSWidget::canNavigateRight() {
    if (navigateRightWidget)
        return true;
    else
        return scrollRight(1, false, true);
}

/***********************************************/
/* begin of theme access methods (get methods) */
/***********************************************/

#define GETWIDGET(x,y) \
    if (this->myWidgetClass.is##x()) return myWidgetClass.get##x(y); \
    else if ((widgetClass)&&(widgetClass->is##x())) return widgetClass->get##x(y); \
    else if (baseWidgetClass) return baseWidgetClass->get##x(y); \
    else return myWidgetClass.get##x(y);


bool MMSWidget::getBgColor(MMSFBColor &bgcolor) {
    GETWIDGET(BgColor, bgcolor);
}

bool MMSWidget::getSelBgColor(MMSFBColor &selbgcolor) {
    GETWIDGET(SelBgColor, selbgcolor);
}

bool MMSWidget::getBgColor_p(MMSFBColor &bgcolor_p) {
    GETWIDGET(BgColor_p, bgcolor_p);
}

bool MMSWidget::getSelBgColor_p(MMSFBColor &selbgcolor_p) {
    GETWIDGET(SelBgColor_p, selbgcolor_p);
}

bool MMSWidget::getBgColor_i(MMSFBColor &bgcolor_i) {
    GETWIDGET(BgColor_i, bgcolor_i);
}

bool MMSWidget::getSelBgColor_i(MMSFBColor &selbgcolor_i) {
    GETWIDGET(SelBgColor_i, selbgcolor_i);
}

bool MMSWidget::getBgImagePath(string &bgimagepath) {
    GETWIDGET(BgImagePath,bgimagepath);
}

bool MMSWidget::getBgImageName(string &bgimagename) {
    GETWIDGET(BgImageName,bgimagename);
}

bool MMSWidget::getSelBgImagePath(string &selbgimagepath) {
    GETWIDGET(SelBgImagePath, selbgimagepath);
}

bool MMSWidget::getSelBgImageName(string &selbgimagename) {
    GETWIDGET(SelBgImageName, selbgimagename);
}

bool MMSWidget::getBgImagePath_p(string &bgimagepath_p) {
    GETWIDGET(BgImagePath_p, bgimagepath_p);
}

bool MMSWidget::getBgImageName_p(string &bgimagename_p) {
    GETWIDGET(BgImageName_p, bgimagename_p);
}

bool MMSWidget::getSelBgImagePath_p(string &selbgimagepath_p) {
    GETWIDGET(SelBgImagePath_p, selbgimagepath_p);
}

bool MMSWidget::getSelBgImageName_p(string &selbgimagename_p) {
    GETWIDGET(SelBgImageName_p, selbgimagename_p);
}

bool MMSWidget::getBgImagePath_i(string &bgimagepath_i) {
    GETWIDGET(BgImagePath_i, bgimagepath_i);
}

bool MMSWidget::getBgImageName_i(string &bgimagename_i) {
    GETWIDGET(BgImageName_i, bgimagename_i);
}

bool MMSWidget::getSelBgImagePath_i(string &selbgimagepath_i) {
    GETWIDGET(SelBgImagePath_i, selbgimagepath_i);
}

bool MMSWidget::getSelBgImageName_i(string &selbgimagename_i) {
    GETWIDGET(SelBgImageName_i, selbgimagename_i);
}

bool MMSWidget::getMargin(unsigned int &margin) {
    GETWIDGET(Margin, margin);
}

bool MMSWidget::getFocusable(bool &focusable, bool check_selectable) {
	if (check_selectable) {
		if (getSelectable(focusable)) {
		    if (focusable) {
		    	GETWIDGET(Focusable, focusable);
		    }
		}
		else {
	    	GETWIDGET(Focusable, focusable);
		}
	}
	else {
    	GETWIDGET(Focusable, focusable);
	}

	return false;
}

bool MMSWidget::getSelectable(bool &selectable) {
    GETWIDGET(Selectable, selectable);
}

bool MMSWidget::getUpArrow(string &uparrow) {
    GETWIDGET(UpArrow, uparrow);
}

bool MMSWidget::getDownArrow(string &downarrow) {
    GETWIDGET(DownArrow, downarrow);
}

bool MMSWidget::getLeftArrow(string &leftarrow) {
    GETWIDGET(LeftArrow, leftarrow);
}

bool MMSWidget::getRightArrow(string &rightarrow) {
    GETWIDGET(RightArrow, rightarrow);
}

bool MMSWidget::getData(string &data) {
    GETWIDGET(Data, data);
}

bool MMSWidget::getNavigateUp(string &navigateup) {
    GETWIDGET(NavigateUp, navigateup);
}

bool MMSWidget::getNavigateDown(string &navigatedown) {
    GETWIDGET(NavigateDown, navigatedown);
}

bool MMSWidget::getNavigateLeft(string &navigateleft) {
    GETWIDGET(NavigateLeft, navigateleft);
}

bool MMSWidget::getNavigateRight(string &navigateright) {
    GETWIDGET(NavigateRight, navigateright);
}


bool MMSWidget::getVSlider(string &vslider) {
    GETWIDGET(VSlider, vslider);
}

bool MMSWidget::getHSlider(string &hslider) {
    GETWIDGET(HSlider, hslider);
}

bool MMSWidget::getImagesOnDemand(bool &imagesondemand) {
    GETWIDGET(ImagesOnDemand, imagesondemand);
}

bool MMSWidget::getBlend(unsigned int &blend) {
    GETWIDGET(Blend, blend);
}

bool MMSWidget::getBlendFactor(double &blendfactor) {
    GETWIDGET(BlendFactor, blendfactor);
}

bool MMSWidget::getScrollOnFocus(bool &scrollonfocus) {
    GETWIDGET(ScrollOnFocus, scrollonfocus);
}

#define GETBORDER(x,y) \
    if (this->myWidgetClass.border.is##x()) return myWidgetClass.border.get##x(y); \
    else if ((widgetClass)&&(widgetClass->border.is##x())) return widgetClass->border.get##x(y); \
    else return baseWidgetClass->border.get##x(y);

#define GETBORDER_IMAGES(x,p,y) \
    if (this->myWidgetClass.border.is##x()) return myWidgetClass.border.get##x(p,y); \
    else if ((widgetClass)&&(widgetClass->border.is##x())) return widgetClass->border.get##x(p,y); \
    else return baseWidgetClass->border.get##x(p,y);



bool MMSWidget::getBorderColor(MMSFBColor &color) {
    GETBORDER(Color, color);
}

bool MMSWidget::getBorderSelColor(MMSFBColor &selcolor) {
    GETBORDER(SelColor, selcolor);
}

bool MMSWidget::getBorderImagePath(string &imagepath) {
    GETBORDER(ImagePath, imagepath);
}

bool MMSWidget::getBorderImageNames(MMSBORDER_IMAGE_NUM num, string &imagename) {
    GETBORDER_IMAGES(ImageNames, num, imagename);
}

bool MMSWidget::getBorderSelImagePath(string &selimagepath) {
    GETBORDER(SelImagePath, selimagepath);
}

bool MMSWidget::getBorderSelImageNames(MMSBORDER_IMAGE_NUM num, string &selimagename) {
    GETBORDER_IMAGES(SelImageNames, num, selimagename);
}

bool MMSWidget::getBorderThickness(unsigned int &thickness) {
    GETBORDER(Thickness, thickness);
}

bool MMSWidget::getBorderMargin(unsigned int &margin) {
    GETBORDER(Margin, margin);
}

bool MMSWidget::getBorderRCorners(bool &rcorners) {
    GETBORDER(RCorners, rcorners);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSWidget::setBgColor(MMSFBColor bgcolor, bool refresh) {
    myWidgetClass.setBgColor(bgcolor);
    if (refresh)
        this->refresh();
}

void MMSWidget::setSelBgColor(MMSFBColor selbgcolor, bool refresh) {
    myWidgetClass.setSelBgColor(selbgcolor);
    if (refresh)
        this->refresh();
}

void MMSWidget::setBgColor_p(MMSFBColor bgcolor_p, bool refresh) {
    myWidgetClass.setBgColor_p(bgcolor_p);
    if (refresh)
        this->refresh();
}

void MMSWidget::setSelBgColor_p(MMSFBColor selbgcolor_p, bool refresh) {
    myWidgetClass.setSelBgColor(selbgcolor_p);
    if (refresh)
        this->refresh();
}

void MMSWidget::setBgColor_i(MMSFBColor bgcolor_i, bool refresh) {
    myWidgetClass.setBgColor_i(bgcolor_i);
    if (refresh)
        this->refresh();
}

void MMSWidget::setSelBgColor_i(MMSFBColor selbgcolor_i, bool refresh) {
    myWidgetClass.setSelBgColor(selbgcolor_i);
    if (refresh)
        this->refresh();
}

void MMSWidget::setBgImagePath(string bgimagepath, bool load, bool refresh) {
    myWidgetClass.setBgImagePath(bgimagepath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->bgimage);
            string path, name;
            if (!getBgImagePath(path)) path = "";
            if (!getBgImageName(name)) name = "";
            this->bgimage = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setBgImageName(string bgimagename, bool load, bool refresh) {
    myWidgetClass.setBgImageName(bgimagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->bgimage);
            string path, name;
            if (!getBgImagePath(path)) path = "";
            if (!getBgImageName(name)) name = "";
            this->bgimage = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setSelBgImagePath(string selbgimagepath, bool load, bool refresh) {
    myWidgetClass.setSelBgImagePath(selbgimagepath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selbgimage);
            string path, name;
            if (!getSelBgImagePath(path)) path = "";
            if (!getSelBgImageName(name)) name = "";
            this->selbgimage = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setSelBgImageName(string selbgimagename, bool load, bool refresh) {
    myWidgetClass.setSelBgImageName(selbgimagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selbgimage);
            string path, name;
            if (!getSelBgImagePath(path)) path = "";
            if (!getSelBgImageName(name)) name = "";
            this->selbgimage = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setBgImagePath_p(string bgimagepath_p, bool load, bool refresh) {
    myWidgetClass.setBgImagePath_p(bgimagepath_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->bgimage_p);
            string path, name;
            if (!getBgImagePath_p(path)) path = "";
            if (!getBgImageName_p(name)) name = "";
            this->bgimage_p = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setBgImageName_p(string bgimagename_p, bool load, bool refresh) {
    myWidgetClass.setBgImageName_p(bgimagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->bgimage_p);
            string path, name;
            if (!getBgImagePath_p(path)) path = "";
            if (!getBgImageName_p(name)) name = "";
            this->bgimage_p = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setSelBgImagePath_p(string selbgimagepath_p, bool load, bool refresh) {
    myWidgetClass.setSelBgImagePath_p(selbgimagepath_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selbgimage_p);
            string path, name;
            if (!getSelBgImagePath_p(path)) path = "";
            if (!getSelBgImageName_p(name)) name = "";
            this->selbgimage_p = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setSelBgImageName_p(string selbgimagename_p, bool load, bool refresh) {
    myWidgetClass.setSelBgImageName_p(selbgimagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selbgimage_p);
            string path, name;
            if (!getSelBgImagePath_p(path)) path = "";
            if (!getSelBgImageName_p(name)) name = "";
            this->selbgimage_p = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setBgImagePath_i(string bgimagepath_i, bool load, bool refresh) {
    myWidgetClass.setBgImagePath_i(bgimagepath_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->bgimage_i);
            string path, name;
            if (!getBgImagePath_i(path)) path = "";
            if (!getBgImageName_i(name)) name = "";
            this->bgimage_i = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setBgImageName_i(string bgimagename_i, bool load, bool refresh) {
    myWidgetClass.setBgImageName_i(bgimagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->bgimage_i);
            string path, name;
            if (!getBgImagePath_i(path)) path = "";
            if (!getBgImageName_i(name)) name = "";
            this->bgimage_i = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setSelBgImagePath_i(string selbgimagepath_i, bool load, bool refresh) {
    myWidgetClass.setSelBgImagePath_i(selbgimagepath_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selbgimage_i);
            string path, name;
            if (!getSelBgImagePath_i(path)) path = "";
            if (!getSelBgImageName_i(name)) name = "";
            this->selbgimage_i = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setSelBgImageName_i(string selbgimagename_i, bool load, bool refresh) {
    myWidgetClass.setSelBgImageName_i(selbgimagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selbgimage_i);
            string path, name;
            if (!getSelBgImagePath_i(path)) path = "";
            if (!getSelBgImageName_i(name)) name = "";
            this->selbgimage_i = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setMargin(unsigned int margin, bool refresh) {
    myWidgetClass.setMargin(margin);

    setInnerGeometry();

    if (refresh)
        this->refresh();
}

bool MMSWidget::setFocusable(bool focusable, bool refresh) {
    if (this->focusable_initial) {
        if ((!focusable)&&(isFocused()))
            setFocus(false, refresh);
        myWidgetClass.setFocusable(focusable);
        return true;
    }
    return false;
}

bool MMSWidget::setSelectable(bool selectable, bool refresh) {
    if (this->selectable_initial) {
        if ((!selectable)&&(isSelected()))
            setSelected(false, refresh);
        myWidgetClass.setSelectable(selectable);
        return true;
    }
    return false;
}

void MMSWidget::setUpArrow(string uparrow, bool refresh) {
    myWidgetClass.setUpArrow(uparrow);
    upArrowWidget = NULL;
    if (refresh)
        this->refresh();
}

void MMSWidget::setDownArrow(string downarrow, bool refresh) {
    myWidgetClass.setDownArrow(downarrow);
    downArrowWidget = NULL;
    if (refresh)
        this->refresh();
}

void MMSWidget::setLeftArrow(string leftarrow, bool refresh) {
    myWidgetClass.setLeftArrow(leftarrow);
    leftArrowWidget = NULL;
    if (refresh)
        this->refresh();
}

void MMSWidget::setRightArrow(string rightarrow, bool refresh) {
    myWidgetClass.setRightArrow(rightarrow);
    rightArrowWidget = NULL;
    if (refresh)
        this->refresh();
}

void MMSWidget::setData(string data) {
    myWidgetClass.setData(data);
}

void MMSWidget::setNavigateUp(string navigateup) {
    myWidgetClass.setNavigateUp(navigateup);
    this->navigateUpWidget = NULL;
    if ((this->rootwindow)&&(navigateup!=""))
        this->navigateUpWidget = this->rootwindow->searchForWidget(navigateup);
}

void MMSWidget::setNavigateDown(string navigatedown) {
    myWidgetClass.setNavigateDown(navigatedown);
    this->navigateDownWidget = NULL;
    if ((this->rootwindow)&&(navigatedown!=""))
        this->navigateDownWidget = this->rootwindow->searchForWidget(navigatedown);
}

void MMSWidget::setNavigateLeft(string navigateleft) {
    myWidgetClass.setNavigateLeft(navigateleft);
    this->navigateLeftWidget = NULL;
    if ((this->rootwindow)&&(navigateleft!=""))
        this->navigateLeftWidget = this->rootwindow->searchForWidget(navigateleft);
}

void MMSWidget::setNavigateRight(string navigateright) {
    myWidgetClass.setNavigateRight(navigateright);
    this->navigateRightWidget = NULL;
    if ((this->rootwindow)&&(navigateright!=""))
        this->navigateRightWidget = this->rootwindow->searchForWidget(navigateright);
}

void MMSWidget::setVSlider(string vslider) {
    myWidgetClass.setVSlider(vslider);
    this->vSliderWidget = NULL;
    if ((this->rootwindow)&&(vslider!=""))
        this->vSliderWidget = this->rootwindow->searchForWidget(vslider);
}

void MMSWidget::setHSlider(string hslider) {
    myWidgetClass.setHSlider(hslider);
    this->hSliderWidget = NULL;
    if ((this->rootwindow)&&(hslider!=""))
        this->hSliderWidget = this->rootwindow->searchForWidget(hslider);
}

void MMSWidget::setImagesOnDemand(bool imagesondemand) {
    myWidgetClass.setImagesOnDemand(imagesondemand);
}

void MMSWidget::setBlend(unsigned int blend, bool refresh) {
    myWidgetClass.setBlend(blend);
    for (unsigned int i=0; i < children.size(); i++)
        children.at(i)->setBlend(blend, false);
    if (refresh)
        this->refresh();
}

void MMSWidget::setBlendFactor(double blendfactor, bool refresh) {
    myWidgetClass.setBlendFactor(blendfactor);
    for (unsigned int i=0; i < children.size(); i++)
        children.at(i)->setBlendFactor(blendfactor, false);
    if (refresh)
        this->refresh();
}

void MMSWidget::setScrollOnFocus(bool scrollonfocus) {
    myWidgetClass.setScrollOnFocus(scrollonfocus);
}

void MMSWidget::setBorderColor(MMSFBColor bordercolor, bool refresh) {
    myWidgetClass.border.setColor(bordercolor);
    if (refresh)
        this->refresh();
}

void MMSWidget::setBorderSelColor(MMSFBColor borderselcolor, bool refresh) {
    myWidgetClass.border.setSelColor(borderselcolor);
    if (refresh)
        this->refresh();
}

void MMSWidget::setBorderImagePath(string borderimagepath, bool load, bool refresh) {
    myWidgetClass.border.setImagePath(borderimagepath);
    if (load)
        if (this->rootwindow) {
            string path, name;
            if (!getBorderImagePath(path)) path = "";
            for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++) {
	            this->rootwindow->im->releaseImage(this->borderimages[i]);
	            if (!getBorderImageNames((MMSBORDER_IMAGE_NUM)i, name)) name = "";
	            this->borderimages[i] = this->rootwindow->im->getImage(path, name);
            }
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setBorderImageNames(string imagename_1, string imagename_2, string imagename_3, string imagename_4,
                                    string imagename_5, string imagename_6, string imagename_7, string imagename_8,
                                    bool load, bool refresh) {
    myWidgetClass.border.setImageNames(imagename_1, imagename_2, imagename_3, imagename_4,
                                       imagename_5, imagename_6, imagename_7, imagename_8);
    if (load)
        if (this->rootwindow) {
            string path, name;
            if (!getBorderImagePath(path)) path = "";
            for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++) {
	            this->rootwindow->im->releaseImage(this->borderimages[i]);
	            if (!getBorderImageNames((MMSBORDER_IMAGE_NUM)i, name)) name = "";
	            this->borderimages[i] = this->rootwindow->im->getImage(path, name);
            }
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setBorderSelImagePath(string borderselimagepath, bool load, bool refresh) {
    myWidgetClass.border.setSelImagePath(borderselimagepath);
    if (load)
        if (this->rootwindow) {
            string path, name;
            if (!getBorderSelImagePath(path)) path = "";
            for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++) {
	            this->rootwindow->im->releaseImage(this->borderselimages[i]);
	            if (!getBorderSelImageNames((MMSBORDER_IMAGE_NUM)i, name)) name = "";
	            this->borderselimages[i] = this->rootwindow->im->getImage(path, name);
            }
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setBorderSelImageNames(string selimagename_1, string selimagename_2, string selimagename_3, string selimagename_4,
                                       string selimagename_5, string selimagename_6, string selimagename_7, string selimagename_8,
                                       bool load, bool refresh) {
    myWidgetClass.border.setSelImageNames(selimagename_1, selimagename_2, selimagename_3, selimagename_4,
                                          selimagename_5, selimagename_6, selimagename_7, selimagename_8);
    if (load)
        if (this->rootwindow) {
            string path, name;
            if (!getBorderSelImagePath(path)) path = "";

            for (int i=0;i<MMSBORDER_IMAGE_NUM_SIZE;i++) {
	            this->rootwindow->im->releaseImage(this->borderselimages[i]);
	            if (!getBorderSelImageNames((MMSBORDER_IMAGE_NUM)i, name)) name = "";
	            this->borderselimages[i] = this->rootwindow->im->getImage(path, name);
            }
        }
    if (refresh)
        this->refresh();
}

void MMSWidget::setBorderThickness(unsigned int borderthickness, bool refresh) {
    myWidgetClass.border.setThickness(borderthickness);

    setInnerGeometry();

    if (refresh)
        this->refresh();
}

void MMSWidget::setBorderMargin(unsigned int bordermargin, bool refresh) {
    myWidgetClass.border.setMargin(bordermargin);

    setInnerGeometry();

    if (refresh)
        this->refresh();
}

void MMSWidget::setBorderRCorners(bool borderrcorners, bool refresh) {
    myWidgetClass.border.setRCorners(borderrcorners);

    if (refresh)
        this->refresh();
}

void MMSWidget::updateFromThemeClass(MMSWidgetClass *themeClass) {

	bool 			b;
	MMSFBColor		c;
	string 			s;
	unsigned int	u;
	double			d;

    if (themeClass->getImagesOnDemand(b))
        setImagesOnDemand(b);
    if (themeClass->getBgColor(c))
        setBgColor(c);
    if (themeClass->getSelBgColor(c))
        setSelBgColor(c);
    if (themeClass->getBgColor_p(c))
        setBgColor_p(c);
    if (themeClass->getSelBgColor_p(c))
        setSelBgColor_p(c);
    if (themeClass->getBgColor_i(c))
        setBgColor_i(c);
    if (themeClass->getSelBgColor_i(c))
        setSelBgColor_i(c);
   	if (themeClass->getBgImagePath(s))
        setBgImagePath(s);
    if (themeClass->getBgImageName(s))
        setBgImageName(s);
    if (themeClass->getSelBgImagePath(s))
        setSelBgImagePath(s);
    if (themeClass->getSelBgImageName(s))
        setSelBgImageName(s);
    if (themeClass->getBgImagePath_p(s))
        setBgImagePath_p(s);
    if (themeClass->getBgImageName_p(s))
        setBgImageName_p(s);
    if (themeClass->getSelBgImagePath_p(s))
        setSelBgImagePath_p(s);
    if (themeClass->getSelBgImageName_p(s))
        setSelBgImageName_p(s);
    if (themeClass->getBgImagePath_i(s))
        setBgImagePath_i(s);
    if (themeClass->getBgImageName_i(s))
        setBgImageName_i(s);
    if (themeClass->getSelBgImagePath_i(s))
        setSelBgImagePath_i(s);
    if (themeClass->getSelBgImageName_i(s))
        setSelBgImageName_i(s);
    if (themeClass->getMargin(u))
        setMargin(u);
    if (themeClass->getFocusable(b))
        setFocusable(b);
    if (themeClass->getSelectable(b))
        setSelectable(b);
    if (themeClass->getUpArrow(s))
        setUpArrow(s);
    if (themeClass->getDownArrow(s))
        setDownArrow(s);
    if (themeClass->getLeftArrow(s))
        setLeftArrow(s);
    if (themeClass->getRightArrow(s))
        setRightArrow(s);
    if (themeClass->getData(s))
        setData(s);
    if (themeClass->getNavigateUp(s))
        setNavigateUp(s);
    if (themeClass->getNavigateDown(s))
        setNavigateDown(s);
    if (themeClass->getNavigateLeft(s))
        setNavigateLeft(s);
    if (themeClass->getNavigateRight(s))
        setNavigateRight(s);
    if (themeClass->getVSlider(s))
        setVSlider(s);
    if (themeClass->getHSlider(s))
        setHSlider(s);
    if (themeClass->getBlend(u))
        setBlend(u);
    if (themeClass->getBlendFactor(d))
        setBlendFactor(d);
    if (themeClass->getScrollOnFocus(b))
        setScrollOnFocus(b);
    if (themeClass->border.getColor(c))
        setBorderColor(c);
    if (themeClass->border.getSelColor(c))
        setBorderSelColor(c);
    if (themeClass->border.getImagePath(s))
        setBorderImagePath(s);
    if (themeClass->border.isImageNames()) {
    	string s[8];
    	themeClass->border.getImageNames(MMSBORDER_IMAGE_NUM_TOP_LEFT, s[0]);
    	themeClass->border.getImageNames(MMSBORDER_IMAGE_NUM_TOP, s[1]);
    	themeClass->border.getImageNames(MMSBORDER_IMAGE_NUM_TOP_RIGHT, s[2]);
    	themeClass->border.getImageNames(MMSBORDER_IMAGE_NUM_RIGHT, s[3]);
    	themeClass->border.getImageNames(MMSBORDER_IMAGE_NUM_BOTTOM_RIGHT, s[4]);
    	themeClass->border.getImageNames(MMSBORDER_IMAGE_NUM_BOTTOM, s[5]);
    	themeClass->border.getImageNames(MMSBORDER_IMAGE_NUM_BOTTOM_LEFT, s[6]);
    	themeClass->border.getImageNames(MMSBORDER_IMAGE_NUM_LEFT, s[7]);
        setBorderImageNames(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]);
    }
    if (themeClass->border.getSelImagePath(s))
        setBorderSelImagePath(s);
    if (themeClass->border.isSelImageNames()) {
    	string s[8];
    	themeClass->border.getSelImageNames(MMSBORDER_IMAGE_NUM_TOP_LEFT, s[0]);
    	themeClass->border.getSelImageNames(MMSBORDER_IMAGE_NUM_TOP, s[1]);
    	themeClass->border.getSelImageNames(MMSBORDER_IMAGE_NUM_TOP_RIGHT, s[2]);
    	themeClass->border.getSelImageNames(MMSBORDER_IMAGE_NUM_RIGHT, s[3]);
    	themeClass->border.getSelImageNames(MMSBORDER_IMAGE_NUM_BOTTOM_RIGHT, s[4]);
    	themeClass->border.getSelImageNames(MMSBORDER_IMAGE_NUM_BOTTOM, s[5]);
    	themeClass->border.getSelImageNames(MMSBORDER_IMAGE_NUM_BOTTOM_LEFT, s[6]);
    	themeClass->border.getSelImageNames(MMSBORDER_IMAGE_NUM_LEFT, s[7]);
        setBorderSelImageNames(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]);
    }
    if (themeClass->border.getThickness(u))
        setBorderThickness(u);
    if (themeClass->border.getMargin(u))
        setBorderMargin(u);
    if (themeClass->border.getRCorners(b))
        setBorderRCorners(b);
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
