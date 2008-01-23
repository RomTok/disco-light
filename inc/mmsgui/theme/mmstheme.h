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

#ifndef MMSTHEME_H_
#define MMSTHEME_H_

#include "theme/mmsdescriptionclass.h"
#include "theme/mmstemplateclass.h"
#include "theme/mmsmainwindowclass.h"
#include "theme/mmspopupwindowclass.h"
#include "theme/mmsrootwindowclass.h"
#include "theme/mmschildwindowclass.h"
#include "theme/mmslabelclass.h"
#include "theme/mmsimageclass.h"
#include "theme/mmsbuttonclass.h"
#include "theme/mmsprogressbarclass.h"
#include "theme/mmsmenuclass.h"
#include "theme/mmstextboxclass.h"
#include "theme/mmsarrowclass.h"
#include "theme/mmssliderclass.h"


MMS_CREATEERROR(MMSThemeError);


class MMSTheme {
    private:
        string              path;       /* path to theme */
        string              themeName;  /* name of the theme e.g. "default" */
        string              themePath;  /* path to theme files */
        string              themeFile;  /* complete name of the theme file */
        MMSDescriptionClass description;/* description of the theme */

        vector<MMSTemplateClass*>       templateClasses;
        vector<MMSMainWindowClass*>     mainWindowClasses;
        vector<MMSPopupWindowClass*>    popupWindowClasses;
        vector<MMSRootWindowClass*>     rootWindowClasses;
        vector<MMSChildWindowClass*>    childWindowClasses;
        vector<MMSLabelClass*>          labelClasses;
        vector<MMSImageClass*>          imageClasses;
        vector<MMSButtonClass*>         buttonClasses;
        vector<MMSProgressBarClass*>    progressBarClasses;
        vector<MMSMenuClass*>           menuClasses;
        vector<MMSTextBoxClass*>        textBoxClasses;
        vector<MMSArrowClass*>          arrowClasses;
        vector<MMSSliderClass*>         sliderClasses;
        
        bool addTemplateClass(MMSTemplateClass *themeClass);
        bool addMainWindowClass(MMSMainWindowClass *themeClass);
        bool addPopupWindowClass(MMSPopupWindowClass *themeClass);
        bool addRootWindowClass(MMSRootWindowClass *themeClass);
        bool addChildWindowClass(MMSChildWindowClass *themeClass);
        bool addLabelClass(MMSLabelClass *themeClass);
        bool addImageClass(MMSImageClass *themeClass);
        bool addButtonClass(MMSButtonClass *themeClass);
        bool addProgressBarClass(MMSProgressBarClass *themeClass);
        bool addMenuClass(MMSMenuClass *themeClass);
        bool addTextBoxClass(MMSTextBoxClass *themeClass);
        bool addArrowClass(MMSArrowClass *themeClass);
        bool addSliderClass(MMSSliderClass *themeClass);

    public:

        MMSMainWindowClass  mainWindowClass;
        MMSPopupWindowClass popupWindowClass;
        MMSRootWindowClass  rootWindowClass;
        MMSChildWindowClass childWindowClass;
        MMSLabelClass       labelClass;
        MMSImageClass       imageClass;
        MMSButtonClass      buttonClass;
        MMSProgressBarClass progressBarClass;
        MMSMenuClass        menuClass;
        MMSTextBoxClass     textBoxClass;
        MMSArrowClass       arrowClass;
        MMSSliderClass      sliderClass;

        MMSTemplateClass*       getTemplateClass(string className = "");
        MMSMainWindowClass*     getMainWindowClass(string className = "");
        MMSPopupWindowClass*    getPopupWindowClass(string className = "");
        MMSRootWindowClass*     getRootWindowClass(string className = "");
        MMSChildWindowClass*    getChildWindowClass(string className = "");
        MMSLabelClass*          getLabelClass(string className = "");
        MMSImageClass*          getImageClass(string className = "");
        MMSButtonClass*         getButtonClass(string className = "");
        MMSProgressBarClass*    getProgressBarClass(string className = "");
        MMSMenuClass*           getMenuClass(string className = "");
        MMSTextBoxClass*        getTextBoxClass(string className = "");
        MMSArrowClass*          getArrowClass(string className = "");
        MMSSliderClass*         getSliderClass(string className = "");

        MMSTheme();
        ~MMSTheme();

        void setTheme(string path, string themeName);
        string getPath();
        string getThemeName();
        string getThemePath();
        string getThemeFile();
        
    /* friends */
    friend class MMSThemeManager;
};


/* access to global theme */
extern MMSTheme *globalTheme;

 
#endif /*MMSTHEME_H_*/



