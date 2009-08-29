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

#include "mmsgui/mmswindowmanager.h"
#include "mmsgui/fb/mmsfbwindowmanager.h"


MMSWindowManager::MMSWindowManager(MMSFBRectangle vrect) {
    this->vrect = vrect;
    this->toplevel = NULL;

    // add language changed callback
    onTargetLangChanged_connection = this->translator.onTargetLangChanged.connect(sigc::mem_fun(this, &MMSWindowManager::onTargetLangChanged));
}

MMSWindowManager::~MMSWindowManager() {
	onTargetLangChanged_connection.disconnect();
}

void MMSWindowManager::reset() {
	mmsfbwindowmanager->reset();
}

MMSFBRectangle MMSWindowManager::getVRect() {
    return this->vrect;
}

void MMSWindowManager::addWindow(MMSWindow *window) {
    // add window to list
    this->windows.push_back(window);
}

void MMSWindowManager::removeWindow(MMSWindow *window){
    /* search for the window and erase it */
    for(unsigned int i = 0; i < windows.size(); i++) {
        if(window != windows.at(i))
            continue;
        windows.erase(windows.begin()+i);
        if (window == this->toplevel)
            this->toplevel = NULL;
        return;
    }
}

bool MMSWindowManager::lowerToBottom(MMSWindow *window) {
	return window->lowerToBottom();
}

bool MMSWindowManager::raiseToTop(MMSWindow *window) {
    // searching for popup windows and count it
	int cnt = 0;
    for(unsigned int i = 0; i < windows.size(); i++) {
        if (windows.at(i)->getType() == MMSWINDOWTYPE_POPUPWINDOW) {
            if (windows.at(i)->isShown()) cnt++;
        }
    }

    // raise window to top, cnt number of windows are over it
    return window->raiseToTop(cnt);
}

bool MMSWindowManager::hideAllMainWindows(bool goback) {
    bool ret = false;

    // searching for main windows
    for(unsigned int i = 0; i < windows.size(); i++) {
        if (windows.at(i)->getType() == MMSWINDOWTYPE_MAINWINDOW) {
            if (windows.at(i)->isShown()) {
                if (this->toplevel == windows.at(i)) {
                    removeWindowFromToplevel(windows.at(i));
                    windows.at(i)->hide(goback);
                }
                else
                    windows.at(i)->hide();
                ret = true;
            }
        }
    }

    // return true if at least one main window was found
    return ret;
}

bool MMSWindowManager::hideAllPopupWindows(bool except_modal) {
    bool ret = false;

    // searching for popup windows
    for(unsigned int i = 0; i < windows.size(); i++) {
        if (windows.at(i)->getType() == MMSWINDOWTYPE_POPUPWINDOW) {
            if (windows.at(i)->isShown()) {
            	if (except_modal) {
					// hide only non-modal popups
					bool modal;
					if (windows.at(i)->getModal(modal)) {
						if (modal)
							continue;
					}
            	}
                windows.at(i)->hide();
                ret = true;
            }
        }
    }

    // return true if at least one popup window was found
    return ret;
}

bool MMSWindowManager::hideAllRootWindows(bool willshown) {
    bool ret = false;

    // search for root windows
    for(unsigned int i = 0; i < windows.size(); i++)
        if (windows.at(i)->getType() == MMSWINDOWTYPE_ROOTWINDOW)
            if (windows.at(i)->isShown()) {
                if (this->toplevel == windows.at(i)) {
                    removeWindowFromToplevel(windows.at(i));
                    windows.at(i)->hide();
                }
                else
                    windows.at(i)->hide();
                ret = true;
            }

    // if at least one root window was hidden and no other will shown, show the default root window
    if ((ret)&&(!willshown))
    	showBackgroundWindow();

    // return true if at least one root window was found
    return ret;
}

void MMSWindowManager::setToplevelWindow(MMSWindow *window) {
    if (window->getType() == MMSWINDOWTYPE_MAINWINDOW) {
        // searching for active popup window
        for(unsigned int i = 0; i < windows.size(); i++) {
            if (windows.at(i)->getType() == MMSWINDOWTYPE_POPUPWINDOW) {
                if (windows.at(i)->isShown()&&(!windows.at(i)->willHide())) {
                    // set active popup window as toplevel
					bool modal;
					if (windows.at(i)->getModal(modal)) {
						if (modal) {
		                	this->toplevel = windows.at(i);
		                    return;
						}
					}
                }
            }
        }
    }
    if (window->getType() == MMSWINDOWTYPE_ROOTWINDOW) {
        // searching for active main window
        for(unsigned int i = 0; i < windows.size(); i++) {
            if (windows.at(i)->getType() == MMSWINDOWTYPE_MAINWINDOW) {
                if (windows.at(i)->isShown()&&(!windows.at(i)->willHide())) {
                    // set active main window as toplevel
                    this->toplevel = windows.at(i);
                    return;
                }
            }
        }
    }
    this->toplevel = window;
}

MMSWindow *MMSWindowManager::getToplevelWindow() {
    return this->toplevel;
}

void MMSWindowManager::removeWindowFromToplevel(MMSWindow *window) {
	// toplevel window?
    if (this->toplevel != window)
    	return;

	if (window->getType() == MMSWINDOWTYPE_POPUPWINDOW) {
		// a popup window will be hidden, so try to find an active
		// popup or main window to get the toplevel status
		for(unsigned int i = 0; i < windows.size(); i++) {
			if (windows.at(i)->getType() == MMSWINDOWTYPE_POPUPWINDOW) {
				if (windows.at(i) != window && windows.at(i)->isShown()) {
					// set active popup window as toplevel
					this->toplevel = windows.at(i);
					return;
				}
			}
		}
		for(unsigned int i = 0; i < windows.size(); i++) {
			if (windows.at(i)->getType() == MMSWINDOWTYPE_MAINWINDOW) {
				if (windows.at(i)->isShown()) {
					// set active main window as toplevel
					this->toplevel = windows.at(i);
					return;
				}
			}
		}
	}

	if ((window->getType() == MMSWINDOWTYPE_MAINWINDOW) || (window->getType() == MMSWINDOWTYPE_POPUPWINDOW)) {
		// a main or popup window will be hidden, so try to find an active
		// root window to get the toplevel status
		for(unsigned int i = 0; i < windows.size(); i++) {
			if (windows.at(i)->getType() == MMSWINDOWTYPE_ROOTWINDOW) {
				if (windows.at(i)->isShown()) {
					// set active root window as toplevel
					this->toplevel = windows.at(i);
					return;
				}
			}
		}
	}

	// no window found, no toplevel :)
	this->toplevel = NULL;
}

void MMSWindowManager::setBackgroundWindow(MMSWindow *window) {
    if (window)
        this->backgroundwindow = window;
    showBackgroundWindow();
}

MMSWindow *MMSWindowManager::getBackgroundWindow() {
    return this->backgroundwindow;
}

void MMSWindowManager::showBackgroundWindow() {
	if (this->backgroundwindow) {
		unsigned int opacity;
		if (this->backgroundwindow->getOpacity(opacity))
			if (opacity)
				this->backgroundwindow->show();
	}
}

void MMSWindowManager::setPointerPosition(int pointer_posx, int pointer_posy, bool pressed) {
	mmsfbwindowmanager->setPointerPosition(pointer_posx, pointer_posy, pressed);
}

MMSTranslator *MMSWindowManager::getTranslator() {
	return &this->translator;
}


void MMSWindowManager::onTargetLangChanged(MMS_LANGUAGE_TYPE lang) {
	// the language has changed, inform all windows
    for (unsigned int i = 0; i < this->windows.size(); i++)
        this->windows.at(i)->targetLangChanged(lang);
}


