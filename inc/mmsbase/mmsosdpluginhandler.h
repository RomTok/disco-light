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
#ifndef MMSOSDPLUGINHANDLER_H_
#define MMSOSDPLUGINHANDLER_H_

#include "mmstools/base.h"
#include "mmstools/mmserror.h"
#include "mmstools/mmsshlhandler.h"
#include "mmsconfig/mmsplugindata.h"
#include "mmsbase/interfaces/immsosdplugin.h"

MMS_CREATEERROR(MMSOSDPluginError);


class MMSOSDPluginHandler {
    private:
        bool 			loaded;
        MMSPluginData 	plugindata;
        IMMSOSDPlugin 	*plugin;
        MMSShlHandler 	*handler;
        MMSMutex 		calllock;
        IMMSSwitcher 	*switcher;

    public:
        /**
         * Constructor for the plugin handler of an OSD plugin.
         *
         * @param	plugindata	The plugindata object as usually read from the database.
         * @param	autoload	If true, the plugin will be loaded (no effect if _plugin is set)
         * @param	_plugin		Pointer to already instantiated plugin object. Usually used for static plugins.
         */
        MMSOSDPluginHandler(MMSPluginData plugindata, bool autoload = false, IMMSOSDPlugin *_plugin = NULL);
        ~MMSOSDPluginHandler();

        bool isLoaded();
        void load();
        void unload();
        MMSPluginData getPluginData();
        void setPluginData(MMSPluginData plugindata);
        void setSwitcherInterface(IMMSSwitcher *switcher);
        void invokeInitialize(void *data = NULL);
        void invokeOnEvent(IMMSEvent event);
        void invokeShutdown(void *data = NULL);
        void invokeShowPreview(void *data = NULL);
        void invokeShow(void *data = NULL);
};

#endif /*MMSOSDPLUGINHANDLER_H_*/
