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

#ifndef MMSFBCONV_H_
#define MMSFBCONV_H_

//! Describes up to 3 planes of an external surface buffer.
typedef struct {
	void	*ptr;
	int 	pitch;
	void	*ptr2;
	int 	pitch2;
	void	*ptr3;
	int 	pitch3;
} MMSFBExternalSurfaceBuffer;

//! Stretch blit with antialiasing
/*!
\author Jens Schneider
*/
void mmsfb_stretchblit_yv12_to_yv12(MMSFBExternalSurfaceBuffer *extbuf, int src_height, int sx, int sy, int sw, int sh,
									unsigned char *dst, int dst_pitch, int dst_height, int dx, int dy, int dw, int dh);


#endif /* MMSFBCONV_H_ */
