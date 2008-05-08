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

#ifndef MMSFONTMANAGER_H_
#define MMSFONTMANAGER_H_

#include "mmsgui/mmsguitools.h"

typedef struct {
    string          fontfile;
    unsigned int    size;
    IDirectFBFont   *font;
} MMSFM_DESC;

class MMSFontManager {
    private:
        vector<MMSFM_DESC>  fonts;

    public:
        MMSFontManager();
        ~MMSFontManager();
        IDirectFBFont *getFont(string path, string filename, unsigned int size);
        void releaseFont(string path, string filename, unsigned int size);
        void releaseFont(IDirectFBFont *font);
};

#endif /*MMSFONTMANAGER_H_*/
