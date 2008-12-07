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

#ifndef MMSFBFONT_H_
#define MMSFBFONT_H_

#include "mmsgui/fb/mmsfbbase.h"

//! descibes a loaded glyph
typedef struct {
	//! pointer to data
	unsigned char	*buffer;
	//! pitch in byte of one row in the buffer
	int 			pitch;
	//! x-offset
	int				left;
	//! y-offset
	int				top;
	//! width in pixel of the glyph bitmap
	int 			width;
	//! height in pixel of the glyph bitmap
	int 			height;
	//! width in pixel of the whole character
	int				advanceX;
} MMSFBFont_Glyph;

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

    	//! maps a character id to a already loaded glyph (see glyphpool)
    	std::map<unsigned int, MMSFBFont_Glyph> charmap;

    	//! glyph pool
    	unsigned char *glyphpool;

    	//! size of glyph pool
    	unsigned int glyphpool_size;

    	//! pointer to next free memory
    	unsigned char *glyphpool_ptr;

    	//! stores the glyph infos for method getGlyph()
    	MMSFBFont_Glyph glyph;

    public:
        MMSFBFont(string filename, int w, int h);
        virtual ~MMSFBFont();

        bool isInitialized();
        void lock();
        void unlock();

        bool getStringWidth(string text, int len, int *width);
        bool getHeight(int *height);

    	MMSFBFont_Glyph *getGlyph(unsigned int character);

	friend class MMSFBSurface;
};

#define MMSFBFONT_GET_UNICODE_CHAR(text, len) \
	for (int cnt = 0; cnt < len; cnt++) { \
		unsigned char c = text[cnt]; \
		unsigned int character = (unsigned int)c; \
		if ((character >= 0xc0)&&(character <= 0xf7)) { \
			if ((character & 0xe0) == 0xc0) { c = text[++cnt]; character = ((character & 0x1f) << 6) | (c & 0x3f); } \
			if ((character & 0xf0) == 0xe0) { c = text[++cnt]; character = ((character & 0x0f) << 6) | (c & 0x3f); \
				if (cnt < len) { c = text[++cnt]; character = (character << 6) | (c & 0x3f); } } \
			if ((character & 0xf8) == 0xf0) { c = text[++cnt]; character = ((character & 0x07) << 6) | (c & 0x3f); \
				if (cnt < len) { c = text[++cnt]; character = (character << 6) | (c & 0x3f); } \
				if (cnt < len) { c = text[++cnt]; character = (character << 6) | (c & 0x3f); } } \
		}

#endif /*MMSFBFONT_H_*/
