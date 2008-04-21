/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
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


TAFF_ATTRDESC MMSGUI_MMSTHEME_ATTR_I[]			= MMSGUI_MMSTHEME_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_MMSDIALOG_ATTR_I[]			= MMSGUI_MMSDIALOG_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_DESCRIPTION_ATTR_I[]		= MMSGUI_DESCRIPTION_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_MAINWINDOW_ATTR_I[]		= MMSGUI_MAINWINDOW_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_CLASS_MAINWINDOW_ATTR_I[]	= MMSGUI_CLASS_MAINWINDOW_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_POPUPWINDOW_ATTR_I[] 		= MMSGUI_POPUPWINDOW_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_CLASS_POPUPWINDOW_ATTR_I[] = MMSGUI_CLASS_POPUPWINDOW_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_ARROW_ATTR_I[]				= MMSGUI_ARROW_ATTR_INIT;
TAFF_ATTRDESC MMSGUI_IMAGE_ATTR_I[]				= MMSGUI_IMAGE_ATTR_INIT;

TAFF_TAGTABLE mmsgui_taff_tagtable[] = {
	{	"mmstheme",		NULL, 	NULL,			MMSGUI_MMSTHEME_ATTR_I				},
	{	"mmsdialog",	NULL, 	NULL,			MMSGUI_MMSDIALOG_ATTR_I				},
	{	"description",	NULL, 	NULL,			MMSGUI_DESCRIPTION_ATTR_I			},
	{	"mainwindow",	NULL, 	NULL,			MMSGUI_MAINWINDOW_ATTR_I			},
	{	"class", 		"type",	"mainwindow",	MMSGUI_CLASS_MAINWINDOW_ATTR_I		},
	{	"popupwindow",	NULL, 	NULL,			MMSGUI_POPUPWINDOW_ATTR_I			},
	{	"class", 		"type",	"popupwindow", 	MMSGUI_CLASS_POPUPWINDOW_ATTR_I		},
	{	"arrow", 		NULL, 	NULL,			MMSGUI_ARROW_ATTR_I					},
	{	"image", 		NULL, 	NULL,			MMSGUI_IMAGE_ATTR_I					},
	{	NULL, 			NULL, 	NULL,			NULL								}
};

TAFF_DESCRIPTION mmsgui_taff_description = { "mmsgui", 1, mmsgui_taff_tagtable };

