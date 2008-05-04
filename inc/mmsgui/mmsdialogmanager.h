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
#include "mmstools/mmstools.h"

/* string constants for xml identifiers */
#define XML_ID_DIALOG       "mmsdialog"
#define XML_ID_VBOX         "vbox"
#define XML_ID_HBOX         "hbox"

class MMSDialogManager {
	private:
        MMSWindow               *rootWindow;

        vector<MMSChildWindow*> childWins;

        string              filename;
        MMSDescriptionClass description;    /* description of the dialog */

        vector<MMSWidget*>  namedWidgets;

        void insertNamedWidget(MMSWidget *widget);

        void throughDoc(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow,
        				MMSTheme *theme, bool only_first_child = false);

        void getDescriptionValues(MMSTaffFile *tafff, MMSTheme *theme);

        void getMainWindowValues(MMSTaffFile *tafff, MMSTheme *theme);
        void getPopupWindowValues(MMSTaffFile *tafff, MMSTheme *theme);
        void getRootWindowValues(MMSTaffFile *tafff, MMSTheme *theme);
        void getChildWindowValues(MMSTaffFile *tafff, MMSWindow *rootWindow, MMSTheme *theme);
        string getTemplateValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getVBoxValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getHBoxValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getLabelValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getButtonValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getImageValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getProgressBarValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getMenuValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getTextBoxValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getArrowValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getSliderValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        
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
