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

#ifndef MMSFBFONT_H_
#define MMSFBFONT_H_

#include "mmsgui/fb/mmsfbbase.h"

//! Font rendering class.
/*!
\author Jens Schneider
*/
class MMSFBFont {
    private:
        //! true if initialized
        bool 		initialized;

        //! to make it thread-safe
        MMSMutex  	Lock;

    	//! pointer to the directfb font
    	void 		*dfbfont;

    	//! static pointer to the freetype library
        static void *ft_library;

        //! pointer to the loaded freetype face
        void 		*ft_face;

        //! font file
        string 	filename;

        //! input width
    	int		w;

    	//! input height
    	int 	h;

    	//! ascender
    	int 	ascender;

    	//! descender
    	int		descender;

    	//! real height of one line
    	int 	height;

    	void *getGlyph(unsigned long character);

    public:
        MMSFBFont(string filename, int w, int h);
        virtual ~MMSFBFont();

        bool isInitialized();
        void lock();
        void unlock();

        bool getStringWidth(string text, int bytes, int *width);
        bool getHeight(int *height);

	friend class MMSFBSurface;
};

#endif /*MMSFBFONT_H_*/
