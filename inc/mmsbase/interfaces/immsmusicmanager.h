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
#ifndef IMMSMUSICMANAGER_H_
#define IMMSMUSICMANAGER_H_

#include "mmstools/base.h"
#include <vector>
#include <sigc++/sigc++.h>

typedef vector<string> PLAYLIST;

class IMMSMusicManager {
	public: 
	    virtual ~IMMSMusicManager() {};
		virtual void setPlayList(PLAYLIST, int offset=0) = 0;
		virtual void stopAll() = 0;
		virtual void next() = 0;
		virtual void prev() = 0;
		virtual void pause() = 0;
		virtual bool hasPlaylist() = 0;
		virtual PLAYLIST getPlaylist() = 0;
		virtual int getPlaylistOffset() = 0;
        virtual void setOnNextSong(sigc::signal<void, int> *onNextSong) = 0;
        virtual void setOnPrevSong(sigc::signal<void, int> *onPrevSong) = 0;
        virtual bool isPlaying() = 0;
        virtual bool getTimes(int *pos, int *length) = 0;
        virtual void setRepeat(bool repeat) = 0;
        virtual void setShuffle(bool shuffle) = 0;
};

#endif /*IMMSMUSICMANAGER_H_*/
