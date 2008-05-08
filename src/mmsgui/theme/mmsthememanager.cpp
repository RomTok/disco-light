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

#include "mmsgui/theme/mmsthememanager.h"

MMSThemeManager *themeManager = NULL;

MMSThemeManager::MMSThemeManager(string themepath, string globalThemeName) {
    if (!themeManager) themeManager = this;
    this->themepath = themepath;
    
//loop von 500 dauert 21 sekunden mit xml 
//loop von 5000 dauert 3 sekunden mit mmstafffile -> verbesserung faktor 70 
//system("date");
//for (int i=0; i < 5000; i++)
	loadGlobalTheme(globalThemeName);

//system("date");
}

MMSThemeManager::~MMSThemeManager() {
}


void MMSThemeManager::loadTheme(string path, string themeName, MMSTheme *theme) {
    if (themeName == "")
        return;
    
    if(path == "")
    	path = this->themepath;
    
    theme->setTheme(path, themeName);

    /* get files */
    string themefile 	 = theme->getThemeFile();
    string themetafffile = themefile + ".taff";

    //check for file
    if(!file_exist(themefile))
        if(!file_exist(themetafffile))
        	throw new MMSThemeManagerError(1, "theme file (" + themefile + ") not found");

    /* open the taff file */
	MMSTaffFile *tafff = new MMSTaffFile(themetafffile, &mmsgui_taff_description,
										 themefile, MMSTAFF_EXTERNAL_TYPE_XML);

	if (!tafff)
        throw new MMSThemeManagerError(1, "could not load theme file " + themefile);

	if (!tafff->isLoaded()) {
		delete tafff;
        throw new MMSThemeManagerError(1, "could not load theme file " + themefile);
	}

	/* get root tag */
	int tagid = tafff->getFirstTag();
	if (tagid < 0) {
		delete tafff;
        throw new MMSThemeManagerError(1, "invalid taff file " + themetafffile);
	}

	/* through the file */
    this->throughFile(tafff, theme);

    /* free the document */
	delete tafff;
}

void MMSThemeManager::loadGlobalTheme(string themeName) {
    /* load global default theme */
    loadTheme("", DEFAULT_THEME, globalTheme);

    if (themeName != DEFAULT_THEME)
        /* overload global default theme with global theme */
        loadTheme("", themeName, globalTheme);
}

MMSTheme *MMSThemeManager::loadLocalTheme(string path, string themeName) {
    /* check if theme is already loaded */
    for (unsigned int i = 0; i < this->localThemes.size(); i++) {
        MMSTheme *theme = this->localThemes.at(i);
        if ((theme->getPath() == path)&&(theme->getThemeName() == themeName))
            /* already loaded */
            return NULL;
    }

    /* new theme */
    MMSTheme *theme = new MMSTheme(); 

    if (themeName == "") {
        /* use the name from already loaded global theme */
        themeName = globalTheme->getThemeName();
    }

    /* load global default theme */
    loadTheme("", DEFAULT_THEME, theme);

    if (themeName != DEFAULT_THEME)
        /* overload global default theme with global theme */
        loadTheme("", themeName, theme);

    /* overload global theme with local default theme */
    loadTheme(path, DEFAULT_THEME, theme);

    if (themeName != DEFAULT_THEME)
        /* overload global theme with local theme */
        loadTheme(path, themeName, theme);

    /* add theme to list */
    this->localThemes.push_back(theme);

    return theme;
}

void MMSThemeManager::deleteLocalTheme(string path, string themeName) {
    for (unsigned int i = 0; i < this->localThemes.size(); i++) {
        MMSTheme *theme = this->localThemes.at(i);
        if ((theme->getPath() == path)&&(theme->getThemeName() == themeName)) {
            delete theme;
            this->localThemes.erase(this->localThemes.begin()+i);
            break;
        }
    }
}

void MMSThemeManager::deleteLocalTheme(MMSTheme **theme) {
    for (unsigned int i = 0; i < this->localThemes.size(); i++) {
        if (this->localThemes.at(i) == *theme) {
            delete *theme;
            this->localThemes.erase(this->localThemes.begin()+i);
            *theme = NULL;
            break;
        }
    }
}


#define GET_THEME_CLASS(method) \
class_name = tafff->getAttributeString(MMSGUI_BASE_ATTR::MMSGUI_BASE_ATTR_IDS_name); \
if (class_name)	method(tafff, theme, class_name); \
else DEBUGMSG("MMSGUI", "name of class not specified (ignoring)");


void MMSThemeManager::throughFile(MMSTaffFile *tafff, MMSTheme *theme) {

	/* check if the correct tag */
	int currTag = tafff->getCurrentTag();
	if (currTag == MMSGUI_TAGTABLE_TAG_MMSTHEME)
        getThemeValues(tafff, theme);
    else {
    	DEBUGMSG("MMSGUI", "no valid theme file: %s", theme->getThemeFile().c_str());
        return;
    }

    /* iterate through childs */
    bool returntag = true;
    int depth = 0;
	while (1) {
		char *class_name;
		bool eof;
		int tid = tafff->getNextTag(eof);
		if (eof) break;
		if (tid < 0) {
			if (depth==0) break;
			depth--;
			continue;
		}
		else {
			depth++;
			if (depth>1) continue;
		}
		
		switch (tid) {
		case MMSGUI_TAGTABLE_TAG_DESCRIPTION:
			getDescriptionValues(tafff, theme);
			break;
		case MMSGUI_TAGTABLE_TAG_MAINWINDOW:
		    getMainWindowValues(tafff, &(theme->mainWindowClass), theme);
		    break;
		case MMSGUI_TAGTABLE_TAG_CHILDWINDOW:
			getChildWindowValues(tafff, &(theme->childWindowClass), theme);
			break;
		case MMSGUI_TAGTABLE_TAG_POPUPWINDOW:
		    getPopupWindowValues(tafff, &(theme->popupWindowClass), theme);
		    break;
		case MMSGUI_TAGTABLE_TAG_ROOTWINDOW:
	        getRootWindowValues(tafff, &(theme->rootWindowClass), theme);
	        break;
		case MMSGUI_TAGTABLE_TAG_LABEL:
            getLabelValues(tafff, &(theme->labelClass), theme);
            break;
		case MMSGUI_TAGTABLE_TAG_IMAGE:
			getImageValues(tafff, &(theme->imageClass), theme);
			break;
		case MMSGUI_TAGTABLE_TAG_BUTTON:
			getButtonValues(tafff, &(theme->buttonClass), theme);
			break;
		case MMSGUI_TAGTABLE_TAG_PROGRESSBAR:
            getProgressBarValues(tafff, &(theme->progressBarClass), theme);
            break;
		case MMSGUI_TAGTABLE_TAG_SLIDER:
            getSliderValues(tafff, &(theme->sliderClass), theme);
            break;
		case MMSGUI_TAGTABLE_TAG_MENU:
            getMenuValues(tafff, &(theme->menuClass), theme);
            break;
		case MMSGUI_TAGTABLE_TAG_TEXTBOX:
            getTextBoxValues(tafff, &(theme->textBoxClass), theme);
            break;
		case MMSGUI_TAGTABLE_TAG_ARROW:
            getArrowValues(tafff, &(theme->arrowClass), theme);
            break;
		case MMSGUI_TAGTABLE_TAG_CLASS_TEMPLATE:
			GET_THEME_CLASS(getTemplateClassValues);
		    break;
		case MMSGUI_TAGTABLE_TAG_CLASS_MAINWINDOW:
			GET_THEME_CLASS(getMainWindowClassValues);
		    break;
		case MMSGUI_TAGTABLE_TAG_CLASS_CHILDWINDOW:
			GET_THEME_CLASS(getChildWindowClassValues);
			break;
		case MMSGUI_TAGTABLE_TAG_CLASS_POPUPWINDOW:
			GET_THEME_CLASS(getPopupWindowClassValues);
		    break;
		case MMSGUI_TAGTABLE_TAG_CLASS_ROOTWINDOW:
			GET_THEME_CLASS(getRootWindowClassValues);
	        break;
		case MMSGUI_TAGTABLE_TAG_CLASS_LABEL:
			GET_THEME_CLASS(getLabelClassValues);
            break;
		case MMSGUI_TAGTABLE_TAG_CLASS_IMAGE:
			GET_THEME_CLASS(getImageClassValues);
			break;
		case MMSGUI_TAGTABLE_TAG_CLASS_BUTTON:
			GET_THEME_CLASS(getButtonClassValues);
			break;
		case MMSGUI_TAGTABLE_TAG_CLASS_PROGRESSBAR:
			GET_THEME_CLASS(getProgressBarClassValues);
            break;
		case MMSGUI_TAGTABLE_TAG_CLASS_SLIDER:
			GET_THEME_CLASS(getSliderClassValues);
            break;
		case MMSGUI_TAGTABLE_TAG_CLASS_MENU:
			GET_THEME_CLASS(getMenuClassValues);
            break;
		case MMSGUI_TAGTABLE_TAG_CLASS_TEXTBOX:
			GET_THEME_CLASS(getTextBoxClassValues);
            break;
		case MMSGUI_TAGTABLE_TAG_CLASS_ARROW:
			GET_THEME_CLASS(getArrowClassValues);
            break;
		}
	}
}

void MMSThemeManager::getThemeValues(MMSTaffFile *tafff, MMSTheme *theme) {
//TODO: read mmstheme attributes (e.g. theme name) here, is it required to do this?
}



void MMSThemeManager::getDescriptionValues(MMSTaffFile *tafff, MMSTheme *theme) {

    theme->description.setAttributesFromXMLNode(tafff);
}


void  MMSThemeManager::getTemplateValues(MMSTaffFile *tafff, MMSTemplateClass *themeClass) {

    themeClass->setAttributesFromXMLNode(tafff);

    themeClass->duplicateXMLNode(tafff);
}

void MMSThemeManager::getMainWindowValues(MMSTaffFile *tafff, MMSMainWindowClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->windowClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->windowClass.setAttributesFromXMLNode(tafff, themePath);

    themeClass->setAttributesFromXMLNode(tafff, themePath);
}

void MMSThemeManager::getPopupWindowValues(MMSTaffFile *tafff, MMSPopupWindowClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->windowClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->windowClass.setAttributesFromXMLNode(tafff, themePath);

    themeClass->setAttributesFromXMLNode(tafff, themePath);
}

void MMSThemeManager::getRootWindowValues(MMSTaffFile *tafff, MMSRootWindowClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->windowClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->windowClass.setAttributesFromXMLNode(tafff, themePath);

    themeClass->setAttributesFromXMLNode(tafff, themePath);
}


void MMSThemeManager::getChildWindowValues(MMSTaffFile *tafff, MMSChildWindowClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->windowClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->windowClass.setAttributesFromXMLNode(tafff, themePath);

    themeClass->setAttributesFromXMLNode(tafff, themePath);
}


void MMSThemeManager::getLabelValues(MMSTaffFile *tafff, MMSLabelClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->setAttributesFromXMLNode(tafff, "", themePath);
}

void  MMSThemeManager::getImageValues(MMSTaffFile *tafff, MMSImageClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->setAttributesFromXMLNode(tafff, "", themePath);
}


void  MMSThemeManager::getButtonValues(MMSTaffFile *tafff, MMSButtonClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->setAttributesFromXMLNode(tafff, "", themePath);
}

void  MMSThemeManager::getProgressBarValues(MMSTaffFile *tafff, MMSProgressBarClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->setAttributesFromXMLNode(tafff, "", themePath);
}

void  MMSThemeManager::getSliderValues(MMSTaffFile *tafff, MMSSliderClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->setAttributesFromXMLNode(tafff, "", themePath);
}

void  MMSThemeManager::getMenuValues(MMSTaffFile *tafff, MMSMenuClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->duplicateXMLNode(tafff);
}

void  MMSThemeManager::getTextBoxValues(MMSTaffFile *tafff, MMSTextBoxClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->setAttributesFromXMLNode(tafff, "", themePath);
}

void  MMSThemeManager::getArrowValues(MMSTaffFile *tafff, MMSArrowClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(tafff, "", themePath);

    themeClass->setAttributesFromXMLNode(tafff, "", themePath);
}

/*
void MMSThemeManager::getClassValues(xmlNode *node, MMSTheme *theme) {
    string type, name;
    xmlChar *xtype, *xname;

	xname = xmlGetProp(node, (const xmlChar*)"name");
	xtype = xmlGetProp(node, (const xmlChar*)"type");
	if (xname) {
		name = (string)(char*)xname;
	    xmlFree(xname);
	}
	if (xtype) {
		type = (string)(char*)xtype;
	    xmlFree(xtype);
	}

    if(name.empty() || type.empty()) {
    	DEBUGMSG("MMSGUI", "class misses name or type (ignoring)");
        return;
    }

    if(type == XML_ID_TEMPLATE)
        getTemplateClassValues(node, theme, name);
    else if(type == XML_ID_MAINWIN)
        getMainWindowClassValues(node, theme, name);
    else if(type == XML_ID_POPUPWIN)
        getPopupWindowClassValues(node, theme, name);
    else if(type == XML_ID_ROOTWIN)
        getRootWindowClassValues(node, theme, name);
    else if(type == XML_ID_CHILDWIN)
        getChildWindowClassValues(node, theme, name);
    else if(type == XML_ID_LABEL)
        getLabelClassValues(node, theme, name);
    else if(type == XML_ID_IMAGE)
        getImageClassValues(node, theme, name);
    else if(type == XML_ID_BUTTON)
        getButtonClassValues(node, theme, name);
    else if(type == XML_ID_PROGRESSBAR)
        getProgressBarClassValues(node, theme, name);
    else if(type == XML_ID_MENU)
        getMenuClassValues(node, theme, name);
    else if(type == XML_ID_TEXTBOX)
        getTextBoxClassValues(node, theme, name);
    else if(type == XML_ID_ARROW)
        getArrowClassValues(node, theme, name);
    else
    	DEBUGMSG("MMSGUI", "invalid class type found (ignoring)");
}
*/

void MMSThemeManager::getTemplateClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSTemplateClass *themeClass = theme->getTemplateClass(className);

    if (!themeClass) {
        themeClass = new MMSTemplateClass;
        getTemplateValues(tafff, themeClass);
        themeClass->setClassName(className);
        if (!theme->addTemplateClass(themeClass))
            delete themeClass;
    }
    else {
        getTemplateValues(tafff, themeClass);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getMainWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSMainWindowClass *themeClass = theme->getMainWindowClass(className);

    if (!themeClass) {
        themeClass = new MMSMainWindowClass;
        getMainWindowValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addMainWindowClass(themeClass))
            delete themeClass;
    }
    else {
        getMainWindowValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getPopupWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSPopupWindowClass *themeClass = theme->getPopupWindowClass(className);

    if (!themeClass) {
        themeClass = new MMSPopupWindowClass;
        getPopupWindowValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addPopupWindowClass(themeClass))
            delete themeClass;
    }
    else {
        getPopupWindowValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getRootWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSRootWindowClass *themeClass = theme->getRootWindowClass(className);

    if (!themeClass) {
        themeClass = new MMSRootWindowClass;
        getRootWindowValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addRootWindowClass(themeClass))
            delete themeClass;
    }
    else {
        getRootWindowValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getChildWindowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSChildWindowClass *themeClass = theme->getChildWindowClass(className);

    if (!themeClass) {
        themeClass = new MMSChildWindowClass;
        getChildWindowValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addChildWindowClass(themeClass))
            delete themeClass;
    }
    else {
        getChildWindowValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getLabelClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSLabelClass *themeClass = theme->getLabelClass(className);

    if (!themeClass) {
        themeClass = new MMSLabelClass;
        getLabelValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addLabelClass(themeClass))
            delete themeClass;
    }
    else {
        getLabelValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getImageClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSImageClass *themeClass = theme->getImageClass(className);

    if (!themeClass) {
        themeClass = new MMSImageClass;
        getImageValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addImageClass(themeClass))
            delete themeClass;
    }
    else {
        getImageValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getButtonClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSButtonClass *themeClass = theme->getButtonClass(className);

    if (!themeClass) {
        themeClass = new MMSButtonClass;
        getButtonValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addButtonClass(themeClass))
            delete themeClass;
    }
    else {
        getButtonValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getProgressBarClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSProgressBarClass *themeClass = theme->getProgressBarClass(className);

    if (!themeClass) {
        themeClass = new MMSProgressBarClass;
        getProgressBarValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addProgressBarClass(themeClass))
            delete themeClass;
    }
    else {
        getProgressBarValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getSliderClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSSliderClass *themeClass = theme->getSliderClass(className);

    if (!themeClass) {
        themeClass = new MMSSliderClass;
        getSliderValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addSliderClass(themeClass))
            delete themeClass;
    }
    else {
        getSliderValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getMenuClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSMenuClass *themeClass = theme->getMenuClass(className);

    if (!themeClass) {
        themeClass = new MMSMenuClass;
        getMenuValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addMenuClass(themeClass))
            delete themeClass;
    }
    else {
        getMenuValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getTextBoxClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSTextBoxClass *themeClass = theme->getTextBoxClass(className);

    if (!themeClass) {
        themeClass = new MMSTextBoxClass;
        getTextBoxValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addTextBoxClass(themeClass))
            delete themeClass;
    }
    else {
        getTextBoxValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getArrowClassValues(MMSTaffFile *tafff, MMSTheme *theme, string className) {
    MMSArrowClass *themeClass = theme->getArrowClass(className);

    if (!themeClass) {
        themeClass = new MMSArrowClass;
        getArrowValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addArrowClass(themeClass))
            delete themeClass;
    }
    else {
        getArrowValues(tafff, themeClass, theme);
        themeClass->setClassName(className);
    }
}




