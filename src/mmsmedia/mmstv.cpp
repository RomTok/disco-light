/***************************************************************************
 *   Copyright (C) 2005-2008 by Berlinux Solutions                         *
 *                                                                         *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>                *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>                    *
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

#include "mmsmedia/mmstv.h"
#include <xine/xineutils.h>

#ifdef __HAVE_INPUT_DVB_MORPHINE__
#include "input_dvb_morphine.h"

#define MMSTV_RECORD_START  (this->usingInputDVBMorphine ? XINE_EVENT_MORPHINE_RECORD_START : XINE_EVENT_INPUT_MENU2)
#define MMSTV_RECORD_STOP   (this->usingInputDVBMorphine ? XINE_EVENT_MORPHINE_RECORD_STOP  : XINE_EVENT_INPUT_MENU2)
#define MMSTV_RECORD_PAUSE  (this->usingInputDVBMorphine ? XINE_EVENT_MORPHINE_RECORD_PAUSE : XINE_EVENT_INPUT_MENU4)

#else

#define MMSTV_RECORD_START  XINE_EVENT_INPUT_MENU2
#define MMSTV_RECORD_STOP   XINE_EVENT_INPUT_MENU2
#define MMSTV_RECORD_PAUSE  XINE_EVENT_INPUT_MENU4

#endif

MMS_CREATEERROR(MMSTVError);

/**
 * Callback, that will be called if xine sends event messages.
 * 
 * It also emits signals that can be handled by sigc++ connectors.
 * 
 * @param   userData    [in/out]    pointer to the MMSTV object
 * @param   event       [in]        pointer to event structure
 */
static void queue_cb(void *userData, const xine_event_t *event) {
    //MMSTV                  *mmstv  = (MMSTV*)userData;
    xine_ui_message_data_t *msg    = (xine_ui_message_data_t*)event->data;

    if(event->type == XINE_EVENT_UI_MESSAGE)
        printf("event: %s\n", (char*)msg + msg->parameters);
}

/**
 * Initializes everything that is needed by MMSTV.
 *
 * @param   window      [in]    main window for dvd playing
 * @param   _channel    [in]    channel to open
 * @param   verbose     [in]    if true the xine engine writes debug messages to stdout
 *  
 * @see MMSAV::MMSAV()
 * @see MMSAV::initialize()
 */
MMSTV::MMSTV(MMSWindow *window, const string _channel, const bool verbose) :
            channel(_channel),
            captureFilename(""),
            recording(false),
#ifdef __HAVE_INPUT_DVB_MORPHINE__
            usingInputDVBMorphine(true) {
#else
            usingInputDVBMorphine(false) {
#endif
    MMSAV::initialize(verbose, window);
}

/**
 * Destructor of MMSTV class.
 */ 
MMSTV::~MMSTV() {
}

/**
 * Calls MMSAV::open() with the queue_cb callback.
 */
void MMSTV::open() {
    MMSAV::open(queue_cb);
}

/**
 * Starts playing.
 *
 * If usingInputDVBMorphine is set, it tries to use our own
 * input plugin for playback.
 * 
 * @param   channel [in]    channel name to be played
 */
void MMSTV::play(const string channel) {
    if(!this->stream) this->open();

    /* first try using our own input plugin */
    if(this->usingInputDVBMorphine) {
        try {
            MMSAV::play("mmsdvb://" + channel, false);
        }
        catch(MMSError *e) {
            /* now use the xine input plugin */
            this->usingInputDVBMorphine = false;
            DEBUGMSG("MMSMedia", "Error while using Morphine's DVB input plugin [" + e->getMessage() + "]");
            DEBUGMSG("MMSMedia", "Using xine's plugin. Not all features will be available.");
            delete e;
            MMSAV::play("dvb://" + channel, false);
        }
    }
    else
        MMSAV::play("dvb://" + channel, false);
    
    this->channel = channel;
}

/**
 * Pauses the stream.
 * 
 * If recording is on, it will be paused, too.
 */
void MMSTV::pause() {
    this->recordPause();
    MMSAV::pause();
}

/**
 * Switch to previous channel.
 * 
 * @note Using ~/.xine/channels.conf and not the Morphine database.
 * 
 * @see MMSTV::next()
 */
void MMSTV::previous() {
    this->sendEvent(XINE_EVENT_INPUT_PREVIOUS);
}

/**
 * Switch to next channel.
 * 
 * @note Using ~/.xine/channels.conf and not the Morphine database.
 *
 * @see MMSTV::previous()
 */
void MMSTV::next() {
    this->sendEvent(XINE_EVENT_INPUT_NEXT);
}

/**
 * Start/Stop recording.
 */
void MMSTV::record() {
    if(this->recording)
        this->sendEvent(MMSTV_RECORD_STOP);
    else
        this->sendEvent(MMSTV_RECORD_START);
    this->recording = !this->recording;
}

/**
 * Start recording.
 */
void MMSTV::recordStart() {
    if(!this->recording) {
        this->sendEvent(MMSTV_RECORD_START);
        this->recording = true;
    }
}

/**
 * Stop recording.
 */
void MMSTV::recordStop() {
    if(this->recording) {
        this->sendEvent(MMSTV_RECORD_STOP);
        this->recording = false;
    }
}

/**
 * Pause recording.
 */
void MMSTV::recordPause() {
    if(this->recording)
        this->sendEvent(MMSTV_RECORD_PAUSE);
}

/**
 * Retrieve the current channel name.
 * 
 * @return  current channel name
 */
string MMSTV::getCurrentChannelName(void) {
    return this->channel;
}

/**
 * Sets the directory where tv recordings will be stored.
 * 
 * It actually changes the value of the xine config option
 * "media.capture.save_dir".
 * 
 * @param   dir [in]    directory to save recordings into
 */
void MMSTV::setRecordDir(const string dir) {
    xine_cfg_entry_t  conf;

    if(!this->xine) return;

    if(xine_config_lookup_entry(this->xine, "media.capture.save_dir", &conf)) {
        conf.str_value = strdup(dir.c_str());
        xine_config_update_entry(this->xine, &conf);
    }
    else
#ifdef XINE_CONFIG_STRING_IS_DIRECTORY_NAME
        xine_config_register_filename(this->xine, 
                "media.capture.save_dir", 
                dir.c_str(), 
                XINE_CONFIG_STRING_IS_DIRECTORY_NAME, 
#else
        xine_config_register_string(this->xine, 
                "media.capture.save_dir", 
                dir.c_str(), 
#endif
                "directory for saving streams", 
                NULL, 
                XINE_CONFIG_SECURITY, 
                NULL , 
                NULL);
}

/**
 * Retrieves the directory where tv recordings will be stored.
 * 
 * @return  current recordings dir
 */
const string MMSTV::getRecordDir() {
    xine_cfg_entry_t  conf;

    if(this->xine && xine_config_lookup_entry(this->xine, "media.capture.save_dir", &conf))
        return string(conf.str_value);

    return xine_get_homedir();
}

/**
 * Retrieves the filename of the current recording if
 * recording is on.
 * 
 * @return  current recording filename or "" if nothing is recorded
 */
const string MMSTV::getRecordFilename() {
    if(this->recording)
        return this->captureFilename;

    return "";
}
