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

#include "mmsconfig/mmsimportpropertydao.h"

MMSImportPropertyDAO::MMSImportPropertyDAO(IMMSDB *connection) {
    setMMSDBConnection(connection);
}

void MMSImportPropertyDAO::setMMSDBConnection(IMMSDB *connection) {
    this->dbConnection = connection;
}

IMMSDB *MMSImportPropertyDAO::getMMSDBConnection() {
    return this->dbConnection;   
}

void MMSImportPropertyDAO::save(MMSImportPropertyData *data) {
    /* do the insert */
    this->getMMSDBConnection()->query(
        "insert into ImportProperties(PluginID,onStartUp,Time,Interval) values('"
        + iToStr(data->getPluginId()) + "','"
        + ((data->getOnStartUp())?"Y":"N") + "','"
        + iToStr(data->getTime()) + "','"
        + iToStr(data->getInterval()) + "')");
/*TODO:return over stack!!!*/
    /* set the ID */
    data->setId(this->getMMSDBConnection()->getLastInsertedID());
}

void MMSImportPropertyDAO::update(MMSImportPropertyData *data) {
    /* do the update */
    this->getMMSDBConnection()->query(
        "update ImportProperties set Time='" + iToStr(data->getTime()) + "',"
        + "onStartUp='" + ((data->getOnStartUp())?"Y":"N") + "',"
        + "Interval='" + iToStr(data->getInterval()) + "' "
        "where ID = '" + iToStr(data->getId()) + "'");
}

void MMSImportPropertyDAO::saveOrUpdate(MMSImportPropertyData *data) {
    /* check if ID is set */
    if (data->getId()<0)
        /* no, have to save */
        save(data);
    else
        /* yes, have to update */
        update(data);
}

MMSImportPropertyData *MMSImportPropertyDAO::moveRecordToData(MMSRecordSet &rs) {
    MMSImportPropertyData *data = new MMSImportPropertyData();

    data->setId(atoi(rs["ID"].c_str()));
    data->setPluginId(atoi(rs["PluginID"].c_str()));
    data->setOnStartUp((rs["onStartUp"]=="Y"));
    data->setTime(atoi(rs["Time"].c_str()));
    data->setInterval(atoi(rs["Interval"].c_str()));
    
    return data;
}

MMSImportPropertyData *MMSImportPropertyDAO::findImportPropertyByPlugin(MMSPluginData *plugin) {
    MMSRecordSet    rs;
    
    this->getMMSDBConnection()->query(
        "select * from ImportProperties where PluginID = " + iToStr(plugin->getId()),
        &rs);

    /* check if result is empty */
    if (rs.getCount()==0)
        throw new MMSImportPropertyDAOError(0,"ImportProperties for PluginID " + iToStr(plugin->getId()) + " not found");

    /* fill the return data */
    return moveRecordToData(rs);
}

