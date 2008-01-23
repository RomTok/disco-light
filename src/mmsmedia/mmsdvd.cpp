/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
 *      Matthias Hardt   <mattmax@morphine.tv>                             *
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
#include "mmsdvd.h"

MMS_CREATEERROR(MMSDVDError);

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
    logger.writeLog("Using " + this->device + " as DVD device");
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
    logger.setIdentity("MMSDVD");
    MMSAV::initialize(verbose, window);

    /* at first check for DVD device */
    checkDevice(device);
}

/**
 * Destructor of MMSDVD class.
 */
MMSDVD::~MMSDVD() {
}

/**
 * Calls MMSAV::open() with the queue_cb callback.
 */
void MMSDVD::open() {
    MMSAV::open(queue_cb);
}

/**
 * Starts playing.
 *
 * If the continue flag is set it tries to continue
 * at the position where it was stopped before. 
 * 
 * @param   cont    if true it tries to continue at a position stopped before
 */
void MMSDVD::play(const bool cont) {
    MMSAV::play("dvd://" + this->device, cont); 
}

/**
 * Playback will be switched to rewind.
 * 
 * @see MMSDVD::slow()
 * @see MMSDVD::ffwd()
 */
void MMSDVD::rewind() {
    this->logger.writeLog("MMSDVD::rewind() not yet implemented");
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
    this->sendEvent(XINE_EVENT_INPUT_PREVIOUS);
    this->setStatus(this->STATUS_PREVIOUS);
}

/**
 * Jump to next chapter.
 * 
 * @see MMSDVD::previous()
 */
void MMSDVD::next() {
    this->sendEvent(XINE_EVENT_INPUT_NEXT);
    this->setStatus(this->STATUS_NEXT);
}

/**
 * Use the previous available angle.
 * 
 * @see MMSDVD::angleNext()
 */
void MMSDVD::anglePrevious() {
    this->sendEvent(XINE_EVENT_INPUT_ANGLE_PREVIOUS);
    this->setStatus(this->STATUS_ANGLE_PREVIOUS);
}

/**
 * Use the next available angle.
 * 
 * @see MMSDVD::anglePrevious()
 */
void MMSDVD::angleNext() {
    this->sendEvent(XINE_EVENT_INPUT_ANGLE_NEXT);
    this->setStatus(this->STATUS_ANGLE_NEXT);
}

/**
 * Use the previous available subtitle channel.
 * 
 * @see MMSDVD::audioChannelNext()
 */
void MMSDVD::audioChannelPrevious() {
    if(--audioChannel < 0) 
        audioChannel = maxAudioChannels;
    xine_set_param(this->stream, XINE_PARAM_AUDIO_CHANNEL_LOGICAL, audioChannel);
    this->setStatus(this->STATUS_AUDIO_PREVIOUS);
}   

/**
 * Use the next available audio channel.
 * 
 * @see MMSDVD::audioChannelPrevious()
 */
void MMSDVD::audioChannelNext() {
    if(++audioChannel >= maxAudioChannels) 
        audioChannel = 0;
    xine_set_param(this->stream, XINE_PARAM_AUDIO_CHANNEL_LOGICAL, audioChannel);
    this->setStatus(this->STATUS_AUDIO_NEXT);
}   

/**
 * Use the previous available subtitle channel.
 * 
 * @see MMSDVD::spuChannelNext()
 */
void MMSDVD::spuChannelPrevious() {
    if(--spuChannel < -1) 
        spuChannel = maxSpuChannels;
    xine_set_param(this->stream, XINE_PARAM_SPU_CHANNEL, spuChannel);
    this->setStatus(this->STATUS_SPU_PREVIOUS);
}   

/**
 * Use the next available subtitle channel.
 * 
 * @see MMSDVD::spuChannelNext()
 */
void MMSDVD::spuChannelNext() {
    if(++spuChannel >= maxSpuChannels) 
        spuChannel = -1;
    xine_set_param(this->stream, XINE_PARAM_SPU_CHANNEL, spuChannel);
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
    this->setStatus(this->STATUS_EJECT);
    xine_dispose(this->stream);

    // call eject on commandline, because xine_eject() didn't work
    char cmd[64];
    sprintf(cmd, "eject %s", this->device.c_str());
    system(cmd);
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
    this->sendEvent(XINE_EVENT_INPUT_UP);
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
    this->sendEvent(XINE_EVENT_INPUT_DOWN);
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
    this->sendEvent(XINE_EVENT_INPUT_LEFT);
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
    this->sendEvent(XINE_EVENT_INPUT_RIGHT);
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
    this->sendEvent(XINE_EVENT_INPUT_SELECT);
}

/**
 * Jump to dvd main menu.
 *
 * @note This may not be possible if the dvd just 
 * started playing and the menu wasn't shown before.
 */
void MMSDVD::showMainMenu() {
    this->sendEvent(XINE_EVENT_INPUT_MENU1);
}

/**
 * Determines if the dvd menu is currently shown.
 *
 * @note It assumes that title 0 is the dvd root menu, 
 * which may not always be true.
 * 
 * @return  true if in dvd menu
 *
 * @see MMSDVD::getTitleNumber()
 */
bool MMSDVD::inMenu() {
	return (this->getTitleNumber() == 0);
}

/**
 * Gets the name of the dvd.
 * If this is not supported, it will be set to 'UNKNOWN'.
 * 
 * @return  title name
 */
string MMSDVD::getTitle() {
    
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
}

/**
 * Gets the chapter number that is currently being played.
 * 
 * @return  chapter number
 */
int MMSDVD::getChapterNumber() {
    return xine_get_stream_info(this->stream, XINE_STREAM_INFO_DVD_CHAPTER_NUMBER);
}

/**
 * Gets amount of chapters available for the current title.
 * 
 * @return  chapter count
 */
int MMSDVD::getChapterCount() {
    return xine_get_stream_info(this->stream, XINE_STREAM_INFO_DVD_CHAPTER_COUNT);
}

/**
 * Gets the title number that is currently being played.
 * 
 * @return  title number
 */
int MMSDVD::getTitleNumber() {
    return xine_get_stream_info(this->stream, XINE_STREAM_INFO_DVD_TITLE_NUMBER);
}

/**
 * Gets amount of titles available on the current dvd.
 * 
 * @return  title count
 */
int MMSDVD::getTitleCount() {
    return xine_get_stream_info(this->stream, XINE_STREAM_INFO_DVD_TITLE_COUNT);
}

/**
 * Updates internal variables for saving current available
 * audio channels and subtitle channels.
 */
void MMSDVD::updateChannelInfo() {
    this->maxAudioChannels = xine_get_stream_info(this->stream, XINE_STREAM_INFO_MAX_AUDIO_CHANNEL);
    this->maxSpuChannels   = xine_get_stream_info(this->stream, XINE_STREAM_INFO_MAX_SPU_CHANNEL);
}
