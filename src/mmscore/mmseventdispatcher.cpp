/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
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

#include "mmscore/mmseventdispatcher.h"
#include "mmscore/mmseventthread.h"


MMSEventDispatcher::MMSEventDispatcher(MMSPluginManager *manager,MMSEventSignupManager *signupmanager) {
    this->manager = manager;
    this->signupmanager = signupmanager;
}

MMSEventDispatcher::~MMSEventDispatcher() {
}

void MMSEventDispatcher::raise(_IMMSEvent *event, int id) {
    MMSEventThread *thread;
    vector <MMSOSDPluginHandler *> osdHandlers;
    vector <MMSCentralPluginHandler *> centralHandlers;
    vector <MMSBackendPluginHandler *> backendHandlers;
    vector <MMSPluginData *> plugins;
    IMMSEvent e(event);

    if (id > 0) {
    	DEBUGMSG("MMSEventdispatcher", "have a direct receiver");
    	try {
    		thread = new MMSEventThread(this->getManager()->getOSDPluginHandler(id), e);
	    } catch(MMSError *error) {
    		thread = new MMSEventThread(this->getManager()->getCentralPluginHandler(id), e);
	    }
        thread->start();
    } else {
    	DEBUGMSG("MMSEventdispatcher", "get receiver plugins");
        /* get all receiver plugins */
        try {
        	plugins = this->getSignupManager()->getReceiverPlugins(event);

        	DEBUGMSG("MMSEventdispatcher", "filter the osd handler");
	        /* get all osd handlers */
	        osdHandlers = getManager()->getOSDPluginHandlers(plugins);
	        for(unsigned int i=0; i<osdHandlers.size();i++) {
	        	DEBUGMSG("MMSEventdispatcher", "%s --> create new event thread for %s.", (osdHandlers.at(i))->getPluginData().getName().c_str(), event->getHeading().c_str());
	            /* start the threads */
	            thread = new MMSEventThread(osdHandlers.at(i), e);
	            thread->start();
	        }

	        DEBUGMSG("MMSEventdispatcher", "filter the central handler");
	        /* get all central handlers */
	        centralHandlers = getManager()->getCentralPluginHandlers(plugins);
	        for(unsigned int i=0; i<centralHandlers.size();i++) {
	        	DEBUGMSG("MMSEventdispatcher", "%s --> create new event thread for %s", (centralHandlers.at(i))->getPluginData().getName().c_str(), event->getHeading().c_str());
	            /* start the threads */
	            thread = new MMSEventThread(centralHandlers.at(i), e);
	            thread->start();
	        }

	        DEBUGMSG("MMSEventdispatcher", "filter the backend handler");
	        /* get all central handlers */
	        backendHandlers = getManager()->getBackendPluginHandlers(plugins);
	        for(unsigned int i=0; i<backendHandlers.size();i++) {
	            /* start the threads */
	        	DEBUGMSG("MMSEventdispatcher", "%s --> create new event thread for %s.", (backendHandlers.at(i))->getPluginData().getName().c_str(), event->getHeading().c_str());
	            thread = new MMSEventThread(backendHandlers.at(i), e);
	            thread->start();
	        }

        } catch (MMSEventSignupManagerError *err) {
        	DEBUGMSG("MMSEventdispatcher", "Error: %s", err->getMessage().c_str());
        	return;
        }
        for(vector<MMSPluginData *>::iterator it = plugins.begin(); it != plugins.end(); ++it)
        	delete *it;
    }
}

MMSPluginManager *MMSEventDispatcher::getManager() {
    return this->manager;
}
MMSEventSignupManager *MMSEventDispatcher::getSignupManager() {
    return this->signupmanager;
}
