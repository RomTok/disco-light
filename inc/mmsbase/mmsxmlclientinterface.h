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

#ifndef MMSXMLCLIENTINTERFACE_H_
#define MMSXMLCLIENTINTERFACE_H_

#include "mmstools/mmstools.h"
#include <libxml/xmlreader.h>

class MMSXMLClientInterface {
	private:
		MMSTCPClient 	*tcl;

	public:
		MMSXMLClientInterface(string host, unsigned int port);

        bool parseAnswer(string *answer, int *rc, string *error);
		bool checkRoot(xmlDocPtr doc, int *rc, string *error);
		bool funcSendEvent(string heading, int pluginid, int *rc, string *error);
};

#endif /*MMSXMLCLIENTINTERFACE_H_*/
#endif /* LIBXML_READER_ENABLED */
