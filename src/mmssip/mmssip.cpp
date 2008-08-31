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

#ifndef _NO_MMSSIP

#include "mmstools/tools.h"
#include "mmstools/mmserror.h"
#include "mmssip/mmssip.h"

/*
 * TODO:
 * - using STUN server
 */

static MMSSip *thiz = NULL;

static void onIncomingCall(pjsua_acc_id, pjsua_call_id, pjsip_rx_data*);
static void onCallState(pjsua_call_id, pjsip_event*);
static void onCallMediaState(pjsua_call_id);
static void onRegistrationState(pjsua_acc_id);
static void onBuddyState(pjsua_buddy_id);

MMSSip::MMSSip(const string    &user,
		       const string    &passwd,
		       const string    &registrar,
		       const string    &realm,
		       const short int &localPort) :
    user(user),
    passwd(passwd),
    registrar(registrar),
    realm(realm) {

	/* only one instance of mmssip allowed */
	if(thiz) {
		DEBUGMSG("MMSSIP", "There's already an instance of MMSSIP running.");
		throw MMSError(0, "There's already an instance of MMSSIP running.");
	}

	thiz = this;

	pj_status_t status;

    /* create pjsua */
    status = pjsua_create();
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error initializing SIP stack (pjsua_create)");
		throw MMSError(0, "Error initializing SIP stack (pjsua_create)");
	}
    DEBUGMSG("MMSSIP", "SIP stack init #1");

    /* configure pjsua */
    pjsua_config         cfg;
    pjsua_logging_config logCfg;

    pjsua_config_default(&cfg);
    cfg.cb.on_incoming_call    = &onIncomingCall;
    cfg.cb.on_call_media_state = &onCallMediaState;
    cfg.cb.on_call_state       = &onCallState;
    cfg.cb.on_reg_state        = &onRegistrationState;
    cfg.cb.on_buddy_state      = &onBuddyState;

    pjsua_logging_config_default(&logCfg);
    logCfg.console_level = 0;

    status = pjsua_init(&cfg, &logCfg, NULL);
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error initializing SIP stack (pjsua_init)");
		throw MMSError(0, "Error initializing SIP stack (pjsua_init)");
	}

    DEBUGMSG("MMSSIP", "SIP stack init #2");

    /* add UDP transport */
    pjsua_transport_config transCfg;

    pjsua_transport_config_default(&transCfg);
    transCfg.port = localPort;
    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transCfg, NULL);
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error initializing SIP stack (pjsua_transport_create)");
		throw MMSError(0, "Error initializing SIP stack (pjsua_transport_create)");
	}

    DEBUGMSG("MMSSIP", "UDP transport created");

    /* start pjsua */
    status = pjsua_start();
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error starting SIP stack (pjsua_start)");
		throw MMSError(0, "Error starting SIP stack (pjsua_start)");
	}

    DEBUGMSG("MMSSIP", "SIP stack started");

    /* register to SIP server */
    pjsua_acc_config accCfg;
    char id[80], reg[80];

    pjsua_acc_config_default(&accCfg);
    snprintf(id, sizeof(id), "sip:%s@%s", user.c_str(), registrar.c_str());
    accCfg.id         = pj_str(id);
    snprintf(reg, sizeof(reg), "sip:%s", registrar.c_str());
    accCfg.reg_uri    = pj_str(reg);
    accCfg.cred_count = 1;
    accCfg.cred_info[0].realm     = pj_str((realm == "") ? (char*)registrar.c_str() : (char*)realm.c_str());
    accCfg.cred_info[0].scheme    = pj_str((char*)"digest");
    accCfg.cred_info[0].username  = pj_str((char*)user.c_str());
    accCfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    accCfg.cred_info[0].data      = pj_str((char*)passwd.c_str());

    status = pjsua_acc_add(&accCfg, PJ_TRUE, &this->accID);
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error registering account sip:" + user + "@" + registrar + " (pjsua_acc_add)");
		throw MMSError(0, "Error registering account sip:" + user + "@" + registrar + " (pjsua_acc_add)");
	}

    DEBUGMSG("MMSSIP", "SIP account registered");

    this->onCallSuccessfull    = new sigc::signal<void, int>;
    this->onCallIncoming       = new sigc::signal<void, int, string>;
    this->onBuddyStatus        = new sigc::signal<void, MMSSipBuddy>;
}

MMSSip::~MMSSip() {
	pjsua_destroy();
	if(this->onCallSuccessfull) {
		this->onCallSuccessfull->clear();
		delete this->onCallSuccessfull;
	}
}

/*
 * Calls a given user.
 *
 * @param	user	[in]	user id
 * @param	domain	[in]	domain for users sip account
 *
 * @note The SIP uri will be "sip:id@domain".
 *
 * @return call id
 *
 * @see MMSSip::hangup()
 */
const int MMSSip::call(const string &user, const string &domain) {
	pj_status_t    status;
	pj_str_t       uri;
    pjsua_call_id  call;
    char           tmp[1024];
    static bool    registeredThread = false;

    const char     *cDomain = ((domain != "") ? domain.c_str() : this->registrar.c_str());

    /* have to register the input thread once, otherwise libpj
     * doesn't work
     */
    if(!registeredThread) {
		pj_bzero(this->pjThreadDesc, sizeof(this->pjThreadDesc));
		status = pj_thread_register("MMSSIP", this->pjThreadDesc, &this->pjThread);
		if(status != PJ_SUCCESS) {
			DEBUGMSG("MMSSIP", "Error registering thread (pj_thread_register)");
			throw MMSError(0, "Error registering thread (pj_thread_register)");
		}
		registeredThread = true;
    }

    snprintf(tmp, 1024, "sip:%s@%s", user.c_str(), cDomain);
	status = pjsua_verify_sip_url(tmp);
	if (status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Invalid callee info sip:" + user + "@" + cDomain);
		throw MMSError(0, "Invalid callee info sip:" + user + "@" + cDomain);
	}

    uri = pj_str(tmp);
	status = pjsua_call_make_call(this->accID, &uri, 0, NULL, NULL, &call);
	if (status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error calling sip:" + user + "@" + cDomain);
		throw MMSError(0, "Error calling sip:" + user + "@" + cDomain);
	}

	/* insert call into activeCalls */
	this->activeCalls.push_back(call);

	return (this->activeCalls.size() - 1);
}

void MMSSip::hangup(const int &id) {
	pjsua_call_id cid = this->activeCalls.at(id);
    pjsua_call_hangup(cid, 0, NULL, NULL);
    this->activeCalls.erase(this->activeCalls.begin() + id);
}

void MMSSip::answer(const int &id) {
    pjsua_call_answer(id, 200, NULL, NULL);

	/* insert call into activeCalls */
	this->activeCalls.push_back(id);
}

void MMSSip::addBuddy(const string &name, const string &uri) {
    pjsua_buddy_config buddyCfg;
    pjsua_buddy_id     buddyId;
    pjsua_buddy_info   buddyInfo;

    pjsua_buddy_config_default(&buddyCfg);
    buddyCfg.uri = pj_str((char*)uri.c_str());
    buddyCfg.subscribe = true;
    if(pjsua_buddy_add(&buddyCfg, &buddyId) == PJ_SUCCESS) {
    	MMSSipBuddy buddy = {name, uri, BUDDY_UNKNOWN};
    	buddies[buddyId] = buddy;
    	if(pjsua_buddy_get_info(buddyId, &buddyInfo) == PJ_SUCCESS) {
    		buddy.status = (MMSSipBuddyStatus)buddyInfo.status;
    	}
       	buddies[buddyId] = buddy;
    }
}

MMSSipBuddy MMSSip::getBuddy(const int &id) {
	return this->buddies[id];
}

/* Callback called by the library upon receiving incoming call */
static void onIncomingCall(pjsua_acc_id  accId,
		                   pjsua_call_id callId,
                           pjsip_rx_data *rdata) {
    pjsua_call_info ci;

    PJ_UNUSED_ARG(accId);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(callId, &ci);

    DEBUGMSG("MMSSIP", "Incoming call from %.*s", (int)ci.remote_info.slen, ci.remote_info.ptr);

    if(thiz && thiz->onCallIncoming)
        thiz->onCallIncoming->emit(callId, ci.remote_info.ptr);
}

/* Callback called by the library when call's state has changed */
static void onCallState(pjsua_call_id callId, pjsip_event *e) {
    pjsua_call_info ci;

    PJ_UNUSED_ARG(e);

    pjsua_call_get_info(callId, &ci);
    DEBUGMSG("MMSSIP", "Call %d state=%d (%.*s)", callId, ci.state, (int)ci.state_text.slen, ci.state_text.ptr);

    switch(ci.state) {
        case PJSIP_INV_STATE_NULL:
        	DEBUGMSG("MMSSIP", "onCallState: PJSIP_INV_STATE_NULL");
        	break;
        case PJSIP_INV_STATE_CALLING:
        	DEBUGMSG("MMSSIP", "onCallState: PJSIP_INV_STATE_CALLING");
        	break;
        case PJSIP_INV_STATE_INCOMING:
        	DEBUGMSG("MMSSIP", "onCallState: PJSIP_INV_STATE_INCOMING");
        	break;
        case PJSIP_INV_STATE_EARLY:
        	DEBUGMSG("MMSSIP", "onCallState: PJSIP_INV_STATE_EARLY");
        	break;
        case PJSIP_INV_STATE_CONNECTING:
        	DEBUGMSG("MMSSIP", "onCallState: PJSIP_INV_STATE_CONNECTING");
        	break;
        case PJSIP_INV_STATE_CONFIRMED:
        	DEBUGMSG("MMSSIP", "onCallState: PJSIP_INV_STATE_CONFIRMED");
            if(thiz && thiz->onCallSuccessfull)
                thiz->onCallSuccessfull->emit(callId);
        	break;
        case PJSIP_INV_STATE_DISCONNECTED:
        	DEBUGMSG("MMSSIP", "lastStatusText: %s", ci.last_status_text);
        	DEBUGMSG("MMSSIP", "onCallState: PJSIP_INV_STATE_DISCONNECTED");
        	break;
        default:

        	break;
    }
}

/* Callback called by the library when call's media state has changed */
static void onCallMediaState(pjsua_call_id callId) {
    pjsua_call_info ci;

    pjsua_call_get_info(callId, &ci);

    if(ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        // When media is active, connect call to sound device.
        pjsua_conf_connect(ci.conf_slot, 0);
        pjsua_conf_connect(0, ci.conf_slot);
    }
}

static void onRegistrationState(pjsua_acc_id id) {
	pjsua_acc_info info;

	if(pjsua_acc_get_info(id, &info) == PJ_SUCCESS) {
	    DEBUGMSG("MMSSIP", (info.has_registration ? "registered" : "not registered"));
	    DEBUGMSG("MMSSIP", "status: %d", info.status);
	    DEBUGMSG("MMSSIP", "status_text: %s", info.status_text);
	    DEBUGMSG("MMSSIP", "online_status: %d", info.online_status);
	    DEBUGMSG("MMSSIP", "online_status_text: %s", info.online_status_text);
	}
}

static void onBuddyState(pjsua_buddy_id id) {
    pjsua_buddy_info info;

    if(pjsua_buddy_get_info(id, &info) == PJ_SUCCESS) {
        if(thiz && thiz->onBuddyStatus)
            thiz->onBuddyStatus->emit(thiz->getBuddy(id));
    }
}

#endif /* _NO_MMSSIP */
