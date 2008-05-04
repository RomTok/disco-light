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

#include "mmsgui/theme/mmsprogressbarclass.h"

//store attribute descriptions here 
TAFF_ATTRDESC MMSGUI_PROGRESSBAR_ATTR_I[] = MMSGUI_PROGRESSBAR_ATTR_INIT;

//address attribute names
#define GETATTRNAME(aname) MMSGUI_PROGRESSBAR_ATTR_I[MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_##aname].name
#define ISATTRNAME(aname) (strcmp(attrname, GETATTRNAME(aname))==0)


MMSProgressBarClass::MMSProgressBarClass() {
    unsetAll();
}

void MMSProgressBarClass::unsetAll() {
    this->className = "";
    unsetColor();
    unsetSelColor();
    unsetProgress();
}

void MMSProgressBarClass::setAttributesFromXMLNode(MMSTaffFile *tafff, string prefix, string path) {
    DFBColor color;

    if (prefix == "") {
		startTAFFScan
		{
	        switch (attrid) {
			case MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_class:
	            setClassName(attrval_str);
				break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_color:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            if (getColorFromString(attrval_str, &color))    
	                setColor(color);
	            break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_color_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.a = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_color_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.r = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_color_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.g = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_color_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.b = attrval_int;
	            setColor(color);
	            break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_selcolor:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            if (getColorFromString(attrval_str, &color))    
	                setSelColor(color);
	            break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_selcolor_a:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.a = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_selcolor_r:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.r = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_selcolor_g:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.g = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_selcolor_b:
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.b = attrval_int;
	            setSelColor(color);
	            break;
			case MMSGUI_PROGRESSBAR_ATTR::MMSGUI_PROGRESSBAR_ATTR_IDS_progress:
	            setProgress(attrval_int);
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
            if (ISATTRNAME(color)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            if (getColorFromString(attrval_str, &color))    
	                setColor(color);
    		}
            else
            if (ISATTRNAME(color_a)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.a = attrval_int;
	            setColor(color);
    		}
            else
            if (ISATTRNAME(color_r)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.r = attrval_int;
	            setColor(color);
    		}
            else
            if (ISATTRNAME(color_g)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.g = attrval_int;
	            setColor(color);
    		}
            else
            if (ISATTRNAME(color_b)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isColor()) color = getColor();
	            color.b = attrval_int;
	            setColor(color);
    		}
            else
            if (ISATTRNAME(selcolor)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            if (getColorFromString(attrval_str, &color))    
	                setSelColor(color);
    		}
            else
            if (ISATTRNAME(selcolor_a)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.a = attrval_int;
	            setSelColor(color);
    		}
            else
            if (ISATTRNAME(selcolor_r)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.r = attrval_int;
	            setSelColor(color);
    		}
            else
            if (ISATTRNAME(selcolor_g)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.g = attrval_int;
	            setSelColor(color);
    		}
            else
            if (ISATTRNAME(selcolor_b)) { 
				color.a = color.r = color.g = color.b = 0;
	            if (isSelColor()) color = getSelColor();
	            color.b = attrval_int;
	            setSelColor(color);
    		}
            else
            if (ISATTRNAME(progress)) { 
	            setProgress(attrval_int);
			}
    	}
    	endTAFFScan_WITHOUT_ID
    }
}

void MMSProgressBarClass::setClassName(string className) {
    this->className = className;
}

string MMSProgressBarClass::getClassName() {
    return this->className;
}

bool MMSProgressBarClass::isColor() {
    return this->iscolor;
}

void MMSProgressBarClass::setColor(DFBColor color) {
    this->color = color;
    this->iscolor = true;
}

void MMSProgressBarClass::unsetColor() {
    this->iscolor = false;
}

DFBColor MMSProgressBarClass::getColor() {
    return this->color;
}

bool MMSProgressBarClass::isSelColor() {
    return this->isselcolor;
}

void MMSProgressBarClass::setSelColor(DFBColor selcolor) {
    this->selcolor = selcolor;
    this->isselcolor = true;
}

void MMSProgressBarClass::unsetSelColor() {
    this->isselcolor = false;
}

DFBColor MMSProgressBarClass::getSelColor() {
    return this->selcolor;
}

bool MMSProgressBarClass::isProgress() {
    return this->isprogress;
}

void MMSProgressBarClass::setProgress(unsigned int progress) {
    this->progress = progress;
    this->isprogress = true;
}

void MMSProgressBarClass::unsetProgress() {
    this->isprogress = false;
}

unsigned int MMSProgressBarClass::getProgress() {
    return this->progress;
}
