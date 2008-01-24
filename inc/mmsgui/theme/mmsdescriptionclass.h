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

#ifndef MMSDESCRIPTIONCLASS_H_
#define MMSDESCRIPTIONCLASS_H_

#include "mmsgui/theme/mmsthemebase.h"

class MMSDescriptionClass {
    private:
        bool    isauthor;
        string  author;
        bool    isemail;
        string  email;
        bool    isdesc;
        string  desc;

    public:
        MMSDescriptionClass();
        //
        void unsetAll();
        //
        void setAttributesFromXMLNode(xmlNode* node);
        //
        bool isAuthor();
        void setAuthor(string author);
        void unsetAuthor();
        string getAuthor();
        //
        bool isEmail();
        void setEmail(string email);
        void unsetEmail();
        string getEmail();
        //
        bool isDesc();
        void setDesc(string desc);
        void unsetDesc();
        string getDesc();
};

#endif /*MMSDESCRIPTIONCLASS_H_*/
