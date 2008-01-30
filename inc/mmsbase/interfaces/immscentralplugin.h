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

#ifndef IMMSCENTRALPLUGIN_H_
#define IMMSCENTRALPLUGIN_H_

#include "mmstools/mmserror.h"
#include "mmsconfig/mmsplugindata.h"
#include "mmsbase/interfaces/immsevent.h"
#include "mmsbase/interfaces/immsswitcher.h"

class IMMSCentralPlugin {
    public:
        virtual bool initialize(MMSPluginData data, IMMSSwitcher *switcher) = 0;
        virtual bool onEvent(IMMSEvent event) = 0;
        virtual bool shutdown() = 0;
        virtual bool showPreview(void *data) = 0;
        virtual bool show(void *data) = 0;
        virtual ~IMMSCentralPlugin() {};
};

#define MMS_EXPORT_CENTRAL_PLUGIN(classname) extern "C" { IMMSCentralPlugin *newCentralPlugin() { return new classname; }}

extern "C"  IMMSCentralPlugin *newCentralPlugin();
typedef IMMSCentralPlugin *(*NEWCENTRALPLUGIN_PROC)();

#endif /*IMMSCENTRALPLUGIN_H_*/
