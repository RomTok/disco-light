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

#include "mmsgui/mmscheckboxwidget.h"

MMSCheckBoxWidget::MMSCheckBoxWidget(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget() {
    create(root, className, theme);
}

MMSCheckBoxWidget::~MMSCheckBoxWidget() {
    // delete images
    if (this->rootwindow) {
        this->rootwindow->im->releaseImage(this->checked_bgimage);
        this->rootwindow->im->releaseImage(this->checked_selbgimage);
        this->rootwindow->im->releaseImage(this->checked_bgimage_p);
        this->rootwindow->im->releaseImage(this->checked_selbgimage_p);
        this->rootwindow->im->releaseImage(this->checked_bgimage_i);
        this->rootwindow->im->releaseImage(this->checked_selbgimage_i);
    }
}

bool MMSCheckBoxWidget::create(MMSWindow *root, string className, MMSTheme *theme) {
	this->type = MMSWIDGETTYPE_CHECKBOX;
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->checkBoxWidgetClass = this->theme->getCheckBoxWidgetClass(className);
    this->baseWidgetClass = &(this->theme->checkBoxWidgetClass.widgetClass);
    if (this->checkBoxWidgetClass) this->widgetClass = &(this->checkBoxWidgetClass->widgetClass); else this->widgetClass = NULL;

    /* clear */
    this->checked_bgimage = NULL;
    this->checked_selbgimage = NULL;
    this->checked_bgimage_p = NULL;
    this->checked_selbgimage_p = NULL;
    this->checked_bgimage_i = NULL;
    this->checked_selbgimage_i = NULL;

    return MMSWidget::create(root, true, false, true, true, false, false, true);
}

void MMSCheckBoxWidget::handleInput(MMSInputEvent *inputevent) {

	// switch the check state
	if (inputevent->type == MMSINPUTEVENTTYPE_KEYPRESS) {
		if (inputevent->key == MMSKEY_SPACE) {
			// toggle the checked status and return
			bool c = false;
			getChecked(c);
			setChecked(!c);
			return;
		}
	}
	else
	if (inputevent->type == MMSINPUTEVENTTYPE_BUTTONRELEASE) {
		if (isPressed()) {
			// toggle the checked status with no refresh, because the base method will refresh the widget
			bool c = false;
			getChecked(c);
			setChecked(!c, false);
		}
	}

	// call base method
	MMSWidget::handleInput(inputevent);
}

MMSWidget *MMSCheckBoxWidget::copyWidget() {
    /* create widget */
    MMSCheckBoxWidget *newWidget = new MMSCheckBoxWidget(this->rootwindow, className);

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    /* reload my images */
    newWidget->checked_bgimage = NULL;
    newWidget->checked_selbgimage = NULL;
    newWidget->checked_bgimage_p = NULL;
    newWidget->checked_selbgimage_p = NULL;
    newWidget->checked_bgimage_i = NULL;
    newWidget->checked_selbgimage_i = NULL;

	if (this->rootwindow) {
		string path, name;

		if (!newWidget->getCheckedBgImagePath(path)) path = "";
		if (!newWidget->getCheckedBgImageName(name)) name = "";
		newWidget->checked_bgimage = this->rootwindow->im->getImage(path, name);

		if (!newWidget->getCheckedSelBgImagePath(path)) path = "";
		if (!newWidget->getCheckedSelBgImageName(name)) name = "";
		newWidget->checked_selbgimage = this->rootwindow->im->getImage(path, name);

		if (!newWidget->getCheckedBgImagePath_p(path)) path = "";
		if (!newWidget->getCheckedBgImageName_p(name)) name = "";
		newWidget->checked_bgimage_p = this->rootwindow->im->getImage(path, name);

		if (!newWidget->getCheckedSelBgImagePath_p(path)) path = "";
		if (!newWidget->getCheckedSelBgImageName_p(name)) name = "";
		newWidget->checked_selbgimage_p = this->rootwindow->im->getImage(path, name);

		if (!newWidget->getCheckedBgImagePath_i(path)) path = "";
		if (!newWidget->getCheckedBgImageName_i(name)) name = "";
		newWidget->checked_bgimage_i = this->rootwindow->im->getImage(path, name);

		if (!newWidget->getCheckedSelBgImagePath_i(path)) path = "";
		if (!newWidget->getCheckedSelBgImageName_i(name)) name = "";
		newWidget->checked_selbgimage_i = this->rootwindow->im->getImage(path, name);
	}

    return newWidget;
}

bool MMSCheckBoxWidget::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

	if (this->rootwindow) {
		string path, name;

		if (!getCheckedBgImagePath(path)) path = "";
		if (!getCheckedBgImageName(name)) name = "";
		this->checked_bgimage = this->rootwindow->im->getImage(path, name);

		if (!getCheckedSelBgImagePath(path)) path = "";
		if (!getCheckedSelBgImageName(name)) name = "";
		this->checked_selbgimage = this->rootwindow->im->getImage(path, name);

		if (!getCheckedBgImagePath_p(path)) path = "";
		if (!getCheckedBgImageName_p(name)) name = "";
		this->checked_bgimage_p = this->rootwindow->im->getImage(path, name);

		if (!getCheckedSelBgImagePath_p(path)) path = "";
		if (!getCheckedSelBgImageName_p(name)) name = "";
		this->checked_selbgimage_p = this->rootwindow->im->getImage(path, name);

		if (!getCheckedBgImagePath_i(path)) path = "";
		if (!getCheckedBgImageName_i(name)) name = "";
		this->checked_bgimage_i = this->rootwindow->im->getImage(path, name);

		if (!getCheckedSelBgImagePath_i(path)) path = "";
		if (!getCheckedSelBgImageName_i(name)) name = "";
		this->checked_selbgimage_i = this->rootwindow->im->getImage(path, name);
	}

    return true;
}


bool MMSCheckBoxWidget::draw(bool *backgroundFilled) {
    bool myBackgroundFilled = false;
    bool         retry = false;

    if (!this->initialized) {
        /* init widget (e.g. load images, fonts, ...) */
        init();
        this->initialized = true;
    }

    if (backgroundFilled) {
    	if (this->has_own_surface)
    		*backgroundFilled = false;
    }
    else
        backgroundFilled = &myBackgroundFilled;


    if ((!this->geomset)||(!this->visible))
        return false;

    // is checked?
    bool c = false;
    getChecked(c);

    if (!c) {
    	// not checked, let base widget draw
        MMSWidget::draw(backgroundFilled);
    }
    else {
    	// checked, have to draw checked state

    	// lock
		if (this->surface) this->surface->lock();

		// draw background
		do {
			/* searching for the background color or image */
			MMSFBColor col;
			MMSFBSurface *suf = NULL;
			col.a = 0;

			if (isActivated()) {
				if (isSelected()) {
					getCheckedSelBgColor(col);
					suf = this->checked_selbgimage;
				}
				else {
					getCheckedBgColor(col);
					suf = this->checked_bgimage;
				}
				if (isPressed()) {
					MMSFBColor mycol;
					if (isSelected()) {
						getCheckedSelBgColor_p(mycol);
						if (mycol.a>0) col=mycol;
						if (this->checked_selbgimage_p)
							suf = this->checked_selbgimage_p;
					}
					else {
						getCheckedBgColor_p(mycol);
						if (mycol.a>0) col=mycol;
						if (this->checked_bgimage_p)
							suf = this->checked_bgimage_p;
					}
				}
			}
			else {
				if (isSelected()) {
					getCheckedSelBgColor_i(col);
					suf = this->checked_selbgimage_i;
				}
				else {
					getCheckedBgColor_i(col);
					suf = this->checked_bgimage_i;
				}
			}

			if (suf) {
				if ((*backgroundFilled)||(retry)||(!this->has_own_surface)) {
					// prepare for blitting
					this->surface->setBlittingFlagsByBrightnessAlphaAndOpacity(brightness, (col.a)?col.a:255, opacity);

					// fill background
					surface->stretchBlit(suf, NULL, &surfaceGeom);
					*backgroundFilled = true;

					// go out of the loop
					break;
				}
				else
					// the color has an alpha value and I need a background before drawing the image
					retry = true;
			}
			else
			if (col.a) {
				if ((*backgroundFilled)||(retry)||(!this->has_own_surface)||((col.a==255)&&(opacity==255))) {
					// prepare for drawing
					this->surface->setDrawingColorAndFlagsByBrightnessAndOpacity(col, brightness, opacity);

					// fill background
					this->surface->fillRectangle(surfaceGeom.x, surfaceGeom.y, surfaceGeom.w, surfaceGeom.h);
					*backgroundFilled = true;

					// go out of the loop
					break;
				}
				else
					// the color has an alpha value and I need a background for it
					retry = true;
			}
			else {
				if ((*backgroundFilled)||(!this->has_own_surface)) {
					// go out of the loop
					if (!*backgroundFilled) {
						if (this->surface) {
							// have no background, clear it
							// this is in case of if I have no own surface
							this->surface->clear();
							*backgroundFilled = true;
						}
					}
					break;
				}
				else
					// no color, no image, I need to search for a background
					retry = true;
			}

			// if not filled then
			if (!*backgroundFilled) {
				// searching for the next parent widget
				MMSWidget *widget = NULL;
				vector<MMSWidget*> wlist;
				if (this->parent)
					widget = this->parent->getDrawableParent(false, false, false, &wlist);

				// the widget found can be to far away from this widget
				// if wlist is filled, i can search for a better parent which has already a own surface
				for (unsigned int i=0; i < wlist.size(); i++) {
					MMSWidget *w = wlist.at(i);
					if ((w->drawable)&&(w->geomset)&&(w->visible)) {
						widget = w;
						break;
					}
				}

				// clear it (complete transparent)
				this->surface->clear();

				if (widget) {
					// drawable parent found, calculate rectangle to copy
					MMSFBRectangle srcrect = widget->getVisibleSurfaceArea();
					srcrect.x+= this->innerGeom.x - widget->innerGeom.x;
					srcrect.y+= this->innerGeom.y - widget->innerGeom.y;
					srcrect.w = this->innerGeom.w;
					srcrect.h = this->innerGeom.h;

					// copy background from parent
					this->surface->setBlittingFlags(MMSFB_BLIT_NOFX);
					this->surface->blit(widget->surface, &srcrect, 0, 0);
				}
				else {
					// no parent found, use background from window
					if (this->rootwindow) {
						MMSFBColor bgcolor;
						this->rootwindow->getBgColor(bgcolor);
						if (!this->rootwindow->bgimage) {
							// draw background with window bgcolor
							if (bgcolor.a) {
								// clear surface
								this->surface->clear(bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
							}
						}
						else {
							// draw background with a part of window bgimage
							MMSFBRectangle src, dst;
							int sw, sh;

							// get width and height of windows background image
							this->rootwindow->bgimage->getSize(&sw, &sh);

							// calculate with window width and height
							int f1 = (this->rootwindow->innerGeom.w * 10000) / sw;
							int f2 = (this->rootwindow->innerGeom.h * 10000) / sh;

							// calculate the source rectangle
							src.x = (5000 + 10000 *(this->innerGeom.x - this->rootwindow->innerGeom.x)) / f1;
							src.w = (5000 + 10000 * this->innerGeom.w) / f1;
							src.y = (5000 + 10000 *(this->innerGeom.y - this->rootwindow->innerGeom.y)) / f2;
							src.h = (5000 + 10000 * this->innerGeom.h) / f2;

							// the destination rectangle
							dst.x = 0;
							dst.w = this->innerGeom.w;
							dst.y = 0;
							dst.h = this->innerGeom.h;

							// copy background from window's bgimage
							this->surface->setBlittingFlagsByBrightnessAlphaAndOpacity(255, (bgcolor.a)?bgcolor.a:255, 255);
							this->surface->stretchBlit(this->rootwindow->bgimage, &src, &dst);
						}
					}
				}

				*backgroundFilled = true;
			}
		} while (retry);

		// unlock
		if (this->surface) this->surface->unlock();
    }

    // draw widgets debug frame
    return MMSWidget::drawDebug();
}



/***********************************************/
/* begin of theme access methods (get methods) */
/***********************************************/

#define GETCHECKBOX(x,y) \
    if (this->myCheckBoxWidgetClass.is##x()) return myCheckBoxWidgetClass.get##x(y); \
    else if ((checkBoxWidgetClass)&&(checkBoxWidgetClass->is##x())) return checkBoxWidgetClass->get##x(y); \
    else return this->theme->checkBoxWidgetClass.get##x(y);


bool MMSCheckBoxWidget::getCheckedBgColor(MMSFBColor &checked_bgcolor) {
    GETCHECKBOX(CheckedBgColor, checked_bgcolor);
}

bool MMSCheckBoxWidget::getCheckedSelBgColor(MMSFBColor &checked_selbgcolor) {
    GETCHECKBOX(CheckedSelBgColor, checked_selbgcolor);
}

bool MMSCheckBoxWidget::getCheckedBgColor_p(MMSFBColor &checked_bgcolor_p) {
    GETCHECKBOX(CheckedBgColor_p, checked_bgcolor_p);
}

bool MMSCheckBoxWidget::getCheckedSelBgColor_p(MMSFBColor &checked_selbgcolor_p) {
    GETCHECKBOX(CheckedSelBgColor_p, checked_selbgcolor_p);
}

bool MMSCheckBoxWidget::getCheckedBgColor_i(MMSFBColor &checked_bgcolor_i) {
    GETCHECKBOX(CheckedBgColor_i, checked_bgcolor_i);
}

bool MMSCheckBoxWidget::getCheckedSelBgColor_i(MMSFBColor &checked_selbgcolor_i) {
    GETCHECKBOX(CheckedSelBgColor_i, checked_selbgcolor_i);
}

bool MMSCheckBoxWidget::getCheckedBgImagePath(string &checked_bgimagepath) {
    GETCHECKBOX(CheckedBgImagePath, checked_bgimagepath);
}

bool MMSCheckBoxWidget::getCheckedBgImageName(string &checked_bgimagename) {
    GETCHECKBOX(CheckedBgImageName, checked_bgimagename);
}

bool MMSCheckBoxWidget::getCheckedSelBgImagePath(string &checked_selbgimagepath) {
    GETCHECKBOX(CheckedSelBgImagePath, checked_selbgimagepath);
}

bool MMSCheckBoxWidget::getCheckedSelBgImageName(string &checked_selbgimagename) {
    GETCHECKBOX(CheckedSelBgImageName, checked_selbgimagename);
}

bool MMSCheckBoxWidget::getCheckedBgImagePath_p(string &checked_bgimagepath_p) {
    GETCHECKBOX(CheckedBgImagePath_p, checked_bgimagepath_p);
}

bool MMSCheckBoxWidget::getCheckedBgImageName_p(string &checked_bgimagename_p) {
    GETCHECKBOX(CheckedBgImageName_p, checked_bgimagename_p);
}

bool MMSCheckBoxWidget::getCheckedSelBgImagePath_p(string &checked_selbgimagepath_p) {
    GETCHECKBOX(CheckedSelBgImagePath_p, checked_selbgimagepath_p);
}

bool MMSCheckBoxWidget::getCheckedSelBgImageName_p(string &checked_selbgimagename_p) {
    GETCHECKBOX(CheckedSelBgImageName_p, checked_selbgimagename_p);
}

bool MMSCheckBoxWidget::getCheckedBgImagePath_i(string &checked_bgimagepath_i) {
    GETCHECKBOX(CheckedBgImagePath_i, checked_bgimagepath_i);
}

bool MMSCheckBoxWidget::getCheckedBgImageName_i(string &checked_bgimagename_i) {
    GETCHECKBOX(CheckedBgImageName_i, checked_bgimagename_i);
}

bool MMSCheckBoxWidget::getCheckedSelBgImagePath_i(string &checked_selbgimagepath_i) {
    GETCHECKBOX(CheckedSelBgImagePath_i, checked_selbgimagepath_i);
}

bool MMSCheckBoxWidget::getCheckedSelBgImageName_i(string &checked_selbgimagename_i) {
    GETCHECKBOX(CheckedSelBgImageName_i, checked_selbgimagename_i);
}

bool MMSCheckBoxWidget::getChecked(bool &checked) {
    GETCHECKBOX(Checked, checked);
}



/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSCheckBoxWidget::setCheckedBgColor(MMSFBColor checked_bgcolor, bool refresh) {
    myCheckBoxWidgetClass.setCheckedBgColor(checked_bgcolor);
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedSelBgColor(MMSFBColor checked_selbgcolor, bool refresh) {
    myCheckBoxWidgetClass.setCheckedSelBgColor(checked_selbgcolor);
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedBgColor_p(MMSFBColor checked_bgcolor_p, bool refresh) {
    myCheckBoxWidgetClass.setCheckedBgColor_p(checked_bgcolor_p);
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedSelBgColor_p(MMSFBColor checked_selbgcolor_p, bool refresh) {
    myCheckBoxWidgetClass.setCheckedSelBgColor(checked_selbgcolor_p);
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedBgColor_i(MMSFBColor checked_bgcolor_i, bool refresh) {
    myCheckBoxWidgetClass.setCheckedBgColor_i(checked_bgcolor_i);
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedSelBgColor_i(MMSFBColor checked_selbgcolor_i, bool refresh) {
    myCheckBoxWidgetClass.setCheckedSelBgColor(checked_selbgcolor_i);
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedBgImagePath(string checked_bgimagepath, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedBgImagePath(checked_bgimagepath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_bgimage);
            string path, name;
            if (!getCheckedBgImagePath(path)) path = "";
            if (!getCheckedBgImageName(name)) name = "";
            this->checked_bgimage = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedBgImageName(string checked_bgimagename, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedBgImageName(checked_bgimagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_bgimage);
            string path, name;
            if (!getCheckedBgImagePath(path)) path = "";
            if (!getCheckedBgImageName(name)) name = "";
            this->checked_bgimage = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedSelBgImagePath(string checked_selbgimagepath, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedSelBgImagePath(checked_selbgimagepath);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_selbgimage);
            string path, name;
            if (!getCheckedSelBgImagePath(path)) path = "";
            if (!getCheckedSelBgImageName(name)) name = "";
            this->checked_selbgimage = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedSelBgImageName(string checked_selbgimagename, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedSelBgImageName(checked_selbgimagename);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_selbgimage);
            string path, name;
            if (!getCheckedSelBgImagePath(path)) path = "";
            if (!getCheckedSelBgImageName(name)) name = "";
            this->checked_selbgimage = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedBgImagePath_p(string checked_bgimagepath_p, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedBgImagePath_p(checked_bgimagepath_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_bgimage_p);
            string path, name;
            if (!getCheckedBgImagePath_p(path)) path = "";
            if (!getCheckedBgImageName_p(name)) name = "";
            this->checked_bgimage_p = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedBgImageName_p(string checked_bgimagename_p, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedBgImageName_p(checked_bgimagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_bgimage_p);
            string path, name;
            if (!getCheckedBgImagePath_p(path)) path = "";
            if (!getCheckedBgImageName_p(name)) name = "";
            this->checked_bgimage_p = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedSelBgImagePath_p(string checked_selbgimagepath_p, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedSelBgImagePath_p(checked_selbgimagepath_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_selbgimage_p);
            string path, name;
            if (!getCheckedSelBgImagePath_p(path)) path = "";
            if (!getCheckedSelBgImageName_p(name)) name = "";
            this->checked_selbgimage_p = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedSelBgImageName_p(string checked_selbgimagename_p, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedSelBgImageName_p(checked_selbgimagename_p);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_selbgimage_p);
            string path, name;
            if (!getCheckedSelBgImagePath_p(path)) path = "";
            if (!getCheckedSelBgImageName_p(name)) name = "";
            this->checked_selbgimage_p = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedBgImagePath_i(string checked_bgimagepath_i, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedBgImagePath_i(checked_bgimagepath_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_bgimage_i);
            string path, name;
            if (!getCheckedBgImagePath_i(path)) path = "";
            if (!getCheckedBgImageName_i(name)) name = "";
            this->checked_bgimage_i = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedBgImageName_i(string checked_bgimagename_i, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedBgImageName_i(checked_bgimagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_bgimage_i);
            string path, name;
            if (!getCheckedBgImagePath_i(path)) path = "";
            if (!getCheckedBgImageName_i(name)) name = "";
            this->checked_bgimage_i = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedSelBgImagePath_i(string checked_selbgimagepath_i, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedSelBgImagePath_i(checked_selbgimagepath_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_selbgimage_i);
            string path, name;
            if (!getCheckedSelBgImagePath_i(path)) path = "";
            if (!getCheckedSelBgImageName_i(name)) name = "";
            this->checked_selbgimage_i = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setCheckedSelBgImageName_i(string checked_selbgimagename_i, bool load, bool refresh) {
    myCheckBoxWidgetClass.setCheckedSelBgImageName_i(checked_selbgimagename_i);
    if (load)
        if (this->rootwindow) {
            this->rootwindow->im->releaseImage(this->checked_selbgimage_i);
            string path, name;
            if (!getCheckedSelBgImagePath_i(path)) path = "";
            if (!getCheckedSelBgImageName_i(name)) name = "";
            this->checked_selbgimage_i = this->rootwindow->im->getImage(path, name);
        }
    if (refresh)
        this->refresh();
}

void MMSCheckBoxWidget::setChecked(bool checked, bool refresh) {
	myCheckBoxWidgetClass.setChecked(checked);
	if (refresh)
        this->refresh();
}



void MMSCheckBoxWidget::updateFromThemeClass(MMSCheckBoxWidgetClass *themeClass) {

	bool 			b;
	MMSFBColor		c;
	string 			s;

    if (themeClass->getCheckedBgColor(c))
        setCheckedBgColor(c);
    if (themeClass->getCheckedSelBgColor(c))
        setCheckedSelBgColor(c);
    if (themeClass->getCheckedBgColor_p(c))
        setCheckedBgColor_p(c);
    if (themeClass->getCheckedSelBgColor_p(c))
        setCheckedSelBgColor_p(c);
    if (themeClass->getCheckedBgColor_i(c))
        setCheckedBgColor_i(c);
    if (themeClass->getCheckedSelBgColor_i(c))
        setCheckedSelBgColor_i(c);
   	if (themeClass->getCheckedBgImagePath(s))
        setCheckedBgImagePath(s);
    if (themeClass->getCheckedBgImageName(s))
        setCheckedBgImageName(s);
    if (themeClass->getCheckedSelBgImagePath(s))
        setCheckedSelBgImagePath(s);
    if (themeClass->getCheckedSelBgImageName(s))
        setCheckedSelBgImageName(s);
    if (themeClass->getCheckedBgImagePath_p(s))
        setCheckedBgImagePath_p(s);
    if (themeClass->getCheckedBgImageName_p(s))
        setCheckedBgImageName_p(s);
    if (themeClass->getCheckedSelBgImagePath_p(s))
        setCheckedSelBgImagePath_p(s);
    if (themeClass->getCheckedSelBgImageName_p(s))
        setCheckedSelBgImageName_p(s);
    if (themeClass->getCheckedBgImagePath_i(s))
        setCheckedBgImagePath_i(s);
    if (themeClass->getCheckedBgImageName_i(s))
        setCheckedBgImageName_i(s);
    if (themeClass->getCheckedSelBgImagePath_i(s))
        setCheckedSelBgImagePath_i(s);
    if (themeClass->getCheckedSelBgImageName_i(s))
        setCheckedSelBgImageName_i(s);
    if (themeClass->getChecked(b))
        setChecked(b);

    MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/