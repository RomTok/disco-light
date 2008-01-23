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

#include "mmsplugindata.h"

MMSPluginData::MMSPluginData() :
    id(-1), type(NULL), name(""), title(""), description(""), path(""), filename(""),
    active(false), icon(""), selectedicon(""), smallicon(""), smallselectedicon(""),
    importProperties(NULL), category(NULL) {
}        

MMSPluginData::~MMSPluginData() {
}

int MMSPluginData::getId() {
    return this->id;
}

void MMSPluginData::setId(int id) {
    this->id = id;   
}

MMSPluginTypeData *MMSPluginData::getType() {
    return this->type;
}

void MMSPluginData::setType(MMSPluginTypeData *type) {
    this->type = type;
}

string MMSPluginData::getName() {
    return this->name;
}
void MMSPluginData::setName(string name) {
    this->name = name;
}

string MMSPluginData::getTitle() {
    return this->title;
}
void MMSPluginData::setTitle(string title) {
    this->title = title;
}

string MMSPluginData::getDescription() {
    return this->description;
}
void MMSPluginData::setDescription(string description) {
    this->description = description;
}

string MMSPluginData::getPath() {
    return this->path;
}
void MMSPluginData::setPath(string path) {
    this->path = path;
}

string MMSPluginData::getFilename() {
    return this->filename;
}

void MMSPluginData::setFilename(string filename) {
    this->filename = filename;
}

bool MMSPluginData::getActive() {
    return this->active;
}

void MMSPluginData::setActive(bool active) {
    this->active = active;
}

string MMSPluginData::getIcon() {
    return this->icon;
}

void MMSPluginData::setIcon(string icon) {
    this->icon = icon;
}

string MMSPluginData::getSelectedIcon() {
    return this->selectedicon;
}

void MMSPluginData::setSelectedIcon(string icon) {
    this->selectedicon = icon;
}

string MMSPluginData::getSmallIcon() {
    return this->smallicon;
}

void MMSPluginData::setSmallIcon(string icon) {
    this->smallicon = icon;
}

string MMSPluginData::getSmallSelectedIcon() {
    return this->smallselectedicon;
}

void MMSPluginData::setSmallSelectedIcon(string icon) {
    this->smallselectedicon = icon;
}

vector<MMSPropertyData *>  MMSPluginData::getProperties() {
    return this->properties;
}
    
void MMSPluginData::setProperties(vector <MMSPropertyData *> props) {
    this->properties = props;    
}

MMSImportPropertyData *MMSPluginData::getImportProperties() {
    return this->importProperties;
}
    
void MMSPluginData::setImportProperties(MMSImportPropertyData *props) {
    this->importProperties = props;    
}

MMSPluginCategoryData *MMSPluginData::getCategory() {
    return this->category;
}

void MMSPluginData::setCategory(MMSPluginCategoryData *category){
    this->category = category;
}


MMSPropertyData*  MMSPluginData::getProperty(string name) {

    for(vector<MMSPropertyData*>::iterator it=properties.begin();it!=properties.end();it++) {
        
        if ((*it)->getParameter()==name)
            return (*it);
    }
    return NULL;
}
