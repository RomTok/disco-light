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

#ifndef IMMSWINDOWMANAGER_H_
#define IMMSWINDOWMANAGER_H_

#include "mmsgui/mmswindow.h"


class IMMSWindowManager {
    public:
        virtual ~IMMSWindowManager() {};

        virtual MMSFBRectangle getVRect() = 0;

        virtual void addWindow(MMSWindow *window) = 0;
        virtual void removeWindow(MMSWindow *window) = 0;

        virtual bool hideAllMainWindows(bool goback = false) = 0;
        virtual bool hideAllPopupWindows() = 0;
        virtual bool hideAllRootWindows(bool willshown = false) = 0;

        virtual void setToplevelWindow(MMSWindow *window) = 0;
        virtual MMSWindow *getToplevelWindow() = 0;
		virtual void removeWindowFromToplevel(MMSWindow *window) = 0;

        virtual void setBackgroundWindow(MMSWindow *window) = 0;
        virtual MMSWindow *getBackgroundWindow() = 0;

        virtual void setPointerPosition(int pointer_posx, int pointer_posy, bool pressed = false) = 0;
};

#endif /*IMMSWINDOWMANAGER_H_*/
