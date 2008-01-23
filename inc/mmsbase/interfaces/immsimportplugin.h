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

#ifndef IMMSIMPORTPLUGIN_H_
#define IMMSIMPORTPLUGIN_H_

#include "mmserror.h"
#include "mmsplugindata.h"

class IMMSImportPlugin {
    public:
        virtual bool initialize(MMSPluginData data) = 0;
        virtual bool execute() = 0;
        virtual bool shutdown() = 0;
        virtual bool cleanUp() = 0;
        virtual ~IMMSImportPlugin() {};
};

#define MMS_EXPORT_IMPORT_PLUGIN(classname) extern "C" { IMMSImportPlugin *newImportPlugin() { return new classname; }}

extern "C"  IMMSImportPlugin *newImportPlugin();
typedef IMMSImportPlugin *(*NEWIMPORTPLUGIN_PROC)();

#endif /*IMMSIMPORTPLUGIN_H_*/
