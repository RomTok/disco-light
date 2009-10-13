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

#ifndef MMSIMPORTSOURCEDAO_H_
#define MMSIMPORTSOURCEDAO_H_

#include "mmstools/mmstools.h"
#include "mmsconfig/mmsimportsourcedata.h"
#include "mmsconfig/mmsplugindata.h"

class MMSImportSourceDAO {

    private:
        IMMSDB *dbConnection;

        void setMMSDBConnection(IMMSDB *connection);
        IMMSDB *getMMSDBConnection();

        void save(MMSImportSourceData *data);
        void update(MMSImportSourceData *data);

        MMSImportSourceData *moveRecordToData(MMSRecordSet &rs);

    public:
        MMSImportSourceDAO(IMMSDB *connection);
        void deleteImportSource(MMSImportSourceData *source);

        void saveOrUpdate(MMSImportSourceData *data);
        void saveOrUpdate(vector<MMSImportSourceData *> dataList);

        vector<MMSImportSourceData *> findImportSourcesByPlugin(MMSPluginData *plugin);
        MMSImportSourceData *   findImportSourcesByID(int id);
        MMSImportSourceData *   findImportSourcesByName(string name);
};

#endif /*MMSIMPORTSOURCEDAO_H_*/
