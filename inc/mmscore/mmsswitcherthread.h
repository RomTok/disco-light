/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
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
