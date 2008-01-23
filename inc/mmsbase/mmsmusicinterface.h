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
#ifndef MMSMUSICINTERFACE_H_
#define MMSMUSICINTERFACE_H_

#include "interfaces/immsmusicmanager.h"
#include <sigc++/sigc++.h>

class MMSMusicInterface {
	private:
	
		static IMMSMusicManager *manager;
		void setManager(IMMSMusicManager *manager);
	public:
			MMSMusicInterface(); 
			~MMSMusicInterface(); 
			void play(string file);
			void play(PLAYLIST list, int offset=0);
			void stop();
			void next();
			void prev();
			void pause();
			bool hasPlaylist();
			PLAYLIST getPlaylist();
			int getPlaylistOffset();
	        sigc::signal<void, int> *onNextSong;
	        sigc::signal<void, int> *onPrevSong;
	        bool isPlaying();
            bool getTimes(int *pos, int *length);
            void setRepeat(bool repeat);
            void setShuffle(bool shuffle);
	
	friend class MMSMusicManager;
};

#endif /*MMSMUSICINTERFACE_H_*/
