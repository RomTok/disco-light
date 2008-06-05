/***************************************************************************
 *   Copyright (C) 2008 by Berlinux Solutions                              *
 *                                                                         *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>                *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#include "mmscore/mmsswitcher.h"

MMSDialogManager          MMSSwitcher::dm;            /**< dialog manager for whole switcher window                      */
MMSMainWindow*            MMSSwitcher::window;        /**< whole switcher window                                         */
map<int, plugin_data_t *> MMSSwitcher::plugins;       /**< loaded plugins                                                */
int                       MMSSwitcher::curr_plugin;   /**< index to pluginSwitchers which points to the current plugin   */

/* public */
MMSSwitcher::MMSSwitcher(MMSPluginData *plugindata) :
    windowmanager(NULL),
    pluginmanager(NULL),
    inputmanager(NULL) {

    /* switcher instantiated by plugin */
    if(plugindata) {
        this->plugindata = plugindata;
        plugin_data_t *pd = new plugin_data_t;
        pd->plugindata = *plugindata;
        pd->previewWin = NULL;
        plugins.insert(std::make_pair(plugindata->getId(), pd));
        return;
    }

    /* switcher start */
	DEBUGMSG("Switcher", "startup");

    this->curr_plugin = -1;
    this->window      = NULL;

    try {
        /* get the active osd and central plugins */
        DataSource source = DataSource(config.getConfigDBDBMS(), 
                                       config.getConfigDBDatabase(),
                                       config.getConfigDBAddress(),
                                       config.getConfigDBPort(),
                                       config.getConfigDBUser(),
                                       config.getConfigDBPassword());

        /* load switcher dialog */
        this->window = (MMSMainWindow*)dm.loadDialog(config.getData() + "/themes/" + config.getTheme() + "/switcher.xml");
        if(!this->window) throw MMSError(0, "Error loading switchers root window");
  
        /* get access to the menu bar */
        this->menuBar = (MMSChildWindow *)this->window->searchForWindow("menuBar");
        if(!this->menuBar) throw MMSError(0, "Error loading switchers menuBar childwindow");
        this->menu    = (MMSMenu *)dm["menu"];
        if(!this->menu) throw MMSError(0, "Error loading switchers menu");

        /* fill the menu */
        MMSPluginService service(&source);
        vector<MMSPluginData *> p = service.getOSDPlugins();
        for(unsigned int i = 0; i < p.size();i++) {
            MMSButton *pluginItem = dynamic_cast<MMSButton*>(menu->newItem());
	    MMSLabel *pluginLabel = dynamic_cast<MMSLabel*>((*pluginItem)["pluginname"]);
            pluginLabel->setText(p.at(i)->getName());
            pluginItem->setBinData((void*)p.at(i));
        }
        p = service.getCentralPlugins();
        for(unsigned int i = 0; i < p.size();i++) {
            MMSButton *pluginItem = dynamic_cast<MMSButton*>(menu->newItem());
	    MMSLabel *pluginLabel = dynamic_cast<MMSLabel*>((*pluginItem)["pluginname"]);	
            pluginLabel->setText(p.at(i)->getName());
            pluginItem->setBinData((void*)p.at(i));
        }

        /* show the menu bar */
        this->menu->setFocus(true);
        this->menuBar->show();

        /* connect onSelectItem callback of the menu widget */
        menu->onSelectItem->connect(sigc::mem_fun(this,&MMSSwitcher::onSelectItem));

        /* connect onReturn callback of the menu widget */
        menu->onReturn->connect(sigc::mem_fun(this,&MMSSwitcher::onReturn));
        
    	/* create inputs */
        subscribeKey(DIKS_MENU);
        subscribeKey(DIKS_BACKSPACE);
    } catch(MMSError *error) {
        DEBUGMSG("Switcher", "Abort due to: " + error->getMessage());
        string msg = error->getMessage();
        delete error;
        throw MMSError(0, msg);
    }
}

MMSSwitcher::~MMSSwitcher() {
	DEBUGMSG("Switcher", "deletion");
    this->plugins.clear();
	this->subscriptions.clear();
}

void MMSSwitcher::setWindowManager(IMMSWindowManager *wm) {
	this->windowmanager = wm;
}

void MMSSwitcher::setPluginManager(MMSPluginManager *pm){
	this->pluginmanager = pm;
}

void MMSSwitcher::setInputManager(MMSInputManager  *im) {
	this->inputmanager = im;
}

void MMSSwitcher::onSubscription(MMSInputSubscription *subscription){
    DFBInputDeviceKeySymbol key;

    subscription->getKey(key);

    if (key == DIKS_BACKSPACE) {
        /* hide all shown popups */
        /* hide all main windows only if no popups to be hide */
        if(this->windowmanager->hideAllPopupWindows()==false) 
            if(this->windowmanager->hideAllMainWindows(true)==false) {
                /* hide all windows different from default root */
                MMSWindow *w = this->windowmanager->getBackgroundWindow();
                if(w && !(w->isShown()))
                    this->windowmanager->hideAllRootWindows(); 
            }
    }
    else {
    	if(this->window->isShown()) {
            /* hide all main and popup windows */
            this->windowmanager->hideAllPopupWindows(); 
            this->windowmanager->hideAllMainWindows(); 
        }
        else {
			DEBUGMSG("Switcher", "try to show");
            /* show the switcher */
    		show();
        }
    }
}

void MMSSwitcher::subscribeKey(DFBInputDeviceKeySymbol key){
	MMSInputSubscription *subscription = new MMSInputSubscription(key);

	this->subscriptions.push_back(subscription);
	subscription->callback.connect(sigc::mem_fun(this, &MMSSwitcher::onSubscription));
	subscription->registerMe();
}


void MMSSwitcher::onSelectItem(MMSWidget *widget) {
    MMSButton *pluginLabel = dynamic_cast<MMSButton*>(widget);
    if(!pluginLabel)
        pluginLabel = dynamic_cast<MMSButton*>(this->menu->getSelectedItem());

    /* no menu item given */
    if(!pluginLabel) {
        return;
	}
	

    MMSPluginData *data = (MMSPluginData*)pluginLabel->getBinData();

    /* return if current plugin is selected plugin */
    if(this->curr_plugin == data->getId()) {
        return;
    }

    /* switch preview windows */
    map<int, plugin_data_t *>::iterator i = this->plugins.find(this->curr_plugin);
    if(i != this->plugins.end()) {
        MMSChildWindow *preview = i->second->previewWin;
        if(preview && preview->isShown())
            preview->hide(false,true);
    } 

    this->curr_plugin = data->getId();

    i = this->plugins.find(this->curr_plugin);
    if(i != this->plugins.end()) {
        MMSChildWindow *preview = i->second->previewWin;
        if(preview && !preview->isShown())
            preview->show();
    }
}

void MMSSwitcher::onReturn(MMSWidget *widget) {
    try {
        MMSLabel *pluginLabel = dynamic_cast<MMSLabel*>(widget);
        if(!pluginLabel)
            pluginLabel = dynamic_cast<MMSLabel*>(this->menu->getSelectedItem());

        /* no menu item given */
        if(!pluginLabel)
            return;

        MMSPluginData *data = (MMSPluginData*)pluginLabel->getBinData();

        if(data->getType()->getName() == "OSD_PLUGIN") {
            MMSOSDPluginHandler *handler = this->pluginmanager->getOSDPluginHandler(data->getId());
            handler->invokeShow(NULL);
        }
        else if(data->getType()->getName() == "CENTRAL_PLUGIN") {
            MMSCentralPluginHandler *handler = this->pluginmanager->getCentralPluginHandler(data->getId());
            handler->invokeShow(NULL);
        }
    } catch(MMSError *error) {
        DEBUGMSG("Switcher", "Abort due to: " + error->getMessage());
    }
}

void MMSSwitcher::show() {
    if(!this->plugins.empty())
        onSelectItem(NULL);
    this->window->show();
}

void MMSSwitcher::hide() {
    this->window->hide();
}

MMSChildWindow* MMSSwitcher::loadPreviewDialog(string filename, MMSTheme *theme, int id) {
    MMSChildWindow *win;

     try {
         win = this->dm.loadChildDialog(filename, theme);
     }
     catch(MMSError *error) {
         string msg = error->getMessage();
         delete error;
         throw MMSError(0, msg);
    }

    if(win) {
        map<int, plugin_data_t *>::iterator i = this->plugins.find(id);
        if(i != this->plugins.end()) {
            i->second->previewWin = win;
        }
    }

    return win;
}

MMSChildWindow* MMSSwitcher::loadInfoBarDialog(string filename, MMSTheme *theme) {
    return NULL;
}

void MMSSwitcher::setVolume(unsigned int volume, bool init) {
}

IMMSSwitcher *MMSSwitcher::newSwitcher(MMSPluginData *plugindata) {
	return new MMSSwitcher(plugindata);
}

bool MMSSwitcher::switchToPluginEx(int toplugin) {
    if (toplugin >= 0) {
    	try {
            MMSPluginData *data = &this->plugins[toplugin]->plugindata;

            if(data->getType()->getName() == "OSD_PLUGIN") {
                MMSOSDPluginHandler *handler = this->pluginmanager->getOSDPluginHandler(data->getId());
                handler->invokeShow(NULL);
            }
            else if(data->getType()->getName() == "CENTRAL_PLUGIN") {
                MMSCentralPluginHandler *handler = this->pluginmanager->getCentralPluginHandler(data->getId());
                handler->invokeShow(NULL);
            }

            return true;
	    } catch(MMSError *error) {
	      	DEBUGMSG("Switcher", "Abort due to: " + error->getMessage());
	    }
    }
    
    return false;
}

bool MMSSwitcher::switchToPlugin() {
    return switchToPluginEx(this->plugindata->getId());
}

bool MMSSwitcher::leavePlugin(bool show_switcher) {
	bool b = false;
	
	if (!this->window->isShown())
		b = this->windowmanager->hideAllMainWindows(true);
	
	if (b==false) {
	    /* hide all windows different from default root */
	    MMSWindow *w = this->windowmanager->getBackgroundWindow();
	    if(w && !w->isShown())
	        this->windowmanager->hideAllRootWindows(); 
	}
	
	if (!this->window->isShown())
		if (show_switcher)
			show();
	
	return true;
}
