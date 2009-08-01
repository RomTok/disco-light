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

#include "mmsgui/fb/mmsfbconv.h"
#include "mmstools/mmstools.h"
#include <string.h>

void mmsfb_fillrectangle_rgb24(MMSFBSurfacePlanes *dst_planes, int dst_height,
						       int dx, int dy, int dw, int dh, MMSFBColor color) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated fill rectangle to RGB24.\n");
		firsttime = false;
	}

	// get the first destination ptr/pitch
	unsigned char *dst = (unsigned char *)dst_planes->ptr;
	int dst_pitch = dst_planes->pitch;

	// prepare a rgb line
	int size = dw * 3;
	if (size > dst_pitch) size = dst_pitch;
	unsigned char line[2048*3];
	unsigned char *ls = line;
	unsigned char *le = line + size;
	unsigned char r = color.r;
	unsigned char g = color.g;
	unsigned char b = color.b;
	while (ls < le) {
		*ls = r; ls++;
		*ls = g; ls++;
		*ls = b; ls++;
	}
	dst+= dx*3 + dy * dst_pitch;
	unsigned char *dst_end = dst + dst_pitch * dh;

	// copy line buffer directly to the destination
	// for all lines
	while (dst < dst_end) {
		memcpy(dst, line, size);
		dst+= dst_pitch;
	}
}

