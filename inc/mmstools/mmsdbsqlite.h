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

/**
 * @file  mmsdbsqlite.h
 * 
 * @brief Header file for sqlite3 database functions.
 * 
 * @author Stefan Schwarzer <sschwarzer@berlinux-solutions.de>
 * @author Guido Madaus     <gmadaus@berlinux-solutions.de>
 * @author Jens Schneider   <pupeider@morphine.tv>
 * 
 * @ingroup mmstools
 */
#ifndef MMSDBSQLITE3_H_
#define MMSDBSQLITE3_H_

#ifdef __ENABLE_SQLITE__

#include "base.h"
#include "mmserror.h"
#include "mmsrecordset.h"
#include "datasource.h"
#include "interfaces/immsdb.h"
#include <sqlite3.h>

#define DBMS_SQLITE3	"SQLITE3"
#define DBMS_FREETDS	"FREETDS"

class MMSDBSQLite : public IMMSDB {
    private:
        string  dbname;
        sqlite3 *dbhandle;
        bool    connected;
        static int getResults(void *rs, int numCols, char **results, char **columnNames);

    public:
    	MMSDBSQLite() : connected(false) {};
        virtual ~MMSDBSQLite();
    
        void connect(DataSource *datasource);
        void disconnect();
        void commit();
        void rollback();
        string getDBName();
        int query(string statement, MMSRecordSet *rs);
        int query(string statement);
        int getLastInsertedID();
};

#endif /*__ENABLE_SQLITE__*/

#endif /*MMSDBSQLITE3_H_*/
