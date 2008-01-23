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

#ifndef MMSPLUGINSERVICE_H_
#define MMSPLUGINSERVICE_H_

#include "base.h"
#include "mmsplugindata.h" 
#include "mmspropertydata.h"
#include "mmsplugintypedata.h"
#include "mmsplugincategorydao.h"
#include "mmsplugintypedao.h"
#include "mmsdbconnmgr.h"
#include "interfaces/immsdb.h"
#include "datasource.h"

class MMSPluginService {
    private:
        DataSource 	*datasource;
        IMMSDB 		*dbconn;
    
    public:
        MMSPluginService (DataSource *datasource);
        ~MMSPluginService();
        
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
