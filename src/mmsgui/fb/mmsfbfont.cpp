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

// static variables
#ifdef __HAVE_XLIB__
FT_Library MMSFBFont::ft_library = NULL;
#endif

MMSFBFont::MMSFBFont(IDirectFBFont *dfbfont, int w, int h) {
	this->dfbfont = dfbfont;
	this->w = w;
	this->h = h;
}

MMSFBFont::~MMSFBFont() {
}

bool MMSFBFont::getStringWidth(string text, int bytes, int *width) {
    if (this->dfbfont->GetStringWidth(this->dfbfont, text.c_str(), -1, width) != DFB_OK)
    	return false;
    return true;
}

bool MMSFBFont::getHeight(int *height) {
	if (this->dfbfont->GetHeight(this->dfbfont, height) != DFB_OK)
		return false;
	return true;
}



