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

#ifndef MMSIMPORTPROPERTYDAO_H_
#define MMSIMPORTPROPERTYDAO_H_

#include "mmstools/mmstools.h"
#include "mmsconfig/mmsimportpropertydata.h"
#include "mmsconfig/mmsplugindata.h"

MMS_CREATEERROR(MMSImportPropertyDAOError);

class MMSImportPropertyDAO {
    
    private:
        IMMSDB *dbConnection;

        void setMMSDBConnection(IMMSDB *connection);
        IMMSDB *getMMSDBConnection();

        void save(MMSImportPropertyData *data);
        void update(MMSImportPropertyData *data);

        MMSImportPropertyData *moveRecordToData(MMSRecordSet rs);

    public:
        MMSImportPropertyDAO(IMMSDB *connection);

        void saveOrUpdate(MMSImportPropertyData *data);

        MMSImportPropertyData *findImportPropertyByPlugin(MMSPluginData *plugin);    
};

#endif /*MMSIMPORTPROPERTYDAO_H_*/
