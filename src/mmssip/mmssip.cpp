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

#ifndef _NO_MMSSIP

#ifdef PJSIP_AUTH_AUTO_SEND_NEXT
#undef PJSIP_AUTH_AUTO_SEND_NEXT
#endif
#define PJSIP_AUTH_AUTO_SEND_NEXT 1

#ifdef PJSIP_AUTH_HEADER_CACHING
#undef PJSIP_AUTH_HEADER_CACHING
#endif
#define PJSIP_AUTH_HEADER_CACHING 1


#include "mmstools/tools.h"
#include "mmstools/mmserror.h"
#include "mmssip/mmssip.h"

/*
 * TODO:
 * - using STUN server
 */

static MMSSip *thiz = NULL;
static bool   registered = false;

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
    logCfg.console_level = 1;

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

    sleep(1);

    /* register to SIP server */
    pjsua_acc_config accCfg;

    char *tmpid = (char*)malloc(80);
    snprintf(tmpid, 80, "sip:%s@%s", user.c_str(), registrar.c_str());
    char *tmpreg = (char*)malloc(80);
    snprintf(tmpreg, 80, "sip:%s", realm.c_str());
    char *tmprealm = (char*)malloc(80);
    snprintf(tmprealm, 80, "*");
    char *tmpuser = (char*)malloc(80);
    snprintf(tmpuser, 80, "%s", user.c_str());
    char *tmppasswd = (char*)malloc(80);
    snprintf(tmppasswd, 80, "%s", passwd.c_str());
    char *tmpscheme = (char*)malloc(80);
    snprintf(tmpscheme, 80, "Digest");

    pjsua_acc_config_default(&accCfg);
    accCfg.reg_timeout  = 60;
    accCfg.id         = pj_str(tmpid);
    accCfg.reg_uri    = pj_str(tmpreg);
    accCfg.cred_count = 1;
    accCfg.cred_info[0].realm     = pj_str(tmprealm);
    accCfg.cred_info[0].scheme    = pj_str(tmpscheme);
    accCfg.cred_info[0].username  = pj_str(tmpuser);
    accCfg.cred_info[0].data_type = 0;
    accCfg.cred_info[0].data      = pj_str(tmppasswd);
    accCfg.publish_enabled        = PJ_FALSE;

    status = pjsua_acc_add(&accCfg, PJ_TRUE, &this->accID);
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error registering account sip:" + user + "@" + registrar + " (pjsua_acc_add)");
		throw MMSError(0, "Error registering account sip:" + user + "@" + registrar + " (pjsua_acc_add)");
	}

    DEBUGMSG("MMSSIP", "SIP account registered");

    this->onCallSuccessfull    = new sigc::signal<void, int>;
    this->onCallIncoming       = new sigc::signal<void, int, string>;
    this->onCallDisconnected   = new sigc::signal<void, int>;
    this->onCalling            = new sigc::signal<void, int>;
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

    if(!registered) {
    	DEBUGMSG("MMSSIP", "Cannot make a call (not registered)");
    	throw MMSError(0, "Cannot make a call (not registered)");
    }

    const char *cDomain;
    if(user.find("@") == string::npos) {
        cDomain = ((domain != "") ? domain.c_str() : this->registrar.c_str());
        snprintf(tmp, 1024, "sip:%s@%s", user.c_str(), cDomain);
    }
    else
        snprintf(tmp, 1024, "sip:%s", user.c_str());

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

	return call;
}

void MMSSip::hangup(int id) {
	pj_status_t        status;
    static bool        registeredThread = false;

    DEBUGMSG("MMSSIP", "calling pjsua_call_hangup");

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

    pjsua_call_hangup(id, 0, NULL, NULL);
}

void MMSSip::answer(int id) {
	pj_status_t        status;
    static bool        registeredThread = false;

    DEBUGMSG("MMSSIP", "calling pjsua_call_answer");

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

    pjsua_call_answer(id, 200, NULL, NULL);
}

void MMSSip::addBuddy(const string &name, const string &uri) {
	pj_status_t        status;
    pjsua_buddy_config buddyCfg;
    pjsua_buddy_id     buddyId;
    pjsua_buddy_info   buddyInfo;
    static bool        registeredThread = false;

    if(!registered) {
    	DEBUGMSG("MMSSIP", "Cannot add buddy (not registered)");
    	throw MMSError(0, "Cannot add buddy (not registered)");
    }

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

    pjsua_buddy_config_default(&buddyCfg);
    char buri[80];
    sprintf(buri, "sip:%s", uri.c_str());
    buddyCfg.uri = pj_str(buri);
    buddyCfg.subscribe = true;
    if(pjsua_buddy_add(&buddyCfg, &buddyId) == PJ_SUCCESS) {
    	DEBUGMSG("MMSSIP", "successfully added buddy " + name);
    	MMSSipBuddy buddy = {name, uri, BUDDY_UNKNOWN};
    	buddies[buddyId] = buddy;
    	if(pjsua_buddy_get_info(buddyId, &buddyInfo) == PJ_SUCCESS) {
    		buddy.status = (MMSSipBuddyStatus)buddyInfo.status;
    	}
       	buddies[buddyId] = buddy;
       	onBuddyState(buddyId);
    }
    else
	    DEBUGMSG("MMSSIP", "failed to add buddy " + name);
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

    DEBUGMSG("MMSSIP", "Incoming call from %.*s (id=%d)", (int)ci.remote_info.slen, ci.remote_info.ptr, callId);

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
            if(thiz && thiz->onCalling)
                thiz->onCalling->emit(callId);
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
        	if(thiz && thiz->onCallDisconnected)
                thiz->onCallDisconnected->emit(callId);
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
		if(info.status == 200) registered = true;
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
