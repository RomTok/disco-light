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

#include "mmsgui/mmsfontmanager.h"
#include "mmstools/mmstools.h"

MMSFontManager::MMSFontManager() {
}

MMSFontManager::~MMSFontManager() {
    /* free all fonts */
    for (unsigned int i = 0; i < this->fonts.size(); i++) {
        this->fonts.at(i).font->Release(this->fonts.at(i).font);
    }
}

IDirectFBFont *MMSFontManager::getFont(string path, string filename, unsigned int size) {
    string          fontfile;
    MMSFM_DESC      fm_desc;

    /* build filename */
    fontfile = path;
    if (fontfile != "") fontfile+= "/";
    fontfile += filename;
    if (fontfile == "")
        return NULL;

    /* search within fonts list */
    for (unsigned int i = 0; i < this->fonts.size(); i++) {
        if (this->fonts.at(i).fontfile == fontfile)
            if (this->fonts.at(i).size == size)
                return this->fonts.at(i).font;
    }

    /* load font */
    fm_desc.font = NULL;
    if (!loadFont(&(fm_desc.font), "", fontfile, size)) {
        DEBUGMSG("MMSGUI", "cannot load font file '%s'", fontfile.c_str());
        return NULL;
    }
    fm_desc.fontfile = fontfile;
    fm_desc.size = size;

    /* add to fonts list and return the font */
    this->fonts.push_back(fm_desc);
    return fm_desc.font; 
}

void MMSFontManager::releaseFont(string path, string filename, unsigned int size) {
    /*TODO*/
}

void MMSFontManager::releaseFont(IDirectFBFont *font) {
    /*TODO*/
}

