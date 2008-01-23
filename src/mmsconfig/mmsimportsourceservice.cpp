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

#include "mmsimportsourceservice.h"
#include "mmsimportsourcedao.h"

MMSImportSourceService::MMSImportSourceService(DataSource *datasource) {

	this->setDataSource(datasource);
    
    MMSDBConnMgr connMgr(datasource); 
    this->dbconn = connMgr.getConnection();
}

void MMSImportSourceService::setDataSource(DataSource *datasource) {
    this->datasource = datasource;
}

DataSource *MMSImportSourceService::getDataSource() {
    return this->datasource;
}

void MMSImportSourceService::setImportSource(vector<MMSImportSourceData *> dataList) {

	this->dbconn->connect(this->datasource);
    
    MMSImportSourceDAO myImportSourceDAO(this->dbconn);
    myImportSourceDAO.saveOrUpdate(dataList);
    
    this->dbconn->disconnect();
}

vector<MMSImportSourceData *> MMSImportSourceService::getImportSourcesByPlugin(MMSPluginData *plugin) {

	this->dbconn->connect(this->datasource);
    
    MMSImportSourceDAO myImportSourceDAO(this->dbconn);
    vector<MMSImportSourceData *> importSources;
    importSources = myImportSourceDAO.findImportSourcesByPlugin(plugin);
    
    /* substitute env vars in the sources */
    for (unsigned i = 0; i < importSources.size(); i++) {
        importSources.at(i)->setSource(substituteEnvVars(importSources.at(i)->getSource()));
    }

    this->dbconn->disconnect();
    
    return importSources;
}

MMSImportSourceData * MMSImportSourceService::getImportSourcesByID(int id) {
    MMSImportSourceData *source;

    this->dbconn->connect(this->datasource);
    
    MMSImportSourceDAO myImportSourceDAO(this->dbconn);
    source = myImportSourceDAO.findImportSourcesByID(id);
    
    this->dbconn->disconnect();
    
    return source;
}

MMSImportSourceData * MMSImportSourceService::getImportSourcesByName(string name) {
    MMSImportSourceData *source;

    this->dbconn->connect(this->datasource);
    
    MMSImportSourceDAO myImportSourceDAO(this->dbconn);
    source = myImportSourceDAO.findImportSourcesByName(name);
    
    this->dbconn->disconnect();
    
    return source;
}
