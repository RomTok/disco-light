/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

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
