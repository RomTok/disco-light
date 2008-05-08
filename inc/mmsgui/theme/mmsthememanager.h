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

#ifndef MMSTHEMEMANAGER_H_
#define MMSTHEMEMANAGER_H_

#include "mmsgui/theme/mmstheme.h"

#define DEFAULT_THEME   "default"

/* string constants for xml identifiers */
#define XML_ID_THEME        "mmstheme"
#define XML_ID_CLASS        "class"


class MMSThemeManager {
    private:
        string				themepath;
        vector<MMSTheme*>   localThemes;    /* list of themes which are currently loaded (without global theme) */

//        void throughFile(xmlNode *node, MMSTheme *theme);
        void throughFile(MMSTaffFile *tafff, MMSTheme *theme);
        
        void getThemeValues(MMSTaffFile *tafff, MMSTheme *theme);
        void getDescriptionValues(MMSTaffFile *tafff, MMSTheme *theme);

        void getTemplateValues(MMSTaffFile *tafff, MMSTemplateClass *themeClass);
        void getMainWindowValues(MMSTaffFile *tafff, MMSMainWindowClass *themeClass, MMSTheme *theme);
        void getPopupWindowValues(MMSTaffFile *tafff, MMSPopupWindowClass *themeClass, MMSTheme *theme);
        void getRootWindowValues(MMSTaffFile *tafff, MMSRootWindowClass *themeClass, MMSTheme *theme);
        void getChildWindowValues(MMSTaffFile *tafff, MMSChildWindowClass *themeClass, MMSTheme *theme);
        void getLabelValues(MMSTaffFile *tafff, MMSLabelClass *themeClass, MMSTheme *theme);
        void getImageValues(MMSTaffFile *tafff, MMSImageClass *themeClass, MMSTheme *theme);
        void getButtonValues(MMSTaffFile *tafff, MMSButtonClass *themeClass, MMSTheme *theme);
        void getProgressBarValues(MMSTaffFile *tafff, MMSProgressBarClass *themeClass, MMSTheme *theme);
        void getSliderValues(MMSTaffFile *tafff, MMSSliderClass *themeClass, MMSTheme *theme);
        void getMenuValues(MMSTaffFile *tafff, MMSMenuClass *themeClass, MMSTheme *theme);
        void getTextBoxValues(MMSTaffFile *tafff, MMSTextBoxClass *themeClass, MMSTheme *theme);
        void getArrowValues(MMSTaffFile *tafff, MMSArrowClass *themeClass, MMSTheme *theme);

//        void getClassValues(MMSTaffFile *tafff, MMSTheme *theme);
        void getTemplateClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getMainWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getPopupWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getRootWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getChildWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getLabelClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getImageClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getButtonClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getProgressBarClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getSliderClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getMenuClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getTextBoxClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getArrowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);


        void loadTheme(string path, string themeName, MMSTheme *theme);

    public:
        MMSThemeManager(string themepath, string globalThemeName = DEFAULT_THEME);
        ~MMSThemeManager();

        void loadGlobalTheme(string themeName);

        MMSTheme *loadLocalTheme(string path, string themeName = "");
        void deleteLocalTheme(string path, string themeName);
        void deleteLocalTheme(MMSTheme **theme);
};

extern MMSThemeManager *themeManager;

MMS_CREATEERROR(MMSThemeManagerError);

#endif /*MMSTHEMEMANAGER_H_*/
