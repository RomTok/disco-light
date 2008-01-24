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

#ifndef MMSPLUGINDAO_H_
#define MMSPLUGINDAO_H_

#include "mmstools/base.h"
#include "mmstools/tools.h"
#include "mmstools/mmserror.h"
#include "mmsconfig/mmsplugindata.h"
#include "mmstools/interfaces/immsdb.h"

class MMSPluginDAO {
    private:
        IMMSDB *dbConnection;

        void setMMSDBConnection(IMMSDB *connection);
        IMMSDB *getMMSDBConnection();

        void save(MMSPluginData *data);
        void update(MMSPluginData *data);

        MMSPluginData *moveRecordToData(MMSRecordSet rs);
        
    public:
        MMSPluginDAO(IMMSDB *connection);
        void deletePlugin(MMSPluginData *plugin);    

        void saveOrUpdate(MMSPluginData *data);
        void saveOrUpdate(vector<MMSPluginData *> dataList);

        MMSPluginData *findPluginByID(int ID);    
        MMSPluginData *findPluginByName(string name);    
        vector<MMSPluginData *> findAllPlugins(const bool inactiveToo = "false");
        vector<MMSPluginData *> findAllPluginsByCategory(MMSPluginCategoryData *category, const bool inactiveToo = "false");
        vector<MMSPluginData *> findAllPluginsByType(MMSPluginTypeData *type, const bool inactiveToo = "false");
        vector<MMSPluginData *> findAllPluginsByType(char *typeName, const bool inactiveToo = "false");
};

#endif /*MMSPLUGINDAO_H_*/
