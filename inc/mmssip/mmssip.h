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

using namespace std;

typedef enum {
    MMSSIP_CALL_STATE_NONE,
    MMSSIP_CALL_STATE_CALLING,
    MMSSIP_CALL_STATE_INCOMING,
    MMSSIP_CALL_STATE_CONNECTING,
    MMSSIP_CALL_STATE_IN_PROGRESS
} CallState;

class MMSSip {
    private:
        string       user,
                     passwd,
                     registrar;
        short int    localPort;

        pjsua_acc_id accID;

        vector<pjsua_call_id> activeCalls;

    public:
    	MMSSip(const string    &user,
    		   const string    &passwd,
    		   const string    &registrar,
    		   const short int &localPort = 5060);

    	~MMSSip();

    	const int call(const string &user, const string &domain);
    	void hangup(const int &id);

        sigc::signal<void, int> *onCallSuccessfull;
};

#endif /* _NO_MMSSIP */

#endif /* MMSSIP_H_ */
