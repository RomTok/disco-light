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

#ifndef MMSWIDGETS_H_
#define MMSWIDGETS_H_

#include "mmsgui/mmshboxwidget.h"
#include "mmsgui/mmsvboxwidget.h"
#include "mmsgui/mmsbuttonwidget.h"
#include "mmsgui/mmslabelwidget.h"
#include "mmsgui/mmsimagewidget.h"
#include "mmsgui/mmsarrowwidget.h"
#include "mmsgui/mmsprogressbarwidget.h"
#include "mmsgui/mmsmenuwidget.h"
#include "mmsgui/mmstextboxwidget.h"
#include "mmsgui/mmssliderwidget.h"
#include "mmsgui/mmsinputwidget.h"
#include "mmsgui/mmscheckboxwidget.h"
#include "mmsgui/mmsgapwidget.h"

// for compatibility reasons map old widget class names to the new names
#define MMSHBox 		MMSHBoxWidget
#define MMSVBox 		MMSVBoxWidget
#define MMSButton 		MMSButtonWidget
#define MMSLabel 		MMSLabelWidget
#define MMSImage 		MMSImageWidget
#define MMSArrow 		MMSArrowWidget
#define MMSProgressBar	MMSProgressBarWidget
#define MMSMenu 		MMSMenuWidget
#define MMSTextBox 		MMSTextBoxWidget
#define MMSSlider 		MMSSliderWidget

#endif /*MMSWIDGETS_H_*/
