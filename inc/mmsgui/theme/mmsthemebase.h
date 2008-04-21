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

#ifndef MMSTHEMEBASE_H_
#define MMSTHEMEBASE_H_

#include "mmstools/mmstools.h"
#include "mmsgui/mmsguitools.h"

#include <libxml/parser.h>
#include <libxml/tree.h>


/* string constants for xml identifiers */
#define XML_ID_DESC         "description"
#define XML_ID_MAINWIN      "mainwindow"
#define XML_ID_POPUPWIN     "popupwindow"
#define XML_ID_ROOTWIN      "rootwindow"
#define XML_ID_CHILDWIN     "childwindow"
#define XML_ID_TEMPLATE     "template"
#define XML_ID_LABEL        "label"
#define XML_ID_IMAGE        "image"
#define XML_ID_BUTTON       "button"
#define XML_ID_PROGRESSBAR  "progressbar"
#define XML_ID_MENU         "menu"
#define XML_ID_MENUITEM     "menuitem"
#define XML_ID_TEXTBOX      "textbox"
#define XML_ID_ARROW        "arrow"
#define XML_ID_SLIDER       "slider"


typedef enum {
    MMSALIGNMENT_NOTSET=0,
    MMSALIGNMENT_CENTER,
    MMSALIGNMENT_LEFT,
    MMSALIGNMENT_RIGHT,
    MMSALIGNMENT_JUSTIFY,
    MMSALIGNMENT_TOP_CENTER,
    MMSALIGNMENT_TOP_LEFT,
    MMSALIGNMENT_TOP_RIGHT,
    MMSALIGNMENT_TOP_JUSTIFY,
    MMSALIGNMENT_BOTTOM_CENTER,
    MMSALIGNMENT_BOTTOM_LEFT,
    MMSALIGNMENT_BOTTOM_RIGHT,
    MMSALIGNMENT_BOTTOM_JUSTIFY
} MMSALIGNMENT;


MMSALIGNMENT getAlignmentFromString(string inputstr);

typedef enum {
    MMSDIRECTION_NOTSET=0,
    MMSDIRECTION_LEFT,
    MMSDIRECTION_RIGHT,
    MMSDIRECTION_UP,
    MMSDIRECTION_DOWN
} MMSDIRECTION;


MMSDIRECTION getDirectionFromString(string inputstr);


#ifdef sfdsdfdfds
#define startXMLScan \
{const xmlpp::Element::AttributeList& attributes = dynamic_cast<const xmlpp::Element*>(node)->get_attributes(); \
    for(xmlpp::Element::AttributeList::const_iterator iterAttrs = attributes.begin(); iterAttrs != attributes.end(); ++iterAttrs) { \
        const xmlpp::Attribute* attribute = *iterAttrs; \
        string attrName = attribute->get_name(); \
        string attrValue = attribute->get_value();

#define endXMLScan }}
#endif


#define startXMLScan \
{xmlAttr *attr = node->properties; \
	for (xmlAttr *cur_attr = attr; cur_attr; cur_attr = cur_attr->next) { \
		string attrName = (string)(char*)(cur_attr->name); \
        string attrValue; \
		xmlChar *xvalue; \
		xvalue = xmlGetProp(node, cur_attr->name); \
    	if (xvalue) { \
    		attrValue = (string)(char*)xvalue; \
    	    xmlFree(xvalue); \
    	}

#define endXMLScan }}



#define getXMLColor \
    if(attrName == "a")    { color.a = atoi(attrValue.c_str()); if (color.a!=255) color.a=0; } \
    else if(attrName == "r") color.r = atoi(attrValue.c_str()); \
    else if(attrName == "g") color.g = atoi(attrValue.c_str()); \
    else if(attrName == "b") color.b = atoi(attrValue.c_str());



#define MMSTHEMECLASS_INIT_STRING(x) \
	this->ed.x = NULL; \
    this->id.is##x = false;

#define MMSTHEMECLASS_FREE_STRING(x) \
	if (this->ed.x) delete this->ed.x; \
	this->ed.x = NULL; \
    this->id.is##x = false;

#define MMSTHEMECLASS_SET_STRING(x) \
	if (!this->ed.x) this->ed.x = new string(x); \
    else *(this->ed.x) = x; \
	this->id.is##x = true;

#define MMSTHEMECLASS_GET_STRING(x) \
	if (!this->id.is##x) return false; \
	x = *(this->ed.x); \
	return true;



#define MMSTHEMECLASS_INIT_STRINGS(x,c) \
	for (int cc=0;cc<c;cc++) \
		this->ed.x[cc] = NULL; \
    this->id.is##x = false;

#define MMSTHEMECLASS_FREE_STRINGS(x,c) \
	for (int cc=0;cc<c;cc++) { \
		if (this->ed.x[cc]) delete this->ed.x[cc]; \
		this->ed.x[cc] = NULL; \
	} \
    this->id.is##x = false;

#define MMSTHEMECLASS_SET_STRINGS(x,c,y) \
	if (!this->ed.x[c]) this->ed.x[c] = new string(y); \
	else *(this->ed.x[c]) = y; \
	this->id.is##x = true;

#define MMSTHEMECLASS_GET_STRINGS(x,c,y) \
	if (!this->id.is##x) return false; \
	if (!this->ed.x[c]) return false; \
	y = *(this->ed.x[c]); \
	return true;

#define MMSTHEMECLASS_UNSET_STRINGS(x,c) \
	for (int cc=0;cc<c;cc++) \
		if (this->ed.x[cc]) *(this->ed.x[cc]) = ""; \
	this->id.is##x = false;



#define MMSTHEMECLASS_INIT_BASIC(x) \
    this->id.is##x = false;

#define MMSTHEMECLASS_FREE_BASIC(x) \
    this->id.is##x = false;

#define MMSTHEMECLASS_SET_BASIC(x) \
    this->id.x = x; \
    this->id.is##x = true;

#define MMSTHEMECLASS_GET_BASIC(x) \
    if (!this->id.is##x) return false; \
    x = this->id.x; \
    return true;


#define MMSTHEMECLASS_UNSET(x) \
    this->id.is##x = false;

#define MMSTHEMECLASS_ISSET(x) \
    return this->id.is##x;




namespace MMSGUI_MMSTHEME_ATTR {

	#define MMSGUI_MMSTHEME_ATTR_ATTRDESC \
		{ "name", TAFF_ATTRTYPE_STRING }
	
	#define MMSGUI_MMSTHEME_ATTR_IDS \
		MMSGUI_MMSTHEME_ATTR_IDS_name
	
	#define MMSGUI_MMSTHEME_ATTR_INIT { \
		MMSGUI_MMSTHEME_ATTR_ATTRDESC, \
		NULL \
	}

	typedef enum {
		MMSGUI_MMSTHEME_ATTR_IDS
	} ids;
}

namespace MMSGUI_MMSDIALOG_ATTR {

	#define MMSGUI_MMSDIALOG_ATTR_ATTRDESC \
		{ "name", TAFF_ATTRTYPE_STRING }
	
	#define MMSGUI_MMSDIALOG_ATTR_IDS \
		MMSGUI_MMSDIALOG_ATTR_IDS_name
	
	#define MMSGUI_MMSDIALOG_ATTR_INIT { \
		MMSGUI_MMSDIALOG_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_MMSDIALOG_ATTR_IDS
	} ids;
}

namespace MMSGUI_DESCRIPTION_ATTR {

	#define MMSGUI_DESCRIPTION_ATTR_ATTRDESC \
		{ "author", TAFF_ATTRTYPE_STRING }, \
		{ "email", TAFF_ATTRTYPE_STRING }, \
		{ "desc", TAFF_ATTRTYPE_STRING }
	
	#define MMSGUI_DESCRIPTION_ATTR_IDS \
		MMSGUI_DESCRIPTION_ATTR_IDS_author, \
		MMSGUI_DESCRIPTION_ATTR_IDS_email, \
		MMSGUI_DESCRIPTION_ATTR_IDS_desc

	#define MMSGUI_DESCRIPTION_ATTR_INIT { \
		MMSGUI_DESCRIPTION_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_DESCRIPTION_ATTR_IDS
	} ids;
}

namespace MMSGUI_CLASS_ATTR {

	#define MMSGUI_CLASS_ATTR_ATTRDESC \
		{ "name", TAFF_ATTRTYPE_STRING }, \
		{ "type", TAFF_ATTRTYPE_STRING }
	
	#define MMSGUI_CLASS_ATTR_IDS \
		MMSGUI_CLASS_ATTR_IDS_name, \
		MMSGUI_CLASS_ATTR_IDS_type
}

namespace MMSGUI_BORDER_ATTR {

	#define MMSGUI_BORDER_ATTR_ATTRDESC \
		{ "border.color", TAFF_ATTRTYPE_STRING }, \
		{ "border.color.a", TAFF_ATTRTYPE_UCHAR }, \
		{ "border.color.r", TAFF_ATTRTYPE_UCHAR }, \
		{ "border.color.g", TAFF_ATTRTYPE_UCHAR }, \
		{ "border.color.b", TAFF_ATTRTYPE_UCHAR }, \
		{ "border.selcolor", TAFF_ATTRTYPE_STRING }, \
		{ "border.selcolor.a", TAFF_ATTRTYPE_UCHAR }, \
		{ "border.selcolor.r", TAFF_ATTRTYPE_UCHAR }, \
		{ "border.selcolor.g", TAFF_ATTRTYPE_UCHAR }, \
		{ "border.selcolor.b", TAFF_ATTRTYPE_UCHAR }, \
		{ "border.image.path", TAFF_ATTRTYPE_STRING }, \
		{ "border.image.top-left", TAFF_ATTRTYPE_STRING }, \
		{ "border.image.top", TAFF_ATTRTYPE_STRING }, \
		{ "border.image.top-right", TAFF_ATTRTYPE_STRING }, \
		{ "border.image.right", TAFF_ATTRTYPE_STRING }, \
		{ "border.image.bottom-right", TAFF_ATTRTYPE_STRING }, \
		{ "border.image.bottom", TAFF_ATTRTYPE_STRING }, \
		{ "border.image.bottom-left", TAFF_ATTRTYPE_STRING }, \
		{ "border.image.left", TAFF_ATTRTYPE_STRING }, \
		{ "border.selimage.path", TAFF_ATTRTYPE_STRING }, \
		{ "border.selimage.top-left", TAFF_ATTRTYPE_STRING }, \
		{ "border.selimage.top", TAFF_ATTRTYPE_STRING }, \
		{ "border.selimage.top-right", TAFF_ATTRTYPE_STRING }, \
		{ "border.selimage.right", TAFF_ATTRTYPE_STRING }, \
		{ "border.selimage.bottom-right", TAFF_ATTRTYPE_STRING }, \
		{ "border.selimage.bottom", TAFF_ATTRTYPE_STRING }, \
		{ "border.selimage.bottom-left", TAFF_ATTRTYPE_STRING }, \
		{ "border.selimage.left", TAFF_ATTRTYPE_STRING }, \
		{ "border.thickness", TAFF_ATTRTYPE_UCHAR }, \
		{ "border.margin", TAFF_ATTRTYPE_UCHAR }, \
		{ "border.rcorners", TAFF_ATTRTYPE_BOOL }
	
	#define MMSGUI_BORDER_ATTR_IDS \
		MMSGUI_BORDER_ATTR_IDS_border_color, \
		MMSGUI_BORDER_ATTR_IDS_border_color_a, \
		MMSGUI_BORDER_ATTR_IDS_border_color_r, \
		MMSGUI_BORDER_ATTR_IDS_border_color_g, \
		MMSGUI_BORDER_ATTR_IDS_border_color_b, \
		MMSGUI_BORDER_ATTR_IDS_border_selcolor, \
		MMSGUI_BORDER_ATTR_IDS_border_selcolor_a, \
		MMSGUI_BORDER_ATTR_IDS_border_selcolor_r, \
		MMSGUI_BORDER_ATTR_IDS_border_selcolor_g, \
		MMSGUI_BORDER_ATTR_IDS_border_selcolor_b, \
		MMSGUI_BORDER_ATTR_IDS_border_image_path, \
		MMSGUI_BORDER_ATTR_IDS_border_image_top_left, \
		MMSGUI_BORDER_ATTR_IDS_border_image_top, \
		MMSGUI_BORDER_ATTR_IDS_border_image_top_right, \
		MMSGUI_BORDER_ATTR_IDS_border_image_right, \
		MMSGUI_BORDER_ATTR_IDS_border_image_bottom_right, \
		MMSGUI_BORDER_ATTR_IDS_border_image_bottom, \
		MMSGUI_BORDER_ATTR_IDS_border_image_bottom_left, \
		MMSGUI_BORDER_ATTR_IDS_border_image_left, \
		MMSGUI_BORDER_ATTR_IDS_border_selimage_path, \
		MMSGUI_BORDER_ATTR_IDS_border_selimage_top_left, \
		MMSGUI_BORDER_ATTR_IDS_border_selimage_top, \
		MMSGUI_BORDER_ATTR_IDS_border_selimage_top_right, \
		MMSGUI_BORDER_ATTR_IDS_border_selimage_right, \
		MMSGUI_BORDER_ATTR_IDS_border_selimage_bottom_right, \
		MMSGUI_BORDER_ATTR_IDS_border_selimage_bottom, \
		MMSGUI_BORDER_ATTR_IDS_border_selimage_bottom_left, \
		MMSGUI_BORDER_ATTR_IDS_border_selimage_left, \
		MMSGUI_BORDER_ATTR_IDS_border_thickness, \
		MMSGUI_BORDER_ATTR_IDS_border_margin, \
		MMSGUI_BORDER_ATTR_IDS_border_rcorners
}

namespace MMSGUI_WIDGET_ATTR {

	#define MMSGUI_WIDGET_ATTR_ATTRDESC \
		{ "bgcolor", TAFF_ATTRTYPE_STRING }, \
		{ "bgimage", TAFF_ATTRTYPE_STRING }
	
	#define MMSGUI_WIDGET_ATTR_IDS \
		MMSGUI_WIDGET_ATTR_IDS_bgcolor, \
		MMSGUI_WIDGET_ATTR_IDS_bgimage
}

namespace MMSGUI_ARROW_ATTR {

	#define MMSGUI_ARROW_ATTR_ATTRDESC \
		{ "color", TAFF_ATTRTYPE_STRING }, \
		{ "selcolor", TAFF_ATTRTYPE_STRING }, \
		{ "direction", TAFF_ATTRTYPE_STRING }
	
	#define MMSGUI_ARROW_ATTR_IDS \
		MMSGUI_ARROW_ATTR_IDS_color, \
		MMSGUI_ARROW_ATTR_IDS_selcolor, \
		MMSGUI_ARROW_ATTR_IDS_direction
	
	#define MMSGUI_ARROW_ATTR_INIT { \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WIDGET_ATTR_ATTRDESC, \
		MMSGUI_ARROW_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WIDGET_ATTR_IDS,
		MMSGUI_ARROW_ATTR_IDS
	} ids;
}

namespace MMSGUI_IMAGE_ATTR {

	#define MMSGUI_IMAGE_ATTR_ATTRDESC \
		{ "image", TAFF_ATTRTYPE_STRING }, \
		{ "image.path", TAFF_ATTRTYPE_STRING }, \
		{ "image.name", TAFF_ATTRTYPE_STRING }
	
	#define MMSGUI_IMAGE_ATTR_IDS \
		MMSGUI_IMAGE_ATTR_IDS_image, \
		MMSGUI_IMAGE_ATTR_IDS_image_path, \
		MMSGUI_IMAGE_ATTR_IDS_image_name
	
	#define MMSGUI_IMAGE_ATTR_INIT { \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WIDGET_ATTR_ATTRDESC, \
		MMSGUI_IMAGE_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WIDGET_ATTR_IDS,
		MMSGUI_IMAGE_ATTR_IDS
	} ids;
}


namespace MMSGUI_WINDOW_ATTR {

	#define MMSGUI_WINDOW_ATTR_ATTRDESC \
		{ "alignment", TAFF_ATTRTYPE_STRING }, \
		{ "dx", TAFF_ATTRTYPE_STRING }, \
		{ "dy", TAFF_ATTRTYPE_STRING }, \
		{ "w", TAFF_ATTRTYPE_STRING }, \
		{ "h", TAFF_ATTRTYPE_STRING }, \
		{ "bgcolor", TAFF_ATTRTYPE_STRING }, \
		{ "bgcolor.a", TAFF_ATTRTYPE_UCHAR }, \
		{ "bgcolor.r", TAFF_ATTRTYPE_UCHAR }, \
		{ "bgcolor.g", TAFF_ATTRTYPE_UCHAR }, \
		{ "bgcolor.b", TAFF_ATTRTYPE_UCHAR }, \
		{ "bgimage", TAFF_ATTRTYPE_STRING }, \
		{ "bgimage.path", TAFF_ATTRTYPE_STRING }, \
		{ "bgimage.name", TAFF_ATTRTYPE_STRING }, \
		{ "opacity", TAFF_ATTRTYPE_UCHAR }, \
		{ "fadein", TAFF_ATTRTYPE_BOOL }, \
		{ "fadeout", TAFF_ATTRTYPE_BOOL }, \
		{ "debug", TAFF_ATTRTYPE_BOOL }, \
		{ "margin", TAFF_ATTRTYPE_UCHAR }, \
		{ "up_arrow", TAFF_ATTRTYPE_STRING }, \
		{ "down_arrow", TAFF_ATTRTYPE_STRING }, \
		{ "left_arrow", TAFF_ATTRTYPE_STRING }, \
		{ "right_arrow", TAFF_ATTRTYPE_STRING }, \
		{ "navigate_up", TAFF_ATTRTYPE_STRING }, \
		{ "navigate_down", TAFF_ATTRTYPE_STRING }, \
		{ "navigate_left", TAFF_ATTRTYPE_STRING }, \
		{ "navigate_right", TAFF_ATTRTYPE_STRING }, \
		{ "own_surface", TAFF_ATTRTYPE_BOOL }, \
		{ "movein", TAFF_ATTRTYPE_STRING }, \
		{ "moveout", TAFF_ATTRTYPE_STRING }

	#define MMSGUI_WINDOW_ATTR_IDS \
		MMSGUI_WINDOW_ATTR_IDS_alignment, \
		MMSGUI_WINDOW_ATTR_IDS_dx, \
		MMSGUI_WINDOW_ATTR_IDS_dy, \
		MMSGUI_WINDOW_ATTR_IDS_w, \
		MMSGUI_WINDOW_ATTR_IDS_h, \
		MMSGUI_WINDOW_ATTR_IDS_bgcolor, \
		MMSGUI_WINDOW_ATTR_IDS_bgcolor_a, \
		MMSGUI_WINDOW_ATTR_IDS_bgcolor_r, \
		MMSGUI_WINDOW_ATTR_IDS_bgcolor_g, \
		MMSGUI_WINDOW_ATTR_IDS_bgcolor_b, \
		MMSGUI_WINDOW_ATTR_IDS_bgimage, \
		MMSGUI_WINDOW_ATTR_IDS_bgimage_path, \
		MMSGUI_WINDOW_ATTR_IDS_bgimage_name, \
		MMSGUI_WINDOW_ATTR_IDS_opacity, \
		MMSGUI_WINDOW_ATTR_IDS_fadein, \
		MMSGUI_WINDOW_ATTR_IDS_fadeout, \
		MMSGUI_WINDOW_ATTR_IDS_debug, \
		MMSGUI_WINDOW_ATTR_IDS_margin, \
		MMSGUI_WINDOW_ATTR_IDS_up_arrow, \
		MMSGUI_WINDOW_ATTR_IDS_down_arrow, \
		MMSGUI_WINDOW_ATTR_IDS_left_arrow, \
		MMSGUI_WINDOW_ATTR_IDS_right_arrow, \
		MMSGUI_WINDOW_ATTR_IDS_navigate_up, \
		MMSGUI_WINDOW_ATTR_IDS_navigate_down, \
		MMSGUI_WINDOW_ATTR_IDS_navigate_left, \
		MMSGUI_WINDOW_ATTR_IDS_navigate_right, \
		MMSGUI_WINDOW_ATTR_IDS_own_surface, \
		MMSGUI_WINDOW_ATTR_IDS_movein, \
		MMSGUI_WINDOW_ATTR_IDS_moveout
}


namespace MMSGUI_MAINWINDOW_ATTR {

	#define MMSGUI_MAINWINDOW_ATTR_INIT { \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WINDOW_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WINDOW_ATTR_IDS
	} ids;
}

namespace MMSGUI_CLASS_MAINWINDOW_ATTR {

	#define MMSGUI_CLASS_MAINWINDOW_ATTR_INIT { \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WINDOW_ATTR_ATTRDESC, \
		MMSGUI_CLASS_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WINDOW_ATTR_IDS,
		MMSGUI_CLASS_ATTR_IDS
	} ids;
}

namespace MMSGUI_POPUPWINDOW_ATTR {

	#define MMSGUI_POPUPWINDOW_ATTR_ATTRDESC \
		{ "duration", TAFF_ATTRTYPE_STRING }
	
	#define MMSGUI_POPUPWINDOW_ATTR_IDS \
		MMSGUI_POPUPWINDOW_ATTR_IDS_duration
	
	#define MMSGUI_POPUPWINDOW_ATTR_INIT { \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WINDOW_ATTR_ATTRDESC, \
		MMSGUI_POPUPWINDOW_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WINDOW_ATTR_IDS,
		MMSGUI_POPUPWINDOW_ATTR_IDS
	} ids;
}

namespace MMSGUI_CLASS_POPUPWINDOW_ATTR {

	#define MMSGUI_CLASS_POPUPWINDOW_ATTR_INIT { \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WINDOW_ATTR_ATTRDESC, \
		MMSGUI_POPUPWINDOW_ATTR_ATTRDESC, \
		MMSGUI_CLASS_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WINDOW_ATTR_IDS,
		MMSGUI_POPUPWINDOW_ATTR_IDS,
		MMSGUI_CLASS_ATTR_IDS
	} ids;
}


extern TAFF_DESCRIPTION mmsgui_taff_description;


typedef enum {
	MMSGUI_TAGTABLE_TAG_MMSTHEME,
	MMSGUI_TAGTABLE_TAG_MMSDIALOG,
	MMSGUI_TAGTABLE_TAG_DESCRIPTION,
	MMSGUI_TAGTABLE_TAG_MAINWINDOW,
	MMSGUI_TAGTABLE_TAG_CLASS_MAINWINDOW,
	MMSGUI_TAGTABLE_TAG_POPUPWINDOW,
	MMSGUI_TAGTABLE_TAG_CLASS_POPUPWINDOW,
	MMSGUI_TAGTABLE_TAG_ARROW,
	MMSGUI_TAGTABLE_TAG_IMAGE
};







#endif /*MMSTHEMEBASE_H_*/


