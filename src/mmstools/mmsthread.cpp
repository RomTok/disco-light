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

#include "mmsthread.h"
#include "mmserror.h"
#include "tools.h"

extern "C" {
#include <direct/debug.h>
#include <direct/thread.h>
#include <direct/trace.h>
}

D_DEBUG_DOMAIN( MMS_Thread, "MMS/Thread", "MMS Thread" );


static void *startmythread(void *thiz) {
	static_cast<MMSThread *>(thiz)->run();
	return NULL;
}

MMSThread::MMSThread(string identity) {
    D_DEBUG_AT( MMS_Thread, "MMSThread( %s )\n", identity.c_str() );

    direct_trace_print_stack(NULL);

    this->identity = identity;

    logger.setIdentity(identity);
    this->isrunning=false;
}

void MMSThread::run() {
	try {
        direct_thread_set_name( this->identity.c_str() );
		this->detach();
        this->isrunning = true;
		threadMain();
        this->isrunning = false;

	} catch(MMSError *error) {
        this->isrunning = false;
	    logger.writeLog("Abort due to: " + error->getMessage());
	}
}

void MMSThread::start() {
	pthread_create(&this->id, NULL, startmythread, static_cast<void *>(this));
}

void MMSThread::detach() {
	pthread_detach(this->id);
}

bool MMSThread::isRunning() {
	return this->isrunning;
}

int MMSThread::cancel() {
	int status;
	if(this->isrunning)
		status = pthread_cancel(this->id);
	this->isrunning=false;
	return status;
	
}
