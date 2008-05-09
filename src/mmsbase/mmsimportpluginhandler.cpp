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

#include "mmsbase/mmsimportpluginhandler.h"

MMSImportPluginHandler::MMSImportPluginHandler(MMSPluginData plugindata, bool autoload) {
    this->loaded = false;
    this->plugin = NULL;
    this->plugindata = plugindata;
    this->handler = NULL;
    
    if(autoload) {
        this->load();
    }
}

MMSImportPluginHandler::~MMSImportPluginHandler() {
    if (this->loaded) {
        delete this->plugin;
        delete this->handler;
    }
}

void MMSImportPluginHandler::invokeInitialize(void *data) {
    if (this->loaded == false)
        throw new MMSImportPluginError(0,"Import Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->initialize(this->plugindata);
    this->calllock.unlock();
}

void MMSImportPluginHandler::invokeExecute(void *data) {
    if (this->loaded == false)
        throw new MMSImportPluginError(0,"Import Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->execute();
    this->calllock.unlock();
}

void MMSImportPluginHandler::invokeShutdown(void *data) {
    if (this->loaded == false)
        throw new MMSImportPluginError(0,"Import Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->shutdown();
    this->calllock.unlock();
}

void MMSImportPluginHandler::invokeCleanUp(void *data) {
    if (this->loaded == false)
        throw new MMSImportPluginError(0,"Import Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->cleanUp();
    this->calllock.unlock();
}

void MMSImportPluginHandler::load() {
    NEWIMPORTPLUGIN_PROC newproc;

    if (this->loaded == true)
        throw new MMSImportPluginError(0,"Import Plugin " + this->plugindata.getName() + " is already loaded");
    this->handler = new MMSShlHandler(this->plugindata.getFilename());
    this->handler->open();
    newproc = (NEWIMPORTPLUGIN_PROC)this->handler->getFunction("newImportPlugin");
    this->plugin = newproc();
    this->loaded = true;
}

void MMSImportPluginHandler::unload() {
    if (this->loaded == false)
        throw new MMSImportPluginError(0,"Import Plugin " + this->plugindata.getName() + " is not loaded");
   delete this->plugin;
   delete this->handler;
   this->plugin = NULL;
   this->handler = NULL;
   this->loaded = false;
}

void MMSImportPluginHandler::setPluginData(MMSPluginData plugindata) {
    this->plugindata = plugindata;
}

MMSPluginData MMSImportPluginHandler::getPluginData() {
    return this->plugindata;
}
