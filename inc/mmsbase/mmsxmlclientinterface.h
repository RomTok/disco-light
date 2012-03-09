/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2012 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <jens.schneider@diskohq.org>,                   *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include <libxml/xmlversion.h>

#ifdef LIBXML_READER_ENABLED

#ifndef MMSXMLCLIENTINTERFACE_H_
#define MMSXMLCLIENTINTERFACE_H_

#include "mmstools/mmstcpclient.h"

#include <map>

extern "C" {
#include <libxml/xmlreader.h>
}

class MMSXMLClientInterface {
	private:
		MMSTCPClient 	*tcl;

	public:
		MMSXMLClientInterface(string host, unsigned int port);

        bool parseAnswer(string *answer, int *rc, string *error);
		bool checkRoot(xmlDocPtr doc, int *rc, string *error);
		bool funcSendEvent(string heading, int pluginid, int *rc, string *error);
		bool funcSendEvent(string heading, map<string, string> *params, int pluginid, int *rc, string *error);
};

#endif /*MMSXMLCLIENTINTERFACE_H_*/
#endif /* LIBXML_READER_ENABLED */
