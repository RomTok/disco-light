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

#ifndef MMSSLIDER_H_
#define MMSSLIDER_H_

#include "mmsgui/mmswidget.h"

//! With this class you can display a slider.
/*!
The slider consists of an image which will be positioned within the slider widget area.
The slider widget cannot be focused.
\author Jens Schneider
*/
class MMSSlider : public MMSWidget {
    private:
        string         className;
        MMSSliderClass *sliderClass;
        MMSSliderClass mySliderClass;

        MMSFBSurface    *image;
        MMSFBSurface    *selimage;
        MMSFBSurface    *image_p;
        MMSFBSurface    *selimage_p;
        MMSFBSurface    *image_i;
        MMSFBSurface    *selimage_i;

        bool create(MMSWindow *root, string className, MMSTheme *theme);

    public:
        MMSSlider(MMSWindow *root, string className, MMSTheme *theme = NULL);
        ~MMSSlider();
        MMSWIDGETTYPE getType() { return MMSWIDGETTYPE_SLIDER; }

        MMSWidget *copyWidget();

        bool init();
        bool draw(bool *backgroundFilled = NULL);

    public:
        /* theme access methods */
        string getImagePath();
        string getImageName();
        string getSelImagePath();
        string getSelImageName();
        string getImagePath_p();
        string getImageName_p();
        string getSelImagePath_p();
        string getSelImageName_p();
        string getImagePath_i();
        string getImageName_i();
        string getSelImagePath_i();
        string getSelImageName_i();
        unsigned int getPosition();

        void setImagePath(string imagepath, bool load = true, bool refresh = true);
        void setImageName(string imagename, bool load = true, bool refresh = true);
        void setImage(string imagepath, string imagename, bool load = true, bool refresh = true);
        void setSelImagePath(string selimagepath, bool load = true, bool refresh = true);
        void setSelImageName(string selimagename, bool load = true, bool refresh = true);
        void setSelImage(string selimagepath, string selimagename, bool load = true, bool refresh = true);
        void setImagePath_p(string imagepath_p, bool load = true, bool refresh = true);
        void setImageName_p(string imagename_p, bool load = true, bool refresh = true);
        void setImage_p(string imagepath_p, string imagename_p, bool load = true, bool refresh = true);
        void setSelImagePath_p(string selimagepath_p, bool load = true, bool refresh = true);
        void setSelImageName_p(string selimagename_p, bool load = true, bool refresh = true);
        void setSelImage_p(string selimagepath_p, string selimagename_p, bool load = true, bool refresh = true);
        void setImagePath_i(string imagepath_i, bool load = true, bool refresh = true);
        void setImageName_i(string imagename_i, bool load = true, bool refresh = true);
        void setImage_i(string imagepath_i, string imagename_i, bool load = true, bool refresh = true);
        void setSelImagePath_i(string selimagepath_i, bool load = true, bool refresh = true);
        void setSelImageName_i(string selimagename_i, bool load = true, bool refresh = true);
        void setSelImage_i(string selimagepath_i, string selimagename_i, bool load = true, bool refresh = true);
        void setPosition(unsigned int pos, bool refresh = true);

        void updateFromThemeClass(MMSSliderClass *themeClass);
};

#endif /*MMSSLIDER_H_*/
