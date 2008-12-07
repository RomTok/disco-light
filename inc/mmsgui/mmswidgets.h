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
