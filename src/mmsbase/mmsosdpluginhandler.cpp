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

#include "mmsbase/mmsosdpluginhandler.h"

MMSOSDPluginHandler::MMSOSDPluginHandler(MMSPluginData plugindata, bool autoload) {
    this->loaded = false;
    this->plugin = NULL;
    this->plugindata = plugindata;
    this->handler = NULL;
    this->switcher = NULL;
    
    if(autoload) {
        this->load();
    }
}

MMSOSDPluginHandler::~MMSOSDPluginHandler() {
    if (this->loaded) {
        delete this->plugin;
        delete this->handler;
    }
}

void MMSOSDPluginHandler::invokeInitialize(void *data) {
    if (this->loaded == false)
        throw new MMSOSDPluginError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->initialize(this->plugindata, this->switcher);
    this->calllock.unlock();
}

void MMSOSDPluginHandler::invokeOnEvent(IMMSEvent event) {
    if (this->loaded == false)
        throw new MMSOSDPluginError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->onEvent(event);
    this->calllock.unlock();
}

void MMSOSDPluginHandler::invokeShutdown(void *data) {
    if (this->loaded == false)
        throw new MMSOSDPluginError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->shutdown();
    this->calllock.unlock();
}

void MMSOSDPluginHandler::invokeShowPreview(void *data) {
    if (this->loaded == false)
        throw new MMSOSDPluginError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");

    this->calllock.lock();
    if (!this->plugin->showPreview(data)) {
        this->calllock.unlock();
        throw new MMSOSDPluginError(1,"OSD Plugin " + this->plugindata.getName() + " has nothing to display (showPreview())");
    }
    this->calllock.unlock();
    
}

void MMSOSDPluginHandler::invokeShow(void *data) {
    if (this->loaded == false)
        throw new MMSOSDPluginError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");

    this->calllock.lock();
    if (!this->plugin->show(data)) {
        this->calllock.unlock();
        throw new MMSOSDPluginError(1,"OSD Plugin " + this->plugindata.getName() + " has nothing to display (show())");
    }
    this->calllock.unlock();
	
}

void MMSOSDPluginHandler::load() {
    NEWOSDPLUGIN_PROC newproc;

    if (this->loaded == true)
        throw new MMSOSDPluginError(0,"OSD Plugin " + this->plugindata.getName() + " is already loaded");
    this->handler = new MMSShlHandler(this->plugindata.getFilename());
    this->handler->open();
    newproc = (NEWOSDPLUGIN_PROC)this->handler->getFunction("newOSDPlugin");
    this->plugin = newproc();
    this->loaded = true;
}

void MMSOSDPluginHandler::unload() {
    if (this->loaded == false)
        throw new MMSOSDPluginError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");
   delete this->plugin;
   delete this->handler;
   this->plugin = NULL;
   this->handler = NULL;
   this->loaded = false;
}

void MMSOSDPluginHandler::setPluginData(MMSPluginData plugindata) {
    this->plugindata = plugindata;
}

MMSPluginData MMSOSDPluginHandler::getPluginData() {
    return this->plugindata;
}

void MMSOSDPluginHandler::setSwitcherInterface(IMMSSwitcher *switcher) {
    this->switcher = switcher;
}
