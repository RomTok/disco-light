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

#ifndef MMSPLUGINMANAGER_H_
#define MMSPLUGINMANAGER_H_

#include "mmsbase/mmsbase.h"

MMS_CREATEERROR(MMSPluginManagerError);

class MMSPluginManager {
    private:
    	DataSource							*source;
        MMSPluginService                    *service;
        vector<MMSOSDPluginHandler *>       osdPluginHandlers;
        vector<MMSCentralPluginHandler *>   centralPluginHandlers;
        vector<MMSImportPluginHandler *>    importPluginHandlers;
        vector<MMSBackendPluginHandler *>   backendPluginHandlers;
        MMSPluginData						*activeosdplugin;
        MMSPluginData						*activecentralplugin;
        IMMSSwitcher 					    *switcher;

    public:
        MMSPluginManager();
        ~MMSPluginManager();

        void loadOSDPlugins();
        void loadCentralPlugins();
        void loadImportPlugins();
        void loadBackendPlugins();

        void initializeOSDPlugins();
        void initializeCentralPlugins();
        void initializeImportPlugins();
        void initializeBackendPlugins();

        void setActiceOSDPlugin(MMSPluginData *plugin);
        MMSPluginData *getActiveOSDPlugin();

        void setActiceCentralPlugin(MMSPluginData *plugin);
        MMSPluginData *getActiveCentralPlugin();

        void setSwitcher(IMMSSwitcher *switcher);

        vector<MMSOSDPluginHandler *> getOSDPluginHandlers(vector<MMSPluginData *> data);
        MMSOSDPluginHandler          *getOSDPluginHandler(int pluginid);

        vector<MMSCentralPluginHandler *> getCentralPluginHandlers(vector<MMSPluginData *> data);
        MMSCentralPluginHandler          *getCentralPluginHandler(int pluginid);

        vector<MMSImportPluginHandler *> getImportPluginHandlers(vector<MMSPluginData *> data);
        MMSImportPluginHandler          *getImportPluginHandler(int pluginid);

        vector<MMSBackendPluginHandler *> getBackendPluginHandlers(vector<MMSPluginData *> data);
        MMSBackendPluginHandler          *getBackendPluginHandler(int pluginid);
};
#endif /*MMSPLUGINMANAGER_H_*/
