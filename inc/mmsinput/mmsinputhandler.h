/*
 * mmsinputhandler.h
 *
 *  Created on: 27.10.2008
 *      Author: sxs
 */

#ifndef MMSINPUTHANDLER_H_
#define MMSINPUTHANDLER_H_

typedef enum {
	MMS_INPUT_UNKNOWN=0,
	MMS_INPUT_KEYBOARD,
	MMS_INPUT_MOUSE,
	MMS_INPUT_REMOTE
} MMS_INPUT_DEVICE;
#include "mmsgui/mmsguitools.h"


class MMSInputHandler  {
	public:
		MMSInputHandler();
		virtual ~MMSInputHandler();
		virtual void grabEvents(MMSInputEvent *inputevent);
};

#endif /* MMSINPUTHANDLER_H_ */
