/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2013 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <jens.schneider@diskohq.org>,                   *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mmsconfig/mmsplugincategorydao.h"
#include <iostream>
#include <stdlib.h>

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
