/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>                    *
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
 * @file  mmsdbmysql.cpp
 *
 * @brief Source file for mysql database functions.
 *
 * @author Stefan Schwarzer <sschwarzer@berlinux-solutions.de>
 * @author Guido Madaus     <gmadaus@berlinux-solutions.de>
 * @author Jens Schneider   <pupeider@morphine.tv>
 * @author Matthias Hardt   <MHardt@berlinux-solutions.de>
 *
 * @ingroup mmstools
 */
#ifdef __ENABLE_MYSQL__

#include <stdlib.h>
#include "mmstools/mmsdbmysql.h"
#include "mmstools/mmslogger.h"

/**
 * @brief ????
 *
 */
MMSDBMySQL::MMSDBMySQL(DataSource *_datasource, bool autoreconnect) :
	IMMSDB(_datasource),
	autoreconnect(autoreconnect) {
	if(!this->datasource)
		throw new MMSError(0, "Cannot instantiate MMSDBMySQL without datasource");
};

/**
 * @brief ????
 *
 */
MMSDBMySQL::~MMSDBMySQL() {
    this->disconnect();
}

/**
 * @brief ????
 *
 */
void MMSDBMySQL::startTransaction() {
    if(mysql_query(&this->dbhandle, "START TRANSACTION") != 0)
        throw new MMSError(0, mysql_error(&this->dbhandle));
}

/**
 * @brief ????
 *
 */
void MMSDBMySQL::commitTransaction() {
#if MYSQL_VERSION_ID >= 40100
    if(mysql_commit(&this->dbhandle) != 0)
#else
    if(mysql_query(&this->dbhandle, "COMMIT") != 0)
#endif
        throw new MMSError(0, mysql_error(&this->dbhandle));
}

/**
 * @brief ????
 *
 */
void MMSDBMySQL::rollbackTransaction() {
#if MYSQL_VERSION_ID >= 40100
    if(mysql_rollback(&this->dbhandle) != 0)
#else
    if(mysql_query(&this->dbhandle, "ROLLBACK") != 0)
#endif
        throw new MMSError(0, mysql_error(&this->dbhandle));
}

/**
 * @brief Opens connection to database.
 */
void MMSDBMySQL::connect() {
	mysql_init(&this->dbhandle);

	// reconnect?
	if(this->autoreconnect) {
		mybool_t r = 1;
		mysql_options(&this->dbhandle, MYSQL_OPT_RECONNECT, &r);
	}

	if(!mysql_real_connect(&this->dbhandle,
			               this->datasource->getAddress().c_str(),
			               this->datasource->getUser().c_str(),
			               this->datasource->getPassword().c_str(),
			               this->datasource->getDatabaseName().c_str(),
			               this->datasource->getPort(),
			               NULL, 0)) {
	       string err = mysql_error(&this->dbhandle);
	       mysql_close(&this->dbhandle);
	       throw new MMSError(0, err);
	}

   this->connected = true;
}

/**
 * @brief Close connection to database.
 *
 * @return void
 */
void MMSDBMySQL::disconnect() {
    if(this->connected) {
        mysql_close(&this->dbhandle);
        this->connected = false;
    }

    this->dbname = "";

    return;
}

/**
 * @brief This function executes given database query and puts the results in MMSRecordSet.
 *        This method is used for select statements
 *
 * @param statement buffer with database query
 *
 * @return Returns the number of affected rows
 */
int MMSDBMySQL::query(string statement, MMSRecordSet *rs) {
	int     numRows = 0;
    string 	message;

    rs->reset();

    if(!this->connected) {
    	message = "Query called but no connection established." + string(" [query was: ") + statement + string("]");
        throw(new MMSError(0, message));
    }

    if(mysql_query(&this->dbhandle, statement.c_str()) != 0) {
        message = mysql_error(&this->dbhandle) + string(" [query was: ") + statement + string("]");
        throw new MMSError(0, message);
    }

    // get results
    MYSQL_RES *results = mysql_use_result(&this->dbhandle);
    if(results) {
    	unsigned int count = mysql_num_fields(results);
    	if(count > 0) {
			MYSQL_FIELD *fields = mysql_fetch_fields(results);
			MYSQL_ROW row;
			while((row = mysql_fetch_row(results))) {
				rs->addRow();
				for(unsigned int i = 0; i < count; i++) {
					if(row[i])
						(*rs)[fields[i].name] = row[i];
				}
			}
    	}
    	numRows = mysql_num_rows(results);
    	mysql_free_result(results);
    }

    //rewind
    rs->setRecordNum(0);

    return numRows;
}

/**
 * @brief This function executes given database query.
 *        This method is used for insert, update and delete statements
 *
 * @param statement buffer with database query
 *
 * @return Returns the number of affected rows
 */
int MMSDBMySQL::query(string statement) {
	int     numRows = 0;
    string 	message;

    if(!this->connected) {
    	message = "Query called but no connection established." + string(" [query was: ") + statement + string("]");
        throw(new MMSError(0, message));
    }

    if(mysql_query(&this->dbhandle, statement.c_str()) != 0) {
        message = mysql_error(&this->dbhandle) + string(" [query was: ") + statement + string("]");
        throw new MMSError(0, message);
    }

    // fetch results if there are some
    MYSQL_RES *results = mysql_store_result(&this->dbhandle);
	if(results) {
		numRows = mysql_num_rows(results);
		mysql_free_result(results);
	}

    // return the number of affected rows
    return numRows;
}

/**
 * @brief Returns the ID of the last inserted record
 *
 * @note This method isn't implemented.
 *
 * @return Returns the ID of the last inserted record
 */
int MMSDBMySQL::getLastInsertedID() {
	int ret = 0;

    if(!this->connected)
        throw(new MMSError(0, "No connection established. Cannot fetch last inserted id."));

    if(mysql_query(&this->dbhandle, "SELECT LAST_INSERT_ID();") != 0) {
        string message = mysql_error(&this->dbhandle) + string(" [query was: SELECT LAST_INSERT_ID();]");
        throw new MMSError(0, message);
    }

    // fetch results if there are some
    MYSQL_RES *results = mysql_store_result(&this->dbhandle);
	if(results) {
		MYSQL_ROW row = mysql_fetch_row(results);
		if(row)
			ret = atoi(row[0]);
		mysql_free_result(results);
	}

	return ret;
}

#endif /*__ENABLE_MYSQL__*/
