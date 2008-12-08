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

#include "mmscore/mmsswitcher.h"
#include "mmscore/mmsswitcherthread.h"

MMSSwitcherThread::MMSSwitcherThread(MMSSwitcher *sw, MMSLabel *date_s, MMSLabel *time_s, MMSLabel *date_p, MMSLabel *time_p) : MMSThread("MMSSwitcherThread") {
    /* init */
    /* working as main thread */
    this->mode = 0;
    this->sw = sw;
    this->curr_date = "";
    this->curr_time = "";
    this->date_s = date_s;
    this->time_s = time_s;
    this->date_p = date_p;
    this->time_p = time_p;
    this->sleepcnt = 10;
    this->invoke_plugin = -1;
    this->plugin_invoked = 0;
    this->my_spt = NULL;
    this->preview_shown = false;
}

MMSSwitcherThread::MMSSwitcherThread(MMSSwitcher *sw) {
    /* init */
    /* working for show preview method of a specific plugin */
    this->mode = 1;
    this->sw = sw;
}

void MMSSwitcherThread::invokeShowPreview() {

    /* reset sleep cnt */
    this->sleepcnt = 10;

    /* lock me */
    this->lock.lock();

    /* save the plugin index */
    this->invoke_plugin = this->sw->curr_plugin;
    this->plugin_invoked = 0;
    this->my_spt = NULL;

    /* unlock me */
    this->lock.unlock();
}

void MMSSwitcherThread::previewShown() {
    this->preview_shown = true;
}

void MMSSwitcherThread::threadMain() {

    if (this->mode == 0) {
        unsigned int cnt = 0;

        while (1) {
            /* lock me */
            this->lock.lock();

            if (cnt % 10 == 0) {
                /* check and update date & time */
            	if ((date_s)||(date_p)||(time_s)||(time_p)) {
		            string datestr, timestr;
		            getCurrentTimeString(NULL, &datestr, &timestr);

		            if (datestr != this->curr_date) {
		                this->curr_date = datestr;
		                if (date_s) date_s->setText(this->curr_date);
		                if (date_p) date_p->setText(this->curr_date);
		            }

		            if (timestr.substr(0, 5) != this->curr_time) {
		                this->curr_time = timestr.substr(0, 5);
		                if (time_s) time_s->setText(this->curr_time);
		                if (time_p) time_p->setText(this->curr_time);
		            }
            	}
            }

            if (this->plugin_invoked) {
                if (cnt - this->plugin_invoked >= 3) {
/*                    if (this->sw->curr_previewWin < 0)
                        this->sw->waitForPreview->show();*/
                    this->plugin_invoked = 0;
                }
                else
                    if (this->preview_shown)
                        this->plugin_invoked = 0;
            }

            if (this->my_spt)
                if (!this->my_spt->isRunning()) {
                    /* if the last invoked show preview thread is not running anymore, check of waitForPreview */
/*                	if (!this->sw->waitForPreview->willHide())
	                	if (this->sw->waitForPreview->isShown()) {
	                        // the wait for preview window is not removed by the plugin thread
	                        // therefore i have to display nopreview window
	                        this->sw->noPreview->show();
	                    }*/
                    this->my_spt = NULL;
                }

            if (this->invoke_plugin >= 0) {
                /* start the showPreviewThread only if it does not running */
            	map<int, plugin_data_t *>::iterator i = this->sw->plugins.find(this->invoke_plugin);
                this->invoke_plugin = -1;
            	if (i != this->sw->plugins.end()) {
            		this->my_spt = i->second->switcher->showPreviewThread;
	                this->plugin_invoked = cnt;
	                this->preview_shown = false;

	                if (!this->my_spt->isRunning())
	                    this->my_spt->start();
            	}
            }

            /* unlock me */
            this->lock.unlock();

            /* sleep a little bit */
            while (this->sleepcnt > 0) {
                this->sleepcnt--;
                msleep(50);
            }
            this->sleepcnt = 10;
            cnt++;
        }
    }
    else
    if (this->mode == 1) {
        /* check if i have to call showpreview */
        if (this->sw->osdhandler)
            this->sw->osdhandler->invokeShowPreview(NULL);
        else
        if (this->sw->centralhandler)
            this->sw->centralhandler->invokeShowPreview(NULL);
    }
}

