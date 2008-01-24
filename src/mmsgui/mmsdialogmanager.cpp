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

#include "mmsgui/mmsdialogmanager.h"
#include "mmsgui/mmsmainwindow.h"
#include "mmsgui/mmspopupwindow.h"
#include "mmsgui/mmsrootwindow.h"
#include "mmsgui/mmschildwindow.h"
#include "mmsgui/mmsvbox.h"
#include "mmsgui/mmshbox.h"
#include "mmsgui/mmslabel.h"
#include "mmsgui/mmsbutton.h"
#include "mmsgui/mmsimage.h"
#include "mmsgui/mmsprogressbar.h"
#include "mmsgui/mmsmenu.h"
#include "mmsgui/mmstextbox.h"
#include "mmsgui/mmsarrow.h"
#include "mmsgui/mmsslider.h"
#include "mmsgui/theme/mmsthememanager.h"


MMSDialogManager::MMSDialogManager() {
	rootWindow = NULL;
}
  
MMSDialogManager::~MMSDialogManager() {
    if (rootWindow)
        delete rootWindow;
}

void MMSDialogManager::insertNamedWidget(MMSWidget *widget) {
    namedWidgets.push_back(widget);
}


MMSWidget* MMSDialogManager::searchForWidget(string name) {
    if (this->rootWindow)
        return this->rootWindow->searchForWidget(name);
    else
        return NULL;
}

MMSWidget* MMSDialogManager::operator[](string name) {
    MMSWidget *widget;

    if ((widget = searchForWidget(name)))
        return widget;
    throw new MMSDialogManagerError(1, "widget " + name + " not found");
}



MMSWindow* MMSDialogManager::loadDialog(string filename, MMSTheme *theme) {

	xmlDoc *parser = NULL;
	
	LIBXML_TEST_VERSION
	
	try {
		parser = xmlReadFile(filename.c_str(), NULL, 0);
		
		if (!parser)
			throw new MMSDialogManagerError(1, "could not parse file");

        //Walk the tree:
		xmlNode* pNode = xmlDocGetRootElement(parser);
  		if (xmlStrcmp(pNode->name, (const xmlChar *) XML_ID_DIALOG))
            throw new MMSDialogManagerError(1, "invalid dialog file");
        this->throughDoc(pNode, NULL, this->rootWindow, theme);

	    /* free the document */
	    xmlFreeDoc(parser);
	}
	catch(MMSError *error) {
	    /* free the document */
    	if (parser)
    		xmlFreeDoc(parser);
        throw new MMSDialogManagerError(1, "could not load dialog file " + filename + ": " + error->getMessage());
	}
	
	return rootWindow;
}

MMSChildWindow* MMSDialogManager::loadChildDialog(string filename, MMSTheme *theme) {
    unsigned int cw_size = childWins.size();

	xmlDoc *parser = NULL;
	
	LIBXML_TEST_VERSION
	
    try {
		parser = xmlReadFile(filename.c_str(), NULL, 0);
		
		if (!parser)
			throw new MMSDialogManagerError(1, "could not parse file");

        //Walk the tree:
		xmlNode* pNode = xmlDocGetRootElement(parser);

		if (xmlStrcmp(pNode->name, (const xmlChar *) XML_ID_DIALOG))
            throw new MMSDialogManagerError(1, "invalid dialog file");

  		if (!this->rootWindow)
            throw new MMSDialogManagerError(1, "no root window loaded");

        this->throughDoc(pNode, NULL, this->rootWindow, theme);

	    /* free the document */
	    xmlFreeDoc(parser);
    }
	catch(MMSError *error) {
	    /* free the document */
    	if (parser)
    		xmlFreeDoc(parser);
        throw new MMSDialogManagerError(1, "could not load dialog file " + filename + ": " + error->getMessage());
	}

    if (cw_size < childWins.size())
        return childWins.at(cw_size);
    else
        return NULL;
}

MMSDescriptionClass MMSDialogManager::getDescription() {
    return this->description;
}

void MMSDialogManager::throughDoc(xmlNode* node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    vector<string> widgetNames;

    /* iterate through childs */
	for (xmlNode *cur_node = node->children; cur_node; cur_node = cur_node->next) {
		string name = (string)(char *)cur_node->name;
		if(name == XML_ID_DESC)
	        getDescriptionValues(cur_node);
        else if(name == XML_ID_MAINWIN)
            getMainWindowValues(cur_node, theme);
        else if(name == XML_ID_POPUPWIN)
            getPopupWindowValues(cur_node, theme);
        else if(name == XML_ID_ROOTWIN)
            getRootWindowValues(cur_node, theme);
        else if(name == XML_ID_CHILDWIN)
            getChildWindowValues(cur_node, rootWindow, theme);
        else {
            string widgetName="";
            if(name == XML_ID_TEMPLATE)
                widgetName = getTemplateValues(cur_node, currentWidget, rootWindow, theme);
            else if(name == XML_ID_VBOX)
                widgetName = getVBoxValues(cur_node, currentWidget, rootWindow, theme);
    	    else if(name == XML_ID_HBOX)
    	        widgetName = getHBoxValues(cur_node, currentWidget, rootWindow, theme);
    	    else if(name == XML_ID_LABEL)
    	        widgetName = getLabelValues(cur_node, currentWidget, rootWindow, theme);
    	    else if(name == XML_ID_BUTTON)
    	        widgetName = getButtonValues(cur_node, currentWidget, rootWindow, theme);
    	    else if(name == XML_ID_IMAGE)
    	        widgetName = getImageValues(cur_node, currentWidget, rootWindow, theme);
    	    else if(name == XML_ID_PROGRESSBAR)
    	        widgetName = getProgressBarValues(cur_node, currentWidget, rootWindow, theme);
            else if(name == XML_ID_MENU)
                widgetName = getMenuValues(cur_node, currentWidget, rootWindow, theme);
            else if(name == XML_ID_TEXTBOX)
                widgetName = getTextBoxValues(cur_node, currentWidget, rootWindow, theme);
            else if(name == XML_ID_ARROW)
                widgetName = getArrowValues(cur_node, currentWidget, rootWindow, theme);
            else if(name == XML_ID_SLIDER)
                widgetName = getSliderValues(cur_node, currentWidget, rootWindow, theme);

            if(widgetName != "") { 
                /* search for duplicate names for the same parent */
                for (int i = 0; i < widgetNames.size(); i++)
                    if (widgetNames.at(i) == widgetName)
                        throw new MMSDialogManagerError(1, "duplicate widget name: " + widgetName);
                widgetNames.push_back(widgetName);
            }
        }
	}
}

void MMSDialogManager::getDescriptionValues(xmlNode *node) {

    this->description.setAttributesFromXMLNode(node);
}

void MMSDialogManager::getMainWindowValues(xmlNode *node, MMSTheme *theme) {
    MMSMainWindowClass themeClass;
    string             name = "", dx = "", dy = "", width = "", height = "";
    
    if(this->rootWindow)
        throw new MMSDialogManagerError(1, "found nested windows, new mainwindow rejected");

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    themeClass.windowClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.windowClass.setAttributesFromXMLNode(node, themePath);
    themeClass.setAttributesFromXMLNode(node, themePath);

    if (themeClass.windowClass.getDx(dx))
        if (getPixelFromSizeHint(NULL, dx, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window dx '" + dx + "'");

    if (themeClass.windowClass.getDy(dy))
        if (getPixelFromSizeHint(NULL, dy, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window dy '" + dy + "'");

    if (themeClass.windowClass.getWidth(width))
        if (getPixelFromSizeHint(NULL, width, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window width '" + width + "'");

    if (themeClass.windowClass.getHeight(height))
        if (getPixelFromSizeHint(NULL, height, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window height '" + height + "'");

    bool os;
    bool *osp = NULL;
    if (themeClass.windowClass.getOwnSurface(os))
    	osp = &os;

    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
    }
    endXMLScan

    MMSALIGNMENT alignment;
    if (!themeClass.windowClass.getAlignment(alignment))
    	alignment = MMSALIGNMENT_NOTSET;
    
    if ((themeClass.windowClass.isDx())||(themeClass.windowClass.isDy()))
        this->rootWindow = new MMSMainWindow(themeClass.getClassName(),
                                             dx,
                                             dy,
                                             width,
                                             height,
                                             alignment,
                                             MMSW_NONE,
                                             theme,
                                             osp);
    else
        this->rootWindow = new MMSMainWindow(themeClass.getClassName(),
                                             width,
                                             height,
                                             alignment,
                                             MMSW_NONE,
                                             theme,
                                             osp);

    this->rootWindow->setName(name);
    ((MMSMainWindow*)this->rootWindow)->updateFromThemeClass(&themeClass);

    throughDoc(node, NULL, this->rootWindow, theme);
}


void MMSDialogManager::getPopupWindowValues(xmlNode *node, MMSTheme *theme) {
    MMSPopupWindowClass themeClass;
    string              name = "", dx = "", dy = "", width = "", height = "";
    
    if(this->rootWindow)
        throw new MMSDialogManagerError(1, "found nested windows, new popupwindow rejected");

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    themeClass.windowClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.windowClass.setAttributesFromXMLNode(node, themePath);
    themeClass.setAttributesFromXMLNode(node, themePath);

    if (themeClass.windowClass.getDx(dx))
        if (getPixelFromSizeHint(NULL, dx, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window dx '" + dx + "'");

    if (themeClass.windowClass.getDy(dy))
        if (getPixelFromSizeHint(NULL, dy, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window dy '" + dy + "'");

    if (themeClass.windowClass.getWidth(width))
        if (getPixelFromSizeHint(NULL, width, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window width '" + width + "'");

    if (themeClass.windowClass.getHeight(height))
        if (getPixelFromSizeHint(NULL, height, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window height '" + height + "'");
    
    bool os;
    bool *osp = NULL;
    if (themeClass.windowClass.getOwnSurface(os))
    	osp = &os;

    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
    }
    endXMLScan

    MMSALIGNMENT alignment;
    if (!themeClass.windowClass.getAlignment(alignment))
    	alignment = MMSALIGNMENT_NOTSET;

    if ((themeClass.windowClass.isDx())||(themeClass.windowClass.isDy()))
        this->rootWindow = new MMSPopupWindow(themeClass.getClassName(),
							                  dx,
								              dy,
								              width,
								              height,
								              alignment,
                                              MMSW_NONE,
                                              theme,
                                              osp,
                                              0);
    else
        this->rootWindow = new MMSPopupWindow(themeClass.getClassName(),
								              width,
								              height,
								              alignment,
                                              MMSW_NONE,
                                              theme,
                                              osp,
                                              0);

    this->rootWindow->setName(name);
    ((MMSPopupWindow*)this->rootWindow)->updateFromThemeClass(&themeClass);

    throughDoc(node, NULL, this->rootWindow, theme);
}

void MMSDialogManager::getRootWindowValues(xmlNode *node, MMSTheme *theme) {
    MMSRootWindowClass themeClass;
    string             name = "", dx = "", dy = "", width = "", height = "";
    
    if(this->rootWindow)
        throw new MMSDialogManagerError(1, "found nested windows, new rootwindow rejected");

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    themeClass.windowClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.windowClass.setAttributesFromXMLNode(node, themePath);
    themeClass.setAttributesFromXMLNode(node, themePath);

    if (themeClass.windowClass.getDx(dx))
        if (getPixelFromSizeHint(NULL, dx, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window dx '" + dx + "'");

    if (themeClass.windowClass.getDy(dy))
        if (getPixelFromSizeHint(NULL, dy, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window dy '" + dy + "'");

    if (themeClass.windowClass.getWidth(width))
        if (getPixelFromSizeHint(NULL, width, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window width '" + width + "'");

    if (themeClass.windowClass.getHeight(height))
        if (getPixelFromSizeHint(NULL, height, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window height '" + height + "'");

    bool os;
    bool *osp = NULL;
    if (themeClass.windowClass.getOwnSurface(os))
    	osp = &os;

    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
    }
    endXMLScan

    MMSALIGNMENT alignment;
    if (!themeClass.windowClass.getAlignment(alignment))
    	alignment = MMSALIGNMENT_NOTSET;

    if ((themeClass.windowClass.isDx())||(themeClass.windowClass.isDy()))
        this->rootWindow = new MMSRootWindow(themeClass.getClassName(),
								             dx,
								             dy,
								             width,
								             height,
								             alignment,
                                             MMSW_NONE,
                                             theme,
                                             osp);
    else
        this->rootWindow = new MMSRootWindow(themeClass.getClassName(),
								             width,
								             height,
								             alignment,
                                             MMSW_NONE,
                                             theme,
                                             osp);

    this->rootWindow->setName(name);
    ((MMSRootWindow*)this->rootWindow)->updateFromThemeClass(&themeClass);

    throughDoc(node, NULL, this->rootWindow, theme);
}

void MMSDialogManager::getChildWindowValues(xmlNode *node, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSChildWindowClass themeClass;
    MMSChildWindow      *childwin;
    string              name = "", dx = "", dy = "", width = "", height = "";
    bool                show = false;
    
    if(!rootWindow)
        throw new MMSDialogManagerError(1, "no parent window found, new childwindow rejected");

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    themeClass.windowClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.windowClass.setAttributesFromXMLNode(node, themePath);
    themeClass.setAttributesFromXMLNode(node, themePath);

    if (themeClass.windowClass.getDx(dx))
        if (getPixelFromSizeHint(NULL, dx, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window dx '" + dx + "'");

    if (themeClass.windowClass.getDy(dy))
        if (getPixelFromSizeHint(NULL, dy, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window dy '" + dy + "'");

    if (themeClass.windowClass.getWidth(width))
        if (getPixelFromSizeHint(NULL, width, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window width '" + width + "'");

    if (themeClass.windowClass.getHeight(height))
        if (getPixelFromSizeHint(NULL, height, 10000, 0) == false)
            throw new MMSDialogManagerError(1, "invalid window height '" + height + "'");

    bool os;
    bool *osp = NULL;
    if (themeClass.windowClass.getOwnSurface(os))
    	osp = &os;
    
    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
        if(attrName == "show")
            show = (attrValue == "true") ? true : false;
    }
    endXMLScan

    MMSALIGNMENT alignment;
    if (!themeClass.windowClass.getAlignment(alignment))
    	alignment = MMSALIGNMENT_NOTSET;

    if ((themeClass.windowClass.isDx())||(themeClass.windowClass.isDy()))
        childwin = new MMSChildWindow(themeClass.getClassName(),
                                      rootWindow,
                                      dx,
                                      dy,
                                      width,
                                      height,
                                      alignment,
                                      MMSW_NONE,
                                      theme,
                                      osp);
    else
        childwin = new MMSChildWindow(themeClass.getClassName(),
                                      rootWindow,
                                      width,
                                      height,
                                      alignment,
                                      MMSW_NONE,
                                      theme,
                                      osp);

    childWins.push_back(childwin);
    childwin->setName(name);
    childwin->updateFromThemeClass(&themeClass);

    throughDoc(node, NULL, childwin, theme);

    if (show) childwin->show();
}

string MMSDialogManager::getTemplateValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSTemplateClass    themeClass, *templateClass;
    MMSHBox             *hbox;
    string              name = "";
    xmlNode         	*xmlNode;
    vector<string>      widgetNames;

    /* read settings from dialog */    
    themeClass.setAttributesFromXMLNode(node);

    /* is a class name given? */
    if (themeClass.getClassName()=="")
        return "";

    /* can load templateClass? */
    if (theme) {
        if (!(templateClass = theme->getTemplateClass(themeClass.getClassName())))
            return "";
    }
    else {
        if (!(templateClass = globalTheme->getTemplateClass(themeClass.getClassName())))
            return "";
    }

    /* are there any childs stored in the templateClass? */
    if (!(xmlNode = templateClass->getXMLNode()))
        return "";

    /* search for attributes which are only supported within dialog */  
    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;

        if(attrName.substr(0, 7) == "widget.") {
            /* search for attributes which are to be set for templates child widgets */
            string widgetName = attrName.substr(7);
            int pos = (int)widgetName.find(".");
            if (pos > 0) {
                widgetName = widgetName.substr(0, pos);

                /* store the requested widget name here */
                widgetNames.push_back(widgetName);
            }  
        }
    }
    endXMLScan

    /* create new hbox as container for the template */
    hbox = new MMSHBox(rootWindow);

    /* add to widget vector if named */
    if(name != "") { 
        hbox->setName(name);
        insertNamedWidget(hbox);
    }

    if (currentWidget)
        currentWidget->add(hbox);
    else
        rootWindow->add(hbox);

    /* yes, parse the childs from templateClass */
    throughDoc(xmlNode, hbox, rootWindow, theme);

    /* for each child widget which is named by attribute */
    for (unsigned int i = 0; i < widgetNames.size(); i++) {
        MMSWidget *mywidget = hbox->searchForWidget(widgetNames.at(i));
        if (mywidget) {
            /* widget found */
            string prefix = "widget." + widgetNames.at(i) + ".";
            switch (mywidget->getType()) {
                case MMSWIDGETTYPE_HBOX:
                    break;
                case MMSWIDGETTYPE_VBOX:
                    break;
                case MMSWIDGETTYPE_BUTTON:
                    {
                        /* read attributes from dialog */
                        MMSButtonClass themeCls;
                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                        /* apply settings from dialog */
                        ((MMSButton*)mywidget)->updateFromThemeClass(&themeCls);
                    }
                    break;
                case MMSWIDGETTYPE_IMAGE:
                    {
                        /* read attributes from dialog */
                        MMSImageClass themeCls;
                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                        /* apply settings from dialog */
                        ((MMSImage*)mywidget)->updateFromThemeClass(&themeCls);
                    }
                    break;
                case MMSWIDGETTYPE_LABEL:
                    {
                        /* read attributes from dialog */
                        MMSLabelClass themeCls;
                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                        /* apply settings from dialog */
                        ((MMSLabel*)mywidget)->updateFromThemeClass(&themeCls);
                    }
                    break;
                case MMSWIDGETTYPE_MENU:
                    {
                        /* read attributes from dialog */
                        MMSMenuClass themeCls;
                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                        /* apply settings from dialog */
                        ((MMSMenu*)mywidget)->updateFromThemeClass(&themeCls);
                    }
                    break;
                case MMSWIDGETTYPE_PROGRESSBAR:
                    {
                        /* read attributes from dialog */
                        MMSProgressBarClass themeCls;
                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                        /* apply settings from dialog */
                        ((MMSProgressBar*)mywidget)->updateFromThemeClass(&themeCls);
                    }
                    break;
                case MMSWIDGETTYPE_TEXTBOX:
                    {
                        /* read attributes from dialog */
                        MMSTextBoxClass themeCls;
                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                        /* apply settings from dialog */
                        ((MMSTextBox*)mywidget)->updateFromThemeClass(&themeCls);
                    }
                    break;
                case MMSWIDGETTYPE_ARROW:
                    {
                        /* read attributes from dialog */
                        MMSArrowClass themeCls;
                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                        /* apply settings from dialog */
                        ((MMSArrow*)mywidget)->updateFromThemeClass(&themeCls);
                    }
                    break;
                case MMSWIDGETTYPE_SLIDER:
                    {
                        /* read attributes from dialog */
                        MMSSliderClass themeCls;
                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                        /* apply settings from dialog */
                        ((MMSSlider*)mywidget)->updateFromThemeClass(&themeCls);
                    }
                    break;
            }
        }        
    } 
    
    /* return the name of the widget */
    return name;
}


string MMSDialogManager::getVBoxValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSVBox *vbox;
    string  name = "";
    string  size = "";
	
    startXMLScan
    {
	    if(attrName == "name")
	        name = attrValue;
	    else if(attrName == "size")
	        size = attrValue;
	}
    endXMLScan
    
	vbox = new MMSVBox(rootWindow);

    /* add to widget vector if named */
    if(name != "") { 
        vbox->setName(name);
        insertNamedWidget(vbox);
    }

	if(size != "") 
	    vbox->setSizeHint(size);

	if (currentWidget)
        currentWidget->add(vbox);
    else
        rootWindow->add(vbox);

	throughDoc(node, vbox, rootWindow, theme);	
    
    /* return the name of the widget */
    return name;
}

string MMSDialogManager::getHBoxValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSHBox *hbox;
    string  name = "";
    string  size = "";
	
    startXMLScan
    {
	    if(attrName == "name")
	        name = attrValue;
	    else if(attrName == "size")
	        size = attrValue;
	}
    endXMLScan
	
	hbox = new MMSHBox(rootWindow);

    /* add to widget vector if named */
    if(name != "") { 
        hbox->setName(name);
        insertNamedWidget(hbox);
    }

	if(size != "") 
	    hbox->setSizeHint(size);

    if (currentWidget)
        currentWidget->add(hbox);
    else
        rootWindow->add(hbox);

	throughDoc(node, hbox, rootWindow, theme);	
    
    /* return the name of the widget */
    return name;
}


string MMSDialogManager::getLabelValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSLabelClass   themeClass;
    MMSLabel        *label;
    string          name = "";
    string          size = "";

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    /* read settings from dialog */    
    themeClass.widgetClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.widgetClass.setAttributesFromXMLNode(node, "", themePath);
    themeClass.setAttributesFromXMLNode(node, "", themePath);

    /* create new label from theme class */
    label = new MMSLabel(rootWindow, themeClass.getClassName(), theme);

    /* apply settings from dialog */
    label->updateFromThemeClass(&themeClass);

    /* search for attributes which are only supported within dialog */  
    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
        else if(attrName == "size")
            size = attrValue;
    }
    endXMLScan

    /* add to widget vector if named */
    if(name != "") { 
        label->setName(name);
        insertNamedWidget(label);
    }

    if(size != "") {
        if (!label->setSizeHint(size))
            throw new MMSDialogManagerError(1, "invalid widget size '" + size + "'");
    }

    if (currentWidget)
        currentWidget->add(label);
    else
        rootWindow->add(label);
    
    throughDoc(node, label, rootWindow, theme);
    
    /* return the name of the widget */
    return name;
}


string MMSDialogManager::getButtonValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSButtonClass  themeClass;
    MMSButton       *button;
    string          name = "";
    string          size = "";

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    /* read settings from dialog */    
    themeClass.widgetClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.widgetClass.setAttributesFromXMLNode(node, "", themePath);
    themeClass.setAttributesFromXMLNode(node, "", themePath);

    /* create new button from theme class */
    button = new MMSButton(rootWindow, themeClass.getClassName(), theme);

    /* apply settings from dialog */
    button->updateFromThemeClass(&themeClass);

    /* search for attributes which are only supported within dialog */  
    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
        else if(attrName == "size")
            size = attrValue;
    }
    endXMLScan

    /* add to widget vector if named */
    if(name != "") { 
        button->setName(name);
        insertNamedWidget(button);
    }

    if(size != "") {
        if (!button->setSizeHint(size))
            throw new MMSDialogManagerError(1, "invalid widget size '" + size + "'");
    }

    if (currentWidget)
        currentWidget->add(button);
    else
        rootWindow->add(button);
    
    throughDoc(node, button, rootWindow, theme);
    
    /* return the name of the widget */
    return name;
}

string MMSDialogManager::getImageValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSImageClass   themeClass;
    MMSImage        *image;
    string          name = "";
    string          size = "";

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    /* read settings from dialog */    
    themeClass.widgetClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.widgetClass.setAttributesFromXMLNode(node, "", themePath);
    themeClass.setAttributesFromXMLNode(node, "", themePath);

    /* create new image from theme class */
    image = new MMSImage(rootWindow, themeClass.getClassName(), theme);

    /* apply settings from dialog */
    image->updateFromThemeClass(&themeClass);

    /* search for attributes which are only supported within dialog */  
    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
        else if(attrName == "size")
            size = attrValue;
    }
    endXMLScan

    /* add to widget vector if named */
    if(name != "") { 
        image->setName(name);
        insertNamedWidget(image);
    }

    if(size != "") {
        if (!image->setSizeHint(size))
            throw new MMSDialogManagerError(1, "invalid widget size '" + size + "'");
    }

    if (currentWidget)
        currentWidget->add(image);
    else
        rootWindow->add(image);
    
    throughDoc(node, image, rootWindow, theme);
    
    /* return the name of the widget */
    return name;
}


string MMSDialogManager::getProgressBarValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSProgressBarClass themeClass;
    MMSProgressBar      *pBar;
    string              name = "";
    string              size = "";

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    /* read settings from dialog */    
    themeClass.widgetClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.widgetClass.setAttributesFromXMLNode(node, "", themePath);
    themeClass.setAttributesFromXMLNode(node, "", themePath);

    /* create new progressbar from theme class */
    pBar = new MMSProgressBar(rootWindow, themeClass.getClassName(), theme);

    /* apply settings from dialog */
    pBar->updateFromThemeClass(&themeClass);

    /* search for attributes which are only supported within dialog */  
    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
        else if(attrName == "size")
            size = attrValue;
    }
    endXMLScan

    /* add to widget vector if named */
    if(name != "") { 
        pBar->setName(name);
        insertNamedWidget(pBar);
    }

    if(size != "") {
        if (!pBar->setSizeHint(size))
            throw new MMSDialogManagerError(1, "invalid widget size '" + size + "'");
    }

    if (currentWidget)
        currentWidget->add(pBar);
    else
        rootWindow->add(pBar);
    
    throughDoc(node, pBar, rootWindow, theme);
    
    /* return the name of the widget */
    return name;
}


string MMSDialogManager::getMenuValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSMenuClass    themeClass;
    MMSMenu         *menu;
    string          name = "";
    string          size = "";
    xmlNode     	*xNode;

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    /* read settings from dialog */    
    themeClass.widgetClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.widgetClass.setAttributesFromXMLNode(node, "", themePath);
    themeClass.setAttributesFromXMLNode(node, "", themePath);

    /* create new menu from theme class */
    menu = new MMSMenu(rootWindow, themeClass.getClassName(), theme);

    /* apply settings from dialog */
    menu->updateFromThemeClass(&themeClass);

    /* search for attributes which are only supported within dialog */  
    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
        else if(attrName == "size")
            size = attrValue;
    }
    endXMLScan

    /* add to widget vector if named */
    if(name != "") { 
        menu->setName(name);
        insertNamedWidget(menu);
    }

    if(size != "") {
        if (!menu->setSizeHint(size))
            throw new MMSDialogManagerError(1, "invalid widget size '" + size + "'");
    }

    if (currentWidget)
        currentWidget->add(menu);
    else
        rootWindow->add(menu);
    
    /* set the item layout, we need a temporary parent widget */
    MMSHBox *tmpWidget = new MMSHBox(NULL);

    /* are there any childs stored in the theme? */
    if ((xNode = menu->getXMLNode()))
        /* yes, parse the childs from theme */
        throughDoc(xNode, tmpWidget, NULL, theme);
    else
        /* no, parse the childs from dialog file */
        throughDoc(node, tmpWidget, NULL, theme);

    bool haveItemTemplate = false;
    MMSWidget *itemTemplate = tmpWidget->disconnectChild();
    if (itemTemplate) {
        menu->setItemTemplate(itemTemplate);
        haveItemTemplate = true;
    }
    else {
        if (xNode) {
            /* try with theme failed, retry with childs from dialog file */
            throughDoc(node, tmpWidget, NULL, theme);
            MMSWidget *itemTemplate = tmpWidget->disconnectChild();
            if (itemTemplate) {
                menu->setItemTemplate(itemTemplate);
                haveItemTemplate = true;
            }
        }
    }

    delete tmpWidget;

    if (haveItemTemplate) {
        /* have a template - search for menü items which are set in the dialog file */
        bool haveItems = false;

        /* iterate through childs */
    	for (xmlNode *cur_node = node->children; cur_node; cur_node = cur_node->next) {
    		string name = (string)(char *)cur_node->name;
    		node = cur_node;
    		
            /* check if a <menuitem> is given */
            if(name != XML_ID_MENUITEM)
                continue;
    		
    		
            /* create new menu item */
            MMSWidget *topwidget = menu->newItem();
            haveItems = true;

            startXMLScan
            {
                if(attrName.substr(0, 7) == "widget.") {
                    /* search for attributes which are to be set for menu items child widgets */
                    string widgetName = attrName.substr(7);
                    int pos = (int)widgetName.find(".");
                    if (pos > 0) {
                        /* widget name found */
                        widgetName = widgetName.substr(0, pos);
                        MMSWidget *widget;
                        if (topwidget->getName() == widgetName)
                            widget = topwidget;
                        else
                            widget = topwidget->searchForWidget(widgetName);

                        if (widget) {
                            /* widget found */
                            /* add attribute to widget */
                            string prefix = "widget." + widgetName + ".";
                            switch (widget->getType()) {
                                case MMSWIDGETTYPE_HBOX:
                                    break;
                                case MMSWIDGETTYPE_VBOX:
                                    break;
                                case MMSWIDGETTYPE_BUTTON:
                                    {
                                        /* read attributes from node */
                                        MMSButtonClass themeCls;
                                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                                        /* apply settings from node */
                                        ((MMSButton*)widget)->updateFromThemeClass(&themeCls);
                                    }
                                    break;
                                case MMSWIDGETTYPE_IMAGE:
                                    {
                                        /* read attributes from node */
                                        MMSImageClass themeCls;
                                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                                        /* apply settings from node */
                                        ((MMSImage*)widget)->updateFromThemeClass(&themeCls);
                                    }
                                    break;
                                case MMSWIDGETTYPE_LABEL:
                                    {
                                        /* read attributes from node */
                                        MMSLabelClass themeCls;
                                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                                        /* apply settings from node */
                                        ((MMSLabel*)widget)->updateFromThemeClass(&themeCls);
                                    }
                                    break;
                                case MMSWIDGETTYPE_MENU:
                                    break;
                                case MMSWIDGETTYPE_PROGRESSBAR:
                                    {
                                        /* read attributes from node */
                                        MMSProgressBarClass themeCls;
                                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                                        /* apply settings from node */
                                        ((MMSProgressBar*)widget)->updateFromThemeClass(&themeCls);
                                    }
                                    break;
                                case MMSWIDGETTYPE_TEXTBOX:
                                    {
                                        /* read attributes from node */
                                        MMSTextBoxClass themeCls;
                                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                                        /* apply settings from node */
                                        ((MMSTextBox*)widget)->updateFromThemeClass(&themeCls);
                                    }
                                    break;
                                case MMSWIDGETTYPE_ARROW:
                                    {
                                        /* read attributes from node */
                                        MMSArrowClass themeCls;
                                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                                        /* apply settings from node */
                                        ((MMSArrow*)widget)->updateFromThemeClass(&themeCls);
                                    }
                                    break;
                                case MMSWIDGETTYPE_SLIDER:
                                    {
                                        /* read attributes from node */
                                        MMSSliderClass themeCls;
                                        themeCls.widgetClass.border.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.widgetClass.setAttributesFromXMLNode(node, prefix, "");
                                        themeCls.setAttributesFromXMLNode(node, prefix, "");
                                        /* apply settings from node */
                                        ((MMSSlider*)widget)->updateFromThemeClass(&themeCls);
                                    }
                                    break;
                            }
                        }
                    }  
                }
            }
            endXMLScan
        }

        if (haveItems)
            menu->setFocus(false, false);
    }
    
    /* return the name of the widget */
    return name;
}


string MMSDialogManager::getTextBoxValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSTextBoxClass themeClass;
    MMSTextBox      *textbox;
    string          name = "";
    string          size = "";

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    /* read settings from dialog */    
    themeClass.widgetClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.widgetClass.setAttributesFromXMLNode(node, "", themePath);
    themeClass.setAttributesFromXMLNode(node, "", themePath);

    /* create new textbox from theme class */
    textbox = new MMSTextBox(rootWindow, themeClass.getClassName(), theme);

    /* apply settings from dialog */
    textbox->updateFromThemeClass(&themeClass);

    /* search for attributes which are only supported within dialog */  
    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
        else if(attrName == "size")
            size = attrValue;
    }
    endXMLScan

    /* add to widget vector if named */
    if(name != "") { 
        textbox->setName(name);
        insertNamedWidget(textbox);
    }

    if(size != "") {
        if (!textbox->setSizeHint(size))
            throw new MMSDialogManagerError(1, "invalid widget size '" + size + "'");
    }

    if (currentWidget)
        currentWidget->add(textbox);
    else
        rootWindow->add(textbox);
    
    throughDoc(node, textbox, rootWindow, theme);
    
    /* return the name of the widget */
    return name;
}

string MMSDialogManager::getArrowValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSArrowClass   themeClass;
    MMSArrow        *arrow;
    string          name = "";
    string          size = "";

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    /* read settings from dialog */    
    themeClass.widgetClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.widgetClass.setAttributesFromXMLNode(node, "", themePath);
    themeClass.setAttributesFromXMLNode(node, "", themePath);

    /* create new arrow from theme class */
    arrow = new MMSArrow(rootWindow, themeClass.getClassName(), theme);

    /* apply settings from dialog */
    arrow->updateFromThemeClass(&themeClass);

    /* search for attributes which are only supported within dialog */  
    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
        else if(attrName == "size")
            size = attrValue;
    }
    endXMLScan

    /* add to widget vector if named */
    if(name != "") { 
        arrow->setName(name);
        insertNamedWidget(arrow);
    }

    if(size != "") {
        if (!arrow->setSizeHint(size))
            throw new MMSDialogManagerError(1, "invalid widget size '" + size + "'");
    }

    if (currentWidget)
        currentWidget->add(arrow);
    else
        rootWindow->add(arrow);
    
    throughDoc(node, arrow, rootWindow, theme);
    
    /* return the name of the widget */
    return name;
}

string MMSDialogManager::getSliderValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme) {
    MMSSliderClass  themeClass;
    MMSSlider       *slider;
    string          name = "";
    string          size = "";

    /* get themepath */
    string themePath = "";
    if (theme)
        themePath = theme->getThemePath();
    else
        themePath = globalTheme->getThemePath();

    /* read settings from dialog */    
    themeClass.widgetClass.border.setAttributesFromXMLNode(node, "", themePath);
    themeClass.widgetClass.setAttributesFromXMLNode(node, "", themePath);
    themeClass.setAttributesFromXMLNode(node, "", themePath);

    /* create new slider from theme class */
    slider = new MMSSlider(rootWindow, themeClass.getClassName(), theme);

    /* apply settings from dialog */
    slider->updateFromThemeClass(&themeClass);

    /* search for attributes which are only supported within dialog */  
    startXMLScan
    {
        if(attrName == "name")
            name = attrValue;
        else if(attrName == "size")
            size = attrValue;
    }
    endXMLScan

    /* add to widget vector if named */
    if(name != "") { 
        slider->setName(name);
        insertNamedWidget(slider);
    }

    if(size != "") {
        if (!slider->setSizeHint(size))
            throw new MMSDialogManagerError(1, "invalid widget size '" + size + "'");
    }

    if (currentWidget)
        currentWidget->add(slider);
    else
        rootWindow->add(slider);
    
    throughDoc(node, slider, rootWindow, theme);
    
    /* return the name of the widget */
    return name;
}



