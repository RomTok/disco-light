/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
 ***************************************************************************/

#include "mmsbase/mmsosdpluginhandler.h"

MMSOSDPluginHandler::MMSOSDPluginHandler(MMSPluginData plugindata, bool autoload, IMMSOSDPlugin *_plugin) :
	loaded(false),
	plugindata(plugindata),
	plugin(_plugin),
	handler(NULL),
	switcher(NULL) {
	if(plugin)
		this->loaded = true;
	else if(autoload)
        this->load();
}

MMSOSDPluginHandler::~MMSOSDPluginHandler() {
    if (this->loaded) {
        delete this->plugin;
        if(this->handler) delete this->handler;
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
    if(this->plugin)
    	this->loaded = true;
}

void MMSOSDPluginHandler::unload() {
    if (this->loaded == false)
        throw new MMSOSDPluginError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");
   if(this->plugin) {
	   delete this->plugin;
	   this->plugin = NULL;
   }
   if(this->handler) {
	   delete this->handler;
	   this->handler = NULL;
   }
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
