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

#ifndef MMSPOPUPWINDOWCLASS_H_
#define MMSPOPUPWINDOWCLASS_H_

#include "mmsgui/theme/mmswindowclass.h"

class MMSPopupWindowClass {
    private:
        string       className;
        bool         isduration;
        unsigned int duration;

    public:
        MMSWindowClass windowClass;

        MMSPopupWindowClass();
        //
        void unsetAll();
        //
        void setAttributesFromXMLNode(xmlNode* node, string path = "");
        //
        void setClassName(string className);
        string getClassName();
        //
        bool isDuration();
        void setDuration(unsigned int duration);
        void unsetDuration();
        unsigned int getDuration();
};

#endif /*MMSPOPUPWINDOWCLASS_H_*/
