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
        void getLabelWidgetValues(MMSTaffFile *tafff, MMSLabelWidgetClass *themeClass, MMSTheme *theme);
        void getImageWidgetValues(MMSTaffFile *tafff, MMSImageWidgetClass *themeClass, MMSTheme *theme);
        void getButtonWidgetValues(MMSTaffFile *tafff, MMSButtonWidgetClass *themeClass, MMSTheme *theme);
        void getProgressBarWidgetValues(MMSTaffFile *tafff, MMSProgressBarWidgetClass *themeClass, MMSTheme *theme);
        void getSliderWidgetValues(MMSTaffFile *tafff, MMSSliderWidgetClass *themeClass, MMSTheme *theme);
        void getMenuWidgetValues(MMSTaffFile *tafff, MMSMenuWidgetClass *themeClass, MMSTheme *theme);
        void getTextBoxWidgetValues(MMSTaffFile *tafff, MMSTextBoxWidgetClass *themeClass, MMSTheme *theme);
        void getArrowWidgetValues(MMSTaffFile *tafff, MMSArrowWidgetClass *themeClass, MMSTheme *theme);
        void getInputWidgetValues(MMSTaffFile *tafff, MMSInputWidgetClass *themeClass, MMSTheme *theme);

//        void getClassValues(MMSTaffFile *tafff, MMSTheme *theme);
        void getTemplateClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getMainWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getPopupWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getRootWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getChildWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getLabelWidgetClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getImageWidgetClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getButtonWidgetClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getProgressBarWidgetClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getSliderWidgetClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getMenuWidgetClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getTextBoxWidgetClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getArrowWidgetClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);
        void getInputWidgetClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className);


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
