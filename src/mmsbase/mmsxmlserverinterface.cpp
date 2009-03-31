/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#include <libxml/xmlversion.h>

#ifdef LIBXML_READER_ENABLED

#include "mmsbase/mmsxmlserverinterface.h"
#include "mmsbase/mmsevent.h"

MMSXMLServerInterface::MMSXMLServerInterface() : MMSServerInterface("MMSXMLServerInterface") {
    LIBXML_TEST_VERSION;
}

MMSXMLServerInterface::~MMSXMLServerInterface() {
    xmlCleanupParser();
}

bool MMSXMLServerInterface::processRequest(string *request, string *answer) {
	*answer = "";

	xmlInitParser();
    xmlDocPtr doc =NULL;

    DEBUGMSG("MMSXMLServerInterface","got request %s", request->c_str());
    if(!request || !answer) { 
        DEBUGMSG("MMSXMLServerInterface","processRequest() error in cmdline");
    	return false;
    }

    DEBUGMSG("MMSXMLServerInterface","new reader");
    doc = xmlReadMemory(request->c_str(),request->length(),"memory.xml",NULL,0);
    
    if(!doc) {
        *answer = "<ret error=\"Problems with xml request.\"/>";
        DEBUGMSG("MMSXMLServerInterface", "Error initializing xmlReader()");
        return false;
    }

    DEBUGMSG("MMSXMLServerInterface","throughdoc");
    throughDoc(doc, answer);
    if(*answer == "") 
    		*answer = "<ret error=\"Unknown error.\"/>";

    xmlFreeDoc(doc);

    return true;
}

bool MMSXMLServerInterface::throughDoc(xmlDocPtr doc, string *answer) {

    if(!doc|| !answer)  {
        DEBUGMSG("MMSXMLServerInterface","throughdoc, error in cmdline");
        return false;
    }

    xmlNodePtr root =  xmlDocGetRootElement(doc);
    if(!root) 
    	return false;
    
    if(xmlStrEqual(root->name, (const xmlChar*)"func")==0) {
        DEBUGMSG("MMSXMLServerInterface", "The root element must be <func> and not <%s>.", root->name);
        return false;
    }

    return throughFunc(root, answer);
}

bool MMSXMLServerInterface::throughFunc(xmlNodePtr node, string *answer) {
    xmlChar *name;

    if(!node|| !answer) return false;

    /* get name of function to be called */
    name = xmlGetProp(node, (const xmlChar*)"name");
    if(!name) {
		/* function not specified */
        *answer = "<ret error=\"Function not specified.\"/>";
        DEBUGMSG("MMSXMLServerInterface", "Function not specified.");
		return false;
	}

	/* check function */
    if(!xmlStrEqual(name, (const xmlChar*)"SendEvent")) {
		/* unknown function */
        *answer = "<ret error=\"Unknown function '" + string((const char*)name) + "'.\"/>";
        DEBUGMSG("MMSXMLServerInterface", "Unknown function '%s'.", name);
		return false;
	}

    return funcSendEvent(node, answer);
}

bool MMSXMLServerInterface::funcSendEvent(xmlNodePtr node, string *answer) {
    xmlChar  *heading; /**pluginid,*/ //*name, *value;
	MMSEvent *event;

    if(!node || !answer) return false;

    /* get attributes */
    heading  = xmlGetProp(node, (const xmlChar*)"heading");
    //pluginid = xmlTextReaderGetAttribute(reader, (const xmlChar*)"pluginid");

    if(!heading /*|| !pluginid*/) return false;

	/* construct event object */
	event = new MMSEvent((const char*)heading);
	event->send();
	
#if 0
    /* through <func/> childs */
    while(xmlTextReaderRead(reader)) {
        name = (xmlChar*)xmlTextReaderConstName(reader);
        if(name && xmlStrEqual(name, (const xmlChar*)"param")) {
            while(xmlTextReaderMoveToNextAttribute(reader)) {
                name  = xmlTextReaderName(reader);
                value = xmlTextReaderValue(reader);
                event->setData((const char*)name, (const char*)value);
                xmlFree(name);
                xmlFree(value);
            }
        }
    }
#endif
	/* build answer */
	*answer = "<ret/>";

    return true;
}

#endif /* LIBXML_READER_ENABLED */
