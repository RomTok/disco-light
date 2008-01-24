/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
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

#include "mmsgui/mmsguitools.h"
#include "mmsgui/mmsguilogger.h"


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

bool getColorFromString(string input, DFBColor *color) {

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

bool getScreenInfo(int *w, int *h, IDirectFBDisplayLayer **layer, IDirectFB *dfb) {
	IDirectFB 				*mydfb = NULL;
	IDirectFBDisplayLayer 	*mylayer;
    DFBDisplayLayerConfig 	cfg;
    MMSGuiLogger logger;

	if (!dfb) {
		if(DirectFBCreate(&mydfb)!= DFB_OK)
        	return false;
	}
	else
		mydfb = dfb;

    mydfb->GetDisplayLayer(mydfb, DLID_PRIMARY, &mylayer);
    mylayer->GetConfiguration(mylayer, &cfg);
    *w=cfg.width;
    *h=cfg.height;
    logger.writeLog("screen resolution: " + iToStr(*w) + "/" + iToStr(*h)); 
    
    if (!layer)
    	mylayer->Release(mylayer);
    else
    	*layer = mylayer;

    if (!dfb)
    	mydfb->Release(mydfb);

    return true;
}

bool loadImage(IDirectFBImageProvider **image, string path, string filename) {
    MMSGuiLogger           logger;
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

    logger.writeLog("using image file '" + imagefile + "'");

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
        logger.writeLog("cannot load image file '" + imagefile + "'");
        return false;
    }

/*    if (!dfb)
        mydfb->Release(mydfb);*/

    *image = myimage;

    return true;
}


bool loadFont(IDirectFBFont **font, string path, string filename, unsigned int size) {
    MMSGuiLogger    logger;
//    IDirectFB       *mydfb = NULL;
    IDirectFBFont   *myfont = NULL;
    string          fontfile;

    /* build filename */
    fontfile = path;
    if (fontfile != "") fontfile+= "/";
    fontfile += filename;

    logger.writeLog("using font file '" + fontfile + "'");

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

    DFBFontDescription desc;
    desc.flags = DFDESC_HEIGHT;
    desc.height = size;
    
//    if (mydfb->CreateFont(mydfb, fontfile.c_str(), &desc, &myfont) != DFB_OK) {
    if (!mmsfb->createFont(&myfont, fontfile, &desc)) {
/*        if (!dfb)
            mydfb->Release(mydfb);*/
        if (myfont)
            myfont->Release(myfont);
        logger.writeLog("cannot load font file '" + fontfile + "'");
        return false;
    }

/*    if (!dfb)
        mydfb->Release(mydfb);*/

    if (*font)
        (*font)->Release(*font);

    *font = myfont;

    return true;
}



