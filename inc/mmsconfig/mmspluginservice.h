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

#ifndef MMSPLUGINSERVICE_H_
#define MMSPLUGINSERVICE_H_

#include "mmstools/mmstools.h"
#include "mmsconfig/mmsplugindata.h"
#include "mmsconfig/mmspropertydata.h"
#include "mmsconfig/mmsplugintypedata.h"
#include "mmsconfig/mmsplugincategorydao.h"
#include "mmsconfig/mmsplugintypedao.h"

class MMSPluginService {
    private:
        IMMSDB 		*dbconn;

    public:
        MMSPluginService (DataSource *datasource);
        virtual ~MMSPluginService();

        void setDataSource(DataSource *datasource);
        DataSource *getDataSource();

        void setPlugin(MMSPluginData *data);
        void setPlugin(vector<MMSPluginData *> dataList);

        MMSPluginData *getPluginByID(int);
        MMSPluginData *getPluginByName(string name);
        vector<MMSPluginData*> getAllPlugins(const bool inactiveToo = false);
        vector<MMSPluginData*> getOSDPlugins(const bool inactiveToo = false);
        vector<MMSPluginData*> getCentralPlugins(const bool inactiveToo = false);
        vector<MMSPluginData*> getImportPlugins(const bool inactiveToo = false);
        vector<MMSPluginData*> getBackendPlugins(const bool inactiveToo = false);
        vector<MMSPluginData*> getPluginsByCategory(MMSPluginCategoryData *category, const bool inactiveToo = false);
        vector<MMSPluginData*> getPluginsByType(MMSPluginTypeData *type, const bool inactiveToo = false);
        MMSPluginCategoryData* getPluginCategoryByName(string name);
        vector<MMSPluginCategoryData*> getPluginCategories();
        MMSPluginTypeData*  getPluginTypeByName(string name);

};

#endif /*MMSPLUGINSERVICE_H_*/
