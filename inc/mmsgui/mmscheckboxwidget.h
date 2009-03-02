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

#ifndef MMSCHECKBOXWIDGET_H_
#define MMSCHECKBOXWIDGET_H_

#include "mmsgui/mmswidget.h"


//! With this class you can display an on/off switch.
/*!
The checkbox is focusable.
\author Jens Schneider
*/
class MMSCheckBoxWidget : public MMSWidget {
    private:
        string          		className;
        MMSCheckBoxWidgetClass 	*checkBoxWidgetClass;
        MMSCheckBoxWidgetClass 	myCheckBoxWidgetClass;

        MMSFBSurface	*checked_bgimage;
        MMSFBSurface	*checked_selbgimage;
        MMSFBSurface	*checked_bgimage_p;
        MMSFBSurface	*checked_selbgimage_p;
        MMSFBSurface	*checked_bgimage_i;
        MMSFBSurface	*checked_selbgimage_i;

        bool create(MMSWindow *root, string className, MMSTheme *theme);
        void handleInput(MMSInputEvent *inputevent);

    public:
        MMSCheckBoxWidget(MMSWindow *root, string className, MMSTheme *theme = NULL);
        ~MMSCheckBoxWidget();

        MMSWidget *copyWidget();

        bool init();
        bool draw(bool *backgroundFilled = NULL);

    public:
        // theme access methods
        bool 	getCheckedBgColor(MMSFBColor &checked_bgcolor);
        bool 	getCheckedSelBgColor(MMSFBColor &checked_selbgcolor);
        bool	getCheckedBgColor_p(MMSFBColor &checked_bgcolor_p);
        bool	getCheckedSelBgColor_p(MMSFBColor &checked_selbgcolor_p);
        bool	getCheckedBgColor_i(MMSFBColor &checked_bgcolor_i);
        bool	getCheckedSelBgColor_i(MMSFBColor &checked_selbgcolor_i);
        bool    getCheckedBgImagePath(string &checked_bgimagepath);
        bool    getCheckedBgImageName(string &checked_bgimagename);
        bool    getCheckedSelBgImagePath(string &checked_selbgimagepath);
        bool    getCheckedSelBgImageName(string &checked_selbgimagename);
        bool    getCheckedBgImagePath_p(string &checked_bgimagepath_p);
        bool    getCheckedBgImageName_p(string &checked_bgimagename_p);
        bool    getCheckedSelBgImagePath_p(string &checked_selbgimagepath_p);
        bool    getCheckedSelBgImageName_p(string &checked_selbgimagename_p);
        bool    getCheckedBgImagePath_i(string &checked_bgimagepath_i);
        bool    getCheckedBgImageName_i(string &checked_bgimagename_i);
        bool    getCheckedSelBgImagePath_i(string &checked_selbgimagepath_i);
        bool    getCheckedSelBgImageName_i(string &checked_selbgimagename_i);
        bool 	getChecked(bool &checked);

        void setCheckedBgColor(MMSFBColor checked_bgcolor, bool refresh = true);
        void setCheckedSelBgColor(MMSFBColor checked_selbgcolor, bool refresh = true);
        void setCheckedBgColor_p(MMSFBColor checked_bgcolor_p, bool refresh = true);
        void setCheckedSelBgColor_p(MMSFBColor checked_selbgcolor_p, bool refresh = true);
        void setCheckedBgColor_i(MMSFBColor checked_bgcolor_i, bool refresh = true);
        void setCheckedSelBgColor_i(MMSFBColor checked_selbgcolor_i, bool refresh = true);
        void setCheckedBgImagePath(string checked_bgimagepath, bool load = true, bool refresh = true);
        void setCheckedBgImageName(string checked_bgimagename, bool load = true, bool refresh = true);
        void setCheckedSelBgImagePath(string checked_selbgimagepath, bool load = true, bool refresh = true);
        void setCheckedSelBgImageName(string checked_selbgimagename, bool load = true, bool refresh = true);
        void setCheckedBgImagePath_p(string checked_bgimagepath_p, bool load = true, bool refresh = true);
        void setCheckedBgImageName_p(string checked_bgimagename_p, bool load = true, bool refresh = true);
        void setCheckedSelBgImagePath_p(string checked_selbgimagepath_p, bool load = true, bool refresh = true);
        void setCheckedSelBgImageName_p(string checked_selbgimagename_p, bool load = true, bool refresh = true);
        void setCheckedBgImagePath_i(string checked_bgimagepath_i, bool load = true, bool refresh = true);
        void setCheckedBgImageName_i(string checked_bgimagename_i, bool load = true, bool refresh = true);
        void setCheckedSelBgImagePath_i(string checked_selbgimagepath_i, bool load = true, bool refresh = true);
        void setCheckedSelBgImageName_i(string checked_selbgimagename_i, bool load = true, bool refresh = true);
        void setChecked(bool checked, bool refresh = true);

        void updateFromThemeClass(MMSCheckBoxWidgetClass *themeClass);
};

#endif /*MMSCHECKBOXWIDGET_H_*/
