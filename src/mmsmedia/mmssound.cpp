/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
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

#include "mmsmedia/mmssound.h"

MMS_CREATEERROR(MMSSoundError);

/**
 * Callback, that will be called if xine sends event messages.
 * 
 * @param   userData    [in/out]    pointer to the MMSSound object
 * @param   event       [in]        pointer to event structure
 */
static void queue_cb(void *userData, const xine_event_t *event) {
    MMSSound *mmssound = (MMSSound*)userData;
    
    if(event->type == XINE_EVENT_UI_PLAYBACK_FINISHED)
        mmssound->onPlaybackFinished->emit();
}

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
    this->onPlaybackFinished = new sigc::signal<void>;
}

/**
 * Destructor of MMSSound class.
 */
MMSSound::~MMSSound() {
    if(this->onPlaybackFinished) {
        this->onPlaybackFinished->clear();
        delete this->onPlaybackFinished;
    }
}

/**
 * Calls MMSAV::open() with the queue_cb callback.
 */
void MMSSound::open() {
    MMSAV::open(queue_cb);
}

/**
 * Starts playing.
 *
 * If the continue flag is set it tries to continue
 * at the position where it was stopped before. 
 *
 * @param   mrl     MRL to play 
 * @param   cont    if true it tries to continue at a position stopped before
 */
void MMSSound::play(string mrl, bool cont) {
    if(!this->stream)
        this->open();
    MMSAV::play(mrl, cont);
}

/**
 * Playback will be switched to fast forward.
 * 
 * There are two different speed settings for fast forward.
 * Twice as fast and four times as fast.
 */
void MMSSound::ffwd() {
    if(this->status != this->STATUS_NONE) {
        this->setStatus(this->STATUS_FFWD);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_FAST_4);
    }
}
