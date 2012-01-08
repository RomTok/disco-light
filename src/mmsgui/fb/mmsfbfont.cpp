/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2012 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mmsgui/fb/mmsfbfont.h"
#include "mmsgui/fb/mmsfb.h"

#include <ft2build.h>
#include FT_GLYPH_H

// static variables
pthread_mutex_t	globalLock = PTHREAD_MUTEX_INITIALIZER;
void *MMSFBFont::ft_library = NULL;
unsigned int MMSFBFont::numReferences = 0;

#define INITCHECK  if(!this->isInitialized()){MMSFB_SetError(0,"MMSFBFont is not initialized");return false;}

MMSFBFont::MMSFBFont(string filename, int w, int h) :
	initialized(false),
#ifdef __HAVE_DIRECTFB__
	dfbfont(NULL),
#endif
	ft_face(NULL),
	filename(filename),
	ascender(0),
	descender(0),
	height(0) {

    if (mmsfb->backend == MMSFB_BE_DFB) {
#ifdef  __HAVE_DIRECTFB__

		// create the dfb font
		DFBResult   		dfbres;
		DFBFontDescription 	desc;
		if (w > 0) {
			desc.flags = DFDESC_WIDTH;
			desc.width = w;
		}
		if (h > 0) {
			desc.flags = DFDESC_HEIGHT;
			desc.height = h;
		}
		if ((dfbres=mmsfb->dfb->CreateFont(mmsfb->dfb, this->filename.c_str(), &desc, (IDirectFBFont**)&this->dfbfont)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFB::CreateFont(" + this->filename + ") failed");
			return;
		}
		this->initialized = true;
#endif
    }
    else {
		// init freetype library
		pthread_mutex_lock(&globalLock);
    	if (!ft_library) {
    		if (FT_Init_FreeType((FT_Library*)&this->ft_library)) {
    			MMSFB_SetError(0, "FT_Init_FreeType() failed");
    			this->ft_library = NULL;
				pthread_mutex_unlock(&globalLock);
    			return;
			}
		}

    	this->numReferences++;
		pthread_mutex_unlock(&globalLock);

    	// load the face
    	if (FT_New_Face((FT_Library)this->ft_library, this->filename.c_str(), 0, (FT_Face*)&this->ft_face)) {
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
    }
}

MMSFBFont::~MMSFBFont() {
	lock();

	for(std::map<unsigned int, MMSFBFont_Glyph>::iterator it = this->charmap.begin();
			it != this->charmap.end(); ++it) {
		if(it->second.buffer)
			free(it->second.buffer);

#ifdef  __HAVE_OPENGL__
		if(mmsfb->bei)
			mmsfb->bei->deleteTexture(it->second.texture);
#endif

	}
	this->charmap.clear();

	if(mmsfb->backend != MMSFB_BE_DFB) {
		if(this->ft_face) {
			FT_Done_Face((FT_Face)this->ft_face);
		}
	}

	unlock();
	
	pthread_mutex_lock(&globalLock);
	this->numReferences--;

	if(mmsfb->backend != MMSFB_BE_DFB) {
		if(this->ft_library && this->numReferences == 0) {
			FT_Done_FreeType((FT_Library)this->ft_library);
			this->ft_library = NULL;
		}
	}
	pthread_mutex_unlock(&globalLock);
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


bool MMSFBFont::getGlyph(unsigned int character, MMSFBFont_Glyph *glyph) {
	if (!glyph) {
		return false;
	}

    if (mmsfb->backend == MMSFB_BE_DFB) {
#ifdef  __HAVE_DIRECTFB__
#endif
    }
    else {
    	if(!this->ft_face) {
    		return false;
    	}

    	bool ret = false;

    	lock();

    	// check if requested character is already loaded
    	std::map<unsigned int, MMSFBFont_Glyph>::iterator it;
    	it = this->charmap.find(character);
    	if (it == this->charmap.end()) {
    		// no, have to load it
			FT_GlyphSlotRec *g = NULL;

			if (!FT_Load_Glyph((FT_Face)this->ft_face, FT_Get_Char_Index((FT_Face)this->ft_face, (FT_ULong)character), FT_LOAD_RENDER)) {
				g = ((FT_Face)this->ft_face)->glyph;
			} else {
				MMSFB_SetError(0, "FT_Load_Glyph() failed for " + this->filename);
			}

			if (!((g)&&(g->format != ft_glyph_format_bitmap))) {
				if (FT_Render_Glyph(g, ft_render_mode_normal)) {
					g = NULL;
					MMSFB_SetError(0, "FT_Render_Glyph() failed for " + this->filename);
				}
			}

			if (!((g)&&(g->bitmap.pixel_mode == ft_pixel_mode_grays))) {
				g = NULL;
				MMSFB_SetError(0, "glyph->bitmap.pixel_mode != ft_pixel_mode_grays for " + this->filename);
			}

			if(!g) {
				unlock();
				return false;
			}

			// setup glyph values
			glyph->left		= g->bitmap_left;
			glyph->top		= g->bitmap_top;
			glyph->width	= g->bitmap.width;
			glyph->height	= g->bitmap.rows;
			glyph->advanceX	= g->advance.x;

#ifdef  __HAVE_OPENGL__
			// add glyph to charmap, we use a pitch which is a multiple of 4 needed e.g. for OGL textures
			if(mmsfb->bei && (g->bitmap.pitch & 3)) {
				glyph->pitch = (g->bitmap.pitch & ~3) + 4;
				glyph->buffer = (unsigned char*)calloc(1, glyph->pitch * glyph->height);
				unsigned char *src = g->bitmap.buffer;
				unsigned char *dst = glyph->buffer;
				for(int i = 0; i < glyph->height; i++) {
					memcpy(dst, src, glyph->pitch);
					src += g->bitmap.pitch;
					dst += glyph->pitch;
				}
			} else
#endif
			{
				glyph->pitch  = g->bitmap.pitch;
				glyph->buffer = (unsigned char*)calloc(1, glyph->pitch * glyph->height);
				memcpy(glyph->buffer, g->bitmap.buffer, glyph->pitch * glyph->height);
			}

			if (MMSFBBase_rotate180) {
				// rotate glyph by 180°
				rotateUCharBuffer180(glyph->buffer, glyph->pitch, glyph->width, glyph->height);
			}

#ifdef  __HAVE_OPENGL__
			// if disko is built and initialized for OpenGL, we create a texture for this glyph
			if (mmsfb->bei) {
				glyph->texture = 0;
				mmsfb->bei->createAlphaTexture(&glyph->texture, glyph->buffer,
												glyph->pitch, glyph->height);
			}
#endif

			// add to charmap
			this->charmap.insert(std::make_pair(character, *glyph));
			ret = true;
    	}
    	else {
    		// already loaded
    		*glyph = it->second;
			ret = true;
    	}

    	unlock();

    	return ret;
    }

    return false;
}


bool MMSFBFont::getStringWidth(string text, int len, int *width) {
    // check if initialized
    INITCHECK;

    // reset width
    if (!width) return false;
	*width = 0;

	// get the length of the string
	if (len < 0) len = text.size();
	if (!len) return true;

    // get the width of the whole string
#ifdef  __HAVE_DIRECTFB__
    if (this->dfbfont) {
		if (((IDirectFBFont*)this->dfbfont)->GetStringWidth((IDirectFBFont*)this->dfbfont, text.c_str(), len, width) != DFB_OK)
			return false;
		return true;
    } else
#endif
    {
    	MMSFBFONT_GET_UNICODE_CHAR(text, len) {
    		MMSFBFont_Glyph glyph;
    		if (!getGlyph(character, &glyph)) break;
			(*width)+=glyph.advanceX >> 6;
    	} }
    	return true;
    }
    return false;
}

bool MMSFBFont::getHeight(int *height) {
    // check if initialized
    INITCHECK;

    // get the height of the font
#ifdef  __HAVE_DIRECTFB__
	if (this->dfbfont) {
		if (((IDirectFBFont*)this->dfbfont)->GetHeight((IDirectFBFont*)this->dfbfont, height) != DFB_OK)
			return false;
		return true;
    } else
#endif
	{
    	*height = this->height;
    	return true;
    }
    return false;
}

bool MMSFBFont::getAscender(int *ascender) {
    // check if initialized
    INITCHECK;

    // get the ascender of the font
#ifdef __HAVE_DIRECTFB__
    if (this->dfbfont) {
	} else
#endif
	{
		*ascender = this->ascender;
		return true;
	}
	return false;
}

bool MMSFBFont::getDescender(int *descender) {
    // check if initialized
    INITCHECK;

    // get the ascender of the font
#ifdef __HAVE_DIRECTFB__
    if (this->dfbfont) {
	}
	else
#endif
	{
		*descender = this->descender;
		return true;
	}
	return false;
}
