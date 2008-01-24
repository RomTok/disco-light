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

#include "mmsconfig/mmspluginservice.h"
#include "mmstools/interfaces/immsdb.h"
#include "mmsconfig/mmsplugindao.h"
#include "mmsconfig/mmspluginpropertydao.h"
#include "mmsconfig/mmsimportpropertydao.h"

MMSPluginService::MMSPluginService(DataSource *datasource) {

	this->setDataSource(datasource);
    
    MMSDBConnMgr connMgr(datasource); 
    this->dbconn = connMgr.getConnection();

    this->dbconn->connect(this->datasource);
}

MMSPluginService::~MMSPluginService() {
	
	this->dbconn->disconnect();
	delete this->dbconn;
}

void MMSPluginService::setDataSource(DataSource *datasource) {
    this->datasource = datasource;
}

DataSource *MMSPluginService::getDataSource() {
    return this->datasource;
}

void MMSPluginService::setPlugin(MMSPluginData *data) {

    
    MMSPluginDAO myPluginDAO(this->dbconn);
    myPluginDAO.saveOrUpdate(data);

    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);
    myPropertyDAO.saveOrUpdate(data);    
    
    return;
}

void MMSPluginService::setPlugin(vector<MMSPluginData *> dataList) {

    MMSPluginDAO myPluginDAO(this->dbconn);
    myPluginDAO.saveOrUpdate(dataList);
    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);
    myPropertyDAO.saveOrUpdate(dataList);    

	return;
}

MMSPluginData *MMSPluginService::getPluginByName(string name) {

    MMSPluginDAO myPluginDAO(this->dbconn);
    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);

    MMSPluginData *plugin = myPluginDAO.findPluginByName(name);
    if(plugin == NULL) {
        return NULL;
    }
    
    vector <MMSPropertyData *> properties;
    properties = myPropertyDAO.findAllPluginPropertiesByPlugin(plugin);
    plugin->setProperties(properties);

    return plugin;    
}

MMSPluginData *MMSPluginService::getPluginByID(int id) {

    MMSPluginDAO myPluginDAO(this->dbconn);
    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);

    MMSPluginData *plugin = myPluginDAO.findPluginByID(id);
    
    vector <MMSPropertyData *> properties;
    properties = myPropertyDAO.findAllPluginPropertiesByPlugin(plugin);
    plugin->setProperties(properties);

    return plugin;    
}

vector<MMSPluginData*> MMSPluginService::getAllPlugins(const bool inactiveToo) {
	MMSLogger logger("MMSCONFIG");

    MMSPluginDAO myPluginDAO(this->dbconn);
	logger.writeLog("create property dao");
    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);

	logger.writeLog("get all plugins");
    vector <MMSPluginData *> pluginList = myPluginDAO.findAllPlugins(inactiveToo);

	logger.writeLog("go through");
    for(unsigned i=0; i<pluginList.size(); i++) {
        vector <MMSPropertyData *> properties;
        properties = myPropertyDAO.findAllPluginPropertiesByPlugin(pluginList.at(i));
        pluginList.at(i)->setProperties(properties);
    }    
    
    return pluginList;    
}

vector<MMSPluginData *> MMSPluginService::getOSDPlugins(const bool inactiveToo) {
	MMSLogger logger("MMSCONFIG");
    
	logger.writeLog("create dao");
    MMSPluginDAO myPluginDAO(this->dbconn);
	logger.writeLog("create property dao");
    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);

	logger.writeLog("get all OSD plugins");
    vector <MMSPluginData *> pluginList = myPluginDAO.findAllPluginsByType(PT_OSD_PLUGIN, inactiveToo);

    for(unsigned i=0; i<pluginList.size(); i++) {
        vector <MMSPropertyData *> properties;
        properties = myPropertyDAO.findAllPluginPropertiesByPlugin(pluginList.at(i));
    	logger.writeLog("get the properties of: " + pluginList.at(i)->getName() + iToStr(pluginList.at(i)->getId()));
        pluginList.at(i)->setProperties(properties);
    }    
    
    logger.writeLog("Working with " + iToStr(pluginList.size()) + " OSD plugins");
    
    return pluginList;    
}

vector<MMSPluginData *> MMSPluginService::getCentralPlugins(const bool inactiveToo) {
	MMSLogger logger("MMSCONFIG");
    

    MMSPluginDAO myPluginDAO(this->dbconn);
    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);

    vector <MMSPluginData *> pluginList = myPluginDAO.findAllPluginsByType(PT_CENTRAL_PLUGIN, inactiveToo);

	logger.writeLog("get the properties of the central plugins.");
    for(unsigned i=0; i<pluginList.size(); i++) {
        vector <MMSPropertyData *> properties;
        properties = myPropertyDAO.findAllPluginPropertiesByPlugin(pluginList.at(i));
        pluginList.at(i)->setProperties(properties);
    }    
    
    return pluginList;    
}

vector<MMSPluginData *> MMSPluginService::getImportPlugins(const bool inactiveToo) {
	MMSLogger logger("MMSCONFIG");
    
    MMSPluginDAO myPluginDAO(this->dbconn);
    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);

    vector <MMSPluginData *> pluginList = myPluginDAO.findAllPluginsByType(PT_IMPORT_PLUGIN, inactiveToo);

	logger.writeLog("get the properties of the import plugins.");
    for(unsigned i=0; i<pluginList.size(); i++) {
        vector <MMSPropertyData *> properties;
        properties = myPropertyDAO.findAllPluginPropertiesByPlugin(pluginList.at(i));
        pluginList.at(i)->setProperties(properties);
    }    
    
    return pluginList;    
}

vector<MMSPluginData *> MMSPluginService::getBackendPlugins(const bool inactiveToo) {
	MMSLogger logger("MMSCONFIG");
    
    MMSPluginDAO myPluginDAO(this->dbconn);
    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);
    
    vector <MMSPluginData *> pluginList = myPluginDAO.findAllPluginsByType(PT_BACKEND_PLUGIN, inactiveToo);

    for(unsigned i=0; i<pluginList.size(); i++) {
        vector <MMSPropertyData *> properties;
        properties = myPropertyDAO.findAllPluginPropertiesByPlugin(pluginList.at(i));
        pluginList.at(i)->setProperties(properties);
    }    
    
    return pluginList;    
}

/* getAllPluginsByCategory */
vector<MMSPluginData*> MMSPluginService::getPluginsByCategory(MMSPluginCategoryData *category, const bool inactiveToo) {

    MMSPluginDAO myPluginDAO(this->dbconn);
    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);
    MMSImportPropertyDAO myImportPropertyDAO(this->dbconn);
    vector <MMSPluginData *> pluginList = myPluginDAO.findAllPluginsByCategory(category, inactiveToo);

    for(unsigned i=0; i<pluginList.size(); i++) {
        if (pluginList.at(i)->getType()->getName() != PT_IMPORT_PLUGIN) {
            
            /* select the plugin properties*/
            vector <MMSPropertyData *> properties;
            properties = myPropertyDAO.findAllPluginPropertiesByPlugin(pluginList.at(i));
            pluginList.at(i)->setProperties(properties);
        }
        else {
            /* select the plugin import properties*/
            MMSImportPropertyData *i_properties;
            i_properties = myImportPropertyDAO.findImportPropertyByPlugin(pluginList.at(i));
            pluginList.at(i)->setImportProperties(i_properties);
        }
    }    
    
    return pluginList;    
}
vector<MMSPluginData*> MMSPluginService::getPluginsByType(MMSPluginTypeData *type, const bool inactiveToo) {

    MMSPluginDAO myPluginDAO(this->dbconn);
    MMSPluginPropertyDAO myPropertyDAO(this->dbconn);
    MMSImportPropertyDAO myImportPropertyDAO(this->dbconn);
    vector <MMSPluginData *> pluginList = myPluginDAO.findAllPluginsByType(type, inactiveToo);

    for(unsigned i=0; i<pluginList.size(); i++) {
        if (pluginList.at(i)->getType()->getName() != PT_IMPORT_PLUGIN) {
            
            /* select the plugin properties*/
            vector <MMSPropertyData *> properties;
            properties = myPropertyDAO.findAllPluginPropertiesByPlugin(pluginList.at(i));
            pluginList.at(i)->setProperties(properties);
        }
        else {
            /* select the plugin import properties*/
            MMSImportPropertyData *i_properties;
            i_properties = myImportPropertyDAO.findImportPropertyByPlugin(pluginList.at(i));
            pluginList.at(i)->setImportProperties(i_properties);
        }
    }    
    
    return pluginList;    
}

MMSPluginCategoryData* MMSPluginService::getPluginCategoryByName(string name) {
    
    MMSPluginCategoryDAO categoryDAO(this->dbconn);
    
    return categoryDAO.findCategoryByName(name);
}

MMSPluginTypeData* MMSPluginService::getPluginTypeByName(string name) {

	MMSPluginTypeDAO typeDAO(this->dbconn);
    
    return typeDAO.findTypeByName(name);
}

vector<MMSPluginCategoryData*> MMSPluginService::getPluginCategories() {
    
    MMSPluginCategoryDAO categoryDAO(this->dbconn);
    
    return categoryDAO.findAllCategories();
}
