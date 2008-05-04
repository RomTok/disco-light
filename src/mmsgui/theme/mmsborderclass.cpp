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

#include "mmsgui/theme/mmsborderclass.h"

//store attribute descriptions here 
TAFF_ATTRDESC MMSGUI_BORDER_ATTR_I[] = MMSGUI_BORDER_ATTR_INIT;

//address attribute names
#define GETATTRNAME(aname) MMSGUI_BORDER_ATTR_I[MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_##aname].name
#define ISATTRNAME(aname) (strcmp(attrname, GETATTRNAME(aname))==0)


MMSBorderClass::MMSBorderClass() {
    initColor();
    initSelColor();

    initImagePath();
    initImageNames();
    initSelImagePath();
    initSelImageNames();

    initThickness();
    initMargin();
    initRCorners();
}

MMSBorderClass::~MMSBorderClass() {
    freeColor();
    freeSelColor();

    freeImagePath();
    freeImageNames();
    freeSelImagePath();
    freeSelImageNames();

    freeThickness();
    freeMargin();
    freeRCorners();
}

MMSBorderClass &MMSBorderClass::operator=(const MMSBorderClass &c) { 
	if (this != &c) {
		/* copy internal fix data area */
		this->id = c.id;

		/* copy external data */
		memset(&(this->ed), 0, sizeof(this->ed));
		if (c.id.isimagepath)
			this->ed.imagepath = new string(*c.ed.imagepath);
		if (c.id.isselimagepath)
			this->ed.selimagepath = new string(*c.ed.selimagepath);
		if (c.id.isimagenames)
			for (int i = 0; i < 8; i++)
				this->ed.imagenames[i] = new string(*c.ed.imagenames[i]);
		if (c.id.isselimagenames)
			for (int i = 0; i < 8; i++)
				this->ed.selimagenames[i] = new string(*c.ed.selimagenames[i]);
	}
	return *this;
}

void MMSBorderClass::unsetAll() {
    unsetColor();
    unsetSelColor();

    unsetImagePath();
    unsetImageNames();
    unsetSelImagePath();
    unsetSelImageNames();

    unsetThickness();
    unsetMargin();
    unsetRCorners();
}

void MMSBorderClass::setAttributesFromXMLNode(MMSTaffFile *tafff, string prefix, string path) {
    DFBColor color;
    bool class_set = false;

    if (prefix == "") {
		startTAFFScan
		{
	        switch (attrid) {
			case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
	            class_set = true;
				break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_color:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) getColor(color);
	            if (getColorFromString(attrval_str, &color))    
	                setColor(color);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_color_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) getColor(color);
	            color.a = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_color_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) getColor(color);
	            color.r = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_color_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) getColor(color);
	            color.g = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_color_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) getColor(color);
	            color.b = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selcolor:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) getSelColor(color);
	            if (getColorFromString(attrval_str, &color))    
	                setSelColor(color);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selcolor_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) getSelColor(color);
	            color.a = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selcolor_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) getSelColor(color);
	            color.r = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selcolor_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) getSelColor(color);
	            color.g = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selcolor_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) getSelColor(color);
	            color.b = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_image_path:
	            if (*attrval_str)
	                setImagePath(attrval_str);
	            else
	                setImagePath(path);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_image_top_left:
	            setImageNames(0, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_image_top:
	            setImageNames(1, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_image_top_right:
	            setImageNames(2, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_image_right:
	            setImageNames(3, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_image_bottom_right:
	            setImageNames(4, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_image_bottom:
	            setImageNames(5, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_image_bottom_left:
	            setImageNames(6, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_image_left:
	            setImageNames(7, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selimage_path:
	            if (*attrval_str)
	                setSelImagePath(attrval_str);
	            else
	                setSelImagePath(path);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selimage_top_left:
	            setSelImageNames(0, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selimage_top:
	            setSelImageNames(1, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selimage_top_right:
	            setSelImageNames(2, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selimage_right:
	            setSelImageNames(3, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selimage_bottom_right:
	            setSelImageNames(4, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selimage_bottom:
	            setSelImageNames(5, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selimage_bottom_left:
	            setSelImageNames(6, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_selimage_left:
	            setSelImageNames(7, attrval_str);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_thickness:
	            setThickness(attrval_int);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_margin:
	            setMargin(attrval_int);
	            break;
			case MMSGUI_BORDER_ATTR::MMSGUI_BORDER_ATTR_IDS_border_rcorners:
	            setRCorners((attrval_int) ? true : false);
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
            if (ISATTRNAME(border_color)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) getColor(color);
	            if (getColorFromString(attrval_str, &color))    
	                setColor(color);
            }
            else
            if (ISATTRNAME(border_color_a)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) getColor(color);
	            color.a = attrval_int;
	            setColor(color);
            }
            else
            if (ISATTRNAME(border_color_r)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) getColor(color);
	            color.r = attrval_int;
	            setColor(color);
            }
            else
            if (ISATTRNAME(border_color_g)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) getColor(color);
	            color.g = attrval_int;
	            setColor(color);
            }
            else
            if (ISATTRNAME(border_color_b)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) getColor(color);
	            color.b = attrval_int;
	            setColor(color);
            }
            else
            if (ISATTRNAME(border_selcolor)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) getSelColor(color);
	            if (getColorFromString(attrval_str, &color))    
	                setSelColor(color);
            }
            else
            if (ISATTRNAME(border_selcolor_a)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) getSelColor(color);
	            color.a = attrval_int;
	            setSelColor(color);
            }
            else
            if (ISATTRNAME(border_selcolor_r)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) getSelColor(color);
	            color.r = attrval_int;
	            setSelColor(color);
            }
            else
            if (ISATTRNAME(border_selcolor_g)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) getSelColor(color);
	            color.g = attrval_int;
	            setSelColor(color);
            }
            else
            if (ISATTRNAME(border_selcolor_b)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) getSelColor(color);
	            color.b = attrval_int;
	            setSelColor(color);
            }
            else
            if (ISATTRNAME(border_image_path)) { 
	            if (*attrval_str)
	                setImagePath(attrval_str);
	            else
	                setImagePath(path);
            }
            else
            if (ISATTRNAME(border_image_top_left)) { 
	            setImageNames(0, attrval_str);
            }
            else
            if (ISATTRNAME(border_image_top)) { 
	            setImageNames(1, attrval_str);
            }
            else
            if (ISATTRNAME(border_image_top_right)) { 
	            setImageNames(2, attrval_str);
            }
            else
            if (ISATTRNAME(border_image_right)) { 
	            setImageNames(3, attrval_str);
            }
            else
            if (ISATTRNAME(border_image_bottom_right)) { 
	            setImageNames(4, attrval_str);
            }
            else
            if (ISATTRNAME(border_image_bottom)) { 
	            setImageNames(5, attrval_str);
            }
            else
            if (ISATTRNAME(border_image_bottom_left)) { 
	            setImageNames(6, attrval_str);
            }
            else
            if (ISATTRNAME(border_image_left)) { 
	            setImageNames(7, attrval_str);
            }
            else
            if (ISATTRNAME(border_selimage_path)) { 
	            if (*attrval_str)
	                setSelImagePath(attrval_str);
	            else
	                setSelImagePath(path);
            }
            else
            if (ISATTRNAME(border_selimage_top_left)) { 
	            setSelImageNames(0, attrval_str);
            }
            else
            if (ISATTRNAME(border_selimage_top)) { 
	            setSelImageNames(1, attrval_str);
            }
            else
            if (ISATTRNAME(border_selimage_top_right)) { 
	            setSelImageNames(2, attrval_str);
            }
            else
            if (ISATTRNAME(border_selimage_right)) { 
	            setSelImageNames(3, attrval_str);
            }
            else
            if (ISATTRNAME(border_selimage_bottom_right)) { 
	            setSelImageNames(4, attrval_str);
            }
            else
            if (ISATTRNAME(border_selimage_bottom)) { 
	            setSelImageNames(5, attrval_str);
            }
            else
            if (ISATTRNAME(border_selimage_bottom_left)) { 
	            setSelImageNames(6, attrval_str);
            }
            else
            if (ISATTRNAME(border_selimage_left)) { 
	            setSelImageNames(7, attrval_str);
            }
            else
            if (ISATTRNAME(border_thickness)) { 
	            setThickness(attrval_int);
            }
            else
            if (ISATTRNAME(border_margin)) { 
	            setMargin(attrval_int);
            }
            else
            if (ISATTRNAME(border_rcorners)) { 
	            setRCorners((attrval_int) ? true : false);
			}
    	}
    	endTAFFScan_WITHOUT_ID
    }
    
    if ((!isImagePath())&&(!class_set)&&(path!=""))
        setImagePath(path);
    if ((!isSelImagePath())&&(!class_set)&&(path!=""))
        setSelImagePath(path);
}


void MMSBorderClass::initColor() {
	MMSTHEMECLASS_INIT_BASIC(color);
}

void MMSBorderClass::freeColor() {
	MMSTHEMECLASS_FREE_BASIC(color);
}

bool MMSBorderClass::isColor() {
	MMSTHEMECLASS_ISSET(color);
}

void MMSBorderClass::unsetColor() {
	MMSTHEMECLASS_UNSET(color);
}

void MMSBorderClass::setColor(const DFBColor &color) {
	MMSTHEMECLASS_SET_BASIC(color);
}

bool MMSBorderClass::getColor(DFBColor &color) {
	MMSTHEMECLASS_GET_BASIC(color);
}


void MMSBorderClass::initSelColor() {
	MMSTHEMECLASS_INIT_BASIC(selcolor);
}

void MMSBorderClass::freeSelColor() {
	MMSTHEMECLASS_FREE_BASIC(selcolor);
}

bool MMSBorderClass::isSelColor() {
	MMSTHEMECLASS_ISSET(selcolor);
}

void MMSBorderClass::unsetSelColor() {
	MMSTHEMECLASS_UNSET(selcolor);
}

void MMSBorderClass::setSelColor(const DFBColor &selcolor) {
	MMSTHEMECLASS_SET_BASIC(selcolor);
}

bool MMSBorderClass::getSelColor(DFBColor &selcolor) {
	MMSTHEMECLASS_GET_BASIC(selcolor);
}



void MMSBorderClass::initImagePath() {
	MMSTHEMECLASS_INIT_STRING(imagepath);
}

void MMSBorderClass::freeImagePath() {
	MMSTHEMECLASS_FREE_STRING(imagepath);
}

bool MMSBorderClass::isImagePath() {
	MMSTHEMECLASS_ISSET(imagepath);
}

void MMSBorderClass::unsetImagePath() {
	MMSTHEMECLASS_UNSET(imagepath);
}

void MMSBorderClass::setImagePath(const string &imagepath) {
	MMSTHEMECLASS_SET_STRING(imagepath);
}

bool MMSBorderClass::getImagePath(string &imagepath) {
	MMSTHEMECLASS_GET_STRING(imagepath);
}




void MMSBorderClass::initImageNames() {
	MMSTHEMECLASS_INIT_STRINGS(imagenames,8);
}

void MMSBorderClass::freeImageNames() {
	MMSTHEMECLASS_FREE_STRINGS(imagenames,8);
}

bool MMSBorderClass::isImageNames() {
	MMSTHEMECLASS_ISSET(imagenames);
}

void MMSBorderClass::unsetImageNames() {
	MMSTHEMECLASS_UNSET_STRINGS(imagenames,8);
}

void MMSBorderClass::setImageNames(const string &imagename_1, const string &imagename_2, const string &imagename_3, const string &imagename_4,
								   const string &imagename_5, const string &imagename_6, const string &imagename_7, const string &imagename_8) {
	MMSTHEMECLASS_SET_STRINGS(imagenames,0,imagename_1);
	MMSTHEMECLASS_SET_STRINGS(imagenames,1,imagename_2);
	MMSTHEMECLASS_SET_STRINGS(imagenames,2,imagename_3);
	MMSTHEMECLASS_SET_STRINGS(imagenames,3,imagename_4);
	MMSTHEMECLASS_SET_STRINGS(imagenames,4,imagename_5);
	MMSTHEMECLASS_SET_STRINGS(imagenames,5,imagename_6);
	MMSTHEMECLASS_SET_STRINGS(imagenames,6,imagename_7);
	MMSTHEMECLASS_SET_STRINGS(imagenames,7,imagename_8);
}

void MMSBorderClass::setImageNames(unsigned int num, const string &imagename) {
	MMSTHEMECLASS_SET_STRINGS(imagenames,num,imagename);
}

bool MMSBorderClass::getImageNames(unsigned int num, string &imagename) {
	MMSTHEMECLASS_GET_STRINGS(imagenames,num,imagename);
}


void MMSBorderClass::initSelImagePath() {
	MMSTHEMECLASS_INIT_STRING(selimagepath);
}

void MMSBorderClass::freeSelImagePath() {
	MMSTHEMECLASS_FREE_STRING(selimagepath);
}

bool MMSBorderClass::isSelImagePath() {
	MMSTHEMECLASS_ISSET(selimagepath);
}

void MMSBorderClass::unsetSelImagePath() {
	MMSTHEMECLASS_UNSET(selimagepath);
}

void MMSBorderClass::setSelImagePath(const string &selimagepath) {
	MMSTHEMECLASS_SET_STRING(selimagepath);
}

bool MMSBorderClass::getSelImagePath(string &selimagepath) {
	MMSTHEMECLASS_GET_STRING(selimagepath);
}



void MMSBorderClass::initSelImageNames() {
	MMSTHEMECLASS_INIT_STRINGS(selimagenames,8);
}

void MMSBorderClass::freeSelImageNames() {
	MMSTHEMECLASS_FREE_STRINGS(selimagenames,8);
}

bool MMSBorderClass::isSelImageNames() {
	MMSTHEMECLASS_ISSET(selimagenames);
}

void MMSBorderClass::unsetSelImageNames() {
	MMSTHEMECLASS_UNSET_STRINGS(selimagenames,8);
}

void MMSBorderClass::setSelImageNames(const string &selimagename_1, const string &selimagename_2, const string &selimagename_3, const string &selimagename_4,
								      const string &selimagename_5, const string &selimagename_6, const string &selimagename_7, const string &selimagename_8) {
	MMSTHEMECLASS_SET_STRINGS(selimagenames,0,selimagename_1);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,1,selimagename_2);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,2,selimagename_3);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,3,selimagename_4);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,4,selimagename_5);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,5,selimagename_6);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,6,selimagename_7);
	MMSTHEMECLASS_SET_STRINGS(selimagenames,7,selimagename_8);
}

void MMSBorderClass::setSelImageNames(unsigned int num, const string &selimagename) {
	MMSTHEMECLASS_SET_STRINGS(selimagenames,num,selimagename);
}

bool MMSBorderClass::getSelImageNames(unsigned int num, string &selimagename) {
	MMSTHEMECLASS_GET_STRINGS(selimagenames,num,selimagename);
}


void MMSBorderClass::initThickness() {
	MMSTHEMECLASS_INIT_BASIC(thickness);
}

void MMSBorderClass::freeThickness() {
	MMSTHEMECLASS_FREE_BASIC(thickness);
}

bool MMSBorderClass::isThickness() {
	MMSTHEMECLASS_ISSET(thickness);
}

void MMSBorderClass::unsetThickness() {
	MMSTHEMECLASS_UNSET(thickness);
}

void MMSBorderClass::setThickness(unsigned int thickness) {
	MMSTHEMECLASS_SET_BASIC(thickness);
}

bool MMSBorderClass::getThickness(unsigned int &thickness) {
	MMSTHEMECLASS_GET_BASIC(thickness);
}

void MMSBorderClass::initMargin() {
	MMSTHEMECLASS_INIT_BASIC(margin);
}

void MMSBorderClass::freeMargin() {
	MMSTHEMECLASS_FREE_BASIC(margin);
}

bool MMSBorderClass::isMargin() {
	MMSTHEMECLASS_ISSET(margin);
}

void MMSBorderClass::unsetMargin() {
	MMSTHEMECLASS_UNSET(margin);
}

void MMSBorderClass::setMargin(unsigned int margin) {
	MMSTHEMECLASS_SET_BASIC(margin);
}

bool MMSBorderClass::getMargin(unsigned int &margin) {
	MMSTHEMECLASS_GET_BASIC(margin);
}

void MMSBorderClass::initRCorners() {
	MMSTHEMECLASS_INIT_BASIC(rcorners);
}

void MMSBorderClass::freeRCorners() {
	MMSTHEMECLASS_FREE_BASIC(rcorners);
}

bool MMSBorderClass::isRCorners() {
	MMSTHEMECLASS_ISSET(rcorners);
}

void MMSBorderClass::unsetRCorners() {
	MMSTHEMECLASS_UNSET(rcorners);
}

void MMSBorderClass::setRCorners(bool rcorners) {
	MMSTHEMECLASS_SET_BASIC(rcorners);
}

bool MMSBorderClass::getRCorners(bool &rcorners) {
	MMSTHEMECLASS_GET_BASIC(rcorners);
}

