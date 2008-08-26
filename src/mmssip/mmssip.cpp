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

static void onIncomingCall(pjsua_acc_id, pjsua_call_id, pjsip_rx_data*);
static void onCallState(pjsua_call_id, pjsip_event*);
static void onCallMediaState(pjsua_call_id);

MMSSip::MMSSip(const string    &user,
		       const string    &passwd,
		       const string    &registrar,
		       const short int &localPort) :
    user(user),
    passwd(passwd),
    registrar(registrar) {

	pj_status_t status;

    /* create pjsua */
    status = pjsua_create();
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error initializing SIP stack (pjsua_create)");
		throw MMSError(0, "Error initializing SIP stack (pjsua_create)");
	}

    /* configure pjsua */
    pjsua_config         cfg;
    pjsua_logging_config logCfg;

    pjsua_config_default(&cfg);
    cfg.cb.on_incoming_call    = &onIncomingCall;
    cfg.cb.on_call_media_state = &onCallMediaState;
    cfg.cb.on_call_state       = &onCallState;

    pjsua_logging_config_default(&logCfg);
    logCfg.console_level = 4;

    status = pjsua_init(&cfg, &logCfg, NULL);
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error initializing SIP stack (pjsua_init)");
		throw MMSError(0, "Error initializing SIP stack (pjsua_init)");
	}

    /* add UDP transport */
    pjsua_transport_config transCfg;

    pjsua_transport_config_default(&transCfg);
    transCfg.port = localPort;
    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transCfg, NULL);
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error initializing SIP stack (pjsua_transport_create)");
		throw MMSError(0, "Error initializing SIP stack (pjsua_transport_create)");
	}

    /* start pjsua */
    status = pjsua_start();
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error starting SIP stack (pjsua_start)");
		throw MMSError(0, "Error starting SIP stack (pjsua_start)");
	}

    /* register to SIP server */
    pjsua_acc_config accCfg;
    char tmp[1024];

    pjsua_acc_config_default(&accCfg);
    snprintf(tmp, sizeof(tmp), "sip:%s@%s", user.c_str(), registrar.c_str());
    accCfg.id         = pj_str(tmp);
    printf("id: %s\n", tmp);
    snprintf(tmp, sizeof(tmp), "sip:%s", registrar.c_str());
    accCfg.reg_uri    = pj_str(tmp);
    printf("reg_uri: %s\n", tmp);
    accCfg.cred_count = 1;
    accCfg.cred_info[0].realm     = pj_str((char*)registrar.c_str());
    accCfg.cred_info[0].scheme    = pj_str((char*)"digest");
    accCfg.cred_info[0].username  = pj_str((char*)user.c_str());
    accCfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    accCfg.cred_info[0].data      = pj_str((char*)passwd.c_str());

    status = pjsua_acc_add(&accCfg, PJ_TRUE, &this->accID);
    if(status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error registering account sip:" + user + "@" + registrar + " (pjsua_acc_add)");
		throw MMSError(0, "Error registering account sip:" + user + "@" + registrar + " (pjsua_acc_add)");
	}
}

MMSSip::~MMSSip() {
	pjsua_destroy();
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
callID MMSSip::call(const string &user, const string &domain) {
	pj_status_t    status;
	pj_str_t       uri;
    pjsua_call_id  call;
    char           tmp[1024];

    snprintf(tmp, 1024, "sip:%s@%s", user.c_str(), domain.c_str());
	status = pjsua_verify_sip_url(tmp);
	if (status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Invalid callee info sip:" + user + "@" + domain);
		throw MMSError(0, "Invalid callee info sip:" + user + "@" + domain);
	}

    uri = pj_str(tmp);
	status = pjsua_call_make_call(this->accID, &uri, 0, NULL, NULL, &call);
	if (status != PJ_SUCCESS) {
		DEBUGMSG("MMSSIP", "Error calling sip:" + user + "@" + domain);
		throw MMSError(0, "Error calling sip:" + user + "@" + domain);
	}

	/* insert call into activeCalls */
	this->activeCalls.push_back(call);

	return (this->activeCalls.size() - 1);
}

void MMSSip::hangup(const callID &id) {
	pjsua_call_id cid = this->activeCalls.at(id);
    pjsua_call_hangup(cid, 0, NULL, NULL);
}


/* Callback called by the library upon receiving incoming call */
static void onIncomingCall(pjsua_acc_id  accId,
		                   pjsua_call_id callId,
                           pjsip_rx_data *rdata) {
    pjsua_call_info ci;

    PJ_UNUSED_ARG(accId);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(callId, &ci);

    DEBUGMSG("MMSSIP", "Incoming call from %.*s!!", (int)ci.remote_info.slen, ci.remote_info.ptr);

    /* Automatically answer incoming calls with 200/OK */
    pjsua_call_answer(callId, 200, NULL, NULL);
}

/* Callback called by the library when call's state has changed */
static void onCallState(pjsua_call_id callId, pjsip_event *e) {
    pjsua_call_info ci;

    PJ_UNUSED_ARG(e);

    pjsua_call_get_info(callId, &ci);
    DEBUGMSG("MMSSIP", "Call %d state=%.*s", callId, (int)ci.state_text.slen, ci.state_text.ptr);
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

#endif /* _NO_MMSSIP */
