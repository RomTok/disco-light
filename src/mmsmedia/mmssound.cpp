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

#include "mmsmedia/mmssound.h"

MMS_CREATEERROR(MMSSoundError);

#ifdef __HAVE_GSTREAMER__
#else
/**
 * Callback, that will be called if xine sends event messages.
 *
 * @param   userData    [in/out]    pointer to the MMSSound object
 * @param   event       [in]        pointer to event structure
 */
static void queue_cb(void *userData, const xine_event_t *event) {

	if(!userData)
		return;

    MMSSound *mmssound = (MMSSound*)userData;

    if(event->type == XINE_EVENT_UI_PLAYBACK_FINISHED)
        mmssound->onPlaybackFinished.emit();
}
#endif

/**
 * Constructor of MMSSound class.
 *
 * @param   verbose [in]    if true the xine engine writes debug messages to stdout
 *
 * @see MMSAV::MMSAV()
 * @see MMSAV::initialize()
 */
MMSSound::MMSSound(const bool verbose) {
    MMSAV::initialize(verbose);
}

/**
 * Destructor of MMSSound class.
 */
MMSSound::~MMSSound() {
    this->onPlaybackFinished.clear();
}

#ifdef __HAVE_GSTREAMER__
#else
/**
 * Calls MMSAV::open() with the queue_cb callback.
 */
void MMSSound::xineOpen() {
    MMSAV::xineOpen(queue_cb);
}
#endif

/**
 * Starts playing.
 *
 * If the continue flag is set it tries to continue
 * at the position where it was stopped before.
 *
 * @param   mrl     MRL to play
 * @param   cont    if true it tries to continue at a position stopped before
 */
void MMSSound::startPlaying(string mrl, bool cont) {
/*    if(!this->stream)
        this->open();*/
    MMSAV::startPlaying(mrl, cont);
}

/**
 * Playback will be switched to fast forward.
 *
 * There are two different speed settings for fast forward.
 * Twice as fast and four times as fast.
 */
void MMSSound::ffwd() {
#ifdef __HAVE_GSTREAMER__
#else
    if(this->status != this->STATUS_NONE) {
        this->setStatus(this->STATUS_FFWD);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_FAST_4);
    }
#endif
}
