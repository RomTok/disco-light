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

#ifdef __HAVE_GSTREAMER__
#else
/**
 * Callback, that will be called if xine sends event messages.
 *
 * It also emits signals that can be handled by sigc++ connectors.
 *
 * @param   userData    [in/out]    pointer to the MMSTV object
 * @param   event       [in]        pointer to event structure
 */
static void queue_cb(void *userData, const xine_event_t *event) {
    MMSTV *mmstv  = static_cast<MMSTV*>(userData);

    if(event->type == XINE_EVENT_UI_MESSAGE) {
        xine_ui_message_data_t *msg = (xine_ui_message_data_t*)event->data;
        DEBUGMSG("MMSTV", "event: %s", (char*)msg + msg->parameters);
    }
    else if(event->type == XINE_EVENT_PROGRESS) {
        xine_progress_data_t *prog = (xine_progress_data_t*)event->data;
        DEBUGMSG("MMSTV", "event: %s (%d%%)", prog->description, prog->percent);
        if(mmstv) mmstv->onProgressChange.emit(prog->percent);
    }
    else if(event->type == XINE_EVENT_UI_PLAYBACK_FINISHED) {
    	DEBUGMSG("MMSTV", "event: signal lost");
    	mmstv->startPlaying(mmstv->getCurrentChannelName());
    } else
    	DEBUGMSG("MMSTV", "event: %u", event->type);
}
#endif

/**
 * Initializes everything that is needed by MMSTV.
 *
 * The timeout attribute is set to 10 seconds.
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
    setTuningTimeout(10);
}

/**
 * Destructor of MMSTV class.
 */
MMSTV::~MMSTV() {
}

#ifdef __HAVE_GSTREAMER__
#else
/**
 * Calls MMSAV::open() with the queue_cb callback.
 */
void MMSTV::xineOpen() {
    MMSAV::xineOpen(queue_cb, this);
}
#endif

/**
 * Starts playing.
 *
 * If usingInputDVBMorphine is set, it tries to use our own
 * input plugin for playback.
 *
 * @param   channel [in]    channel name to be played
 */
void MMSTV::startPlaying(const string channel) {
//    if(!this->stream) this->open();

    if(strncasecmp(channel.c_str(), "OTH:",4)==0) {
    	FILE *fp;
    	fp=fopen(channel.c_str(),"r");
    	if(fp!=NULL){
        	char line[1024];
        	if(!fgets(line,1024,fp))
                throw new MMSTVError(0, "Error reading from file " + channel);
        	this->channel = line;
        	fclose(fp);
        	printf("play %s\n", this->channel.c_str());
            MMSAV::startPlaying(this->channel, false);

    	}

    } else {
		/* first try using our own input plugin */
		if(this->usingInputDVBMorphine) {
			try {
				MMSAV::startPlaying("mmsdvb://" + channel, false);
			}
			catch(MMSError *e) {
				/* now use the xine input plugin */
				this->usingInputDVBMorphine = false;
				DEBUGMSG("MMSMedia", "Error while using Morphine's DVB input plugin [" + e->getMessage() + "]");
				DEBUGMSG("MMSMedia", "Using xine's plugin. Not all features will be available.");
				delete e;
				MMSAV::startPlaying("dvb://" + channel, false);
			}
		}
		else
			MMSAV::startPlaying("dvb://" + channel, false);
		this->channel = channel;
	}
}

/**
 * Continues playing the stream.
 *
 * If recording is on, it will be continued, too.
 */
void MMSTV::play() {
    //TODO: recording
	//this->recordPause();
    MMSAV::play();
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
 * Sets the maximum time for tuning to a channel.
 *
 * @param	timeout	[in]	timeout in seconds
 *
 * @note A value of 0 means infinite. Otherwise a minimum of 5
 * seconds is required.
 *
 * @see MMSTV::getTuningTimeout()
 */
void MMSTV::setTuningTimeout(const unsigned int timeout) {
#ifdef __HAVE_GSTREAMER__
#else
    xine_cfg_entry_t  conf;

    if(!this->xine) return;
    this->timeout = timeout;

    if(xine_config_lookup_entry(this->xine, "media.dvb.tuning_timeout", &conf)) {
        conf.num_value = timeout;
        xine_config_update_entry(this->xine, &conf);
    }
    else
    	xine_config_register_num(this->xine, "media.dvb.tuning_timeout", timeout,
    	                         "Number of seconds until tuning times out.",
    	                         "Leave at 0 means try forever. "
    	                         "Greater than 0 means wait that many seconds to get a lock. Minimum is 5 seconds.",
    	                         XINE_CONFIG_SECURITY, NULL, NULL);
#endif
}

/**
 * Returns the setting for the tuning timeout.
 *
 * @return timeout in seconds
 *
 * @see MMSTV::setTuningTimeout()
 */
const unsigned int MMSTV::getTuningTimeout() {
	return this->timeout;
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
#ifdef __HAVE_GSTREAMER__
#else
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
#endif
}

/**
 * Retrieves the directory where tv recordings will be stored.
 *
 * @return  current recordings dir
 */
const string MMSTV::getRecordDir() {
#ifdef __HAVE_GSTREAMER__
#else
    xine_cfg_entry_t  conf;

    if(this->xine && xine_config_lookup_entry(this->xine, "media.capture.save_dir", &conf))
        return string(conf.str_value);

    return xine_get_homedir();
#endif
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
