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

#include "mmsgui/theme/mmsmenuclass.h"

//store attribute descriptions here 
TAFF_ATTRDESC MMSGUI_MENU_ATTR_I[] = MMSGUI_MENU_ATTR_INIT;

//address attribute names
#define GETATTRNAME(aname) MMSGUI_MENU_ATTR_I[MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_##aname].name
#define ISATTRNAME(aname) (strcmp(attrname, GETATTRNAME(aname))==0)


MMSMenuClass::MMSMenuClass() {
    this->tafff = NULL;
    unsetAll();
}

MMSMenuClass::~MMSMenuClass() {
    if (this->tafff)
        delete this->tafff;
    this->tafff = NULL;
}

void MMSMenuClass::unsetAll() {
    this->className = "";
    if (this->tafff)
        delete this->tafff;
    this->tafff = NULL;
    unsetItemWidth();
    unsetItemHeight();
    unsetItemHMargin();
    unsetItemVMargin();
    unsetCols();
    unsetDimItems();
    unsetFixedPos();
    unsetHLoop();
    unsetVLoop();
    unsetTransItems();
    unsetDimTop();
    unsetDimBottom();
    unsetDimLeft();
    unsetDimRight();
    unsetTransTop();
    unsetTransBottom();
    unsetTransLeft();
    unsetTransRight();
    unsetZoomSelWidth();
    unsetZoomSelHeight();
    unsetZoomSelShiftX();
    unsetZoomSelShiftY();
    unsetSmoothScrolling();
}

void MMSMenuClass::setAttributesFromXMLNode(MMSTaffFile *tafff, string prefix, string path) {

    if (prefix == "") {
		startTAFFScan
		{
			switch (attrid) {
			case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
	            setClassName(attrval_str);
				break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_item_width:
	            setItemWidth(attrval_str);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_item_height:
	            setItemHeight(attrval_str);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_item_hmargin:
	            setItemHMargin(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_item_vmargin:
	            setItemVMargin(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_cols:
	            setCols(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_dim_items:
	            setDimItems(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_fixed_pos:
	            setFixedPos(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_hloop:
	            setHLoop((attrval_int) ? true : false);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_vloop:
	            setVLoop((attrval_int) ? true : false);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_trans_items:
	            setTransItems(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_dim_top:
	            setDimTop(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_dim_bottom:
	            setDimBottom(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_dim_left:
	            setDimLeft(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_dim_right:
	            setDimRight(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_trans_top:
	            setTransTop(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_trans_bottom:
	            setTransBottom(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_trans_left:
	            setTransLeft(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_trans_right:
	            setTransRight(attrval_int);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_zoomsel_width:
	            setZoomSelWidth(attrval_str);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_zoomsel_height:
	            setZoomSelHeight(attrval_str);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_zoomsel_shiftx:
	            setZoomSelShiftX(attrval_str);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_zoomsel_shifty:
	            setZoomSelShiftY(attrval_str);
	            break;
			case MMSGUI_MENU_ATTR::MMSGUI_MENU_ATTR_IDS_smooth_scrolling:
	            setSmoothScrolling((attrval_int) ? true : false);
	            break;
			}
		}
		endTAFFScan
    }
    else {
    	unsigned int pl = strlen(prefix.c_str());
    	
    	startTAFFScan_WITHOUT_ID
    	{
    		/* check if attrname has correct prefix */
    		if (pl >= strlen(attrname))
        		continue;
            if (memcmp(attrname, prefix.c_str(), pl)!=0)
            	continue;
            attrname = &attrname[pl];

    		/* okay, correct prefix, check attributes now */
            if (ISATTRNAME(item_width)) { 
	            setItemWidth(attrval_str);
            }
            else
            if (ISATTRNAME(item_height)) { 
	            setItemHeight(attrval_str);
            }
            else
            if (ISATTRNAME(item_hmargin)) { 
	            setItemHMargin(attrval_int);
            }
            else
            if (ISATTRNAME(item_vmargin)) { 
	            setItemVMargin(attrval_int);
            }
            else
            if (ISATTRNAME(cols)) { 
	            setCols(attrval_int);
            }
            else
            if (ISATTRNAME(dim_items)) { 
	            setDimItems(attrval_int);
            }
            else
            if (ISATTRNAME(fixed_pos)) { 
	            setFixedPos(attrval_int);
            }
            else
            if (ISATTRNAME(hloop)) { 
	            setHLoop((attrval_int) ? true : false);
            }
            else
            if (ISATTRNAME(vloop)) { 
	            setVLoop((attrval_int) ? true : false);
            }
            else
            if (ISATTRNAME(trans_items)) { 
	            setTransItems(attrval_int);
            }
            else
            if (ISATTRNAME(dim_top)) { 
	            setDimTop(attrval_int);
            }
            else
            if (ISATTRNAME(dim_bottom)) { 
	            setDimBottom(attrval_int);
            }
            else
            if (ISATTRNAME(dim_left)) { 
	            setDimLeft(attrval_int);
            }
            else
            if (ISATTRNAME(dim_right)) { 
	            setDimRight(attrval_int);
            }
            else
            if (ISATTRNAME(trans_top)) { 
	            setTransTop(attrval_int);
            }
            else
            if (ISATTRNAME(trans_bottom)) { 
	            setTransBottom(attrval_int);
            }
            else
            if (ISATTRNAME(trans_left)) { 
	            setTransLeft(attrval_int);
            }
            else
            if (ISATTRNAME(trans_right)) { 
	            setTransRight(attrval_int);
            }
            else
            if (ISATTRNAME(zoomsel_width)) { 
	            setZoomSelWidth(attrval_str);
            }
            else
            if (ISATTRNAME(zoomsel_height)) { 
	            setZoomSelHeight(attrval_str);
            }
            else
            if (ISATTRNAME(zoomsel_shiftx)) { 
	            setZoomSelShiftX(attrval_str);
            }
            else
            if (ISATTRNAME(zoomsel_shifty)) {  
	            setZoomSelShiftY(attrval_str);
            }
            else
            if (ISATTRNAME(smooth_scrolling)) { 
	            setSmoothScrolling((attrval_int) ? true : false);
			}
    	}
    	endTAFFScan_WITHOUT_ID
    }
}

void MMSMenuClass::duplicateXMLNode(MMSTaffFile *tafff) {
    if (this->tafff)
        delete this->tafff;
    this->tafff = tafff->copyCurrentTag();
}

MMSTaffFile *MMSMenuClass::getXMLNode() {
	if (this->tafff)
		this->tafff->getFirstTag();
    return this->tafff;
}

void MMSMenuClass::setClassName(string className) {
    this->className = className;
}

string MMSMenuClass::getClassName() {
    return this->className;
}

bool MMSMenuClass::isItemWidth() {
    return this->isitemwidth;
}

void MMSMenuClass::setItemWidth(string itemwidth) {
    this->itemwidth = itemwidth;
    this->isitemwidth = true;
}

void MMSMenuClass::unsetItemWidth() {
    this->isitemwidth = false;
}

string MMSMenuClass::getItemWidth() {
    return this->itemwidth;
}

bool MMSMenuClass::isItemHeight() {
    return this->isitemheight;
}

void MMSMenuClass::setItemHeight(string itemheight) {
    this->itemheight = itemheight;
    this->isitemheight = true;
}

void MMSMenuClass::unsetItemHeight() {
    this->isitemheight = false;
}

string MMSMenuClass::getItemHeight() {
    return this->itemheight;
}

bool MMSMenuClass::isItemHMargin() {
    return this->isitemhmargin;
}

void MMSMenuClass::setItemHMargin(unsigned int itemhmargin) {
    this->itemhmargin = itemhmargin;
    this->isitemhmargin = true;
}

void MMSMenuClass::unsetItemHMargin() {
    this->isitemhmargin = false;
}

unsigned int MMSMenuClass::getItemHMargin() {
    return this->itemhmargin;
}

bool MMSMenuClass::isItemVMargin() {
    return this->isitemvmargin;
}

void MMSMenuClass::setItemVMargin(unsigned int itemvmargin) {
    this->itemvmargin = itemvmargin;
    this->isitemvmargin = true;
}

void MMSMenuClass::unsetItemVMargin() {
    this->isitemvmargin = false;
}

unsigned int MMSMenuClass::getItemVMargin() {
    return this->itemvmargin;
}

bool MMSMenuClass::isCols() {
    return this->iscols;
}

void MMSMenuClass::setCols(unsigned int cols) {
    this->cols = cols;
    this->iscols = true;
}

void MMSMenuClass::unsetCols() {
    this->iscols = false;
}

unsigned int MMSMenuClass::getCols() {
    return this->cols;
}

bool MMSMenuClass::isDimItems() {
    return this->isdimitems;
}

void MMSMenuClass::setDimItems(unsigned int dimitems) {
    this->dimitems = dimitems;
    this->isdimitems = true;
}

void MMSMenuClass::unsetDimItems() {
    this->isdimitems = false;
}

unsigned int MMSMenuClass::getDimItems() {
    return this->dimitems;
}

bool MMSMenuClass::isFixedPos() {
    return this->isfixedpos;
}

void MMSMenuClass::setFixedPos(int fixedpos) {
    this->fixedpos = fixedpos;
    this->isfixedpos = true;
}

void MMSMenuClass::unsetFixedPos() {
    this->isfixedpos = false;
}

int MMSMenuClass::getFixedPos() {
    return this->fixedpos;
}

bool MMSMenuClass::isHLoop() {
    return this->ishloop;
}

void MMSMenuClass::setHLoop(bool hloop) {
    this->hloop = hloop;
    this->ishloop = true;
}

void MMSMenuClass::unsetHLoop() {
    this->ishloop = false;
}

bool MMSMenuClass::getHLoop() {
    return this->hloop;
}

bool MMSMenuClass::isVLoop() {
    return this->isvloop;
}

void MMSMenuClass::setVLoop(bool vloop) {
    this->vloop = vloop;
    this->isvloop = true;
}

void MMSMenuClass::unsetVLoop() {
    this->isvloop = false;
}

bool MMSMenuClass::getVLoop() {
    return this->vloop;
}

bool MMSMenuClass::isTransItems() {
    return this->istransitems;
}

void MMSMenuClass::setTransItems(unsigned int transitems) {
    this->transitems = transitems;
    this->istransitems = true;
}

void MMSMenuClass::unsetTransItems() {
    this->istransitems = false;
}

unsigned int MMSMenuClass::getTransItems() {
    return this->transitems;
}

bool MMSMenuClass::isDimTop() {
    return this->isdimtop;
}

void MMSMenuClass::setDimTop(unsigned int dimtop) {
    this->dimtop = dimtop;
    this->isdimtop = true;
}

void MMSMenuClass::unsetDimTop() {
    this->isdimtop = false;
}

unsigned int MMSMenuClass::getDimTop() {
    return this->dimtop;
}

bool MMSMenuClass::isDimBottom() {
    return this->isdimbottom;
}

void MMSMenuClass::setDimBottom(unsigned int dimbottom) {
    this->dimbottom = dimbottom;
    this->isdimbottom = true;
}

void MMSMenuClass::unsetDimBottom() {
    this->isdimbottom = false;
}

unsigned int MMSMenuClass::getDimBottom() {
    return this->dimbottom;
}

bool MMSMenuClass::isDimLeft() {
    return this->isdimleft;
}

void MMSMenuClass::setDimLeft(unsigned int dimleft) {
    this->dimleft = dimleft;
    this->isdimleft = true;
}

void MMSMenuClass::unsetDimLeft() {
    this->isdimleft = false;
}

unsigned int MMSMenuClass::getDimLeft() {
    return this->dimleft;
}

bool MMSMenuClass::isDimRight() {
    return this->isdimright;
}

void MMSMenuClass::setDimRight(unsigned int dimright) {
    this->dimright = dimright;
    this->isdimright = true;
}

void MMSMenuClass::unsetDimRight() {
    this->isdimright = false;
}

unsigned int MMSMenuClass::getDimRight() {
    return this->dimright;
}

bool MMSMenuClass::isTransTop() {
    return this->istranstop;
}

void MMSMenuClass::setTransTop(unsigned int transtop) {
    this->transtop = transtop;
    this->istranstop = true;
}

void MMSMenuClass::unsetTransTop() {
    this->istranstop = false;
}

unsigned int MMSMenuClass::getTransTop() {
    return this->transtop;
}

bool MMSMenuClass::isTransBottom() {
    return this->istransbottom;
}

void MMSMenuClass::setTransBottom(unsigned int transbottom) {
    this->transbottom = transbottom;
    this->istransbottom = true;
}

void MMSMenuClass::unsetTransBottom() {
    this->istransbottom = false;
}

unsigned int MMSMenuClass::getTransBottom() {
    return this->transbottom;
}

bool MMSMenuClass::isTransLeft() {
    return this->istransleft;
}

void MMSMenuClass::setTransLeft(unsigned int transleft) {
    this->transleft = transleft;
    this->istransleft = true;
}

void MMSMenuClass::unsetTransLeft() {
    this->istransleft = false;
}

unsigned int MMSMenuClass::getTransLeft() {
    return this->transleft;
}

bool MMSMenuClass::isTransRight() {
    return this->istransright;
}

void MMSMenuClass::setTransRight(unsigned int transright) {
    this->transright = transright;
    this->istransright = true;
}

void MMSMenuClass::unsetTransRight() {
    this->istransright = false;
}

unsigned int MMSMenuClass::getTransRight() {
    return this->transright;
}



bool MMSMenuClass::isZoomSelWidth() {
    return this->iszoomselwidth;
}

void MMSMenuClass::setZoomSelWidth(string zoomselwidth) {
    this->zoomselwidth = zoomselwidth;
    this->iszoomselwidth = true;
}

void MMSMenuClass::unsetZoomSelWidth() {
    this->iszoomselwidth = false;
}

string MMSMenuClass::getZoomSelWidth() {
    return this->zoomselwidth;
}

bool MMSMenuClass::isZoomSelHeight() {
    return this->iszoomselheight;
}

void MMSMenuClass::setZoomSelHeight(string zoomselheight) {
    this->zoomselheight = zoomselheight;
    this->iszoomselheight = true;
}

void MMSMenuClass::unsetZoomSelHeight() {
    this->iszoomselheight = false;
}

string MMSMenuClass::getZoomSelHeight() {
    return this->zoomselheight;
}

bool MMSMenuClass::isZoomSelShiftX() {
    return this->iszoomselshiftx;
}

void MMSMenuClass::setZoomSelShiftX(string zoomselshiftx) {
    this->zoomselshiftx = zoomselshiftx;
    this->iszoomselshiftx = true;
}

void MMSMenuClass::unsetZoomSelShiftX() {
    this->iszoomselshiftx = false;
}

string MMSMenuClass::getZoomSelShiftX() {
    return this->zoomselshiftx;
}

bool MMSMenuClass::isZoomSelShiftY() {
    return this->iszoomselshifty;
}

void MMSMenuClass::setZoomSelShiftY(string zoomselshifty) {
    this->zoomselshifty = zoomselshifty;
    this->iszoomselshifty = true;
}

void MMSMenuClass::unsetZoomSelShiftY() {
    this->iszoomselshifty = false;
}

string MMSMenuClass::getZoomSelShiftY() {
    return this->zoomselshifty;
}

bool MMSMenuClass::isSmoothScrolling() {
    return this->issmoothscrolling;
}

void MMSMenuClass::setSmoothScrolling(bool smoothscrolling) {
    this->smoothscrolling = smoothscrolling;
    this->issmoothscrolling = true;
}

void MMSMenuClass::unsetSmoothScrolling() {
    this->issmoothscrolling = false;
}

bool MMSMenuClass::getSmoothScrolling() {
    return this->smoothscrolling;
}

