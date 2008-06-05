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

#ifndef MMSFILEDIALOG_H_
#define MMSFILEDIALOG_H_

#include "mmsgui/mmswindow.h"
#include "mmsgui/mmsdialogmanager.h"

class MMSFileDialog {
    private:
    	string 				dialogfile;
    	MMSDialogManager	*dm;
    	MMSWindow			*dialogwindow;
    	
    public:
        MMSFileDialog();
        MMSFileDialog(MMSWindow *dialogwindow);
        ~MMSFileDialog();
        bool loadFileDialog(MMSWindow *parent, string dialogfile = "", MMSTheme *theme = NULL);
        bool isInitialized();
        
        bool show();
};

#endif /*MMSFILEDIALOG_H_*/
