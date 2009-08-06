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

#ifndef __MMSGST_H__
#define __MMSGST_H__

#ifdef __HAVE_GSTREAMER__

#include <stdlib.h>
#include <gst/gst.h>
#include <unistd.h>

#include "mmsgui/mmsgui.h"

//! freeing any allocated gstreamer memory
void mmsGstFree();

//! create a pipeline
GstElement *mmsGstLaunch(const char *pipeline_description);

//! init a gstreamer pipeline with an uri and connect it to an surface
GstElement *mmsGstInit(const string uri, MMSFBSurface *surface);

//! init a gstreamer pipeline with an uri and connect it to an window
GstElement *mmsGstInit(const string uri, MMSWindow *window);

//! start playback of a pipeline
bool mmsGstPlay(GstElement *pipelineX);

//! send key press event to a pipeline
bool mmsGstSendKeyPress(GstElement *pipeline, MMSKeySymbol key);

//! send key release event to a pipeline
bool mmsGstSendKeyRelease(GstElement *pipeline, MMSKeySymbol key);

//! send button press event to a pipeline
bool mmsGstSendButtonPress(GstElement *pipeline, int posx, int posy);

//! send button release event to a pipeline
bool mmsGstSendButtonRelease(GstElement *pipeline, int posx, int posy);

//! send axis motion event to a pipeline
bool mmsGstSendAxisMotion(GstElement *pipeline, int posx, int posy);

#endif

#endif /* __MMSGST_H__ */
