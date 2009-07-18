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

#ifndef MMSCONFIGDATA_H_
#define MMSCONFIGDATA_H_

#include "mmstools/mmstypes.h"
#include "mmstools/mmstools.h"

using namespace std;


class MMSConfigDataGlobal {
    public:
        string logfile;
    	string inputmap;
    	string prefix;
        string theme;
        string sysconfig;
        string data;
        bool   stdout;
        int    inputinterval;
    	string firstplugin;
        bool   shutdown;
        string shutdowncmd;
	    string inputmode;

	    MMSConfigDataGlobal() :
	    	logfile("/tmp/mmscore"),
	    	inputmap("default"),
	    	prefix(""),
	        theme("default"),
	        sysconfig(""),
	        data(""),
	        stdout(false),
	        inputinterval(0),
	    	firstplugin("<none>"),
	        shutdown(false),
	        shutdowncmd(""),
		    inputmode("") {}
};

class MMSConfigDataDB {
    public:
    	string       dbms;
    	string       address;
    	unsigned int port;
    	string       user;
    	string       password;
   	    string       database;

   	    MMSConfigDataDB(const string database = "") :
			dbms(DBMS_SQLITE3),
			address(""),
			port(0),
			user(""),
			password(""),
			database("") {}
};

class MMSConfigDataLayer {
    public:
		int    					id;
    	MMSFBRectangle			rect;
		MMSFBSurfacePixelFormat pixelformat;
	    string 					options;
	    string 					buffermode;

   	    MMSConfigDataLayer() :
   			id(0),
   			rect(MMSFBRectangle(50,50,800,600)),
   			pixelformat(MMSFB_PF_RGB16),
   		    options(""),
   		    buffermode("BACKSYSTEM") {}
};

class MMSConfigDataGraphics {
    public:
	    MMSFBBackend 			backend;
	    MMSFBOutputType			outputtype;
	    MMSConfigDataLayer		videolayer;
	    MMSConfigDataLayer		graphicslayer;
	    MMSFBRectangle			vrect;
	    MMSFBRectangle			touchrect;
	    MMSFBPointerMode		pointer;
	    MMSFBSurfacePixelFormat graphicswindowpixelformat;
	    MMSFBSurfacePixelFormat graphicssurfacepixelformat;
	    bool   					extendedaccel;
	    string 					allocmethod;
	    MMSFBFullScreenMode		fullscreen;
	    bool					hideapplication;

   	    MMSConfigDataGraphics() :
   		    backend(MMSFB_BE_NONE),							// set MMSFB_BE_NONE for compatibility reason
   		    outputtype(MMSFB_OT_NONE),
   		    vrect(MMSFBRectangle(0,0,0,0)),
   		    touchrect(MMSFBRectangle(0,0,0,0)),
   		    pointer(MMSFB_PM_FALSE),						// use the mouse pointer, default no
   		    graphicswindowpixelformat(MMSFB_PF_NONE),		// supported values: ARGB, AiRGB or AYUV, NONE means auto detection
   		    graphicssurfacepixelformat(MMSFB_PF_NONE),		// supported values: ARGB, AiRGB or AYUV, NONE means auto detection
   		    extendedaccel(true),							// use lowlevel disko routines for faster pixel manipulation
   		    allocmethod(""),								// the current alloc method
   		    fullscreen(MMSFB_FSM_FALSE),					// x11 fullscreen?, default no
   		    hideapplication(false) {}
};

class MMSConfigDataLanguage {
	public:
		MMS_LANGUAGE_TYPE sourcelang;
		MMS_LANGUAGE_TYPE defaulttargetlang;
		bool			  addtranslations;
		string			  languagefiledir;

		MMSConfigDataLanguage() :
			sourcelang(MMSLANG_UKN),
			defaulttargetlang(MMSLANG_UKN),
			addtranslations(false),
			languagefiledir("") {}
};

class MMSConfigData {
    private:
    	static MMSConfigDataGlobal 		global;
    	static MMSConfigDataDB     		configdb, datadb;
    	static MMSConfigDataGraphics    graphics;
    	static MMSConfigDataLanguage	language;

    public:
        /* call this to set the values */
        MMSConfigData(MMSConfigDataGlobal 	global,
                      MMSConfigDataDB     	configdb,
                      MMSConfigDataDB     	datadb,
                      MMSConfigDataGraphics graphics,
                      MMSConfigDataLanguage	language);

        /* call if you want to read the values */
        MMSConfigData();
        ~MMSConfigData();

        /* global section getters */
        const string getLogfile();
        const string getInputMap();
        const string getPrefix();
        const string getTheme();
        const string getSysConfig();
        const string getData();
        const bool   getStdout();
        const int    getInputInterval();
        const string getFirstPlugin();
        const bool   getShutdown();
        const string getShutdownCmd();
        const string getInputMode();

        /* db section getters */
        const string       getConfigDBDBMS();
        const string       getConfigDBAddress();
        const unsigned int getConfigDBPort();
        const string       getConfigDBUser();
        const string       getConfigDBPassword();
        const string       getConfigDBDatabase();
        const string       getDataDBDBMS();
        const string       getDataDBAddress();
        const unsigned int getDataDBPort();
        const string       getDataDBUser();
        const string       getDataDBPassword();
        const string       getDataDBDatabase();

        /* graphics section getters */
        const MMSConfigDataLayer getVideoLayer();
        const MMSConfigDataLayer getGraphicsLayer();
        const MMSFBBackend getBackend();
        const MMSFBOutputType getOutputType();
        const MMSFBRectangle getVRect();
        const MMSFBRectangle getTouchRect();
        const MMSFBPointerMode getPointer();
        const MMSFBSurfacePixelFormat getGraphicsWindowPixelformat();
        const MMSFBSurfacePixelFormat getGraphicsSurfacePixelformat();
        const bool   getExtendedAccel();
        const string getAllocMethod();
        const MMSFBFullScreenMode   getFullScreen();
        const bool   getHideApplication();

        /* language section getters */
    	const MMS_LANGUAGE_TYPE getSourceLang();
    	const MMS_LANGUAGE_TYPE getDefaultTargetLang();
    	const bool			    getAddTranslations();
    	const string 			getLanguagefileDir();

};

#endif /*MMSCONFIGDATA_H_*/

