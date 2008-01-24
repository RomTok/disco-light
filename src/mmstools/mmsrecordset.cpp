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

#include "mmstools/mmsrecordset.h"

MMSRecordSet::~MMSRecordSet() {
    rows.erase(rows.begin(), rows.end());
}

void MMSRecordSet::addRow() {
    this->count++;
    this->recnum = this->count-1;
    rows.push_back(new map<string, string>);
}

bool MMSRecordSet::next() {
    if(this->recnum < this->count - 1) {
        this->recnum++;
        return true;
    }

    return false;
}

bool MMSRecordSet::previous() {
    if(this->recnum > 0) {
        this->recnum--;
        return true;
    }

    return false;
}

unsigned int MMSRecordSet::getCount() {
    return (unsigned int) this->count;
}

string MMSRecordSet::getInfo() {
    return this->info;
}

bool MMSRecordSet::setRecordNum(int num) {
    if(num < this->count) { 
        this->recnum = num;
        return true;
    }
     
    return false;
}

int MMSRecordSet::getRecordNum() {
    return this->recnum;
}

string defret = "";

string &MMSRecordSet::operator[](string key) {
	if(this->recnum == -1)
		return defret;
		
    return (*rows.at(this->recnum))[key];
}

bool MMSRecordSet::reset() {
    rows.erase(rows.begin(), rows.end());
    
    this->recnum = -1;
    this->count  = 0;
    this->info   = "not specified";

    return true;
}
