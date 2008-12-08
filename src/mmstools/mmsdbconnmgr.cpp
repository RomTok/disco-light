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

#include "mmstools/mmsdbconnmgr.h"

#ifdef __ENABLE_SQLITE__
#include "mmstools/mmsdbsqlite.h"
#endif
#ifdef __ENABLE_MYSQL__
#include "mmstools/mmsdbmysql.h"
#endif
#ifdef __ENABLE_FREETDS__
#include "mmstools/mmsdbfreetds.h"
#endif

MMSDBConnMgr::MMSDBConnMgr(DataSource *datasource) {
	this->datasource = datasource;
}

IMMSDB *MMSDBConnMgr::getConnection() {
	#ifdef __ENABLE_SQLITE__
		if((datasource->getDBMS()==DBMS_SQLITE3) || datasource->getDBMS()=="")
			return new MMSDBSQLite(datasource);
	#endif

	#ifdef __ENABLE_MYSQL__
		if(datasource->getDBMS()==DBMS_MYSQL)
			return new MMSDBMySQL(datasource);
	#endif

	#ifdef __ENABLE_FREETDS__
		if(datasource->getDBMS()==DBMS_FREETDS)
			return new MMSDBFreeTDS(datasource);
	#endif

	return NULL;

}

