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
#include <string.h>

#ifdef LIBXML_READER_ENABLED
#include "mmsbase/mmsxmlclientinterface.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "mmstools/tools.h"

MMSXMLClientInterface::MMSXMLClientInterface(string host, unsigned int port) {
    LIBXML_TEST_VERSION;

	this->tcl = new MMSTCPClient(host, port);
}

bool MMSXMLClientInterface::parseAnswer(string *answer, int *rc, string *error) {
    bool             ret = false;
    xmlDocPtr		 doc;

    xmlInitParser();
    doc = xmlReadMemory(answer->c_str(),answer->length(),"memory.xml",NULL,0);

    if(!doc) {
        DEBUGMSG("MMSXMLClientInterface", "Error initializing doc()");
        return false;
    }
    if(checkRoot(doc, rc, error)) {
        ret = true;
	}

	xmlFreeDoc(doc);

    return ret;
}

bool MMSXMLClientInterface::checkRoot(xmlDocPtr doc, int *rc, string *error) {
    xmlChar *name, *attr;

    if (!doc) 
    	return false;
#if 0
    /* check root element */
    name = (xmlChar*)xmlTextReaderConstName(reader);
    if(!name || !xmlStrEqual(name, (const xmlChar*)"ret")) {
        DEBUGMSG("MMSXMLClientInterface", "The root element must be <ret> and not <%s>.", name);
        return false;
    }

    /* get attribute rc */
    *rc = 0;
    attr = xmlTextReaderGetAttribute(reader, (const xmlChar*)"rc");
    if(attr) *rc = atoi((const char*)attr);
    xmlFree(attr);

    /* get attribute error */
    *error = "";
    attr = xmlTextReaderGetAttribute(reader, (const xmlChar*)"error");
    if(attr) *error = strdup((const char*)attr);
    xmlFree(attr);
#endif
    return true;
}

bool MMSXMLClientInterface::funcSendEvent(string heading, int pluginid, int *rc, string *error) {
	string rbuf, abuf;

	/* build request */
	rbuf = "<func name=\"SendEvent\" heading=\"" + heading + "\"";
	if (pluginid>=0) rbuf+= " pluginid=\"" + iToStr(pluginid) + "\"";
	rbuf+= "/>";

	/* call server */
	if(!tcl->connectToServer()) {
		DEBUGMSG("MMSBASE", "connection to server failed");
	}
	
	tcl->sendAndReceive(rbuf, &abuf);

	DEBUGMSG("MMSBASE", "got response %s", abuf.c_str());
	/* parse answer */
	if(parseAnswer(&abuf, rc, error)) {
		/* parse for more values here */

		return true;
	}

	return false;
}

#endif /* LIBXML_READER_ENABLED */

