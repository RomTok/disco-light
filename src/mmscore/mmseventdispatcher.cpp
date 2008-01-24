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

#include "mmscore/mmseventdispatcher.h"
#include "mmscore/mmseventthread.h"


MMSEventDispatcher::MMSEventDispatcher(MMSPluginManager *manager,MMSEventSignupManager *signupmanager) {
	logger.setIdentity("MMSEventdispatcher");
    this->manager = manager;
    this->signupmanager = signupmanager;
}

MMSEventDispatcher::~MMSEventDispatcher() {
}

void MMSEventDispatcher::raise(IMMSEvent *event, int id) {
    MMSEventThread *thread;
    vector <MMSOSDPluginHandler *> osdHandlers;
    vector <MMSCentralPluginHandler *> centralHandlers;
    vector <MMSBackendPluginHandler *> backendHandlers;
    vector <MMSPluginData *> plugins;


    if (id > 0) {
    	logger.writeLog("have a direct receiver");
    	try {
    		thread = new MMSEventThread(this->getManager()->getOSDPluginHandler(id),event);
	    } catch(MMSError *error) {
    		thread = new MMSEventThread(this->getManager()->getCentralPluginHandler(id),event);
	    }
        thread->start();
    } else {
    	logger.writeLog("get receiver plugins");
        /* get all receiver plugins */
        try {
        	plugins = this->getSignupManager()->getReceiverPlugins((MMSEvent *)event);

	    	logger.writeLog("filter the osd handler");
	        /* get all osd handlers */
	        osdHandlers = getManager()->getOSDPluginHandlers(plugins);
	        for(int i=0; i<osdHandlers.size();i++) {
	            logger.writeLog("create new event thread");
	            /* start the threads */
	            thread = new MMSEventThread(osdHandlers.at(i),event);
	            thread->start();
	        }
	
	    	logger.writeLog("filter the central handler");
	        /* get all central handlers */
	        centralHandlers = getManager()->getCentralPluginHandlers(plugins);
	        for(int i=0; i<centralHandlers.size();i++) {
	            logger.writeLog("create new event thread");
	            /* start the threads */
	            thread = new MMSEventThread(centralHandlers.at(i),event);
	            thread->start();
	        }
	
	    	logger.writeLog("filter the backend handler");
	        /* get all central handlers */
	        backendHandlers = getManager()->getBackendPluginHandlers(plugins);
	        for(int i=0; i<backendHandlers.size();i++) {
	            /* start the threads */
	            logger.writeLog("create new event thread");
	            thread = new MMSEventThread(backendHandlers.at(i),event);
	            thread->start();
	        }

        } catch (MMSEventSignupManagerError *err) {
        	logger.writeLog(err->getMessage());
        	return;
        }	
    }
}

MMSPluginManager *MMSEventDispatcher::getManager() {
    return this->manager;
}
MMSEventSignupManager *MMSEventDispatcher::getSignupManager() {
    return this->signupmanager;
}
