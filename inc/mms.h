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

#ifndef MMS_H_
#define MMS_H_

#include <mmscore/mmspluginmanager.h>
#include <mmscore/mmscorelogger.h>
#include <mmscore/mmseventdispatcher.h>
#include <mmscore/mmseventthread.h>
#include <mmscore/mmseventsignupmanager.h>
#include <mmscore/mmsimportscheduler.h>
#include <mmscore/mmsinit.h>
#include <mmscore/mmstranslator.h>
#include <mmsgui/mmsgui.h>
#include <mmscore/mmsswitcher.h>
#include <mmstools/mmstools.h>
#include <mmsbase/mmsbase.h>
#include <mmsconfig/mmsconfig.h>
#include <mmsinput/mmsinput.h>
#ifdef __HAVE_MMSMEDIA__
#include <mmscore/mmsmusicmanager.h>
#include <mmsmedia/mmsmedia.h>
#endif
#ifdef __HAVE_MMSSIP__
#include <mmssip/mmssip.h>
#endif
#ifdef __HAVE_MMSFLASH__
#include <mmsflash/mmsflash.h>
#endif

#endif /*MMS_H_*/

