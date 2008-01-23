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

#include "datasource.h" 

DataSource::DataSource(const string _dbms, 
    		           const string _dbName, 
    			       const string _address, 
    			       const unsigned int _port,
    			       const string _user,
    			       const string _password) :
    dbms(_dbms),
    dbName(_dbName),
    address(_address),
    port(_port),
    user(_user),
    password(_password) {
	
}

DataSource::~DataSource() {
	
}

void DataSource::setDBMS(const string dbms) {
    this->dbms = dbms;
}

const string DataSource::getDBMS() {
    return this->dbms;
}

void DataSource::setAddress(const string address) {
    this->address = address;
}

const string DataSource::getAddress() {
    return this->address;
}

void DataSource::setPort(const unsigned int port) {
    this->port = port;
}

const unsigned int DataSource::getPort() {
    return this->port;
}

void DataSource::setDatabaseName(const string dbName) {
    this->dbName = dbName;
}

const string DataSource::getDatabaseName() {
    return this->dbName;
}

void DataSource::setUser(const string user) {
    this->user = user;
}

const string DataSource::getUser() {
    return this->user;
}

void DataSource::setPassword(const string password) {
    this->password = password;
}

const string DataSource::getPassword() {
    return this->password;
}
