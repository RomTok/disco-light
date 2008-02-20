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

#include "mmsmedia/mmsvideo.h"

MMS_CREATEERROR(MMSVideoError);

/**
 * Initializes everything that is needed by MMSVideo.
 *
 * @param   window  [in]    main window for video playback
 * @param   verbose [in]    if true the xine engine writes debug messages to stdout
 *  
 * @see MMSAV::MMSAV()
 * @see MMSAV::initialize()
 */
MMSVideo::MMSVideo(MMSWindow *window, const bool verbose) {
    MMSAV::initialize(verbose, window);
}

/**
 * Destructor of MMSVideo class.
 */
MMSVideo::~MMSVideo() {
}

/**
 * Starts playing.
 *
 * If the continue flag is set it tries to continue
 * at the position where it was stopped before. 
 * 
 * @param   file    file to play
 * @param   cont    if true it tries to continue at a position stopped before
 * 
 * @exception   MMSAVError stream could not be opened
 */
void MMSVideo::startPlaying(const string file, const bool cont) {
    
   string::size_type loc = file.find( "://", 0 );
   if( loc != string::npos ) {
	   /* we found a mrl, so we play file directly */
	   MMSAV::startPlaying(file, cont);
   } 
   else {
	   /* we found no mrl type, it seems to be a file */
	   MMSAV::startPlaying("file://" + file, cont);
   }
}
