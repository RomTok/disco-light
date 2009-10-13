/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
 ***************************************************************************/

#ifndef IMMSOSDPLUGIN_H_
#define IMMSOSDPLUGIN_H_

#include "mmstools/mmserror.h"
#include "mmsconfig/mmsplugindata.h"
#include "mmsbase/interfaces/immsevent.h"
#include "mmsbase/interfaces/immsswitcher.h"

class IMMSOSDPlugin {
    public:
        virtual bool initialize(MMSPluginData data, IMMSSwitcher *switcher) = 0;
        virtual bool onEvent(IMMSEvent event) = 0;
        virtual bool shutdown() = 0;
        virtual bool showPreview(void *data) = 0;
        virtual bool show(void *data) = 0;
        virtual ~IMMSOSDPlugin() {};
};

#define MMS_EXPORT_OSD_PLUGIN(classname) extern "C" { IMMSOSDPlugin *newOSDPlugin() { return new classname; }}

extern "C"  IMMSOSDPlugin *newOSDPlugin();
typedef IMMSOSDPlugin *(*NEWOSDPLUGIN_PROC)();

#endif /*IMMSOSDPLUGIN_H_*/
