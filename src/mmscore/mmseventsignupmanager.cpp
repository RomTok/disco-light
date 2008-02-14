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

#include "mmscore/mmseventsignupmanager.h"
#include "mmstools/tools.h"

MMSEventSignupManager::MMSEventSignupManager() {
}

void MMSEventSignupManager::signup(IMMSEventSignup *signup) {
    this->signupmutex.lock();

    this->signups.push_back((MMSEventSignup *)signup);

    this->signupmutex.unlock();
}

vector<MMSPluginData *> MMSEventSignupManager::getReceiverPlugins(_IMMSEvent *event) {
    vector<MMSPluginData *> mydata;

    for(unsigned int i=0;i<this->signups.size();i++) {
        for(unsigned int y=0;y<this->signups.at(i)->getSubScriptions().size();y++) {
      		 DEBUGMSG("MMSEventSignupManager", "compare %s to %s - size: %d", this->signups.at(i)->getSubScriptions().at(y)->c_str(), event->getHeading().c_str(), this->signups.at(i)->getSubScriptions().at(y)->size()); 
            /*compare heading of event with subsciptions */
            if(strncmp(this->signups.at(i)->getSubScriptions().at(y)->c_str(),
                       event->getHeading().c_str(),
                       this->signups.at(i)->getSubScriptions().at(y)->size())==0) {
                      
                /* add plugindata to return vector*/
                MMSPluginData *myplugin = new MMSPluginData;
/* PUPEIDER
                myplugin->setId(signups.at(i)->getPluginData().getId());
                myplugin->setName(signups.at(i)->getPluginData().getName());
                myplugin->setFilename(signups.at(i)->getPluginData().getFilename());
                myplugin->setType(signups.at(i)->getPluginData().getType());
                myplugin->setProperties(signups.at(i)->getPluginData().getProperties());
*/
                *myplugin = signups.at(i)->getPluginData();
                
                mydata.push_back(myplugin);
            }
        }
    }
    if(mydata.empty())
        throw new MMSEventSignupManagerError(0,"no subscriptions found");
    
    return mydata;
}
