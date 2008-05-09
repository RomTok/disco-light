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

#ifndef MMSOSDPLUGINHANDLER_H_
#define MMSOSDPLUGINHANDLER_H_

#include "mmstools/base.h"
#include "mmstools/mmserror.h"
#include "mmstools/mmsshlhandler.h"
#include "mmsconfig/mmsplugindata.h"
#include "mmsbase/interfaces/immsosdplugin.h"

MMS_CREATEERROR(MMSOSDPluginError);


class MMSOSDPluginHandler {
    private:
        bool 			loaded;
        MMSPluginData 	plugindata;
        IMMSOSDPlugin 	*plugin;
        MMSShlHandler 	*handler;
        MMSMutex 		calllock;
        IMMSSwitcher 	*switcher;
       
    public:
        MMSOSDPluginHandler(MMSPluginData plugindata, bool autoload = false);
        ~MMSOSDPluginHandler();
        
        bool isLoaded();
        void load();
        void unload();
        MMSPluginData getPluginData();
        void setPluginData(MMSPluginData plugindata);
        void setSwitcherInterface(IMMSSwitcher *switcher);
        void invokeInitialize(void *data = NULL);
        void invokeOnEvent(IMMSEvent event);
        void invokeShutdown(void *data = NULL);
        void invokeShowPreview(void *data = NULL);
        void invokeShow(void *data = NULL);
};

#endif /*MMSOSDPLUGINHANDLER_H_*/
