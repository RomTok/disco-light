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

#include "mmsconfig/mmsrcparser.h"
#include "mmstools/tools.h"

MMSRcParser::MMSRcParser() {
	this->global.logfile       = "/tmp/mmscore";
    this->global.firstplugin   = "<none>";
    this->global.inputmap      = "default";
    this->global.theme         = "default";
    this->global.stdout        = false;
    this->global.inputinterval = 0;
	this->global.shutdown      = false;

    this->configdb.database = "/tmp/mmsconfigdb";
	this->datadb.database   = "/tmp/mmsdatadb";

    this->graphics.xres                       = 800;
    this->graphics.yres                       = 600;
    this->graphics.backend                    = "";
    this->graphics.outputtype                 = "VESAFB";
    this->graphics.videolayerid               = 0;
    this->graphics.videolayerpixelformat      = "RGB16";
    this->graphics.videolayeroptions          = "";
    this->graphics.videolayerbuffermode       = "BACKSYSTEM";
	this->graphics.graphicslayerid            = 0;
	this->graphics.graphicslayerpixelformat   = "RGB16";
    this->graphics.graphicslayeroptions       = "";
    this->graphics.graphicslayerbuffermode    = "BACKSYSTEM";
    this->graphics.vrect.x                    = 0;
    this->graphics.vrect.y                    = 0;
    this->graphics.vrect.w                    = 0;
    this->graphics.vrect.h                    = 0;
    this->graphics.touchrect.x                = 0;
    this->graphics.touchrect.y                = 0;
    this->graphics.touchrect.w                = 0;
    this->graphics.touchrect.h                = 0;
    this->graphics.pointer                    = "NONE";	// use the mouse pointer, default no
    this->graphics.showpointer                = false;	// show the mouse pointer, default no
	this->graphics.graphicswindowpixelformat  = "";		// supported values: ARGB or AYUV, empty string means autodetection
	this->graphics.graphicssurfacepixelformat = "";		// supported values: ARGB or AYUV, empty string means autodetection
    this->graphics.extendedaccel              = true;	// use lowlevel disko routines for faster pixel manipulation
    this->graphics.allocmethod                = "";		// the current alloc method
	this->graphics.fullscreen    			  = false;	// x11 fullscreen?, default no
}

MMSRcParser::~MMSRcParser() {

}

void MMSRcParser::parseFile(string filename) {

	try {
		xmlDoc *parser = NULL;

		LIBXML_TEST_VERSION

#ifdef __ENABLE_LOG__
		parser = xmlReadFile((char *)filename.c_str(), NULL, XML_PARSE_NOBLANKS | XML_PARSE_NONET);
#else
		parser = xmlReadFile((char *)filename.c_str(), NULL, XML_PARSE_NOBLANKS | XML_PARSE_NONET | XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
#endif

		if(parser == NULL) {
			throw new MMSRcParserError(1,"Could not parse file:" + filename);
		}

		if(parser) {
			//Walk the tree:
			xmlNode* pNode = xmlDocGetRootElement(parser);
	  		if(xmlStrcmp(pNode->name, (const xmlChar *) "mmsrc")) {
	  			std::cout << "invalid configuration file (" << filename << ") - does not contain mmsrc root node" << std::endl;
	  			throw new MMSRcParserError(1, "invalid file");
	  		}
	      	this->throughFile(pNode);

            if (this->graphics.vrect.w <= 0)
                this->graphics.vrect.w = this->graphics.xres;
            if (this->graphics.vrect.h <= 0)
                this->graphics.vrect.h = this->graphics.yres;
            if ((this->graphics.vrect.x < 0)||(this->graphics.vrect.x > this->graphics.xres))
                this->graphics.vrect.x = 0;
            if ((this->graphics.vrect.y < 0)||(this->graphics.vrect.y > this->graphics.yres))
                this->graphics.vrect.y = 0;
            if (this->graphics.vrect.w - this->graphics.vrect.x > this->graphics.xres)
                this->graphics.vrect.w = this->graphics.xres - this->graphics.vrect.x;
            if (this->graphics.vrect.h - this->graphics.vrect.y > this->graphics.yres)
                this->graphics.vrect.h = this->graphics.yres - this->graphics.vrect.y;


    	    /*free the document */
    	    xmlFreeDoc(parser);
		}

	    /*
	     *Free the global variables that may
	     *have been allocated by the parser.
	     */
	    xmlCleanupParser();
	}
	catch(MMSError *error)
	{
		std::cout << "Exception caught: " << error->getMessage() << std::endl;
		throw new MMSRcParserError(1,error->getMessage());
	}

}

void MMSRcParser::getMMSRc(MMSConfigDataGlobal 		&global,
		                   MMSConfigDataDB     		&configdb,
		                   MMSConfigDataDB     		&datadb,
		                   MMSConfigDataGraphics    &graphics) {
	global   = this->global;
	configdb = this->configdb;
	datadb   = this->datadb;
	graphics = this->graphics;
}

/**
 * Checks for version of the configuration file.
 *
 * If the version does not match, an exception is thrown.
 *
 * @param	node	should be the mmsrc root node
 */
void MMSRcParser::checkVersion(xmlNode* node) {

	xmlChar *version;

	version = xmlGetProp(node, (const xmlChar*)"version");

	if(!version) {
		std::cout << "Configuration file misses version entity!" << std::endl;
		throw new MMSRcParserError(1, "missing version");
	}

	// sep version
	version[1]=0;
	version[3]=0;
	int mav = atoi((char*)&version[0]);
	int miv = atoi((char*)&version[2]);

	// currently check for disko 1.5
	if ((mav > 1)||(miv > 5)) {
		std::cout << "Version of configuration file does not match!" << std::endl;
		xmlFree(version);
		throw new MMSRcParserError(1, "version mismatch");
	}

	xmlFree(version);
}

void MMSRcParser::throughGlobal(xmlNode* node) {
	xmlNode *cur_node = NULL;
	xmlChar *parname;
	xmlChar *parvalue;

	node = node->xmlChildrenNode;

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if(!xmlStrcmp(cur_node->name, (const xmlChar *) "text")) continue;
		if(!xmlStrcmp(cur_node->name, (const xmlChar *) "comment"))	continue;
		if(xmlStrcmp(cur_node->name, (const xmlChar *) "parameter")) {
			printf("RcParser: ignoring tag <%s/>\n", cur_node->name);
			continue;
		}

    	parname  = xmlGetProp(cur_node, (const xmlChar*)"name");
    	parvalue = xmlGetProp(cur_node, (const xmlChar*)"value");

    	if(parname == NULL && parvalue == NULL)
    		continue;

	    if(!xmlStrcmp(parname, (const xmlChar *) "logfile"))
		    this->global.logfile = string((const char *)parvalue);
		else if(!xmlStrcmp(parname, (const xmlChar *) "inputmap"))
			this->global.inputmap = string((const char *)parvalue);
		else if(!xmlStrcmp(parname, (const xmlChar *) "prefix"))
			this->global.prefix = string((const char *)parvalue);
        else if(!xmlStrcmp(parname, (const xmlChar *) "theme"))
            this->global.theme = string((const char *)parvalue);
		else if(!xmlStrcmp(parname, (const xmlChar *) "sysconfig"))
			this->global.sysconfig = string((const char *)parvalue);
		else if(!xmlStrcmp(parname, (const xmlChar *) "data"))
			this->global.data = string((const char *)parvalue);
        else if(!xmlStrcmp(parname, (const xmlChar *) "inputinterval"))
            this->global.inputinterval = strToInt(string((const char *)parvalue));
		else if(!xmlStrcmp(parname, (const xmlChar *) "firstplugin"))
			this->global.firstplugin = string((const char *)parvalue);
        else if(!xmlStrcmp(parname, (const xmlChar *) "shutdown"))
            this->global.shutdown = strToBool(string((const char *)parvalue));
        else if(!xmlStrcmp(parname, (const xmlChar *) "shutdowncmd"))
            this->global.shutdowncmd = string((const char *)parvalue);
		else
			printf("RcParser: ignoring parameter '%s' in tag <global/>\n", parname);

	    xmlFree(parname);
	    xmlFree(parvalue);
	}
}

void MMSRcParser::throughDBSettings(xmlNode* node) {
	MMSConfigDataDB *db;
	xmlChar *type;

	type = xmlGetProp(node, (const xmlChar*)"type");

	if(!xmlStrcmp(type, (const xmlChar *) "config")) {
		db = &this->configdb;
	}
	else if(!xmlStrcmp(type, (const xmlChar *) "data")) {
		db = &this->datadb;
	}
	else {
		throw new MMSRcParserError(1, "unknown database type (" + string((const char*)type) + ")");
	}

	xmlFree(type);

	xmlNode *cur_node = NULL;
	xmlChar *parname;
	xmlChar *parvalue;

	for (cur_node = node->xmlChildrenNode; cur_node; cur_node = cur_node->next) {
		if(!xmlStrcmp(cur_node->name, (const xmlChar *) "text")) continue;
		if(!xmlStrcmp(cur_node->name, (const xmlChar *) "comment"))	continue;
		if(xmlStrcmp(cur_node->name, (const xmlChar *) "parameter")) {
			printf("RcParser: ignoring tag <%s/>\n", cur_node->name);
			continue;
		}

		parname  = xmlGetProp(cur_node, (const xmlChar*)"name");
		parvalue = xmlGetProp(cur_node, (const xmlChar*)"value");

		if(!xmlStrcmp(parname, (const xmlChar *) "dbms"))
            db->dbms = string((const char *)parvalue);
        else if(!xmlStrcmp(parname, (const xmlChar *) "address"))
            db->address = string((const char *)parvalue);
        else if(!xmlStrcmp(parname, (const xmlChar *) "port"))
            db->port = atoi((const char*)parvalue);
        else if(!xmlStrcmp(parname, (const xmlChar *) "user"))
            db->user = string((const char *)parvalue);
        else if(!xmlStrcmp(parname, (const xmlChar *) "password"))
            db->password = string((const char *)parvalue);
		else if(!xmlStrcmp(parname, (const xmlChar *) "database"))
		    db->database = string((const char *)parvalue);
		else
			printf("RcParser: ignoring parameter '%s' in tag <dbsettings/>\n", parname);

	    xmlFree(parname);
	    xmlFree(parvalue);
	}
}

void MMSRcParser::throughGraphics(xmlNode* node) {

	xmlNode *cur_node = NULL;
	xmlChar *parname;
	xmlChar *parvalue;

	node = node->xmlChildrenNode;

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if(!xmlStrcmp(cur_node->name, (const xmlChar *) "text")) continue;
		if(!xmlStrcmp(cur_node->name, (const xmlChar *) "comment"))	continue;
		if(xmlStrcmp(cur_node->name, (const xmlChar *) "parameter")) {
			printf("RcParser: ignoring tag <%s/>\n", cur_node->name);
			continue;
		}

    	parname  = xmlGetProp(cur_node, (const xmlChar*)"name");
    	parvalue = xmlGetProp(cur_node, (const xmlChar*)"value");

		if(!xmlStrcmp(parname, (const xmlChar *) "xres"))
			this->graphics.xres = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "yres"))
	        this->graphics.yres = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "backend"))
	        this->graphics.backend = strToUpr(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "outputtype"))
	        this->graphics.outputtype = strToUpr(string((const char *)parvalue));
		else if(!xmlStrcmp(parname, (const xmlChar *) "videolayerid"))
			this->graphics.videolayerid = atoi((const char *)parvalue);
	    else if(!xmlStrcmp(parname, (const xmlChar *) "videolayerpixelformat"))
	        this->graphics.videolayerpixelformat = strToUpr(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "videolayeroptions"))
	        this->graphics.videolayeroptions = strToUpr(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "videolayerbuffermode"))
	        this->graphics.videolayerbuffermode = strToUpr(string((const char *)parvalue));
		else if(!xmlStrcmp(parname, (const xmlChar *) "graphicslayerid"))
			this->graphics.graphicslayerid = atoi((const char *)parvalue);
		else if(!xmlStrcmp(parname, (const xmlChar *) "graphicslayerpixelformat"))
			this->graphics.graphicslayerpixelformat = strToUpr(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "graphicslayeroptions"))
	        this->graphics.graphicslayeroptions = strToUpr(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "graphicslayerbuffermode"))
	        this->graphics.graphicslayerbuffermode = strToUpr(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "vrect.x"))
	        this->graphics.vrect.x = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "vrect.y"))
	        this->graphics.vrect.y = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "vrect.w"))
	        this->graphics.vrect.w = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "vrect.h"))
	        this->graphics.vrect.h = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "touchrect.x"))
	        this->graphics.touchrect.x = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "touchrect.y"))
	        this->graphics.touchrect.y = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "touchrect.w"))
	        this->graphics.touchrect.w = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "touchrect.h"))
	    	this->graphics.touchrect.h = strToInt(string((const char *)parvalue));
        else if(!xmlStrcmp(parname, (const xmlChar *) "pointer")) {
            this->graphics.pointer = strToUpr(string((const char *)parvalue));
            if(graphics.pointer == "EXTERNAL" || graphics.pointer == "INTERNAL" || graphics.pointer == "TRUE")
            	graphics.showpointer = true;
        } else if(!xmlStrcmp(parname, (const xmlChar *) "graphicswindowpixelformat"))
			this->graphics.graphicswindowpixelformat = strToUpr(string((const char *)parvalue));
		else if(!xmlStrcmp(parname, (const xmlChar *) "graphicssurfacepixelformat"))
			this->graphics.graphicssurfacepixelformat = strToUpr(string((const char *)parvalue));
        else if(!xmlStrcmp(parname, (const xmlChar *) "extendedaccel"))
            this->graphics.extendedaccel = strToBool(string((const char *)parvalue));
		else if(!xmlStrcmp(parname, (const xmlChar *) "allocmethod"))
			this->graphics.allocmethod = strToUpr(string((const char *)parvalue));
        else if(!xmlStrcmp(parname, (const xmlChar *) "fullscreen"))
            this->graphics.fullscreen = strToBool(string((const char *)parvalue));
		else
			printf("RcParser: ignoring parameter '%s' in tag <graphics/>\n", parname);

	    xmlFree(parname);
	    xmlFree(parvalue);

	}
}

void MMSRcParser::throughFile(xmlNode* node) {

	xmlNode *cur_node = NULL;

	if(node==NULL)
		return;

	if(!xmlStrcmp(node->name, (const xmlChar *) "mmsrc")) {
		checkVersion(node);

		node = node->xmlChildrenNode;
    }
	else {
		this->throughFile(node->next);
		return;
	}

	for (cur_node = node; cur_node; cur_node = cur_node->next) {

		if(!xmlStrcmp(cur_node->name, (const xmlChar *) "text")) continue;
		if(!xmlStrcmp(cur_node->name, (const xmlChar *) "comment"))	continue;

		if(!xmlStrcmp(cur_node->name, (const xmlChar *) "global"))
			throughGlobal(cur_node);
		else if(!xmlStrcmp(cur_node->name, (const xmlChar *) "dbsettings"))
			throughDBSettings(cur_node);
		else if((!xmlStrcmp(cur_node->name, (const xmlChar *) "dfbsettings"))
			  ||(!xmlStrcmp(cur_node->name, (const xmlChar *) "graphics")))
			throughGraphics(cur_node);
		else
			printf("RcParser: ignoring tag <%s/>\n", cur_node->name);
	}
}

