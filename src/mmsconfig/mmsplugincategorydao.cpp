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

#include "mmsconfig/mmsplugincategorydao.h"
#include <iostream>

MMSPluginCategoryDAO::MMSPluginCategoryDAO(IMMSDB *myConnection) {
    MMSPluginCategoryDAO::setMMSDBConnection(myConnection);
}

IMMSDB *MMSPluginCategoryDAO::getMMSDBConnection() {
    return this->dbConnection;
}

void MMSPluginCategoryDAO::setMMSDBConnection(IMMSDB *connection) {
    this->dbConnection = connection;
}

MMSPluginCategoryData *MMSPluginCategoryDAO::findCategoryByName(string name) {
    MMSRecordSet            rs;
    MMSPluginCategoryData   *category;
    /* do query */
    this->getMMSDBConnection()->query(
        "select * from Category where CategoryName = '" + name + "'", &rs);

    /* check if result is empty */
    if (rs.getCount()==0)
        return NULL;

    /* create object */
    category = new MMSPluginCategoryData;
    category->setID(atoi(rs["ID"].c_str()));
    category->setName(rs["CategoryName"]);

    return category;
}

vector<MMSPluginCategoryData *> MMSPluginCategoryDAO::findAllCategories() {
    MMSRecordSet            rs;
    vector<MMSPluginCategoryData   *> categories;
    MMSPluginCategoryData   *category;
    
    /* do query */
    this->getMMSDBConnection()->query(
        "select * from Category", &rs);

    /* check if result is empty */
    if (rs.getCount()==0)
        return categories;

    /* create object */
    do {
        
        category = new MMSPluginCategoryData;
        category->setID(atoi(rs["ID"].c_str()));
        category->setName(rs["CategoryName"]);
        categories.push_back(category);
        
    } while (rs.next());

    return categories;

}
