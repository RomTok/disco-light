/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2013 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <jens.schneider@diskohq.org>,                   *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mmsbase/mmscentralpluginhandler.h"

MMSCentralPluginHandler::MMSCentralPluginHandler(MMSPluginData plugindata, bool autoload, IMMSCentralPlugin *_plugin) :
	loaded(false),
	initialized(false),
	plugindata(plugindata),
	plugin(_plugin),
	handler(NULL),
	switcher(NULL) {
	if(plugin)
		this->loaded = true;
	else if(autoload)
        this->load();
}

MMSCentralPluginHandler::~MMSCentralPluginHandler() {
    if (this->loaded) {
        delete this->plugin;
        if(this->handler) delete this->handler;
    }
}

void MMSCentralPluginHandler::invokeInitialize(void *data) {
    if (!this->loaded)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is not loaded");
    if (this->initialized)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is already initialized");

    this->calllock.lock();
    this->initialized = this->plugin->initialize(this->plugindata, this->switcher);
    this->calllock.unlock();
}

void MMSCentralPluginHandler::invokeOnEvent(IMMSEvent event) {
    if (!this->loaded)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is not loaded");
    if (!this->initialized)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is not initialized");

    this->calllock.lock();
    this->plugin->onEvent(event);
    this->calllock.unlock();
}

void MMSCentralPluginHandler::invokeShutdown(void *data) {
    if (!this->loaded)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is not loaded");
    if (!this->initialized)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is not initialized");

    this->calllock.lock();
    this->plugin->shutdown();
    this->calllock.unlock();
}

void MMSCentralPluginHandler::invokeShowPreview(void *data) {
    if (!this->loaded)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is not loaded");
    if (!this->initialized)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is not initialized");

    this->calllock.lock();
    if (!this->plugin->showPreview(data)) {
        this->calllock.unlock();
    	throw MMSCentralPluginError(1,"Central Plugin " + this->plugindata.getName() + " has nothing to display (showPreview())");
    }
    this->calllock.unlock();
}

void MMSCentralPluginHandler::invokeShow(void *data) {
    if (!this->loaded)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is not loaded");
    if (!this->initialized)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is not initialized");

    this->calllock.lock();
    if (!this->plugin->show(data)) {
        this->calllock.unlock();
        throw MMSCentralPluginError(1,"Central Plugin " + this->plugindata.getName() + " has nothing to display (show())");
	}
    this->calllock.unlock();
}

bool MMSCentralPluginHandler::isLoaded() {
	return this->loaded;
}

bool MMSCentralPluginHandler::isInitialized() {
	return this->initialized;
}

void MMSCentralPluginHandler::load() {
    if (this->loaded)
        throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is already loaded");

    this->handler = new MMSShlHandler(this->plugindata.getFilename());
    this->handler->open();
    NEWCENTRALPLUGIN_PROC newproc = (NEWCENTRALPLUGIN_PROC)this->handler->getFunction("newCentralPlugin");
    this->plugin = newproc();

    if (this->plugin)
    	this->loaded = true;
}

void MMSCentralPluginHandler::unload() {
	if (!this->loaded)
		throw MMSCentralPluginError(0,"Central Plugin " + this->plugindata.getName() + " is not loaded");

    if (this->plugin) {
 	   delete this->plugin;
 	   this->plugin = NULL;
    }

    if (this->handler) {
 	   delete this->handler;
 	   this->handler = NULL;
    }

    this->loaded = false;
    this->initialized = false;
}

void MMSCentralPluginHandler::setPluginData(MMSPluginData plugindata) {
    this->plugindata = plugindata;
}

MMSPluginData MMSCentralPluginHandler::getPluginData() {
    return this->plugindata;
}

void MMSCentralPluginHandler::setSwitcherInterface(IMMSSwitcher *switcher) {
    this->switcher = switcher;
}

