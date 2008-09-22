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

#include "mmsgui/mmswindowmanager.h"
#include "mmsgui/fb/mmsfbwindowmanager.h"


MMSWindowManager::MMSWindowManager(DFBRectangle vrect) {
    this->vrect = vrect;
    this->toplevel = NULL;
}

MMSWindowManager::~MMSWindowManager(){
}

DFBRectangle MMSWindowManager::getVRect() {
    return this->vrect;
}

void MMSWindowManager::addWindow(MMSWindow *window) {
    /* add window to list */
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

bool MMSWindowManager::hideAllMainWindows(bool goback) {
    bool ret = false;

    /* search for main windows */
    for(unsigned int i = 0; i < windows.size(); i++)
        if (windows.at(i)->getType() == MMSWINDOWTYPE_MAINWINDOW)
            if (windows.at(i)->isShown()) {
                if (this->toplevel == windows.at(i)) {
                    removeWindowFromToplevel(windows.at(i));
                    windows.at(i)->hide(goback);
                }
                else
                    windows.at(i)->hide();
                ret = true;
            }

    /* return true if at least one main window was found */
    return ret;
}

bool MMSWindowManager::hideAllPopupWindows() {
    bool ret = false;

    /* search for popup windows */
    for(unsigned int i = 0; i < windows.size(); i++)
        if (windows.at(i)->getType() == MMSWINDOWTYPE_POPUPWINDOW)
            if (windows.at(i)->isShown()) {
                windows.at(i)->hide();
                ret = true;
            }

    /* return true if at least one popup window was found */
    return ret;
}

bool MMSWindowManager::hideAllRootWindows(bool willshown) {
    bool ret = false;

    /* search for root windows */
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

    /* if at least one root window was hidden and no other will shown, show the default root window */
    if ((ret)&&(!willshown))
    	showBackgroundWindow();

    /* return true if at least one root window was found */
    return ret;
}

void MMSWindowManager::setToplevelWindow(MMSWindow *window) {
    if (window->getType() == MMSWINDOWTYPE_ROOTWINDOW) {
        /* search for active main window */
        for(unsigned int i = 0; i < windows.size(); i++)
            if (windows.at(i)->getType() == MMSWINDOWTYPE_MAINWINDOW)
                if (windows.at(i)->isShown()&&(!windows.at(i)->willHide())) {
                    /* set active main window as toplevel */
                    this->toplevel = windows.at(i); 
                    return;
                }
    }
    this->toplevel = window;
}

MMSWindow *MMSWindowManager::getToplevelWindow() {
    return this->toplevel;
}

void MMSWindowManager::removeWindowFromToplevel(MMSWindow *window) {
    if (this->toplevel == window) {
        if (window->getType() == MMSWINDOWTYPE_MAINWINDOW) {
            /* search for active root window */
            for(unsigned int i = 0; i < windows.size(); i++)
                if (windows.at(i)->getType() == MMSWINDOWTYPE_ROOTWINDOW)
                    if (windows.at(i)->isShown()) {
                        /* set active root window as toplevel */
                        this->toplevel = windows.at(i); 
                        return;
                    }
        }
        this->toplevel = NULL;
    }
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

void MMSWindowManager::setPointerPosition(int pointer_posx, int pointer_posy) {
	mmsfbwindowmanager->setPointerPosition(pointer_posx, pointer_posy);
}

