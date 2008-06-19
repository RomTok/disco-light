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
#include "mmsinfo/mmsinfo.h"

#define FILEDIALOG_TITLE	"filedialog_title"
#define FILEDIALOG_OK		"filedialog_ok"
#define FILEDIALOG_CANCEL	"filedialog_cancel"
#define FILEDIALOG_PATH		"filedialog_path"
#define FILEDIALOG_NAME		"filedialog_name"
#define FILEDIALOG_FILELIST	"filedialog_filelist"
#define PATH_OR_FILE		"pof"
#define FILEDIALOG_UP		"filedialog_up"
#define FILEDIALOG_DOWN		"filedialog_down"


MMSFileDialog::MMSFileDialog() {
	this->dm = NULL;
	this->path = "/";
	this->filename = "";	
	this->dialogwindow = NULL;
	
	// initialize the callbacks
    this->onOK     = new sigc::signal<void, MMSFileDialog*>;
    this->onCancel = new sigc::signal<void>;
}

MMSFileDialog::MMSFileDialog(string path, string filename, MMSWindow *dialogwindow) {
	this->dm = NULL;
	this->path = path;
	this->filename = filename;
	this->dialogwindow = dialogwindow;
	
	// initialize the callbacks
    this->onOK     = new sigc::signal<void, MMSFileDialog*>;
    this->onCancel = new sigc::signal<void>;
}	

MMSFileDialog::~MMSFileDialog() {
	
	// delete the callbacks
    if (this->onOK)
    	delete this->onOK;
    if (this->onCancel)
    	delete this->onCancel;

    // delete the dialog
    if (this->dm)
		delete dm;
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
		this->dialogwindow = this->dm->loadChildDialog((string)getPrefix() + "share/disko/mmsgui/mmsfiledialog.xml", theme);
	
	if (!this->dialogwindow)
		return false;

	// get access to the widgets
	this->filedialog_title = (MMSLabelWidget*)this->dialogwindow->searchForWidget(FILEDIALOG_TITLE);
	this->filedialog_ok = this->dialogwindow->searchForWidget(FILEDIALOG_OK);
	this->filedialog_cancel = this->dialogwindow->searchForWidget(FILEDIALOG_CANCEL);
	this->filedialog_path = (MMSLabelWidget*)this->dialogwindow->searchForWidget(FILEDIALOG_PATH);
	this->filedialog_name = (MMSInputWidget*)this->dialogwindow->searchForWidget(FILEDIALOG_NAME);
	this->filedialog_filelist = (MMSMenuWidget*)this->dialogwindow->searchForWidget(FILEDIALOG_FILELIST);
	this->filedialog_up = (MMSButtonWidget*)this->dialogwindow->searchForWidget(FILEDIALOG_UP);
	this->filedialog_down = (MMSButtonWidget*)this->dialogwindow->searchForWidget(FILEDIALOG_DOWN);

	// check something and/or connect callbacks if widgets does exist
	if (this->filedialog_title)
		if (this->filedialog_title->getType() != MMSWIDGETTYPE_LABEL)
			this->filedialog_title = NULL;
	if (this->filedialog_ok)
		if (this->filedialog_ok->getType() == MMSWIDGETTYPE_BUTTON)
			this->filedialog_ok->onReturn->connect(sigc::mem_fun(this,&MMSFileDialog::onReturn));
	if (this->filedialog_cancel)
		if (this->filedialog_cancel->getType() == MMSWIDGETTYPE_BUTTON)
			this->filedialog_cancel->onReturn->connect(sigc::mem_fun(this,&MMSFileDialog::onReturn));
	if (this->filedialog_path)
		if (this->filedialog_path->getType() != MMSWIDGETTYPE_LABEL)
			this->filedialog_path = NULL;
	if (this->filedialog_name)
		if (this->filedialog_name->getType() != MMSWIDGETTYPE_INPUT)
			this->filedialog_name = NULL;
	if (this->filedialog_filelist)
		if (this->filedialog_filelist->getType() == MMSWIDGETTYPE_MENU) {
			this->filedialog_filelist->onReturn->connect(sigc::mem_fun(this,&MMSFileDialog::onReturn));
			this->filedialog_filelist->onSelectItem->connect(sigc::mem_fun(this,&MMSFileDialog::onSelectItem));
		}
		else
			this->filedialog_filelist = NULL;
	if (this->filedialog_up)
		if (this->filedialog_up->getType() == MMSWIDGETTYPE_BUTTON)
			this->filedialog_up->onReturn->connect(sigc::mem_fun(this,&MMSFileDialog::onReturn));
		else
			this->filedialog_up = NULL;
	if (this->filedialog_down)
		if (this->filedialog_down->getType() == MMSWIDGETTYPE_BUTTON)
			this->filedialog_down->onReturn->connect(sigc::mem_fun(this,&MMSFileDialog::onReturn));
		else
			this->filedialog_down = NULL;

	return true;
}

bool MMSFileDialog::isInitialized() {
	return (this->dialogwindow);
}

bool MMSFileDialog::setTitle(string title) {
	if (filedialog_title) {
		filedialog_title->setText(title);
		return true;
	}
	return false;
}

bool MMSFileDialog::show() {
	if (!isInitialized()) return false;

	// re-init the dialog
	if (this->filedialog_name)
		this->filedialog_name->setText(this->filename);
	fillMenu();
	
	// show the dialog
	this->dialogwindow->setFocus();
	this->dialogwindow->show();
	
	return true;
}

void MMSFileDialog::onReturn(MMSWidget *widget) {
	if (widget == this->filedialog_ok) {
		if (this->filename!="") {
			// hide the window
			dialogwindow->hide();

			// call callback
			if (this->onOK)
				this->onOK->emit(this);
		}
	}
	else
	if (widget == this->filedialog_cancel) {
		// hide the window
		dialogwindow->hide();

		// call callback
		if (this->onCancel)
			this->onCancel->emit();
	}
	else
	if (widget == this->filedialog_filelist) {
		unsigned int sel = this->filedialog_filelist->getSelected();
		if ((sel==0)&&(this->path != "/")) {
			// back item
			int pos = (int)path.rfind("/");
			if (pos >= 0) {
				// go to the parent dir
				this->path = this->path.substr(0, pos);
				if (this->path=="") this->path="/";
				fillMenu();
			}
		}
		else {
			MMSWidget *item = this->filedialog_filelist->getItem(sel);
			if (item) {
				string data;
				if (item->getData(data)) {
		            if ((int)data.find("D.") == 0) {
		            	// go to the subdir
		            	this->path = data.substr(2);
		            	fillMenu();
		            }
		            else
		            if ((int)data.find("F.") == 0) {
		            	printf("file = %s\n", data.c_str());
		            }
				}
			}
		}
	}
	else
	if (widget == this->filedialog_up) {
		if (this->filedialog_filelist)
			this->filedialog_filelist->scrollUp();
	}
	else
	if (widget == this->filedialog_down) {
		if (this->filedialog_filelist)
			this->filedialog_filelist->scrollDown();
	}
}

void MMSFileDialog::onSelectItem(MMSWidget *widget) {
	if (!this->filedialog_name)
		return;

	// set the name of the file
	string data;
	if (widget->getData(data))
        if ((int)data.find("F.") == 0) {
        	int pos = (int)data.rfind("/");
        	if (pos > 0) {
        		this->filename = data.substr(pos+1);
        		this->filedialog_name->setText(this->filename);
        	}
        }
}

bool MMSFileDialog::fillMenu() {
	// check and clear
	if (!this->filedialog_filelist) return false;
	this->filedialog_filelist->clear();

	// update path
	if (this->filedialog_path)
		this->filedialog_path->setText(path);
	
	// create back item
	if (this->path != "/") {
		MMSWidget *item = filedialog_filelist->newItem();
		if (item) {
			MMSLabelWidget *label = (MMSLabelWidget*)item->searchForWidget(PATH_OR_FILE);
			if ((label)&&(label->getType() == MMSWIDGETTYPE_LABEL)) {
				label->setText("..");
			}
		}
	}

	// searching for files and directories
	MMSFileSearch *fs = new MMSFileSearch(this->path, "*", false, false, true);
	if (!fs) return false;
	MMSFILEENTRY_LIST list = fs->execute();

	// create menu items - directories
	for(MMSFILEENTRY_LIST::iterator it = list.begin();it!=list.end();it++) {
		if ((*it)->isdir) {
			MMSWidget *item = filedialog_filelist->newItem();
			if (item) {
				MMSLabelWidget *label = (MMSLabelWidget*)item->searchForWidget(PATH_OR_FILE);
				if ((label)&&(label->getType() == MMSWIDGETTYPE_LABEL)) {
					label->setText("[" + (*it)->basename + "]");
					item->setData("D." + (*it)->name);
				}
			}
		}
	}

	// create menu items - files
	for(MMSFILEENTRY_LIST::iterator it = list.begin();it!=list.end();it++) {
		if (!(*it)->isdir) {
			MMSWidget *item = filedialog_filelist->newItem();
			if (item) {
				MMSLabelWidget *label = (MMSLabelWidget*)item->searchForWidget(PATH_OR_FILE);
				if ((label)&&(label->getType() == MMSWIDGETTYPE_LABEL)) {
					label->setText((*it)->basename);
					item->setData("F." + (*it)->name);
				}
			}
		}
	}

	// selection to the first item
	this->filedialog_filelist->setSelected(0);
	
	return true;
}


