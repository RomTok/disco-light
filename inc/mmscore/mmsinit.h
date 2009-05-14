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

#ifndef MMSINIT_H_
#define MMSINIT_H_

#include <stdlib.h>
#include <string>
#include <mmsbase/interfaces/immsswitcher.h>

//! type of the init flags
typedef int MMSINIT_FLAGS;

//! none
#define MMSINIT_NONE			0x00000000
//! initializing the window manager
#define MMSINIT_WINDOWMANAGER 	0x00000001
//! initializing the plugin manager (the configdb where the plugins are defined is required)
#define MMSINIT_PLUGINMANAGER 	0x00000002
//! initializing the event manager
#define MMSINIT_EVENTS 			0x00000004
//! initializing the graphic backends (x11/dfb)
#define MMSINIT_GRAPHICS 		0x00000008
//! initializing the input manager
#define MMSINIT_INPUTS 			0x00000010
//! initializing the theme manager
#define MMSINIT_THEMEMANAGER	0x00000020
//! initializing the graphic backends including the window, the input and the theme manager
#define MMSINIT_WINDOWS			0x00000039
//! initializing all components
#define MMSINIT_FULL 			0x0000003f

bool mmsInit(MMSINIT_FLAGS flags, int argc = 0, char *argv[] = NULL, string configfile = "",
			 string appl_name = "Disko Application", string appl_icon_name = "Disko Application",
		     MMSConfigDataGlobal *global = NULL, MMSConfigDataDB *configdb = NULL, MMSConfigDataDB *datadb = NULL,
		     MMSConfigDataGraphics *graphics = NULL, MMSConfigDataLanguage *language = NULL);

bool mmsRelease();

bool registerSwitcher(IMMSSwitcher *switcher);

IMMSWindowManager *getWindowManager();

MMSPluginManager *getPluginManager();

//! get access to the video layer
/*!
\return pointer to the MMSFBLayer video layer object
\note If using only one layer, the graphics and video layer are the same.
*/
MMSFBLayer *getVideoLayer();

//! get access to the graphics layer
/*!
\return pointer to the MMSFBLayer graphics layer object
\note If using only one layer, the graphics and video layer are the same.
*/
MMSFBLayer *getGraphicsLayer();

#endif /*MMSINIT_H_*/
