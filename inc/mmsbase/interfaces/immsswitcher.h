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

#ifndef IMMSSWITCHER_H_
#define IMMSSWITCHER_H_

#include "mmsgui/mmschildwindow.h"
#include "mmsconfig/mmsplugindata.h"

class MMSPluginManager;
class MMSInputManager;

class IMMSSwitcher {
    public:
        virtual ~IMMSSwitcher() {};

        virtual void show() = 0;
        virtual void hide() = 0;

		virtual void setWindowManager(IMMSWindowManager *wm) = 0;
		virtual void setPluginManager(MMSPluginManager *pm) = 0;
		virtual void setInputManager(MMSInputManager  *im) = 0;

		virtual MMSChildWindow* loadPreviewDialog(string filename, MMSTheme *theme = NULL, int id=-1) = 0;
        virtual MMSChildWindow* loadInfoBarDialog(string filename, MMSTheme *theme = NULL) = 0;

        virtual void setVolume(unsigned int volume, bool init = false) = 0;

        virtual IMMSSwitcher *newSwitcher(MMSPluginData *plugindata) = 0;

        virtual bool switchToPlugin() = 0;
        virtual bool leavePlugin(bool show_switcher) = 0;
};

#endif /*IMMSSWITCHER_H_*/
