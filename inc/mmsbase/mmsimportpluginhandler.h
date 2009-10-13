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

#ifndef MMSIMPORTPLUGINHANDLER_H_
#define MMSIMPORTPLUGINHANDLER_H_

#include "mmstools/base.h"
#include "mmstools/mmserror.h"
#include "mmstools/mmsshlhandler.h"
#include "mmstools/mmsmutex.h"
#include "mmsconfig/mmsplugindata.h"
#include "mmsbase/interfaces/immsimportplugin.h"

MMS_CREATEERROR(MMSImportPluginError);


class MMSImportPluginHandler {
    private:
        bool 				loaded;
        MMSPluginData 		plugindata;
        IMMSImportPlugin	*plugin;
        MMSShlHandler 		*handler;
        MMSMutex 			calllock;

    public:
        /**
         * Constructor for the plugin handler of a backend plugin.
         *
         * @param	plugindata	The plugindata object as usually read from the database.
         * @param	autoload	If true, the plugin will be loaded (no effect if _plugin is set)
         * @param	_plugin		Pointer to already instantiated plugin object. Usually used for static plugins.
         */
        MMSImportPluginHandler(MMSPluginData plugindata, bool autoload = false, IMMSImportPlugin *_plugin = NULL);
        ~MMSImportPluginHandler();

        bool isLoaded();
        void load();
        void unload();
        MMSPluginData getPluginData();
        void setPluginData(MMSPluginData plugindata);
        void invokeInitialize(void *data = NULL);
        void invokeExecute(void *data = NULL);
        void invokeShutdown(void *data = NULL);
        void invokeCleanUp(void *data = NULL);
};

#endif /*MMSIMPORTPLUGINHANDLER_H_*/
