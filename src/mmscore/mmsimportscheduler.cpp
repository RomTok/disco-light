/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
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

#include "mmsimportscheduler.h"

#define SCHEDULER_SLEEP_TIME 10 /* sleep time in seconds */

MMSImportScheduler::MMSImportScheduler(MMSPluginManager *pluginManager) {
    logger.setIdentity("IMPORTSCHEDULER");

    MMSConfigData *config = new MMSConfigData();
    DataSource    *source = new DataSource(config->getConfigDBDBMS(), 
    		                               config->getConfigDBDatabase(),
    		                               config->getConfigDBAddress(),
    		                               config->getConfigDBPort(),
    		                               config->getConfigDBUser(),
    		                               config->getConfigDBPassword());
    delete config;

    /* run with the minimum priority */	
//	this->setSchedulePriority(PRIORITY_MIN);

    /* save access to plugin manager */
    this->pluginManager = pluginManager;

    /* initialize the service objects */
    this->pluginService = new MMSPluginService(source);
    this->importPropertyService = new MMSImportPropertyService(source);
}

MMSImportScheduler::~MMSImportScheduler() {
    for(vector<IMPORT_PLUGINS*>::iterator it(importPlugins.begin()); it != importPlugins.end(); ++it) {
        delete(((IMPORT_PLUGINS*)(*it))->plugin);
        delete(((IMPORT_PLUGINS*)(*it))->importProperty);
        delete(((IMPORT_PLUGINS*)(*it))->pluginHandler);
        delete(*it);
    }
    delete this->pluginService;
    delete this->importPropertyService;
}

void MMSImportScheduler::getImportPlugins() {
    vector<MMSPluginData *> pluginList;

    logger.writeLog("getImportPlugins()");
    /* get all import plugins */
    pluginList = this->pluginService->getImportPlugins();

    int base_time = time(NULL);

    /* go through the old importPlugins list -> delete entries */
    for(unsigned i=0; i<importPlugins.size(); i++) {
        /* check if entry is in pluginList */
        bool found=false;
        unsigned j=0;
        logger.writeLog("work with " + importPlugins.at(i)->plugin->getName());
        while(j<pluginList.size()) {
            if (importPlugins.at(i)->plugin->getId() == pluginList.at(j)->getId()) {
                found=true;
                break;
            }
            j++;
        }

        if (!found) {
            /* delete old entry from importPlugins */
            logger.writeLog("delete " + importPlugins.at(i)->plugin->getName());
            importPlugins.erase(importPlugins.begin()+i);
        }
        if (!importPlugins.at(i)->plugin->getActive()) {
            /* delete incactive plugin */
            logger.writeLog("delete inactive" + importPlugins.at(i)->plugin->getName());
            importPlugins.erase(importPlugins.begin()+i);
        }
        
    }

    /* go through the new plugin list -> get import properties for each import plugin */
    for(unsigned i=0; i<pluginList.size(); i++) {
        /* check if entry is already in importPlugins */
        bool found=false;
        unsigned j=0;
        while(j<importPlugins.size()) {
            if (pluginList.at(i)->getId() == importPlugins.at(j)->plugin->getId()) {
                found=true;
                break;
            }
            j++;
        }

        if (!pluginList.at(i)->getActive()) {
            /* delete incactive plugin */
            continue;
        }

        if (!found) {
            /* have to create new importPlugins entry */
            IMPORT_PLUGINS *ip = new IMPORT_PLUGINS;
        
            /* fill plugin */
            ip->plugin = pluginList.at(i);
    
            /* fill import property */
            ip->importProperty = this->importPropertyService->getImportPropertyByPlugin(ip->plugin);
    
            /* get the handler */
            ip->pluginHandler = this->pluginManager->getImportPluginHandler(ip->plugin->getId());
    
            /* calculate the nextTime */
            ip->nextTime = base_time;
            if (!ip->importProperty->getOnStartUp())
                if (ip->importProperty->getTime())
                    ip->nextTime += ip->importProperty->getTime();
                else
                    ip->nextTime = 0;
            base_time += SCHEDULER_SLEEP_TIME;
    
            /* add item */
            importPlugins.push_back(ip);
        }
        else {
            /* take the old entry and update it */
            /* update plugin */
            importPlugins.at(j)->plugin = pluginList.at(i);
    
            /* update import property */
            importPlugins.at(j)->importProperty = this->importPropertyService->getImportPropertyByPlugin(importPlugins.at(j)->plugin);
    
            /* get the handler */
            importPlugins.at(j)->pluginHandler = this->pluginManager->getImportPluginHandler(importPlugins.at(j)->plugin->getId());
        }
    }
}

void MMSImportScheduler::threadMain() {
    time_t curr_time;

    /* wait a little so other components can start faster */
    sleep(2);

    try {
	    while(1) {
    	        /* get all import plugins */
    	        getImportPlugins();
    	
    	        /* get the current time */
    	        curr_time = time(NULL);
    	
    	        /* through all my import plugins */
    	        for(int i=0; i<importPlugins.size(); i++) {
    	            /* check something */
    	            if (!importPlugins.at(i)->nextTime) continue;
    	            if (importPlugins.at(i)->nextTime > curr_time) continue;
    	            if (importPlugins.at(i)->importProperty->getInterval() <= 0)
    	                importPlugins.at(i)->nextTime = 0;
    	            else
    	                importPlugins.at(i)->nextTime = curr_time + importPlugins.at(i)->importProperty->getInterval();
    	
    	            /* invoke execute import */
                    /* TODO: open a thread */
                    try {
    	                importPlugins.at(i)->pluginHandler->invokeExecute(NULL);
                    } catch(MMSError *error) { 
                         logger.writeLog("Abort import due to: " + error->getMessage());
                    }
                    
    	        }
    	
    	        sleep(SCHEDULER_SLEEP_TIME);
	    }
    } catch(MMSError *error) {
        logger.writeLog("Abort due to: " + error->getMessage());
        delete error;
    }

    /* self destruct */
    delete this;
}

void MMSImportScheduler::executeImport(int pluginID) {
	for(int i = 0;i<importPlugins.size();i++) {
		if(importPlugins.at(i)->plugin->getId()==pluginID) {
			importPlugins.at(i)->pluginHandler->invokeExecute();
			return;
		}
	}
}

