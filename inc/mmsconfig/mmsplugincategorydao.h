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

#ifndef MMSPLUGINCATEGORYDAO_H_
#define MMSPLUGINCATEGORYDAO_H_

#include "mmstools/base.h"
#include "mmstools/tools.h"
#include "mmstools/mmserror.h"
#include "mmsconfig/mmsplugincategorydata.h"
#include "mmstools/interfaces/immsdb.h"

MMS_CREATEERROR(MMSPluginCategoryDAOError);

class MMSPluginCategoryDAO {
    private:
        IMMSDB *dbConnection;
        void setMMSDBConnection(IMMSDB *connection);
        IMMSDB *getMMSDBConnection();

    public:
        MMSPluginCategoryDAO(IMMSDB *connection);
        MMSPluginCategoryData *findCategoryByName(string name);    
        vector<MMSPluginCategoryData *> findAllCategories();
};

#endif /*MMSPLUGINCATEGORYDAO_H_*/
