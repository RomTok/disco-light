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
 * @file  mmsdbfreetds.h
 * 
 * @brief Header file for freetds database functions.
 * 
 * @author Stefan Schwarzer <sschwarzer@berlinux-solutions.de>
 * @author Guido Madaus     <gmadaus@berlinux-solutions.de>
 * @author Jens Schneider   <pupeider@morphine.tv>
 * 
 * @ingroup mmstools
 */
#ifndef MMSDBFREETDS_H_
#define MMSDBFREETDS_H_

#ifdef __ENABLE_FREETDS__

#include "base.h"
#include "mmserror.h"
#include "mmsrecordset.h"
#include "datasource.h"
#include "interfaces/immsdb.h"
#include <sql.h>
#include <map>
#include <sqlext.h>

#define DIAG_TYPE_ENV   1
#define DIAG_TYPE_DBC   2
#define DIAG_TYPE_STMT  3

#define FREETDS_SP_EXEC_CMD		"CALL "

class MMSDBFreeTDS : public IMMSDB {
    private:
        string  	dbname;
        SQLHDBC 	*dbhandle;
        SQLHENV 	henv;
   	 	SQLHSTMT	hstmt;
        bool    	connected;

    public:
    	MMSDBFreeTDS() : connected(false) {};
        virtual ~MMSDBFreeTDS();
    
        void connect(DataSource *datasource);
        void disconnect();
        void commit();
        void rollback();
        string getDBName();
        int query(string statement, MMSRecordSet *rs);
        int query(string statement);
        int executeSP(string spName, MMSDB_SP_ARGLIST argList, MMSRecordSet *rs);
        int executeSP(string spName, MMSDB_SP_ARGLIST argList);
        int getLastInsertedID();
};

#endif /* __ENABLE_FREETDS__ */

#endif /*MMSDBFREETDS_H_*/
