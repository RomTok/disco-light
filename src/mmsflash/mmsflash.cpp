/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>                *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#ifndef _NO_MMSFLASH

#include "mmsflash/mmsflash.h"
#include "mmsflash/mmsflashthread.h"

extern "C" {
#include <swfdec/swfdec.h>
#include <swfdec/swfdec_buffer.h>
}

#include <cairo.h>
#include <cairo-features.h>


// static variables
bool MMSFlash::swfdec_initialized = false;


MMSFlash::MMSFlash(MMSWindow *window) {
	// init the window
	this->window = window;
	this->window->onHandleInput->connect(sigc::mem_fun(this,&MMSFlash::onHandleInput));

	// init all others
	this->ready 				= false;
	this->playing				= false;
	this->swfdec_player 		= NULL;
	this->swfdec_rate   		= 0;
	this->width					= 0;
	this->height				= 0;
	this->flash_temp_surface	= NULL;
	this->loaderthread			= new MMSFlashThread(this, MMSFLASHTHREAD_MODE_LOADER, "MMSFlashLoaderThread");
	this->playerthread			= new MMSFlashThread(this, MMSFLASHTHREAD_MODE_PLAYER, "MMSFlashPlayerThread");

	if (!this->swfdec_initialized) {
		swfdec_init();
		this->swfdec_initialized = true;
	}
}

MMSFlash::~MMSFlash() {
	stopThreads();
	if (this->flash_temp_surface)
		delete this->flash_temp_surface;
	if (this->swfdec_player)
		g_object_unref(this->swfdec_player);
}


void MMSFlash::loader(bool &stop) {

	// lock me
	lock.lock();
	this->ready = false;
	this->playing = false;

    // new player object
	if (this->swfdec_player)
		g_object_unref(this->swfdec_player);
    this->swfdec_player = swfdec_player_new(NULL);
    if (!this->swfdec_player) {
    	lock.unlock();
    	fprintf(stderr, "Cannot get a new SwfdecPlayer object\n");
    	return;
    }

    // set url
	SwfdecURL *url = swfdec_url_new(this->filename.c_str());
    swfdec_player_set_url((SwfdecPlayer*)this->swfdec_player, url);
    swfdec_url_free(url);

    // check if player is initialized
    swfdec_player_advance((SwfdecPlayer*)this->swfdec_player, 0);
    if (!swfdec_player_is_initialized((SwfdecPlayer*)this->swfdec_player)) {
    	g_object_unref(this->swfdec_player);
    	this->swfdec_player = NULL;
    	lock.unlock();
    	fprintf(stderr, "Cannot initialize SwfdecPlayer object\n");
    	return;
    }

    // get frame rate
    this->swfdec_rate = swfdec_player_get_rate((SwfdecPlayer*)this->swfdec_player);

    // get size of the flash image
    guint ww,hh;
    swfdec_player_get_default_size((SwfdecPlayer*)this->swfdec_player, &ww, &hh);
    this->width=ww;
    this->height=hh;

    // (re-)create surface for cairo/swfdec
    if (this->flash_temp_surface)
    	delete flash_temp_surface;
	this->window->getLayer()->createSurface(&(this->flash_temp_surface), this->width, this->height, MMSFB_PF_ARGB, 0);
	if (!this->flash_temp_surface) {
    	g_object_unref(this->swfdec_player);
    	this->swfdec_player = NULL;
    	lock.unlock();
    	fprintf(stderr, "Cannot create temporary surface\n");
    	return;
	}
	void *ptr;
	int pitch;
	this->flash_temp_surface->lock(DSLF_WRITE, &ptr, &pitch);
	this->flash_temp_surface->unlock();
    this->cairosurface = cairo_image_surface_create_for_data((unsigned char*)ptr, CAIRO_FORMAT_ARGB32,
    															this->width, this->height, pitch);
	if (!this->cairosurface) {
		delete this->flash_temp_surface;
		this->flash_temp_surface = NULL;
    	g_object_unref(this->swfdec_player);
    	this->swfdec_player = NULL;
    	lock.unlock();
    	fprintf(stderr, "Cannot create cairo surface\n");
    	return;
	}
    this->cairo = cairo_create((cairo_surface_t *)cairosurface);
    cairo_surface_destroy((cairo_surface_t *)cairosurface);
    if (!this->cairo) {
		delete this->flash_temp_surface;
		this->flash_temp_surface = NULL;
    	g_object_unref(this->swfdec_player);
    	this->swfdec_player = NULL;
    	lock.unlock();
    	fprintf(stderr, "Cannot create cairo surface\n");
    	return;
	}

    // ready for playing
    this->ready = true;

    // unlock me
	lock.unlock();
}

void MMSFlash::player(bool &stop) {
	// waiting for ready state
	this->playing = false;
	while ((!stop)&&(!this->ready)) {
    	msleep(50);
    	if ((!this->ready)&&(!this->loaderthread->isRunning()))
			return;
    }
	this->playing = true;

    // until stopped
    unsigned int sleep_time = (unsigned int)(1000 / this->swfdec_rate);
	while (!stop) {
		if (!this->window->isShown(true)) {
			// window is not shown, so playback is not needed
			msleep(500);
			continue;
		}

		// lock me
		lock.lock();

		// get start time
	    unsigned int start_ts = getMTimeStamp();

	    // get background color of the current frame and clear the surface
	    unsigned int bg = swfdec_player_get_background_color((SwfdecPlayer*)this->swfdec_player);
	    this->flash_temp_surface->clear( (bg >> 16) & 0xff, (bg >> 8) & 0xff, bg & 0xff, bg >> 24 );

	    // let swfdec render to the temporary surface
	    swfdec_player_render((SwfdecPlayer*)this->swfdec_player, (cairo_t *)this->cairo);

		// unlock me
		lock.unlock();

		// do an stretchblit to the window and flip it
	    this->window->getSurface()->stretchBlit(this->flash_temp_surface, NULL, NULL);
	    this->window->flip();

	    // calc sleep time and sleep
	    unsigned int end_ts = getMTimeStamp();
	    end_ts = getMDiff(start_ts, end_ts);
	    if (end_ts < sleep_time)
	    	msleep(sleep_time - end_ts);
	}

	// reset state
	this->playing = false;
}

void MMSFlash::stopThreads(void) {
	// stop it all
	this->loaderthread->invokeStop();
	this->playerthread->invokeStop();
	this->loaderthread->waitUntilStopped();
	this->playerthread->waitUntilStopped();

	// reset state
	this->ready = false;
	this->playing = false;
}

unsigned int MMSFlash::mapKey(DFBInputDeviceKeySymbol key) {
	switch (key) {
	case DIKS_CURSOR_UP:
		return SWFDEC_KEY_UP;
	case DIKS_CURSOR_DOWN:
		return SWFDEC_KEY_DOWN;
	case DIKS_CURSOR_LEFT:
		return SWFDEC_KEY_LEFT;
	case DIKS_CURSOR_RIGHT:
		return SWFDEC_KEY_RIGHT;
	case DIKS_SPACE:
		return SWFDEC_KEY_SPACE;
	case DIKS_BACKSPACE:
		return SWFDEC_KEY_BACKSPACE;
	case DIKS_SLASH:
		return SWFDEC_KEY_SLASH;
	case DIKS_BACKSLASH:
		return SWFDEC_KEY_BACKSLASH;
	case DIKS_TAB:
		return SWFDEC_KEY_TAB;
	case DIKS_CLEAR:
		return SWFDEC_KEY_CLEAR;
	case DIKS_RETURN:
		return SWFDEC_KEY_ENTER;
	case DIKS_SHIFT:
		return SWFDEC_KEY_SHIFT;
	case DIKS_CONTROL:
		return SWFDEC_KEY_CONTROL;
	case DIKS_ALT:
		return SWFDEC_KEY_ALT;
	case DIKS_CAPS_LOCK:
		return SWFDEC_KEY_CAPS_LOCK;
	case DIKS_ESCAPE:
		return SWFDEC_KEY_ESCAPE;
	case DIKS_PAGE_UP:
		return SWFDEC_KEY_PAGE_UP;
	case DIKS_PAGE_DOWN:
		return SWFDEC_KEY_PAGE_DOWN;
	case DIKS_END:
		return SWFDEC_KEY_END;
	case DIKS_HOME:
		return SWFDEC_KEY_HOME;
	case DIKS_INSERT:
		return SWFDEC_KEY_INSERT;
	case DIKS_DELETE:
		return SWFDEC_KEY_DELETE;
	case DIKS_HELP:
		return SWFDEC_KEY_HELP;
	case DIKS_0:
		return SWFDEC_KEY_0;
	case DIKS_1:
		return SWFDEC_KEY_1;
	case DIKS_2:
		return SWFDEC_KEY_2;
	case DIKS_3:
		return SWFDEC_KEY_3;
	case DIKS_4:
		return SWFDEC_KEY_4;
	case DIKS_5:
		return SWFDEC_KEY_5;
	case DIKS_6:
		return SWFDEC_KEY_6;
	case DIKS_7:
		return SWFDEC_KEY_7;
	case DIKS_8:
		return SWFDEC_KEY_8;
	case DIKS_9:
		return SWFDEC_KEY_9;
	case DIKS_CAPITAL_A:
	case DIKS_SMALL_A:
		return SWFDEC_KEY_A;
	case DIKS_CAPITAL_B:
	case DIKS_SMALL_B:
		return SWFDEC_KEY_B;
	case DIKS_CAPITAL_C:
	case DIKS_SMALL_C:
		return SWFDEC_KEY_C;
	case DIKS_CAPITAL_D:
	case DIKS_SMALL_D:
		return SWFDEC_KEY_D;
	case DIKS_CAPITAL_E:
	case DIKS_SMALL_E:
		return SWFDEC_KEY_E;
	case DIKS_CAPITAL_F:
	case DIKS_SMALL_F:
		return SWFDEC_KEY_F;
	case DIKS_CAPITAL_G:
	case DIKS_SMALL_G:
		return SWFDEC_KEY_G;
	case DIKS_CAPITAL_H:
	case DIKS_SMALL_H:
		return SWFDEC_KEY_H;
	case DIKS_CAPITAL_I:
	case DIKS_SMALL_I:
		return SWFDEC_KEY_I;
	case DIKS_CAPITAL_J:
	case DIKS_SMALL_J:
		return SWFDEC_KEY_J;
	case DIKS_CAPITAL_K:
	case DIKS_SMALL_K:
		return SWFDEC_KEY_K;
	case DIKS_CAPITAL_L:
	case DIKS_SMALL_L:
		return SWFDEC_KEY_L;
	case DIKS_CAPITAL_M:
	case DIKS_SMALL_M:
		return SWFDEC_KEY_M;
	case DIKS_CAPITAL_N:
	case DIKS_SMALL_N:
		return SWFDEC_KEY_N;
	case DIKS_CAPITAL_O:
	case DIKS_SMALL_O:
		return SWFDEC_KEY_O;
	case DIKS_CAPITAL_P:
	case DIKS_SMALL_P:
		return SWFDEC_KEY_P;
	case DIKS_CAPITAL_Q:
	case DIKS_SMALL_Q:
		return SWFDEC_KEY_Q;
	case DIKS_CAPITAL_R:
	case DIKS_SMALL_R:
		return SWFDEC_KEY_R;
	case DIKS_CAPITAL_S:
	case DIKS_SMALL_S:
		return SWFDEC_KEY_S;
	case DIKS_CAPITAL_T:
	case DIKS_SMALL_T:
		return SWFDEC_KEY_T;
	case DIKS_CAPITAL_U:
	case DIKS_SMALL_U:
		return SWFDEC_KEY_U;
	case DIKS_CAPITAL_V:
	case DIKS_SMALL_V:
		return SWFDEC_KEY_V;
	case DIKS_CAPITAL_W:
	case DIKS_SMALL_W:
		return SWFDEC_KEY_W;
	case DIKS_CAPITAL_X:
	case DIKS_SMALL_X:
		return SWFDEC_KEY_X;
	case DIKS_CAPITAL_Y:
	case DIKS_SMALL_Y:
		return SWFDEC_KEY_Y;
	case DIKS_CAPITAL_Z:
	case DIKS_SMALL_Z:
		return SWFDEC_KEY_Z;
	case DIKS_F1:
		return SWFDEC_KEY_F1;
	case DIKS_F2:
		return SWFDEC_KEY_F2;
	case DIKS_F3:
		return SWFDEC_KEY_F3;
	case DIKS_F4:
		return SWFDEC_KEY_F4;
	case DIKS_F5:
		return SWFDEC_KEY_F5;
	case DIKS_F6:
		return SWFDEC_KEY_F6;
	case DIKS_F7:
		return SWFDEC_KEY_F7;
	case DIKS_F8:
		return SWFDEC_KEY_F8;
	case DIKS_F9:
		return SWFDEC_KEY_F9;
	case DIKS_F10:
		return SWFDEC_KEY_F10;
	case DIKS_F11:
		return SWFDEC_KEY_F11;
	case DIKS_F12:
		return SWFDEC_KEY_F12;
	case DIKS_NUM_LOCK:
		return SWFDEC_KEY_NUM_LOCK;
	case DIKS_SCROLL_LOCK:
		return SWFDEC_KEY_SCROLL_LOCK;
	case DIKS_SEMICOLON:
		return SWFDEC_KEY_SEMICOLON;
	case DIKS_EQUALS_SIGN:
		return SWFDEC_KEY_EQUAL;
	case DIKS_COMMA:
		return SWFDEC_KEY_COMMA;
	case DIKS_MINUS_SIGN:
		return SWFDEC_KEY_MINUS;
	case DIKS_PERIOD:
		return SWFDEC_KEY_DOT;
	case DIKS_GRAVE_ACCENT:
		return SWFDEC_KEY_GRAVE;
	case DIKS_APOSTROPHE:
		return SWFDEC_KEY_APOSTROPHE;
	}

	return 0;
}

bool MMSFlash::onHandleInput(MMSWindow *window, MMSInputEvent *input) {
	unsigned int 	key;

	// check state
	if (!this->swfdec_player)
		return false;
	if (!input)
		return false;

	// lock me
	lock.lock();

	// calculate the pointer position within flash image
	int posx = input->posx;
	int posy = input->posy;
	switch (input->type) {
	case MMSINPUTEVENTTYPE_BUTTONPRESS:
	case MMSINPUTEVENTTYPE_BUTTONRELEASE:
	case MMSINPUTEVENTTYPE_AXISMOTION: {
		DFBRectangle ig = window->getGeometry();
		if (this->width != ig.w)
			posx = ((posx * (this->width << 8)) / ig.w) >> 8;
		if (this->height != ig.h)
			posy = ((posy * (this->height << 8)) / ig.h) >> 8;
		break;
	}
	}

	// send event to the player
	switch (input->type) {
	case MMSINPUTEVENTTYPE_KEYPRESS:
		if ((key = mapKey(input->key)))
			swfdec_player_key_press((SwfdecPlayer*)this->swfdec_player, key, 0);
		break;
	case MMSINPUTEVENTTYPE_KEYRELEASE:
		if ((key = mapKey(input->key)))
			swfdec_player_key_release((SwfdecPlayer*)this->swfdec_player, key, 0);
		break;
	case MMSINPUTEVENTTYPE_BUTTONPRESS:
		swfdec_player_mouse_press((SwfdecPlayer*)this->swfdec_player, posx, posy, 1);
		break;
	case MMSINPUTEVENTTYPE_BUTTONRELEASE:
		swfdec_player_mouse_release((SwfdecPlayer*)this->swfdec_player, posx, posy, 1);
		break;
	case MMSINPUTEVENTTYPE_AXISMOTION:
		swfdec_player_mouse_move((SwfdecPlayer*)this->swfdec_player, posx, posy);
		break;
	}

    // unlock me
	lock.unlock();

	return true;
}

void MMSFlash::startPlaying(string filename) {
	// check if i have a window
	if (!this->window)
        return;

	// first, stop the threads
	stopThreads();

	// check filename
    string prefix = filename.substr(0, 7);
    strToUpr(&prefix);
    if ((prefix != "FILE://")&&(prefix != "HTTP://"))
    	if (filename.substr(0,1) != "/") {
    		char path[1024];
    		memset(path, 0, sizeof(path));
			this->filename = "file://" + (string)getcwd(path, 1024) + "/" + filename;
    	}
    	else
    		this->filename = "file://" + filename;
    else
    	this->filename = filename;

    // start the loader
    this->loaderthread->start();

    // start the player
    this->playerthread->start();
}

bool MMSFlash::isReady() {
    // waiting until loader thread is started
    while (!this->loaderthread->isStarted())
    	msleep(50);

    // waiting for ready flag
    while ((!this->ready)&&(this->loaderthread->isRunning()))
   		msleep(50);

    return this->ready;
}

bool MMSFlash::isPlaying(bool wait) {
	if (wait)
    	while ((!this->playing)&&(this->playerthread->isRunning()))
        	msleep(50);
	return this->playing;
}


#endif /* _NO_MMSFLASH */
