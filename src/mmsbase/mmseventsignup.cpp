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

#include "mmsbase/mmseventsignup.h"

MMSEventSignup::MMSEventSignup(MMSPluginData data) {
    this->data = data;
}

MMSEventSignup::~MMSEventSignup() {
   this->subscriptions.clear();
        
} 

IMMSEventSignupManager *MMSEventSignup::getManager() {
    return this->manager;
}

void MMSEventSignup::setManager(IMMSEventSignupManager *manager) {
    this->manager = manager;
}

void MMSEventSignup::add(string subscription) {

   /* check for doublettes */
   if (this->subscriptions.size() > 0) {
        for(unsigned int i; i<this->subscriptions.size();i++) {
            //subscriptions.at(i)->
            if(subscription.compare(*(subscriptions.at(i)))==0)
                throw new MMSEventSignupError(0,"subscription already made");
        }
   }
   this->subscriptions.push_back(new string(subscription));
}

void MMSEventSignup::executeSignup() {
    this->manager->signup(this);
}

MMSPluginData MMSEventSignup::getPluginData() {
    return this->data;
}

IMMSEventSignupManager *MMSEventSignup::manager = NULL;

vector<string *> MMSEventSignup::getSubScriptions() {
    return this->subscriptions;
}
