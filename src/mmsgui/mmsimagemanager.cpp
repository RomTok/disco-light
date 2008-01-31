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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mmsgui/mmsimagemanager.h"
#include "mmsgui/mmsguilogger.h"
#include "mmsgui/mmsgifloader.h"

MMSImageManager::MMSImageManager(MMSFBLayer *layer) {
    this->layer = layer;
}

MMSImageManager::~MMSImageManager() {
    /* free all surfaces */
    for (unsigned int i = 0; i < this->images.size(); i++) {
        for (int j = 0; j < this->images.at(i)->sufcount; j++)
            if (this->images.at(i)->suf[j].surface)
                delete this->images.at(i)->suf[j].surface;
        delete this->images.at(i);
    }
}

MMSFBSurface *MMSImageManager::getImage(const string &path, const string &filename, MMSIM_DESC_SUF **surfdesc) {
    string                  imagefile;
    IDirectFBImageProvider  *imageprovider = NULL;
    MMSIM_DESC              *im_desc = NULL;
    DFBSurfaceDescription   surface_desc;
    int                     reload_image = -1;

    /* build filename */
    imagefile = path;
    if (imagefile != "") imagefile+= "/";
    imagefile += filename;
    if (imagefile == "")
        return NULL;
    if (imagefile.substr(imagefile.size()-1,1)=="/")
        return NULL;

    /* search within images list */
    for (unsigned int i = 0; i < this->images.size(); i++) {
        if (this->images.at(i)->imagefile == imagefile) {
            /* already loaded, check if the file has changed */
            struct stat statbuf;
            if (stat(imagefile.c_str(), &statbuf)==0) {
                if (statbuf.st_mtime != this->images.at(i)->mtime) {
                    /* file was modified, reload it */ 
                    reload_image = (int)i;
                    this->images.at(i)->mtime = statbuf.st_mtime;
                    break;
                }
                else {
                    /* do not reload */
                    this->images.at(i)->usecount++;
                    if (surfdesc)
                        *surfdesc = this->images.at(i)->suf;
                    return this->images.at(i)->suf[0].surface;
                }
            }
            else {
                /* do not reload */
                this->images.at(i)->usecount++;
                if (surfdesc)
                    *surfdesc = this->images.at(i)->suf;
                return this->images.at(i)->suf[0].surface;
            }
        }
    }

    /* init im_desc */
    im_desc = new MMSIM_DESC;
    memset(im_desc->suf, 0, sizeof(im_desc->suf));
    im_desc->suf[0].delaytime = im_desc->suf[1].delaytime = MMSIM_DESC_SUF_END; 
    im_desc->sufcount = 0;
    im_desc->loading = false;

    /* first try to load GIF formated files */
    if (isGIF(imagefile)) {
        /* it's an GIF file */
        im_desc->imagefile = imagefile;

        if (reload_image < 0) {
            /* get the modification time of the file */
            struct stat statbuf;
            if (stat(imagefile.c_str(), &statbuf)==0)
                im_desc->mtime = statbuf.st_mtime;
            else
                im_desc->mtime = 0;
        }

        if (reload_image < 0) {
            /* load it */
            MMSGIFLoader *gifloader = new MMSGIFLoader(im_desc, this->layer);
            im_desc->loading = true;
            gifloader->start();
   
            /* wait of at least first image of GIF */
            int s = 10*1000;
            while ((im_desc->sufcount <= 0) && (im_desc->loading)) {
                usleep(s);
                if (s < 100*1000)
                    s+= 10*1000;
            }

            if (im_desc->sufcount > 0) {
                logger.writeLog("ImageManager has loaded: '" + imagefile + "'");
        
                /* add to images list and return the surface */
                im_desc->usecount = 1;
                this->images.push_back(im_desc);
                if (surfdesc)
                    *surfdesc = this->images.at(this->images.size()-1)->suf;
                return im_desc->suf[0].surface;
            }
            else {
                /* failed to load */
                logger.writeLog("cannot load image file '" + imagefile + "'");
                delete im_desc;
                return NULL;
            }
        }
        else {
            /* increase usecount */
            this->images.at(reload_image)->usecount++;

//TODO
            delete im_desc;
            if (surfdesc)
                *surfdesc = this->images.at(reload_image)->suf;
            return this->images.at(reload_image)->suf[0].surface;
        } 
    }
    else {
        /* failed, try it with DFB providers */
        if (!loadImage(&imageprovider, "", imagefile)) {
            logger.writeLog("cannot load image file '" + imagefile + "'");
            if (reload_image < 0) {
                delete im_desc;
                return NULL;
            }
            else {
                this->images.at(reload_image)->usecount++;
                delete im_desc;
                if (surfdesc)
                    *surfdesc = this->images.at(reload_image)->suf;
                return this->images.at(reload_image)->suf[0].surface;
            }
        }
        im_desc->imagefile = imagefile;
    
        if (reload_image < 0) {
            /* get the modification time of the file */
            struct stat statbuf;
            if (stat(imagefile.c_str(), &statbuf)==0)
                im_desc->mtime = statbuf.st_mtime;
            else
                im_desc->mtime = 0;
        }
    
        /* get surface description */
        if (imageprovider->GetSurfaceDescription(imageprovider, &surface_desc)!=DFB_OK) {
            /* release imageprovider */
            imageprovider->Release(imageprovider);
            logger.writeLog("cannot read surface desciption from image file '" + imagefile + "'");
            if (reload_image < 0) {
                delete im_desc;
                return NULL;
            }
            else {
                this->images.at(reload_image)->usecount++;
                delete im_desc;
                if (surfdesc)
                    *surfdesc = this->images.at(reload_image)->suf;
                return this->images.at(reload_image)->suf[0].surface;
            }
        }
    
        if (reload_image < 0) {
            /* create a surface */
            if (!this->layer->createSurface(&(im_desc->suf[0].surface), surface_desc.width, surface_desc.height)) { 
                /* release imageprovider */
                imageprovider->Release(imageprovider);
                logger.writeLog("cannot create surface for image file '" + imagefile + "'");
                delete im_desc;
                return NULL;
            }
            im_desc->sufcount = 1;
    
            /* render to the surface */
            if (imageprovider->RenderTo(imageprovider, im_desc->suf[0].surface->getDFBSurface(), NULL)!=DFB_OK) {
                /* release imageprovider */
                imageprovider->Release(imageprovider);
                delete im_desc->suf[0].surface;
                logger.writeLog("cannot render image file '" + imagefile + "'");
                delete im_desc;
                return NULL;
            }
        
            /* release imageprovider */
            imageprovider->Release(imageprovider);
        
            logger.writeLog("ImageManager has loaded: '" + imagefile + "'");
    
            /* add to images list and return the surface */
            im_desc->usecount = 1;
            this->images.push_back(im_desc);
            if (surfdesc)
                *surfdesc = this->images.at(this->images.size()-1)->suf;
            return im_desc->suf[0].surface; 
        }
        else {
            /* increase usecount */
            this->images.at(reload_image)->usecount++;
    
            /* check if I have to resize the surface */
            int w, h;
            this->images.at(reload_image)->suf[0].surface->getSize(&w, &h);
            if ((w != surface_desc.width) || (h != surface_desc.height))
                this->images.at(reload_image)->suf[0].surface->resize(surface_desc.width, surface_desc.height);
    
            /* render to the surface */
            if (imageprovider->RenderTo(imageprovider, this->images.at(reload_image)->suf[0].surface->getDFBSurface(), NULL)!=DFB_OK) {
                /* release imageprovider */
                imageprovider->Release(imageprovider);
                logger.writeLog("cannot render image file '" + imagefile + "'");
                delete im_desc;
                if (surfdesc)
                    *surfdesc = this->images.at(reload_image)->suf;
                return this->images.at(reload_image)->suf[0].surface;
            }
        
            /* release imageprovider */
            imageprovider->Release(imageprovider);
        
            logger.writeLog("ImageManager has reloaded: '" + imagefile + "'");
    
            /* return the surface */
            delete im_desc;
            if (surfdesc)
                *surfdesc = this->images.at(reload_image)->suf;
            return this->images.at(reload_image)->suf[0].surface; 
        }
    }
}

MMSFBSurface *MMSImageManager::newImage(const string &name, unsigned int width, unsigned int height, string pixelformat) {
//    DFBSurfaceDescription   desc;
    MMSIM_DESC              *im_desc = NULL;


    if (name != "") {
        /* search name within images list */
        for (unsigned int i = 0; i < this->images.size(); i++) {
            if (this->images.at(i)->name == name) {
                /* found, must not create a new image */
                this->images.at(i)->usecount++;
                return this->images.at(i)->suf[0].surface;
            }
        }
    }

    /* init */
    im_desc = new MMSIM_DESC;
    memset(im_desc->suf, 0, sizeof(im_desc->suf));
    im_desc->suf[0].delaytime = im_desc->suf[1].delaytime = MMSIM_DESC_SUF_END; 
    im_desc->sufcount = 0;
    im_desc->loading = false;

    im_desc->name = name;

/*    desc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT);
    desc.width = width;
    desc.height = height;
    desc.pixelformat = pixelformat;
*/
//    if (this->dfb->CreateSurface(this->dfb, &desc, &(im_desc.surface)) != DFB_OK)
    if (!this->layer->createSurface(&(im_desc->suf[0].surface), width, height, pixelformat)) 
        return NULL;
    im_desc->sufcount = 1;
    im_desc->imagefile = "";

    /* add to images list and return the surface */
    im_desc->usecount = 1;
    this->images.push_back(im_desc);
    return im_desc->suf[0].surface; 
}

void MMSImageManager::releaseImage(const string &path, const string &filename) {
    string imagefile;

    /* build filename */
    imagefile = path;
    if (imagefile != "") imagefile+= "/";
    imagefile += filename;
    if (imagefile == "")
        return;
    if (imagefile.substr(imagefile.size()-1,1)=="/")
        return;

    /* search within images list */
    for (unsigned int i = 0; i < this->images.size(); i++) {
        if (this->images.at(i)->imagefile == imagefile) {
            /* surface does exist in memory */
            this->images.at(i)->usecount--;
            if (this->images.at(i)->usecount <= 0) {
                /* this surface is not used anymore */
                logger.writeLog("ImageManager deletes: '" + this->images.at(i)->imagefile + "'");

                for (int j = 0; j < this->images.at(i)->sufcount; j++)
                    if (this->images.at(i)->suf[j].surface)
                        delete this->images.at(i)->suf[j].surface;

                delete this->images.at(i);
                this->images.erase(this->images.begin()+i);
                break;
            }
        }
    }
}

void MMSImageManager::releaseImage(MMSFBSurface *surface) {
    /* NULL? */
    if (!surface) return;

    /* search within images list */
    for (unsigned int i = 0; i < this->images.size(); i++) {
        if (this->images.at(i)->suf[0].surface == surface) {
            /* surface does exist in memory */
            this->images.at(i)->usecount--;
            if (this->images.at(i)->usecount <= 0) {
                /* this surface is not used anymore */
                logger.writeLog("ImageManager deletes: '" + this->images.at(i)->imagefile + "'");

                for (int j = 0; j < this->images.at(i)->sufcount; j++)
                    if (this->images.at(i)->suf[j].surface)
                        delete this->images.at(i)->suf[j].surface;

                delete this->images.at(i);
                this->images.erase(this->images.begin()+i);
                break;
            }
        }
    }
}

