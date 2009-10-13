/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
 ***************************************************************************/

#ifndef DATASOURCE_H_
#define DATASOURCE_H_

#include "mmstools/base.h"

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
    	DataSource(const DataSource& d);
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
