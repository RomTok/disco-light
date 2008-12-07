/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#ifndef MMSFILEDIALOG_H_
#define MMSFILEDIALOG_H_

#include "mmsgui/mmswindows.h"
#include "mmsgui/mmswidgets.h"
#include "mmsgui/mmsdialogmanager.h"

class MMSFileDialog {
    private:
    	string 				dialogfile;
    	MMSDialogManager	*dm;
    	MMSWindow			*dialogwindow;

    	MMSLabelWidget		*filedialog_title;

    	MMSWidget			*filedialog_ok;
    	MMSWidget			*filedialog_cancel;

    	MMSLabelWidget		*filedialog_path;
    	MMSInputWidget		*filedialog_name;

    	MMSMenuWidget		*filedialog_filelist;

    	MMSButtonWidget		*filedialog_up;
    	MMSButtonWidget		*filedialog_down;

    	string				path;
    	string				filename;

    	void onReturn(MMSWidget *widget);
    	void onSelectItem(MMSWidget *widget);

    	bool fillMenu();

    public:
        MMSFileDialog();
        MMSFileDialog(string path, string filename, MMSWindow *dialogwindow = NULL);
        ~MMSFileDialog();
        bool loadFileDialog(MMSWindow *parent, string dialogfile = "", MMSTheme *theme = NULL);
        bool isInitialized();

        bool setTitle(string title);

        bool show();

        sigc::signal<void, MMSFileDialog*> *onOK;
        sigc::signal<void> *onCancel;
};

#endif /*MMSFILEDIALOG_H_*/
