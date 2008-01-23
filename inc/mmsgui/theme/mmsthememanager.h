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

#ifndef MMSTHEMEMANAGER_H_
#define MMSTHEMEMANAGER_H_

#include "theme/mmstheme.h"
#include "mmsguilogger.h"

#define DEFAULT_THEME   "default"

/* string constants for xml identifiers */
#define XML_ID_THEME        "mmstheme"
#define XML_ID_CLASS        "class"


class MMSThemeManager {
    private:
        MMSGuiLogger        logger; 
        string				themepath;
        vector<MMSTheme*>   localThemes;    /* list of themes which are currently loaded (without global theme) */

        void throughFile(xmlNode* node, MMSTheme *theme);
        
        void getThemeValues(xmlNode* node, MMSTheme *theme);
        void getDescriptionValues(xmlNode* node, MMSTheme *theme);

        void getTemplateValues(xmlNode* node, MMSTemplateClass *themeClass);
        void getMainWindowValues(xmlNode* node, MMSMainWindowClass *themeClass, MMSTheme *theme);
        void getPopupWindowValues(xmlNode* node, MMSPopupWindowClass *themeClass, MMSTheme *theme);
        void getRootWindowValues(xmlNode* node, MMSRootWindowClass *themeClass, MMSTheme *theme);
        void getChildWindowValues(xmlNode* node, MMSChildWindowClass *themeClass, MMSTheme *theme);
        void getLabelValues(xmlNode* node, MMSLabelClass *themeClass, MMSTheme *theme);
        void getImageValues(xmlNode* node, MMSImageClass *themeClass, MMSTheme *theme);
        void getButtonValues(xmlNode* node, MMSButtonClass *themeClass, MMSTheme *theme);
        void getProgressBarValues(xmlNode* node, MMSProgressBarClass *themeClass, MMSTheme *theme);
        void getMenuValues(xmlNode* node, MMSMenuClass *themeClass, MMSTheme *theme);
        void getTextBoxValues(xmlNode* node, MMSTextBoxClass *themeClass, MMSTheme *theme);
        void getArrowValues(xmlNode* node, MMSArrowClass *themeClass, MMSTheme *theme);

        void getClassValues(xmlNode *node, MMSTheme *theme);
        void getTemplateClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getMainWindowClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getPopupWindowClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getRootWindowClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getChildWindowClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getLabelClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getImageClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getButtonClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getProgressBarClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getMenuClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getTextBoxClassValues(xmlNode *node, MMSTheme *theme, string className);
        void getArrowClassValues(xmlNode *node, MMSTheme *theme, string className);


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
