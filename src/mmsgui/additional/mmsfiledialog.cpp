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

MMSFileDialog::MMSFileDialog(string dialogfile, MMSTheme *theme) {
	this->dialogfile = dialogfile;
	if (this->dialogfile != "")
		this->dialogwindow = this->dm.loadDialog(this->dialogfile, theme);
	else
		this->dialogwindow = this->dm.loadDialog("share/disko/mmsfiledialog.xml", theme);
}

MMSFileDialog::MMSFileDialog(MMSWindow *dialogwindow) {
	this->dialogwindow = dialogwindow;
}

MMSFileDialog::~MMSFileDialog() {
	if (this->dm.isLoaded()) {
		// i have loaded the dialogfile, so i must delete it
		if (this->dialogwindow)
			delete this->dialogwindow;
	}
}

bool MMSFileDialog::show() {
	if (!this->dialogwindow) return false;
	
	this->dialogwindow->show();
	return true;
}

