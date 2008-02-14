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

#include "mmsconfig/mmsplugindao.h"
#include "mmstools/mmstools.h"
#include "mmsconfig/mmsconfigqueries.h"
#include <iostream>

MMSPluginDAO::MMSPluginDAO(IMMSDB *myConnection) {
    this->setMMSDBConnection(myConnection);
}

IMMSDB *MMSPluginDAO::getMMSDBConnection() {
    return this->dbConnection;
}

void MMSPluginDAO::setMMSDBConnection(IMMSDB *connection) {
    this->dbConnection = connection;
}

void MMSPluginDAO::deletePlugin(MMSPluginData *plugin) {
    // delete from Plugins where (id = 'plugin->id');
}


void MMSPluginDAO::save(MMSPluginData *data) {
    MMSRecordSet    rs;

    /* do the insert */
    this->getMMSDBConnection()->query(PLUGINDAO_SAVE(iToStr(data->getType()->getID()), 
    									data->getName(),
    									data->getTitle(),
    									data->getDescription(),
    									data->getFilename(),
    									data->getPath(),
    									((data->getActive())?"Y":"N"),
    									data->getIcon(),
    									data->getSelectedIcon(),	
    									data->getSmallIcon(),
    									data->getSmallSelectedIcon()));
    

    /* set the ID */
    data->setId(this->getMMSDBConnection()->getLastInsertedID());
}

void MMSPluginDAO::update(MMSPluginData *data) {
    /* do the update */
    this->getMMSDBConnection()->query(PLUGINDAO_UPDATE(
    								  	data->getFilename(),
    								  	((data->getActive())?"Y":"N"),
        								data->getDescription(),
        								iToStr(data->getId())));
}

void MMSPluginDAO::saveOrUpdate(MMSPluginData *data) {
    /* check if ID is set */
    if (data->getId()<0)
        /* no, have to save */
        save(data);
    else
        /* yes, have to update */
        update(data);
}

void MMSPluginDAO::saveOrUpdate(vector<MMSPluginData *> dataList) {
    /* for each item */
    for (unsigned i=0;i<dataList.size();i++) {
        /* check if ID is set */
        if (dataList.at(i)->getId()<0)
            /* no, have to save */
            save(dataList.at(i));
        else
            /* yes, have to update */
            update(dataList.at(i));
    }
}

MMSPluginData *MMSPluginDAO::moveRecordToData(MMSRecordSet rs) {
    MMSPluginData *data = new MMSPluginData();

    data->setId(atoi(rs["ID"].c_str()));
    data->setName(rs["PluginName"]);
    data->setTitle(rs["PluginTitle"]);
    data->setDescription(rs["PluginDescription"]);
    data->setFilename(rs["Filename"]);
    data->setPath(rs["PluginPath"]);
    data->setActive((rs["Active"] == "Y"));
    data->setIcon(rs["Icon"]);
    data->setSelectedIcon(rs["SelectedIcon"]);
    data->setSmallIcon(rs["SmallIcon"]);
    data->setSmallSelectedIcon(rs["SmallSelectedIcon"]);
    
    return data;
}

MMSPluginData *MMSPluginDAO::findPluginByName(string name) {
    MMSPluginData   *plugin;
    MMSRecordSet    rs;

    /* select a plugin */
    this->getMMSDBConnection()->query(PLUGINDAO_F_PLUGIN_BY_NAME(name), &rs);

    /* check if result is empty */
    if (rs.getCount()==0)
        return NULL;
    
    /* set the values */
    plugin = moveRecordToData(rs);
    MMSPluginCategoryData *category = new MMSPluginCategoryData();
    MMSPluginTypeData *plugintype = new MMSPluginTypeData();

    if(!rs["CategoryID"].empty())
        category->setID(atoi(rs["CategoryID"].c_str()));
    
    if(!rs["CategoryName"].empty())
        category->setName(rs["CategoryName"]);

    if(!rs["PluginTypeID"].empty())
        plugintype->setID(atoi(rs["PluginTypeID"].c_str()));
    plugintype->setName(rs["PluginTypeName"]);
    plugin->setType(plugintype);
    plugin->setCategory(category);

    return plugin;

}

MMSPluginData *MMSPluginDAO::findPluginByID(int id) {
    MMSPluginData   *plugin;
    MMSRecordSet    rs;

    /* select a plugin */
    this->getMMSDBConnection()->query(PLUGINDAO_F_PLUGIN_BY_ID(iToStr(id)), &rs);

    /* check if result is empty */
    if (rs.getCount()==0)
        return NULL;
    
    /* set the values */
    plugin = moveRecordToData(rs);
    MMSPluginCategoryData *category = new MMSPluginCategoryData();
    MMSPluginTypeData *plugintype = new MMSPluginTypeData();

    if(!rs["CategoryID"].empty())
        category->setID(atoi(rs["CategoryID"].c_str()));
    
    if(!rs["CategoryName"].empty())
        category->setName(rs["CategoryName"]);

    if(!rs["PluginTypeID"].empty())
        plugintype->setID(atoi(rs["PluginTypeID"].c_str()));
    plugintype->setName(rs["PluginTypeName"]);
    plugin->setType(plugintype);
    plugin->setCategory(category);

    return plugin;
}

vector<MMSPluginData *> MMSPluginDAO::findAllPlugins(const bool inactiveToo){
    string                  query;    
    vector<MMSPluginData *> pluginList;
    MMSRecordSet            rs;

    if(!inactiveToo)
    	query = PLUGINDAO_FIND_ALL_ACTIVE_PLUGINS;
    else
    	query = PLUGINDAO_FIND_ALL_PLUGINS;
    
    this->getMMSDBConnection()->query(query, &rs);
    
    /* check if result is empty */
    if (rs.getCount()==0) return pluginList;

    /* for each result record */
    do {
        /* set the values */
        MMSPluginData *plugin = moveRecordToData(rs);
		MMSPluginCategoryData *category = new MMSPluginCategoryData();
        MMSPluginTypeData *plugintype = new MMSPluginTypeData();

        if(!rs["CategoryID"].empty())
        	category->setID(atoi(rs["CategoryID"].c_str()));
        
        if(!rs["CategoryName"].empty())
            category->setName(rs["CategoryName"]);

		if(!rs["PluginTypeID"].empty())
        	plugintype->setID(atoi(rs["PluginTypeID"].c_str()));
        plugintype->setName(rs["PluginTypeName"]);
        plugin->setType(plugintype);
        plugin->setCategory(category);

        /* push to list */
        pluginList.push_back(plugin);
    } while(rs.next() == true);
    
    return pluginList;
}

vector<MMSPluginData *> MMSPluginDAO::findAllPluginsByCategory(MMSPluginCategoryData *category, const bool inactiveToo){
    string                  query;    
    vector<MMSPluginData *> pluginList;
    MMSRecordSet            rs;
    
    // select all plugins
    if(!inactiveToo)
    	query = PLUGINDAO_F_ACTIVE_PLUGINS_BY_CATEGORY(category->getName());
    else
    	query = PLUGINDAO_F_ALL_PLUGINS_BY_CATEGORY(category->getName());
    	
    this->getMMSDBConnection()->query(query, &rs);

    /* check if result is empty */
    if (rs.getCount()==0) return pluginList;

    /* for each result record */
    do {
        /* set the values */
        MMSPluginData *plugin = moveRecordToData(rs);
        MMSPluginTypeData *plugintype = new MMSPluginTypeData();

        plugintype->setID(atoi(rs["PluginTypeID"].c_str()));
        plugintype->setName(rs["PluginTypeName"]);
        plugin->setType(plugintype);

        /* push to list */
        pluginList.push_back(plugin);
    } while(rs.next() == true);
        
    return pluginList;
}

vector<MMSPluginData *> MMSPluginDAO::findAllPluginsByType(MMSPluginTypeData *type, const bool inactiveToo){
        
    return this->findAllPluginsByType((char *)type->getName().c_str(), inactiveToo);
}

vector<MMSPluginData *> MMSPluginDAO::findAllPluginsByType(string typeName, const bool inactiveToo) {
    string                  query;    
    vector<MMSPluginData *> pluginList;
    MMSRecordSet            rs;
    
    // select all plugins
    if(!inactiveToo)
    	query = PLUGINDAO_F_ACTIVE_PLUGINS_BY_TYPE(typeName);
    else
    	query = PLUGINDAO_F_ALL_PLUGINS_BY_TYPE(typeName);

    this->getMMSDBConnection()->query(query, &rs);

    DEBUGMSG("MMSPluginDAO", "Found %d records.", rs.getCount());

    /* check if result is empty */
    if (rs.getCount()==0) return pluginList;
    
    // rewind the resultset
    rs.setRecordNum(0);
    
    /* for each result record */
    do {
        /* set the values */
        MMSPluginData *plugin = moveRecordToData(rs);
        MMSPluginTypeData *plugintype = new MMSPluginTypeData();
        MMSPluginCategoryData *plugincategory = new MMSPluginCategoryData();

        if(!rs["CategoryID"].empty())
            plugincategory->setID(atoi(rs["CategoryID"].c_str()));
        
        if(!rs["CategoryName"].empty())
            plugincategory->setName(rs["CategoryName"]);

        plugintype->setID(atoi(rs["PluginTypeID"].c_str()));
        plugintype->setName(rs["PluginTypeName"]);
        plugin->setType(plugintype);
        plugin->setCategory(plugincategory);

        /* push to list */
        pluginList.push_back(plugin);
    } while(rs.next() == true);
        
    return pluginList;
}
