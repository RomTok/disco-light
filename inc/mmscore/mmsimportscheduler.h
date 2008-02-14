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

#ifndef MMSIMPORTSCHEDULER_H_
#define MMSIMPORTSCHEDULER_H_

#include "mmsbase/interfaces/immsimportscheduler.h"
#include "mmsbase/mmsbase.h"
#include "mmsconfig/mmsconfig.h"
#include "mmscore/mmspluginmanager.h"

typedef struct {
    MMSPluginData           *plugin;
    MMSImportPropertyData   *importProperty;
    MMSImportPluginHandler  *pluginHandler;
    int                     nextTime;
} IMPORT_PLUGINS;

class MMSImportScheduler : public MMSThread {
    private:
        MMSPluginManager            *pluginManager;
        MMSPluginService            *pluginService;
        MMSImportPropertyService    *importPropertyService;
        vector<IMPORT_PLUGINS*>     importPlugins;

        void getImportPlugins();

    public:
		void executeImport(int pluginID);
        MMSImportScheduler(MMSPluginManager *pluginManager);
        virtual ~MMSImportScheduler();
        virtual void threadMain();
};

#endif /*MMSIMPORTSCHEDULER_H_*/
