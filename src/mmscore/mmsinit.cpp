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

#include "mmscore/mmsinit.h"

#include "mms.h"

static MMSPluginManager             *pluginmanager      = NULL;
static MMSEventDispatcher           *eventdispatcher    = NULL;
static MMSEventSignupManager        *eventsignupmanager = NULL;
static MMSConfigData                *config             = NULL;
static MMSEvent                     *masterevent        = NULL;
static MMSEventSignup               *mastereventsignup  = NULL;
/* static MMSImportScheduler           *importscheduler    = NULL; */
static MMSInputManager              *inputs             = NULL;
static MMSConfigDataGlobal          rcGlobal;
static MMSConfigDataDB              rcConfigDB, rcDataDB;
static MMSConfigDataDFB             rcDFB;
static MMSWindowManager             *windowmanager = NULL;

static void on_exit() {
	if(pluginmanager) delete pluginmanager;
}


bool mmsInit(MMSINIT_FLAGS flags, int argc, char *argv[], string configfile) {

	try {
        MMSRcParser rcparser;

        if(configfile != "") {
        	printf("set configfile: %s\n", configfile.c_str());
	        rcparser.parseFile(configfile);
	        rcparser.getMMSRc(rcGlobal, rcConfigDB, rcDataDB, rcDFB);
        } else {

		    try {
		    	string filename = getenv("HOME") + string("/.disko/diskorc.xml");
		        rcparser.parseFile("./etc/diskorc.xml");
		        rcparser.getMMSRc(rcGlobal, rcConfigDB, rcDataDB, rcDFB);

		    } catch (MMSRcParserError *ex) {
		        try {
		        rcparser.parseFile("/etc/diskorc.xml");
		        rcparser.getMMSRc(rcGlobal, rcConfigDB, rcDataDB, rcDFB);


		        } catch (MMSRcParserError *ex) {
					rcConfigDB.database  = "/tmp/mmsconfigdb";
					rcDataDB.database    = "/tmp/mmsdatadb";
					rcGlobal.logfile     = "/tmp/mmscore";
		            rcGlobal.theme       = "default";
		            rcGlobal.firstplugin = "<none>";
		        }
		    }
        }
        config = new MMSConfigData(rcGlobal, rcConfigDB, rcDataDB, rcDFB);

        DEBUGMSG("Core", "ConfigDB:                   " + config->getConfigDBDatabase() + " (" + config->getConfigDBAddress() + ")");
        DEBUGMSG("Core", "DataDB:                     " + config->getDataDBDatabase() + " (" + config->getDataDBAddress() + ")");
        DEBUGMSG("Core", "Logfile:                    " + config->getLogfile());
        DEBUGMSG("Core", "First plugin:               " + config->getFirstPlugin());
        DEBUGMSG("Core", "Input map:                  " + config->getInputMap());
        DEBUGMSG("Core", "Prefix:                     " + config->getPrefix());
        DEBUGMSG("Core", "Theme:                      " + config->getTheme());
        DEBUGMSG("Core", "Resolution:                 " + iToStr(config->getXres()) + "x" + iToStr(config->getYres()));
        DEBUGMSG("Core", "Output type:                " + config->getOutputType());
        DEBUGMSG("Core", "Video layer id:             " + iToStr(config->getVideoLayerId()));
        DEBUGMSG("Core", "Video layer pixelformat:    " + config->getVideoLayerPixelformat());
        DEBUGMSG("Core", "Video layer options:        " + config->getVideoLayerOptions());
        DEBUGMSG("Core", "Video layer buffermode:     " + config->getVideoLayerBufferMode());
        DEBUGMSG("Core", "Graphics layer id:          " + iToStr(config->getGraphicsLayerId()));
        DEBUGMSG("Core", "Graphics layer pixelformat: " + config->getGraphicsLayerPixelformat());
        DEBUGMSG("Core", "Graphics layer options:     " + config->getGraphicsLayerOptions());
        DEBUGMSG("Core", "Graphics layer buffermode:  " + config->getGraphicsLayerBufferMode());
        DEBUGMSG("Core", "Visible screen area:        " + iToStr(config->getVRect().x) + "," + iToStr(config->getVRect().y) + "," + iToStr(config->getVRect().w) + "," + iToStr(config->getVRect().h));

        if (config->getStdout())
            DEBUGMSG("Core", "Log to stdout:              yes");
        else
            DEBUGMSG("Core", "Log to stdout:              no");

        DEBUGMSG("Core", "Input Interval:             " + iToStr(config->getInputInterval()) + " ms");

        if (config->getShutdown())
            DEBUGMSG("Core", "Call shutdown command:      yes");
        else
            DEBUGMSG("Core", "Call shutdown command:      no");

        DEBUGMSG("Core", "Shutdown command:           " + config->getShutdownCmd());

        DEBUGMSG("Core", "Touch pad/screen area:      " + iToStr(config->getTouchRect().x) + "," + iToStr(config->getTouchRect().y) + "," + iToStr(config->getTouchRect().w) + "," + iToStr(config->getTouchRect().h));

        DEBUGMSG("Core", "initialize frame buffer");

        mmsfbmanager.init(argc,argv);
        mmsfbmanager.applySettings();

        DEBUGMSG("Core", "starting theme manager");
        themeManager = new MMSThemeManager(config->getData(),config->getTheme());

        if(flags & MMSINIT_WINDOWMANAGER || flags == MMSINIT_FULL) {
            DEBUGMSG("Core", "starting window manager");
            DFBRectangle vrect;
            vrect.x = config->getVRect().x;
            vrect.y = config->getVRect().y;
            vrect.w = config->getVRect().w;
            vrect.h = config->getVRect().h;

            windowmanager = new MMSWindowManager(vrect);

	        DEBUGMSG("Core", "creating background window");
	        MMSRootWindow *rootwin = new MMSRootWindow("background_rootwindow","100%","100%");
	        rootwin->setWindowManager((IMMSWindowManager*)windowmanager);
	        windowmanager->setBackgroundWindow(rootwin);
        }

        DEBUGMSG("Core", "creating input manager");
        inputs = new MMSInputManager(config->getSysConfig() + "/inputmap.xml", config->getInputMap());
		inputs->setWindowManager((IMMSWindowManager*)windowmanager);

        //inputs
		DEBUGMSG("Core", "add input device");
		inputs->addDevice(DIDID_KEYBOARD, config->getInputInterval());

		DEBUGMSG("Core", "creating master subscription");
	  	MMSInputSubscription sub1(inputs);

        if(flags & MMSINIT_PLUGINMANAGER || flags == MMSINIT_FULL) {
	        DEBUGMSG("Core", "creating PluginManager");
	        pluginmanager = new MMSPluginManager();

	        DEBUGMSG("Core", "loading Backend Plugins...");
	        pluginmanager->loadBackendPlugins();

	        DEBUGMSG("Core", "loading OSD Plugins...");
	        pluginmanager->loadOSDPlugins();

	        DEBUGMSG("Core", "loading Central Plugins...");
	        pluginmanager->loadCentralPlugins();

	        DEBUGMSG("Core", "loading Import Plugins...");
	        pluginmanager->loadImportPlugins();

	        DEBUGMSG("Core", "initialize Import Plugins...");
	        pluginmanager->initializeImportPlugins();
        }

        if(flags & MMSINIT_EVENTS || flags == MMSINIT_FULL) {

        	DEBUGMSG("Core", "creating EventSignupManager");
        	eventsignupmanager = new MMSEventSignupManager();

	        DEBUGMSG("Core", "creating EventDispatcher");
	        eventdispatcher = new MMSEventDispatcher(pluginmanager,eventsignupmanager);

	        masterevent = new MMSEvent();
	        masterevent->setDispatcher(eventdispatcher);

	        MMSPluginData data;
	        mastereventsignup = new MMSEventSignup(data);
	        mastereventsignup->setManager(eventsignupmanager);
        }

//        DEBUGMSG("Core", "starting ImportScheduler");
//        importscheduler = new MMSImportScheduler(pluginmanager);
//        importscheduler->start();

//        DEBUGMSG("Core", "starting music manager");
//        soundmanager = new MMSMusicManager();

        DEBUGMSG("Core", "wait for inputs");
		/* here must be a barrier implemented */
        inputs->startListen();


    	atexit(on_exit);

    } catch(MMSError *error) {
        DEBUGMSG("Core", "Abort due to: " + error->getMessage());
        fprintf(stderr, "Error initializing disko: %s\n", error->getMessage().c_str());
        delete error;
        return false;
    }

	return true;
}

bool registerSwitcher(IMMSSwitcher *switcher) {
    DEBUGMSG("Core", "registering switcher");
    switcher->setInputManager(inputs);
    switcher->setWindowManager((IMMSWindowManager*)windowmanager);
    if(pluginmanager) {
        switcher->setPluginManager(pluginmanager);
        pluginmanager->setSwitcher(switcher);

        DEBUGMSG("Core", "initialize Backend Plugins...");
        pluginmanager->initializeBackendPlugins();

        DEBUGMSG("Core", "initialize OSD Plugins...");
        pluginmanager->initializeOSDPlugins();

        DEBUGMSG("Core", "initialize Central Plugins...");
        pluginmanager->initializeCentralPlugins();
    }

    return true;
}
