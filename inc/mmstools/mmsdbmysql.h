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

/**
 * @file  mmsdbmysql.h
 *
 * @brief Header file for mysql database functions.
 *
 * @author Stefan Schwarzer <sschwarzer@berlinux-solutions.de>
 * @author Guido Madaus     <gmadaus@berlinux-solutions.de>
 * @author Jens Schneider   <pupeider@morphine.tv>
 * @author Matthias Hardt   <MHardt@berlinux-solutions.de>
 *
 * @ingroup mmstools
 */
#ifndef MMSDBMYSQL_H_
#define MMSDBMYSQL_H_

#ifdef __ENABLE_MYSQL__

#include "mmstools/base.h"
#include "mmstools/mmserror.h"
#include "mmstools/mmsrecordset.h"
#include "mmstools/datasource.h"
#include "mmstools/interfaces/immsdb.h"

#include <mysql.h>

class MMSDBMySQL : public IMMSDB {
    private:
        MYSQL      	dbhandle;
        bool		autoreconnect;

    public:
    	MMSDBMySQL(DataSource *datasource = NULL, bool autoreconnect = false);
        virtual ~MMSDBMySQL();

        void connect();
        void disconnect();
        void startTransaction();
        void commitTransaction();
        void rollbackTransaction();
        int query(string statement, MMSRecordSet *rs);
        int query(string statement);
        int getLastInsertedID();
};

#endif /*__ENABLE_MYSQL__*/

#endif /*MMSDBMYSQL_H_*/
