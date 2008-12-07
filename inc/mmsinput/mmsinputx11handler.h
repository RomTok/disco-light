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
