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
#include "mmscore/mmsmusicmanager.h"

MMSMusicManager::MMSMusicManager() :
    onNextSong(NULL),
    onPrevSong(NULL),
    cont(false),
    repeat(false),
    shuffle(false) {
	/* register myself to the music interface */
	this->interface.setManager(this);
    this->player.onPlaybackFinished->connect(sigc::mem_fun(this, &MMSMusicManager::next));
}

MMSMusicManager::~MMSMusicManager() {
	if(this->player.isPlaying())
		this->player.stop();
}

void MMSMusicManager::setPlayList(PLAYLIST list,int offset) {
	this->mutex.lock();
	this->playlist = list;
	this->offset = offset;
    this->alreadyPlayed.clear();
    for(unsigned int i = 0; i < playlist.size(); i++)
        this->alreadyPlayed.push_back(false);
    
	DEBUGMSG("MMSMusicManager", "got playlist size: %d offset: %d", list.size(), offset);
	string file = this->playlist.at(this->offset);
	if(player.isPlaying())
		player.stop();
	player.play(file,(const bool)cont);
    this->alreadyPlayed.at(this->offset) = true;
	this->mutex.unlock();
}

void MMSMusicManager::stopAll() {
	this->mutex.lock();
	if(player.isPlaying())
		player.stop();
	cont=false;
	this->mutex.unlock();
}

void MMSMusicManager::next() {
	this->mutex.lock();
    if(this->shuffle && (this->playlist.size() > 2)) {
        int newOffset;
        do {
            newOffset = int((double(rand())/RAND_MAX) * (this->playlist.size() - 1));
        }
        while(this->alreadyPlayed.at(newOffset));
        this->offset = newOffset;
    }
    else { 
    	this->offset++;
    	if(this->offset==(int)this->playlist.size()) {
            if(!this->repeat) return;
    		this->offset=0;
    	}
    }
	string file = this->playlist.at(this->offset);

	if(player.isPlaying())
		player.stop();
	player.play((const string)file,(const bool)false);
    this->alreadyPlayed.at(this->offset) = true;
	if(this->onNextSong)
		this->onNextSong->emit(this->offset);

	this->mutex.unlock();
	
}

void MMSMusicManager::prev() {
	this->mutex.lock();
	if(player.isPlaying())
		player.stop();
    if(this->shuffle && (this->playlist.size() > 2)) {
        int newOffset;
        do {
            newOffset = int((double(rand())/RAND_MAX) * (this->playlist.size() - 1));
        }
        while(this->alreadyPlayed.at(newOffset));
        this->offset = newOffset;
    }
    else { 
	    this->offset--;
        if(this->offset < 0)
		  this->offset = this->playlist.size() - 1;
	}
	string file = this->playlist.at(this->offset);
	player.play(file,false);			
    this->alreadyPlayed.at(this->offset) = true;
	if(this->onPrevSong)
		this->onPrevSong->emit(this->offset);
	this->mutex.unlock();
}

void MMSMusicManager::pause() {
	this->mutex.lock();
	//if(player.isPlaying())
		player.pause();
	cont=true;
	this->mutex.unlock();

}

bool MMSMusicManager::hasPlaylist() {
	this->mutex.lock();
	if(this->playlist.size()>0) {
		this->mutex.unlock();
		return true;
	}
	this->mutex.unlock();
	return false;
}

PLAYLIST MMSMusicManager::getPlaylist() {
	return this->playlist;
}

int MMSMusicManager::getPlaylistOffset() {
	return this->offset;

}

bool MMSMusicManager::isPlaying() {
	return player.isPlaying();
}

bool MMSMusicManager::getTimes(int *pos, int *length) {
    return player.getTimes(pos, length);   
}

void MMSMusicManager::setOnNextSong(sigc::signal<void, int> *onNextSong) {
	this->onNextSong = onNextSong;
}

void MMSMusicManager::setOnPrevSong(sigc::signal<void, int> *onPrevSong) {
	this->onPrevSong = onPrevSong;
}

void MMSMusicManager::setRepeat(bool repeat) {
    this->repeat = repeat;
}

void MMSMusicManager::setShuffle(bool shuffle) {
    this->shuffle = shuffle;
}
