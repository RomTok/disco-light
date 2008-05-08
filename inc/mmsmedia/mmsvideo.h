/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
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

#ifndef MMSVIDEO_H_
#define MMSVIDEO_H_

#include "mmsmedia/mmsav.h"

/**
 * @brief   Handles Video playback.
 * 
 * @ingroup     mmsmedia mmslibs
 *
 * @author      Matthias Hardt (mattmax@morphine.tv)
 * @version     1.0.3
 * @date        11/07/2007
 * 
 * This class is derived from MMSAV and specialized in
 * handling the playback of video files.
 */
class MMSVideo : public MMSAV {
    public:
        MMSVideo(MMSWindow *window, const bool verbose = false);
        ~MMSVideo();
        
        void startPlaying(const string file, const bool cont = true);
};

#endif /*MMSVIDEO_H_*/
