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

#include "mmstools/mmstools.h"
#include "mmsconfig/mmsconfigdata.h"
#include <string.h>

// static variables
MMSConfigDataGlobal 	MMSConfigData::global;
MMSConfigDataDB     	MMSConfigData::configdb;
MMSConfigDataDB     	MMSConfigData::datadb;
MMSConfigDataGraphics   MMSConfigData::graphics;
MMSConfigDataLanguage   MMSConfigData::language;

MMSConfigData::MMSConfigData(MMSConfigDataGlobal 	global,
                             MMSConfigDataDB     	configdb,
                             MMSConfigDataDB     	datadb,
                             MMSConfigDataGraphics  graphics,
                             MMSConfigDataLanguage	language) {
    this->global   = global;
    this->configdb = configdb;
    this->datadb   = datadb;
    this->graphics = graphics;
    this->language = language;
#ifdef HARD_PREFIX
    this->global.prefix = HARD_PREFIX;
#endif
}

MMSConfigData::MMSConfigData() {
#ifdef HARD_PREFIX
    this->global.prefix = HARD_PREFIX;
#endif
}

MMSConfigData::~MMSConfigData() {
}

/* global section getters */
const string MMSConfigData::getLogfile() {
	return this->global.logfile;
}

const string MMSConfigData::getInputMap() {
	return this->global.inputmap;
}

const string MMSConfigData::getPrefix() {
    if(this->global.prefix != "")
        return this->global.prefix;

    FILE *stream;
    char prefix[1024];
    memset(prefix,0,1024);

    stream = popen("pkg-config --variable=prefix mmstools","r");
    if(stream!=NULL) {
        if(fgets(prefix,1024,stream)!=NULL) {
            prefix[strlen(prefix)-1]='/';
            fclose(stream);
            this->global.prefix = prefix;
            return this->global.prefix;
        }

    }

    stream = fopen("./bin/mmscmd.bin","r");
    if(stream != NULL) {
        sprintf(prefix,"./");
        fclose(stream);
        this->global.prefix = prefix;
        return this->global.prefix;
    }

    return this->global.prefix;
}

const string MMSConfigData::getTheme() {
    return this->global.theme;
}

const string MMSConfigData::getSysConfig(){
	return this->global.sysconfig;
}

const string MMSConfigData::getData(){
	return this->global.data;
}

const bool MMSConfigData::getStdout() {
    return this->global.stdout;
}

const int MMSConfigData::getInputInterval() {
    return this->global.inputinterval;
}

const string MMSConfigData::getFirstPlugin() {
	return this->global.firstplugin;
}

const bool   MMSConfigData::getShutdown() {
	return this->global.shutdown;
}

const string MMSConfigData::getShutdownCmd() {
	return this->global.shutdowncmd;
}

const string MMSConfigData::getInputMode() {
	return this->global.inputmode;
}

/* db section getters */
const string MMSConfigData::getConfigDBDBMS() {
	return this->configdb.dbms;
}

const string MMSConfigData::getConfigDBAddress() {
	return this->configdb.address;
}

const unsigned int MMSConfigData::getConfigDBPort() {
	return this->configdb.port;
}

const string MMSConfigData::getConfigDBUser() {
	return this->configdb.user;
}

const string MMSConfigData::getConfigDBPassword() {
	return this->configdb.password;
}

const string MMSConfigData::getConfigDBDatabase() {
    return this->configdb.database;
};

const string MMSConfigData::getDataDBDBMS() {
	return this->datadb.dbms;
}

const string MMSConfigData::getDataDBAddress() {
	return this->datadb.address;
}

const unsigned int MMSConfigData::getDataDBPort() {
	return this->datadb.port;
}

const string MMSConfigData::getDataDBUser() {
	return this->datadb.user;
}

const string MMSConfigData::getDataDBPassword() {
	return this->datadb.password;
}
const string MMSConfigData::getDataDBDatabase() {
    return this->datadb.database;
};

/* graphics section getters */
const int MMSConfigData::getXres() {
    return this->graphics.xres;
}

const int MMSConfigData::getYres() {
    return this->graphics.yres;
}

/* graphics section getters */
const int MMSConfigData::getXpos() {
    return this->graphics.xpos;
}

const int MMSConfigData::getYpos() {
    return this->graphics.ypos;
}

const MMSFBBackend MMSConfigData::getBackend() {
    return this->graphics.backend;
}

const MMSFBOutputType MMSConfigData::getOutputType() {
    return this->graphics.outputtype;
}

const int MMSConfigData::getVideoLayerId() {
	return this->graphics.videolayerid;
}

const MMSFBSurfacePixelFormat MMSConfigData::getVideoLayerPixelformat() {
    return this->graphics.videolayerpixelformat;
}

const string MMSConfigData::getVideoLayerOptions() {
    return this->graphics.videolayeroptions;
}

const string MMSConfigData::getVideoLayerBufferMode() {
    return this->graphics.videolayerbuffermode;
}

const int MMSConfigData::getGraphicsLayerId() {
	return this->graphics.graphicslayerid;
}

const MMSFBSurfacePixelFormat MMSConfigData::getGraphicsLayerPixelformat() {
    return this->graphics.graphicslayerpixelformat;
}

const string MMSConfigData::getGraphicsLayerOptions() {
    return this->graphics.graphicslayeroptions;
}

const string MMSConfigData::getGraphicsLayerBufferMode() {
    return this->graphics.graphicslayerbuffermode;
}

const MMSFBRectangle MMSConfigData::getVRect() {
    return this->graphics.vrect;
}

const MMSFBRectangle MMSConfigData::getTouchRect() {
    return this->graphics.touchrect;
}

const MMSFBPointerMode MMSConfigData::getPointer() {
    return this->graphics.pointer;
}

const MMSFBSurfacePixelFormat MMSConfigData::getGraphicsWindowPixelformat() {
    return this->graphics.graphicswindowpixelformat;
}

const MMSFBSurfacePixelFormat MMSConfigData::getGraphicsSurfacePixelformat() {
    return this->graphics.graphicssurfacepixelformat;
}

const bool MMSConfigData::getExtendedAccel() {
    return this->graphics.extendedaccel;
}

const string MMSConfigData::getAllocMethod() {
    return this->graphics.allocmethod;
}

const MMSFBFullScreenMode MMSConfigData::getFullScreen() {
	return this->graphics.fullscreen;
}

const bool   MMSConfigData::getHideApplication() {
	return this->graphics.hideapplication;
}


const MMS_LANGUAGE_TYPE MMSConfigData::getSourceLang() {
	return this->language.sourcelang;
}

const MMS_LANGUAGE_TYPE MMSConfigData::getDefaultTargetLang() {
	return this->language.defaulttargetlang;
}

const bool			    MMSConfigData::getAddTranslations() {
	return this->language.addtranslations;
}

const string 			MMSConfigData::getLanguagefileDir() {
	return this->language.languagefiledir;
}

