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

#include "mmsgui/theme/mmstemplateclass.h"

MMSTemplateClass::MMSTemplateClass() {
    this->doc = NULL;
    unsetAll();
}

MMSTemplateClass::~MMSTemplateClass() {
    if (this->doc)
        xmlFreeDoc(this->doc);
}

void MMSTemplateClass::unsetAll() {
    this->className = "";
    if (this->doc)
        xmlFreeDoc(this->doc);
    this->doc = NULL;
    this->node = NULL;
}

void MMSTemplateClass::setAttributesFromXMLNode(xmlNode* node) {
    startXMLScan
    {
        if(attrName == "class")
            setClassName(attrValue);
    }
    endXMLScan 
}

void MMSTemplateClass::duplicateXMLNode(xmlNode* node) {
    if (this->doc)
        xmlFreeDoc(this->doc);
    this->doc = NULL;
    this->node = NULL;
    this->doc = xmlNewDoc(node->doc->version);
    this->node = xmlDocCopyNodeList(this->doc, node);
}

xmlNode *MMSTemplateClass::getXMLNode() {
    return this->node;
}

void MMSTemplateClass::setClassName(string className) {
    this->className = className;
}

string MMSTemplateClass::getClassName() {
    return this->className;
}

