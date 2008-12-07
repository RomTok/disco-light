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

#ifndef MMSFLASH_H_
#define MMSFLASH_H_

#ifndef _NO_MMSFLASH

#include "mmsgui/mmsgui.h"

class MMSFlashThread;

//! Handles Flash playback and navigation.
/*!
\author Jens Schneider
*/
class MMSFlash {
	private:
		//! swfdec must be initialized once
		static bool swfdec_initialized;

		//! this class is ready for playing
		bool			ready;

		//! this class is playing
		bool			playing;

        //! to make it thread-safe :)
		MMSMutex		lock;

		//! window that will display the flash file
		MMSWindow		*window;

		//! flash file to play
		string			filename;

	    //! swfdec player
	    void			*swfdec_player;

	    //! frame rate
	    double			swfdec_rate;

	    //! width of the flash image
	    int 			width;

	    //! height of the flash image
	    int				height;

	    //! temporary surface to let the swfdec render to
	    MMSFBSurface	*flash_temp_surface;

	    //! temporary cairo surface
	    void			*cairosurface;

	    //! temporary cairo
	    void			*cairo;

	    //! loader thread
	    MMSFlashThread	*loaderthread;

	    //! player thread
	    MMSFlashThread	*playerthread;

        //! Internal method: Loading Flash, runs in the loader thread.
	    void loader(bool &stop);

        //! Internal method: Playing Flash, runs in the player thread.
	    void player(bool &stop);

        //! Internal method: Stop my threads.
	    void stopThreads(void);

        //! Internal method: Map keys.
	    unsigned int mapKey(MMSKeySymbol key);

        //! Internal method: Will be called on user input.
	    bool onHandleInput(MMSWindow *window, MMSInputEvent *input);

    public:

    	//! The Flash constructor.
    	MMSFlash(MMSWindow *window);

    	//! The Flash destructor.
    	~MMSFlash();

        //! Start loading/playing of the given flash file.
        /*!
        If a other file is currently playing, the playback will be stopped before starting the new one.
        \param filename		flash file to play
        */
        void startPlaying(string filename);

        //! Is the flash file ready for playing?
        /*!
        \return true if ready, else there are problems while loading the flash file
        \note This function is waiting for the ready or error state.
        */
        bool isReady();

        //! Is the flash file playing?
        /*!
        \return true if playing
        */
        bool isPlaying(bool wait = false);

    friend class MMSFlashThread;
};

#endif /* _NO_MMSFLASH */

#endif /*MMSFLASH_H_*/
