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

#include "mmsgui/mmsslider.h"

MMSSlider::MMSSlider(MMSWindow *root, string className, MMSTheme *theme) {
    create(root, className, theme);
} 

MMSSlider::~MMSSlider() {
    if (this->rootwindow) {
        this->rootwindow->im->releaseImage(this->image);
        this->rootwindow->im->releaseImage(this->selimage);
        this->rootwindow->im->releaseImage(this->image_p);
        this->rootwindow->im->releaseImage(this->selimage_p);
        this->rootwindow->im->releaseImage(this->image_i);
        this->rootwindow->im->releaseImage(this->selimage_i);
    }
}

bool MMSSlider::create(MMSWindow *root, string className, MMSTheme *theme) {
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->sliderClass = this->theme->getSliderClass(className);
    this->baseWidgetClass = &(this->theme->imageClass.widgetClass);
    if (this->sliderClass) this->widgetClass = &(this->sliderClass->widgetClass); else this->widgetClass = NULL;

    /* clear */    
    this->image = NULL;
    this->selimage = NULL;
    this->image_p = NULL;
    this->selimage_p = NULL;
    this->image_i = NULL;
    this->selimage_i = NULL;

    /* create widget base */
    return MMSWidget::create(root, true, false, false, true, true, true);
}

MMSWidget *MMSSlider::copyWidget() {
    /* create widget */
    MMSSlider *newWidget = new MMSSlider(this->rootwindow, className); 

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
        newWidget->image = this->rootwindow->im->getImage(newWidget->getImagePath(), newWidget->getImageName());
        newWidget->selimage = this->rootwindow->im->getImage(newWidget->getSelImagePath(), newWidget->getSelImageName());
        newWidget->image_p = this->rootwindow->im->getImage(newWidget->getImagePath_p(), newWidget->getImageName_p());
        newWidget->selimage_p = this->rootwindow->im->getImage(newWidget->getSelImagePath_p(), newWidget->getSelImageName_p());
        newWidget->image_i = this->rootwindow->im->getImage(newWidget->getImagePath_i(), newWidget->getImageName_i());
        newWidget->selimage_i = this->rootwindow->im->getImage(newWidget->getSelImagePath_i(), newWidget->getSelImageName_i());
    }

    return newWidget;
}

bool MMSSlider::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

    /* load images */    
    this->image = this->rootwindow->im->getImage(getImagePath(), getImageName());
    this->selimage = this->rootwindow->im->getImage(getSelImagePath(), getSelImageName());
    this->image_p = this->rootwindow->im->getImage(getImagePath_p(), getImageName_p());
    this->selimage_p = this->rootwindow->im->getImage(getSelImagePath_p(), getSelImageName_p());
    this->image_i = this->rootwindow->im->getImage(getImagePath_i(), getImageName_i());
    this->selimage_i = this->rootwindow->im->getImage(getSelImagePath_i(), getSelImageName_i());

    return true;
}

bool MMSSlider::draw(bool *backgroundFilled) {
    bool myBackgroundFilled = false;

    if (backgroundFilled) {
    	if (this->has_own_surface)
    		*backgroundFilled = false;
    }
    else
        backgroundFilled = &myBackgroundFilled;

    /* draw widget basics */
    if (MMSWidget::draw(backgroundFilled)) {

        /* lock */
        this->surface->lock();

        /* draw my things */
        DFBRectangle surfaceGeom = getSurfaceGeometry();
        
        /* searching for the image */
        MMSFBSurface *suf = NULL;

        if (isActivated()) {
        	if (!isPressed()) {
	            if (isSelected())
	                suf = this->selimage;
	            else
	                suf = this->image;
        	}
        	else {
                if (isSelected())
                    suf = this->selimage_p;
                else
                    suf = this->image_p;
        	}
        }
        else {
            if (isSelected())
                suf = this->selimage_i;
            else
                suf = this->image_i;
        }

        if (suf) {
            /* prepare for blitting */        
            this->surface->setBlittingFlagsByBrightnessAlphaAndOpacity(this->brightness, 255, opacity);

            /* calculate position */
            int w, h;
            suf->getSize(&w, &h);
            if (surfaceGeom.w < w) w = surfaceGeom.w;
            if (surfaceGeom.h < h) h = surfaceGeom.h;
            if (surfaceGeom.w - w < surfaceGeom.h - h) {
                surfaceGeom.y += ((surfaceGeom.h - h) * getPosition()) / 100;
                w = surfaceGeom.w;
                surfaceGeom.h = h;
            }
            else {
                surfaceGeom.x += ((surfaceGeom.w - w) * getPosition()) / 100;
                surfaceGeom.w = w;
                h = surfaceGeom.h;
            }

            /* blit */
            this->surface->stretchBlit(suf, NULL, &surfaceGeom);
        }

        /* unlock */
        this->surface->unlock();

        /* update window surface with an area of surface */
        updateWindowSurfaceWithSurface(!*backgroundFilled);
    }

    /* draw widgets debug frame */
    return MMSWidget::drawDebug();
}

/***********************************************/
/* begin of theme access methods (get methods) */
/***********************************************/

#define GETSLIDER(x) \
    if (this->mySliderClass.is##x()) return mySliderClass.get##x(); \
    else if ((sliderClass)&&(sliderClass->is##x())) return sliderClass->get##x(); \
    else return this->theme->sliderClass.get##x();

string MMSSlider::getImagePath() {
    GETSLIDER(ImagePath);
}

string MMSSlider::getImageName() {
    GETSLIDER(ImageName);
}

string MMSSlider::getSelImagePath() {
    GETSLIDER(SelImagePath);
}

string MMSSlider::getSelImageName() {
    GETSLIDER(SelImageName);
}

string MMSSlider::getImagePath_p() {
    GETSLIDER(ImagePath_p);
}

string MMSSlider::getImageName_p() {
    GETSLIDER(ImageName_p);
}

string MMSSlider::getSelImagePath_p() {
    GETSLIDER(SelImagePath_p);
}

string MMSSlider::getSelImageName_p() {
    GETSLIDER(SelImageName_p);
}

string MMSSlider::getImagePath_i() {
    GETSLIDER(ImagePath_i);
}

string MMSSlider::getImageName_i() {
    GETSLIDER(ImageName_i);
}

string MMSSlider::getSelImagePath_i() {
    GETSLIDER(SelImagePath_i);
}

string MMSSlider::getSelImageName_i() {
    GETSLIDER(SelImageName_i);
}

unsigned int MMSSlider::getPosition() {
    GETSLIDER(Position);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSSlider::setImagePath(string imagepath, bool load, bool refresh) {
    mySliderClass.setImagePath(imagepath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image);
            this->image = this->rootwindow->im->getImage(getImagePath(), getImageName());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setImageName(string imagename, bool load, bool refresh) {
    mySliderClass.setImageName(imagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image);
            this->image = this->rootwindow->im->getImage(getImagePath(), getImageName());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setImage(string imagepath, string imagename, bool load, bool refresh) {
    mySliderClass.setImagePath(imagepath);
    mySliderClass.setImageName(imagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image);
            this->image = this->rootwindow->im->getImage(getImagePath(), getImageName());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setSelImagePath(string selimagepath, bool load, bool refresh) {
    mySliderClass.setSelImagePath(selimagepath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage);
            this->selimage = this->rootwindow->im->getImage(getSelImagePath(), getSelImageName());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setSelImageName(string selimagename, bool load, bool refresh) {
    mySliderClass.setSelImageName(selimagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage);
            this->selimage = this->rootwindow->im->getImage(getSelImagePath(), getSelImageName());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setSelImage(string selimagepath, string selimagename, bool load, bool refresh) {
    mySliderClass.setSelImagePath(selimagepath);
    mySliderClass.setSelImageName(selimagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage);
            this->selimage = this->rootwindow->im->getImage(getSelImagePath(), getSelImageName());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setImagePath_p(string imagepath_p, bool load, bool refresh) {
    mySliderClass.setImagePath_p(imagepath_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_p);
            this->image_p = this->rootwindow->im->getImage(getImagePath_p(), getImageName_p());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setImageName_p(string imagename_p, bool load, bool refresh) {
    mySliderClass.setImageName_p(imagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_p);
            this->image_p = this->rootwindow->im->getImage(getImagePath_p(), getImageName_p());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setImage_p(string imagepath_p, string imagename_p, bool load, bool refresh) {
    mySliderClass.setImagePath_p(imagepath_p);
    mySliderClass.setImageName_p(imagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_p);
            this->image_p = this->rootwindow->im->getImage(getImagePath_p(), getImageName_p());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setSelImagePath_p(string selimagepath_p, bool load, bool refresh) {
    mySliderClass.setSelImagePath_p(selimagepath_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_p);
            this->selimage_p = this->rootwindow->im->getImage(getSelImagePath_p(), getSelImageName_p());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setSelImageName_p(string selimagename_p, bool load, bool refresh) {
    mySliderClass.setSelImageName_p(selimagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_p);
            this->selimage_p = this->rootwindow->im->getImage(getSelImagePath_p(), getSelImageName_p());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setSelImage_p(string selimagepath_p, string selimagename_p, bool load, bool refresh) {
    mySliderClass.setSelImagePath_p(selimagepath_p);
    mySliderClass.setSelImageName_p(selimagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_p);
            this->selimage_p = this->rootwindow->im->getImage(getSelImagePath_p(), getSelImageName_p());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setImagePath_i(string imagepath_i, bool load, bool refresh) {
    mySliderClass.setImagePath_i(imagepath_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_i);
            this->image_i = this->rootwindow->im->getImage(getImagePath_i(), getImageName_i());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setImageName_i(string imagename_i, bool load, bool refresh) {
    mySliderClass.setImageName_i(imagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_i);
            this->image_i = this->rootwindow->im->getImage(getImagePath_i(), getImageName_i());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setImage_i(string imagepath_i, string imagename_i, bool load, bool refresh) {
    mySliderClass.setImagePath_i(imagepath_i);
    mySliderClass.setImageName_i(imagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->image_i);
            this->image_i = this->rootwindow->im->getImage(getImagePath_i(), getImageName_i());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setSelImagePath_i(string selimagepath_i, bool load, bool refresh) {
    mySliderClass.setSelImagePath_i(selimagepath_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_i);
            this->selimage_i = this->rootwindow->im->getImage(getSelImagePath_i(), getSelImageName_i());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setSelImageName_i(string selimagename_i, bool load, bool refresh) {
    mySliderClass.setSelImageName_i(selimagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_i);
            this->selimage_i = this->rootwindow->im->getImage(getSelImagePath_i(), getSelImageName_i());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setSelImage_i(string selimagepath_i, string selimagename_i, bool load, bool refresh) {
    mySliderClass.setSelImagePath_i(selimagepath_i);
    mySliderClass.setSelImageName_i(selimagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->selimage_i);
            this->selimage_i = this->rootwindow->im->getImage(getSelImagePath_i(), getSelImageName_i());
        }
    if (refresh)
        this->refresh();
}

void MMSSlider::setPosition(unsigned int pos, bool refresh) {
    mySliderClass.setPosition(pos);
    if (refresh)
        this->refresh();
}

void MMSSlider::updateFromThemeClass(MMSSliderClass *themeClass) {
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
    if (themeClass->isPosition())
        setPosition(themeClass->getPosition());

    MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/
