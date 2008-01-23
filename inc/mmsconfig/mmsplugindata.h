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

#ifndef MMSPLUGINDATA_H_
#define MMSPLUGINDATA_H_

#include "base.h"
#include "mmspropertydata.h"
#include "mmsplugincategorydata.h"
#include "mmsplugintypedata.h"
#include "mmsimportpropertydata.h"

/* plugin types */
#define PT_OSD_PLUGIN       "OSD_PLUGIN"
#define PT_CENTRAL_PLUGIN   "CENTRAL_PLUGIN"
#define PT_IMPORT_PLUGIN    "IMPORT_PLUGIN"
#define PT_BACKEND_PLUGIN   "BACKEND_PLUGIN"

class MMSPluginData {
    friend class MMSPluginDAO;
    friend class MMSXmlCmdRequestParser;
    private:
        int                         id;
        MMSPluginTypeData           *type;
        string                      name;
        string                      title;
        string                      description;
        string                      path;
        string                      filename;
        bool                        active;
        string                      icon;
        string                      selectedicon;
        string                      smallicon;
        string                      smallselectedicon;
        vector  <MMSPropertyData *> properties;
        MMSImportPropertyData       *importProperties;
        MMSPluginCategoryData       *category;
        
        void    setId(int id);

    public:
        MMSPluginData();
        ~MMSPluginData();

        int     getId();

        MMSPluginTypeData  *getType();
        void    setType(MMSPluginTypeData *type);

        string  getName();
        void    setName(string name);

        string  getTitle();
        void    setTitle(string title);

        string  getDescription();
        void    setDescription(string description);

        string  getFilename();
        void    setFilename(string filename);

        string  getPath();
        void    setPath(string path);

        bool    getActive();
        void    setActive(bool active);

        string  getIcon();
        void    setIcon(string icon);

        string  getSelectedIcon();
        void    setSelectedIcon(string icon);

        string  getSmallIcon();
        void    setSmallIcon(string icon);

        string  getSmallSelectedIcon();
        void    setSmallSelectedIcon(string icon);

        vector<MMSPropertyData *>  getProperties();
        MMSPropertyData*  getProperty(string name);
        void    setProperties(vector <MMSPropertyData *> props);

        MMSImportPropertyData *getImportProperties();
        void    setImportProperties(MMSImportPropertyData *props);

        MMSPluginCategoryData *getCategory();
        void    setCategory(MMSPluginCategoryData *category);
};

#endif /*MMSPLUGINDATA_H_*/
