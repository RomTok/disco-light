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

#include "mmsgui/fb/mmsfbfont.h"
#include "mmsgui/fb/mmsfb.h"

#ifdef __HAVE_XLIB__
#include <ft2build.h>
#include FT_GLYPH_H
#endif

// static variables
void *MMSFBFont::ft_library = NULL;

#define INITCHECK  if(!this->isInitialized()){MMSFB_SetError(0,"MMSFBFont is not initialized");return false;}

MMSFBFont::MMSFBFont(string filename, int w, int h) {
	this->initialized 	= false;
	this->dfbfont 		= NULL;
	this->ft_library 	= NULL;
	this->ft_face 		= NULL;
	this->filename 		= filename;
	this->w 			= w;
	this->h 			= h;

    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		// create the dfb font
		DFBResult   		dfbres;
		DFBFontDescription 	desc;
		if (this->w > 0) {
			desc.flags = DFDESC_WIDTH;
			desc.width = this->w;
		}
		if (this->h > 0) {
			desc.flags = DFDESC_HEIGHT;
			desc.height = this->h;
		}
		if ((dfbres=mmsfb->dfb->CreateFont(mmsfb->dfb, this->filename.c_str(), &desc, (IDirectFBFont**)&this->dfbfont)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFB::CreateFont(" + this->filename + ") failed");
			return;
		}
		this->initialized = true;
#endif
    }
    else {
#ifdef  __HAVE_XLIB__
		// init freetype library
    	if (!ft_library) {
    		if (FT_Init_FreeType((FT_Library*)&this->ft_library)) {
    			MMSFB_SetError(0, "FT_Init_FreeType() failed");
    			return;
			}
		}

    	// load the face
    	if (FT_New_Face((FT_Library)ft_library, this->filename.c_str(), 0, (FT_Face*)&this->ft_face)) {
    		this->ft_face = NULL;
			MMSFB_SetError(0, "FT_New_Face(" + this->filename + ") failed");
			return;
    	}

    	// select the charmap
    	if (FT_Select_Charmap((FT_Face)this->ft_face, ft_encoding_unicode)) {
    		FT_Done_Face((FT_Face)this->ft_face);
    		this->ft_face = NULL;
			MMSFB_SetError(0, "FT_Select_Charmap(ft_encoding_unicode) for " + this->filename + " failed");
			return;
    	}

    	// set the font size
    	if (FT_Set_Char_Size((FT_Face)this->ft_face, w << 6, h << 6, 0, 0)) {
    		FT_Done_Face((FT_Face)this->ft_face);
    		this->ft_face = NULL;
			MMSFB_SetError(0, "FT_Set_Char_Size(" + iToStr(w << 6) + "," + iToStr(h << 6) + ") for " + this->filename + " failed");
			return;
    	}

    	// try to load a first glyph
    	if (FT_Load_Glyph((FT_Face)this->ft_face, FT_Get_Char_Index((FT_Face)this->ft_face, '0'), FT_LOAD_RENDER)) {
    		FT_Done_Face((FT_Face)this->ft_face);
    		this->ft_face = NULL;
			MMSFB_SetError(0, "FT_Load_Glyph('0') for " + this->filename + " failed");
			return;
    	}
    	if (((FT_Face)this->ft_face)->glyph->format != ft_glyph_format_bitmap) {
    		FT_Done_Face((FT_Face)this->ft_face);
    		this->ft_face = NULL;
			MMSFB_SetError(0, "Glyph format is not ft_glyph_format_bitmap for " + this->filename);
			return;
    	}

    	this->ascender = ((FT_Face)this->ft_face)->size->metrics.ascender >> 6;
    	this->descender = abs(((FT_Face)this->ft_face)->size->metrics.descender >> 6);
    	this->height = this->ascender + this->descender + 1;

    	this->initialized = true;
#endif
    }
}

MMSFBFont::~MMSFBFont() {
}

bool MMSFBFont::isInitialized() {
    return this->initialized;
}

void MMSFBFont::lock() {
	this->Lock.lock();
}

void MMSFBFont::unlock() {
	this->Lock.unlock();
}

void *MMSFBFont::getGlyph(unsigned int character) {
    if (mmsfb->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
#endif
    }
    else {
#ifdef  __HAVE_XLIB__
	FT_GlyphSlotRec *glyph = NULL;
	if (!FT_Load_Glyph((FT_Face)this->ft_face, FT_Get_Char_Index((FT_Face)this->ft_face, (FT_ULong)character), FT_LOAD_RENDER))
		glyph = ((FT_Face)this->ft_face)->glyph;
	else
		MMSFB_SetError(0, "FT_Load_Glyph() failed for " + this->filename);
	if (!((glyph)&&(glyph->format != ft_glyph_format_bitmap)))
		if (FT_Render_Glyph(glyph, ft_render_mode_normal)) {
			glyph = NULL;
			MMSFB_SetError(0, "FT_Render_Glyph() failed for " + this->filename);
		}
	return glyph;
#endif
    }

    return NULL;
}

bool MMSFBFont::getStringWidth(string text, int bytes, int *width) {
    // check if initialized
    INITCHECK;

    // get the width of the whole string
    if (this->dfbfont) {
#ifdef  __HAVE_DIRECTFB__
		if (((IDirectFBFont*)this->dfbfont)->GetStringWidth((IDirectFBFont*)this->dfbfont, text.c_str(), -1, width) != DFB_OK)
			return false;
		return true;
#endif
    }
    else {
#ifdef  __HAVE_XLIB__
    	lock();
    	*width = 0;
    	unsigned int len = text.size();
    	for (unsigned int i = 0; i < len; i++) {
			if (FT_Load_Glyph((FT_Face)this->ft_face, FT_Get_Char_Index((FT_Face)this->ft_face, (FT_ULong)text[i]), FT_LOAD_RENDER))
				break;
			if (((FT_Face)this->ft_face)->glyph->format != ft_glyph_format_bitmap)
				if (FT_Render_Glyph(((FT_Face)this->ft_face)->glyph, ft_render_mode_normal))
					break;
			*width+=((FT_Face)this->ft_face)->glyph->advance.x >> 6;
    	}
    	unlock();
    	return true;
#endif
    }
    return false;
}

bool MMSFBFont::getHeight(int *height) {
    // check if initialized
    INITCHECK;

    // get the height of the font
	if (this->dfbfont) {
#ifdef  __HAVE_DIRECTFB__
		if (((IDirectFBFont*)this->dfbfont)->GetHeight((IDirectFBFont*)this->dfbfont, height) != DFB_OK)
			return false;
		return true;
#endif
    }
    else {
#ifdef  __HAVE_XLIB__
    	lock();
    	*height = this->height;
    	unlock();
    	return true;
#endif
    }
    return false;
}



