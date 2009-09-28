/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

MMSEventSignup::MMSEventSignup(MMSPluginData data) : plugindataset(true), onSubscription(NULL) {
    this->data = data;
}

MMSEventSignup::MMSEventSignup() : plugindataset(false) {
	onSubscription = new sigc::signal<void,_IMMSEvent*>;
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
        for(unsigned int i=0; i<this->subscriptions.size();i++) {
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


sigc::signal<void,_IMMSEvent*> *MMSEventSignup::getSignal() {
	return this->onSubscription;
}

bool MMSEventSignup::isPlugin() {
	return this->plugindataset;
}

