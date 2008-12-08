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

#include "mmsconfig/mmsconfig.h"
#include "mmscore/mmspluginmanager.h"
#include "mmsbase/interfaces/immsswitcher.h"

MMSPluginManager::MMSPluginManager() {
    MMSConfigData *config = new MMSConfigData();
    this->source          = new DataSource(config->getConfigDBDBMS(),
    		                               config->getConfigDBDatabase(),
    		                               config->getConfigDBAddress(),
    		                               config->getConfigDBPort(),
    		                               config->getConfigDBUser(),
    		                               config->getConfigDBPassword());
    delete config;

    this->service = new MMSPluginService(source);
}

MMSPluginManager::~MMSPluginManager() {
    if(this->source)  delete this->source;
    if(this->service) delete this->service;
    for(vector<MMSOSDPluginHandler*>::iterator it = this->osdPluginHandlers.begin(); it != this->osdPluginHandlers.end(); ++it)
       delete *it;
    for(vector<MMSCentralPluginHandler*>::iterator it = this->centralPluginHandlers.begin(); it != this->centralPluginHandlers.end(); ++it)
       delete *it;
    for(vector<MMSImportPluginHandler*>::iterator it = this->importPluginHandlers.begin(); it != this->importPluginHandlers.end(); ++it)
       delete *it;
    for(vector<MMSBackendPluginHandler*>::iterator it = this->backendPluginHandlers.begin(); it != this->backendPluginHandlers.end(); ++it)
       delete *it;
}

void MMSPluginManager::loadOSDPlugins() {
    vector<MMSPluginData *> data;

    if (this->osdPluginHandlers.size() > 0) {
        throw new MMSPluginManagerError(0,"OSD Plugins already loaded");
    }

    DEBUGMSG("MMSCore", "getOSDPlugins from service");
    data = this->service->getOSDPlugins();

    for(unsigned int i=0;i<data.size();i++) {
        MMSOSDPluginHandler *myhandler;
        myhandler = new MMSOSDPluginHandler(*(data.at(i)),true);
        this->osdPluginHandlers.push_back(myhandler);
        DEBUGMSG("MMSCore", " %s", data.at(i)->getName().c_str());
    }
}

void MMSPluginManager::loadCentralPlugins() {
    vector<MMSPluginData *> data;

    if (this->centralPluginHandlers.size() > 0) {
        throw new MMSPluginManagerError(0,"Central Plugins already loaded");
    }

    data = this->service->getCentralPlugins();

    for(unsigned int i=0;i<data.size();i++) {
        MMSCentralPluginHandler *myhandler;
        myhandler = new MMSCentralPluginHandler(*(data.at(i)),true);
        this->centralPluginHandlers.push_back(myhandler);
        DEBUGMSG("MMSCore", " %s", data.at(i)->getName().c_str());
    }
}

void MMSPluginManager::loadImportPlugins() {
    vector<MMSPluginData *> data;

    if (this->importPluginHandlers.size() > 0) {
        throw new MMSPluginManagerError(0,"Import Plugins already loaded");
    }

    data = this->service->getImportPlugins();

    for(unsigned int i=0;i<data.size();i++) {
        MMSImportPluginHandler *myhandler;
        myhandler = new MMSImportPluginHandler(*(data.at(i)),true);
        this->importPluginHandlers.push_back(myhandler);
        DEBUGMSG("MMSCore", " %s", data.at(i)->getName().c_str());
    }
}

void MMSPluginManager::loadBackendPlugins() {
    vector<MMSPluginData *> data;

    if (this->backendPluginHandlers.size() > 0) {
        throw new MMSPluginManagerError(0,"Backend Plugins already loaded");
    }

    data = this->service->getBackendPlugins();

    for(unsigned int i=0;i<data.size();i++) {
        MMSBackendPluginHandler *myhandler;
        myhandler = new MMSBackendPluginHandler(*(data.at(i)),true);
        this->backendPluginHandlers.push_back(myhandler);
        DEBUGMSG("MMSCore", " %s", data.at(i)->getName().c_str());
    }
}

void MMSPluginManager::initializeOSDPlugins() {
    for(unsigned int i=0;i<this->osdPluginHandlers.size();i++) {
        MMSPluginData pd = this->osdPluginHandlers.at(i)->getPluginData();
        this->osdPluginHandlers.at(i)->setSwitcherInterface(switcher->newSwitcher(&pd));
        this->osdPluginHandlers.at(i)->invokeInitialize();
    }
}

void MMSPluginManager::initializeCentralPlugins() {
    for(unsigned int i=0;i<this->centralPluginHandlers.size();i++) {
        MMSPluginData pd = this->centralPluginHandlers.at(i)->getPluginData();
        this->centralPluginHandlers.at(i)->setSwitcherInterface(switcher->newSwitcher(&pd));
        this->centralPluginHandlers.at(i)->invokeInitialize();
    }
}

void MMSPluginManager::initializeImportPlugins() {
    for(unsigned int i=0;i<this->importPluginHandlers.size();i++) {
        this->importPluginHandlers.at(i)->invokeInitialize();
    }
}

void MMSPluginManager::initializeBackendPlugins() {
    for(unsigned int i=0;i<this->backendPluginHandlers.size();i++) {
        MMSPluginData pd = this->backendPluginHandlers.at(i)->getPluginData();
        this->backendPluginHandlers.at(i)->setSwitcherInterface(switcher->newSwitcher(&pd));
        this->backendPluginHandlers.at(i)->invokeInitialize();
    }
}

vector<MMSOSDPluginHandler *> MMSPluginManager::getOSDPluginHandlers(vector <MMSPluginData *> data) {
    vector<MMSOSDPluginHandler *> myhandlers;

    for(unsigned int y=0; y<data.size();y++) {
        for(unsigned int i = 0; i<osdPluginHandlers.size();i++) {
            if(this->osdPluginHandlers.at(i)->getPluginData().getId() == data.at(y)->getId()) {
                if (this->osdPluginHandlers.at(i)->getPluginData().getType()->getName() == PT_OSD_PLUGIN)
                    myhandlers.push_back(this->osdPluginHandlers.at(i));
                else
                    throw new MMSPluginManagerError(0,"handler for id " + iToStr(data.at(y)->getId()) + " is not a osd plugin");
            }
        }
    }
    return myhandlers;
}

MMSOSDPluginHandler *MMSPluginManager::getOSDPluginHandler(int pluginid) {
    for(unsigned int i=0;i<osdPluginHandlers.size();i++) {
        if(this->osdPluginHandlers.at(i)->getPluginData().getId() == pluginid)
            if (this->osdPluginHandlers.at(i)->getPluginData().getType()->getName() == PT_OSD_PLUGIN)
                return this->osdPluginHandlers.at(i);
            else
                throw new MMSPluginManagerError(0,"handler for id " + iToStr(pluginid) + " is not a osd plugin");
    }

    throw new MMSPluginManagerError(0,"osd plugin handler for id " + iToStr(pluginid) + " was not found");
}

vector<MMSCentralPluginHandler *> MMSPluginManager::getCentralPluginHandlers(vector <MMSPluginData *> data) {
    vector<MMSCentralPluginHandler *> myhandlers;

    for(unsigned int y=0; y<data.size();y++) {
        for(unsigned int i = 0; i<centralPluginHandlers.size();i++) {
            if(this->centralPluginHandlers.at(i)->getPluginData().getId() == data.at(y)->getId()) {
                if (this->centralPluginHandlers.at(i)->getPluginData().getType()->getName() == PT_CENTRAL_PLUGIN)
                    myhandlers.push_back(this->centralPluginHandlers.at(i));
                else
                    throw new MMSPluginManagerError(0,"handler for id " + iToStr(data.at(y)->getId()) + " is not a central plugin");
            }
        }
    }
    return myhandlers;
}

MMSCentralPluginHandler *MMSPluginManager::getCentralPluginHandler(int pluginid) {
    for(unsigned int i=0;i<centralPluginHandlers.size();i++) {
        if(this->centralPluginHandlers.at(i)->getPluginData().getId() == pluginid)
            if (this->centralPluginHandlers.at(i)->getPluginData().getType()->getName() == PT_CENTRAL_PLUGIN)
                return this->centralPluginHandlers.at(i);
            else
                throw new MMSPluginManagerError(0,"handler for id " + iToStr(pluginid) + " is not a central plugin");
    }

    throw new MMSPluginManagerError(0,"central plugin handler for id " + iToStr(pluginid) + " was not found");
}

vector<MMSImportPluginHandler *> MMSPluginManager::getImportPluginHandlers(vector <MMSPluginData *> data) {
    vector<MMSImportPluginHandler *> myhandlers;

    for(unsigned int y=0; y<data.size();y++) {
        for(unsigned int i = 0; i<this->importPluginHandlers.size();i++) {
            if(this->importPluginHandlers.at(i)->getPluginData().getId() == data.at(y)->getId()) {
                if (this->importPluginHandlers.at(i)->getPluginData().getType()->getName() == PT_IMPORT_PLUGIN)
                    myhandlers.push_back(this->importPluginHandlers.at(i));
                else
                    throw new MMSPluginManagerError(0,"handler for id " + iToStr(data.at(y)->getId()) + " is not a import plugin");
            }
        }
    }
    return myhandlers;
}

MMSImportPluginHandler *MMSPluginManager::getImportPluginHandler(int pluginid) {
    for(unsigned int i=0;i<this->importPluginHandlers.size();i++) {
        if(this->importPluginHandlers.at(i)->getPluginData().getId() == pluginid)
            if (this->importPluginHandlers.at(i)->getPluginData().getType()->getName() == PT_IMPORT_PLUGIN)
                return this->importPluginHandlers.at(i);
            else
                throw new MMSPluginManagerError(0,"handler for id " + iToStr(pluginid) + " is not a import plugin");
    }

    throw new MMSPluginManagerError(0,"import plugin handler for id " + iToStr(pluginid) + " was not found");
}

vector<MMSBackendPluginHandler *> MMSPluginManager::getBackendPluginHandlers(vector <MMSPluginData *> data) {
    vector<MMSBackendPluginHandler *> myhandlers;

    for(unsigned int y=0; y<data.size();y++) {
        for(unsigned int i = 0; i<this->backendPluginHandlers.size();i++) {
            if(this->backendPluginHandlers.at(i)->getPluginData().getId() == data.at(y)->getId()) {
                if (this->backendPluginHandlers.at(i)->getPluginData().getType()->getName() == PT_BACKEND_PLUGIN)
                    myhandlers.push_back(this->backendPluginHandlers.at(i));
                else
                    throw new MMSPluginManagerError(0,"handler for id " + iToStr(data.at(y)->getId()) + " is not a backend plugin");
            }
        }
    }
    return myhandlers;
}

MMSBackendPluginHandler *MMSPluginManager::getBackendPluginHandler(int pluginid) {
    for(unsigned int i=0;i<this->backendPluginHandlers.size();i++) {
        if(this->backendPluginHandlers.at(i)->getPluginData().getId() == pluginid)
            if (this->backendPluginHandlers.at(i)->getPluginData().getType()->getName() == PT_BACKEND_PLUGIN)
                return this->backendPluginHandlers.at(i);
            else
                throw new MMSPluginManagerError(0,"handler for id " + iToStr(pluginid) + " is not a backend plugin");
    }

    throw new MMSPluginManagerError(0,"backend plugin handler for id " + iToStr(pluginid) + " was not found");
}


void MMSPluginManager::setActiceOSDPlugin(MMSPluginData *plugin) {
	this->activeosdplugin = plugin;
}

MMSPluginData *MMSPluginManager::getActiveOSDPlugin() {
	return this->activeosdplugin;
}

void MMSPluginManager::setActiceCentralPlugin(MMSPluginData *plugin) {
	this->activecentralplugin = plugin;
}

MMSPluginData *MMSPluginManager::getActiveCentralPlugin() {
	return this->activecentralplugin;
}

void MMSPluginManager::setSwitcher(IMMSSwitcher *switcher) {
	this->switcher = switcher;
}
