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
#include "mmsinput/mmsinputx11handler.h"
#include "mmsgui/fb/mmsfb.h"

extern MMSFB *mmsfb;

#ifdef __HAVE_XLIB__
static MMSKeySymbol getKeyFromX11(KeySym xSymbol) {

	switch(xSymbol) {
		case XK_a                : return MMSKEY_SMALL_A;
		case XK_b                : return MMSKEY_SMALL_B;
		case XK_c                : return MMSKEY_SMALL_C;
		case XK_d                : return MMSKEY_SMALL_D;
		case XK_e                : return MMSKEY_SMALL_E;
		case XK_f                : return MMSKEY_SMALL_F;
		case XK_g                : return MMSKEY_SMALL_G;
		case XK_h                : return MMSKEY_SMALL_H;
		case XK_i                : return MMSKEY_SMALL_I;
		case XK_j                : return MMSKEY_SMALL_J;
		case XK_k                : return MMSKEY_SMALL_K;
		case XK_l                : return MMSKEY_SMALL_L;
		case XK_m                : return MMSKEY_SMALL_M;
		case XK_n                : return MMSKEY_SMALL_N;
		case XK_o                : return MMSKEY_SMALL_O;
		case XK_p                : return MMSKEY_SMALL_P;
		case XK_q                : return MMSKEY_SMALL_Q;
		case XK_r                : return MMSKEY_SMALL_R;
		case XK_s                : return MMSKEY_SMALL_S;
		case XK_t                : return MMSKEY_SMALL_T;
		case XK_u                : return MMSKEY_SMALL_U;
		case XK_v                : return MMSKEY_SMALL_V;
		case XK_w                : return MMSKEY_SMALL_W;
		case XK_x                : return MMSKEY_SMALL_X;
		case XK_y                : return MMSKEY_SMALL_Y;
		case XK_z                : return MMSKEY_SMALL_Z;
		case XK_0                : return MMSKEY_0;
		case XK_1                : return MMSKEY_1;
		case XK_2                : return MMSKEY_2;
		case XK_3                : return MMSKEY_3;
		case XK_4                : return MMSKEY_4;
		case XK_5                : return MMSKEY_5;
		case XK_6                : return MMSKEY_6;
		case XK_7                : return MMSKEY_7;
		case XK_8                : return MMSKEY_8;
		case XK_9                : return MMSKEY_9;
		case XK_F1               : return MMSKEY_F1;
		case XK_F2               : return MMSKEY_F2;
		case XK_F3               : return MMSKEY_F3;
		case XK_F4               : return MMSKEY_F4;
		case XK_F5               : return MMSKEY_F5;
		case XK_F6               : return MMSKEY_F6;
		case XK_F7               : return MMSKEY_F7;
		case XK_F8               : return MMSKEY_F8;
		case XK_F9               : return MMSKEY_F9;
		case XK_F10              : return MMSKEY_F10;
		case XK_F11              : return MMSKEY_F11;
		case XK_F12              : return MMSKEY_F12;
		case XK_Caps_Lock        : return MMSKEY_CAPS_LOCK;
		case XK_Num_Lock         : return MMSKEY_NUM_LOCK;
		case XK_Scroll_Lock      : return MMSKEY_SCROLL_LOCK;
		case XK_Escape           : return MMSKEY_ESCAPE;
		case XK_Left             : return MMSKEY_CURSOR_LEFT;
		case XK_Right            : return MMSKEY_CURSOR_RIGHT;
		case XK_Up               : return MMSKEY_CURSOR_UP;
		case XK_Down             : return MMSKEY_CURSOR_DOWN;
		case XK_Tab              : return MMSKEY_TAB;
		case XK_ISO_Left_Tab     : return MMSKEY_TAB;
		case XK_Return           : return MMSKEY_RETURN;
		case XK_space            : return MMSKEY_SPACE;
		case XK_BackSpace        : return MMSKEY_BACKSPACE;
		case XK_Insert           : return MMSKEY_INSERT;
		case XK_Delete           : return MMSKEY_DELETE;
		case XK_Home             : return MMSKEY_HOME;
		case XK_End              : return MMSKEY_END;
		case XK_Page_Up          : return MMSKEY_PAGE_UP;
		case XK_Page_Down        : return MMSKEY_PAGE_DOWN;
		case XK_Print            : return MMSKEY_PRINT;
		case XK_Pause            : return MMSKEY_PAUSE;
		case XK_minus            : return MMSKEY_MINUS_SIGN;
		case XK_equal            : return MMSKEY_EQUALS_SIGN;
		case XK_backslash        : return MMSKEY_BACKSLASH;
		case XK_semicolon        : return MMSKEY_SEMICOLON;
		case XK_comma            : return MMSKEY_COMMA;
		case XK_period           : return MMSKEY_PERIOD;
		case XK_slash            : return MMSKEY_SLASH;
		case XK_Shift_L          : return MMSKEY_SHIFT;
		case XK_Shift_R          : return MMSKEY_SHIFT;
		case XK_Control_L        : return MMSKEY_CONTROL;
		case XK_Control_R        : return MMSKEY_CONTROL;
		case XK_Alt_L            : return MMSKEY_ALT;
		case XK_Alt_R            : return MMSKEY_ALT;
		case XK_Meta_L           : return MMSKEY_META;
		case XK_Meta_R           : return MMSKEY_META;
		case XK_Super_L          : return MMSKEY_SUPER;
		case XK_Super_R          : return MMSKEY_SUPER;
		case XK_Hyper_L          : return MMSKEY_HYPER;
		case XK_Hyper_R          : return MMSKEY_HYPER;
		case XK_Mode_switch      : return MMSKEY_ALTGR;
		case XK_KP_Divide        : return MMSKEY_SLASH;
		case XK_KP_Multiply      : return MMSKEY_ASTERISK;
		case XK_KP_Subtract      : return MMSKEY_MINUS_SIGN;
		case XK_KP_Add           : return MMSKEY_PLUS_SIGN;
		case XK_KP_Enter         : return MMSKEY_RETURN;
		case XK_KP_Space         : return MMSKEY_SPACE;
		case XK_KP_Tab           : return MMSKEY_TAB;
		case XK_KP_F1            : return MMSKEY_F1;
		case XK_KP_F2            : return MMSKEY_F2;
		case XK_KP_F3            : return MMSKEY_F3;
		case XK_KP_F4            : return MMSKEY_F4;
		case XK_KP_Equal         : return MMSKEY_EQUALS_SIGN;
		case XK_KP_Separator     : return MMSKEY_COLON;
		case XK_KP_Delete        : return MMSKEY_DELETE;
		case XK_KP_Insert        : return MMSKEY_INSERT;
		case XK_KP_End           : return MMSKEY_END;
		case XK_KP_Down          : return MMSKEY_CURSOR_DOWN;
		case XK_KP_Page_Down     : return MMSKEY_PAGE_DOWN;
		case XK_KP_Left          : return MMSKEY_CURSOR_LEFT;
		case XK_KP_Begin         : return MMSKEY_BEGIN;
		case XK_KP_Right         : return MMSKEY_CURSOR_RIGHT;
		case XK_KP_Home          : return MMSKEY_HOME;
		case XK_KP_Up            : return MMSKEY_CURSOR_UP;
		case XK_KP_Page_Up       : return MMSKEY_PAGE_UP;
		case XK_KP_Decimal       : return MMSKEY_PERIOD;
		case XK_KP_0             : return MMSKEY_0;
		case XK_KP_1             : return MMSKEY_1;
		case XK_KP_2             : return MMSKEY_2;
		case XK_KP_3             : return MMSKEY_3;
		case XK_KP_4             : return MMSKEY_4;
		case XK_KP_5             : return MMSKEY_5;
		case XK_KP_6             : return MMSKEY_6;
		case XK_KP_7             : return MMSKEY_7;
		case XK_KP_8             : return MMSKEY_8;
		case XK_KP_9             : return MMSKEY_9;



		default:	return MMSKEY_UNKNOWN;
	}
}
#endif

MMSInputX11Handler::MMSInputX11Handler(MMS_INPUT_DEVICE device) {
#ifdef __HAVE_XLIB__
	this->window = *((Window*) mmsfb->getX11Window());
	this->display = (Display *)mmsfb->getX11Display();
	printf("\nwindow %d, %x\n",window, display);


#else
	throw new MMSError(0,typeid(this).name() + " is empty. compile X11 support!");
#endif
}

MMSInputX11Handler::~MMSInputX11Handler() {
#ifdef __HAVE_XLIB__
#endif
}

void MMSInputX11Handler::grabEvents(MMSInputEvent *inputevent) {
#ifdef __HAVE_XLIB__
    XEvent event;
    while(1) {
    	//
    	XNextEvent(this->display,  &event);
    	if(event.type==KeyPress) {
    		inputevent->type = MMSINPUTEVENTTYPE_KEYPRESS;
    		KeySym xSymbol = XKeycodeToKeysym(this->display , event.xkey.keycode,0 );
    		inputevent->key = getKeyFromX11(xSymbol);
    		return;
    	}
    	if(event.type==KeyRelease) {
    		inputevent->type = MMSINPUTEVENTTYPE_KEYRELEASE;
    		KeySym xSymbol = XKeycodeToKeysym(this->display , event.xkey.keycode,0 );
    		inputevent->key = getKeyFromX11(xSymbol);
    		return;
    	}

    }

#else
	throw new MMSError(0,typeid(this).name() + " is empty. compile X11 support!");
#endif

}


