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

#ifndef MMSVIDEO_H_
#define MMSVIDEO_H_

#include "mmsmedia/mmsav.h"
#include <queue>

/**
 * @brief   Handles Video playback.
 *
 * @ingroup     mmsmedia
 *
 * @author      Matthias Hardt (MHardt@berlinux-solutions.de)
 *
 * This class is derived from MMSAV and specialized in
 * handling the playback of video files.
 */
class MMSVideo : public MMSAV {
	private:
		std::queue<string>	playlist;	/**< internal playlist */

#ifdef __HAVE_GSTREAMER__
#endif
#ifdef __HAVE_XINE__
        void xineOpen();
#endif

    public:
        MMSVideo(MMSWindow *window, const bool verbose = false);
        ~MMSVideo();

        void startPlaying(const string file, const bool cont = true);
        void add2Playlist(const string file);
        void playNext();

        /**
         * Callback that is used to receive buffering progress changes.
         */
        sigc::signal<void, const unsigned short> onProgressChange;
};

#endif /*MMSVIDEO_H_*/
