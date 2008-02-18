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
#ifndef MMSMUSICMANAGER_H_
#define MMSMUSICMANAGER_H_

#include "mmsbase/mmsmusicinterface.h"
#include "mmsmedia/mmssound.h"
#include "mmstools/mmsmutex.h"

class MMSMusicManager : public IMMSMusicManager {
	private: 
		MMSMusicInterface 		interface;
		MMSMutex 				mutex;
		sigc::signal<void, int> *onNextSong;
		sigc::signal<void, int> *onPrevSong;
		PLAYLIST 				playlist;
		MMSSound 				player;		
		int      				offset;
		bool 					cont, repeat, shuffle;
        vector<bool> 			alreadyPlayed;
	public:
		MMSMusicManager();
		virtual ~MMSMusicManager();
		void setPlayList(PLAYLIST, int offset=0);
		void stopAll();
		void next();
		void prev();
		void pause();
		bool hasPlaylist();
		PLAYLIST getPlaylist();
		int getPlaylistOffset();
        void setOnNextSong(sigc::signal<void, int> *onNextSong);
        void setOnPrevSong(sigc::signal<void, int> *onPrevSong);
        bool isPlaying();
        bool isPaused();
        bool getTimes(int *pos, int *length);
        void setRepeat(bool repeat);
        void setShuffle(bool shuffle);
};

#endif /*MMSMUSICMANAGER_H_*/
