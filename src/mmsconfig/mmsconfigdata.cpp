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

#include "mmstools/mmstools.h"
#include "mmsconfig/mmsconfigdata.h"

MMSConfigDataDB::MMSConfigDataDB (const string database) :
    dbms(DBMS_SQLITE),
    address(""),
    port(0),
    user(""),
    password(""),
   	database("") {
}


MMSConfigData::MMSConfigData(MMSConfigDataGlobal global,
                             MMSConfigDataDB     configdb,
                             MMSConfigDataDB     datadb,
                             MMSConfigDataDFB    dfb) {
    this->global   = global;
    this->configdb = configdb;
    this->datadb   = datadb;
    this->dfb      = dfb;
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

/* dfb section getters */
const int MMSConfigData::getXres() {
    return this->dfb.xres;
}

const int MMSConfigData::getYres() {
    return this->dfb.yres;
}

const string MMSConfigData::getOutputType() {
    return this->dfb.outputtype;
}

const int MMSConfigData::getVideoLayerId() {
	return this->dfb.videolayerid;
}

const string MMSConfigData::getVideoLayerPixelformat() {
    return this->dfb.videolayerpixelformat;
}

const string MMSConfigData::getVideoLayerOptions() {
    return this->dfb.videolayeroptions;
}

const string MMSConfigData::getVideoLayerBufferMode() {
    return this->dfb.videolayerbuffermode;
}

const int MMSConfigData::getGraphicsLayerId() {
	return this->dfb.graphicslayerid;
}

const string MMSConfigData::getGraphicsLayerPixelformat() {
    return this->dfb.graphicslayerpixelformat;
}

const string MMSConfigData::getGraphicsLayerOptions() {
    return this->dfb.graphicslayeroptions;
}

const string MMSConfigData::getGraphicsLayerBufferMode() {
    return this->dfb.graphicslayerbuffermode;
}

const CVRECT MMSConfigData::getVRect() {
    return this->dfb.vrect;
}

const CVRECT MMSConfigData::getTouchRect() {
    return this->dfb.touchrect;
}

/* static variables */
MMSConfigDataGlobal MMSConfigData::global;
MMSConfigDataDB     MMSConfigData::configdb;
MMSConfigDataDB     MMSConfigData::datadb;
MMSConfigDataDFB    MMSConfigData::dfb;
