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

#ifndef MMSPLUGINPROPERTYDAO_H_
#define MMSPLUGINPROPERTYDAO_H_

#include "mmstools.h"
#include "mmspropertydata.h"
#include "mmsplugindata.h"

class MMSPluginPropertyDAO {

    private:
        IMMSDB *dbConnection;
        void setMMSDBConnection(IMMSDB *connection);
        IMMSDB *getMMSDBConnection();

    public:
        MMSPluginPropertyDAO(IMMSDB *connection);
        vector <MMSPropertyData *> findAllPluginPropertiesByPlugin(MMSPluginData *plugin);
        void saveOrUpdate(MMSPluginData *plugin);
        void saveOrUpdate(vector<MMSPluginData *> plugins);
};
#endif /*MMSPLUGINPROPERTYDAO_H_*/
