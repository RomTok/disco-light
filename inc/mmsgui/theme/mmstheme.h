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

#ifndef MMSTHEME_H_
#define MMSTHEME_H_

#include "mmsgui/theme/mmsdescriptionclass.h"
#include "mmsgui/theme/mmstemplateclass.h"
#include "mmsgui/theme/mmsmainwindowclass.h"
#include "mmsgui/theme/mmspopupwindowclass.h"
#include "mmsgui/theme/mmsrootwindowclass.h"
#include "mmsgui/theme/mmschildwindowclass.h"
#include "mmsgui/theme/mmslabelclass.h"
#include "mmsgui/theme/mmsimageclass.h"
#include "mmsgui/theme/mmsbuttonclass.h"
#include "mmsgui/theme/mmsprogressbarclass.h"
#include "mmsgui/theme/mmsmenuclass.h"
#include "mmsgui/theme/mmstextboxclass.h"
#include "mmsgui/theme/mmsarrowclass.h"
#include "mmsgui/theme/mmssliderclass.h"


MMS_CREATEERROR(MMSThemeError);


extern TAFF_DESCRIPTION mmsgui_taff_description;

typedef enum {
	MMSGUI_TAGTABLE_TAG_MMSTHEME,
	MMSGUI_TAGTABLE_TAG_MMSDIALOG,
	MMSGUI_TAGTABLE_TAG_DESCRIPTION,
	MMSGUI_TAGTABLE_TAG_VBOX,
	MMSGUI_TAGTABLE_TAG_HBOX,
	MMSGUI_TAGTABLE_TAG_TEMPLATE,
	MMSGUI_TAGTABLE_TAG_CLASS_TEMPLATE,
	MMSGUI_TAGTABLE_TAG_MAINWINDOW,
	MMSGUI_TAGTABLE_TAG_CLASS_MAINWINDOW,
	MMSGUI_TAGTABLE_TAG_CHILDWINDOW,
	MMSGUI_TAGTABLE_TAG_CLASS_CHILDWINDOW,
	MMSGUI_TAGTABLE_TAG_POPUPWINDOW,
	MMSGUI_TAGTABLE_TAG_CLASS_POPUPWINDOW,
	MMSGUI_TAGTABLE_TAG_ROOTWINDOW,
	MMSGUI_TAGTABLE_TAG_CLASS_ROOTWINDOW,
	MMSGUI_TAGTABLE_TAG_ARROW,
	MMSGUI_TAGTABLE_TAG_CLASS_ARROW,
	MMSGUI_TAGTABLE_TAG_BUTTON,
	MMSGUI_TAGTABLE_TAG_CLASS_BUTTON,
	MMSGUI_TAGTABLE_TAG_IMAGE,
	MMSGUI_TAGTABLE_TAG_CLASS_IMAGE,
	MMSGUI_TAGTABLE_TAG_LABEL,
	MMSGUI_TAGTABLE_TAG_CLASS_LABEL,
	MMSGUI_TAGTABLE_TAG_MENU,
	MMSGUI_TAGTABLE_TAG_CLASS_MENU,
	MMSGUI_TAGTABLE_TAG_MENUITEM,
	MMSGUI_TAGTABLE_TAG_PROGRESSBAR,
	MMSGUI_TAGTABLE_TAG_CLASS_PROGRESSBAR,
	MMSGUI_TAGTABLE_TAG_SLIDER,
	MMSGUI_TAGTABLE_TAG_CLASS_SLIDER,
	MMSGUI_TAGTABLE_TAG_TEXTBOX,
	MMSGUI_TAGTABLE_TAG_CLASS_TEXTBOX
};




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



