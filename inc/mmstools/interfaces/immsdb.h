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

#ifndef IMMSDB_H_
#define IMMSDB_H_

#include "mmstools/mmsrecordset.h"
#include "mmstools/datasource.h"

typedef map<string, string> MMSDB_SP_ARGLIST;

class IMMSDB {
	protected:
        string     dbname;
		DataSource *datasource;
		bool       connected;

	public:
		IMMSDB(DataSource *_datasource) : datasource(_datasource), connected(false) {};
		virtual ~IMMSDB() {};

		virtual void connect() = 0;
		virtual void disconnect() = 0;
		virtual int query(string statement, MMSRecordSet *rs) = 0;
		virtual int query(string statement) = 0;
		virtual int getLastInsertedID() = 0;
		virtual void startTransaction() = 0;
		virtual void commitTransaction() = 0;
		virtual void rollbackTransaction() = 0;

		/**
		 * @brief Returns the name of the associated database
		 *
		 * @return The name of the associated database
		 */
		string getDBName() { return this->dbname; }
};

#endif /*IMMSDB_H_*/
