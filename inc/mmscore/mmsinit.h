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

#ifndef MMSINIT_H_
#define MMSINIT_H_

#include <stdlib.h>
#include <string>
#include <mmsbase/interfaces/immsswitcher.h>

typedef enum {
	MMSINIT_NONE 			= 0x00000000,
	MMSINIT_WINDOWMANAGER 	= 0x00000001,
	MMSINIT_PLUGINMANAGER 	= 0x00000002,
	MMSINIT_EVENTS 			= 0x00000004,
	MMSINIT_GRAPHICS 		= 0x00000008,
	MMSINIT_INPUTS 			= 0x00000010,
	MMSINIT_FULL 			= 0x0000001f
} MMSINIT_FLAGS;

bool mmsInit(MMSINIT_FLAGS flags, int argc = 0, char *argv[] = NULL, string configfile = "");

bool registerSwitcher(IMMSSwitcher *switcher);

IMMSWindowManager *getWindowManager();

#endif /*MMSINIT_H_*/
