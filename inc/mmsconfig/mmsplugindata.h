/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
 ***************************************************************************/

#ifndef MMSPLUGINDATA_H_
#define MMSPLUGINDATA_H_

#include "mmstools/base.h"
#include "mmsconfig/mmspropertydata.h"
#include "mmsconfig/mmsplugincategorydata.h"
#include "mmsconfig/mmsplugintypedata.h"
#include "mmsconfig/mmsimportpropertydata.h"

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
        int							orderpos;

        void    setId(int id);

    public:
        MMSPluginData();
        MMSPluginData(const MMSPluginData &pd);
        ~MMSPluginData();

        MMSPluginData& operator=(const MMSPluginData &pd);

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

        int		getOrderpos();
        void    setOrderpos(int orderpos);
};

#endif /*MMSPLUGINDATA_H_*/
