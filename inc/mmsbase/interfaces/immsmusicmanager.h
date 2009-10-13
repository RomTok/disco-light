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

#ifndef IMMSMUSICMANAGER_H_
#define IMMSMUSICMANAGER_H_

#include "mmstools/base.h"
#include <vector>
#include <sigc++/sigc++.h>

typedef vector<string> PLAYLIST;

class IMMSMusicManager {
	public:
	    virtual ~IMMSMusicManager() {};
		virtual void init(PLAYLIST, int offset=0) = 0;
		virtual void stopAll() = 0;
		virtual void next() = 0;
		virtual void prev() = 0;
		virtual void play() = 0;
		virtual void pause() = 0;
		virtual bool hasPlaylist() = 0;
		virtual PLAYLIST getPlaylist() = 0;
		virtual int getPlaylistOffset() = 0;
        virtual void setOnNextSong(sigc::signal<void, int> *onNextSong) = 0;
        virtual void setOnPrevSong(sigc::signal<void, int> *onPrevSong) = 0;
        virtual bool isPlaying() = 0;
        virtual bool isPaused() = 0;
        virtual bool getTimes(int *pos, int *length) = 0;
        virtual void setRepeat(bool repeat) = 0;
        virtual void setShuffle(bool shuffle) = 0;
};

#endif /*IMMSMUSICMANAGER_H_*/
