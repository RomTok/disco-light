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

#ifndef MMSSOUND_H_
#define MMSSOUND_H_

#include "mmsav.h"

/**
 * @brief   Handles any kind of music playback.
 * 
 * @ingroup     mmsmedia mmslibs
 *
 * @author      Matthias Hardt (mattmax@morphine.tv)
 * @version     1.0.3
 * @date        11/07/2007
 * 
 * This class is derived from MMSAV and specialized in
 * handling music playback.
 */
class MMSSound : public MMSAV {
    private:
        void initialize(const bool verbose);

    protected:
    
    public:
    
        /* if you specify the source, the constructor will try to open it */
        MMSSound(const bool verbose = false);

        /* destructor */
        ~MMSSound();
        
        void open();
        void startPlaying(string mrl, bool cont);
        void ffwd();
        
        /**
         * callback that will be called if playback finished
         */
        sigc::signal<void> *onPlaybackFinished;
};

#endif /*MMSSOUND_H_*/
