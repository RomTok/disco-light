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
