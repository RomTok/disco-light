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

#ifndef MMSINPUTHANDLER_H_
#define MMSINPUTHANDLER_H_

#include "mmsgui/mmsguitools.h"
#include "mmsbase/mmsbase.h"

#ifndef DFBCHECK
    #define DFBCHECK( x... ) \
    {\
         DFBResult err = x;\
         if (err != DFB_OK) {\
              fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );\
              DirectFBErrorFatal( #x, err );\
         }\
    }
#endif



class MMSInputHandler  {
	private:
		MMSConfigData *config;

		IDirectFBInputDevice    *input;
		IDirectFB			    *dfb;
		IDirectFBEventBuffer    *keybuffer;
		bool					quit;

		DFBRectangle	screen_rect;
		DFBRectangle	pointer_rect;

		int				xfac;
		int				yfac;
		
		int				pointer_xpos;
		int				pointer_ypos;

		int				pointer_old_xpos;
		int				pointer_old_ypos;

		int				button_pressed;
		
	public:
		MMSInputHandler(DFBInputDeviceID device);
		~MMSInputHandler();
		void grabEvents(MMSInputEvent *inputevent);
};

#endif /*MMSINPUTHANDLER_H_*/
