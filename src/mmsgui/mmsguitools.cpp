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

#include "mmsgui/mmsguitools.h"
#include <stdlib.h>

bool getPixelFromSizeHint(int *retpix, string hint, int maxpixel, int secondaxis) {
    std::string::size_type pos;
    pos = hint.rfind("px");
    if (pos == std::string::npos) {
        /* its not px */
        pos = hint.rfind("%");
        if (pos == std::string::npos) {
            /* its not %, failed */
            pos = hint.rfind("$");
            if (pos == std::string::npos) {
                /* its not $, failed */
                return false;
            }
            else {
                /* use factor with secondaxis parameter */
                int pix = (int)(atof(hint.substr(0,pos).c_str()) * (double)secondaxis);
                if (pix > maxpixel) {
                    /* greater than maxpixel, failed */
                    return false;
                }
                if (retpix) *retpix = pix;
                return true;
            }
        }
        else {
            /* calculate percent to pixel */
            int pix = atoi(hint.substr(0,pos).c_str());
            if (pix > 100) {
                /* greater than 100%, failed */
                return false;
            }
            pix = (pix * maxpixel) / 100;

            if (hint.substr(pos+1,1)=="-") {
                /* subtract a value from calculated percent size */
                pix-=atoi(hint.substr(pos+2).c_str());
            }
            else
            if (hint.substr(pos+1,1)=="+") {
                /* add a value to calculated percent size */
                pix+=atoi(hint.substr(pos+2).c_str());
            }

            if (retpix) *retpix = pix;
            return true;
        }
    }
    else {
        /* use pixel values */
        int pix = atoi(hint.substr(0,pos).c_str());
        if (pix > maxpixel) {
            /* greater than maxpixel, failed */
            return false;
        }

        if (hint.substr(pos+2,1)=="-") {
            /* subtract a value from pixel */
            pix-=atoi(hint.substr(pos+3).c_str());
        }
        else
        if (hint.substr(pos+2,1)=="+") {
            /* add a value to pixel */
            pix+=atoi(hint.substr(pos+3).c_str());
        }

        if (retpix) *retpix = pix;
        return true;
    }
}

bool getColorFromString(string input, MMSFBColor *color) {

    color->r = 0;
    color->g = 0;
    color->b = 0;
    color->a = 0;

    if (input == "")
        return false;

    if (input.size()!=9)
        return false;

    if (input.substr(0,1)!="#")
        return false;

    color->r = hexToInt(input.substr(1,2).c_str());
    color->g = hexToInt(input.substr(3,2).c_str());
    color->b = hexToInt(input.substr(5,2).c_str());
    color->a = hexToInt(input.substr(7,2).c_str());

    return true;
}


#ifdef  __HAVE_DIRECTFB__
bool loadImage(IDirectFBImageProvider **image, string path, string filename) {
//    IDirectFB              *mydfb = NULL;
    IDirectFBImageProvider *myimage = NULL;
    string                 imagefile;

    /* free old image */
    if (*image) {
        (*image)->Release(*image);
        *image = NULL;
    }

    /* build filename */
    imagefile = path;
    if (imagefile != "") imagefile+= "/";
    imagefile += filename;

    DEBUGMSG("MMSGUI", "using image file '%s'", imagefile.c_str());

    if (filename == "")
        return false;

    if (filename.substr(filename.size()-1) == "/")
        return false;

    /* open dfb access */
/*    if (!dfb) {
        if(DirectFBCreate(&mydfb)!= DFB_OK)
            return false;
    }
    else
        mydfb = dfb;*/

//    if (mydfb->CreateImageProvider(mydfb, imagefile.c_str(), &myimage) != DFB_OK) {
    if (!mmsfb->createImageProvider(&myimage, imagefile)) {
/*        if (!dfb)
            mydfb->Release(mydfb);*/
        if (myimage)
            myimage->Release(myimage);
        DEBUGMSG("MMSGUI", "cannot load image file '%s'", imagefile.c_str());
        return false;
    }

/*    if (!dfb)
        mydfb->Release(mydfb);*/

    *image = myimage;

    return true;
}
#endif

bool loadFont(MMSFBFont **font, string path, string filename, int width, int height) {
    MMSFBFont	*myfont = NULL;
    string    	fontfile;

    /* build filename */
    fontfile = path;
    if (fontfile != "") fontfile+= "/";
    fontfile += filename;

    DEBUGMSG("MMSGUI", "using font file '%s'", fontfile.c_str());

    if (filename == "")
        return false;

    if (filename.substr(filename.size()-1) == "/")
        return false;

    if (!mmsfb->createFont(&myfont, fontfile, width, height)) {
        if (myfont)
            delete myfont;
        DEBUGMSG("MMSGUI", "cannot load font file '%s'", fontfile.c_str());
        return false;
    }

    if (*font)
        delete *font;

    *font = myfont;

    return true;
}


unsigned int getFrameNum(unsigned int delay_time) {
	// a frame every 50ms
	unsigned int ret = delay_time / 50;
	if (ret < 2) ret = 2;
	return ret;
}

unsigned int getFrameDelay(unsigned int start_ts, unsigned int end_ts) {
	unsigned int diff = getMDiff(start_ts, end_ts);
	if (25 > diff)
		return 25-diff;
	else
		return 0;
}

