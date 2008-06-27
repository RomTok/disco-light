/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
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

    this->dfb.xres                     = 800;
    this->dfb.yres                     = 600;
    this->dfb.outputtype               = "vesafb";
    this->dfb.videolayerid             = 0;
    this->dfb.videolayerpixelformat    = "RGB16";
    this->dfb.videolayeroptions        = "";
    this->dfb.videolayerbuffermode     = "BACKSYSTEM";
	this->dfb.graphicslayerid          = 0;
	this->dfb.graphicslayerpixelformat = "RGB16";
//    this->dfb.graphicslayeroptions     = "SRC_COLORKEY";
    this->dfb.graphicslayeroptions     = "";
    this->dfb.graphicslayerbuffermode  = "BACKSYSTEM";
    this->dfb.vrect.x                  = 0;
    this->dfb.vrect.y                  = 0;
    this->dfb.vrect.w                  = 0;
    this->dfb.vrect.h                  = 0;
    this->dfb.touchrect.x              = 0;
    this->dfb.touchrect.y              = 0;
    this->dfb.touchrect.w              = 0;
    this->dfb.touchrect.h              = 0;
    this->dfb.pointer                  = false;
}

MMSRcParser::~MMSRcParser() {

}

void MMSRcParser::parseFile(string filename) {

	try {
		xmlDoc *parser = NULL;

		LIBXML_TEST_VERSION

		parser = xmlReadFile((char *)filename.c_str(), NULL, 0);

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

            if (this->dfb.vrect.w <= 0)
                this->dfb.vrect.w = this->dfb.xres;
            if (this->dfb.vrect.h <= 0)
                this->dfb.vrect.h = this->dfb.yres;
            if ((this->dfb.vrect.x < 0)||(this->dfb.vrect.x > this->dfb.xres))
                this->dfb.vrect.x = 0;
            if ((this->dfb.vrect.y < 0)||(this->dfb.vrect.y > this->dfb.yres))
                this->dfb.vrect.y = 0;
            if (this->dfb.vrect.w - this->dfb.vrect.x > this->dfb.xres)
                this->dfb.vrect.w = this->dfb.xres - this->dfb.vrect.x;
            if (this->dfb.vrect.h - this->dfb.vrect.y > this->dfb.yres)
                this->dfb.vrect.h = this->dfb.yres - this->dfb.vrect.y;


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

void MMSRcParser::getMMSRc(MMSConfigDataGlobal &global,
		                   MMSConfigDataDB     &configdb,
		                   MMSConfigDataDB     &datadb,
		                   MMSConfigDataDFB    &dfb) {
	global   = this->global;
	configdb = this->configdb;
	datadb   = this->datadb;
	dfb      = this->dfb;
}

/**
 * Checks for version of the configuration file.
 *
 * If the version does not match, an exception is thrown.
 *
 * @note	By now it checks exactly for version 1.1.0.
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

	if(xmlStrcmp(version, (const xmlChar *) "1.1.0")) {
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
		if(xmlStrcmp(cur_node->name, (const xmlChar *) "parameter"))
			continue;

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
        else if(!xmlStrcmp(parname, (const xmlChar *) "inputintervall"))
            this->global.inputinterval = strToInt(string((const char *)parvalue));
		else if(!xmlStrcmp(parname, (const xmlChar *) "firstplugin"))
			this->global.firstplugin = string((const char *)parvalue);
        else if(!xmlStrcmp(parname, (const xmlChar *) "shutdown"))
            this->global.shutdown = strToBool(string((const char *)parvalue));
        else if(!xmlStrcmp(parname, (const xmlChar *) "shutdowncmd"))
            this->global.shutdowncmd = string((const char *)parvalue);

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
		if(xmlStrcmp(cur_node->name, (const xmlChar *) "parameter"))
			continue;

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
		else if(!xmlStrcmp(parname, (const xmlChar *) "database")) {
		    db->database = string((const char *)parvalue);
		}

	    xmlFree(parname);
	    xmlFree(parvalue);
	}
}

void MMSRcParser::throughDFBSettings(xmlNode* node) {

	xmlNode *cur_node = NULL;
	xmlChar *parname;
	xmlChar *parvalue;

	node = node->xmlChildrenNode;

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if(xmlStrcmp(cur_node->name, (const xmlChar *) "parameter"))
			continue;

    	parname  = xmlGetProp(cur_node, (const xmlChar*)"name");
    	parvalue = xmlGetProp(cur_node, (const xmlChar*)"value");

		if(!xmlStrcmp(parname, (const xmlChar *) "xres"))
			this->dfb.xres = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "yres"))
	        this->dfb.yres = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "outputtype"))
	        this->dfb.outputtype = string((const char *)parvalue);
		else if(!xmlStrcmp(parname, (const xmlChar *) "videolayerid"))
			this->dfb.videolayerid = atoi((const char *)parvalue);
	    else if(!xmlStrcmp(parname, (const xmlChar *) "videolayerpixelformat"))
	        this->dfb.videolayerpixelformat = string((const char *)parvalue);
	    else if(!xmlStrcmp(parname, (const xmlChar *) "videolayeroptions"))
	        this->dfb.videolayeroptions = string((const char *)parvalue);
	    else if(!xmlStrcmp(parname, (const xmlChar *) "videolayerbuffermode"))
	        this->dfb.videolayerbuffermode = string((const char *)parvalue);
		else if(!xmlStrcmp(parname, (const xmlChar *) "graphicslayerid"))
			this->dfb.graphicslayerid = atoi((const char *)parvalue);
		else if(!xmlStrcmp(parname, (const xmlChar *) "graphicslayerpixelformat"))
			this->dfb.graphicslayerpixelformat = string((const char *)parvalue);
	    else if(!xmlStrcmp(parname, (const xmlChar *) "graphicslayeroptions"))
	        this->dfb.graphicslayeroptions = string((const char *)parvalue);
	    else if(!xmlStrcmp(parname, (const xmlChar *) "graphicslayerbuffermode"))
	        this->dfb.graphicslayerbuffermode = string((const char *)parvalue);
	    else if(!xmlStrcmp(parname, (const xmlChar *) "vrect.x"))
	        this->dfb.vrect.x = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "vrect.y"))
	        this->dfb.vrect.y = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "vrect.w"))
	        this->dfb.vrect.w = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "vrect.h"))
	        this->dfb.vrect.h = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "touchrec.x"))
	        this->dfb.touchrect.x = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "touchrect.y"))
	        this->dfb.touchrect.y = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "touchrect.w"))
	        this->dfb.touchrect.w = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "touchrect.h"))
	    	this->dfb.touchrect.h = strToInt(string((const char *)parvalue));
        else if(!xmlStrcmp(parname, (const xmlChar *) "pointer"))
            this->dfb.pointer = strToBool(string((const char *)parvalue));

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

		if(!xmlStrcmp(cur_node->name, (const xmlChar *) "global"))
			throughGlobal(cur_node);
		else if(!xmlStrcmp(cur_node->name, (const xmlChar *) "dbsettings"))
			throughDBSettings(cur_node);
		else if(!xmlStrcmp(cur_node->name, (const xmlChar *) "dfbsettings"))
			throughDFBSettings(cur_node);
	}
}

