/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
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

#include "mmsgui/theme/mmswidgetclass.h"


MMSWidgetClass::MMSWidgetClass() {
	initBgColor();
    initSelBgColor();
    initBgColor_p();
    initSelBgColor_p();
    initBgColor_i();
    initSelBgColor_i();
    
    initBgImagePath();
    initBgImageName();
    initSelBgImagePath();
    initSelBgImageName();
    initBgImagePath_p();
    initBgImageName_p();
    initSelBgImagePath_p();
    initSelBgImageName_p();
    initBgImagePath_i();
    initBgImageName_i();
    initSelBgImagePath_i();
    initSelBgImageName_i();

    initMargin();
    initFocusable();
    initSelectable();

    initUpArrow();
    initDownArrow();
    initLeftArrow();
    initRightArrow();

    initData();

    initNavigateUp();
    initNavigateDown();
    initNavigateLeft();
    initNavigateRight();

    initVSlider();
    initHSlider();

    initImagesOnDemand();

    initBlend();
    initBlendFactor();
}

MMSWidgetClass::~MMSWidgetClass() {
	freeBgColor();
    freeSelBgColor();
    freeBgColor_p();
    freeSelBgColor_p();
    freeBgColor_i();
    freeSelBgColor_i();
    
    freeBgImagePath();
    freeBgImageName();
    freeSelBgImagePath();
    freeSelBgImageName();
    freeBgImagePath_p();
    freeBgImageName_p();
    freeSelBgImagePath_p();
    freeSelBgImageName_p();
    freeBgImagePath_i();
    freeBgImageName_i();
    freeSelBgImagePath_i();
    freeSelBgImageName_i();

    freeMargin();
    freeFocusable();
    freeSelectable();

    freeUpArrow();
    freeDownArrow();
    freeLeftArrow();
    freeRightArrow();

    freeData();

    freeNavigateUp();
    freeNavigateDown();
    freeNavigateLeft();
    freeNavigateRight();

    freeVSlider();
    freeHSlider();

    freeImagesOnDemand();

    freeBlend();
    freeBlendFactor();
}

MMSWidgetClass &MMSWidgetClass::operator=(const MMSWidgetClass &c) { 
	if (this != &c) {
		/* copy internal fix data area */
		this->border = c.border; 
		this->id = c.id;

		/* copy external data */
		memset(&(this->ed), 0, sizeof(this->ed));
		if (c.id.isbgimagepath)
			this->ed.bgimagepath = new string(*c.ed.bgimagepath);
		if (c.id.isbgimagename)
			this->ed.bgimagename = new string(*c.ed.bgimagename);
		if (c.id.isselbgimagepath)
			this->ed.selbgimagepath = new string(*c.ed.selbgimagepath);
		if (c.id.isselbgimagename)
			this->ed.selbgimagename = new string(*c.ed.selbgimagename);
		if (c.id.isbgimagepath_p)
			this->ed.bgimagepath_p = new string(*c.ed.bgimagepath_p);
		if (c.id.isbgimagename_p)
			this->ed.bgimagename_p = new string(*c.ed.bgimagename_p);
		if (c.id.isselbgimagepath_p)
			this->ed.selbgimagepath_p = new string(*c.ed.selbgimagepath_p);
		if (c.id.isselbgimagename_p)
			this->ed.selbgimagename_p = new string(*c.ed.selbgimagename_p);
		if (c.id.isbgimagepath_i)
			this->ed.bgimagepath_i = new string(*c.ed.bgimagepath_i);
		if (c.id.isbgimagename_i)
			this->ed.bgimagename_i = new string(*c.ed.bgimagename_i);
		if (c.id.isselbgimagepath_i)
			this->ed.selbgimagepath_i = new string(*c.ed.selbgimagepath_i);
		if (c.id.isselbgimagename_i)
			this->ed.selbgimagename_i = new string(*c.ed.selbgimagename_i);
		if (c.id.isuparrow)
			this->ed.uparrow = new string(*c.ed.uparrow);
		if (c.id.isdownarrow)
			this->ed.downarrow = new string(*c.ed.downarrow);
		if (c.id.isleftarrow)
			this->ed.leftarrow = new string(*c.ed.leftarrow);
		if (c.id.isleftarrow)
			this->ed.leftarrow = new string(*c.ed.leftarrow);
		if (c.id.isdata)
			this->ed.data = new string(*c.ed.data);
		if (c.id.isnavigateup)
			this->ed.navigateup = new string(*c.ed.navigateup);
		if (c.id.isnavigatedown)
			this->ed.navigatedown = new string(*c.ed.navigatedown);
		if (c.id.isnavigateleft)
			this->ed.navigateleft = new string(*c.ed.navigateleft);
		if (c.id.isnavigateright)
			this->ed.navigateright = new string(*c.ed.navigateright);
		if (c.id.isvslider)
			this->ed.vslider = new string(*c.ed.vslider);
		if (c.id.ishslider)
			this->ed.hslider = new string(*c.ed.hslider);
	}
	return *this;
}

void MMSWidgetClass::unsetAll() {
    unsetBgColor();
    unsetSelBgColor();
    unsetBgColor_p();
    unsetSelBgColor_p();
    unsetBgColor_i();
    unsetSelBgColor_i();
    
    unsetBgImagePath();
    unsetBgImageName();
    unsetSelBgImagePath();
    unsetSelBgImageName();
    unsetBgImagePath_p();
    unsetBgImageName_p();
    unsetSelBgImagePath_p();
    unsetSelBgImageName_p();
    unsetBgImagePath_i();
    unsetBgImageName_i();
    unsetSelBgImagePath_i();
    unsetSelBgImageName_i();

    unsetMargin();
    unsetFocusable();
    unsetSelectable();

    unsetUpArrow();
    unsetDownArrow();
    unsetLeftArrow();
    unsetRightArrow();

    unsetData();

    unsetNavigateUp();
    unsetNavigateDown();
    unsetNavigateLeft();
    unsetNavigateRight();

    unsetVSlider();
    unsetHSlider();

    unsetImagesOnDemand();

    unsetBlend();
    unsetBlendFactor();
}

void MMSWidgetClass::setAttributesFromXMLNode(xmlNode* node, string prefix, string path) {
    bool class_set = false;

    startXMLScan
    {
        DFBColor color;
        color.a = 0;
        color.r = 0;
        color.g = 0;
        color.b = 0;

        if(attrName == "class") {
            class_set = true;
        }
        else if(attrName == prefix + "bgcolor") {
            if (isBgColor()) getBgColor(color);

            if (getColorFromString(attrValue, &color))    
                setBgColor(color);
        }
        else if(attrName == prefix + "bgcolor.a") {
            if (isBgColor()) getBgColor(color);
    
            color.a = atoi(attrValue.c_str());

            setBgColor(color);
        }
        else if(attrName == prefix + "bgcolor.r") {
            if (isBgColor()) getBgColor(color);
    
            color.r = atoi(attrValue.c_str());

            setBgColor(color);
        }
        else if(attrName == prefix + "bgcolor.g") {
            if (isBgColor()) getBgColor(color);
    
            color.g = atoi(attrValue.c_str());

            setBgColor(color);
        }
        else if(attrName == prefix + "bgcolor.b") {
            if (isBgColor()) getBgColor(color);
    
            color.b = atoi(attrValue.c_str());

            setBgColor(color);
        }
        else if(attrName == prefix + "selbgcolor") {
            if (isSelBgColor()) getSelBgColor(color);

            if (getColorFromString(attrValue, &color))    
                setSelBgColor(color);
        }
        else if(attrName == prefix + "selbgcolor.a") {
            if (isSelBgColor()) getSelBgColor(color);
    
            color.a = atoi(attrValue.c_str());

            setSelBgColor(color);
        }
        else if(attrName == prefix + "selbgcolor.r") {
            if (isSelBgColor()) getSelBgColor(color);
    
            color.r = atoi(attrValue.c_str());

            setSelBgColor(color);
        }
        else if(attrName == prefix + "selbgcolor.g") {
            if (isSelBgColor()) getSelBgColor(color);
    
            color.g = atoi(attrValue.c_str());

            setSelBgColor(color);
        }
        else if(attrName == prefix + "selbgcolor.b") {
            if (isSelBgColor()) getSelBgColor(color);
    
            color.b = atoi(attrValue.c_str());

            setSelBgColor(color);
        }
        else if(attrName == prefix + "bgcolor_p") {
            if (isBgColor_p()) getBgColor_p(color);

            if (getColorFromString(attrValue, &color))    
                setBgColor_p(color);
        }
        else if(attrName == prefix + "bgcolor_p.a") {
            if (isBgColor_p()) getBgColor_p(color);
    
            color.a = atoi(attrValue.c_str());

            setBgColor_p(color);
        }
        else if(attrName == prefix + "bgcolor_p.r") {
            if (isBgColor_p()) getBgColor_p(color);
    
            color.r = atoi(attrValue.c_str());

            setBgColor_p(color);
        }
        else if(attrName == prefix + "bgcolor_p.g") {
            if (isBgColor_p()) getBgColor_p(color);
    
            color.g = atoi(attrValue.c_str());

            setBgColor_p(color);
        }
        else if(attrName == prefix + "bgcolor_p.b") {
            if (isBgColor_p()) getBgColor_p(color);
    
            color.b = atoi(attrValue.c_str());

            setBgColor_p(color);
        }
        else if(attrName == prefix + "selbgcolor_p") {
            if (isSelBgColor_p()) getSelBgColor_p(color);

            if (getColorFromString(attrValue, &color))    
                setSelBgColor_p(color);
        }
        else if(attrName == prefix + "selbgcolor_p.a") {
            if (isSelBgColor_p()) getSelBgColor_p(color);
    
            color.a = atoi(attrValue.c_str());

            setSelBgColor_p(color);
        }
        else if(attrName == prefix + "selbgcolor_p.r") {
            if (isSelBgColor_p()) getSelBgColor_p(color);
    
            color.r = atoi(attrValue.c_str());

            setSelBgColor_p(color);
        }
        else if(attrName == prefix + "selbgcolor_p.g") {
            if (isSelBgColor_p()) getSelBgColor_p(color);
    
            color.g = atoi(attrValue.c_str());

            setSelBgColor_p(color);
        }
        else if(attrName == prefix + "selbgcolor_p.b") {
            if (isSelBgColor_p()) getSelBgColor_p(color);
    
            color.b = atoi(attrValue.c_str());

            setSelBgColor_p(color);
        }
        else if(attrName == prefix + "bgcolor_i") {
            if (isBgColor_i()) getBgColor_i(color);

            if (getColorFromString(attrValue, &color))    
                setBgColor_i(color);
        }
        else if(attrName == prefix + "bgcolor_i.a") {
            if (isBgColor_i()) getBgColor_i(color);
    
            color.a = atoi(attrValue.c_str());

            setBgColor_i(color);
        }
        else if(attrName == prefix + "bgcolor_i.r") {
            if (isBgColor_i()) getBgColor_i(color);
    
            color.r = atoi(attrValue.c_str());

            setBgColor_i(color);
        }
        else if(attrName == prefix + "bgcolor_i.g") {
            if (isBgColor_i()) getBgColor_i(color);
    
            color.g = atoi(attrValue.c_str());

            setBgColor_i(color);
        }
        else if(attrName == prefix + "bgcolor_i.b") {
            if (isBgColor_i()) getBgColor_i(color);
    
            color.b = atoi(attrValue.c_str());

            setBgColor_i(color);
        }
        else if(attrName == prefix + "selbgcolor_i") {
            if (isSelBgColor_i()) getSelBgColor_i(color);

            if (getColorFromString(attrValue, &color))    
                setSelBgColor_i(color);
        }
        else if(attrName == prefix + "selbgcolor_i.a") {
            if (isSelBgColor_i()) getSelBgColor_i(color);
    
            color.a = atoi(attrValue.c_str());

            setSelBgColor_i(color);
        }
        else if(attrName == prefix + "selbgcolor_i.r") {
            if (isSelBgColor_i()) getSelBgColor_i(color);
    
            color.r = atoi(attrValue.c_str());

            setSelBgColor_i(color);
        }
        else if(attrName == prefix + "selbgcolor_i.g") {
            if (isSelBgColor_i()) getSelBgColor_i(color);
    
            color.g = atoi(attrValue.c_str());

            setSelBgColor_i(color);
        }
        else if(attrName == prefix + "selbgcolor_i.b") {
            if (isSelBgColor_i()) getSelBgColor_i(color);
    
            color.b = atoi(attrValue.c_str());

            setSelBgColor_i(color);
        }
        else if(attrName == prefix + "bgimage") {
            if (attrValue != "")
                setBgImagePath("");
            else
                setBgImagePath(path);
            setBgImageName(attrValue);
        }
        else if(attrName == prefix + "bgimage.path") {
            if (attrValue != "")
                setBgImagePath(attrValue);
            else
                setBgImagePath(path);
        }
        else if(attrName == prefix + "bgimage.name") {
            setBgImageName(attrValue);
        }
        else if(attrName == prefix + "selbgimage") {
            if (attrValue != "")
                setSelBgImagePath("");
            else
                setSelBgImagePath(path);
            setSelBgImageName(attrValue);
        }
        else if(attrName == prefix + "selbgimage.path") {
            if (attrValue != "")
                setSelBgImagePath(attrValue);
            else
                setSelBgImagePath(path);
        }
        else if(attrName == prefix + "selbgimage.name") {
            setSelBgImageName(attrValue);
        }
        else if(attrName == prefix + "bgimage_p") {
            if (attrValue != "")
                setBgImagePath_p("");
            else
                setBgImagePath_p(path);
            setBgImageName_p(attrValue);
        }
        else if(attrName == prefix + "bgimage_p.path") {
            if (attrValue != "")
                setBgImagePath_p(attrValue);
            else
                setBgImagePath_p(path);
        }
        else if(attrName == prefix + "bgimage_p.name") {
            setBgImageName_p(attrValue);
        }
        else if(attrName == prefix + "selbgimage_p") {
            if (attrValue != "")
                setSelBgImagePath_p("");
            else
                setSelBgImagePath_p(path);
            setSelBgImageName_p(attrValue);
        }
        else if(attrName == prefix + "selbgimage_p.path") {
            if (attrValue != "")
                setSelBgImagePath_p(attrValue);
            else
                setSelBgImagePath_p(path);
        }
        else if(attrName == prefix + "selbgimage_p.name") {
            setSelBgImageName_p(attrValue);
        }
        else if(attrName == prefix + "bgimage_i") {
            if (attrValue != "")
                setBgImagePath_i("");
            else
                setBgImagePath_i(path);
            setBgImageName_i(attrValue);
        }
        else if(attrName == prefix + "bgimage_i.path") {
            if (attrValue != "")
                setBgImagePath_i(attrValue);
            else
                setBgImagePath_i(path);
        }
        else if(attrName == prefix + "bgimage_i.name") {
            setBgImageName_i(attrValue);
        }
        else if(attrName == prefix + "selbgimage_i") {
            if (attrValue != "")
                setSelBgImagePath_i("");
            else
                setSelBgImagePath_i(path);
            setSelBgImageName_i(attrValue);
        }
        else if(attrName == prefix + "selbgimage_i.path") {
            if (attrValue != "")
                setSelBgImagePath_i(attrValue);
            else
                setSelBgImagePath_i(path);
        }
        else if(attrName == prefix + "selbgimage_i.name") {
            setSelBgImageName_i(attrValue);
        }
        else if(attrName == prefix + "margin") {
            setMargin(atoi(attrValue.c_str()));
        }
        else if(attrName == prefix + "focusable") {
            setFocusable((attrValue == "true") ? true : false);
        }
        else if(attrName == prefix + "selectable") {
            setSelectable((attrValue == "true") ? true : false);
        }
        else if(attrName == prefix + "up_arrow") {
            setUpArrow(attrValue);
        }
        else if(attrName == prefix + "down_arrow") {
            setDownArrow(attrValue);
        }
        else if(attrName == prefix + "left_arrow") {
            setLeftArrow(attrValue);
        }
        else if(attrName == prefix + "right_arrow") {
            setRightArrow(attrValue);
        }
        else if(attrName == prefix + "data") {
            setData(attrValue);
        }
        else if(attrName == prefix + "navigate_up") {
            setNavigateUp(attrValue);
        }
        else if(attrName == prefix + "navigate_down") {
            setNavigateDown(attrValue);
        }
        else if(attrName == prefix + "navigate_left") {
            setNavigateLeft(attrValue);
        }
        else if(attrName == prefix + "navigate_right") {
            setNavigateRight(attrValue);
        }
        else if(attrName == prefix + "vslider") {
            setVSlider(attrValue);
        }
        else if(attrName == prefix + "hslider") {
            setHSlider(attrValue);
        }
        else if(attrName == prefix + "imagesondemand")
            setImagesOnDemand((attrValue == "true") ? true : false);
        else if(attrName == prefix + "blend")
            setBlend(atoi(attrValue.c_str()));
        else if(attrName == prefix + "blend_factor")
            setBlendFactor(atof(attrValue.c_str()));
    }
    endXMLScan 

    if ((!isBgImagePath())&&(!class_set)&&(path!=""))
        setBgImagePath(path);
    if ((!isSelBgImagePath())&&(!class_set)&&(path!=""))
        setSelBgImagePath(path);
    if ((!isBgImagePath_p())&&(!class_set)&&(path!=""))
        setBgImagePath_p(path);
    if ((!isSelBgImagePath_p())&&(!class_set)&&(path!=""))
        setSelBgImagePath_p(path);
    if ((!isBgImagePath_i())&&(!class_set)&&(path!=""))
        setBgImagePath_i(path);
    if ((!isSelBgImagePath_i())&&(!class_set)&&(path!=""))
        setSelBgImagePath_i(path);
}

void MMSWidgetClass::initBgColor() {
	MMSTHEMECLASS_INIT_BASIC(bgcolor);
}

void MMSWidgetClass::freeBgColor() {
	MMSTHEMECLASS_FREE_BASIC(bgcolor);
}

bool MMSWidgetClass::isBgColor() {
	MMSTHEMECLASS_ISSET(bgcolor);
}

void MMSWidgetClass::unsetBgColor() {
	MMSTHEMECLASS_UNSET(bgcolor);
}

void MMSWidgetClass::setBgColor(const DFBColor &bgcolor) {
	MMSTHEMECLASS_SET_BASIC(bgcolor);
}

bool MMSWidgetClass::getBgColor(DFBColor &bgcolor) {
	MMSTHEMECLASS_GET_BASIC(bgcolor);
}

void MMSWidgetClass::initSelBgColor() {
	MMSTHEMECLASS_INIT_BASIC(selbgcolor);
}

void MMSWidgetClass::freeSelBgColor() {
	MMSTHEMECLASS_FREE_BASIC(selbgcolor);
}

bool MMSWidgetClass::isSelBgColor() {
	MMSTHEMECLASS_ISSET(selbgcolor);
}

void MMSWidgetClass::unsetSelBgColor() {
	MMSTHEMECLASS_UNSET(selbgcolor);
}

void MMSWidgetClass::setSelBgColor(const DFBColor &selbgcolor) {
	MMSTHEMECLASS_SET_BASIC(selbgcolor);
}

bool MMSWidgetClass::getSelBgColor(DFBColor &selbgcolor) {
	MMSTHEMECLASS_GET_BASIC(selbgcolor);
}


void MMSWidgetClass::initBgColor_p() {
	MMSTHEMECLASS_INIT_BASIC(bgcolor_p);
}

void MMSWidgetClass::freeBgColor_p() {
	MMSTHEMECLASS_FREE_BASIC(bgcolor_p);
}

bool MMSWidgetClass::isBgColor_p() {
	MMSTHEMECLASS_ISSET(bgcolor_p);
}

void MMSWidgetClass::unsetBgColor_p() {
	MMSTHEMECLASS_UNSET(bgcolor_p);
}

void MMSWidgetClass::setBgColor_p(const DFBColor &bgcolor_p) {
	MMSTHEMECLASS_SET_BASIC(bgcolor_p);
}

bool MMSWidgetClass::getBgColor_p(DFBColor &bgcolor_p) {
	MMSTHEMECLASS_GET_BASIC(bgcolor_p);
}

void MMSWidgetClass::initSelBgColor_p() {
	MMSTHEMECLASS_INIT_BASIC(selbgcolor_p);
}

void MMSWidgetClass::freeSelBgColor_p() {
	MMSTHEMECLASS_FREE_BASIC(selbgcolor_p);
}

bool MMSWidgetClass::isSelBgColor_p() {
	MMSTHEMECLASS_ISSET(selbgcolor_p);
}

void MMSWidgetClass::unsetSelBgColor_p() {
	MMSTHEMECLASS_UNSET(selbgcolor_p);
}

void MMSWidgetClass::setSelBgColor_p(const DFBColor &selbgcolor_p) {
	MMSTHEMECLASS_SET_BASIC(selbgcolor_p);
}

bool MMSWidgetClass::getSelBgColor_p(DFBColor &selbgcolor_p) {
	MMSTHEMECLASS_GET_BASIC(selbgcolor_p);
}

void MMSWidgetClass::initBgColor_i() {
	MMSTHEMECLASS_INIT_BASIC(bgcolor_i);
}

void MMSWidgetClass::freeBgColor_i() {
	MMSTHEMECLASS_FREE_BASIC(bgcolor_i);
}

bool MMSWidgetClass::isBgColor_i() {
	MMSTHEMECLASS_ISSET(bgcolor_i);
}

void MMSWidgetClass::unsetBgColor_i() {
	MMSTHEMECLASS_UNSET(bgcolor_i);
}

void MMSWidgetClass::setBgColor_i(const DFBColor &bgcolor_i) {
	MMSTHEMECLASS_SET_BASIC(bgcolor_i);
}

bool MMSWidgetClass::getBgColor_i(DFBColor &bgcolor_i) {
	MMSTHEMECLASS_GET_BASIC(bgcolor_i);
}

void MMSWidgetClass::initSelBgColor_i() {
	MMSTHEMECLASS_INIT_BASIC(selbgcolor_i);
}

void MMSWidgetClass::freeSelBgColor_i() {
	MMSTHEMECLASS_FREE_BASIC(selbgcolor_i);
}

bool MMSWidgetClass::isSelBgColor_i() {
	MMSTHEMECLASS_ISSET(selbgcolor_i);
}

void MMSWidgetClass::unsetSelBgColor_i() {
	MMSTHEMECLASS_UNSET(selbgcolor_i);
}

void MMSWidgetClass::setSelBgColor_i(const DFBColor &selbgcolor_i) {
	MMSTHEMECLASS_SET_BASIC(selbgcolor_i);
}

bool MMSWidgetClass::getSelBgColor_i(DFBColor &selbgcolor_i) {
	MMSTHEMECLASS_GET_BASIC(selbgcolor_i);
}


void MMSWidgetClass::initBgImagePath() {
	MMSTHEMECLASS_INIT_STRING(bgimagepath);
}

void MMSWidgetClass::freeBgImagePath() {
	MMSTHEMECLASS_FREE_STRING(bgimagepath);
}

bool MMSWidgetClass::isBgImagePath() {
	MMSTHEMECLASS_ISSET(bgimagepath);
}

void MMSWidgetClass::unsetBgImagePath() {
	MMSTHEMECLASS_UNSET(bgimagepath);
}

void MMSWidgetClass::setBgImagePath(const string &bgimagepath) {
	MMSTHEMECLASS_SET_STRING(bgimagepath);
}

bool MMSWidgetClass::getBgImagePath(string &bgimagepath) {
	MMSTHEMECLASS_GET_STRING(bgimagepath);
}



void MMSWidgetClass::initBgImageName() {
	MMSTHEMECLASS_INIT_STRING(bgimagename);
}

void MMSWidgetClass::freeBgImageName() {
	MMSTHEMECLASS_FREE_STRING(bgimagename);
}

bool MMSWidgetClass::isBgImageName() {
	MMSTHEMECLASS_ISSET(bgimagename);
}

void MMSWidgetClass::unsetBgImageName() {
	MMSTHEMECLASS_UNSET(bgimagename);
}

void MMSWidgetClass::setBgImageName(const string &bgimagename) {
	MMSTHEMECLASS_SET_STRING(bgimagename);
}

bool MMSWidgetClass::getBgImageName(string &bgimagename) {
	MMSTHEMECLASS_GET_STRING(bgimagename);
}

void MMSWidgetClass::initSelBgImagePath() {
	MMSTHEMECLASS_INIT_STRING(selbgimagepath);
}

void MMSWidgetClass::freeSelBgImagePath() {
	MMSTHEMECLASS_FREE_STRING(selbgimagepath);
}

bool MMSWidgetClass::isSelBgImagePath() {
	MMSTHEMECLASS_ISSET(selbgimagepath);
}

void MMSWidgetClass::unsetSelBgImagePath() {
	MMSTHEMECLASS_UNSET(selbgimagepath);
}

void MMSWidgetClass::setSelBgImagePath(const string &selbgimagepath) {
	MMSTHEMECLASS_SET_STRING(selbgimagepath);
}

bool MMSWidgetClass::getSelBgImagePath(string &selbgimagepath) {
	MMSTHEMECLASS_GET_STRING(selbgimagepath);
}

void MMSWidgetClass::initSelBgImageName() {
	MMSTHEMECLASS_INIT_STRING(selbgimagename);
}

void MMSWidgetClass::freeSelBgImageName() {
	MMSTHEMECLASS_FREE_STRING(selbgimagename);
}

bool MMSWidgetClass::isSelBgImageName() {
	MMSTHEMECLASS_ISSET(selbgimagename);
}

void MMSWidgetClass::unsetSelBgImageName() {
	MMSTHEMECLASS_UNSET(selbgimagename);
}

void MMSWidgetClass::setSelBgImageName(const string &selbgimagename) {
	MMSTHEMECLASS_SET_STRING(selbgimagename);
}

bool MMSWidgetClass::getSelBgImageName(string &selbgimagename) {
	MMSTHEMECLASS_GET_STRING(selbgimagename);
}

void MMSWidgetClass::initBgImagePath_p() {
	MMSTHEMECLASS_INIT_STRING(bgimagepath_p);
}

void MMSWidgetClass::freeBgImagePath_p() {
	MMSTHEMECLASS_FREE_STRING(bgimagepath_p);
}

bool MMSWidgetClass::isBgImagePath_p() {
	MMSTHEMECLASS_ISSET(bgimagepath_p);
}

void MMSWidgetClass::unsetBgImagePath_p() {
	MMSTHEMECLASS_UNSET(bgimagepath_p);
}

void MMSWidgetClass::setBgImagePath_p(const string &bgimagepath_p) {
	MMSTHEMECLASS_SET_STRING(bgimagepath_p);
}

bool MMSWidgetClass::getBgImagePath_p(string &bgimagepath_p) {
	MMSTHEMECLASS_GET_STRING(bgimagepath_p);
}

void MMSWidgetClass::initBgImageName_p() {
	MMSTHEMECLASS_INIT_STRING(bgimagename_p);
}

void MMSWidgetClass::freeBgImageName_p() {
	MMSTHEMECLASS_FREE_STRING(bgimagename_p);
}

bool MMSWidgetClass::isBgImageName_p() {
	MMSTHEMECLASS_ISSET(bgimagename_p);
}

void MMSWidgetClass::unsetBgImageName_p() {
	MMSTHEMECLASS_UNSET(bgimagename_p);
}

void MMSWidgetClass::setBgImageName_p(const string &bgimagename_p) {
	MMSTHEMECLASS_SET_STRING(bgimagename_p);
}

bool MMSWidgetClass::getBgImageName_p(string &bgimagename_p) {
	MMSTHEMECLASS_GET_STRING(bgimagename_p);
}

void MMSWidgetClass::initSelBgImagePath_p() {
	MMSTHEMECLASS_INIT_STRING(selbgimagepath_p);
}

void MMSWidgetClass::freeSelBgImagePath_p() {
	MMSTHEMECLASS_FREE_STRING(selbgimagepath_p);
}

bool MMSWidgetClass::isSelBgImagePath_p() {
	MMSTHEMECLASS_ISSET(selbgimagepath_p);
}

void MMSWidgetClass::unsetSelBgImagePath_p() {
	MMSTHEMECLASS_UNSET(selbgimagepath_p);
}

void MMSWidgetClass::setSelBgImagePath_p(const string &selbgimagepath_p) {
	MMSTHEMECLASS_SET_STRING(selbgimagepath_p);
}

bool MMSWidgetClass::getSelBgImagePath_p(string &selbgimagepath_p) {
	MMSTHEMECLASS_GET_STRING(selbgimagepath_p);
}

void MMSWidgetClass::initSelBgImageName_p() {
	MMSTHEMECLASS_INIT_STRING(selbgimagename_p);
}

void MMSWidgetClass::freeSelBgImageName_p() {
	MMSTHEMECLASS_FREE_STRING(selbgimagename_p);
}

bool MMSWidgetClass::isSelBgImageName_p() {
	MMSTHEMECLASS_ISSET(selbgimagename_p);
}

void MMSWidgetClass::unsetSelBgImageName_p() {
	MMSTHEMECLASS_UNSET(selbgimagename_p);
}

void MMSWidgetClass::setSelBgImageName_p(const string &selbgimagename_p) {
	MMSTHEMECLASS_SET_STRING(selbgimagename_p);
}

bool MMSWidgetClass::getSelBgImageName_p(string &selbgimagename_p) {
	MMSTHEMECLASS_GET_STRING(selbgimagename_p);
}


void MMSWidgetClass::initBgImagePath_i() {
	MMSTHEMECLASS_INIT_STRING(bgimagepath_i);
}

void MMSWidgetClass::freeBgImagePath_i() {
	MMSTHEMECLASS_FREE_STRING(bgimagepath_i);
}

bool MMSWidgetClass::isBgImagePath_i() {
	MMSTHEMECLASS_ISSET(bgimagepath_i);
}

void MMSWidgetClass::unsetBgImagePath_i() {
	MMSTHEMECLASS_UNSET(bgimagepath_i);
}

void MMSWidgetClass::setBgImagePath_i(const string &bgimagepath_i) {
	MMSTHEMECLASS_SET_STRING(bgimagepath_i);
}

bool MMSWidgetClass::getBgImagePath_i(string &bgimagepath_i) {
	MMSTHEMECLASS_GET_STRING(bgimagepath_i);
}

void MMSWidgetClass::initBgImageName_i() {
	MMSTHEMECLASS_INIT_STRING(bgimagename_i);
}

void MMSWidgetClass::freeBgImageName_i() {
	MMSTHEMECLASS_FREE_STRING(bgimagename_i);
}

bool MMSWidgetClass::isBgImageName_i() {
	MMSTHEMECLASS_ISSET(bgimagename_i);
}

void MMSWidgetClass::unsetBgImageName_i() {
	MMSTHEMECLASS_UNSET(bgimagename_i);
}

void MMSWidgetClass::setBgImageName_i(const string &bgimagename_i) {
	MMSTHEMECLASS_SET_STRING(bgimagename_i);
}

bool MMSWidgetClass::getBgImageName_i(string &bgimagename_i) {
	MMSTHEMECLASS_GET_STRING(bgimagename_i);
}

void MMSWidgetClass::initSelBgImagePath_i() {
	MMSTHEMECLASS_INIT_STRING(selbgimagepath_i);
}

void MMSWidgetClass::freeSelBgImagePath_i() {
	MMSTHEMECLASS_FREE_STRING(selbgimagepath_i);
}

bool MMSWidgetClass::isSelBgImagePath_i() {
	MMSTHEMECLASS_ISSET(selbgimagepath_i);
}

void MMSWidgetClass::unsetSelBgImagePath_i() {
	MMSTHEMECLASS_UNSET(selbgimagepath_i);
}

void MMSWidgetClass::setSelBgImagePath_i(const string &selbgimagepath_i) {
	MMSTHEMECLASS_SET_STRING(selbgimagepath_i);
}

bool MMSWidgetClass::getSelBgImagePath_i(string &selbgimagepath_i) {
	MMSTHEMECLASS_GET_STRING(selbgimagepath_i);
}

void MMSWidgetClass::initSelBgImageName_i() {
	MMSTHEMECLASS_INIT_STRING(selbgimagename_i);
}

void MMSWidgetClass::freeSelBgImageName_i() {
	MMSTHEMECLASS_FREE_STRING(selbgimagename_i);
}

bool MMSWidgetClass::isSelBgImageName_i() {
	MMSTHEMECLASS_ISSET(selbgimagename_i);
}

void MMSWidgetClass::unsetSelBgImageName_i() {
	MMSTHEMECLASS_UNSET(selbgimagename_i);
}

void MMSWidgetClass::setSelBgImageName_i(const string &selbgimagename_i) {
	MMSTHEMECLASS_SET_STRING(selbgimagename_i);
}

bool MMSWidgetClass::getSelBgImageName_i(string &selbgimagename_i) {
	MMSTHEMECLASS_GET_STRING(selbgimagename_i);
}


void MMSWidgetClass::initMargin() {
	MMSTHEMECLASS_INIT_BASIC(margin);
}

void MMSWidgetClass::freeMargin() {
	MMSTHEMECLASS_FREE_BASIC(margin);
}

bool MMSWidgetClass::isMargin() {
	MMSTHEMECLASS_ISSET(margin);
}

void MMSWidgetClass::unsetMargin() {
	MMSTHEMECLASS_UNSET(margin);
}

void MMSWidgetClass::setMargin(unsigned int margin) {
	MMSTHEMECLASS_SET_BASIC(margin);
}

bool MMSWidgetClass::getMargin(unsigned int &margin) {
	MMSTHEMECLASS_GET_BASIC(margin);
}

void MMSWidgetClass::initFocusable() {
    this->id.focusable = false;
    MMSTHEMECLASS_INIT_BASIC(focusable);
}

void MMSWidgetClass::freeFocusable() {
    this->id.focusable = false;
    MMSTHEMECLASS_FREE_BASIC(focusable);
}

bool MMSWidgetClass::isFocusable() {
	MMSTHEMECLASS_ISSET(focusable);
}

void MMSWidgetClass::unsetFocusable() {
    this->id.focusable = false;
    MMSTHEMECLASS_UNSET(focusable);
}

void MMSWidgetClass::setFocusable(bool focusable) {
	MMSTHEMECLASS_SET_BASIC(focusable);
}

bool MMSWidgetClass::getFocusable(bool &focusable) {
	MMSTHEMECLASS_GET_BASIC(focusable);
}

void MMSWidgetClass::initSelectable() {
    this->id.selectable = false;
    MMSTHEMECLASS_INIT_BASIC(selectable);
}

void MMSWidgetClass::freeSelectable() {
    this->id.selectable = false;
    MMSTHEMECLASS_FREE_BASIC(selectable);
}

bool MMSWidgetClass::isSelectable() {
	MMSTHEMECLASS_ISSET(selectable);
}

void MMSWidgetClass::unsetSelectable() {
    this->id.selectable = false;
    MMSTHEMECLASS_UNSET(selectable);
}

void MMSWidgetClass::setSelectable(bool selectable) {
	MMSTHEMECLASS_SET_BASIC(selectable);
}

bool MMSWidgetClass::getSelectable(bool &selectable) {
	MMSTHEMECLASS_GET_BASIC(selectable);
}




void MMSWidgetClass::initUpArrow() {
	MMSTHEMECLASS_INIT_STRING(uparrow);
}

void MMSWidgetClass::freeUpArrow() {
	MMSTHEMECLASS_FREE_STRING(uparrow);
}

bool MMSWidgetClass::isUpArrow() {
	MMSTHEMECLASS_ISSET(uparrow);
}

void MMSWidgetClass::unsetUpArrow() {
	MMSTHEMECLASS_UNSET(uparrow);
}

void MMSWidgetClass::setUpArrow(const string &uparrow) {
	MMSTHEMECLASS_SET_STRING(uparrow);
}

bool MMSWidgetClass::getUpArrow(string &uparrow) {
	MMSTHEMECLASS_GET_STRING(uparrow);
}

void MMSWidgetClass::initDownArrow() {
	MMSTHEMECLASS_INIT_STRING(downarrow);
}

void MMSWidgetClass::freeDownArrow() {
	MMSTHEMECLASS_FREE_STRING(downarrow);
}

bool MMSWidgetClass::isDownArrow() {
	MMSTHEMECLASS_ISSET(downarrow);
}

void MMSWidgetClass::unsetDownArrow() {
	MMSTHEMECLASS_UNSET(downarrow);
}

void MMSWidgetClass::setDownArrow(const string &downarrow) {
	MMSTHEMECLASS_SET_STRING(downarrow);
}

bool MMSWidgetClass::getDownArrow(string &downarrow) {
	MMSTHEMECLASS_GET_STRING(downarrow);
}

void MMSWidgetClass::initLeftArrow() {
	MMSTHEMECLASS_INIT_STRING(leftarrow);
}

void MMSWidgetClass::freeLeftArrow() {
	MMSTHEMECLASS_FREE_STRING(leftarrow);
}

bool MMSWidgetClass::isLeftArrow() {
	MMSTHEMECLASS_ISSET(leftarrow);
}

void MMSWidgetClass::unsetLeftArrow() {
	MMSTHEMECLASS_UNSET(leftarrow);
}

void MMSWidgetClass::setLeftArrow(const string &leftarrow) {
	MMSTHEMECLASS_SET_STRING(leftarrow);
}

bool MMSWidgetClass::getLeftArrow(string &leftarrow) {
	MMSTHEMECLASS_GET_STRING(leftarrow);
}

void MMSWidgetClass::initRightArrow() {
	MMSTHEMECLASS_INIT_STRING(rightarrow);
}

void MMSWidgetClass::freeRightArrow() {
	MMSTHEMECLASS_FREE_STRING(rightarrow);
}

bool MMSWidgetClass::isRightArrow() {
	MMSTHEMECLASS_ISSET(rightarrow);
}

void MMSWidgetClass::unsetRightArrow() {
	MMSTHEMECLASS_UNSET(rightarrow);
}

void MMSWidgetClass::setRightArrow(const string &rightarrow) {
	MMSTHEMECLASS_SET_STRING(rightarrow);
}

bool MMSWidgetClass::getRightArrow(string &rightarrow) {
	MMSTHEMECLASS_GET_STRING(rightarrow);
}



void MMSWidgetClass::initData() {
	MMSTHEMECLASS_INIT_STRING(data);
}

void MMSWidgetClass::freeData() {
	MMSTHEMECLASS_FREE_STRING(data);
}

bool MMSWidgetClass::isData() {
	MMSTHEMECLASS_ISSET(data);
}

void MMSWidgetClass::unsetData() {
	MMSTHEMECLASS_UNSET(data);
}

void MMSWidgetClass::setData(const string &data) {
	MMSTHEMECLASS_SET_STRING(data);
}

bool MMSWidgetClass::getData(string &data) {
	MMSTHEMECLASS_GET_STRING(data);
}

void MMSWidgetClass::initNavigateUp() {
	MMSTHEMECLASS_INIT_STRING(navigateup);
}

void MMSWidgetClass::freeNavigateUp() {
	MMSTHEMECLASS_FREE_STRING(navigateup);
}

bool MMSWidgetClass::isNavigateUp() {
	MMSTHEMECLASS_ISSET(navigateup);
}

void MMSWidgetClass::unsetNavigateUp() {
	MMSTHEMECLASS_UNSET(navigateup);
}

void MMSWidgetClass::setNavigateUp(const string &navigateup) {
	MMSTHEMECLASS_SET_STRING(navigateup);
}

bool MMSWidgetClass::getNavigateUp(string &navigateup) {
	MMSTHEMECLASS_GET_STRING(navigateup);
}

void MMSWidgetClass::initNavigateDown() {
	MMSTHEMECLASS_INIT_STRING(navigatedown);
}

void MMSWidgetClass::freeNavigateDown() {
	MMSTHEMECLASS_FREE_STRING(navigatedown);
}

bool MMSWidgetClass::isNavigateDown() {
	MMSTHEMECLASS_ISSET(navigatedown);
}

void MMSWidgetClass::unsetNavigateDown() {
	MMSTHEMECLASS_UNSET(navigatedown);
}

void MMSWidgetClass::setNavigateDown(const string &navigatedown) {
	MMSTHEMECLASS_SET_STRING(navigatedown);
}

bool MMSWidgetClass::getNavigateDown(string &navigatedown) {
	MMSTHEMECLASS_GET_STRING(navigatedown);
}

void MMSWidgetClass::initNavigateLeft() {
	MMSTHEMECLASS_INIT_STRING(navigateleft);
}

void MMSWidgetClass::freeNavigateLeft() {
	MMSTHEMECLASS_FREE_STRING(navigateleft);
}

bool MMSWidgetClass::isNavigateLeft() {
	MMSTHEMECLASS_ISSET(navigateleft);
}

void MMSWidgetClass::unsetNavigateLeft() {
	MMSTHEMECLASS_UNSET(navigateleft);
}

void MMSWidgetClass::setNavigateLeft(const string &navigateleft) {
	MMSTHEMECLASS_SET_STRING(navigateleft);
}

bool MMSWidgetClass::getNavigateLeft(string &navigateleft) {
	MMSTHEMECLASS_GET_STRING(navigateleft);
}

void MMSWidgetClass::initNavigateRight() {
	MMSTHEMECLASS_INIT_STRING(navigateright);
}

void MMSWidgetClass::freeNavigateRight() {
	MMSTHEMECLASS_FREE_STRING(navigateright);
}

bool MMSWidgetClass::isNavigateRight() {
	MMSTHEMECLASS_ISSET(navigateright);
}

void MMSWidgetClass::unsetNavigateRight() {
	MMSTHEMECLASS_UNSET(navigateright);
}

void MMSWidgetClass::setNavigateRight(const string &navigateright) {
	MMSTHEMECLASS_SET_STRING(navigateright);
}

bool MMSWidgetClass::getNavigateRight(string &navigateright) {
	MMSTHEMECLASS_GET_STRING(navigateright);
}


void MMSWidgetClass::initVSlider() {
	MMSTHEMECLASS_INIT_STRING(vslider);
}

void MMSWidgetClass::freeVSlider() {
	MMSTHEMECLASS_FREE_STRING(vslider);
}

bool MMSWidgetClass::isVSlider() {
	MMSTHEMECLASS_ISSET(vslider);
}

void MMSWidgetClass::unsetVSlider() {
	MMSTHEMECLASS_UNSET(vslider);
}

void MMSWidgetClass::setVSlider(const string &vslider) {
	MMSTHEMECLASS_SET_STRING(vslider);
}

bool MMSWidgetClass::getVSlider(string &vslider) {
	MMSTHEMECLASS_GET_STRING(vslider);
}

void MMSWidgetClass::initHSlider() {
	MMSTHEMECLASS_INIT_STRING(hslider);
}

void MMSWidgetClass::freeHSlider() {
	MMSTHEMECLASS_FREE_STRING(hslider);
}

bool MMSWidgetClass::isHSlider() {
	MMSTHEMECLASS_ISSET(hslider);
}

void MMSWidgetClass::unsetHSlider() {
	MMSTHEMECLASS_UNSET(hslider);
}

void MMSWidgetClass::setHSlider(const string &hslider) {
	MMSTHEMECLASS_SET_STRING(hslider);
}

bool MMSWidgetClass::getHSlider(string &hslider) {
	MMSTHEMECLASS_GET_STRING(hslider);
}

bool MMSWidgetClass::isImagesOnDemand() {
	MMSTHEMECLASS_ISSET(imagesondemand);
}

void MMSWidgetClass::initImagesOnDemand() {
	MMSTHEMECLASS_INIT_BASIC(imagesondemand);
}

void MMSWidgetClass::freeImagesOnDemand() {
	MMSTHEMECLASS_FREE_BASIC(imagesondemand);
}

void MMSWidgetClass::unsetImagesOnDemand() {
	MMSTHEMECLASS_UNSET(imagesondemand);
}

void MMSWidgetClass::setImagesOnDemand(bool imagesondemand) {
	MMSTHEMECLASS_SET_BASIC(imagesondemand);
}

bool MMSWidgetClass::getImagesOnDemand(bool &imagesondemand) {
	MMSTHEMECLASS_GET_BASIC(imagesondemand);
}

void MMSWidgetClass::initBlend() {
	MMSTHEMECLASS_INIT_BASIC(blend);
}

void MMSWidgetClass::freeBlend() {
	MMSTHEMECLASS_FREE_BASIC(blend);
}

bool MMSWidgetClass::isBlend() {
	MMSTHEMECLASS_ISSET(blend);
}

void MMSWidgetClass::unsetBlend() {
	MMSTHEMECLASS_UNSET(blend);
}

void MMSWidgetClass::setBlend(unsigned int blend) {
	MMSTHEMECLASS_SET_BASIC(blend);
}

bool MMSWidgetClass::getBlend(unsigned int &blend) {
	MMSTHEMECLASS_GET_BASIC(blend);
}

void MMSWidgetClass::initBlendFactor() {
	MMSTHEMECLASS_INIT_BASIC(blendfactor);
}

void MMSWidgetClass::freeBlendFactor() {
	MMSTHEMECLASS_FREE_BASIC(blendfactor);
}

bool MMSWidgetClass::isBlendFactor() {
	MMSTHEMECLASS_ISSET(blendfactor);
}

void MMSWidgetClass::unsetBlendFactor() {
	MMSTHEMECLASS_UNSET(blendfactor);
}

void MMSWidgetClass::setBlendFactor(double blendfactor) {
	MMSTHEMECLASS_SET_BASIC(blendfactor);
}

bool MMSWidgetClass::getBlendFactor(double &blendfactor) {
	MMSTHEMECLASS_GET_BASIC(blendfactor);
}

