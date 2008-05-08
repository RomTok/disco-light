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

#include "mmsgui/theme/mmssliderclass.h"

//store attribute descriptions here 
TAFF_ATTRDESC MMSGUI_SLIDER_ATTR_I[] = MMSGUI_SLIDER_ATTR_INIT;

//address attribute names
#define GETATTRNAME(aname) MMSGUI_SLIDER_ATTR_I[MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_##aname].name
#define ISATTRNAME(aname) (strcmp(attrname, GETATTRNAME(aname))==0)


MMSSliderClass::MMSSliderClass() {
    unsetAll();
}

void MMSSliderClass::unsetAll() {
    this->className = "";
    unsetImagePath();
    unsetImageName();
    unsetSelImagePath();
    unsetSelImageName();
    unsetImagePath_p();
    unsetImageName_p();
    unsetSelImagePath_p();
    unsetSelImageName_p();
    unsetImagePath_i();
    unsetImageName_i();
    unsetSelImagePath_i();
    unsetSelImageName_i();
    unsetPosition();
}

void MMSSliderClass::setAttributesFromXMLNode(MMSTaffFile *tafff, string prefix, string path) {
    bool class_set = false;

    if (prefix == "") {
		startTAFFScan
		{
	        switch (attrid) {
			case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
	            setClassName(attrval_str);
	            class_set = true;
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_image:
	            if (*attrval_str)
	                setImagePath("");
	            else
	                setImagePath(path);
	            setImageName(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_image_path:
	            if (*attrval_str)
	                setImagePath(attrval_str);
	            else
	                setImagePath(path);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_image_name:
	            setImageName(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_selimage:
	            if (*attrval_str)
	                setSelImagePath("");
	            else
	                setSelImagePath(path);
	            setSelImageName(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_selimage_path:
	            if (*attrval_str)
	                setSelImagePath(attrval_str);
	            else
	                setSelImagePath(path);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_selimage_name:
	            setSelImageName(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_image_p:
	            if (*attrval_str)
	                setImagePath_p("");
	            else
	                setImagePath_p(path);
	            setImageName_p(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_image_p_path:
	            if (*attrval_str)
	                setImagePath_p(attrval_str);
	            else
	                setImagePath_p(path);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_image_p_name:
	            setImageName_p(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_selimage_p:
	            if (*attrval_str)
	                setSelImagePath_p("");
	            else
	                setSelImagePath_p(path);
	            setSelImageName_p(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_selimage_p_path:
	            if (*attrval_str)
	                setSelImagePath_p(attrval_str);
	            else
	                setSelImagePath_p(path);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_selimage_p_name:
	            setSelImageName_p(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_image_i:
	            if (*attrval_str)
	                setImagePath_i("");
	            else
	                setImagePath_i(path);
	            setImageName_i(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_image_i_path:
	            if (*attrval_str)
	                setImagePath_i(attrval_str);
	            else
	                setImagePath_i(path);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_image_i_name:
	            setImageName_i(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_selimage_i:
	            if (*attrval_str)
	                setSelImagePath_i("");
	            else
	                setSelImagePath_i(path);
	            setSelImageName_i(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_selimage_i_path:
	            if (*attrval_str)
	                setSelImagePath_i(attrval_str);
	            else
	                setSelImagePath_i(path);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_selimage_i_name:
	            setSelImageName_i(attrval_str);
				break;
			case MMSGUI_SLIDER_ATTR::MMSGUI_SLIDER_ATTR_IDS_position:
	            setPosition(attrval_int);
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
            if (ISATTRNAME(image)) { 
	            if (*attrval_str)
	                setImagePath("");
	            else
	                setImagePath(path);
	            setImageName(attrval_str);
            }
            else
            if (ISATTRNAME(image_path)) { 
	            if (*attrval_str)
	                setImagePath(attrval_str);
	            else
	                setImagePath(path);
            }
            else
            if (ISATTRNAME(image_name)) { 
	            setImageName(attrval_str);
            }
            else
            if (ISATTRNAME(selimage)) { 
	            if (*attrval_str)
	                setSelImagePath("");
	            else
	                setSelImagePath(path);
	            setSelImageName(attrval_str);
            }
            else
            if (ISATTRNAME(selimage_path)) { 
	            if (*attrval_str)
	                setSelImagePath(attrval_str);
	            else
	                setSelImagePath(path);
            }
            else
            if (ISATTRNAME(selimage_name)) { 
	            setSelImageName(attrval_str);
            }
            else
            if (ISATTRNAME(image_p)) { 
	            if (*attrval_str)
	                setImagePath_p("");
	            else
	                setImagePath_p(path);
	            setImageName_p(attrval_str);
            }
            else
            if (ISATTRNAME(image_p_path)) { 
	            if (*attrval_str)
	                setImagePath_p(attrval_str);
	            else
	                setImagePath_p(path);
            }
            else
            if (ISATTRNAME(image_p_name)) { 
	            setImageName_p(attrval_str);
            }
            else
            if (ISATTRNAME(selimage_p)) { 
	            if (*attrval_str)
	                setSelImagePath_p("");
	            else
	                setSelImagePath_p(path);
	            setSelImageName_p(attrval_str);
            }
            else
            if (ISATTRNAME(selimage_p_path)) { 
	            if (*attrval_str)
	                setSelImagePath_p(attrval_str);
	            else
	                setSelImagePath_p(path);
            }
            else
            if (ISATTRNAME(selimage_p_name)) {  
	            setSelImageName_p(attrval_str);
            }
            else
            if (ISATTRNAME(image_i)) { 
	            if (*attrval_str)
	                setImagePath_i("");
	            else
	                setImagePath_i(path);
	            setImageName_i(attrval_str);
            }
            else
            if (ISATTRNAME(image_i_path)) { 
	            if (*attrval_str)
	                setImagePath_i(attrval_str);
	            else
	                setImagePath_i(path);
            }
            else
            if (ISATTRNAME(image_i_name)) { 
	            setImageName_i(attrval_str);
            }
            else
            if (ISATTRNAME(selimage_i)) { 
	            if (*attrval_str)
	                setSelImagePath_i("");
	            else
	                setSelImagePath_i(path);
	            setSelImageName_i(attrval_str);
            }
            else
            if (ISATTRNAME(selimage_i_path)) { 
	            if (*attrval_str)
	                setSelImagePath_i(attrval_str);
	            else
	                setSelImagePath_i(path);
            }
            else
            if (ISATTRNAME(selimage_i_name)) { 
	            setSelImageName_i(attrval_str);
            }
            else
            if (ISATTRNAME(position)) { 
	            setPosition(attrval_int);
			}
    	}
    	endTAFFScan_WITHOUT_ID
    }
    
    if ((!isImagePath())&&(!class_set)&&(path!=""))
        setImagePath(path);
    if ((!isSelImagePath())&&(!class_set)&&(path!=""))
        setSelImagePath(path);
    if ((!isImagePath_p())&&(!class_set)&&(path!=""))
        setImagePath_p(path);
    if ((!isSelImagePath_p())&&(!class_set)&&(path!=""))
        setSelImagePath_p(path);
    if ((!isImagePath_i())&&(!class_set)&&(path!=""))
        setImagePath_i(path);
    if ((!isSelImagePath_i())&&(!class_set)&&(path!=""))
        setSelImagePath_i(path);
}

void MMSSliderClass::setClassName(string className) {
    this->className = className;
}

string MMSSliderClass::getClassName() {
    return this->className;
}

bool MMSSliderClass::isImagePath() {
    return this->isimagepath;
}

void MMSSliderClass::setImagePath(string imagepath) {
    this->imagepath = imagepath;
    this->isimagepath = true;
}

void MMSSliderClass::unsetImagePath() {
    this->isimagepath = false;
}

string MMSSliderClass::getImagePath() {
    return this->imagepath;
}

bool MMSSliderClass::isImageName() {
    return this->isimagename;
}

void MMSSliderClass::setImageName(string imagename) {
    this->imagename = imagename;
    this->isimagename = true;
}

void MMSSliderClass::unsetImageName() {
    this->isimagename = false;
}

string MMSSliderClass::getImageName() {
    return this->imagename;
}

bool MMSSliderClass::isSelImagePath() {
    return this->isselimagepath;
}

void MMSSliderClass::setSelImagePath(string selimagepath) {
    this->selimagepath = selimagepath;
    this->isselimagepath = true;
}

void MMSSliderClass::unsetSelImagePath() {
    this->isselimagepath = false;
}

string MMSSliderClass::getSelImagePath() {
    return this->selimagepath;
}

bool MMSSliderClass::isSelImageName() {
    return this->isselimagename;
}

void MMSSliderClass::setSelImageName(string selimagename) {
    this->selimagename = selimagename;
    this->isselimagename = true;
}

void MMSSliderClass::unsetSelImageName() {
    this->isselimagename = false;
}

string MMSSliderClass::getSelImageName() {
    return this->selimagename;
}

bool MMSSliderClass::isImagePath_p() {
    return this->isimagepath_p;
}

void MMSSliderClass::setImagePath_p(string imagepath_p) {
    this->imagepath_p = imagepath_p;
    this->isimagepath_p = true;
}

void MMSSliderClass::unsetImagePath_p() {
    this->isimagepath_p = false;
}

string MMSSliderClass::getImagePath_p() {
    return this->imagepath_p;
}

bool MMSSliderClass::isImageName_p() {
    return this->isimagename_p;
}

void MMSSliderClass::setImageName_p(string imagename_p) {
    this->imagename_p = imagename_p;
    this->isimagename_p = true;
}

void MMSSliderClass::unsetImageName_p() {
    this->isimagename_p = false;
}

string MMSSliderClass::getImageName_p() {
    return this->imagename_p;
}

bool MMSSliderClass::isSelImagePath_p() {
    return this->isselimagepath_p;
}

void MMSSliderClass::setSelImagePath_p(string selimagepath_p) {
    this->selimagepath_p = selimagepath_p;
    this->isselimagepath_p = true;
}

void MMSSliderClass::unsetSelImagePath_p() {
    this->isselimagepath_p = false;
}

string MMSSliderClass::getSelImagePath_p() {
    return this->selimagepath_p;
}

bool MMSSliderClass::isSelImageName_p() {
    return this->isselimagename_p;
}

void MMSSliderClass::setSelImageName_p(string selimagename_p) {
    this->selimagename_p = selimagename_p;
    this->isselimagename_p = true;
}

void MMSSliderClass::unsetSelImageName_p() {
    this->isselimagename_p = false;
}

string MMSSliderClass::getSelImageName_p() {
    return this->selimagename_p;
}

bool MMSSliderClass::isImagePath_i() {
    return this->isimagepath_i;
}

void MMSSliderClass::setImagePath_i(string imagepath_i) {
    this->imagepath_i = imagepath_i;
    this->isimagepath_i = true;
}

void MMSSliderClass::unsetImagePath_i() {
    this->isimagepath_i = false;
}

string MMSSliderClass::getImagePath_i() {
    return this->imagepath_i;
}

bool MMSSliderClass::isImageName_i() {
    return this->isimagename_i;
}

void MMSSliderClass::setImageName_i(string imagename_i) {
    this->imagename_i = imagename_i;
    this->isimagename_i = true;
}

void MMSSliderClass::unsetImageName_i() {
    this->isimagename_i = false;
}

string MMSSliderClass::getImageName_i() {
    return this->imagename_i;
}

bool MMSSliderClass::isSelImagePath_i() {
    return this->isselimagepath_i;
}

void MMSSliderClass::setSelImagePath_i(string selimagepath_i) {
    this->selimagepath_i = selimagepath_i;
    this->isselimagepath_i = true;
}

void MMSSliderClass::unsetSelImagePath_i() {
    this->isselimagepath_i = false;
}

string MMSSliderClass::getSelImagePath_i() {
    return this->selimagepath_i;
}

bool MMSSliderClass::isSelImageName_i() {
    return this->isselimagename_i;
}

void MMSSliderClass::setSelImageName_i(string selimagename_i) {
    this->selimagename_i = selimagename_i;
    this->isselimagename_i = true;
}

void MMSSliderClass::unsetSelImageName_i() {
    this->isselimagename_i = false;
}

string MMSSliderClass::getSelImageName_i() {
    return this->selimagename_i;
}

bool MMSSliderClass::isPosition() {
    return this->isposition;
}

void MMSSliderClass::setPosition(unsigned int position) {
    this->position = position;
    this->isposition = true;
}

void MMSSliderClass::unsetPosition() {
    this->isposition = false;
}

unsigned int MMSSliderClass::getPosition() {
    return this->position;
}

