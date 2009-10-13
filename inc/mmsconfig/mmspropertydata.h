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

#ifndef MMSPROPERTYDATA_H_
#define MMSPROPERTYDATA_H_

#include "mmstools/base.h"

#define MMSPROPERTYTYPE_STRING  "string"
#define MMSPROPERTYTYPE_INTEGER "integer"

class MMSPropertyData {

    friend class MMSPluginPropertyDAO;

    private:
        int     id;
        string  parameter;
        string  value;
        string  type;
        int     max;
        int 	min;
        vector <string> vallist;
        char	separator;
        bool    issetindb;

        void    setID(int id);

    public:
        MMSPropertyData();
        int     getID();
        string  getParameter();
        void    setParameter(string parameter);
        string  getValue();
        void    setValue(string value);
        string  getType();
        void    setType(string type);
        int     getMax();
        void    setMax(int max);
        int     getMin();
        void    setMin(int min);
        vector <string> getVallist();
        void    setVallist(vector <string> vallist);
        char    getSeparator();
        void    setSeparator(char separator);
        bool    isSetInDb();
        void    setisSetinDb(bool issetindb);
};
#endif /*MMSPROPERTYDATA_H_*/
