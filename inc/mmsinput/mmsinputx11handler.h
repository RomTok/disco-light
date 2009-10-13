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

/*
 * mmsinputx11handler.h
 *
 *  Created on: 30.10.2008
 *      Author: sxs
 */

#ifndef MMSINPUTX11HANDLER_H_
#define MMSINPUTX11HANDLER_H_

#include "mmsgui/mmsguitools.h"
#include "mmsbase/mmsbase.h"
#include "mmsinput/mmsinputhandler.h"


#ifdef  __HAVE_XLIB__
#include <X11/Xlib.h>
#include <X11/Xutil.h>



class MMSInputX11Handler : public MMSInputHandler {
	private:
		MMSConfigData *config;

		bool					quit;
		Window 					window;
		Display					*display;

	public:
		MMSInputX11Handler(MMS_INPUT_DEVICE device);
		~MMSInputX11Handler();
		void grabEvents(MMSInputEvent *inputevent);
};

#else
class MMSInputX11Handler : public MMSInputHandler {
	public:
		MMSInputX11Handler(MMS_INPUT_DEVICE device);
		~MMSInputX11Handler();
		void grabEvents(MMSInputEvent *inputevent);
};

#endif

#endif /* MMSINPUTX11HANDLER_H_ */
