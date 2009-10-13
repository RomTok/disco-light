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

