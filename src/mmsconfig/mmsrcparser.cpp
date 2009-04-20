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

#include <cstring>
#include "mmsconfig/mmsrcparser.h"
#include "mmstools/tools.h"

#define WRONG_VALUE(parname, parvalue, validvals, addmsg) throw new MMSRcParserError(1, "wrong value '" + string(parvalue) + "' for parameter '" + string((const char *)parname) + "'\n valid value(s): " + validvals + "\n " + addmsg);


MMSRcParser::MMSRcParser() {
	this->global.logfile       = "/tmp/mmscore";
    this->global.firstplugin   = "<none>";
    this->global.inputmap      = "default";
    this->global.theme         = "default";
    this->global.stdout        = false;
    this->global.inputinterval = 0;
	this->global.shutdown      = false;
	this->global.inputmode     = "";

    this->configdb.database = "/tmp/mmsconfigdb";
	this->datadb.database   = "/tmp/mmsdatadb";

    this->graphics.xres                       = 800;
    this->graphics.yres                       = 600;
    this->graphics.xpos                       = 50;
    this->graphics.ypos                       = 50;
    // set MMSFB_BE_NONE for compatibility reason
    this->graphics.backend                    = MMSFB_BE_NONE;
    this->graphics.outputtype                 = MMSFB_OT_NONE;
    this->graphics.videolayerid               = 0;
    this->graphics.videolayerpixelformat      = MMSFB_PF_RGB16;
    this->graphics.videolayeroptions          = "";
    this->graphics.videolayerbuffermode       = "BACKSYSTEM";
	this->graphics.graphicslayerid            = 0;
	this->graphics.graphicslayerpixelformat   = MMSFB_PF_RGB16;
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
    this->graphics.pointer                    = MMSFB_PM_FALSE;	// use the mouse pointer, default no
	this->graphics.graphicswindowpixelformat  = MMSFB_PF_NONE;	// supported values: ARGB, AiRGB or AYUV, NONE means auto detection
	this->graphics.graphicssurfacepixelformat = MMSFB_PF_NONE;	// supported values: ARGB, AiRGB or AYUV, NONE means auto detection
    this->graphics.extendedaccel              = true;	// use lowlevel disko routines for faster pixel manipulation
    this->graphics.allocmethod                = "";		// the current alloc method
	this->graphics.fullscreen    			  = MMSFB_FSM_FALSE;	// x11 fullscreen?, default no
	this->graphics.hideapplication            = false;

	this->language.addtranslations = false;
	this->language.defaulttargetlang = MMSLANG_UKN;
	this->language.sourcelang = MMSLANG_UKN;
	this->language.languagefiledir = "";
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
		std::cout << "RcParser exception: " << error->getMessage() << std::endl;
		throw new MMSRcParserError(1,error->getMessage());
	}

}

void MMSRcParser::getMMSRc(MMSConfigDataGlobal 		&global,
		                   MMSConfigDataDB     		&configdb,
		                   MMSConfigDataDB     		&datadb,
		                   MMSConfigDataGraphics    &graphics,
		                   MMSConfigDataLanguage    &language) {
	global   = this->global;
	configdb = this->configdb;
	datadb   = this->datadb;
	graphics = this->graphics;
	language = this->language;
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
        else if(!xmlStrcmp(parname, (const xmlChar *) "inputmode"))
            this->global.inputmode = string((const char *)parvalue);
		else
			printf("RcParser: ignoring parameter '%s' in tag <global/>\n", parname);

	    xmlFree(parname);
	    xmlFree(parvalue);
	}
}

void MMSRcParser::throughLanguage(xmlNode* node) {
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

	    if(!xmlStrcmp(parname, (const xmlChar *) "sourcelang"))
	    	this->language.sourcelang = strToLang((const char *)parvalue);
        else if(!xmlStrcmp(parname, (const xmlChar *) "defaultdestlang"))
            this->language.defaulttargetlang = strToLang((const char *)parvalue);
        else if(!xmlStrcmp(parname, (const xmlChar *) "addtranslations"))
            this->language.addtranslations = strToBool(string((const char *)parvalue));
        else if(!xmlStrcmp(parname, (const xmlChar *) "languagefiledir"))
        	this->language.languagefiledir = string((const char *)parvalue);
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
	    else if(!xmlStrcmp(parname, (const xmlChar *) "xpos"))
	        this->graphics.xpos = strToInt(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "ypos"))
	        this->graphics.ypos = strToInt(string((const char *)parvalue));
        else if(!xmlStrcmp(parname, (const xmlChar *) "backend")) {
        	string val = string((const char *)parvalue);
        	// get/check value
            if ((this->graphics.backend = getMMSFBBackendFromString(strToUpr(val))) == MMSFB_BE_NONE)
            	WRONG_VALUE(parname, val, MMSFB_BE_VALID_VALUES, "");

        	// check value against output type
            if (this->graphics.outputtype == MMSFB_OT_VESAFB) {
            	switch (this->graphics.backend) {
            	case MMSFB_BE_DFB:
            	case MMSFB_BE_FBDEV:
            		// okay
            		break;
            	default:
            		WRONG_VALUE(parname, val, MMSFB_BE_VALID_VALUES_OT_FB, "-> this depends on outputtype=\"VESAFB\"");
            	}
            }
            else
            if (this->graphics.outputtype == MMSFB_OT_MATROXFB) {
            	switch (this->graphics.backend) {
            	case MMSFB_BE_DFB:
            	case MMSFB_BE_FBDEV:
            		// okay
            		break;
            	default:
            		WRONG_VALUE(parname, val, MMSFB_BE_VALID_VALUES_OT_FB, "-> this depends on outputtype=\"MATROXFB\"");
            	}
            }
            else
            if (this->graphics.outputtype == MMSFB_OT_VIAFB) {
            	switch (this->graphics.backend) {
            	case MMSFB_BE_DFB:
            	case MMSFB_BE_FBDEV:
            		// okay
            		break;
            	default:
            		WRONG_VALUE(parname, val, MMSFB_BE_VALID_VALUES_OT_FB, "-> this depends on outputtype=\"VIAFB\"");
            	}
            }
            else
            if (this->graphics.outputtype == MMSFB_OT_X11) {
            	switch (this->graphics.backend) {
            	case MMSFB_BE_DFB:
            	case MMSFB_BE_X11:
            		// okay
            		break;
            	default:
            		WRONG_VALUE(parname, val, MMSFB_BE_VALID_VALUES_OT_X11, "-> this depends on outputtype=\"X11\"");
            	}
            }
            else
            if (this->graphics.outputtype == MMSFB_OT_XSHM) {
            	switch (this->graphics.backend) {
            	case MMSFB_BE_X11:
            		// okay
            		break;
            	default:
            		WRONG_VALUE(parname, val, MMSFB_BE_VALID_VALUES_OT_X, "-> this depends on outputtype=\"XSHM\"");
            	}
            }
            else
            if (this->graphics.outputtype == MMSFB_OT_XVSHM) {
            	switch (this->graphics.backend) {
            	case MMSFB_BE_X11:
            		// okay
            		break;
            	default:
            		WRONG_VALUE(parname, val, MMSFB_BE_VALID_VALUES_OT_X, "-> this depends on outputtype=\"XVSHM\"");
            	}
            }
            else
            if (this->graphics.outputtype == MMSFB_OT_DAVINCIFB) {
            	switch (this->graphics.backend) {
            	case MMSFB_BE_DFB:
            	case MMSFB_BE_FBDEV:
            		// okay
            		break;
            	default:
            		WRONG_VALUE(parname, val, MMSFB_BE_VALID_VALUES_OT_FB, "-> this depends on outputtype=\"DAVINCIFB\"");
            	}
            }
        }
        else if(!xmlStrcmp(parname, (const xmlChar *) "outputtype")) {
        	string val = string((const char *)parvalue);
        	// get/check value
            if ((this->graphics.outputtype = getMMSFBOutputTypeFromString(strToUpr(val))) == MMSFB_OT_NONE)
            	WRONG_VALUE(parname, val, MMSFB_OT_VALID_VALUES, "");

        	// check value against backend type
            if (this->graphics.backend == MMSFB_BE_DFB) {
            	switch (this->graphics.outputtype) {
            	case MMSFB_OT_VESAFB:
            	case MMSFB_OT_MATROXFB:
            	case MMSFB_OT_VIAFB:
            	case MMSFB_OT_X11:
            	case MMSFB_OT_DAVINCIFB:
            		// okay
            		break;
            	default:
            		WRONG_VALUE(parname, val, MMSFB_OT_VALID_VALUES_BE_DFB, "-> this depends on backend=\"DFB\"");
            	}
            }
            else
			if (this->graphics.backend == MMSFB_BE_X11) {
				switch (this->graphics.outputtype) {
				case MMSFB_OT_X11:
				case MMSFB_OT_XSHM:
				case MMSFB_OT_XVSHM:
					// okay
					break;
				default:
					WRONG_VALUE(parname, val, MMSFB_OT_VALID_VALUES_BE_X11, "-> this depends on backend=\"X11\"");
				}
			}
            else
			if (this->graphics.backend == MMSFB_BE_FBDEV) {
				switch (this->graphics.outputtype) {
            	case MMSFB_OT_VESAFB:
            	case MMSFB_OT_MATROXFB:
            	case MMSFB_OT_DAVINCIFB:
					// okay
					break;
				default:
					WRONG_VALUE(parname, val, MMSFB_OT_VALID_VALUES_BE_FBDEV, "-> this depends on backend=\"FBDEV\"");
				}
			}
        }
		else if(!xmlStrcmp(parname, (const xmlChar *) "videolayerid"))
			this->graphics.videolayerid = atoi((const char *)parvalue);
	    else if(!xmlStrcmp(parname, (const xmlChar *) "videolayerpixelformat")) {
        	string val = string((const char *)parvalue);
            if ((this->graphics.videolayerpixelformat = getMMSFBPixelFormatFromString(strToUpr(val))) == MMSFB_PF_NONE)
            	WRONG_VALUE(parname, val, MMSFB_PF_VALID_VALUES_LAYER, "");
	    }
	    else if(!xmlStrcmp(parname, (const xmlChar *) "videolayeroptions"))
	        this->graphics.videolayeroptions = strToUpr(string((const char *)parvalue));
	    else if(!xmlStrcmp(parname, (const xmlChar *) "videolayerbuffermode"))
	        this->graphics.videolayerbuffermode = strToUpr(string((const char *)parvalue));
		else if(!xmlStrcmp(parname, (const xmlChar *) "graphicslayerid"))
			this->graphics.graphicslayerid = atoi((const char *)parvalue);
		else if(!xmlStrcmp(parname, (const xmlChar *) "graphicslayerpixelformat")) {
        	string val = string((const char *)parvalue);
            if ((this->graphics.graphicslayerpixelformat = getMMSFBPixelFormatFromString(strToUpr(val))) == MMSFB_PF_NONE)
            	WRONG_VALUE(parname, val, MMSFB_PF_VALID_VALUES_LAYER, "");
	    }
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
        	string val = string((const char *)parvalue);
            if ((this->graphics.pointer = getMMSFBPointerModeFromString(strToUpr(val))) == MMSFB_PM_NONE)
            	WRONG_VALUE(parname, val, MMSFB_PM_VALID_VALUES, "");
        }
        else if(!xmlStrcmp(parname, (const xmlChar *) "graphicswindowpixelformat")) {
        	string val = string((const char *)parvalue);
            if ((this->graphics.graphicswindowpixelformat = getMMSFBPixelFormatFromString(strToUpr(val))) == MMSFB_PF_NONE) {
            	string val2 = strToUpr(val);
            	if ((val2 != "") && (val2 != "AUTODETECT") && (val2 != "AUTO"))
            		WRONG_VALUE(parname, val, MMSFB_PF_VALID_VALUES_WINDOWS, "");
            }
	    }
		else if(!xmlStrcmp(parname, (const xmlChar *) "graphicssurfacepixelformat")) {
        	string val = string((const char *)parvalue);
            if ((this->graphics.graphicssurfacepixelformat = getMMSFBPixelFormatFromString(strToUpr(val))) == MMSFB_PF_NONE) {
            	string val2 = strToUpr(val);
            	if ((val2 != "") && (val2 != "AUTODETECT") && (val2 != "AUTO"))
            		WRONG_VALUE(parname, val, MMSFB_PF_VALID_VALUES_SURFACES, "");
            }
	    }
        else if(!xmlStrcmp(parname, (const xmlChar *) "extendedaccel"))
            this->graphics.extendedaccel = strToBool(string((const char *)parvalue));
		else if(!xmlStrcmp(parname, (const xmlChar *) "allocmethod"))
			this->graphics.allocmethod = strToUpr(string((const char *)parvalue));
        else if(!xmlStrcmp(parname, (const xmlChar *) "fullscreen")) {
        	string val = string((const char *)parvalue);
            if ((this->graphics.fullscreen = getMMSFBFullScreenModeFromString(strToUpr(val))) == MMSFB_FSM_NONE)
            	WRONG_VALUE(parname, val, MMSFB_FSM_VALID_VALUES, "");
        } else if(!xmlStrcmp(parname, (const xmlChar *) "hideapplication")) {
        	this->graphics.hideapplication = strToBool(string((const char *)parvalue));
        }
        else
			printf("RcParser: ignoring parameter '%s' in tag <graphics/>\n", parname);

	    xmlFree(parname);
	    xmlFree(parvalue);

	}

	// checking layer ids
	if (this->graphics.backend == MMSFB_BE_X11) {
		switch (this->graphics.outputtype) {
		case MMSFB_OT_X11:
		case MMSFB_OT_XVSHM:
			if (this->graphics.videolayerid != 0)
				WRONG_VALUE("videolayerid", iToStr(this->graphics.videolayerid), "0", "-> this depends on backend=\"X11\", outputtype=\"X11/XVSHM\"");
			if (this->graphics.graphicslayerid != 0)
				WRONG_VALUE("graphicslayerid", iToStr(this->graphics.graphicslayerid), "0", "-> this depends on backend=\"X11\", outputtype=\"X11/XVSHM\"");
			break;
		case MMSFB_OT_XSHM:
			if (this->graphics.videolayerid != 0)
				WRONG_VALUE("videolayerid", iToStr(this->graphics.videolayerid), "0", "-> this depends on backend=\"X11\", outputtype=\"XSHM\"");
			if (this->graphics.graphicslayerid != 0)
				WRONG_VALUE("graphicslayerid", iToStr(this->graphics.graphicslayerid), "0", "-> this depends on backend=\"X11\", outputtype=\"XSHM\"");
			break;
		default:
			break;
		}
	}
	else
	if (this->graphics.backend == MMSFB_BE_FBDEV) {
		switch (this->graphics.outputtype) {
		case MMSFB_OT_DAVINCIFB:
			if (this->graphics.videolayerid != 1)
				WRONG_VALUE("videolayerid", iToStr(this->graphics.videolayerid), "1", "-> this depends on backend=\"FBDEV\", outputtype=\"DAVINCIFB\"");
			if (this->graphics.graphicslayerid != 0)
				WRONG_VALUE("graphicslayerid", iToStr(this->graphics.graphicslayerid), "0", "-> this depends on backend=\"FBDEV\", outputtype=\"DAVINCIFB\"");
			break;
		default:
			break;
		}
	}

	// checking pixelformats
	if (this->graphics.backend == MMSFB_BE_X11) {
		switch (this->graphics.outputtype) {
		case MMSFB_OT_X11:
		case MMSFB_OT_XVSHM:
			if (this->graphics.videolayerpixelformat != MMSFB_PF_YV12)
				WRONG_VALUE("videolayerpixelformat", getMMSFBPixelFormatString(this->graphics.videolayerpixelformat), MMSFB_PF_VALID_VALUES_BE_X11_OT_XVSHM, "-> this depends on backend=\"X11\", outputtype=\"X11/XVSHM\"");
			if (this->graphics.graphicslayerpixelformat != MMSFB_PF_YV12)
				WRONG_VALUE("graphicslayerpixelformat", getMMSFBPixelFormatString(this->graphics.graphicslayerpixelformat), MMSFB_PF_VALID_VALUES_BE_X11_OT_XVSHM, "-> this depends on backend=\"X11\", outputtype=\"X11/XVSHM\"");
			break;
		case MMSFB_OT_XSHM:
			if (this->graphics.videolayerpixelformat != MMSFB_PF_RGB32)
				WRONG_VALUE("videolayerpixelformat", getMMSFBPixelFormatString(this->graphics.videolayerpixelformat), MMSFB_PF_VALID_VALUES_BE_X11_OT_XSHM, "-> this depends on backend=\"X11\", outputtype=\"XSHM\"");
			if (this->graphics.graphicslayerpixelformat != MMSFB_PF_RGB32)
				WRONG_VALUE("graphicslayerpixelformat", getMMSFBPixelFormatString(this->graphics.graphicslayerpixelformat), MMSFB_PF_VALID_VALUES_BE_X11_OT_XSHM, "-> this depends on backend=\"X11\", outputtype=\"XSHM\"");
			break;
		default:
			break;
		}
	}
	else
	if (this->graphics.backend == MMSFB_BE_FBDEV) {
		switch (this->graphics.outputtype) {
		case MMSFB_OT_DAVINCIFB:
			if (this->graphics.videolayerpixelformat != MMSFB_PF_YUY2)
				WRONG_VALUE("videolayerpixelformat", getMMSFBPixelFormatString(this->graphics.videolayerpixelformat), MMSFB_PF_VALID_VALUES_BE_FBDEV_OT_DAVINCIFB_LAYER_1, "-> this depends on backend=\"FBDEV\", outputtype=\"DAVINCIFB\"");
			if   ((this->graphics.graphicslayerpixelformat != MMSFB_PF_ARGB3565)
				&&(this->graphics.graphicslayerpixelformat != MMSFB_PF_RGB16))
				WRONG_VALUE("graphicslayerpixelformat", getMMSFBPixelFormatString(this->graphics.graphicslayerpixelformat), MMSFB_PF_VALID_VALUES_BE_FBDEV_OT_DAVINCIFB_LAYER_0, "-> this depends on backend=\"FBDEV\", outputtype=\"DAVINCIFB\"");
			break;
		default:
			break;
		}
	}

	if ((this->graphics.backend == MMSFB_BE_X11)||(this->graphics.backend == MMSFB_BE_FBDEV)) {
		// overwite values needed for this backends
		this->graphics.extendedaccel = true;
		this->graphics.allocmethod = "MALLOC";
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
		else if(!xmlStrcmp(cur_node->name, (const xmlChar *) "language"))
			throughLanguage(cur_node);
		else
			printf("RcParser: ignoring tag <%s/>\n", cur_node->name);
	}
}

