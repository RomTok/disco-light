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

#include "mmsdbaccess.h"

MMSDBAccess::MMSDBAccess() {
}

MMSDBAccess::~MMSDBAccess() {
}

dbinfo *MMSDBAccess::getDBInfo() {
    return new dbinfo;
}

dbinfo *MMSDBAccess::findDB(string name) {
    return new dbinfo;
}

bool   MMSDBAccess::nextDB() {
    return false;
}
bool   MMSDBAccess::previusDB() {
    return false;
}

bool   MMSDBAccess::createDB(string dbname) {
    return false;
}

bool   MMSDBAccess::removeDB(string dbname) {
    return false;
}

//MMSDB  *MMSDBAccess::connectDB(string dbname) {
//    return false;
//}
