/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
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

#ifndef MMSTV_H_
#define MMSTV_H_

#include "mmsmedia/mmsav.h"

/**
 * @brief   Handles TV playback.
 * 
 * @ingroup     mmsmedia mmslibs
 *
 * @author      Matthias Hardt (mattmax@morphine.tv)
 * @version     1.0.3
 * @date        11/07/2007
 * 
 * This class is derived from MMSAV and specialized in
 * handling TV playback. It only works for DVB-T/C/S streams.
 */
class MMSTV : public MMSAV {
    private:
        string	     channel;               /**< currently played channel name              */
        string       captureFilename;       /**< filename for saved stream                  */
        unsigned int timeout;               /**< tuning timeout in seconds                  */
        bool         recording,             /**< if true recording is on                    */
                     usingInputDVBMorphine; /**< if true our own xine input plugin is used  */

    public:
        MMSTV(MMSWindow *window, const string channel = "", const bool verbose = false);
        ~MMSTV();

        void open();
        void startPlaying(const string channel = "");
        void play();
        void pause();
        void previous();
        void next();
        void record();
        void recordStart();
        void recordStop();
        void recordPause();
        
        string getCurrentChannelName(void);
        
        void setTuningTimeout(const unsigned int timeout);
        const unsigned int getTuningTimeout();
        void setRecordDir(const string dir);
        const string getRecordDir();
        const string getRecordFilename();

        /**
         * Callback that is used to receive buffering progress changes.
         */
        sigc::signal<void, const unsigned short> onProgressChange;
};

#endif /*MMSTV_H_*/
