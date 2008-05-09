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

#ifndef MMSEVENTDISPATCHER_H_
#define MMSEVENTDISPATCHER_H_

#include "mmsbase/mmsbase.h"
#include "mmsbase/interfaces/immseventdispatcher.h"
#include "mmscore/mmspluginmanager.h"
#include "mmscore/mmseventsignupmanager.h"

MMS_CREATEERROR(MMSEventDispatcherError);

class MMSEventDispatcher : public IMMSEventDispatcher {
    private:
        MMSPluginManager *manager;
        MMSPluginManager *getManager();
        MMSEventSignupManager *signupmanager;
        MMSEventSignupManager *getSignupManager();

    public:
        MMSEventDispatcher(MMSPluginManager *manager,MMSEventSignupManager *signupmanager);
        virtual ~MMSEventDispatcher();
        void raise(_IMMSEvent *event, int id = -1);
};

#endif /*MMSEVENTDISPATCHER_H_*/
