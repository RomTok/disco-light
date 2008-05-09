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

#include "mmsgui/theme/mmswindowclass.h"


MMSWindowClass::MMSWindowClass() {
    initAlignment();
    initDx();
    initDy();
    initWidth();
    initHeight();
    initBgColor();
    initBgImagePath();
    initBgImageName();
    initOpacity();
    initFadeIn();
    initFadeOut();
    initDebug();
    initMargin();
    initUpArrow();
    initDownArrow();
    initLeftArrow();
    initRightArrow();
    initNavigateUp();
    initNavigateDown();
    initNavigateLeft();
    initNavigateRight();
    initOwnSurface();
    initMoveIn();
    initMoveOut();
}

MMSWindowClass::~MMSWindowClass() {
    freeAlignment();
    freeDx();
    freeDy();
    freeWidth();
    freeHeight();
    freeBgColor();
    freeBgImagePath();
    freeBgImageName();
    freeOpacity();
    freeFadeIn();
    freeFadeOut();
    freeDebug();
    freeMargin();
    freeUpArrow();
    freeDownArrow();
    freeLeftArrow();
    freeRightArrow();
    freeNavigateUp();
    freeNavigateDown();
    freeNavigateLeft();
    freeNavigateRight();
    freeOwnSurface();
    freeMoveIn();
    freeMoveOut();
}

MMSWindowClass &MMSWindowClass::operator=(const MMSWindowClass &c) { 
	if (this != &c) {
		/* copy internal fix data area */
		this->border = c.border; 
		this->id = c.id;

		/* copy external data */
		memset(&(this->ed), 0, sizeof(this->ed));
		if (c.id.isdx)
			this->ed.dx = new string(*c.ed.dx);
		if (c.id.isdy)
			this->ed.dy = new string(*c.ed.dy);
		if (c.id.iswidth)
			this->ed.width = new string(*c.ed.width);
		if (c.id.isheight)
			this->ed.height = new string(*c.ed.height);
		if (c.id.isbgimagepath)
			this->ed.bgimagepath = new string(*c.ed.bgimagepath);
		if (c.id.isbgimagename)
			this->ed.bgimagename = new string(*c.ed.bgimagename);
		if (c.id.isuparrow)
			this->ed.uparrow = new string(*c.ed.uparrow);
		if (c.id.isdownarrow)
			this->ed.downarrow = new string(*c.ed.downarrow);
		if (c.id.isleftarrow)
			this->ed.leftarrow = new string(*c.ed.leftarrow);
		if (c.id.isrightarrow)
			this->ed.rightarrow = new string(*c.ed.rightarrow);
		if (c.id.isnavigateup)
			this->ed.navigateup = new string(*c.ed.navigateup);
		if (c.id.isnavigatedown)
			this->ed.navigatedown = new string(*c.ed.navigatedown);
		if (c.id.isnavigateleft)
			this->ed.navigateleft = new string(*c.ed.navigateleft);
		if (c.id.isnavigateright)
			this->ed.navigateright = new string(*c.ed.navigateright);
	}
	return *this;
}

void MMSWindowClass::unsetAll() {
    unsetAlignment();
    unsetDx();
    unsetDy();
    unsetWidth();
    unsetHeight();
    unsetBgColor();
    unsetBgImagePath();
    unsetBgImageName();
    unsetOpacity();
    unsetFadeIn();
    unsetFadeOut();
    unsetDebug();
    unsetMargin();
    unsetUpArrow();
    unsetDownArrow();
    unsetLeftArrow();
    unsetRightArrow();
    unsetNavigateUp();
    unsetNavigateDown();
    unsetNavigateLeft();
    unsetNavigateRight();
    unsetOwnSurface();
    unsetMoveIn();
    unsetMoveOut();
}

void MMSWindowClass::setAttributesFromTAFF(MMSTaffFile *tafff, string path) {
    DFBColor color;
    bool class_set = false;

	startTAFFScan
	{
        switch (attrid) {
		case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
            class_set = true;
			break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_alignment:
            setAlignment(getAlignmentFromString(attrval_str));
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_dx:
            setDx(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_dy:
            setDy(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_w:
            setWidth(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_h:
            setHeight(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_bgcolor:
			color.a = color.r = color.g = color.b = 0;
            if (isBgColor()) getBgColor(color);
            if (getColorFromString(attrval_str, &color))    
                setBgColor(color);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_bgcolor_a:
			color.a = color.r = color.g = color.b = 0;
            if (isBgColor()) getBgColor(color);
            color.a = attrval_int;
            setBgColor(color);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_bgcolor_r:
			color.a = color.r = color.g = color.b = 0;
            if (isBgColor()) getBgColor(color);
            color.r = attrval_int;
            setBgColor(color);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_bgcolor_g:
			color.a = color.r = color.g = color.b = 0;
            if (isBgColor()) getBgColor(color);
            color.g = attrval_int;
            setBgColor(color);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_bgcolor_b:
			color.a = color.r = color.g = color.b = 0;
            if (isBgColor()) getBgColor(color);
            color.b = attrval_int;
            setBgColor(color);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_bgimage:
            if (*attrval_str)
                setBgImagePath("");
            else
                setBgImagePath(path);
            setBgImageName(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_bgimage_path:
            if (*attrval_str)
                setBgImagePath(attrval_str);
            else
                setBgImagePath(path);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_bgimage_name:
            setBgImageName(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_opacity:
            setOpacity(attrval_int);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_fadein:
            setFadeIn((attrval_int) ? true : false);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_fadeout:
            setFadeOut((attrval_int) ? true : false);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_debug:
            setDebug((attrval_int) ? true : false);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_margin:
            setMargin(attrval_int);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_up_arrow:
            setUpArrow(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_down_arrow:
            setDownArrow(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_left_arrow:
            setLeftArrow(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_right_arrow:
            setRightArrow(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_navigate_up:
            setNavigateUp(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_navigate_down:
            setNavigateDown(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_navigate_left:
            setNavigateLeft(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_navigate_right:
            setNavigateRight(attrval_str);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_own_surface:
            setOwnSurface((attrval_int) ? true : false);
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_movein:
            setMoveIn(getDirectionFromString(attrval_str));
            break;
		case MMSGUI_WINDOW_ATTR::MMSGUI_WINDOW_ATTR_IDS_moveout:
            setMoveOut(getDirectionFromString(attrval_str));
            break;
		}
	}
	endTAFFScan
    
    if ((!isBgImagePath())&&(!class_set)&&(path!=""))
        setBgImagePath(path);
}

void MMSWindowClass::initAlignment() {
	MMSTHEMECLASS_INIT_BASIC(alignment);
}

void MMSWindowClass::freeAlignment() {
	MMSTHEMECLASS_FREE_BASIC(alignment);
}

bool MMSWindowClass::isAlignment() {
	MMSTHEMECLASS_ISSET(alignment);
}

void MMSWindowClass::unsetAlignment() {
	MMSTHEMECLASS_UNSET(alignment);
}

void MMSWindowClass::setAlignment(MMSALIGNMENT alignment) {
	MMSTHEMECLASS_SET_BASIC(alignment);
}

bool MMSWindowClass::getAlignment(MMSALIGNMENT &alignment) {
	MMSTHEMECLASS_GET_BASIC(alignment);
}

void MMSWindowClass::initDx() {
	MMSTHEMECLASS_INIT_STRING(dx);
}

void MMSWindowClass::freeDx() {
	MMSTHEMECLASS_FREE_STRING(dx);
}

bool MMSWindowClass::isDx() {
	MMSTHEMECLASS_ISSET(dx);
}

void MMSWindowClass::unsetDx() {
	MMSTHEMECLASS_UNSET(dx);
}

void MMSWindowClass::setDx(const string &dx) {
	MMSTHEMECLASS_SET_STRING(dx);
}

bool MMSWindowClass::getDx(string &dx) {
	MMSTHEMECLASS_GET_STRING(dx);
}

void MMSWindowClass::initDy() {
	MMSTHEMECLASS_INIT_STRING(dy);
}

void MMSWindowClass::freeDy() {
	MMSTHEMECLASS_FREE_STRING(dy);
}

bool MMSWindowClass::isDy() {
	MMSTHEMECLASS_ISSET(dy);
}

void MMSWindowClass::unsetDy() {
	MMSTHEMECLASS_UNSET(dy);
}

void MMSWindowClass::setDy(const string &dy) {
	MMSTHEMECLASS_SET_STRING(dy);
}

bool MMSWindowClass::getDy(string &dy) {
	MMSTHEMECLASS_GET_STRING(dy);
}

void MMSWindowClass::initWidth() {
	MMSTHEMECLASS_INIT_STRING(width);
}

void MMSWindowClass::freeWidth() {
	MMSTHEMECLASS_FREE_STRING(width);
}

bool MMSWindowClass::isWidth() {
	MMSTHEMECLASS_ISSET(width);
}

void MMSWindowClass::unsetWidth() {
	MMSTHEMECLASS_UNSET(width);
}

void MMSWindowClass::setWidth(const string &width) {
	MMSTHEMECLASS_SET_STRING(width);
}

bool MMSWindowClass::getWidth(string &width) {
	MMSTHEMECLASS_GET_STRING(width);
}

void MMSWindowClass::initHeight() {
	MMSTHEMECLASS_INIT_STRING(height);
}

void MMSWindowClass::freeHeight() {
	MMSTHEMECLASS_FREE_STRING(height);
}

bool MMSWindowClass::isHeight() {
	MMSTHEMECLASS_ISSET(height);
}

void MMSWindowClass::unsetHeight() {
	MMSTHEMECLASS_UNSET(height);
}

void MMSWindowClass::setHeight(const string &height) {
	MMSTHEMECLASS_SET_STRING(height);
}

bool MMSWindowClass::getHeight(string &height) {
	MMSTHEMECLASS_GET_STRING(height);
}

void MMSWindowClass::initBgColor() {
	MMSTHEMECLASS_INIT_BASIC(bgcolor);
}

void MMSWindowClass::freeBgColor() {
	MMSTHEMECLASS_FREE_BASIC(bgcolor);
}

bool MMSWindowClass::isBgColor() {
	MMSTHEMECLASS_ISSET(bgcolor);
}

void MMSWindowClass::unsetBgColor() {
	MMSTHEMECLASS_UNSET(bgcolor);
}

void MMSWindowClass::setBgColor(const DFBColor &bgcolor) {
	MMSTHEMECLASS_SET_BASIC(bgcolor);
}

bool MMSWindowClass::getBgColor(DFBColor &bgcolor) {
	MMSTHEMECLASS_GET_BASIC(bgcolor);
}

void MMSWindowClass::initBgImagePath() {
	MMSTHEMECLASS_INIT_STRING(bgimagepath);
}

void MMSWindowClass::freeBgImagePath() {
	MMSTHEMECLASS_FREE_STRING(bgimagepath);
}

bool MMSWindowClass::isBgImagePath() {
	MMSTHEMECLASS_ISSET(bgimagepath);
}

void MMSWindowClass::unsetBgImagePath() {
	MMSTHEMECLASS_UNSET(bgimagepath);
}

void MMSWindowClass::setBgImagePath(const string &bgimagepath) {
	MMSTHEMECLASS_SET_STRING(bgimagepath);
}

bool MMSWindowClass::getBgImagePath(string &bgimagepath) {
	MMSTHEMECLASS_GET_STRING(bgimagepath);
}

void MMSWindowClass::initBgImageName() {
	MMSTHEMECLASS_INIT_STRING(bgimagename);
}

void MMSWindowClass::freeBgImageName() {
	MMSTHEMECLASS_FREE_STRING(bgimagename);
}

bool MMSWindowClass::isBgImageName() {
	MMSTHEMECLASS_ISSET(bgimagename);
}

void MMSWindowClass::unsetBgImageName() {
	MMSTHEMECLASS_UNSET(bgimagename);
}

void MMSWindowClass::setBgImageName(const string &bgimagename) {
	MMSTHEMECLASS_SET_STRING(bgimagename);
}

bool MMSWindowClass::getBgImageName(string &bgimagename) {
	MMSTHEMECLASS_GET_STRING(bgimagename);
}

void MMSWindowClass::initOpacity() {
	MMSTHEMECLASS_INIT_BASIC(opacity);
}

void MMSWindowClass::freeOpacity() {
	MMSTHEMECLASS_FREE_BASIC(opacity);
}

bool MMSWindowClass::isOpacity() {
	MMSTHEMECLASS_ISSET(opacity);
}

void MMSWindowClass::unsetOpacity() {
	MMSTHEMECLASS_UNSET(opacity);
}

void MMSWindowClass::setOpacity(unsigned int opacity) {
	MMSTHEMECLASS_SET_BASIC(opacity);
}

bool MMSWindowClass::getOpacity(unsigned int &opacity) {
	MMSTHEMECLASS_GET_BASIC(opacity);
}

void MMSWindowClass::initFadeIn() {
	MMSTHEMECLASS_INIT_BASIC(fadein);
}

void MMSWindowClass::freeFadeIn() {
	MMSTHEMECLASS_FREE_BASIC(fadein);
}

bool MMSWindowClass::isFadeIn() {
	MMSTHEMECLASS_ISSET(fadein);
}

void MMSWindowClass::unsetFadeIn() {
	MMSTHEMECLASS_UNSET(fadein);
}

void MMSWindowClass::setFadeIn(bool fadein) {
	MMSTHEMECLASS_SET_BASIC(fadein);
}

bool MMSWindowClass::getFadeIn(bool &fadein) {
	MMSTHEMECLASS_GET_BASIC(fadein);
}

void MMSWindowClass::initFadeOut() {
	MMSTHEMECLASS_INIT_BASIC(fadeout);
}

void MMSWindowClass::freeFadeOut() {
	MMSTHEMECLASS_FREE_BASIC(fadeout);
}

bool MMSWindowClass::isFadeOut() {
	MMSTHEMECLASS_ISSET(fadeout);
}

void MMSWindowClass::unsetFadeOut() {
	MMSTHEMECLASS_UNSET(fadeout);
}

void MMSWindowClass::setFadeOut(bool fadeout) {
	MMSTHEMECLASS_SET_BASIC(fadeout);
}

bool MMSWindowClass::getFadeOut(bool &fadeout) {
	MMSTHEMECLASS_GET_BASIC(fadeout);
}

void MMSWindowClass::initDebug() {
	MMSTHEMECLASS_INIT_BASIC(debug);
}

void MMSWindowClass::freeDebug() {
	MMSTHEMECLASS_FREE_BASIC(debug);
}

bool MMSWindowClass::isDebug() {
	MMSTHEMECLASS_ISSET(debug);
}

void MMSWindowClass::unsetDebug() {
	MMSTHEMECLASS_UNSET(debug);
}

void MMSWindowClass::setDebug(bool debug) {
	MMSTHEMECLASS_SET_BASIC(debug);
}

bool MMSWindowClass::getDebug(bool &debug) {
	MMSTHEMECLASS_GET_BASIC(debug);
}

void MMSWindowClass::initMargin() {
	MMSTHEMECLASS_INIT_BASIC(margin);
}

void MMSWindowClass::freeMargin() {
	MMSTHEMECLASS_FREE_BASIC(margin);
}

bool MMSWindowClass::isMargin() {
	MMSTHEMECLASS_ISSET(margin);
}

void MMSWindowClass::unsetMargin() {
	MMSTHEMECLASS_UNSET(margin);
}

void MMSWindowClass::setMargin(unsigned int margin) {
	MMSTHEMECLASS_SET_BASIC(margin);
}

bool MMSWindowClass::getMargin(unsigned int &margin) {
	MMSTHEMECLASS_GET_BASIC(margin);
}

void MMSWindowClass::initUpArrow() {
	MMSTHEMECLASS_INIT_STRING(uparrow);
}

void MMSWindowClass::freeUpArrow() {
	MMSTHEMECLASS_FREE_STRING(uparrow);
}

bool MMSWindowClass::isUpArrow() {
	MMSTHEMECLASS_ISSET(uparrow);
}

void MMSWindowClass::unsetUpArrow() {
	MMSTHEMECLASS_UNSET(uparrow);
}

void MMSWindowClass::setUpArrow(const string &uparrow) {
	MMSTHEMECLASS_SET_STRING(uparrow);
}

bool MMSWindowClass::getUpArrow(string &uparrow) {
	MMSTHEMECLASS_GET_STRING(uparrow);
}

void MMSWindowClass::initDownArrow() {
	MMSTHEMECLASS_INIT_STRING(downarrow);
}

void MMSWindowClass::freeDownArrow() {
	MMSTHEMECLASS_FREE_STRING(downarrow);
}

bool MMSWindowClass::isDownArrow() {
	MMSTHEMECLASS_ISSET(downarrow);
}

void MMSWindowClass::unsetDownArrow() {
	MMSTHEMECLASS_UNSET(downarrow);
}

void MMSWindowClass::setDownArrow(const string &downarrow) {
	MMSTHEMECLASS_SET_STRING(downarrow);
}

bool MMSWindowClass::getDownArrow(string &downarrow) {
	MMSTHEMECLASS_GET_STRING(downarrow);
}

void MMSWindowClass::initLeftArrow() {
	MMSTHEMECLASS_INIT_STRING(leftarrow);
}

void MMSWindowClass::freeLeftArrow() {
	MMSTHEMECLASS_FREE_STRING(leftarrow);
}

bool MMSWindowClass::isLeftArrow() {
	MMSTHEMECLASS_ISSET(leftarrow);
}

void MMSWindowClass::unsetLeftArrow() {
	MMSTHEMECLASS_UNSET(leftarrow);
}

void MMSWindowClass::setLeftArrow(const string &leftarrow) {
	MMSTHEMECLASS_SET_STRING(leftarrow);
}

bool MMSWindowClass::getLeftArrow(string &leftarrow) {
	MMSTHEMECLASS_GET_STRING(leftarrow);
}

void MMSWindowClass::initRightArrow() {
	MMSTHEMECLASS_INIT_STRING(rightarrow);
}

void MMSWindowClass::freeRightArrow() {
	MMSTHEMECLASS_FREE_STRING(rightarrow);
}

bool MMSWindowClass::isRightArrow() {
	MMSTHEMECLASS_ISSET(rightarrow);
}

void MMSWindowClass::unsetRightArrow() {
	MMSTHEMECLASS_UNSET(rightarrow);
}

void MMSWindowClass::setRightArrow(const string &rightarrow) {
	MMSTHEMECLASS_SET_STRING(rightarrow);
}

bool MMSWindowClass::getRightArrow(string &rightarrow) {
	MMSTHEMECLASS_GET_STRING(rightarrow);
}

void MMSWindowClass::initNavigateUp() {
	MMSTHEMECLASS_INIT_STRING(navigateup);
}

void MMSWindowClass::freeNavigateUp() {
	MMSTHEMECLASS_FREE_STRING(navigateup);
}

bool MMSWindowClass::isNavigateUp() {
	MMSTHEMECLASS_ISSET(navigateup);
}

void MMSWindowClass::unsetNavigateUp() {
	MMSTHEMECLASS_UNSET(navigateup);
}

void MMSWindowClass::setNavigateUp(const string &navigateup) {
	MMSTHEMECLASS_SET_STRING(navigateup);
}

bool MMSWindowClass::getNavigateUp(string &navigateup) {
	MMSTHEMECLASS_GET_STRING(navigateup);
}

void MMSWindowClass::initNavigateDown() {
	MMSTHEMECLASS_INIT_STRING(navigatedown);
}

void MMSWindowClass::freeNavigateDown() {
	MMSTHEMECLASS_FREE_STRING(navigatedown);
}

bool MMSWindowClass::isNavigateDown() {
	MMSTHEMECLASS_ISSET(navigatedown);
}

void MMSWindowClass::unsetNavigateDown() {
	MMSTHEMECLASS_UNSET(navigatedown);
}

void MMSWindowClass::setNavigateDown(const string &navigatedown) {
	MMSTHEMECLASS_SET_STRING(navigatedown);
}

bool MMSWindowClass::getNavigateDown(string &navigatedown) {
	MMSTHEMECLASS_GET_STRING(navigatedown);
}

void MMSWindowClass::initNavigateLeft() {
	MMSTHEMECLASS_INIT_STRING(navigateleft);
}

void MMSWindowClass::freeNavigateLeft() {
	MMSTHEMECLASS_FREE_STRING(navigateleft);
}

bool MMSWindowClass::isNavigateLeft() {
	MMSTHEMECLASS_ISSET(navigateleft);
}

void MMSWindowClass::unsetNavigateLeft() {
	MMSTHEMECLASS_UNSET(navigateleft);
}

void MMSWindowClass::setNavigateLeft(const string &navigateleft) {
	MMSTHEMECLASS_SET_STRING(navigateleft);
}

bool MMSWindowClass::getNavigateLeft(string &navigateleft) {
	MMSTHEMECLASS_GET_STRING(navigateleft);
}

void MMSWindowClass::initNavigateRight() {
	MMSTHEMECLASS_INIT_STRING(navigateright);
}

void MMSWindowClass::freeNavigateRight() {
	MMSTHEMECLASS_FREE_STRING(navigateright);
}

bool MMSWindowClass::isNavigateRight() {
	MMSTHEMECLASS_ISSET(navigateright);
}

void MMSWindowClass::unsetNavigateRight() {
	MMSTHEMECLASS_UNSET(navigateright);
}

void MMSWindowClass::setNavigateRight(const string &navigateright) {
	MMSTHEMECLASS_SET_STRING(navigateright);
}

bool MMSWindowClass::getNavigateRight(string &navigateright) {
	MMSTHEMECLASS_GET_STRING(navigateright);
}

void MMSWindowClass::initOwnSurface() {
	MMSTHEMECLASS_INIT_BASIC(ownsurface);
}

void MMSWindowClass::freeOwnSurface() {
	MMSTHEMECLASS_FREE_BASIC(ownsurface);
}

bool MMSWindowClass::isOwnSurface() {
	MMSTHEMECLASS_ISSET(ownsurface);
}

void MMSWindowClass::unsetOwnSurface() {
	MMSTHEMECLASS_UNSET(ownsurface);
}

void MMSWindowClass::setOwnSurface(bool ownsurface) {
	MMSTHEMECLASS_SET_BASIC(ownsurface);
}

bool MMSWindowClass::getOwnSurface(bool &ownsurface) {
	MMSTHEMECLASS_GET_BASIC(ownsurface);
}

void MMSWindowClass::initMoveIn() {
	MMSTHEMECLASS_INIT_BASIC(movein);
}

void MMSWindowClass::freeMoveIn() {
	MMSTHEMECLASS_FREE_BASIC(movein);
}

bool MMSWindowClass::isMoveIn() {
	MMSTHEMECLASS_ISSET(movein);
}

void MMSWindowClass::unsetMoveIn() {
	MMSTHEMECLASS_UNSET(movein);
}

void MMSWindowClass::setMoveIn(MMSDIRECTION movein) {
	MMSTHEMECLASS_SET_BASIC(movein);
}

bool MMSWindowClass::getMoveIn(MMSDIRECTION &movein) {
	MMSTHEMECLASS_GET_BASIC(movein);
}


void MMSWindowClass::initMoveOut() {
	MMSTHEMECLASS_INIT_BASIC(moveout);
}

void MMSWindowClass::freeMoveOut() {
	MMSTHEMECLASS_FREE_BASIC(moveout);
}

bool MMSWindowClass::isMoveOut() {
	MMSTHEMECLASS_ISSET(moveout);
}

void MMSWindowClass::unsetMoveOut() {
	MMSTHEMECLASS_UNSET(moveout);
}

void MMSWindowClass::setMoveOut(MMSDIRECTION moveout) {
	MMSTHEMECLASS_SET_BASIC(moveout);
}

bool MMSWindowClass::getMoveOut(MMSDIRECTION &moveout) {
	MMSTHEMECLASS_GET_BASIC(moveout);
}

