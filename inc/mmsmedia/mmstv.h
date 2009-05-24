/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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
 * @ingroup     mmsmedia
 *
 * @author      Matthias Hardt (MHardt@berlinux-solutions.de)
 * @version     1.5.0
 * @date        12/31/2008
 *
 * This class is derived from MMSAV and specialized in
 * handling TV playback. It only works for DVB-T/C/S streams.
 */
class MMSTV : public MMSAV {
    private:
        string	     channel;               /**< currently played channel name              */
        string       captureFilename;       /**< filename for saved stream                  */
        unsigned int timeout;               /**< tuning timeout in seconds                  */
        bool         recording;             /**< if true recording is on                    */

#ifdef __HAVE_XINE_BLDVB__
        bool		 useBlDvb;				/**< if true use our own dvb input plugin		*/
#endif

#ifdef __HAVE_XINE__
        void xineOpen();
#endif

    public:
#ifdef __HAVE_XINE_BLDVB__
    	MMSTV(MMSWindow *window, const string channel = "", const bool verbose = false, const bool useBlDvb = true);
#else
    	MMSTV(MMSWindow *window, const string channel = "", const bool verbose = false);
#endif
        ~MMSTV();

        void startPlaying(const string channel = "");
        void play();
#ifdef __HAVE_XINE_BLDVB__
        void play(const string &channel);
#endif
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
