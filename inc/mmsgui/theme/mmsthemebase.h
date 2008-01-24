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





#endif /*MMSTHEMEBASE_H_*/


