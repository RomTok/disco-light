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

#include "mmsinput/mmsinputlishandler.h"
#include "mmsgui/fb/mmsfb.h"
#include <typeinfo>
#include <linux/keyboard.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <linux/vt.h>

MMSInputLISHandler::MMSInputLISHandler(MMS_INPUT_DEVICE device) {
#ifdef __HAVE_FBDEV__
	// init event buffer
	this->ie_read_pos = 0;
	this->ie_write_pos = 0;
	this->shift_pressed = false;
	this->altgr_pressed = false;
	this->is_caps_lock = false;

	// get access to the framebuffer console
	this->kb_fd = -1;
	if (mmsfb->mmsfbdev)
		mmsfb->mmsfbdev->vtGetFd(&this->kb_fd);
#else
	throw new MMSError(0,(string)typeid(this).name() + " is empty. compile FBDEV support!");
#endif
}

MMSInputLISHandler::~MMSInputLISHandler() {
}

MMSKeySymbol MMSInputLISHandler::getSymbol(int code, unsigned short value) {
	unsigned char type  = KTYP(value);
	unsigned char index = KVAL(value);

	switch (type) {
		case KT_FN:
            if (index < 12)
         	   return (MMSKeySymbol)(MMSKEY_F1 + index);
			break;
		case KT_LETTER:
		case KT_LATIN:
			switch (index) {
			case 0x1c: return MMSKEY_PRINT;
			case 0x7f: return MMSKEY_BACKSPACE;
			case 0x08: return MMSKEY_BACKSPACE;
			case 0x09: return MMSKEY_TAB;
			case 0x0d: return MMSKEY_RETURN;
			case 0x18: return MMSKEY_CANCEL;
			case 0x1b: return MMSKEY_ESCAPE;
			case 0x20: return MMSKEY_SPACE;
			case 0x21: return MMSKEY_EXCLAMATION_MARK;
			case 0x22: return MMSKEY_QUOTATION;
			case 0x23: return MMSKEY_NUMBER_SIGN;
			case 0x24: return MMSKEY_DOLLAR_SIGN;
			case 0x25: return MMSKEY_PERCENT_SIGN;
			case 0x26: return MMSKEY_AMPERSAND;
			case 0x27: return MMSKEY_APOSTROPHE;
			case 0x28: return MMSKEY_PARENTHESIS_LEFT;
			case 0x29: return MMSKEY_PARENTHESIS_RIGHT;
			case 0x2a: return MMSKEY_ASTERISK;
			case 0x2b: return MMSKEY_PLUS_SIGN;
			case 0x2c: return MMSKEY_COMMA;
			case 0x2d: return MMSKEY_MINUS_SIGN;
			case 0x2e: return MMSKEY_PERIOD;
			case 0x2f: return MMSKEY_SLASH;
			case 0x30: return MMSKEY_0;
			case 0x31: return MMSKEY_1;
			case 0x32: return MMSKEY_2;
			case 0x33: return MMSKEY_3;
			case 0x34: return MMSKEY_4;
			case 0x35: return MMSKEY_5;
			case 0x36: return MMSKEY_6;
			case 0x37: return MMSKEY_7;
			case 0x38: return MMSKEY_8;
			case 0x39: return MMSKEY_9;
			case 0x3a: return MMSKEY_COLON;
			case 0x3b: return MMSKEY_SEMICOLON;
			case 0x3c: return MMSKEY_LESS_THAN_SIGN;
			case 0x3d: return MMSKEY_EQUALS_SIGN;
			case 0x3e: return MMSKEY_GREATER_THAN_SIGN;
			case 0x3f: return MMSKEY_QUESTION_MARK;
			case 0x40: return MMSKEY_AT;
			case 0x41: return MMSKEY_CAPITAL_A;
			case 0x42: return MMSKEY_CAPITAL_B;
			case 0x43: return MMSKEY_CAPITAL_C;
			case 0x44: return MMSKEY_CAPITAL_D;
			case 0x45: return MMSKEY_CAPITAL_E;
			case 0x46: return MMSKEY_CAPITAL_F;
			case 0x47: return MMSKEY_CAPITAL_G;
			case 0x48: return MMSKEY_CAPITAL_H;
			case 0x49: return MMSKEY_CAPITAL_I;
			case 0x4a: return MMSKEY_CAPITAL_J;
			case 0x4b: return MMSKEY_CAPITAL_K;
			case 0x4c: return MMSKEY_CAPITAL_L;
			case 0x4d: return MMSKEY_CAPITAL_M;
			case 0x4e: return MMSKEY_CAPITAL_N;
			case 0x4f: return MMSKEY_CAPITAL_O;
			case 0x50: return MMSKEY_CAPITAL_P;
			case 0x51: return MMSKEY_CAPITAL_Q;
			case 0x52: return MMSKEY_CAPITAL_R;
			case 0x53: return MMSKEY_CAPITAL_S;
			case 0x54: return MMSKEY_CAPITAL_T;
			case 0x55: return MMSKEY_CAPITAL_U;
			case 0x56: return MMSKEY_CAPITAL_V;
			case 0x57: return MMSKEY_CAPITAL_W;
			case 0x58: return MMSKEY_CAPITAL_X;
			case 0x59: return MMSKEY_CAPITAL_Y;
			case 0x5a: return MMSKEY_CAPITAL_Z;
			case 0x5b: return MMSKEY_SQUARE_BRACKET_LEFT;
			case 0x5c: return MMSKEY_BACKSLASH;
			case 0x5d: return MMSKEY_SQUARE_BRACKET_RIGHT;
			case 0x5e: return MMSKEY_CIRCUMFLEX_ACCENT;
			case 0x5f: return MMSKEY_UNDERSCORE;
			case 0x60: return MMSKEY_GRAVE_ACCENT;
			case 0x61: return MMSKEY_SMALL_A;
			case 0x62: return MMSKEY_SMALL_B;
			case 0x63: return MMSKEY_SMALL_C;
			case 0x64: return MMSKEY_SMALL_D;
			case 0x65: return MMSKEY_SMALL_E;
			case 0x66: return MMSKEY_SMALL_F;
			case 0x67: return MMSKEY_SMALL_G;
			case 0x68: return MMSKEY_SMALL_H;
			case 0x69: return MMSKEY_SMALL_I;
			case 0x6a: return MMSKEY_SMALL_J;
			case 0x6b: return MMSKEY_SMALL_K;
			case 0x6c: return MMSKEY_SMALL_L;
			case 0x6d: return MMSKEY_SMALL_M;
			case 0x6e: return MMSKEY_SMALL_N;
			case 0x6f: return MMSKEY_SMALL_O;
			case 0x70: return MMSKEY_SMALL_P;
			case 0x71: return MMSKEY_SMALL_Q;
			case 0x72: return MMSKEY_SMALL_R;
			case 0x73: return MMSKEY_SMALL_S;
			case 0x74: return MMSKEY_SMALL_T;
			case 0x75: return MMSKEY_SMALL_U;
			case 0x76: return MMSKEY_SMALL_V;
			case 0x77: return MMSKEY_SMALL_W;
			case 0x78: return MMSKEY_SMALL_X;
			case 0x79: return MMSKEY_SMALL_Y;
			case 0x7a: return MMSKEY_SMALL_Z;
			case 0x7b: return MMSKEY_CURLY_BRACKET_LEFT;
			case 0x7c: return MMSKEY_VERTICAL_BAR;
			case 0x7d: return MMSKEY_CURLY_BRACKET_RIGHT;
			case 0x7e: return MMSKEY_TILDE;
			default:   return MMSKEY_UNKNOWN;
			}
			break;
	}

    switch (value) {
         case K_LEFT:    return MMSKEY_CURSOR_LEFT;
         case K_RIGHT:   return MMSKEY_CURSOR_RIGHT;
         case K_UP:      return MMSKEY_CURSOR_UP;
         case K_DOWN:    return MMSKEY_CURSOR_DOWN;
         case K_ENTER:   return MMSKEY_RETURN;
         case K_CTRL:    return MMSKEY_CONTROL;
         case K_SHIFT:   return MMSKEY_SHIFT;
         case K_ALT:     return MMSKEY_ALT;
         case K_ALTGR:   return MMSKEY_ALTGR;
         case K_INSERT:  return MMSKEY_INSERT;
         case K_REMOVE:  return MMSKEY_DELETE;
         case K_FIND:    return MMSKEY_HOME;
         case K_SELECT:  return MMSKEY_END;
         case K_PGUP:    return MMSKEY_PAGE_UP;
         case K_PGDN:    return MMSKEY_PAGE_DOWN;
         case K_NUM:     return MMSKEY_NUM_LOCK;
         case K_HOLD:    return MMSKEY_SCROLL_LOCK;
         case K_PAUSE:   return MMSKEY_PAUSE;
         case K_BREAK:   return MMSKEY_BREAK;
         case K_CAPS:    return MMSKEY_CAPS_LOCK;
         case K_P0:      return MMSKEY_INSERT;
         case K_P1:      return MMSKEY_END;
         case K_P2:      return MMSKEY_CURSOR_DOWN;
         case K_P3:      return MMSKEY_PAGE_DOWN;
         case K_P4:      return MMSKEY_CURSOR_LEFT;
         case K_P5:      return MMSKEY_BEGIN;
         case K_P6:      return MMSKEY_CURSOR_RIGHT;
         case K_P7:      return MMSKEY_HOME;
         case K_P8:      return MMSKEY_CURSOR_UP;
         case K_P9:      return MMSKEY_PAGE_UP;
         case K_PPLUS:   return MMSKEY_PLUS_SIGN;
         case K_PMINUS:  return MMSKEY_MINUS_SIGN;
         case K_PSTAR:   return MMSKEY_ASTERISK;
         case K_PSLASH:  return MMSKEY_SLASH;
         case K_PENTER:  return MMSKEY_RETURN;
         case K_PCOMMA:  return MMSKEY_COMMA;
         case K_PDOT:    return MMSKEY_PERIOD;
         case K_PPARENL: return MMSKEY_PARENTHESIS_LEFT;
         case K_PPARENR: return MMSKEY_PARENTHESIS_RIGHT;
    }

	return MMSKEY_UNKNOWN;
}

unsigned short MMSInputLISHandler::readValue(unsigned char table, unsigned char index) {
	struct kbentry entry;
	entry.kb_table = table;
	entry.kb_index = index;
	entry.kb_value = 0;
	ioctl(this->kb_fd, KDGKBENT, &entry);
	return entry.kb_value;
}

MMSKeySymbol MMSInputLISHandler::getKeyFromCode(bool pressed, unsigned char code) {
    unsigned short value;

	// try with normtab
	value = readValue(K_NORMTAB, code);
	MMSKeySymbol ks = getSymbol(code, value);

	// check special keys
    switch (ks) {
    case MMSKEY_SHIFT:
    	this->shift_pressed = pressed;
    	break;
    case MMSKEY_ALTGR:
    	this->altgr_pressed = pressed;
    	break;
    case MMSKEY_CAPS_LOCK:
    	if (!pressed) {
    		// work only with the key release event
   			this->is_caps_lock = !this->is_caps_lock;
			updateLED();
    	}
    	break;
    default:
        if ((this->shift_pressed)||(this->is_caps_lock)) {
            if (!this->altgr_pressed) {
            	// shift is pressed
            	value = readValue(K_SHIFTTAB, code);
            	ks = getSymbol(code, value);
            }
            else {
            	// shift+altgr is pressed
        		value = readValue(K_ALTSHIFTTAB, code);
        		ks = getSymbol(code, value);
            }
        }
        else
        if (this->altgr_pressed) {
        	// altgr is pressed
    		value = readValue(K_ALTTAB, code);
    		ks = getSymbol(code, value);
        }
    	break;
    }

    return ks;
}

void MMSInputLISHandler::readKeyboardEvents() {
	// set new write pos for the event buffer
	int old_ie_write_pos = this->ie_write_pos;
	this->ie_write_pos++;
	if (this->ie_write_pos >= MMSINPUTLISHANDLER_EVENT_BUFFER_SIZE)
		this->ie_write_pos = 0;
	if (this->ie_read_pos == this->ie_write_pos) {
		// buffer is full
		this->ie_write_pos = old_ie_write_pos;
		return;
	}

	// read next key
	unsigned char key;
	if (read(this->kb_fd, &key, 1) == 1) {
		// new event
		MMSInputEvent *inputevent = &this->ie_buffer[old_ie_write_pos];
		if (key & 0x80) {
			inputevent->type = MMSINPUTEVENTTYPE_KEYRELEASE;
			inputevent->key = getKeyFromCode(false, key & 0x7f);
		}
		else {
			inputevent->type = MMSINPUTEVENTTYPE_KEYPRESS;
			inputevent->key = getKeyFromCode(true, key & 0x7f);
		}
	}
}

void MMSInputLISHandler::updateLED() {
	int locks = 0;
	if (this->is_caps_lock)
		locks |= K_CAPSLOCK;
    ioctl(this->kb_fd, KDSKBLED, locks);
}

void MMSInputLISHandler::grabEvents(MMSInputEvent *inputevent) {
#ifdef __HAVE_FBDEV__
	while (1) {
		// read new events from keyboard to buffer
		readKeyboardEvents();

		// is buffer empty?
		if (this->ie_read_pos == this->ie_write_pos) {
			usleep(10000);
			continue;
		}

		// there is at least one event in the buffer, return next event
		*inputevent = this->ie_buffer[this->ie_read_pos];
		this->ie_read_pos++;
		if (this->ie_read_pos >= MMSINPUTLISHANDLER_EVENT_BUFFER_SIZE)
			this->ie_read_pos = 0;
		return;
	}
#else
	throw new MMSError(0,(string)typeid(this).name() + " is empty. compile FBDEV support!");
#endif
}

