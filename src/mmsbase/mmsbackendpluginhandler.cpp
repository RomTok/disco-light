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

#include "mmsbackendpluginhandler.h"

MMSBackendPluginHandler::MMSBackendPluginHandler(MMSPluginData plugindata, bool autoload) {
    this->loaded = false;
    this->plugin = NULL;
    this->plugindata = plugindata;
    this->handler = NULL;
    this->switcher = NULL;
    
    if(autoload) {
        this->load();
    }
    
}

MMSBackendPluginHandler::~MMSBackendPluginHandler() {
    if (this->loaded) {
        delete this->plugin;
        delete this->handler;
    }
}

void MMSBackendPluginHandler::invokeInitialize(void *data) {
    if (this->loaded == false)
        throw new MMSBackendPluginHandlerError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->initialize(this->plugindata, this->switcher);
    this->calllock.unlock();
}

void MMSBackendPluginHandler::invokeOnEvent(void *data) {
    if (this->loaded == false)
        throw new MMSBackendPluginHandlerError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->onEvent((IMMSEvent *)data);
    this->calllock.unlock();
} 

void MMSBackendPluginHandler::invokeShutdown(void *data) {
    if (this->loaded == false)
        throw new MMSBackendPluginHandlerError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->shutdown();
    this->calllock.unlock();
}


void MMSBackendPluginHandler::load() {
    NEWBACKENDPLUGIN_PROC newproc;

    if (this->loaded == true)
        throw new MMSBackendPluginHandlerError(0,"OSD Plugin " + this->plugindata.getName() + " is already loaded");
    this->handler = new MMSShlHandler(this->plugindata.getFilename());
    this->handler->open();
    newproc = (NEWBACKENDPLUGIN_PROC)this->handler->getFunction("newBackendPlugin");
    this->plugin = newproc();
    this->loaded = true;
}

void MMSBackendPluginHandler::unload() {
    if (this->loaded == false)
        throw new MMSBackendPluginHandlerError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");
   delete this->plugin;
   delete this->handler;
   this->plugin = NULL;
   this->handler = NULL;
   this->loaded = false;
}

void MMSBackendPluginHandler::setPluginData(MMSPluginData plugindata) {
    this->plugindata = plugindata;
}

MMSPluginData MMSBackendPluginHandler::getPluginData() {
    return this->plugindata;
}

void MMSBackendPluginHandler::setSwitcherInterface(IMMSSwitcher *switcher) {
    this->switcher = switcher;
}

