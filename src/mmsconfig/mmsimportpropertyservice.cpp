/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
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

#include "mmsconfig/mmsimportpropertyservice.h"
#include "mmstools/interfaces/immsdb.h"
#include "mmsconfig/mmsimportpropertydao.h"

MMSImportPropertyService::MMSImportPropertyService(DataSource *datasource) :
    dbconn(NULL) {
    MMSDBConnMgr connMgr(datasource); 
    if((this->dbconn = connMgr.getConnection()))
        this->dbconn->connect();
}

MMSImportPropertyService::~MMSImportPropertyService() {
	if(this->dbconn) {
		this->dbconn->disconnect();
		delete this->dbconn;
	}
}

void MMSImportPropertyService::setImportProperty(MMSImportPropertyData *data) {
    MMSImportPropertyDAO myImportPropertyDAO(this->dbconn);
    myImportPropertyDAO.saveOrUpdate(data);
}

MMSImportPropertyData *MMSImportPropertyService::getImportPropertyByPlugin(MMSPluginData *plugin) {
    MMSImportPropertyDAO myImportPropertyDAO(this->dbconn);
    MMSImportPropertyData *importProperty = new MMSImportPropertyData();
    importProperty = myImportPropertyDAO.findImportPropertyByPlugin(plugin);
    
    return importProperty;
}
