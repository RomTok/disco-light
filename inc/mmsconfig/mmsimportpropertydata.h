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

#ifndef MMSIMPORTPROPERTYDATA_H_
#define MMSIMPORTPROPERTYDATA_H_

#include "mmstools/base.h"

class MMSImportPropertyData {
    friend class MMSImportPropertyDAO;
    private:
        int     id;
        int     pluginId;
        bool    onStartUp;
        int     time;
        int     interval;
        
        void    setId(int id);

    public:
        MMSImportPropertyData();
        ~MMSImportPropertyData();

        int     getId();

        int     getPluginId();
        void    setPluginId(int pluginId);

        bool getOnStartUp(void);
        void setOnStartUp(bool onStartUp);

        int getTime(void);
        void setTime(int time);

        int getInterval(void);
        void setInterval(int interval);
};

#endif /*MMSIMPORTPROPERTYDATA_H_*/
