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

#ifndef DATASOURCE_H_
#define DATASOURCE_H_

#include "base.h"

#define DBMS_SQLITE		"SQLITE"
#define DBMS_FREETDS	"FREETDS"

class DataSource {

    private:
    	string       dbms;
    	string       address;
        unsigned int port;
        string       dbName;
        string       user;
        string       password;
    
    public:
    	DataSource(const string dbms, 
    			   const string dbName, 
    			   const string address    = "", 
    			   const unsigned int port = 0,
    			   const string user       = "",
    			   const string password   = "");
    	~DataSource();
    
        void setDBMS(const string dbms);
        const string getDBMS();
        void setAddress(const string address);
        const string getAddress();
        void setPort(const unsigned int port);
        const unsigned int getPort();
        void setDatabaseName(const string dbName);
        const string getDatabaseName();
        void setUser(const string user);
        const string getUser();
        void setPassword(const string password);
        const string getPassword();
};

#endif /*DATASOURCE_H_*/
