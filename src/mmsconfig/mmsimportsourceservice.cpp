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

#include "mmsconfig/mmsimportsourceservice.h"
#include "mmsconfig/mmsimportsourcedao.h"

MMSImportSourceService::MMSImportSourceService(DataSource *datasource) :
    dbconn(NULL) {
    MMSDBConnMgr connMgr(datasource);
    if((this->dbconn = connMgr.getConnection()))
    	this->dbconn->connect();
}

MMSImportSourceService::~MMSImportSourceService() {
	if(this->dbconn) {
		this->dbconn->disconnect();
		delete this->dbconn;
	}
}

void MMSImportSourceService::setImportSource(vector<MMSImportSourceData *> dataList) {
    MMSImportSourceDAO myImportSourceDAO(this->dbconn);
    myImportSourceDAO.saveOrUpdate(dataList);
}

vector<MMSImportSourceData *> MMSImportSourceService::getImportSourcesByPlugin(MMSPluginData *plugin) {
    MMSImportSourceDAO myImportSourceDAO(this->dbconn);
    vector<MMSImportSourceData *> importSources;
    importSources = myImportSourceDAO.findImportSourcesByPlugin(plugin);

    /* substitute env vars in the sources */
    for (unsigned i = 0; i < importSources.size(); i++) {
        importSources.at(i)->setSource(substituteEnvVars(importSources.at(i)->getSource()));
    }

    return importSources;
}

MMSImportSourceData * MMSImportSourceService::getImportSourcesByID(int id) {
    MMSImportSourceDAO myImportSourceDAO(this->dbconn);
    return myImportSourceDAO.findImportSourcesByID(id);
}

MMSImportSourceData * MMSImportSourceService::getImportSourcesByName(string name) {
    MMSImportSourceDAO myImportSourceDAO(this->dbconn);
    return myImportSourceDAO.findImportSourcesByName(name);
}
