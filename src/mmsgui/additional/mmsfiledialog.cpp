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

#include "mmsgui/additional/mmsfiledialog.h"

MMSFileDialog::MMSFileDialog() {
	this->dm = NULL;
	this->dialogwindow = NULL;
}

MMSFileDialog::MMSFileDialog(MMSWindow *dialogwindow) {
	this->dm = NULL;
	this->dialogwindow = dialogwindow;
}

MMSFileDialog::~MMSFileDialog() {
	if (this->dm) {
		if (this->dm->isLoaded()) {
			// i have loaded the dialogwindow, so i must delete it
			if (this->dialogwindow)
				delete this->dialogwindow;
		}
		delete dm;
	}
}


bool MMSFileDialog::loadFileDialog(MMSWindow *parent, string dialogfile, MMSTheme *theme) {
	this->dm = new MMSDialogManager(parent);
	this->dialogfile = dialogfile;
	this->dialogwindow = NULL;

	if (this->dialogfile != "")
		// load a user specified dialog file
		this->dialogwindow = this->dm->loadDialog(this->dialogfile, theme);
	else
	if (parent)
		// load the default dialog file which includes a child window
		// do this only if a parent window is given!!!
		this->dialogwindow = this->dm->loadChildDialog("share/disko/mmsfiledialog.xml", theme);
}

bool MMSFileDialog::isInitialized() {
	return (this->dialogwindow);
}

bool MMSFileDialog::show() {
	if (!isInitialized()) return false;
	
	this->dialogwindow->show();
	return true;
}

