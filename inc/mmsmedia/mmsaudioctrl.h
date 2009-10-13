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
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
 ***************************************************************************/

#ifdef __HAVE_MIXER__
#ifndef MMSAUDIOCTRL_H_
#define MMSAUDIOCTRL_H_

#include <alsa/asoundlib.h>

#include "mmstools/mmstools.h"

MMS_CREATEERROR(MMSAudioCtrlError);

/**
 * @brief   Controls the audio device.
 *
 * @ingroup     mmsmedia
 *
 * @author      Jens Schneider (pupeider@morphine.tv)
 * @version     1.5.0
 * @date        12/31/2008
 */
class MMSAudioCtrl {
    private:
        static snd_mixer_t      *handle;    /**< alsa handle to mixer           */
        static snd_mixer_elem_t *elem;      /**< alsa mixer element handle      */
        static string           card;       /**< card id                        */
        static int              volume;     /**< volume (in percent)            */
        static long             xval;       /**< re-calculated volume for card  */
        static bool             muteFlag;   /**< if true sound is muted         */
        static long             pmin, pmax; /**< range (not in percent)         */
        static string           channel;    /**< current audio channel          */

    public:
        /* constructor */
        MMSAudioCtrl(string channel="");

        /* destructor */
        ~MMSAudioCtrl();

        /* set volume (count in percent (0-100)) */
        void setVolume(int count);

        /* get volume */
        int getVolume(bool dfCard = false);

        /* change volume (count in percent (0-100)) */
        void volumeUp(int count = 10);

        /* change volume (count in percent (0-100)) */
        void volumeDown(int count = 10);

        /* check if is already mute */
        bool isMute();

        /* toggle mute on/off */
        void mute();
};

#endif /*MMSAUDIOCTRL_H_*/
#endif /* __HAVE_MIXER__ */
