/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
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

#include <fstream>
#include <stdlib.h>
#include "mmsmedia/mmsdvd.h"

MMS_CREATEERROR(MMSDVDError);

#ifdef __HAVE_GSTREAMER__
#endif
#ifdef __HAVE_XINE__
/**
 * Callback, that will be called if xine sends event messages.
 *
 * At the moment it just handles the error message if an encrypted
 * dvd cannot be played and it updates the channel information.
 *
 * It also emits signals that can be handled by sigc++ connectors.
 *
 * @param   userData    [in/out]    pointer to the MMSDVD object
 * @param   event       [in]        pointer to event structure
 */
static void queue_cb(void *userData, const xine_event_t *event) {
	MMSDVD                 *mmsdvd = (MMSDVD*)userData;
    xine_ui_message_data_t *msg    = (xine_ui_message_data_t*)event->data;

	switch(event->type) {
        case XINE_EVENT_UI_MESSAGE:
			if(msg->type == XINE_MSG_ENCRYPTED_SOURCE)
			    mmsdvd->onError->emit(string("We don't support encrypted DVDs!"));
			else if(msg->explanation)
			    mmsdvd->onError->emit(string((char*)msg + msg->parameters));
    	    break;
        case XINE_EVENT_UI_CHANNELS_CHANGED:
            mmsdvd->updateChannelInfo();
            break;
    }
}
#endif

/**
 * Check for DVD device.
 *
 * It uses xine health check to implement this.
 * First it will check for a given device. If it fails
 * it also checks for '/dev/dvd'.
 *
 * @param   device  [in]    device to check as dvd device
 *
 * @exception   MMSDVDError no usable dvd device found
 */
void MMSDVD::checkDevice(const string device) {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
		string                  d = device;
		xine_health_check_t hc, *result;

		if(d.length() == 0)
			d = "/dev/dvd";

		hc.dvd_dev = xine_config_register_string(xine, "input.dvd_device", d.c_str(), "device used as dvd drive", NULL, 0, NULL, NULL);
		result = xine_health_check(&hc, CHECK_DVDROM);
		if(result->status != XINE_HEALTH_CHECK_OK) {
			if(d != "dev/dvd") {
				hc.dvd_dev = xine_config_register_string(xine, "input.dvd_device", "/dev/dvd", "device used as dvd drive", NULL, 0, NULL, NULL);
				result = xine_health_check(&hc, CHECK_DVDROM);
				if(result->status != XINE_HEALTH_CHECK_OK)
					throw new MMSDVDError(0, "No DVD Device found at " + device + " and /dev/dvd");
			}
			else
				throw new MMSDVDError(0, "No DVD Device found at /dev/dvd");
		}

		this->device = d;
		DEBUGMSG("MMSMedia", "Using " + this->device + " as DVD device");
#endif
    }
}

/**
 * Constructor of MMSDVD class.
 *
 * It does initializing by calling MMSAV::initialize()
 * and checks the given device. If the device is not correct
 * "/dev/dvd" will be used.
 *
 * @param   window  [in]    main window for dvd playing
 * @param   device  [in]    device to check as dvd device
 * @param   verbose [in]    if true the xine engine writes debug messages to stdout
 */
MMSDVD::MMSDVD(MMSWindow *window, const string device, const bool verbose) :
            audioChannel(0),
            spuChannel(0),
            maxAudioChannels(0),
            maxSpuChannels(0) {
    MMSAV::initialize(verbose, window);

    /* at first check for DVD device */
    checkDevice(device);

    /* save window width/height */
    MMSFBRectangle rect = window->getGeometry();
    this->windowWidth  = rect.w;
    this->windowHeight = rect.h;
}

/**
 * Destructor of MMSDVD class.
 */
MMSDVD::~MMSDVD() {
}

#ifdef __HAVE_GSTREAMER__
#else
/**
 * Calls MMSAV::open() with the queue_cb callback.
 */
void MMSDVD::xineOpen() {
    MMSAV::xineOpen(queue_cb, (void*)this);
}
#endif

/**
 * Starts playing.
 *
 * If the continue flag is set it tries to continue
 * at the position where it was stopped before.
 *
 * @param   cont    if true it tries to continue at a position stopped before
 */
void MMSDVD::startPlaying(const bool cont) {
	string mrl = "dvd://" + this->device;

	/* play root title if not continuing (otherwise */
	/* xine uses last played chapter                */
	if(!cont) mrl += "/0";
    MMSAV::startPlaying(mrl, cont);
}

/**
 * Playback will be switched to rewind.
 *
 * @see MMSDVD::slow()
 * @see MMSDVD::ffwd()
 */
void MMSDVD::rewind() {
    DEBUGMSG("MMSMedia", "MMSDVD::rewind() not yet implemented");
#if 0
    if(this->status != this->STATUS_NONE) {
        this->setStatus(this->STATUS_REWIND);
        xine_trick_mode (this->stream, XINE_TRICK_MODE_FAST_REWIND, 1);
    }
#endif
}

/**
 * Jump to previous chapter.
 *
 * @see MMSDVD::next()
 */
void MMSDVD::previous() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->sendEvent(XINE_EVENT_INPUT_PREVIOUS);
#endif
    }
    this->setStatus(this->STATUS_PREVIOUS);
}

/**
 * Jump to next chapter.
 *
 * @see MMSDVD::previous()
 */
void MMSDVD::next() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->sendEvent(XINE_EVENT_INPUT_NEXT);
#endif
    }
    this->setStatus(this->STATUS_NEXT);
}

/**
 * Use the previous available angle.
 *
 * @see MMSDVD::angleNext()
 */
void MMSDVD::anglePrevious() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->sendEvent(XINE_EVENT_INPUT_ANGLE_PREVIOUS);
#endif
    }
    this->setStatus(this->STATUS_ANGLE_PREVIOUS);
}

/**
 * Use the next available angle.
 *
 * @see MMSDVD::anglePrevious()
 */
void MMSDVD::angleNext() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->sendEvent(XINE_EVENT_INPUT_ANGLE_NEXT);
#endif
    }
    this->setStatus(this->STATUS_ANGLE_NEXT);
}

/**
 * Use the previous available subtitle channel.
 *
 * @see MMSDVD::audioChannelNext()
 */
void MMSDVD::audioChannelPrevious() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
		if(--audioChannel < 0)
			audioChannel = maxAudioChannels;
		xine_set_param(this->stream, XINE_PARAM_AUDIO_CHANNEL_LOGICAL, audioChannel);
#endif
    }
    this->setStatus(this->STATUS_AUDIO_PREVIOUS);
}

/**
 * Use the next available audio channel.
 *
 * @see MMSDVD::audioChannelPrevious()
 */
void MMSDVD::audioChannelNext() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
		if(++audioChannel >= maxAudioChannels)
			audioChannel = 0;
		xine_set_param(this->stream, XINE_PARAM_AUDIO_CHANNEL_LOGICAL, audioChannel);
#endif
    }
    this->setStatus(this->STATUS_AUDIO_NEXT);
}

/**
 * Use the previous available subtitle channel.
 *
 * @see MMSDVD::spuChannelNext()
 */
void MMSDVD::spuChannelPrevious() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
		if(--spuChannel < -1)
			spuChannel = maxSpuChannels;
		xine_set_param(this->stream, XINE_PARAM_SPU_CHANNEL, spuChannel);
#endif
    }
    this->setStatus(this->STATUS_SPU_PREVIOUS);
}

/**
 * Use the next available subtitle channel.
 *
 * @see MMSDVD::spuChannelNext()
 */
void MMSDVD::spuChannelNext() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
		if(++spuChannel >= maxSpuChannels)
			spuChannel = -1;
		xine_set_param(this->stream, XINE_PARAM_SPU_CHANNEL, spuChannel);
#endif
    }
    this->setStatus(this->STATUS_SPU_NEXT);
}

/**
 * Eject the dvd.
 *
 * It disposes the xine stream and sends a shell
 * command to eject the dvd.
 *
 * @todo    use libhal for ejecting
 */
void MMSDVD::eject() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
		this->setStatus(this->STATUS_EJECT);
		xine_dispose(this->stream);

		// call eject on commandline, because xine_eject() didn't work
		char cmd[64];
		sprintf(cmd, "eject %s", this->device.c_str());
		system(cmd);
#endif
    }
}

/**
 * The same as pressing 'Up' in a dvd menu.
 *
 * @see MMSDVD::menuDown()
 * @see MMSDVD::menuLeft()
 * @see MMSDVD::menuRight()
 * @see MMSDVD::menuSelect()
 */
void MMSDVD::menuUp() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->sendEvent(XINE_EVENT_INPUT_UP);
#endif
    }
}

/**
 * The same as pressing 'Down' in a dvd menu.
 *
 * @see MMSDVD::menuUp()
 * @see MMSDVD::menuLeft()
 * @see MMSDVD::menuRight()
 * @see MMSDVD::menuSelect()
 */
void MMSDVD::menuDown() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->sendEvent(XINE_EVENT_INPUT_DOWN);
#endif
    }
}

/**
 * The same as pressing 'Left' in a dvd menu.
 *
 * @see MMSDVD::menuUp()
 * @see MMSDVD::menuDown()
 * @see MMSDVD::menuRight()
 * @see MMSDVD::menuSelect()
 */
void MMSDVD::menuLeft() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->sendEvent(XINE_EVENT_INPUT_LEFT);
#endif
    }
}

/**
 * The same as pressing 'Right' in a dvd menu.
 *
 * @see MMSDVD::menuUp()
 * @see MMSDVD::menuDown()
 * @see MMSDVD::menuLeft()
 * @see MMSDVD::menuSelect()
 */
void MMSDVD::menuRight() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->sendEvent(XINE_EVENT_INPUT_RIGHT);
#endif
    }
}

/**
 * The same as pressing 'Select' in a dvd menu.
 *
 * @see MMSDVD::menuUp()
 * @see MMSDVD::menuDown()
 * @see MMSDVD::menuLeft()
 * @see MMSDVD::menuRight()
 */
void MMSDVD::menuSelect() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->sendEvent(XINE_EVENT_INPUT_SELECT);
#endif
    }
}

/**
 * Jump to dvd main menu.
 *
 * @note This may not be possible if the dvd just
 * started playing and the menu wasn't shown before.
 */
void MMSDVD::showMainMenu() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->sendEvent(XINE_EVENT_INPUT_MENU1);
#endif
    }
}

/**
 * Send mouse-event.
 *
 * @note This is for internal use only.
 *
 * @param	event	[in]	which xine input event to send
 * @param	x		[in]	x coordinate
 * @param	y		[in]	y coordinate
 *
 * @see MMSDVD::mouseButton()
 * @see MMSDVD::mouseMove()
 */
void MMSDVD::mouseEvent(const unsigned int event, const unsigned int x, const unsigned int y) const {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
		xine_event_t		e;
		xine_input_data_t	eData;

		int streamW = xine_get_stream_info(this->stream, XINE_STREAM_INFO_VIDEO_WIDTH);
		int streamH = xine_get_stream_info(this->stream, XINE_STREAM_INFO_VIDEO_HEIGHT);

		e.type 			= event;
		e.data 			= &eData;
		e.data_length 	= sizeof(xine_input_data_t);
		eData.button 	= 1;
		eData.x 		= (int)((float)x / this->windowWidth * streamW);
		eData.y 		= (int)((float)y / this->windowHeight * streamH);
		xine_event_send(this->stream, &e);
#endif
    }
}

/**
 * Send mouse-button-event.
 *
 * @param	x	[in]	x coordinate
 * @param	y	[in]	y coordinate
 *
 * @see MMSDVD::mouseMove()
 */
void MMSDVD::mouseButton(const unsigned int x, const unsigned int y) const {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->mouseEvent(XINE_EVENT_INPUT_MOUSE_BUTTON, x, y);
#endif
    }
}

/**
 * Send mouse-move-event.
 *
 * @param	x	[in]	x coordinate
 * @param	y	[in]	y coordinate
 *
 * @see MMSDVD::mouseButton()
 */
void MMSDVD::mouseMove(const unsigned int x, const unsigned int y) const {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	this->mouseEvent(XINE_EVENT_INPUT_MOUSE_MOVE, x, y);
#endif
    }
}

/**
 * Determines if the dvd menu is currently shown.
 *
 * @note It assumes that menus don't have chapters.
 *
 * @return  true if in dvd menu
 *
 * @see MMSDVD::getChapterNumber()
 */
bool MMSDVD::inMenu() {
	return (this->getChapterNumber() == 0);
}

/**
 * Gets the name of the dvd.
 * If this is not supported, it will be set to 'UNKNOWN'.
 *
 * @return  title name
 */
string MMSDVD::getTitle() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
    	return "";
#endif
    }
    else {
#ifdef __HAVE_XINE__
    if(this->status > STATUS_NONE) {
        char *title;
        title = (char*)xine_get_meta_info(this->stream, XINE_META_INFO_TITLE);
        if(title)
            return string(title);
    }
	else {
        char buf[32841];
        ifstream fstr(this->device.c_str(), ios::in | ios::binary);
        if(fstr) {
            if(fstr.read(buf, sizeof(buf) - 1)) {
                buf[sizeof(buf) - 1] = 0;
                return string(buf + 32808);
            }
        }
    }

    return "";
#endif
    }
}

/**
 * Gets the chapter number that is currently being played.
 *
 * @return  chapter number
 */
int MMSDVD::getChapterNumber() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
    	return 0;
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	return xine_get_stream_info(this->stream, XINE_STREAM_INFO_DVD_CHAPTER_NUMBER);
#endif
    }
}

/**
 * Gets amount of chapters available for the current title.
 *
 * @return  chapter count
 */
int MMSDVD::getChapterCount() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
    	return 0;
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	return xine_get_stream_info(this->stream, XINE_STREAM_INFO_DVD_CHAPTER_COUNT);
#endif
    }
}

/**
 * Gets the title number that is currently being played.
 *
 * @return  title number
 */
int MMSDVD::getTitleNumber() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
    	return 0;
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	return xine_get_stream_info(this->stream, XINE_STREAM_INFO_DVD_TITLE_NUMBER);
#endif
    }
}

/**
 * Gets amount of titles available on the current dvd.
 *
 * @return  title count
 */
int MMSDVD::getTitleCount() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
    	return 0;
#endif
    }
    else {
#ifdef __HAVE_XINE__
    	return xine_get_stream_info(this->stream, XINE_STREAM_INFO_DVD_TITLE_COUNT);
#endif
    }
}

/**
 * Updates internal variables for saving current available
 * audio channels and subtitle channels.
 */
void MMSDVD::updateChannelInfo() {
    if (this->backend == MMSMEDIA_BE_GST) {
#ifdef __HAVE_GSTREAMER__
#endif
    }
    else {
#ifdef __HAVE_XINE__
		this->maxAudioChannels = xine_get_stream_info(this->stream, XINE_STREAM_INFO_MAX_AUDIO_CHANNEL);
		this->maxSpuChannels   = xine_get_stream_info(this->stream, XINE_STREAM_INFO_MAX_SPU_CHANNEL);
#endif
    }
}
