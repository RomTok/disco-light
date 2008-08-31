/***************************************************************************
 *   Copyright (C) 2008 by                                                 *
 *                                                                         *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>                    *
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

class MMSSip {
    private:
        string       user,
                     passwd,
                     registrar,
                     realm;
        short int    localPort;

        pjsua_acc_id          accID;
        pj_thread_desc        pjThreadDesc;
        pj_thread_t           *pjThread;

        map<int, MMSSipBuddy> buddies;

    public:
    	MMSSip(const string    &user,
    		   const string    &passwd,
    		   const string    &registrar,
    		   const string    &realm = "",
    		   const short int &localPort = 5060);

    	~MMSSip();

    	const int call(const string &user, const string &domain = "");
    	void hangup(int id);
    	void answer(int id);
    	void addBuddy(const string &name, const string &uri);
        MMSSipBuddy	getBuddy(const int &id);

        sigc::signal<void, int> 		           *onCallSuccessfull;
        sigc::signal<void, int, string>            *onCallIncoming;
        sigc::signal<void, int>                    *onCallDisconnected;
        sigc::signal<void, int>                    *onCalling;
        sigc::signal<void, MMSSipBuddy>            *onBuddyStatus;
};

#endif /* _NO_MMSSIP */

#endif /* MMSSIP_H_ */
