/***************************************************************************
 *   Copyright (C) 2005-2008 by Berlinux Solutions                         *
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

#ifndef MMSSWITCHERTHREAD_H_
#define MMSSWITCHERTHREAD_H_

#include "mms.h"

class MMSSwitcher;

class MMSSwitcherThread : public MMSThread {
    private:
        int     mode;   /**< mode of the thread, 0: main thread, 1: show preview thread */
        
        MMSMutex  lock;

        string curr_date;
        string curr_time;        

        MMSSwitcher *sw;
        MMSLabel    *date_s;
        MMSLabel    *time_s;
        MMSLabel    *date_p;
        MMSLabel    *time_p;

        unsigned int    sleepcnt;

        int                 invoke_plugin;
        unsigned int        plugin_invoked;
        MMSSwitcherThread   *my_spt;
        bool                preview_shown;

        void threadMain();

    public:
        MMSSwitcherThread(MMSSwitcher *sw, MMSLabelWidget *date_s, MMSLabelWidget *time_s, MMSLabelWidget *date_p, MMSLabelWidget *time_p);
        MMSSwitcherThread(MMSSwitcher *sw);

        void invokeShowPreview();
        void previewShown();
};


#endif /*MMSSWITCHERTHREAD_H_*/
