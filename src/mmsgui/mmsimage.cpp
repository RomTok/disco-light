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

#include "mmsimage.h"


//#define __PUPTRACE__


MMSImage::MMSImage(MMSWindow *root, string className, MMSTheme *theme) {
    create(root, className, theme);
} 

MMSImage::~MMSImage() {
    if (imageThread) {
        imageThread->stop();
        imageThread=NULL;
    }

    if (this->rootwindow) {
        this->rootwindow->im->releaseImage(this->image);
        this->rootwindow->im->releaseImage(this->selimage);
        this->rootwindow->im->releaseImage(this->image_p);
        this->rootwindow->im->releaseImage(this->selimage_p);
        this->rootwindow->im->releaseImage(this->image_i);
        this->rootwindow->im->releaseImage(this->selimage_i);
    }
}

bool MMSImage::create(MMSWindow *root, string className, MMSTheme *theme) {
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->imageClass = this->theme->getImageClass(className);
    this->baseWidgetClass = &(this->theme->imageClass.widgetClass);
    if (this->imageClass) this->widgetClass = &(this->imageClass->widgetClass); else this->widgetClass = NULL;

    /* clear */    
    this->image = NULL;
    image_loaded = false;
    image_curr_index = 0;

    this->selimage = NULL;
    selimage_loaded = false;
    selimage_curr_index = 0;

    this->image_p = NULL;
    image_p_loaded = false;
    image_p_curr_index = 0;

    this->selimage_p = NULL;
    selimage_p_loaded = false;
    selimage_p_curr_index = 0;
    
    this->image_i = NULL;
    image_i_loaded = false;
    image_i_curr_index = 0;

    this->selimage_i = NULL;
    selimage_i_loaded = false;
    selimage_i_curr_index = 0;

    imageThread = NULL;

    /* create widget base */
    return MMSWidget::create(root, true, false, false, true, true, true);
}

void MMSImage::loadMyImage(string path, string filename, MMSFBSurface **surface, MMSIM_DESC_SUF **surfdesc, unsigned int *index) {
    /* pause the imageThread */
    if (this->imageThread)
        this->imageThread->pause(true);
    
    /* get image from imagemanager */
    *surface = this->rootwindow->im->getImage(path, filename, surfdesc);
    *index = 0;
    if (!*surface) {
        if (this->imageThread)
            this->imageThread->pause(false);
        return;
    }
    if (!*surfdesc) {
        if (this->imageThread)
            this->imageThread->pause(false);
        return;
    }

    /* check if I have more than one image for animation */
    if ((*surfdesc)[1].delaytime == MMSIM_DESC_SUF_END) {
        if (this->imageThread)
            this->imageThread->pause(false);
        return;
    }

    /* yes, I have something to animate */
    if (this->imageThread) {
        /* toggle pause off */
        this->imageThread->pause(false);
    }
    else {
        /* start a thread for it */
        this->imageThread = new MMSImageThread(this);
        this->imageThread->start();
    }
}

MMSWidget *MMSImage::copyWidget() {
    /* create widget */
    MMSImage *newWidget = new MMSImage(this->rootwindow, className); 

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    /* reload my images */    
    newWidget->image = NULL;
    newWidget->selimage = NULL;
    newWidget->image_p = NULL;
    newWidget->selimage_p = NULL;
    newWidget->image_i = NULL;
    newWidget->selimage_i = NULL;
    if (this->rootwindow) {
    	bool b;
    	if (!getImagesOnDemand(b))
    		b = false;
        if ((!b)||(newWidget->isVisible())) {
            loadMyImage(newWidget->getImagePath(), newWidget->getImageName(),
                        &newWidget->image, &(newWidget->image_suf), &image_curr_index);
            image_loaded = true;
            loadMyImage(newWidget->getSelImagePath(), newWidget->getSelImageName(),
                        &newWidget->selimage, &(newWidget->selimage_suf), &selimage_curr_index);
            selimage_loaded = true;

            loadMyImage(newWidget->getImagePath_p(), newWidget->getImageName_p(),
                        &newWidget->image_p, &(newWidget->image_p_suf), &image_p_curr_index);
            image_p_loaded = true;
            loadMyImage(newWidget->getSelImagePath_p(), newWidget->getSelImageName_p(),
                        &newWidget->selimage_p, &(newWidget->selimage_p_suf), &selimage_p_curr_index);
            selimage_p_loaded = true;
            
            loadMyImage(newWidget->getImagePath_i(), newWidget->getImageName_i(),
                        &newWidget->image_i, &(newWidget->image_i_suf), &image_i_curr_index);
            image_i_loaded = true;
            loadMyImage(newWidget->getSelImagePath_i(), newWidget->getSelImageName_i(),
                        &newWidget->selimage_i, &(newWidget->selimage_i_suf), &selimage_i_curr_index);
            selimage_i_loaded = true;
        }
    }

    return newWidget;
}

bool MMSImage::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

    bool b;
    if (!getImagesOnDemand(b))
    	b = false;

    if ((!b)||(this->isVisible())) {
        /* load images */    
        if (!image_loaded) {
            loadMyImage(getImagePath(), getImageName(), &this->image, &(this->image_suf), &image_curr_index);
            image_loaded = true;
        }
        if (!selimage_loaded) {
            loadMyImage(getSelImagePath(), getSelImageName(), &this->selimage, &(this->selimage_suf), &selimage_curr_index);
            selimage_loaded = true;
        }

        if (!image_p_loaded) {
            loadMyImage(getImagePath_p(), getImageName_p(), &this->image_p, &(this->image_p_suf), &image_p_curr_index);
            image_p_loaded = true;
        }
        if (!selimage_p_loaded) {
            loadMyImage(getSelImagePath_p(), getSelImageName_p(), &this->selimage_p, &(this->selimage_p_suf), &selimage_p_curr_index);
            selimage_p_loaded = true;
        }

        if (!image_i_loaded) {
            loadMyImage(getImagePath_i(), getImageName_i(), &this->image_i, &(this->image_i_suf), &image_i_curr_index);
            image_i_loaded = true;
        }
        if (!selimage_i_loaded) {
            loadMyImage(getSelImagePath_i(), getSelImageName_i(), &this->selimage_i, &(this->selimage_i_suf), &selimage_i_curr_index);
            selimage_i_loaded = true;
        }
    }

    return true;
}


void MMSImage::workWithRatio(MMSFBSurface *suf, DFBRectangle *surfaceGeom) {
    int w, h, dw, dh, ratio;

    if (getUseRatio()) {
	    /* use ratio from image */
	    suf->getSize(&w, &h);
	
	    dw = w - surfaceGeom->w;
	    dh = h - surfaceGeom->h;
	
	    if (dw || dh) {
	        ratio = (10000 * w) / h;
	        bool fw = false, fh = false;
	
	        if (getFitWidth())
	        	fw = true;
	        else
	        if (getFitHeight())
	        	fh = true;
	
	        if (((dw > dh)&&(!fh))||(fw)) {
	        	/* change image height */
	            int t = (10000 * surfaceGeom->w + 5000) / ratio;
	
	            /* work with alignment */
	            MMSALIGNMENT alignment = getAlignment();
	            if (alignment == MMSALIGNMENT_NOTSET) alignment = MMSALIGNMENT_CENTER;
	            switch (alignment) {
	                case MMSALIGNMENT_CENTER:
	                	surfaceGeom->y = (surfaceGeom->h - t) / 2;
	                    break;
	                case MMSALIGNMENT_LEFT:
	                	surfaceGeom->y = (surfaceGeom->h - t) / 2;
	                    break;
	                case MMSALIGNMENT_RIGHT:
	                	surfaceGeom->y = (surfaceGeom->h - t) / 2;
	                    break;
	                case MMSALIGNMENT_TOP_CENTER:
	                	surfaceGeom->y = 0;
	                    break;
	                case MMSALIGNMENT_TOP_LEFT:
	                	surfaceGeom->y = 0;
	                    break;
	                case MMSALIGNMENT_TOP_RIGHT:
	                	surfaceGeom->y = 0;
	                    break;
	                case MMSALIGNMENT_BOTTOM_CENTER:
	                	surfaceGeom->y = surfaceGeom->h - t;
	                    break;
	                case MMSALIGNMENT_BOTTOM_LEFT:
	                	surfaceGeom->y = surfaceGeom->h - t;
	                    break;
	                case MMSALIGNMENT_BOTTOM_RIGHT:
	                	surfaceGeom->y = surfaceGeom->h - t;
	                    break;
	                default:
	                	surfaceGeom->y = 0;
	                    break;
	            }
	            
	            surfaceGeom->h = t;
	        }
	        else {
	        	/* change image width */
	            int t = (surfaceGeom->h * ratio + 5000) / 10000;
	
	            /* work with alignment */
	            MMSALIGNMENT alignment = getAlignment();
	            if (alignment == MMSALIGNMENT_NOTSET) alignment = MMSALIGNMENT_CENTER;
	            switch (alignment) {
	                case MMSALIGNMENT_CENTER:
	    	            surfaceGeom->x = (surfaceGeom->w - t) / 2;
	                    break;
	                case MMSALIGNMENT_LEFT:
	                	surfaceGeom->x = 0;
	                    break;
	                case MMSALIGNMENT_RIGHT:
	    	            surfaceGeom->x = surfaceGeom->w - t;
	                    break;
	                case MMSALIGNMENT_TOP_CENTER:
	    	            surfaceGeom->x = (surfaceGeom->w - t) / 2;
	                    break;
	                case MMSALIGNMENT_TOP_LEFT:
	                	surfaceGeom->x = 0;
	                    break;
	                case MMSALIGNMENT_TOP_RIGHT:
	    	            surfaceGeom->x = surfaceGeom->w - t;
	                    break;
	                case MMSALIGNMENT_BOTTOM_CENTER:
	    	            surfaceGeom->x = (surfaceGeom->w - t) / 2;
	                    break;
	                case MMSALIGNMENT_BOTTOM_LEFT:
	                	surfaceGeom->x = 0;
	                    break;
	                case MMSALIGNMENT_BOTTOM_RIGHT:
	    	            surfaceGeom->x = surfaceGeom->w - t;
	                    break;
	                default:
	                	surfaceGeom->x = 0;
	                    break;
	            }
	
	            surfaceGeom->w = t;
	        }
	    }
    }
}


bool MMSImage::draw(bool *backgroundFilled) {
    bool myBackgroundFilled = false;

    if (backgroundFilled) {
    	if (this->has_own_surface)
    		*backgroundFilled = false;
    }
    else
        backgroundFilled = &myBackgroundFilled;

    /* lock */
    this->surface->lock();

    /* draw widget basics */
    if (MMSWidget::draw(backgroundFilled)) {

        /* draw my things */
        DFBRectangle surfaceGeom;

        /* get the blend value */
        unsigned int blend;
        getBlend(blend);
            
        /* searching for the image */
        MMSFBSurface *suf = NULL;
        MMSFBSurface *suf2= NULL;

        if (isActivated()) {
            if (!isPressed()) {
	            if (isSelected()) {
	                suf = (this->selimage)?this->selimage_suf[selimage_curr_index].surface:NULL;
	                suf2= (this->image)?this->image_suf[image_curr_index].surface:NULL;
	            }
	            else {
	                suf = (this->image)?this->image_suf[image_curr_index].surface:NULL;
	                suf2= (this->selimage)?this->selimage_suf[selimage_curr_index].surface:NULL;
	            }
            }
            else {
                if (isSelected()) {
                    suf = (this->selimage_p)?this->selimage_p_suf[selimage_p_curr_index].surface:NULL;
                    suf2= (this->image_p)?this->image_p_suf[image_p_curr_index].surface:NULL;
                }
                else {
                    suf = (this->image_p)?this->image_p_suf[image_p_curr_index].surface:NULL;
                    suf2= (this->selimage_p)?this->selimage_p_suf[selimage_p_curr_index].surface:NULL;
                }
            }
        }
        else {
            if (isSelected()) {
                suf = (this->selimage_i)?this->selimage_i_suf[selimage_i_curr_index].surface:NULL;
                suf2= (this->image_i)?this->image_i_suf[image_i_curr_index].surface:NULL;
            }
            else {
                suf = (this->image_i)?this->image_i_suf[image_i_curr_index].surface:NULL;
                suf2= (this->selimage_i)?this->selimage_i_suf[selimage_i_curr_index].surface:NULL;
            }
        }

		if (!blend) {
			/* blend not set */
	        if (suf) {
	            /* prepare for blitting */        
	            this->surface->setBlittingFlagsByBrightnessAlphaAndOpacity(this->brightness, 255, opacity);
	
	            /* get surface geometry */
                surfaceGeom = getSurfaceGeometry();

                /* work with aspect ratio */
	            workWithRatio(suf, &surfaceGeom);
	
	            /* normal stretchblit */
	            this->surface->stretchBlit(suf, NULL, &surfaceGeom);
	        }
		}
		else {
			/* do blend between suf and suf2 */
			if (blend > 255)
				blend=255;

			/* background image */
	        if (suf) {
	            /* prepare for blitting */
	        	double blendfactor;
	        	getBlendFactor(blendfactor);
	        	unsigned int a = (unsigned int)(255 - (blendfactor * (double)blend));
	        	if (a > 255) a = 255;
	            this->surface->setBlittingFlagsByBrightnessAlphaAndOpacity(this->brightness, a, opacity);

	            /* get surface geometry */
                surfaceGeom = getSurfaceGeometry();

                /* work with aspect ratio */
                workWithRatio(suf, &surfaceGeom);
	
	            /* normal stretchblit */
	            this->surface->stretchBlit(suf, NULL, &surfaceGeom);
	        }

	        /* foreground image which will blended */
	        if (suf2) {
	            /* prepare for blitting */        
	            this->surface->setBlittingFlagsByBrightnessAlphaAndOpacity(this->brightness, blend, opacity);
	
	            /* get surface geometry */
                surfaceGeom = getSurfaceGeometry();

                /* work with aspect ratio */
	            workWithRatio(suf2, &surfaceGeom);
	
	            /* normal stretchblit */
	            this->surface->stretchBlit(suf2, NULL, &surfaceGeom);
	        }
		}

        /* update window surface with an area of surface */
        updateWindowSurfaceWithSurface(!*backgroundFilled);
    }

    /* unlock */
    this->surface->unlock();

    /* draw widgets debug frame */
    return MMSWidget::drawDebug();
}



void MMSImage::setVisible(bool visible, bool refresh) {
	bool b;
	
    if (getImagesOnDemand(b))
    	if (b) {
	        /* load/unload on demand */
	        if (visible) {
	            /* load image on demand */
	            if (!this->isVisible()) {
	                if (!image_loaded) {
	                    loadMyImage(getImagePath(), getImageName(), &this->image, &(this->image_suf), &image_curr_index);
	                    image_loaded = true;
	                }
	                if (!selimage_loaded) {
	                    loadMyImage(getSelImagePath(), getSelImageName(), &this->selimage, &(this->selimage_suf), &selimage_curr_index);
	                    selimage_loaded = true;
	                }
	                if (!image_p_loaded) {
	                    loadMyImage(getImagePath_p(), getImageName_p(), &this->image_p, &(this->image_p_suf), &image_p_curr_index);
	                    image_p_loaded = true;
	                }
	                if (!selimage_p_loaded) {
	                    loadMyImage(getSelImagePath_p(), getSelImageName_p(), &this->selimage_p, &(this->selimage_p_suf), &selimage_p_curr_index);
	                    selimage_p_loaded = true;
	                }
	                if (!image_i_loaded) {
	                    loadMyImage(getImagePath_i(), getImageName_i(), &this->image_i, &(this->image_i_suf), &image_i_curr_index);
	                    image_i_loaded = true;
	                }
	                if (!selimage_i_loaded) {
	                    loadMyImage(getSelImagePath_i(), getSelImageName_i(), &this->selimage_i, &(this->selimage_i_suf), &selimage_i_curr_index);
	                    selimage_i_loaded = true;
	                }
	            }
	        }
	        else {
	            /* unload image on demand */
	            if (this->isVisible()) {
	                if (image_loaded) {
	                    this->rootwindow->im->releaseImage(this->image);
	                    this->image = NULL;
	                    image_loaded = false;
	                }
	                if (selimage_loaded) {
	                    this->rootwindow->im->releaseImage(this->selimage);
	                    this->selimage = NULL;
	                    selimage_loaded = false;
	                }
	                if (image_p_loaded) {
	                    this->rootwindow->im->releaseImage(this->image_p);
	                    this->image_p = NULL;
	                    image_p_loaded = false;
	                }
	                if (selimage_p_loaded) {
	                    this->rootwindow->im->releaseImage(this->selimage_p);
	                    this->selimage_p = NULL;
	                    selimage_p_loaded = false;
	                }
	                if (image_i_loaded) {
	                    this->rootwindow->im->releaseImage(this->image_i);
	                    this->image_i = NULL;
	                    image_i_loaded = false;
	                }
	                if (selimage_i_loaded) {
	                    this->rootwindow->im->releaseImage(this->selimage_i);
	                    this->selimage_i = NULL;
	                    selimage_i_loaded = false;
	                }
	            }
	        }
    	}

    /* do widget basics */
    MMSWidget::setVisible(visible, refresh);
}

/***********************************************/
/* begin of theme access methods (get methods) */
/***********************************************/

#define GETIMAGE(x) \
    if (this->myImageClass.is##x()) return myImageClass.get##x(); \
    else if ((imageClass)&&(imageClass->is##x())) return imageClass->get##x(); \
    else return this->theme->imageClass.get##x();

string MMSImage::getImagePath() {
    GETIMAGE(ImagePath);
}

string MMSImage::getImageName() {
    GETIMAGE(ImageName);
}

string MMSImage::getSelImagePath() {
    GETIMAGE(SelImagePath);
}

string MMSImage::getSelImageName() {
    GETIMAGE(SelImageName);
}

string MMSImage::getImagePath_p() {
    GETIMAGE(ImagePath_p);
}

string MMSImage::getImageName_p() {
    GETIMAGE(ImageName_p);
}

string MMSImage::getSelImagePath_p() {
    GETIMAGE(SelImagePath_p);
}

string MMSImage::getSelImageName_p() {
    GETIMAGE(SelImageName_p);
}

string MMSImage::getImagePath_i() {
    GETIMAGE(ImagePath_i);
}

string MMSImage::getImageName_i() {
    GETIMAGE(ImageName_i);
}

string MMSImage::getSelImagePath_i() {
    GETIMAGE(SelImagePath_i);
}

string MMSImage::getSelImageName_i() {
    GETIMAGE(SelImageName_i);
}

bool MMSImage::getUseRatio() {
    GETIMAGE(UseRatio);
}

bool MMSImage::getFitWidth() {
    GETIMAGE(FitWidth);
}

bool MMSImage::getFitHeight() {
    GETIMAGE(FitHeight);
}

MMSALIGNMENT MMSImage::getAlignment() {
    GETIMAGE(Alignment);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSImage::setImagePath(string imagepath, bool load, bool refresh) {
    myImageClass.setImagePath(imagepath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image);
            this->image = NULL;
            image_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getImagePath(), getImageName(), &this->image, &(this->image_suf), &image_curr_index);
                image_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setImageName(string imagename, bool load, bool refresh) {
    myImageClass.setImageName(imagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image);
            this->image = NULL;
            image_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getImagePath(), getImageName(), &this->image, &(this->image_suf), &image_curr_index);
                image_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setImage(string imagepath, string imagename, bool load, bool refresh) {
    myImageClass.setImagePath(imagepath);
    myImageClass.setImageName(imagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image);
            this->image = NULL;
            image_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getImagePath(), getImageName(), &this->image, &(this->image_suf), &image_curr_index);
                image_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setSelImagePath(string selimagepath, bool load, bool refresh) {
    myImageClass.setSelImagePath(selimagepath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage);
            this->selimage = NULL;
            selimage_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getSelImagePath(), getSelImageName(), &this->selimage, &(this->selimage_suf), &selimage_curr_index);
                selimage_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setSelImageName(string selimagename, bool load, bool refresh) {
    myImageClass.setSelImageName(selimagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage);
            this->selimage = NULL;
            selimage_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getSelImagePath(), getSelImageName(), &this->selimage, &(this->selimage_suf), &selimage_curr_index);
                selimage_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setSelImage(string selimagepath, string selimagename, bool load, bool refresh) {
    myImageClass.setSelImagePath(selimagepath);
    myImageClass.setSelImageName(selimagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage);
            this->selimage = NULL;
            selimage_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getSelImagePath(), getSelImageName(), &this->selimage, &(this->selimage_suf), &selimage_curr_index);
                selimage_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}



void MMSImage::setImagePath_p(string imagepath_p, bool load, bool refresh) {
    myImageClass.setImagePath_p(imagepath_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_p);
            this->image_p = NULL;
            image_p_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getImagePath_p(), getImageName_p(), &this->image_p, &(this->image_p_suf), &image_p_curr_index);
                image_p_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setImageName_p(string imagename_p, bool load, bool refresh) {
    myImageClass.setImageName_p(imagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_p);
            this->image_p = NULL;
            image_p_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getImagePath_p(), getImageName_p(), &this->image_p, &(this->image_p_suf), &image_p_curr_index);
                image_p_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setImage_p(string imagepath_p, string imagename_p, bool load, bool refresh) {
    myImageClass.setImagePath_p(imagepath_p);
    myImageClass.setImageName_p(imagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_p);
            this->image_p = NULL;
            image_p_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getImagePath_p(), getImageName_p(), &this->image_p, &(this->image_p_suf), &image_p_curr_index);
                image_p_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setSelImagePath_p(string selimagepath_p, bool load, bool refresh) {
    myImageClass.setSelImagePath_p(selimagepath_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_p);
            this->selimage_p = NULL;
            selimage_p_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getSelImagePath_p(), getSelImageName_p(), &this->selimage_p, &(this->selimage_p_suf), &selimage_p_curr_index);
                selimage_p_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setSelImageName_p(string selimagename_p, bool load, bool refresh) {
    myImageClass.setSelImageName_p(selimagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_p);
            this->selimage_p = NULL;
            selimage_p_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getSelImagePath_p(), getSelImageName_p(), &this->selimage_p, &(this->selimage_p_suf), &selimage_p_curr_index);
                selimage_p_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setSelImage_p(string selimagepath_p, string selimagename_p, bool load, bool refresh) {
    myImageClass.setSelImagePath_p(selimagepath_p);
    myImageClass.setSelImageName_p(selimagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_p);
            this->selimage_p = NULL;
            selimage_p_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getSelImagePath_p(), getSelImageName_p(), &this->selimage_p, &(this->selimage_p_suf), &selimage_p_curr_index);
                selimage_p_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}



void MMSImage::setImagePath_i(string imagepath_i, bool load, bool refresh) {
    myImageClass.setImagePath_i(imagepath_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_i);
            this->image_i = NULL;
            image_i_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getImagePath_i(), getImageName_i(), &this->image_i, &(this->image_i_suf), &image_i_curr_index);
                image_i_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setImageName_i(string imagename_i, bool load, bool refresh) {
    myImageClass.setImageName_i(imagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_i);
            this->image_i = NULL;
            image_i_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getImagePath_i(), getImageName_i(), &this->image_i, &(this->image_i_suf), &image_i_curr_index);
                image_i_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setImage_i(string imagepath_i, string imagename_i, bool load, bool refresh) {
    myImageClass.setImagePath_i(imagepath_i);
    myImageClass.setImageName_i(imagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_i);
            this->image_i = NULL;
            image_i_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getImagePath_i(), getImageName_i(), &this->image_i, &(this->image_i_suf), &image_i_curr_index);
                image_i_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setSelImagePath_i(string selimagepath_i, bool load, bool refresh) {
    myImageClass.setSelImagePath_i(selimagepath_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_i);
            this->selimage_i = NULL;
            selimage_i_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getSelImagePath_i(), getSelImageName_i(), &this->selimage_i, &(this->selimage_i_suf), &selimage_i_curr_index);
                selimage_i_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setSelImageName_i(string selimagename_i, bool load, bool refresh) {
    myImageClass.setSelImageName_i(selimagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_i);
            this->selimage_i = NULL;
            selimage_i_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getSelImagePath_i(), getSelImageName_i(), &this->selimage_i, &(this->selimage_i_suf), &selimage_i_curr_index);
                selimage_i_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setSelImage_i(string selimagepath_i, string selimagename_i, bool load, bool refresh) {
    myImageClass.setSelImagePath_i(selimagepath_i);
    myImageClass.setSelImageName_i(selimagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_i);
            this->selimage_i = NULL;
            selimage_i_loaded = false;
            bool b;
            if (!getImagesOnDemand(b))
            	b = false;
            if ((!b)||(this->isVisible())) {
                loadMyImage(getSelImagePath_i(), getSelImageName_i(), &this->selimage_i, &(this->selimage_i_suf), &selimage_i_curr_index);
                selimage_i_loaded = true;
            }
        }
    if (refresh)
        this->refresh();
}

void MMSImage::setUseRatio(bool useratio, bool refresh) {
    myImageClass.setUseRatio(useratio);
    if (refresh)
        this->refresh();
}

void MMSImage::setFitWidth(bool fitwidth, bool refresh) {
    myImageClass.setFitWidth(fitwidth);
    if (refresh)
        this->refresh();
}

void MMSImage::setFitHeight(bool fitheight, bool refresh) {
    myImageClass.setFitHeight(fitheight);
    if (refresh)
        this->refresh();
}

void MMSImage::setAlignment(MMSALIGNMENT alignment, bool refresh) {
    myImageClass.setAlignment(alignment);
    if (refresh)
        this->refresh();
}

void MMSImage::updateFromThemeClass(MMSImageClass *themeClass) {
    if (themeClass->isImagePath())
        setImagePath(themeClass->getImagePath());
    if (themeClass->isImageName())
        setImageName(themeClass->getImageName());
    if (themeClass->isSelImagePath())
        setSelImagePath(themeClass->getSelImagePath());
    if (themeClass->isSelImageName())
        setSelImageName(themeClass->getSelImageName());
    if (themeClass->isImagePath_p())
        setImagePath_p(themeClass->getImagePath_p());
    if (themeClass->isImageName_p())
        setImageName_p(themeClass->getImageName_p());
    if (themeClass->isSelImagePath_p())
        setSelImagePath_p(themeClass->getSelImagePath_p());
    if (themeClass->isSelImageName_p())
        setSelImageName_p(themeClass->getSelImageName_p());
    if (themeClass->isImagePath_i())
        setImagePath_i(themeClass->getImagePath_i());
    if (themeClass->isImageName_i())
        setImageName_i(themeClass->getImageName_i());
    if (themeClass->isSelImagePath_i())
        setSelImagePath_i(themeClass->getSelImagePath_i());
    if (themeClass->isSelImageName_i())
        setSelImageName_i(themeClass->getSelImageName_i());
    if (themeClass->isUseRatio())
        setUseRatio(themeClass->getUseRatio());
    if (themeClass->isFitWidth())
        setFitWidth(themeClass->getFitWidth());
    if (themeClass->isFitHeight())
        setFitHeight(themeClass->getFitHeight());
    if (themeClass->isAlignment())
        setAlignment(themeClass->getAlignment());

    MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
