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

#include "mmsgui/theme/mmsthemebase.h"

MMSALIGNMENT getAlignmentFromString(string inputstr) {
    MMSALIGNMENT alignment;

    alignment = MMSALIGNMENT_NOTSET;

    if (inputstr == "center")
        alignment = MMSALIGNMENT_CENTER;
    else if (inputstr == "left")
        alignment = MMSALIGNMENT_LEFT;
    else if (inputstr == "right")
        alignment = MMSALIGNMENT_RIGHT;
    else if (inputstr == "justify")
        alignment = MMSALIGNMENT_JUSTIFY;
    else if (inputstr == "top-center")
        alignment = MMSALIGNMENT_TOP_CENTER;
    else if (inputstr == "top-left")
        alignment = MMSALIGNMENT_TOP_LEFT;
    else if (inputstr == "top-right")
        alignment = MMSALIGNMENT_TOP_RIGHT;
    else if (inputstr == "top-justify")
        alignment = MMSALIGNMENT_TOP_JUSTIFY;
    else if (inputstr == "bottom-center")
        alignment = MMSALIGNMENT_BOTTOM_CENTER;
    else if (inputstr == "bottom-left")
        alignment = MMSALIGNMENT_BOTTOM_LEFT;
    else if (inputstr == "bottom-right")
        alignment = MMSALIGNMENT_BOTTOM_RIGHT;
    else if (inputstr == "bottom-justify")
        alignment = MMSALIGNMENT_BOTTOM_JUSTIFY;

    return alignment;
}

MMSDIRECTION getDirectionFromString(string inputstr) {
    MMSDIRECTION direction;

    direction = MMSDIRECTION_NOTSET;

    if (inputstr == "left")
        direction = MMSDIRECTION_LEFT;
    else if (inputstr == "right")
        direction = MMSDIRECTION_RIGHT;
    else if (inputstr == "up")
        direction = MMSDIRECTION_UP;
    else if (inputstr == "down")
        direction = MMSDIRECTION_DOWN;

    return direction;
}



