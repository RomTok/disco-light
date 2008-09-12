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

#include "mmsgui/mmslabelwidgetthread.h"

MMSLabelWidgetThread::MMSLabelWidgetThread(MMSLabelWidget *label) {
    this->label = label;
    this->inWait = false;
    this->stopThread = false;
    this->pauseThread = false;
}

void MMSLabelWidgetThread::wait(unsigned int delaytime) {
    this->inWait = true;
    usleep(delaytime);
    while (this->pauseThread)
        usleep(1000);
    this->inWait = false;
}

void MMSLabelWidgetThread::stop() {
    while (!this->inWait)
        usleep(1000);
    this->stopThread = true;
}

void MMSLabelWidgetThread::pause(bool pauseThread) {
    if (pauseThread) {
        while (!this->inWait)
            usleep(1000);
        this->pauseThread = true;
    }
    else {
        this->pauseThread = false;
    }
}

void MMSLabelWidgetThread::doIt() {

    /* init */
    this->inWait = false;
    this->stopThread = false;
    this->pauseThread = false;

    while (1) {
        /* first check if the window is shown */
        MMSWindow *win = this->label->getRootWindow();
        if (!win) {
            wait(1000000);
            if (this->stopThread)
                return;
            else
                continue;
        }
        if (!win->isShown()) {
            wait(1000000);
            if (this->stopThread)
                return;
            else
                continue;
        }

        /* get the next sleep time */
        unsigned int delaytime = 500;
        bool changed = false;

        if (label->getSlidable()) {
            if (label->slid_width > 0) {
            	DFBRectangle surfaceGeom = label->getSurfaceGeometry();
                if (label->slid_width > surfaceGeom.w) {
                	// we should slide the label text
                	if (label->slid_offset >= label->slid_width)
                		// from the beginning 
                		label->slid_offset = -surfaceGeom.w;
                	else
                		// increase offset
                		label->slid_offset+=8;
                	changed = true;
                }
            }
        }
        
        /* refresh the widget */
        if (changed)
            this->label->refresh();

        /* sleep */
        if (delaytime > 0)
            wait(delaytime*1000);
        else
            wait(1000000);
        if (this->stopThread)
            return;
    }
}

void MMSLabelWidgetThread::threadMain() {
    doIt();
    delete this;
}

