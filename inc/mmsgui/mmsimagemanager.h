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

#ifndef MMSIMAGEMANAGER_H_
#define MMSIMAGEMANAGER_H_

#include "mmsgui/mmsguitools.h"
#include "mmstools/mmstools.h"
#include "mmstools/mmslogger.h"

#define MMSIM_MAX_DESC_SUF      64
#define MMSIM_DESC_SUF_LOADING  0xfffffffe
#define MMSIM_DESC_SUF_END      0xffffffff

typedef struct {
    MMSFBSurface    *surface;   //pointer to an surface
    unsigned int    delaytime;  //milliseconds
} MMSIM_DESC_SUF;

typedef struct {
    string          name;                       //extra name
    string          imagefile;                  //filename or url
    time_t          mtime;                      //modification time of the file
    int             usecount;                   //number of times this image is used
    MMSIM_DESC_SUF  suf[MMSIM_MAX_DESC_SUF+1];  //buffer for 'sub'-surfaces of the image (e.g. neede for GIF)
    int             sufcount;                   //number of surfaces available
    bool            loading;                    //true if loader is already running
} MMSIM_DESC;

class MMSImageManager {
    private:
        MMSFBLayer          *layer;   		// this is the layer on which the image is to display
        string				pixelformat;	// pixelformat for all my images
        bool				usetaff;		// use the taff (image) format?
        MMSTAFF_PF			taffpf;			// pixelformat for the taff converter

        vector<MMSIM_DESC*> images;

        bool surface2TAFF(string &imagefile, MMSFBSurface *surface);

        bool loadGIF(string file, MMSIM_DESC *desc);

    public:
        MMSImageManager(MMSFBLayer *layer);
        ~MMSImageManager();
        MMSFBSurface *getImage(const string &path, const string &filename, MMSIM_DESC_SUF **surfdesc = NULL);
        MMSFBSurface *newImage(const string &name, unsigned int width, unsigned int height, string pixelformat = ""); 
        void releaseImage(const string &path, const string &filename);
        void releaseImage(MMSFBSurface *surface);
};

#endif /*MMSIMAGEMANAGER_H_*/
