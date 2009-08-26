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

#include "mmsbase/mmsbackendpluginhandler.h"

MMSBackendPluginHandler::MMSBackendPluginHandler(MMSPluginData plugindata, bool autoload, IMMSBackendPlugin *_plugin) :
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

MMSBackendPluginHandler::~MMSBackendPluginHandler() {
    if (this->loaded) {
        delete this->plugin;
        if(this->handler) delete this->handler;
    }
}

void MMSBackendPluginHandler::invokeInitialize(void *data) {
    if (this->loaded == false)
        throw new MMSBackendPluginHandlerError(0,"Backend Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->initialize(this->plugindata, this->switcher);
    this->calllock.unlock();
}

void MMSBackendPluginHandler::invokeOnEvent(IMMSEvent event) {
    if (this->loaded == false)
        throw new MMSBackendPluginHandlerError(0,"Backend Plugin " + this->plugindata.getName() + " is not loaded");
    this->calllock.lock();
    this->plugin->onEvent(event);
    this->calllock.unlock();
}

void MMSBackendPluginHandler::invokeShutdown(void *data) {
    if (this->loaded == false)
        throw new MMSBackendPluginHandlerError(0,"Backend Plugin " + this->plugindata.getName() + " is not loaded");
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
    if(this->plugin)
    	this->loaded = true;
}

void MMSBackendPluginHandler::unload() {
    if (this->loaded == false)
        throw new MMSBackendPluginHandlerError(0,"OSD Plugin " + this->plugindata.getName() + " is not loaded");
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

void MMSBackendPluginHandler::setPluginData(MMSPluginData plugindata) {
    this->plugindata = plugindata;
}

MMSPluginData MMSBackendPluginHandler::getPluginData() {
    return this->plugindata;
}

void MMSBackendPluginHandler::setSwitcherInterface(IMMSSwitcher *switcher) {
    this->switcher = switcher;
}

