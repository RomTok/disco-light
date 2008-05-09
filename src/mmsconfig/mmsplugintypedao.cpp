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

#include "mmsconfig/mmsplugintypedao.h"
#include <iostream>

MMSPluginTypeDAO::MMSPluginTypeDAO(IMMSDB *myConnection) {
    MMSPluginTypeDAO::setMMSDBConnection(myConnection);
}

IMMSDB *MMSPluginTypeDAO::getMMSDBConnection() {
    return this->dbConnection;
}

void MMSPluginTypeDAO::setMMSDBConnection(IMMSDB *connection) {
    this->dbConnection = connection;
}

MMSPluginTypeData *MMSPluginTypeDAO::findTypeByName(string name) {
    MMSRecordSet            rs;
    MMSPluginTypeData   *type;
    /* do query */
    this->getMMSDBConnection()->query(
        "select * from PluginTypes where PluginTypeName = '" + name + "'", &rs);

    /* check if result is empty */
    if (rs.getCount()==0)
        return NULL;

    /* create object */
    type = new MMSPluginTypeData();
    type->setID(atoi(rs["ID"].c_str()));
    type->setName(rs["PluginTypeName"]);

    return type;
}

vector<MMSPluginTypeData *> MMSPluginTypeDAO::findAllTypes() {
    MMSRecordSet            rs;
    vector<MMSPluginTypeData   *> types;
    MMSPluginTypeData   *type;
    
    /* do query */
    this->getMMSDBConnection()->query(
        "select * from PluginTypes", &rs);

    /* check if result is empty */
    if (rs.getCount()==0)
        return types;

    /* create object */
    do {
        
        type = new MMSPluginTypeData;
        type->setID(atoi(rs["ID"].c_str()));
        type->setName(rs["PluginTypeName"]);
        types.push_back(type);
        
    } while (rs.next());

    return types;

}
