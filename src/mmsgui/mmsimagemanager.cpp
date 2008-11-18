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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "mmsgui/mmsimagemanager.h"
#include "mmsgui/mmsgifloader.h"
//#include <png.h>


MMSImageManager::MMSImageManager(MMSFBLayer *layer) {
	// save layer
	this->layer = layer;

    // get the pixelformat, create a little temp surface
	this->pixelformat = MMSFB_PF_NONE;
	MMSFBSurface *ts;
    if (this->layer->createSurface(&ts, 8, 1)) {
    	// okay, get the pixelformat from surface
    	ts->getPixelFormat(&this->pixelformat);
    	delete ts;
    }

    // use taff?
	this->usetaff = false;
    if (this->pixelformat == MMSFB_PF_ARGB) {
    	this->usetaff = true;
    	this->taffpf = MMSTAFF_PF_ARGB;
    }
    else
    if (this->pixelformat == MMSFB_PF_AiRGB) {
    	this->usetaff = true;
    	this->taffpf = MMSTAFF_PF_AiRGB;
    }
    else
    if (this->pixelformat == MMSFB_PF_AYUV) {
    	this->usetaff = true;
    	this->taffpf = MMSTAFF_PF_AYUV;
    }
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

#ifdef sfsfsfs
bool read_png(const char *filename, void **buf, int *width, int *height, bool premultiplied) {
	FILE 			*fp;
	char			png_sig[8];
    png_structp     png_ptr = NULL;
    png_infop       info_ptr = NULL;
    png_infop       end_info_ptr = NULL;
    png_bytep       *row_pointers = NULL;

    /* check if file does exist and if it is an png format */
    *buf = NULL;
    fp = fopen(filename, "rb");
    if (!fp)
    	return false;
    if (fread(png_sig, 1, sizeof(png_sig), fp)!=sizeof(png_sig)) {
        fclose(fp);
    	return false;
    }
    if (!png_check_sig((png_byte*)png_sig, sizeof(png_sig))) {
        fclose(fp);
    	return false;
    }

    /* init png structs and abend handler */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
    	return false;
    }
    png_set_sig_bytes(png_ptr, sizeof(png_sig));

    if (setjmp(png_ptr->jmpbuf)) {
    	//abend from libpng
    	DEBUGOUT("png read error\n");
    	png_destroy_read_struct(&png_ptr, (info_ptr)?&info_ptr:NULL, (end_info_ptr)?&end_info_ptr:NULL);
        if (row_pointers) free(row_pointers);
    	if (*buf) free(*buf);
        *buf = NULL;
        fclose(fp);
        return false;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
    	png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
    	return false;
    }

    end_info_ptr = png_create_info_struct(png_ptr);
    if (!end_info_ptr) {
    	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
    	return false;
    }

    /* read png infos */
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    png_uint_32 w;
    png_uint_32 h;
    int bit_depth;
    int color_type;
    png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);

    /* check the png format */
    if (((bit_depth != 8)&&(bit_depth != 16)) || (color_type != PNG_COLOR_TYPE_RGB_ALPHA)) {
    	/* we only support ARGB png images */
    	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);
        fclose(fp);
    	return false;
    }
    *width = w;
    *height = h;

    /* set input transformations */
    if (bit_depth == 16)
    	png_set_strip_16(png_ptr);
    png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
    png_set_bgr(png_ptr);
    png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    /* allocate memory for row pointers */
    row_pointers = (png_bytep*)malloc(*height * sizeof(png_bytep));
    if (!row_pointers) {
    	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);
        fclose(fp);
    	return false;
    }

    /* allocate memory for image data */
    int ww=(*width)*4;
    *buf = malloc(ww * *height);
    if (!*buf) {
    	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);
        free(row_pointers);
        fclose(fp);
    	return false;
    }
    char *b = (char*)*buf;
    for (int i = 0; i < *height; i++) {
    	row_pointers[i]=(png_byte*)b;
    	b+=ww;
    }

    /* read the image data */
    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, end_info_ptr);

    /* should i pre-multiply with alpha channel? */
    if (premultiplied) {
		unsigned int *src = (unsigned int*)*buf;
	    for (int i = *width * *height; i > 0; i--) {
	    	register unsigned int s = *src;
	        register unsigned int a = (s >> 24) + 1;
	        *src = ((((s & 0x00ff00ff) * a) >> 8) & 0x00ff00ff) |
	               ((((s & 0x0000ff00) * a) >> 8) & 0x0000ff00) |
	               ((((s & 0xff000000))));
	        src++;
	    }
    }

    /* all right */
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);
    free(row_pointers);
    fclose(fp);
	return true;
}
#endif


MMSFBSurface *MMSImageManager::getImage(const string &path, const string &filename, MMSIM_DESC_SUF **surfdesc,
										int mirror_size) {
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
            gifloader->start();
            gifloader->block();

            if (im_desc->sufcount > 0) {
            	DEBUGMSG("MMSGUI", "ImageManager has loaded: '%s'", imagefile.c_str());

                /* add to images list and return the surface */
                im_desc->usecount = 1;
                this->images.push_back(im_desc);
                if (surfdesc)
                    *surfdesc = this->images.at(this->images.size()-1)->suf;
                return im_desc->suf[0].surface;
            }
            else {
                /* failed to load */
            	DEBUGMSG("MMSGUI", "cannot load image file '%s'",imagefile.c_str());
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
        /* failed, try to read from taff? */
/*
struct  timeval tv;
gettimeofday(&tv, NULL);
DEBUGOUT("start > %d\n", tv.tv_usec);
*/



    	if (this->usetaff) {
    		// yes, try with taff
    		// assume: the taffpf (supported taff pixelformat) is correctly set
	    	// first : try to read taff image without special pixelformat
    		// second: try with pixelformat from my surfaces
    		bool retry = false;
    		do {
    			MMSTaffFile *tafff;
    			if (retry) {
	    			retry = false;
	    			DEBUGOUT("ImageManager, retry\n");
    				// have to convert taff with special destination pixelformat
    				tafff = new MMSTaffFile(imagefile + ".taff", NULL,
		    								"", MMSTAFF_EXTERNAL_TYPE_IMAGE);
        			if (tafff) {
        				// set external file and requested pixelformat
	    				tafff->setExternal(imagefile, MMSTAFF_EXTERNAL_TYPE_IMAGE);
	    				DEBUGOUT("ImageManager, taffpf = %d\n", taffpf);
	    				tafff->setDestinationPixelFormat(taffpf);
	    				tafff->setMirrorEffect(mirror_size);
	    				// convert it
	    				if (!tafff->convertExternal2TAFF()) {
	    					// conversion failed
	    					delete tafff;
	    					break;
	    				}
	    				delete tafff;
        			}
    			}

				// load image
				tafff = new MMSTaffFile(imagefile + ".taff", NULL,
	    								"", MMSTAFF_EXTERNAL_TYPE_IMAGE);
    			if (tafff) {
    				if (!tafff->isLoaded()) {
        				// set external file and mirror effect
	    				tafff->setExternal(imagefile, MMSTAFF_EXTERNAL_TYPE_IMAGE);
	    				tafff->setMirrorEffect(mirror_size);
	    				// convert it
	    				if (!tafff->convertExternal2TAFF()) {
	    					// conversion failed
	    					delete tafff;
	    					break;
	    				}
	    				delete tafff;
	    				tafff = new MMSTaffFile(imagefile + ".taff", NULL,
	    	    								"", MMSTAFF_EXTERNAL_TYPE_IMAGE);
    				}
    			}
    			if (tafff) {
		    		if (tafff->isLoaded()) {

			    		// load the attributes
		    	    	int 		attrid;
		    	    	char 		*value_str;
		    	    	int  		value_int;
				    	void 		*img_buf = NULL;
				    	int 		img_width = 0;
				    	int 		img_height= 0;
				    	int 		img_pitch = 0;
				    	int 		img_size  = 0;
				    	MMSTAFF_PF 	img_pixelformat = MMSTAFF_PF_ARGB;
				    	bool 		img_premultiplied = true;
				    	int 		img_mirror_size = 0;

				    	while ((attrid=tafff->getNextAttribute(&value_str, &value_int, NULL))>=0) {
				    		switch (attrid) {
				    		case MMSTAFF_IMAGE_RAWIMAGE_ATTR::MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_width:
				    			img_width = value_int;
				    			break;
				    		case MMSTAFF_IMAGE_RAWIMAGE_ATTR::MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_height:
				    			img_height = value_int;
				    			break;
				    		case MMSTAFF_IMAGE_RAWIMAGE_ATTR::MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_pitch:
				    			img_pitch = value_int;
				    			break;
				    		case MMSTAFF_IMAGE_RAWIMAGE_ATTR::MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_size:
				    			img_size = value_int;
				    			break;
				    		case MMSTAFF_IMAGE_RAWIMAGE_ATTR::MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_data:
				    			img_buf = value_str;
				    			break;
				    		case MMSTAFF_IMAGE_RAWIMAGE_ATTR::MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_pixelformat:
				    			img_pixelformat = (MMSTAFF_PF)value_int;
				    			break;
				    		case MMSTAFF_IMAGE_RAWIMAGE_ATTR::MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_premultiplied:
				    			img_premultiplied = (value_int);
				    			break;
				    		case MMSTAFF_IMAGE_RAWIMAGE_ATTR::MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_mirror_size:
				    			img_mirror_size = value_int;
				    			break;
				    		}
				    	}

				    	if (img_pixelformat != taffpf) {
				    		DEBUGOUT("ImageManager, taffpf = %d\n", (int)taffpf);
				    		// the image from the file has not the same pixelformat as the surface
				    		if (!retry) {
				    			// retry with surface pixelformat
				    			DEBUGOUT("ImageManager, request new pixf\n");
				    			retry = true;
				    			delete tafff;
				    			continue;
				    		}
				    		else
				    			retry = false;
				    	}
				    	else
				    	if (img_mirror_size != mirror_size) {
				    		DEBUGOUT("ImageManager, mirror_size = %d\n", (int)mirror_size);
				    		// the image from the file has not the same mirror_size
				    		if (!retry) {
				    			// retry with given mirror_size
				    			DEBUGOUT("ImageManager, request new mirrot_size\n");
				    			retry = true;
				    			delete tafff;
				    			continue;
				    		}
				    		else
				    			retry = false;
				    	}
				    	else
				    	if ((img_width)&&(img_height)&&(img_pitch)&&(img_size)&&(img_buf)) {
				        	/* successfully read */
				    		DEBUGOUT("ImageManager, use pixf = %d\n", (int)taffpf);
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
				                /* create a surface */
				                if (!this->layer->createSurface(&(im_desc->suf[0].surface), img_width, img_height, this->pixelformat)) {
				                    DEBUGMSG("MMSGUI", "cannot create surface for image file '%s'", imagefile.c_str());
				                    delete im_desc;
				                    return NULL;
				                }
				                im_desc->sufcount = 1;

				                /* copy img_buf to the surface */
				                char *suf_ptr;
				                int suf_pitch;
				                im_desc->suf[0].surface->lock(MMSFB_LOCK_WRITE, (void**)&suf_ptr, &suf_pitch);

				                if (img_pitch == suf_pitch)
				                	memcpy(suf_ptr, img_buf, img_pitch * img_height);
				                else {
				                	/* copy each line */
				                	char *img_b = (char*)img_buf;
				                	for (int i = 0; i < img_height; i++) {
				                		memcpy(suf_ptr, img_b, img_pitch);
				                		suf_ptr+=suf_pitch;
				                		img_b+=img_pitch;
				                	}
				                }
				                im_desc->suf[0].surface->unlock();

				                /* free */
				                delete tafff;
	/*
	gettimeofday(&tv, NULL);
	DEBUGOUT("end < %d\n", tv.tv_usec);

	string ss;
	im_desc->suf[0].surface->getPixelFormat(&ss);
	DEBUGOUT("png loaded: width=%d,height=%d,pitch=%d,pf=%s\n", img_width, img_height, dfbsuf_pitch,  ss.c_str());
	*/

				                DEBUGMSG("MMSGUI", "ImageManager has loaded: '%s'", imagefile.c_str());

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
				                if ((w != img_width) || (h != img_height))
				                    this->images.at(reload_image)->suf[0].surface->resize(img_width, img_height);

				                /* copy img_buf to the surface */
				                char *suf_ptr;
				                int suf_pitch;
				                im_desc->suf[0].surface->lock(MMSFB_LOCK_WRITE, (void**)&suf_ptr, &suf_pitch);

				                if (img_pitch == suf_pitch)
				                	/* copy in one block */
				                	memcpy(suf_ptr, img_buf, img_pitch * img_height);
				                else {
				                	/* copy each line */
				                	char *img_b = (char*)img_buf;
				                	for (int i = 0; i < img_height; i++) {
				                		memcpy(suf_ptr, img_b, img_pitch);
				                		suf_ptr+=suf_pitch;
				                		img_b+=img_pitch;
				                	}
				                }
				                im_desc->suf[0].surface->unlock();

				                /* free */
				                delete tafff;

				                DEBUGMSG("MMSGUI", "ImageManager has reloaded: '%s'", imagefile.c_str());

				                /* return the surface */
				                delete im_desc;
				                if (surfdesc)
				                    *surfdesc = this->images.at(reload_image)->suf;
				                return this->images.at(reload_image)->suf[0].surface;
				            }
				    	}
		    		}

		            /* free */
		            delete tafff;
		        }
    		} while (retry);
    	}


        /* failed, try it with DFB providers */
    	if (!loadImage(&imageprovider, "", imagefile)) {
        	DEBUGMSG("MMSGUI", "cannot load image file '%s'", imagefile.c_str());
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


#ifdef  __HAVE_DIRECTFB__
        /* get surface description */
        if (imageprovider->GetSurfaceDescription(imageprovider, &surface_desc)!=DFB_OK) {
            /* release imageprovider */
            imageprovider->Release(imageprovider);
            DEBUGMSG("MMSGUI", "cannot read surface desciption from image file '%s'", imagefile.c_str());
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
            if (!this->layer->createSurface(&(im_desc->suf[0].surface), surface_desc.width, surface_desc.height, this->pixelformat)) {
                /* release imageprovider */
                imageprovider->Release(imageprovider);
                DEBUGMSG("MMSGUI", "cannot create surface for image file '%s'", imagefile.c_str());
                delete im_desc;
                return NULL;
            }
            im_desc->sufcount = 1;

            // check if dfb surface available
            if (!im_desc->suf[0].surface->getDFBSurface()) {
                /* release imageprovider */
                imageprovider->Release(imageprovider);
                delete im_desc->suf[0].surface;
                DEBUGMSG("MMSGUI", "cannot render image file '%s' because it is not a DFB surface", imagefile.c_str());
                delete im_desc;
                return NULL;
            }

            /* render to the surface */
            if (imageprovider->RenderTo(imageprovider, (IDirectFBSurface *)im_desc->suf[0].surface->getDFBSurface(), NULL)!=DFB_OK) {
                /* release imageprovider */
                imageprovider->Release(imageprovider);
                delete im_desc->suf[0].surface;
                DEBUGMSG("MMSGUI", "cannot render image file '%s'", imagefile.c_str());
                delete im_desc;
                return NULL;
            }

            /* release imageprovider */
            imageprovider->Release(imageprovider);

/*
gettimeofday(&tv, NULL);
DEBUGOUT("end < %d\n", tv.tv_usec);
*/

            DEBUGMSG("MMSGUI", "ImageManager has loaded: '%s'", imagefile.c_str());

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
            if (imageprovider->RenderTo(imageprovider, (IDirectFBSurface *)this->images.at(reload_image)->suf[0].surface->getDFBSurface(), NULL)!=DFB_OK) {
                /* release imageprovider */
                imageprovider->Release(imageprovider);
                DEBUGMSG("MMSGUI", "cannot render image file '%s'", imagefile.c_str());
                delete im_desc;
                if (surfdesc)
                    *surfdesc = this->images.at(reload_image)->suf;
                return this->images.at(reload_image)->suf[0].surface;
            }

            /* release imageprovider */
            imageprovider->Release(imageprovider);

            DEBUGMSG("MMSGUI", "ImageManager has reloaded: '%s'", imagefile.c_str());

            /* return the surface */
            delete im_desc;
            if (surfdesc)
                *surfdesc = this->images.at(reload_image)->suf;
            return this->images.at(reload_image)->suf[0].surface;
        }
#endif
    }
}

MMSFBSurface *MMSImageManager::newImage(const string &name, unsigned int width, unsigned int height, MMSFBSurfacePixelFormat pixelformat) {
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
    if (!this->layer->createSurface(&(im_desc->suf[0].surface), width, height, (pixelformat==MMSFB_PF_NONE)?this->pixelformat:pixelformat))
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
            	DEBUGMSG("MMSGUI", "ImageManager deletes: '%s'", this->images.at(i)->imagefile.c_str());

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
            	DEBUGMSG("MMSGUI", "ImageManager deletes: '%s'", this->images.at(i)->imagefile.c_str());

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

