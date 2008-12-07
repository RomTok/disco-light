/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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
