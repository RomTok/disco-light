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

#ifndef IMMSSWITCHER_H_
#define IMMSSWITCHER_H_

#include "mmschildwindow.h"
#include "mmsplugindata.h"


class IMMSSwitcher {
    public:
        virtual ~IMMSSwitcher() {};
        
        virtual void show() = 0;
        virtual void hide() = 0;

        virtual MMSChildWindow* loadPreviewDialog(string filename, MMSTheme *theme = NULL) = 0;
        virtual MMSChildWindow* loadInfoBarDialog(string filename, MMSTheme *theme = NULL) = 0;

        virtual void setVolume(unsigned int volume, bool init = false) = 0;

        virtual IMMSSwitcher *newSwitcher(MMSPluginData *plugindata) = 0;

        virtual bool switchToPlugin() = 0;
        
        
/*        virtual DFBRectangle getVRect() = 0;

        virtual void addWindow(MMSWindow *window) = 0;
        virtual void removeWindow(MMSWindow *window) = 0;

        virtual bool hideAllMainWindows(bool goback = false) = 0;
        virtual bool hideAllPopupWindows() = 0;
        virtual bool hideAllRootWindows() = 0;

        virtual void setToplevelWindow(MMSWindow *window) = 0;
        virtual MMSWindow *getToplevelWindow() = 0;
        virtual void removeWindowFromToplevel(MMSWindow *window) = 0;

        virtual void setBackgroundWindow(MMSWindow *window) = 0;
        virtual MMSWindow *getBackgroundWindow() = 0;*/
};

#endif /*IMMSSWITCHER_H_*/
