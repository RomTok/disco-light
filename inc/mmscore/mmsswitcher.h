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

#ifndef MMSSWITCHER_H_
#define MMSSWITCHER_H_

#include <mmsbase/interfaces/immsswitcher.h>
#include <mmscore/mmspluginmanager.h>
#include <mmscore/mmsswitcherthread.h>
#include <mmsinput/mmsinput.h>
#include <mmsgui/mmsgui.h>

typedef struct {
    MMSPluginData 				plugindata;
    vector<MMSChildWindow *> 	previewWins;
    MMSSwitcher					*switcher;
} plugin_data_t;


class MMSSwitcher : public IMMSSwitcher {
	private:
        MMSConfigData                        config;

        MMSPluginData                        *plugindata;        /**< for plugin owned switcher instances                           */

        static IMMSWindowManager             *windowmanager;
        static MMSPluginManager              *pluginmanager;
        static MMSInputManager               *inputmanager;
        static vector<MMSInputSubscription*> subscriptions;

        static MMSDialogManager              dm;                 /**< dialog manager for whole switcher window                      */
        static MMSMainWindow                 *window;            /**< whole switcher window                                         */

        MMSChildWindow                       *menuBar;           /**< shows the plugin menu                                         */
        MMSMenuWidget                        *menu;              /**< plugin menu                                                   */
        MMSChildWindow                       *menuBar_static;    /**< shows the static plugin menu                                  */
        MMSMenuWidget                        *menu_static;       /**< plugin static menu, switcher does not control it              */

        static map<int, plugin_data_t *>     plugins;            /**< loaded plugins                                                */
        static int                           curr_plugin;        /**< index to pluginSwitchers which points to the current plugin   */
        vector<MMSPluginData *> 			 osdplugs;
        vector<MMSPluginData *> 			 centralplugs;
        

        MMSOSDPluginHandler                  *osdhandler;
        MMSCentralPluginHandler              *centralhandler;
        MMSSwitcherThread                    *showPreviewThread; /**< a separate thread for each plugin                             */
        static MMSSwitcherThread             *switcherThread;    /**< my update thread                                              */
        
        void setMenuItemValues(MMSWidget *item);
		int  searchingForImage(string pluginpath, string imagename, string *path);
        void onSelectItem(MMSWidget *widget);
        void onReturn(MMSWidget *widget);
        bool onBeforeShowPreview(MMSWindow *win);
        bool switchToPluginEx(int toplugin);

	public:
        /* initialization */
		MMSSwitcher(MMSPluginData *plugindata = NULL);
		~MMSSwitcher();
		void setWindowManager(IMMSWindowManager *wm);
		void setPluginManager(MMSPluginManager *pm);
		void setInputManager(MMSInputManager  *im);
		void onSubscription(MMSInputSubscription *subscription);
		void subscribeKey(DFBInputDeviceKeySymbol key);

        /* methods for IMMSSwitcher */
        virtual void show();
        virtual void hide();
        virtual MMSChildWindow* loadPreviewDialog(string filename, MMSTheme *theme = NULL, int id=-1);
        virtual MMSChildWindow* loadInfoBarDialog(string filename, MMSTheme *theme = NULL);
        virtual void setVolume(unsigned int volume, bool init = false);
        virtual IMMSSwitcher *newSwitcher(MMSPluginData *plugindata);
        virtual bool switchToPlugin();
        virtual bool leavePlugin(bool show_switcher);
        
    friend class MMSSwitcherThread;
};

#endif /*MMSSWITCHER_H_*/
