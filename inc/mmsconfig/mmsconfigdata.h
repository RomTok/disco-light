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

using namespace std;



typedef struct {
    int x;
    int y;
    int w;
    int h;
} CVRECT;

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
};

class MMSConfigDataDB {
    public:
    	string       dbms;
    	string       address;
    	unsigned int port;
    	string       user;
    	string       password;
   	    string       database;

   	    MMSConfigDataDB(const string database = "");
};

class MMSConfigDataGraphics {
    public:
	    int    					xres;
	    int    					yres;
	    int    					xpos;
	    int    					ypos;
	    MMSFBBackend 			backend;
	    MMSFBOutputType			outputtype;
		int    					videolayerid;
		MMSFBSurfacePixelFormat videolayerpixelformat;
	    string 					videolayeroptions;
	    string 					videolayerbuffermode;
		int    					graphicslayerid;
		MMSFBSurfacePixelFormat graphicslayerpixelformat;
	    string 					graphicslayeroptions;
	    string 					graphicslayerbuffermode;
	    CVRECT 					vrect;
	    CVRECT 					touchrect;
	    MMSFBPointerMode		pointer;
	    MMSFBSurfacePixelFormat graphicswindowpixelformat;
	    MMSFBSurfacePixelFormat graphicssurfacepixelformat;
	    bool   					extendedaccel;
	    string 					allocmethod;
	    MMSFBFullScreenMode		fullscreen;
	    bool					hideapplication;
};

class MMSConfigDataLanguage {
	public:
		MMS_LANGUAGE_TYPE sourcelang;
		MMS_LANGUAGE_TYPE defaulttargetlang;
		bool			  addtranslations;
		string			  languagefiledir;
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
        const int    getXres();
        const int    getYres();
        const int    getXpos();
        const int    getYpos();
        const MMSFBBackend getBackend();
        const MMSFBOutputType getOutputType();
        const int    getVideoLayerId();
        const MMSFBSurfacePixelFormat getVideoLayerPixelformat();
        const string getVideoLayerOptions();
        const string getVideoLayerBufferMode();
        const int    getGraphicsLayerId();
        const MMSFBSurfacePixelFormat getGraphicsLayerPixelformat();
        const string getGraphicsLayerOptions();
        const string getGraphicsLayerBufferMode();
        const CVRECT getVRect();
        const CVRECT getTouchRect();
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

