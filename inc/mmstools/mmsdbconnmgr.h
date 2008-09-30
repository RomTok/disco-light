/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
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
#ifndef MMSDBCONNMGR_H_
#define MMSDBCONNMGR_H_

#include "mmstools/datasource.h"
#include "mmstools/interfaces/immsdb.h"

#define DBMS_SQLITE3	"SQLITE3"
#define DBMS_FREETDS	"FREETDS"
#define DBMS_MYSQL 		"MYSQL"

class MMSDBConnMgr {
       private:
               DataSource *datasource;
       public:
    	   MMSDBConnMgr(DataSource *datasource);

    	   IMMSDB *getConnection();
};

#endif /*MMSDBCONNMGR_H_*/
