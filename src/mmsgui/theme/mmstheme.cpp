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

#include "mmsgui/theme/mmstheme.h"

/* initialize the theme object which stores the global theme */
MMSTheme *globalTheme = new MMSTheme();


TAFF_ATTRDESC MMSGUI_MMSTHEME_ATTR_I[]			= MMSGUI_MMSTHEME_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_MMSDIALOG_ATTR_I[]			= MMSGUI_MMSDIALOG_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_BASE_ATTR_I[]				= MMSGUI_BASE_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_NONE_ATTR_I[]              = { NULL, TAFF_ATTRTYPE_NONE };


TAFF_TAGTABLE mmsgui_taff_tagtable[] = {
	{	"mmstheme",		NULL, 	NULL,			MMSGUI_MMSTHEME_ATTR_I			},
	{	"mmsdialog",	NULL, 	NULL,			MMSGUI_MMSDIALOG_ATTR_I			},
	{	"description",	NULL, 	NULL,			MMSGUI_DESCRIPTION_ATTR_I		},
	{	"vbox",			NULL, 	NULL,			MMSGUI_BASE_ATTR_I				},
	{	"hbox",			NULL, 	NULL,			MMSGUI_BASE_ATTR_I				},
	{	"template", 	NULL,	NULL,			MMSGUI_TEMPLATE_ATTR_I			},
	{	"class", 		"type",	"template",		MMSGUI_TEMPLATE_ATTR_I			},
	{	"mainwindow",	NULL, 	NULL,			MMSGUI_MAINWINDOW_ATTR_I		},
	{	"class", 		"type",	"mainwindow",	MMSGUI_MAINWINDOW_ATTR_I		},
	{	"childwindow",	NULL, 	NULL,			MMSGUI_CHILDWINDOW_ATTR_I		},
	{	"class", 		"type",	"childwindow",	MMSGUI_CHILDWINDOW_ATTR_I		},
	{	"popupwindow",	NULL, 	NULL,			MMSGUI_POPUPWINDOW_ATTR_I		},
	{	"class", 		"type",	"popupwindow", 	MMSGUI_POPUPWINDOW_ATTR_I		},
	{	"rootwindow",	NULL, 	NULL,			MMSGUI_ROOTWINDOW_ATTR_I		},
	{	"class", 		"type",	"rootwindow",	MMSGUI_ROOTWINDOW_ATTR_I		},
	{	"arrow", 		NULL, 	NULL,			MMSGUI_ARROW_ATTR_I				},
	{	"class", 		"type",	"arrow",		MMSGUI_ARROW_ATTR_I				},
	{	"button", 		NULL, 	NULL,			MMSGUI_BUTTON_ATTR_I			},
	{	"class", 		"type",	"button",		MMSGUI_BUTTON_ATTR_I			},
	{	"image", 		NULL, 	NULL,			MMSGUI_IMAGE_ATTR_I				},
	{	"class", 		"type",	"image",		MMSGUI_IMAGE_ATTR_I				},
	{	"label", 		NULL, 	NULL,			MMSGUI_LABEL_ATTR_I				},
	{	"class", 		"type",	"label",		MMSGUI_LABEL_ATTR_I				},
	{	"menu", 		NULL, 	NULL,			MMSGUI_MENU_ATTR_I				},
	{	"class", 		"type",	"menu",			MMSGUI_MENU_ATTR_I				},
	{	"menuitem",		NULL, 	NULL,			MMSGUI_BASE_ATTR_I				},
	{	"progressbar",	NULL, 	NULL,			MMSGUI_PROGRESSBAR_ATTR_I		},
	{	"class", 		"type",	"progressbar",	MMSGUI_PROGRESSBAR_ATTR_I		},
	{	"slider",		NULL, 	NULL,			MMSGUI_SLIDER_ATTR_I			},
	{	"class", 		"type",	"slider",		MMSGUI_SLIDER_ATTR_I			},
	{	"textbox",		NULL, 	NULL,			MMSGUI_TEXTBOX_ATTR_I			},
	{	"class", 		"type",	"textbox",		MMSGUI_TEXTBOX_ATTR_I			},
	{	"separator",	NULL, 	NULL,			MMSGUI_NONE_ATTR_I				},
	{	NULL, 			NULL, 	NULL,			NULL							}
};

TAFF_DESCRIPTION mmsgui_taff_description = { "mmsgui", 1, mmsgui_taff_tagtable };





bool MMSTheme::addTemplateClass(MMSTemplateClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < templateClasses.size(); i++)
        if (templateClasses.at(i)->getClassName() == className)
            return false;
    templateClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addMainWindowClass(MMSMainWindowClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < mainWindowClasses.size(); i++)
        if (mainWindowClasses.at(i)->getClassName() == className)
            return false;
    mainWindowClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addPopupWindowClass(MMSPopupWindowClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < popupWindowClasses.size(); i++)
        if (popupWindowClasses.at(i)->getClassName() == className)
            return false;
    popupWindowClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addRootWindowClass(MMSRootWindowClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < rootWindowClasses.size(); i++)
        if (rootWindowClasses.at(i)->getClassName() == className)
            return false;
    rootWindowClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addChildWindowClass(MMSChildWindowClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < childWindowClasses.size(); i++)
        if (childWindowClasses.at(i)->getClassName() == className)
            return false;
    childWindowClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addLabelClass(MMSLabelClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < labelClasses.size(); i++)
        if (labelClasses.at(i)->getClassName() == className)
            return false;
    labelClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addImageClass(MMSImageClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < imageClasses.size(); i++)
        if (imageClasses.at(i)->getClassName() == className)
            return false;
    imageClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addButtonClass(MMSButtonClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < buttonClasses.size(); i++)
        if (buttonClasses.at(i)->getClassName() == className)
            return false;
    buttonClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addProgressBarClass(MMSProgressBarClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < progressBarClasses.size(); i++)
        if (progressBarClasses.at(i)->getClassName() == className)
            return false;
    progressBarClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addMenuClass(MMSMenuClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < menuClasses.size(); i++)
        if (menuClasses.at(i)->getClassName() == className)
            return false;
    menuClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addTextBoxClass(MMSTextBoxClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < textBoxClasses.size(); i++)
        if (textBoxClasses.at(i)->getClassName() == className)
            return false;
    textBoxClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addArrowClass(MMSArrowClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < arrowClasses.size(); i++)
        if (arrowClasses.at(i)->getClassName() == className)
            return false;
    arrowClasses.push_back(themeClass);
    return true;
}

bool MMSTheme::addSliderClass(MMSSliderClass *themeClass) {
    string className = themeClass->getClassName();
    if (className == "") return false;
    for (unsigned int i = 0; i < sliderClasses.size(); i++)
        if (sliderClasses.at(i)->getClassName() == className)
            return false;
    sliderClasses.push_back(themeClass);
    return true;
}

#ifdef ssfsf
void MMSTheme::addSimpleHSliderClass(MMSSIMPLESLIDERH_THEME *simpleHSliderClass, const string name) {
    /* add to hash lookup table */
    pair<SIMPLEHSLIDERCLASSMAP::iterator, bool> p = simpleHSliderClassMap.insert(pair<const char*, MMSSIMPLESLIDERH_THEME*>(name.c_str(), (MMSSIMPLESLIDERH_THEME*)simpleHSliderClass));
    if(!p.second)
        throw new MMSThemeError(1, "duplicate class name: " + name);

    simpleHSliderClasses.push_back(simpleHSliderClass);    
}

void MMSTheme::addSimpleVSliderClass(MMSSIMPLESLIDERV_THEME *simpleVSliderClass, const string name) {
    /* add to hash lookup table */
    pair<SIMPLEVSLIDERCLASSMAP::iterator, bool> p = simpleVSliderClassMap.insert(pair<const char*, MMSSIMPLESLIDERV_THEME*>(name.c_str(), (MMSSIMPLESLIDERV_THEME*)simpleVSliderClass));
    if(!p.second)
        throw new MMSThemeError(1, "duplicate class name: " + name);

    simpleVSliderClasses.push_back(simpleVSliderClass);        
}

void MMSTheme::addSimpleHMenuClass(MMSSIMPLEHMENU_THEME *simpleHMenuClass, const string name) {
    /* add to hash lookup table */
    pair<SIMPLEHMENUCLASSMAP::iterator, bool> p = simpleHMenuClassMap.insert(pair<const char*, MMSSIMPLEHMENU_THEME*>(name.c_str(), (MMSSIMPLEHMENU_THEME*)simpleHMenuClass));
    if(!p.second)
        throw new MMSThemeError(1, "duplicate class name: " + name);

    simpleHMenuClasses.push_back(simpleHMenuClass);            
}

void MMSTheme::addSimpleVMenuClass(MMSSIMPLEVMENU_THEME *simpleVMenuClass, const string name) {
    /* add to hash lookup table */
    pair<SIMPLEVMENUCLASSMAP::iterator, bool> p = simpleVMenuClassMap.insert(pair<const char*, MMSSIMPLEVMENU_THEME*>(name.c_str(), (MMSSIMPLEVMENU_THEME*)simpleVMenuClass));
    if(!p.second)
        throw new MMSThemeError(1, "duplicate class name: " + name);

    simpleVMenuClasses.push_back(simpleVMenuClass);                
}
#endif

#ifdef ssfsf
void MMSTheme::addTextboxClass(MMSTEXTBOX_THEME *textboxClass, const string name) {
    /* add to hash lookup table */
    pair<TEXTBOXCLASSMAP::iterator, bool> p = textboxClassMap.insert(pair<const char*, MMSTEXTBOX_THEME*>(name.c_str(), (MMSTEXTBOX_THEME*)textboxClass));
    if(!p.second)
        throw new MMSThemeError(1, "duplicate class name: " + name);

    textboxClasses.push_back(textboxClass);                    
}
#endif

MMSTemplateClass* MMSTheme::getTemplateClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < templateClasses.size(); i++)
        if (templateClasses.at(i)->getClassName() == className)
            return templateClasses.at(i);
    return NULL;
}

MMSMainWindowClass* MMSTheme::getMainWindowClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < mainWindowClasses.size(); i++)
        if (mainWindowClasses.at(i)->getClassName() == className)
            return mainWindowClasses.at(i);
    return NULL;
}

MMSPopupWindowClass* MMSTheme::getPopupWindowClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < popupWindowClasses.size(); i++)
        if (popupWindowClasses.at(i)->getClassName() == className)
            return popupWindowClasses.at(i);
    return NULL;
}

MMSRootWindowClass* MMSTheme::getRootWindowClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < rootWindowClasses.size(); i++)
        if (rootWindowClasses.at(i)->getClassName() == className)
            return rootWindowClasses.at(i);
    return NULL;
}

MMSChildWindowClass* MMSTheme::getChildWindowClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < childWindowClasses.size(); i++)
        if (childWindowClasses.at(i)->getClassName() == className)
            return childWindowClasses.at(i);
    return NULL;
}

MMSLabelClass* MMSTheme::getLabelClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < labelClasses.size(); i++)
        if (labelClasses.at(i)->getClassName() == className)
            return labelClasses.at(i);
    return NULL;
}

MMSImageClass* MMSTheme::getImageClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < imageClasses.size(); i++)
        if (imageClasses.at(i)->getClassName() == className)
            return imageClasses.at(i);
    return NULL;
}

MMSButtonClass* MMSTheme::getButtonClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < buttonClasses.size(); i++)
        if (buttonClasses.at(i)->getClassName() == className)
            return buttonClasses.at(i);
    return NULL;
}

MMSProgressBarClass* MMSTheme::getProgressBarClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < progressBarClasses.size(); i++)
        if (progressBarClasses.at(i)->getClassName() == className)
            return progressBarClasses.at(i);
    return NULL;
}

MMSMenuClass* MMSTheme::getMenuClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < menuClasses.size(); i++)
        if (menuClasses.at(i)->getClassName() == className)
            return menuClasses.at(i);
    return NULL;
}

MMSTextBoxClass* MMSTheme::getTextBoxClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < textBoxClasses.size(); i++)
        if (textBoxClasses.at(i)->getClassName() == className)
            return textBoxClasses.at(i);
    return NULL;
}

MMSArrowClass* MMSTheme::getArrowClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < arrowClasses.size(); i++)
        if (arrowClasses.at(i)->getClassName() == className)
            return arrowClasses.at(i);
    return NULL;
}

MMSSliderClass* MMSTheme::getSliderClass(string className) {
    if (className=="") return NULL;
    for (unsigned int i = 0; i < sliderClasses.size(); i++)
        if (sliderClasses.at(i)->getClassName() == className)
            return sliderClasses.at(i);
    return NULL;
}


MMSTheme::MMSTheme() {

    /* initialize the theme with default values */
    DFBColor color;
    color.a = 0;
    color.r = 0;
    color.g = 0;
    color.b = 0;

    /* MMSMainWindow */
    {
        /* base window settings */
        this->mainWindowClass.windowClass.setAlignment(MMSALIGNMENT_CENTER);
        this->mainWindowClass.windowClass.setDx("0px");
        this->mainWindowClass.windowClass.setDy("0px");
        this->mainWindowClass.windowClass.setWidth("100%");
        this->mainWindowClass.windowClass.setHeight("100%");
        this->mainWindowClass.windowClass.setBgColor(color);
        this->mainWindowClass.windowClass.setBgImagePath("");
        this->mainWindowClass.windowClass.setBgImageName("");
        this->mainWindowClass.windowClass.setOpacity(255);
        this->mainWindowClass.windowClass.setFadeIn(false);
        this->mainWindowClass.windowClass.setFadeOut(false);
        this->mainWindowClass.windowClass.setDebug(false);
        this->mainWindowClass.windowClass.setMargin(0);
        this->mainWindowClass.windowClass.setUpArrow("");
        this->mainWindowClass.windowClass.setDownArrow("");
        this->mainWindowClass.windowClass.setLeftArrow("");
        this->mainWindowClass.windowClass.setRightArrow("");
        this->mainWindowClass.windowClass.setNavigateUp("");
        this->mainWindowClass.windowClass.setNavigateDown("");
        this->mainWindowClass.windowClass.setNavigateLeft("");
        this->mainWindowClass.windowClass.setNavigateRight("");
        this->mainWindowClass.windowClass.setOwnSurface(true);
        this->mainWindowClass.windowClass.setMoveIn(MMSDIRECTION_NOTSET);
        this->mainWindowClass.windowClass.setMoveOut(MMSDIRECTION_NOTSET);

        /* base window border settings */
        this->mainWindowClass.windowClass.border.setColor(color);
        this->mainWindowClass.windowClass.border.setSelColor(color);
        this->mainWindowClass.windowClass.border.setImagePath("");
        this->mainWindowClass.windowClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->mainWindowClass.windowClass.border.setSelImagePath("");
        this->mainWindowClass.windowClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->mainWindowClass.windowClass.border.setThickness(0);
        this->mainWindowClass.windowClass.border.setMargin(0);
        this->mainWindowClass.windowClass.border.setRCorners(false);
    }

    /* MMSPopupWindow */
    {
        /* base window settings */
        this->popupWindowClass.windowClass.setAlignment(MMSALIGNMENT_CENTER);
        this->popupWindowClass.windowClass.setDx("0px");
        this->popupWindowClass.windowClass.setDy("0px");
        this->popupWindowClass.windowClass.setWidth("100%");
        this->popupWindowClass.windowClass.setHeight("100%");
        this->popupWindowClass.windowClass.setBgColor(color);
        this->popupWindowClass.windowClass.setBgImagePath("");
        this->popupWindowClass.windowClass.setBgImageName("");
        this->popupWindowClass.windowClass.setOpacity(255);
        this->popupWindowClass.windowClass.setFadeIn(false);
        this->popupWindowClass.windowClass.setFadeOut(false);
        this->popupWindowClass.windowClass.setDebug(false);
        this->popupWindowClass.windowClass.setMargin(0);
        this->popupWindowClass.windowClass.setUpArrow("");
        this->popupWindowClass.windowClass.setDownArrow("");
        this->popupWindowClass.windowClass.setLeftArrow("");
        this->popupWindowClass.windowClass.setRightArrow("");
        this->popupWindowClass.windowClass.setNavigateUp("");
        this->popupWindowClass.windowClass.setNavigateDown("");
        this->popupWindowClass.windowClass.setNavigateLeft("");
        this->popupWindowClass.windowClass.setNavigateRight("");
        this->popupWindowClass.windowClass.setOwnSurface(true);
        this->popupWindowClass.windowClass.setMoveIn(MMSDIRECTION_NOTSET);
        this->popupWindowClass.windowClass.setMoveOut(MMSDIRECTION_NOTSET);

        /* base window border settings */
        this->popupWindowClass.windowClass.border.setColor(color);
        this->popupWindowClass.windowClass.border.setSelColor(color);
        this->popupWindowClass.windowClass.border.setImagePath("");
        this->popupWindowClass.windowClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->popupWindowClass.windowClass.border.setSelImagePath("");
        this->popupWindowClass.windowClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->popupWindowClass.windowClass.border.setThickness(0);
        this->popupWindowClass.windowClass.border.setMargin(0);
        this->popupWindowClass.windowClass.border.setRCorners(false);

        /* popup window settings */
        this->popupWindowClass.setDuration(5);
    }

    /* MMSRootWindow */
    {
        /* base window settings */
        this->rootWindowClass.windowClass.setAlignment(MMSALIGNMENT_CENTER);
        this->rootWindowClass.windowClass.setDx("0px");
        this->rootWindowClass.windowClass.setDy("0px");
        this->rootWindowClass.windowClass.setWidth("100%");
        this->rootWindowClass.windowClass.setHeight("100%");
        this->rootWindowClass.windowClass.setBgColor(color);
        this->rootWindowClass.windowClass.setBgImagePath("");
        this->rootWindowClass.windowClass.setBgImageName("");
        this->rootWindowClass.windowClass.setOpacity(255);
        this->rootWindowClass.windowClass.setFadeIn(false);
        this->rootWindowClass.windowClass.setFadeOut(false);
        this->rootWindowClass.windowClass.setDebug(false);
        this->rootWindowClass.windowClass.setMargin(0);
        this->rootWindowClass.windowClass.setUpArrow("");
        this->rootWindowClass.windowClass.setDownArrow("");
        this->rootWindowClass.windowClass.setLeftArrow("");
        this->rootWindowClass.windowClass.setRightArrow("");
        this->rootWindowClass.windowClass.setNavigateUp("");
        this->rootWindowClass.windowClass.setNavigateDown("");
        this->rootWindowClass.windowClass.setNavigateLeft("");
        this->rootWindowClass.windowClass.setNavigateRight("");
        this->rootWindowClass.windowClass.setOwnSurface(true);
        this->rootWindowClass.windowClass.setMoveIn(MMSDIRECTION_NOTSET);
        this->rootWindowClass.windowClass.setMoveOut(MMSDIRECTION_NOTSET);

        /* base window border settings */
        this->rootWindowClass.windowClass.border.setColor(color);
        this->rootWindowClass.windowClass.border.setSelColor(color);
        this->rootWindowClass.windowClass.border.setImagePath("");
        this->rootWindowClass.windowClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->rootWindowClass.windowClass.border.setSelImagePath("");
        this->rootWindowClass.windowClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->rootWindowClass.windowClass.border.setThickness(0);
        this->rootWindowClass.windowClass.border.setMargin(0);
        this->rootWindowClass.windowClass.border.setRCorners(false);
    }

    /* MMSChildWindow */
    {
        /* base window settings */
        this->childWindowClass.windowClass.setAlignment(MMSALIGNMENT_CENTER);
        this->childWindowClass.windowClass.setDx("0px");
        this->childWindowClass.windowClass.setDy("0px");
        this->childWindowClass.windowClass.setWidth("100%");
        this->childWindowClass.windowClass.setHeight("100%");
        this->childWindowClass.windowClass.setBgColor(color);
        this->childWindowClass.windowClass.setBgImagePath("");
        this->childWindowClass.windowClass.setBgImageName("");
        this->childWindowClass.windowClass.setOpacity(255);
        this->childWindowClass.windowClass.setFadeIn(false);
        this->childWindowClass.windowClass.setFadeOut(false);
        this->childWindowClass.windowClass.setDebug(false);
        this->childWindowClass.windowClass.setMargin(0);
        this->childWindowClass.windowClass.setUpArrow("");
        this->childWindowClass.windowClass.setDownArrow("");
        this->childWindowClass.windowClass.setLeftArrow("");
        this->childWindowClass.windowClass.setRightArrow("");
        this->childWindowClass.windowClass.setNavigateUp("");
        this->childWindowClass.windowClass.setNavigateDown("");
        this->childWindowClass.windowClass.setNavigateLeft("");
        this->childWindowClass.windowClass.setNavigateRight("");
        this->childWindowClass.windowClass.setOwnSurface(true);
        this->childWindowClass.windowClass.setMoveIn(MMSDIRECTION_NOTSET);
        this->childWindowClass.windowClass.setMoveOut(MMSDIRECTION_NOTSET);

        /* base window border settings */
        this->childWindowClass.windowClass.border.setColor(color);
        this->childWindowClass.windowClass.border.setSelColor(color);
        this->childWindowClass.windowClass.border.setImagePath("");
        this->childWindowClass.windowClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->childWindowClass.windowClass.border.setSelImagePath("");
        this->childWindowClass.windowClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->childWindowClass.windowClass.border.setThickness(0);
        this->childWindowClass.windowClass.border.setMargin(0);
        this->childWindowClass.windowClass.border.setRCorners(false);
    }

    /* MMSLabel */
    {
        /* base widget settings */
        this->labelClass.widgetClass.setBgColor(color);
        this->labelClass.widgetClass.setSelBgColor(color);
        this->labelClass.widgetClass.setBgColor_p(color);
        this->labelClass.widgetClass.setSelBgColor_p(color);
        this->labelClass.widgetClass.setBgColor_i(color);
        this->labelClass.widgetClass.setSelBgColor_i(color);
        this->labelClass.widgetClass.setBgImagePath("");
        this->labelClass.widgetClass.setBgImageName("");
        this->labelClass.widgetClass.setSelBgImagePath("");
        this->labelClass.widgetClass.setSelBgImageName("");
        this->labelClass.widgetClass.setBgImagePath_p("");
        this->labelClass.widgetClass.setBgImageName_p("");
        this->labelClass.widgetClass.setSelBgImagePath_p("");
        this->labelClass.widgetClass.setSelBgImageName_p("");
        this->labelClass.widgetClass.setBgImagePath_i("");
        this->labelClass.widgetClass.setBgImageName_i("");
        this->labelClass.widgetClass.setSelBgImagePath_i("");
        this->labelClass.widgetClass.setSelBgImageName_i("");
        this->labelClass.widgetClass.setMargin(0);
        this->labelClass.widgetClass.setFocusable(false);
        this->labelClass.widgetClass.setSelectable(true);
        this->labelClass.widgetClass.setUpArrow("");
        this->labelClass.widgetClass.setDownArrow("");
        this->labelClass.widgetClass.setLeftArrow("");
        this->labelClass.widgetClass.setRightArrow("");
        this->labelClass.widgetClass.setData("");
        this->labelClass.widgetClass.setNavigateUp("");
        this->labelClass.widgetClass.setNavigateDown("");
        this->labelClass.widgetClass.setNavigateLeft("");
        this->labelClass.widgetClass.setNavigateRight("");
        this->labelClass.widgetClass.setVSlider("");
        this->labelClass.widgetClass.setHSlider("");
        this->labelClass.widgetClass.setImagesOnDemand(false);
        this->labelClass.widgetClass.setBlend(0);
        this->labelClass.widgetClass.setBlendFactor(0);

        /* base widget border settings */
        this->labelClass.widgetClass.border.setColor(color);
        this->labelClass.widgetClass.border.setSelColor(color);
        this->labelClass.widgetClass.border.setImagePath("");
        this->labelClass.widgetClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->labelClass.widgetClass.border.setSelImagePath("");
        this->labelClass.widgetClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->labelClass.widgetClass.border.setThickness(0);
        this->labelClass.widgetClass.border.setMargin(0);
        this->labelClass.widgetClass.border.setRCorners(false);

        /* label settings */
        this->labelClass.setFontPath("./themes/default");
        this->labelClass.setFontName("decker.ttf");
        this->labelClass.setFontSize(16);
        this->labelClass.setAlignment(MMSALIGNMENT_CENTER);
        DFBColor c;
        c.a = 255;
        c.r = 192;
        c.g = 192;
        c.b = 192;
        this->labelClass.setColor(c);
        c.a = 255;
        c.r = 255;
        c.g = 255;
        c.b = 255;
        this->labelClass.setSelColor(c);
        this->labelClass.setText("");
    }

    /* MMSImage */
    {
        /* base widget settings */
        this->imageClass.widgetClass.setBgColor(color);
        this->imageClass.widgetClass.setSelBgColor(color);
        this->imageClass.widgetClass.setBgColor_p(color);
        this->imageClass.widgetClass.setSelBgColor_p(color);
        this->imageClass.widgetClass.setBgColor_i(color);
        this->imageClass.widgetClass.setSelBgColor_i(color);
        this->imageClass.widgetClass.setBgImagePath("");
        this->imageClass.widgetClass.setBgImageName("");
        this->imageClass.widgetClass.setSelBgImagePath("");
        this->imageClass.widgetClass.setSelBgImageName("");
        this->imageClass.widgetClass.setBgImagePath_p("");
        this->imageClass.widgetClass.setBgImageName_p("");
        this->imageClass.widgetClass.setSelBgImagePath_p("");
        this->imageClass.widgetClass.setSelBgImageName_p("");
        this->imageClass.widgetClass.setBgImagePath_i("");
        this->imageClass.widgetClass.setBgImageName_i("");
        this->imageClass.widgetClass.setSelBgImagePath_i("");
        this->imageClass.widgetClass.setSelBgImageName_i("");
        this->imageClass.widgetClass.setMargin(0);
        this->imageClass.widgetClass.setFocusable(false);
        this->imageClass.widgetClass.setSelectable(true);
        this->imageClass.widgetClass.setUpArrow("");
        this->imageClass.widgetClass.setDownArrow("");
        this->imageClass.widgetClass.setLeftArrow("");
        this->imageClass.widgetClass.setRightArrow("");
        this->imageClass.widgetClass.setData("");
        this->imageClass.widgetClass.setNavigateUp("");
        this->imageClass.widgetClass.setNavigateDown("");
        this->imageClass.widgetClass.setNavigateLeft("");
        this->imageClass.widgetClass.setNavigateRight("");
        this->imageClass.widgetClass.setVSlider("");
        this->imageClass.widgetClass.setHSlider("");
        this->imageClass.widgetClass.setImagesOnDemand(false);
        this->imageClass.widgetClass.setBlend(0);
        this->imageClass.widgetClass.setBlendFactor(0);

        /* base widget border settings */
        this->imageClass.widgetClass.border.setColor(color);
        this->imageClass.widgetClass.border.setSelColor(color);
        this->imageClass.widgetClass.border.setImagePath("");
        this->imageClass.widgetClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->imageClass.widgetClass.border.setSelImagePath("");
        this->imageClass.widgetClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->imageClass.widgetClass.border.setThickness(0);
        this->imageClass.widgetClass.border.setMargin(0);
        this->imageClass.widgetClass.border.setRCorners(false);

        /* image settings */
        this->imageClass.setImagePath("");
        this->imageClass.setImageName("");
        this->imageClass.setSelImagePath("");
        this->imageClass.setSelImageName("");
        this->imageClass.setImagePath_p("");
        this->imageClass.setImageName_p("");
        this->imageClass.setSelImagePath_p("");
        this->imageClass.setSelImageName_p("");
        this->imageClass.setImagePath_i("");
        this->imageClass.setImageName_i("");
        this->imageClass.setSelImagePath_i("");
        this->imageClass.setSelImageName_i("");
        this->imageClass.setUseRatio(false);
        this->imageClass.setFitWidth(false);
        this->imageClass.setFitHeight(false);
        this->imageClass.setAlignment(MMSALIGNMENT_CENTER);
    }

    /* MMSButton */
    {
        /* base widget settings */
        this->buttonClass.widgetClass.setBgColor(color);
        this->buttonClass.widgetClass.setSelBgColor(color);
        this->buttonClass.widgetClass.setBgColor_p(color);
        this->buttonClass.widgetClass.setSelBgColor_p(color);
        this->buttonClass.widgetClass.setBgColor_i(color);
        this->buttonClass.widgetClass.setSelBgColor_i(color);
        this->buttonClass.widgetClass.setBgImagePath("");
        this->buttonClass.widgetClass.setBgImageName("");
        this->buttonClass.widgetClass.setSelBgImagePath("");
        this->buttonClass.widgetClass.setSelBgImageName("");
        this->buttonClass.widgetClass.setBgImagePath_p("");
        this->buttonClass.widgetClass.setBgImageName_p("");
        this->buttonClass.widgetClass.setSelBgImagePath_p("");
        this->buttonClass.widgetClass.setSelBgImageName_p("");
        this->buttonClass.widgetClass.setBgImagePath_i("");
        this->buttonClass.widgetClass.setBgImageName_i("");
        this->buttonClass.widgetClass.setSelBgImagePath_i("");
        this->buttonClass.widgetClass.setSelBgImageName_i("");
        this->buttonClass.widgetClass.setMargin(0);
        this->buttonClass.widgetClass.setFocusable(true);
        this->buttonClass.widgetClass.setSelectable(true);
        this->buttonClass.widgetClass.setUpArrow("");
        this->buttonClass.widgetClass.setDownArrow("");
        this->buttonClass.widgetClass.setLeftArrow("");
        this->buttonClass.widgetClass.setRightArrow("");
        this->buttonClass.widgetClass.setData("");
        this->buttonClass.widgetClass.setNavigateUp("");
        this->buttonClass.widgetClass.setNavigateDown("");
        this->buttonClass.widgetClass.setNavigateLeft("");
        this->buttonClass.widgetClass.setNavigateRight("");
        this->buttonClass.widgetClass.setVSlider("");
        this->buttonClass.widgetClass.setHSlider("");
        this->buttonClass.widgetClass.setImagesOnDemand(false);
        this->buttonClass.widgetClass.setBlend(0);
        this->buttonClass.widgetClass.setBlendFactor(0);

        /* base widget border settings */
        this->buttonClass.widgetClass.border.setColor(color);
        this->buttonClass.widgetClass.border.setSelColor(color);
        this->buttonClass.widgetClass.border.setImagePath("");
        this->buttonClass.widgetClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->buttonClass.widgetClass.border.setSelImagePath("");
        this->buttonClass.widgetClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->buttonClass.widgetClass.border.setThickness(0);
        this->buttonClass.widgetClass.border.setMargin(0);
        this->buttonClass.widgetClass.border.setRCorners(false);
    }

    /* MMSProgressBar */
    {
        /* base widget settings */
        this->progressBarClass.widgetClass.setBgColor(color);
        this->progressBarClass.widgetClass.setSelBgColor(color);
        this->progressBarClass.widgetClass.setBgColor_p(color);
        this->progressBarClass.widgetClass.setSelBgColor_p(color);
        this->progressBarClass.widgetClass.setBgColor_i(color);
        this->progressBarClass.widgetClass.setSelBgColor_i(color);
        this->progressBarClass.widgetClass.setBgImagePath("");
        this->progressBarClass.widgetClass.setBgImageName("");
        this->progressBarClass.widgetClass.setSelBgImagePath("");
        this->progressBarClass.widgetClass.setSelBgImageName("");
        this->progressBarClass.widgetClass.setBgImagePath_p("");
        this->progressBarClass.widgetClass.setBgImageName_p("");
        this->progressBarClass.widgetClass.setSelBgImagePath_p("");
        this->progressBarClass.widgetClass.setSelBgImageName_p("");
        this->progressBarClass.widgetClass.setBgImagePath_i("");
        this->progressBarClass.widgetClass.setBgImageName_i("");
        this->progressBarClass.widgetClass.setSelBgImagePath_i("");
        this->progressBarClass.widgetClass.setSelBgImageName_i("");
        this->progressBarClass.widgetClass.setMargin(0);
        this->progressBarClass.widgetClass.setFocusable(false);
        this->progressBarClass.widgetClass.setSelectable(true);
        this->progressBarClass.widgetClass.setUpArrow("");
        this->progressBarClass.widgetClass.setDownArrow("");
        this->progressBarClass.widgetClass.setLeftArrow("");
        this->progressBarClass.widgetClass.setRightArrow("");
        this->progressBarClass.widgetClass.setData("");
        this->progressBarClass.widgetClass.setNavigateUp("");
        this->progressBarClass.widgetClass.setNavigateDown("");
        this->progressBarClass.widgetClass.setNavigateLeft("");
        this->progressBarClass.widgetClass.setNavigateRight("");
        this->progressBarClass.widgetClass.setVSlider("");
        this->progressBarClass.widgetClass.setHSlider("");
        this->progressBarClass.widgetClass.setImagesOnDemand(false);
        this->progressBarClass.widgetClass.setBlend(0);
        this->progressBarClass.widgetClass.setBlendFactor(0);

        /* base widget border settings */
        this->progressBarClass.widgetClass.border.setColor(color);
        this->progressBarClass.widgetClass.border.setSelColor(color);
        this->progressBarClass.widgetClass.border.setImagePath("");
        this->progressBarClass.widgetClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->progressBarClass.widgetClass.border.setSelImagePath("");
        this->progressBarClass.widgetClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->progressBarClass.widgetClass.border.setThickness(0);
        this->progressBarClass.widgetClass.border.setMargin(0);
        this->progressBarClass.widgetClass.border.setRCorners(false);

        /* progressbar settings */
        DFBColor c;
        c.a = 0;
        c.r = 0;
        c.g = 0;
        c.b = 0;
        this->progressBarClass.setColor(c);
        c.a = 255;
        c.r = 255;
        c.g = 255;
        c.b = 255;
        this->progressBarClass.setSelColor(c);
        this->progressBarClass.setProgress(100);
    }

    /* MMSMenu */
    {
        /* base widget settings */
        this->menuClass.widgetClass.setBgColor(color);
        this->menuClass.widgetClass.setSelBgColor(color);
        this->menuClass.widgetClass.setBgColor_p(color);
        this->menuClass.widgetClass.setSelBgColor_p(color);
        this->menuClass.widgetClass.setBgColor_i(color);
        this->menuClass.widgetClass.setSelBgColor_i(color);
        this->menuClass.widgetClass.setBgImagePath("");
        this->menuClass.widgetClass.setBgImageName("");
        this->menuClass.widgetClass.setSelBgImagePath("");
        this->menuClass.widgetClass.setSelBgImageName("");
        this->menuClass.widgetClass.setBgImagePath_p("");
        this->menuClass.widgetClass.setBgImageName_p("");
        this->menuClass.widgetClass.setSelBgImagePath_p("");
        this->menuClass.widgetClass.setSelBgImageName_p("");
        this->menuClass.widgetClass.setBgImagePath_i("");
        this->menuClass.widgetClass.setBgImageName_i("");
        this->menuClass.widgetClass.setSelBgImagePath_i("");
        this->menuClass.widgetClass.setSelBgImageName_i("");
        this->menuClass.widgetClass.setMargin(0);
        this->menuClass.widgetClass.setFocusable(true);
        this->menuClass.widgetClass.setSelectable(true);
        this->menuClass.widgetClass.setUpArrow("");
        this->menuClass.widgetClass.setDownArrow("");
        this->menuClass.widgetClass.setLeftArrow("");
        this->menuClass.widgetClass.setRightArrow("");
        this->menuClass.widgetClass.setData("");
        this->menuClass.widgetClass.setNavigateUp("");
        this->menuClass.widgetClass.setNavigateDown("");
        this->menuClass.widgetClass.setNavigateLeft("");
        this->menuClass.widgetClass.setNavigateRight("");
        this->menuClass.widgetClass.setVSlider("");
        this->menuClass.widgetClass.setHSlider("");
        this->menuClass.widgetClass.setImagesOnDemand(false);
        this->menuClass.widgetClass.setBlend(0);
        this->menuClass.widgetClass.setBlendFactor(0);

        /* base widget border settings */
        this->menuClass.widgetClass.border.setColor(color);
        this->menuClass.widgetClass.border.setSelColor(color);
        this->menuClass.widgetClass.border.setImagePath("");
        this->menuClass.widgetClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->menuClass.widgetClass.border.setSelImagePath("");
        this->menuClass.widgetClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->menuClass.widgetClass.border.setThickness(0);
        this->menuClass.widgetClass.border.setMargin(0);
        this->menuClass.widgetClass.border.setRCorners(false);

        /* menu settings */
        this->menuClass.setItemWidth("");
        this->menuClass.setItemHeight("");
        this->menuClass.setItemHMargin(0);
        this->menuClass.setItemVMargin(0);
        this->menuClass.setCols(0);
        this->menuClass.setDimItems(0);
        this->menuClass.setFixedPos(-1);
        this->menuClass.setHLoop(false);
        this->menuClass.setVLoop(false);
        this->menuClass.setTransItems(0);
        this->menuClass.setDimTop(0);
        this->menuClass.setDimBottom(0);
        this->menuClass.setDimLeft(0);
        this->menuClass.setDimRight(0);
        this->menuClass.setTransTop(0);
        this->menuClass.setTransBottom(0);
        this->menuClass.setTransLeft(0);
        this->menuClass.setTransRight(0);
        this->menuClass.setZoomSelWidth("");
        this->menuClass.setZoomSelHeight("");
        this->menuClass.setZoomSelShiftX("");
        this->menuClass.setZoomSelShiftY("");
        this->menuClass.setSmoothScrolling(false);
    }

    /* MMSTextBox */
    {
        /* base widget settings */
        this->textBoxClass.widgetClass.setBgColor(color);
        this->textBoxClass.widgetClass.setSelBgColor(color);
        this->textBoxClass.widgetClass.setBgColor_p(color);
        this->textBoxClass.widgetClass.setSelBgColor_p(color);
        this->textBoxClass.widgetClass.setBgColor_i(color);
        this->textBoxClass.widgetClass.setSelBgColor_i(color);
        this->textBoxClass.widgetClass.setBgImagePath("");
        this->textBoxClass.widgetClass.setBgImageName("");
        this->textBoxClass.widgetClass.setSelBgImagePath("");
        this->textBoxClass.widgetClass.setSelBgImageName("");
        this->textBoxClass.widgetClass.setBgImagePath_p("");
        this->textBoxClass.widgetClass.setBgImageName_p("");
        this->textBoxClass.widgetClass.setSelBgImagePath_p("");
        this->textBoxClass.widgetClass.setSelBgImageName_p("");
        this->textBoxClass.widgetClass.setBgImagePath_i("");
        this->textBoxClass.widgetClass.setBgImageName_i("");
        this->textBoxClass.widgetClass.setSelBgImagePath_i("");
        this->textBoxClass.widgetClass.setSelBgImageName_i("");
        this->textBoxClass.widgetClass.setMargin(0);
        this->textBoxClass.widgetClass.setFocusable(true);
        this->textBoxClass.widgetClass.setSelectable(true);
        this->textBoxClass.widgetClass.setUpArrow("");
        this->textBoxClass.widgetClass.setDownArrow("");
        this->textBoxClass.widgetClass.setLeftArrow("");
        this->textBoxClass.widgetClass.setRightArrow("");
        this->textBoxClass.widgetClass.setData("");
        this->textBoxClass.widgetClass.setNavigateUp("");
        this->textBoxClass.widgetClass.setNavigateDown("");
        this->textBoxClass.widgetClass.setNavigateLeft("");
        this->textBoxClass.widgetClass.setNavigateRight("");
        this->textBoxClass.widgetClass.setVSlider("");
        this->textBoxClass.widgetClass.setHSlider("");
        this->textBoxClass.widgetClass.setImagesOnDemand(false);
        this->textBoxClass.widgetClass.setBlend(0);
        this->textBoxClass.widgetClass.setBlendFactor(0);

        /* base widget border settings */
        this->textBoxClass.widgetClass.border.setColor(color);
        this->textBoxClass.widgetClass.border.setSelColor(color);
        this->textBoxClass.widgetClass.border.setImagePath("");
        this->textBoxClass.widgetClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->textBoxClass.widgetClass.border.setSelImagePath("");
        this->textBoxClass.widgetClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->textBoxClass.widgetClass.border.setThickness(0);
        this->textBoxClass.widgetClass.border.setMargin(0);
        this->textBoxClass.widgetClass.border.setRCorners(false);

        /* textbox settings */
        this->textBoxClass.setFontPath("./themes/default");
        this->textBoxClass.setFontName("decker.ttf");
        this->textBoxClass.setFontSize(16);
        this->textBoxClass.setAlignment(MMSALIGNMENT_CENTER);
        this->textBoxClass.setWrap(true);
        this->textBoxClass.setSplitWords(true);
        DFBColor c;
        c.a = 255;
        c.r = 192;
        c.g = 192;
        c.b = 192;
        this->textBoxClass.setColor(c);
        c.a = 255;
        c.r = 255;
        c.g = 255;
        c.b = 255;
        this->textBoxClass.setSelColor(c);
        this->textBoxClass.setText("");
    }

    /* MMSArrow */
    {
        /* base widget settings */
        this->arrowClass.widgetClass.setBgColor(color);
        this->arrowClass.widgetClass.setSelBgColor(color);
        this->arrowClass.widgetClass.setBgColor_p(color);
        this->arrowClass.widgetClass.setSelBgColor_p(color);
        this->arrowClass.widgetClass.setBgColor_i(color);
        this->arrowClass.widgetClass.setSelBgColor_i(color);
        this->arrowClass.widgetClass.setBgImagePath("");
        this->arrowClass.widgetClass.setBgImageName("");
        this->arrowClass.widgetClass.setSelBgImagePath("");
        this->arrowClass.widgetClass.setSelBgImageName("");
        this->arrowClass.widgetClass.setBgImagePath_p("");
        this->arrowClass.widgetClass.setBgImageName_p("");
        this->arrowClass.widgetClass.setSelBgImagePath_p("");
        this->arrowClass.widgetClass.setSelBgImageName_p("");
        this->arrowClass.widgetClass.setBgImagePath_i("");
        this->arrowClass.widgetClass.setBgImageName_i("");
        this->arrowClass.widgetClass.setSelBgImagePath_i("");
        this->arrowClass.widgetClass.setSelBgImageName_i("");
        this->arrowClass.widgetClass.setMargin(0);
        this->arrowClass.widgetClass.setFocusable(false);
        this->arrowClass.widgetClass.setSelectable(true);
        this->arrowClass.widgetClass.setUpArrow("");
        this->arrowClass.widgetClass.setDownArrow("");
        this->arrowClass.widgetClass.setLeftArrow("");
        this->arrowClass.widgetClass.setRightArrow("");
        this->arrowClass.widgetClass.setData("");
        this->arrowClass.widgetClass.setNavigateUp("");
        this->arrowClass.widgetClass.setNavigateDown("");
        this->arrowClass.widgetClass.setNavigateLeft("");
        this->arrowClass.widgetClass.setNavigateRight("");
        this->arrowClass.widgetClass.setVSlider("");
        this->arrowClass.widgetClass.setHSlider("");
        this->arrowClass.widgetClass.setImagesOnDemand(false);
        this->arrowClass.widgetClass.setBlend(0);
        this->arrowClass.widgetClass.setBlendFactor(0);

        /* base widget border settings */
        this->arrowClass.widgetClass.border.setColor(color);
        this->arrowClass.widgetClass.border.setSelColor(color);
        this->arrowClass.widgetClass.border.setImagePath("");
        this->arrowClass.widgetClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->arrowClass.widgetClass.border.setSelImagePath("");
        this->arrowClass.widgetClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->arrowClass.widgetClass.border.setThickness(0);
        this->arrowClass.widgetClass.border.setMargin(0);
        this->arrowClass.widgetClass.border.setRCorners(false);

        /* arrow settings */
        DFBColor c;
        c.a = 255;
        c.r = 192;
        c.g = 192;
        c.b = 192;
        this->arrowClass.setColor(c);
        c.a = 255;
        c.r = 255;
        c.g = 255;
        c.b = 255;
        this->arrowClass.setSelColor(c);
        this->arrowClass.setDirection(MMSDIRECTION_LEFT);
    }

    /* MMSSlider */
    {
        /* base widget settings */
        this->sliderClass.widgetClass.setBgColor(color);
        this->sliderClass.widgetClass.setSelBgColor(color);
        this->sliderClass.widgetClass.setBgColor_p(color);
        this->sliderClass.widgetClass.setSelBgColor_p(color);
        this->sliderClass.widgetClass.setBgColor_i(color);
        this->sliderClass.widgetClass.setSelBgColor_i(color);
        this->sliderClass.widgetClass.setBgImagePath("");
        this->sliderClass.widgetClass.setBgImageName("");
        this->sliderClass.widgetClass.setSelBgImagePath("");
        this->sliderClass.widgetClass.setSelBgImageName("");
        this->sliderClass.widgetClass.setBgImagePath_p("");
        this->sliderClass.widgetClass.setBgImageName_p("");
        this->sliderClass.widgetClass.setSelBgImagePath_p("");
        this->sliderClass.widgetClass.setSelBgImageName_p("");
        this->sliderClass.widgetClass.setBgImagePath_i("");
        this->sliderClass.widgetClass.setBgImageName_i("");
        this->sliderClass.widgetClass.setSelBgImagePath_i("");
        this->sliderClass.widgetClass.setSelBgImageName_i("");
        this->sliderClass.widgetClass.setMargin(0);
        this->sliderClass.widgetClass.setFocusable(false);
        this->sliderClass.widgetClass.setSelectable(true);
        this->sliderClass.widgetClass.setUpArrow("");
        this->sliderClass.widgetClass.setDownArrow("");
        this->sliderClass.widgetClass.setLeftArrow("");
        this->sliderClass.widgetClass.setRightArrow("");
        this->sliderClass.widgetClass.setData("");
        this->sliderClass.widgetClass.setNavigateUp("");
        this->sliderClass.widgetClass.setNavigateDown("");
        this->sliderClass.widgetClass.setNavigateLeft("");
        this->sliderClass.widgetClass.setNavigateRight("");
        this->sliderClass.widgetClass.setVSlider("");
        this->sliderClass.widgetClass.setHSlider("");
        this->sliderClass.widgetClass.setImagesOnDemand(false);
        this->sliderClass.widgetClass.setBlend(0);
        this->sliderClass.widgetClass.setBlendFactor(0);

        /* base widget border settings */
        this->sliderClass.widgetClass.border.setColor(color);
        this->sliderClass.widgetClass.border.setSelColor(color);
        this->sliderClass.widgetClass.border.setImagePath("");
        this->sliderClass.widgetClass.border.setImageNames("", "", "", "", "", "", "", "");
        this->sliderClass.widgetClass.border.setSelImagePath("");
        this->sliderClass.widgetClass.border.setSelImageNames("", "", "", "", "", "", "", "");
        this->sliderClass.widgetClass.border.setThickness(0);
        this->sliderClass.widgetClass.border.setMargin(0);
        this->sliderClass.widgetClass.border.setRCorners(false);

        /* slider settings */
        this->sliderClass.setImagePath("");
        this->sliderClass.setImageName("");
        this->sliderClass.setSelImagePath("");
        this->sliderClass.setSelImageName("");
        this->sliderClass.setImagePath_p("");
        this->sliderClass.setImageName_p("");
        this->sliderClass.setSelImagePath_p("");
        this->sliderClass.setSelImageName_p("");
        this->sliderClass.setImagePath_i("");
        this->sliderClass.setImageName_i("");
        this->sliderClass.setSelImagePath_i("");
        this->sliderClass.setSelImageName_i("");
        this->sliderClass.setPosition(0);
    }

}

MMSTheme::~MMSTheme() {
}

void MMSTheme::setTheme(string path, string themeName) {
    this->path      = path;
    this->themeName = themeName;
    if (path == "")
        this->themePath = "./themes/" + themeName;
    else
        this->themePath = path + "/themes/" + themeName;
    this->themeFile = this->themePath + "/theme.xml";
}

string MMSTheme::getPath() {
    return this->path;
}

string MMSTheme::getThemeName() {
    return this->themeName;
}

string MMSTheme::getThemePath() {
    return this->themePath;
}

string MMSTheme::getThemeFile() {
    return this->themeFile;
}




