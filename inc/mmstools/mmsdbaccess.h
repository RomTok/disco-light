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

#ifndef MMSDBACCESS_H
#define MMSDBACCESS_H

#include "mmstools/mmsrecordset.h"
#include "mmstools/mmsdbconnmgr.h"

typedef struct {
    string name;
    string pluginid;
    string desc;
} dbinfo;

class MMSDBAccess {

    public:
        MMSDBAccess();
        ~MMSDBAccess();
        dbinfo *getDBInfo();
        dbinfo *findDB(string name);
        bool   nextDB();
        bool   previusDB();
        bool   createDB(string dbname);
        bool   removeDB(string dbname);
        //MMSDBConnMgr  *connectDB(string dbname);
};

#endif /* MMSDBACCESS_H */
