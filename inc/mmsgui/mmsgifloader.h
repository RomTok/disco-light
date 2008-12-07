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

#ifndef MMSGIFLOADER_H_
#define MMSGIFLOADER_H_

#include "mmsgui/mmsimagemanager.h"
#include <sigc++/sigc++.h>

class MMSGIFLoader : public MMSThread {
    private:
        //GIF: header
        typedef struct {
            char signature[3+1];
            char version[3+1];
        } MMS_GIF_HEADER;

        //GIF: logical screen descriptor
        typedef struct {
            unsigned short  width;
            unsigned short  height;
            unsigned char   flags;
            unsigned char   bgcolor;
            unsigned char   ratio;
            bool            global_color_table;
        } MMS_GIF_LSD;

        //GIF: color table
        typedef struct {
            unsigned short  size;
            unsigned char   table[3*256];
        } MMS_GIF_CT;

        //GIF: graphic control extension
        typedef struct {
            unsigned char   flags;
            unsigned short  delaytime;
            unsigned char   transcolor;
            bool            transparent_color;
            unsigned char   disposal;
        } MMS_GIF_GCE;

        //GIF: image descriptor
        typedef struct {
            unsigned short  x;
            unsigned short  y;
            unsigned short  w;
            unsigned short  h;
            unsigned char   flags;
            bool            local_color_table;
            bool            interlaced;
        } MMS_GIF_ID;

        //name of gif file
        MMSIM_DESC *desc;

        //display layer
        MMSFBLayer *layer;

        //file pointer
        MMSFile     *myfile;

        //gif data
        MMS_GIF_HEADER  gif_header;
        MMS_GIF_LSD     gif_lsd;
        MMS_GIF_CT      gif_gct;

        pthread_cond_t	cond;
        pthread_mutex_t	mutex;

        bool loadHeader();
        bool loadBlocks();

        void threadMain();

    public:
        MMSGIFLoader(MMSIM_DESC      *desc,
        		     MMSFBLayer      *layer);

        void block();
};

bool isGIF(string file);

#endif /*MMSGIFLOADER_H_*/
