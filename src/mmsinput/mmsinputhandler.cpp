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

#include "mmsinput/mmsinputhandler.h"
#include <directfb_keynames.h>

//static  DirectFBKeySymbolNames(keynames);
//static  DirectFBKeyIdentifierNames(idnames);

/*static int compare_symbol( const void *a, const void *b ) {
     DFBInputDeviceKeySymbol *symbol  = (DFBInputDeviceKeySymbol *) a;
     struct DFBKeySymbolName *symname = (struct DFBKeySymbolName *) b;

     return *symbol - symname->symbol;
}*/


MMSInputHandler::MMSInputHandler(DFBInputDeviceID device) {
	if(DirectFBCreate(&(this->dfb))!= DFB_OK)
        return;

	this->config = new MMSConfigData();


	/*grab the input device */
    //DFBCHECK(dfb->GetInputDevice( dfb, device, &input ));

	/*create an input queue */
    //DFBCHECK(input->CreateEventBuffer( input, &keybuffer ));
//    DFBCHECK(dfb->CreateInputEventBuffer(dfb,DICAPS_KEYS, (DFBBoolean)1, &keybuffer));
	DFBCHECK(dfb->CreateInputEventBuffer(dfb,DICAPS_ALL, (DFBBoolean)1, &keybuffer));

	/* get the screen rectangle */
	this->screen_rect.x = config->getVRect().x;
	this->screen_rect.y = config->getVRect().y;
	this->screen_rect.w = config->getVRect().w;
	this->screen_rect.h = config->getVRect().h;

	/* get the pointer rectangle */
	this->pointer_rect.x = config->getTouchRect().x;
	this->pointer_rect.y = config->getTouchRect().y;
	this->pointer_rect.w = config->getTouchRect().w;
	this->pointer_rect.h = config->getTouchRect().h;
	if ((this->pointer_rect.w<=0)||(this->pointer_rect.h<=0))
		if (config->getPointer()) {
			// no touch rect given but pointer needed
			this->pointer_rect.x = this->screen_rect.x;
			this->pointer_rect.y = this->screen_rect.y;
			this->pointer_rect.w = this->screen_rect.w;
			this->pointer_rect.h = this->screen_rect.h;
		}

	/* calculate a factor between screen and pointer rectangle */
	if ((this->pointer_rect.w > 0)&&(this->pointer_rect.h > 0)) {
		this->xfac = (100 * this->screen_rect.w) / this->pointer_rect.w;
		this->yfac = (100 * this->screen_rect.h) / this->pointer_rect.h;
		this->pointer_xpos = this->pointer_old_xpos = this->screen_rect.x + this->screen_rect.w / 2;
		this->pointer_ypos = this->pointer_old_ypos = this->screen_rect.y + this->screen_rect.h / 2;
	}
	else {
		/* this means that touch pad/screen is not used */
		this->pointer_rect.w = 0;
		this->pointer_rect.h = 0;
	}

	this->button_pressed = 0;

	this->quit=false;
}

MMSInputHandler::~MMSInputHandler()  {
	if(this->config) delete this->config;
}

void MMSInputHandler::grabEvents(MMSInputEvent *inputevent) {
	DFBInputEvent evt;

    bool event_buffered = false;
    inputevent->type = MMSINPUTEVENTTYPE_NONE;

	while(1) {
		/* wait for event with 100ms timeout */
		if (keybuffer->WaitForEventWithTimeout(keybuffer, 0, 100) != DFB_OK) {
			if (event_buffered)
				return;
			continue;
		}

	    /* process keybuffer */
	    if (keybuffer->PeekEvent( keybuffer, DFB_EVENT(&evt)) == DFB_OK) {
	    	/* check if i have to get it */
			if (event_buffered)
		    	if ((evt.type == DIET_BUTTONRELEASE) && (inputevent->type == MMSINPUTEVENTTYPE_BUTTONPRESS))
		    		/* if the next event is buttonrelease and the current buffered event is buttonpress */
		    		/* return the current event buttonpress */
		    		return;

	    	/* get the event */
		    if (keybuffer->GetEvent( keybuffer, DFB_EVENT(&evt)) == DFB_OK) {
		    	switch (evt.type) {
		    	case DIET_KEYPRESS:
		    			if (!this->button_pressed) {
			    			/* work with keypress event */
/*				        	symbol_name = (DFBKeySymbolName *)bsearch( &evt.key_symbol,keynames,sizeof(keynames) / sizeof(keynames[0]) - 1, sizeof(keynames[0]), compare_symbol );
				        	string name = symbol_name->name;
				        	logger.writeLog("key '" + name + "' pressed, id: " + iToStr(symbol_name->symbol));
*/
				        	/* fill the return structure */
				        	inputevent->type = MMSINPUTEVENTTYPE_KEYPRESS;
			        		inputevent->key = evt.key_symbol;
			        		return;
		    			}
		    			break;
		    	case DIET_KEYRELEASE:
		    			if (!this->button_pressed) {
			    			/* work with keyrelease event */
				        	/* fill the return structure */
				        	inputevent->type = MMSINPUTEVENTTYPE_KEYRELEASE;
			        		inputevent->key = evt.key_symbol;
			        		return;
		    			}
		    			break;
		    	case DIET_BUTTONPRESS:
		    			/* work with pressed buttons */
		    			if (this->pointer_rect.w) {
		    				if (evt.button == DIBI_LEFT) {
			    				/* work only with left button */
				    			/* fill the return structure */
					        	inputevent->type = MMSINPUTEVENTTYPE_BUTTONPRESS;
				        		inputevent->posx = this->pointer_xpos;
				        		inputevent->posy = this->pointer_ypos;

				    			/* buttonpress event */
				    			this->button_pressed++;

				    			if (this->button_pressed > 1)
				    				/* buttonpress event raised two times for touch pad/screens */
					        		return;

				    			/* all buttonpress events will be buffered */
				    			event_buffered = true;
			    			}
		    			}
		    			break;
		    	case DIET_BUTTONRELEASE:
	    				/* work with released buttons */
		    			if (this->pointer_rect.w) {
			    			if (evt.button == DIBI_LEFT) {
				    			/* fill the return structure */
					        	inputevent->type = MMSINPUTEVENTTYPE_BUTTONRELEASE;
				        		inputevent->posx = this->pointer_xpos;
				        		inputevent->posy = this->pointer_ypos;

				    			/* buttonrelease event */
				        		this->button_pressed--;

				    			if (!this->button_pressed)
				    				/* buttonrelease event raised two times for touch pad/screens */
					        		return;

				    			/* buttonrelease event will be buffered */
				    			event_buffered = true;
			    			}
		    			}
		    			break;
		    	case DIET_AXISMOTION:
		    			if (this->pointer_rect.w) {
				       	 	if (evt.axis == DIAI_X) {
				       	 		/* x axis */
					        	if (evt.axisabs < 0) {
					        		/* work with relative value (e.g. normal mouse move) */
									if ((evt.axisrel>1)||(evt.axisrel<-1))
										this->pointer_xpos+= evt.axisrel*2;
									else
										this->pointer_xpos+= evt.axisrel;
					        		if (this->pointer_xpos < this->screen_rect.x)
					        			this->pointer_xpos = this->screen_rect.x;
					        		else
					        		if (this->pointer_xpos > this->screen_rect.x + this->screen_rect.w - 1)
					        			this->pointer_xpos = this->screen_rect.x + this->screen_rect.w - 1;
					        	}
					        	else
					        		/* work with absolute position (e.g. touch pad/screen) */
					        		this->pointer_xpos = this->screen_rect.x
		  			        						 + ((this->xfac * (evt.axisabs - this->pointer_rect.x)) / 100);
				       	 	}
				       	 	else
			        	 	if (evt.axis == DIAI_Y) {
				       	 		/* y axis */
					        	if (evt.axisabs < 0) {
					        		/* work with relative value (e.g. normal mouse move) */
									if ((evt.axisrel>1)||(evt.axisrel<-1))
										this->pointer_ypos+= evt.axisrel*3;
									else
										this->pointer_ypos+= evt.axisrel;
					        		if (this->pointer_ypos < this->screen_rect.y)
					        			this->pointer_ypos = this->screen_rect.y;
					        		else
					        		if (this->pointer_ypos > this->screen_rect.y + this->screen_rect.h - 1)
					        			this->pointer_ypos = this->screen_rect.y + this->screen_rect.h - 1;
					        	}
					        	else
					        		/* work with absolute position (e.g. touch pad/screen) */
					        		this->pointer_ypos = this->screen_rect.y
		  			        						 + ((this->yfac * (evt.axisabs - this->pointer_rect.y)) / 100);
			        	 	}

				       	 	if ((this->pointer_xpos != this->pointer_old_xpos)||(this->pointer_ypos != this->pointer_old_ypos)) {
				       	 		/* the position of the mouse pointer has changed */
					       	 	/* save the old pointer */
					       		this->pointer_old_xpos = this->pointer_xpos;
					       		this->pointer_old_ypos = this->pointer_ypos;

				    			/* fill the return structure */
					        	inputevent->type = MMSINPUTEVENTTYPE_AXISMOTION;
				        		inputevent->posx = this->pointer_xpos;
				        		inputevent->posy = this->pointer_ypos;

				        		/* if we are waiting for the second buttonpress event (touch pad/screen) */
				        		/* --> we cannot return directly */
				        		if (!event_buffered)
				        			return;
				       	 	}
		    			}
		    			break;
                    default:
	        		    break;
		    	}
			}
	    }
	}
}

