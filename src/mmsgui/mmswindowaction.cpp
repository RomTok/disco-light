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

#include "mmsgui/mmswindowaction.h"

MMSWindowAction::MMSWindowAction(class MMSWindow* destination) : MMSThread("MMSWindowAction") {
	this->window = destination;
	this->id = idFactory.getId();
	this->action = MMSWACTION_NONE;
	this->stopaction = false;
	this->cancelBroadcast.connect(sigc::mem_fun(this,&MMSWindowAction::onCancelBroadcast));
}

MMSWindowAction::~MMSWindowAction() {
}

int hideCnt = 0;

void MMSWindowAction::threadMain() {

    try {

    	this->stopaction = false;
    
    	switch(this->action) {
    		case MMSWACTION_SHOW:
                while (hideCnt) msleep(100);
    			this->window->showAction(&(this->stopaction));
    			break;
    		case MMSWACTION_HIDE:
                hideCnt++;
    			this->window->hideAction(&(this->stopaction));
                hideCnt--;
    			break;
    		default:
    			break;		
    	}
    	this->stopaction = false;

    } catch(MMSError *error) {
        logger.writeLog("Abort due to: " + error->getMessage());
    }
}


void MMSWindowAction::cancelCleanup() {
	this->window->instantHide();
}

void MMSWindowAction::onCancelBroadcast(int type) {
	if (this->isRunning()) {
        if (this->action == MMSWACTION_HIDE) {
            while (this->isRunning()) msleep(100);
            return;
        }

		this->stopaction = true;
		int i = 0;
		while (this->stopaction) {
			msleep(100);
			i++;
			if (i > 20) this->cancel();
		}
		if (!this->stopaction) {
			this->window->hideAction(&(this->stopaction));
        }
		else
			this->stopaction = false;
	}
}

void MMSWindowAction::setAction(MMSWACTION action) {
	this->action=action;
}

sigc::signal<void,int> MMSWindowAction::cancelBroadcast;

