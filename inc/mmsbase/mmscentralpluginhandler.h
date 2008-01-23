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

#ifndef MMSCENTRALPLUGINHANDLER_H_
#define MMSCENTRALPLUGINHANDLER_H_

#include "mmstools.h"
#include "mmsconfig.h"
#include "interfaces/immscentralplugin.h"
#include "mmsshlhandler.h"

MMS_CREATEERROR(MMSCentralPluginError);


class MMSCentralPluginHandler {
    private:
        bool loaded;
        MMSPluginData plugindata;
        IMMSCentralPlugin *plugin;
        MMSShlHandler *handler;
        MMSMutex calllock;
        IMMSSwitcher *switcher;
       
    public:
        MMSCentralPluginHandler(MMSPluginData plugindata, bool autoload = false);
        ~MMSCentralPluginHandler();
        
        bool isLoaded();
        void load();
        void unload();
        MMSPluginData getPluginData();
        void setPluginData(MMSPluginData plugindata);
        void setSwitcherInterface(IMMSSwitcher *switcher);
        void invokeInitialize(void *data = NULL);
        void invokeOnEvent(void *data = NULL);
        void invokeShutdown(void *data = NULL);
        void invokeShowPreview(void *data = NULL);
        void invokeShow(void *data = NULL);
};

#endif /*MMSCENTRALPLUGINHANDLER_H_*/
