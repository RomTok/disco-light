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

#include "mmsxmlclientinterface.h"

#ifdef LIBXML_READER_ENABLED

MMSXMLClientInterface::MMSXMLClientInterface(string host, unsigned int port) {
    LIBXML_TEST_VERSION;

	logger.setIdentity("MMSXMLClientInterface");

	this->tcl = new MMSTCPClient(host, port);
}

bool MMSXMLClientInterface::parseAnswer(string *answer, int *rc, string *error) {
    xmlTextReaderPtr reader;
    bool             ret = false;

    if(xmlReaderNewMemory(reader,              /* reader             */
                          answer->c_str(),     /* buffer             */
                          answer->length(),    /* size               */
                          NULL,                /* base URL           */
                          NULL,                /* encoding           */
                          XML_PARSE_NOBLANKS)  /* remove blank nodes */
     == -1) {
        logger.writeLog("Error initializing xmlReader()");
        return false;
    }
    
    if(xmlTextReaderRead(reader)) {
        if(checkRoot(reader, rc, error))
            ret = true;
    }

    xmlFreeTextReader(reader);

    return ret;
}

bool MMSXMLClientInterface::checkRoot(xmlTextReaderPtr reader, int *rc, string *error) {
    xmlChar *name, *attr;

    if (!reader) return false;

    /* check root element */
    name = (xmlChar*)xmlTextReaderConstName(reader);
    if(!name || !xmlStrEqual(name, (const xmlChar*)"ret")) {
        logger.writeLog("The root element must be <ret> and not <" + string((const char*)name) + ">.");
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

    return true;
}

bool MMSXMLClientInterface::funcSendEvent(string heading, int pluginid, int *rc, string *error) {
	string rbuf, abuf;
	
	/* build request */
	rbuf = "<func name=\"SendEvent\" heading=\"" + heading + "\"";
	if (pluginid>=0) rbuf+= " pluginid=\"" + iToStr(pluginid) + "\"";
	rbuf+= "/>";

	/* call server */
	tcl->connectToServer();
	tcl->sendAndReceive(rbuf, &abuf);

	/* parse answer */
	if(parseAnswer(&abuf, rc, error)) {
		/* parse for more values here */

		return true;
	}

	return false;
}

#endif /* LIBXML_READER_ENABLED */

