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

#ifndef MMSSOUND_H_
#define MMSSOUND_H_

#include "mmsav.h"

/**
 * @brief   Handles any kind of music playback.
 *
 * @ingroup     mmsmedia
 *
 * @author      Matthias Hardt (MHardt@berlinux-solutions.de)
 * @version     1.5.0
 * @date        12/31/2008
 *
 * This class is derived from MMSAV and specialized in
 * handling music playback.
 */
class MMSSound : public MMSAV {
    private:
        void initialize(const bool verbose);
#ifdef __HAVE_GSTREAMER__
#else
        void xineOpen();
#endif

    protected:

    public:

        /* if you specify the source, the constructor will try to open it */
        MMSSound(const bool verbose = false);

        /* destructor */
        ~MMSSound();

        void startPlaying(string mrl, bool cont);
        void ffwd();

        /**
         * callback that will be called if playback finished
         */
        sigc::signal<void> onPlaybackFinished;
};

#endif /*MMSSOUND_H_*/
