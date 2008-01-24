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

#ifndef MMSDIALOGMANAGER_H_
#define MMSDIALOGMANAGER_H_

#include "mmsgui/mmswindow.h"
#include "mmsgui/mmschildwindow.h"
#include "mmsgui/mmsguilogger.h"
#include "mmstools/mmstools.h"

/* string constants for xml identifiers */
#define XML_ID_DIALOG       "mmsdialog"
#define XML_ID_VBOX         "vbox"
#define XML_ID_HBOX         "hbox"

class MMSDialogManager {
	private:
        MMSGuiLogger logger;

        MMSWindow               *rootWindow;

        vector<MMSChildWindow*> childWins;

        string              filename;
        MMSDescriptionClass description;    /* description of the dialog */

        vector<MMSWidget*>  namedWidgets;

        void insertNamedWidget(MMSWidget *widget);

        void throughDoc(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);

        void getDescriptionValues(xmlNode *node);

        void getMainWindowValues(xmlNode *node, MMSTheme *theme);
        void getPopupWindowValues(xmlNode *node, MMSTheme *theme);
        void getRootWindowValues(xmlNode *node, MMSTheme *theme);
        void getChildWindowValues(xmlNode *node, MMSWindow *rootWindow, MMSTheme *theme);
        string getTemplateValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getVBoxValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getHBoxValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getLabelValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getButtonValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getImageValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getProgressBarValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getMenuValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getTextBoxValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getArrowValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getSliderValues(xmlNode *node, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        
    public:
        MMSDialogManager();
        ~MMSDialogManager();
        MMSWindow* loadDialog(string filename, MMSTheme *theme = NULL);
        MMSChildWindow* loadChildDialog(string filename, MMSTheme *theme = NULL);
        MMSWidget* searchForWidget(string name);
        MMSWidget* operator[](string name);

        MMSDescriptionClass getDescription();
};

MMS_CREATEERROR(MMSDialogManagerError);

#endif /*MMSDIALOGMANAGER_H_*/
