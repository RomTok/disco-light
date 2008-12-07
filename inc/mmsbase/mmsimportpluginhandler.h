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

#ifndef MMSIMPORTPLUGINHANDLER_H_
#define MMSIMPORTPLUGINHANDLER_H_

#include "mmstools/base.h"
#include "mmstools/mmserror.h"
#include "mmstools/mmsshlhandler.h"
#include "mmstools/mmsmutex.h"
#include "mmsconfig/mmsplugindata.h"
#include "mmsbase/interfaces/immsimportplugin.h"

MMS_CREATEERROR(MMSImportPluginError);


class MMSImportPluginHandler {
    private:
        bool 				loaded;
        MMSPluginData 		plugindata;
        IMMSImportPlugin	*plugin;
        MMSShlHandler 		*handler;
        MMSMutex 			calllock;

    public:
        MMSImportPluginHandler(MMSPluginData plugindata, bool autoload = false);
        ~MMSImportPluginHandler();

        bool isLoaded();
        void load();
        void unload();
        MMSPluginData getPluginData();
        void setPluginData(MMSPluginData plugindata);
        void invokeInitialize(void *data = NULL);
        void invokeExecute(void *data = NULL);
        void invokeShutdown(void *data = NULL);
        void invokeCleanUp(void *data = NULL);
};

#endif /*MMSIMPORTPLUGINHANDLER_H_*/
