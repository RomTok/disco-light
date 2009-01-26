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

#ifndef MMSSIP_H_
#define MMSSIP_H_

#ifndef _NO_MMSSIP

#include <pjsua-lib/pjsua.h>
#include <iostream>
#include <sigc++/sigc++.h>
#include <map>

using namespace std;

typedef struct {
	string user;
	string passwd;
	string registrar;
	string realm;
	bool   autoanswer;
} MMSSipAccount;

typedef enum {
	BUDDY_ONLINE		= PJSUA_BUDDY_STATUS_ONLINE,
	BUDDY_OFFLINE		= PJSUA_BUDDY_STATUS_OFFLINE,
	BUDDY_UNKNOWN		= PJSUA_BUDDY_STATUS_UNKNOWN
} MMSSipBuddyStatus;

typedef struct {
	string 				name;
	string 				uri;
	MMSSipBuddyStatus	status;
} MMSSipBuddy;

typedef struct {
	pj_thread_desc	desc;
	pj_thread_t		*thread;
} MMSSipThread;

class MMSSip {
    private:
        string       stunserver,
                     nameserver;
        short int    localPort;

        vector<MMSSipThread>	threadInfo;

        map<int, MMSSipAccount>	accounts;
        int						defaultAccount;
        map<int, MMSSipBuddy> 	buddies;

    public:
        MMSSip(const string    &user,
    		   const string    &passwd,
    		   const string    &registrar,
    		   const string    &realm = "",
    		   const string    &stunserver = "",
    		   const string    &nameserver = "",
    		   const short int &localPort = 5060);

    	~MMSSip();

    	const bool registerAccount(const string &user,
								   const string &passwd,
								   const string &registrar,
								   const string &realm,
								   const bool defaultAccount = false,
								   const bool autoanswer = false);
    	const int call(const string &user, const string &domain = "");
    	void hangup(int id = PJSUA_INVALID_ID);
    	void answer(int id);
    	void addBuddy(const string &name, const string &uri);
        MMSSipBuddy	getBuddy(const int &id);
        bool setSpeakerVolume(const unsigned int percent);
        int  getSpeakerVolume();
        bool getAutoAnswer(const int accountId);
        void setAutoAnswer(const int accountId, const bool value = true);
        bool registerRingtone(const string &filename);

        sigc::signal<void, int> 		           *onCallSuccessfull;
        sigc::signal<void, int, string>            *onCallIncoming;
        sigc::signal<void, int>                    *onCallDisconnected;
        sigc::signal<void, int>                    *onCalling;
        sigc::signal<void, MMSSipBuddy>            *onBuddyStatus;
};

#endif /* _NO_MMSSIP */

#endif /* MMSSIP_H_ */
