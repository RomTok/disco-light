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
/*
    	if (!ft_library) {
    		// init freetype library
    		if (FT_Init_FreeType(&ft_library)) {
    			MMSFB_SetError(0, "FT_Init_FreeType() failed");
    			return false;
			}
		}
    	FT_Face face;
    	if (FT_New_Face(library, "/home/jys/workspace_local/disko-tutorials/firststeps/04/themes/default/DejaVuSansMono.ttf", 0, &face))
    		return -1;
    	if (FT_Select_Charmap(face, ft_encoding_unicode))
    		return -1;
    	int fw = 250;
    	int fh = 250;
    	if (FT_Set_Char_Size(face, fw << 6, fh << 6, 0, 0))
    		return -1;
    	FT_Int load_flags = FT_LOAD_DEFAULT;
    	face->generic.data = (void *)(unsigned long) load_flags;
    	face->generic.finalizer = NULL;
*/

#endif
    }
}

MMSFBFont::~MMSFBFont() {
}

bool MMSFBFont::isInitialized() {
    return this->initialized;
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
    	return true;
#endif
    }
    return false;
}



