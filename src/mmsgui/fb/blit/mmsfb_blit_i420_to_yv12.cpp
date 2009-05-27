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

void mmsfb_blit_i420_to_yv12(MMSFBSurfacePlanes *src_planes, int src_height, int sx, int sy, int sw, int sh,
							 MMSFBSurfacePlanes *dst_planes, int dst_height, int dx, int dy) {
	// first time?
	static bool firsttime = true;
	if (firsttime) {
		printf("DISKO: Using accelerated copy I420 to YV12.\n");
		firsttime = false;
	}

	// prepare source planes
	MMSFBSurfacePlanes source_planes;
	if ((src_planes->ptr2)&&(src_planes->ptr3)) {
		source_planes = *src_planes;
	}
	else {
		source_planes.ptr = src_planes->ptr;
		source_planes.pitch = src_planes->pitch;
		source_planes.ptr2 = (unsigned char *)source_planes.ptr + source_planes.pitch * src_height;
		source_planes.pitch2 = src_planes->pitch;
		source_planes.ptr3 = (unsigned char *)source_planes.ptr + source_planes.pitch * src_height + (source_planes.pitch >> 1) * (src_height >> 1);
		source_planes.pitch3 = src_planes->pitch;
	}

	// now we can use the YV12 to YV12 blit
	mmsfb_blit_yv12_to_yv12(&source_planes, src_height, sx, sy, sw, sh,
							dst_planes, dst_height, dx, dy);
}

