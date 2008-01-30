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

#ifndef IMMSEVENT_H_
#define IMMSEVENT_H_
#include "mmstools/base.h"
#include "mmstools/mmshandle.h"

class _IMMSEvent {
    public:
        virtual ~_IMMSEvent() {}; 
        virtual void setHeading(string heading) = 0;
        virtual string getHeading() = 0;
        virtual string getData(string key) = 0;
        virtual void setData(string key, string value) = 0;
        virtual void clear() = 0;
        virtual void send() = 0;
    
};

typedef MMSHandle<_IMMSEvent> IMMSEvent;

#endif /*IMMSEVENT_H_*/
