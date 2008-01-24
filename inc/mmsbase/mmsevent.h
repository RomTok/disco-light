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

#ifndef MMSEVENT_H_
#define MMSEVENT_H_

#include "mmstools/base.h"
#include "mmsbase/interfaces/immsevent.h"
#include "mmsbase/interfaces/immseventdispatcher.h"
#include <map>

class MMSEvent : public IMMSEvent {
    private:
        static IMMSEventDispatcher *dispatcher;
        string heading;
        std::map<string,string> data;
        
    public:
        MMSEvent(string heading = "noreceiver");
        ~MMSEvent();
        virtual void setHeading(string heading);
        virtual string getHeading();
        virtual string getData(string key);
        virtual void setData(string key, string value);
        virtual void clear();
        virtual void send();
        void setDispatcher(IMMSEventDispatcher *dispatcher);
        void sendTo(int);
};

#endif /*MMSEVENT_H_*/
