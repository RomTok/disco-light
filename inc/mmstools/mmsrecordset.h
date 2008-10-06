/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
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

#ifndef MMSRECORDSET_H_
#define MMSRECORDSET_H_

#include <string>
#include <vector>
#include <map>

using namespace std;

class MMSRecordSet {
    private:
        int    count;
        string info;
        int    recnum;
        vector< map<string, string>* > rows;

    public:
        MMSRecordSet(string _info = "not specified") : count(0), info(_info), recnum(-1) {};
        ~MMSRecordSet();

        bool            	next();
        bool            	previous();
        const unsigned int  getCount() const;
        const string        getInfo() const;
        bool            	setRecordNum(int num);
        const int       	getRecordNum() const;
        void            	addRow();
        bool            	reset();

        string & operator[](string key);
        const string & operator[](const string key) const;
};

#endif /*MMSRECORDSET_H_*/
