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

#include "mmstools/mmsshlhandler.h"
#include "mmstools/mmsfile.h"
#ifdef __HAVE_DL__
#include <dlfcn.h>
#endif

MMSShlHandler::~MMSShlHandler() {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#endif

    if(this->isloaded) {
        dlclose(this->handle);
    }
}

void MMSShlHandler::close() {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#endif

    if(this->isloaded) {
        dlclose(this->handle);
        this->isloaded = false;
    } else {
        throw new MMSShlError(0,"shared library " + this->name + " is not loaded");
    }
}

void MMSShlHandler::open() {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#endif

    if(this->isloaded) {
        throw new MMSShlError(0,"shared library " + this->name + " is alread loaded");
    }
    else {
        this->handle = dlopen(this->name.c_str(),RTLD_NOW);

        if(this->handle == NULL)
            throw new MMSShlError(0,"shared library " + this->name + " cannot be loaded: " + dlerror());

        this->isloaded = true;
    }
}

void MMSShlHandler::setName(string name) {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#endif

    this->name = name;
}

string MMSShlHandler::getName() {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#endif

    return this->name;
}

void *MMSShlHandler::getFunction(string name) {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#endif

    void *ret = NULL;

    if(this->isloaded) {
        ret = dlsym(this->handle,name.c_str());
        if(ret == NULL)
            throw new MMSShlError(0,"symbol " + name + " cannot be retrieved: " + dlerror());
    } else {
        throw new MMSShlError(0,"shared library " + this->name + " is not loaded");
    }

    return ret;
}

bool MMSShlHandler::isLoaded() {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#endif

    return this->isloaded;
}
