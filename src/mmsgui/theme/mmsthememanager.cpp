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

#include "theme/mmsthememanager.h"

MMSThemeManager *themeManager = NULL;

MMSThemeManager::MMSThemeManager(string themepath, string globalThemeName) {
    if (!themeManager) themeManager = this;
    this->themepath = themepath;
    
    loadGlobalTheme(globalThemeName);
}

MMSThemeManager::~MMSThemeManager() {
}

void MMSThemeManager::loadTheme(string path, string themeName, MMSTheme *theme) {
    if (themeName == "")
        return;
    
    if(path == "")
    	path = this->themepath;
    
    theme->setTheme(path, themeName);

    xmlDoc *parser = NULL;
	
	LIBXML_TEST_VERSION
	
    try {
		parser = xmlReadFile(theme->getThemeFile().c_str(), NULL, 0);

		if (!parser)
			throw new MMSThemeManagerError(1, "could not parse file");
		
        //Walk the tree:
		xmlNode* pNode = xmlDocGetRootElement(parser);
        this->throughFile(pNode, theme);

	    /* free the document */
	    xmlFreeDoc(parser);
    }
	catch(MMSError *error) {
	    /* free the document */
    	if (parser)
    		xmlFreeDoc(parser);
        throw new MMSThemeManagerError(1, "could not load theme file " + theme->getThemeFile() + ": " + error->getMessage());
	}
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

void MMSThemeManager::throughFile(xmlNode* node, MMSTheme *theme) {
//	const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
//	const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);

//	if(nodeText && nodeText->is_white_space())
//    	return;    	

    /* check if theme is given */
	if(!xmlStrcmp(node->name, (const xmlChar *) XML_ID_THEME))
        getThemeValues(node, theme);
    else {
        logger.writeLog("no valid theme file: " + theme->getThemeFile());
        return;
    }

    /* iterate through childs */
	for (xmlNode *cur_node = node->children; cur_node; cur_node = cur_node->next) {
		string name = (string)(char *)cur_node->name;
		if(name == XML_ID_DESC)
	        getDescriptionValues(cur_node, theme);
	    else if(name == XML_ID_MAINWIN)
	        getMainWindowValues(cur_node,  &(theme->mainWindowClass), theme);
	    else if(name == XML_ID_POPUPWIN)
	        getPopupWindowValues(cur_node,  &(theme->popupWindowClass), theme);
        else if(name == XML_ID_ROOTWIN)
            getRootWindowValues(cur_node,  &(theme->rootWindowClass), theme);
        else if(name == XML_ID_CHILDWIN)
            getChildWindowValues(cur_node,  &(theme->childWindowClass), theme);
        else if(name == XML_ID_LABEL)
            getLabelValues(cur_node,  &(theme->labelClass), theme);
        else if(name == XML_ID_IMAGE)
            getImageValues(cur_node,  &(theme->imageClass), theme);
	    else if(name == XML_ID_BUTTON)
            getButtonValues(cur_node,  &(theme->buttonClass), theme);
        else if(name == XML_ID_PROGRESSBAR)
            getProgressBarValues(cur_node,  &(theme->progressBarClass), theme);
        else if(name == XML_ID_MENU)
            getMenuValues(cur_node,  &(theme->menuClass), theme);
        else if(name == XML_ID_TEXTBOX)
            getTextBoxValues(cur_node,  &(theme->textBoxClass), theme);
        else if(name == XML_ID_ARROW)
            getArrowValues(cur_node,  &(theme->arrowClass), theme);

        else if(name == XML_ID_CLASS)
            getClassValues(cur_node, theme);
	}  
}

void MMSThemeManager::getThemeValues(xmlNode* node, MMSTheme *theme) {
#ifdef sdsdfs
	if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node)) {
        const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
        for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter) {
    	    const xmlpp::Attribute* attribute = *iter;
/*            if(attribute->get_name() == "name")
                theme->name = attribute->get_value();*/
        }
    }
#endif
}



void MMSThemeManager::getDescriptionValues(xmlNode* node, MMSTheme *theme) {

    theme->description.setAttributesFromXMLNode(node);
}


void  MMSThemeManager::getTemplateValues(xmlNode* node, MMSTemplateClass *themeClass) {

    themeClass->setAttributesFromXMLNode(node);

    themeClass->duplicateXMLNode(node);
}

void MMSThemeManager::getMainWindowValues(xmlNode* node, MMSMainWindowClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->windowClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->windowClass.setAttributesFromXMLNode(node, themePath);

    themeClass->setAttributesFromXMLNode(node, themePath);
}

void MMSThemeManager::getPopupWindowValues(xmlNode* node, MMSPopupWindowClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->windowClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->windowClass.setAttributesFromXMLNode(node, themePath);

    themeClass->setAttributesFromXMLNode(node, themePath);
}

void MMSThemeManager::getRootWindowValues(xmlNode* node, MMSRootWindowClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->windowClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->windowClass.setAttributesFromXMLNode(node, themePath);

    themeClass->setAttributesFromXMLNode(node, themePath);
}


void MMSThemeManager::getChildWindowValues(xmlNode* node, MMSChildWindowClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->windowClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->windowClass.setAttributesFromXMLNode(node, themePath);

    themeClass->setAttributesFromXMLNode(node, themePath);
}


void MMSThemeManager::getLabelValues(xmlNode* node, MMSLabelClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(node, "", themePath);

    themeClass->setAttributesFromXMLNode(node, "", themePath);
}

void  MMSThemeManager::getImageValues(xmlNode* node, MMSImageClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(node, "", themePath);

    themeClass->setAttributesFromXMLNode(node, "", themePath);
}


void  MMSThemeManager::getButtonValues(xmlNode* node, MMSButtonClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(node, "", themePath);

    themeClass->setAttributesFromXMLNode(node, "", themePath);
}

void  MMSThemeManager::getProgressBarValues(xmlNode* node, MMSProgressBarClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(node, "", themePath);

    themeClass->setAttributesFromXMLNode(node, "", themePath);
}

void  MMSThemeManager::getMenuValues(xmlNode* node, MMSMenuClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(node, "", themePath);

    themeClass->setAttributesFromXMLNode(node, "", themePath);

    themeClass->duplicateXMLNode(node);
}

void  MMSThemeManager::getTextBoxValues(xmlNode* node, MMSTextBoxClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(node, "", themePath);

    themeClass->setAttributesFromXMLNode(node, "", themePath);
}

void  MMSThemeManager::getArrowValues(xmlNode* node, MMSArrowClass *themeClass, MMSTheme *theme) {

    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    
    themeClass->widgetClass.border.setAttributesFromXMLNode(node, "", themePath);

    themeClass->widgetClass.setAttributesFromXMLNode(node, "", themePath);

    themeClass->setAttributesFromXMLNode(node, "", themePath);
}


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
        logger.writeLog("class misses name or type (ignoring)");
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
        logger.writeLog("invalid class type found (ignoring)");
}

void MMSThemeManager::getTemplateClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSTemplateClass *themeClass = theme->getTemplateClass(className);

    if (!themeClass) {
        themeClass = new MMSTemplateClass;
        getTemplateValues(node, themeClass);
        themeClass->setClassName(className);
        if (!theme->addTemplateClass(themeClass))
            delete themeClass;
    }
    else {
        getTemplateValues(node, themeClass);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getMainWindowClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSMainWindowClass *themeClass = theme->getMainWindowClass(className);

    if (!themeClass) {
        themeClass = new MMSMainWindowClass;
        getMainWindowValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addMainWindowClass(themeClass))
            delete themeClass;
    }
    else {
        getMainWindowValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getPopupWindowClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSPopupWindowClass *themeClass = theme->getPopupWindowClass(className);

    if (!themeClass) {
        themeClass = new MMSPopupWindowClass;
        getPopupWindowValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addPopupWindowClass(themeClass))
            delete themeClass;
    }
    else {
        getPopupWindowValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getRootWindowClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSRootWindowClass *themeClass = theme->getRootWindowClass(className);

    if (!themeClass) {
        themeClass = new MMSRootWindowClass;
        getRootWindowValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addRootWindowClass(themeClass))
            delete themeClass;
    }
    else {
        getRootWindowValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getChildWindowClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSChildWindowClass *themeClass = theme->getChildWindowClass(className);

    if (!themeClass) {
        themeClass = new MMSChildWindowClass;
        getChildWindowValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addChildWindowClass(themeClass))
            delete themeClass;
    }
    else {
        getChildWindowValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getLabelClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSLabelClass *themeClass = theme->getLabelClass(className);

    if (!themeClass) {
        themeClass = new MMSLabelClass;
        getLabelValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addLabelClass(themeClass))
            delete themeClass;
    }
    else {
        getLabelValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getImageClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSImageClass *themeClass = theme->getImageClass(className);

    if (!themeClass) {
        themeClass = new MMSImageClass;
        getImageValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addImageClass(themeClass))
            delete themeClass;
    }
    else {
        getImageValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getButtonClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSButtonClass *themeClass = theme->getButtonClass(className);

    if (!themeClass) {
        themeClass = new MMSButtonClass;
        getButtonValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addButtonClass(themeClass))
            delete themeClass;
    }
    else {
        getButtonValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getProgressBarClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSProgressBarClass *themeClass = theme->getProgressBarClass(className);

    if (!themeClass) {
        themeClass = new MMSProgressBarClass;
        getProgressBarValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addProgressBarClass(themeClass))
            delete themeClass;
    }
    else {
        getProgressBarValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getMenuClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSMenuClass *themeClass = theme->getMenuClass(className);

    if (!themeClass) {
        themeClass = new MMSMenuClass;
        getMenuValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addMenuClass(themeClass))
            delete themeClass;
    }
    else {
        getMenuValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getTextBoxClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSTextBoxClass *themeClass = theme->getTextBoxClass(className);

    if (!themeClass) {
        themeClass = new MMSTextBoxClass;
        getTextBoxValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addTextBoxClass(themeClass))
            delete themeClass;
    }
    else {
        getTextBoxValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}

void MMSThemeManager::getArrowClassValues(xmlNode *node, MMSTheme *theme, string className) {
    MMSArrowClass *themeClass = theme->getArrowClass(className);

    if (!themeClass) {
        themeClass = new MMSArrowClass;
        getArrowValues(node, themeClass, theme);
        themeClass->setClassName(className);
        if (!theme->addArrowClass(themeClass))
            delete themeClass;
    }
    else {
        getArrowValues(node, themeClass, theme);
        themeClass->setClassName(className);
    }
}




