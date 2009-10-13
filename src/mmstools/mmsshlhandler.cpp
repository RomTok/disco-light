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

#include "mmstools/mmsshlhandler.h"
#include "mmstools/mmsfile.h"
#ifdef __HAVE_DL__
#include <dlfcn.h>
#endif

MMSShlHandler::~MMSShlHandler() {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#else

    if(this->isloaded) {
        dlclose(this->handle);
    }
#endif
}

void MMSShlHandler::close() {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#else

    if(this->isloaded) {
        dlclose(this->handle);
        this->isloaded = false;
    } else {
        throw new MMSShlError(0,"shared library " + this->name + " is not loaded");
    }
#endif
}

void MMSShlHandler::open() {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#else

    if(this->isloaded) {
        throw new MMSShlError(0,"shared library " + this->name + " is alread loaded");
    }
    else {
        this->handle = dlopen(this->name.c_str(),RTLD_NOW);

        if(this->handle == NULL)
            throw new MMSShlError(0,"shared library " + this->name + " cannot be loaded: " + dlerror());

        this->isloaded = true;
    }
#endif
}

void MMSShlHandler::setName(string name) {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#else

    this->name = name;
#endif
}

string MMSShlHandler::getName() {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#else

    return this->name;
#endif
}

void *MMSShlHandler::getFunction(string name) {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#else

    void *ret = NULL;

    if(this->isloaded) {
        ret = dlsym(this->handle,name.c_str());
        if(ret == NULL)
            throw new MMSShlError(0,"symbol " + name + " cannot be retrieved: " + dlerror());
    } else {
        throw new MMSShlError(0,"shared library " + this->name + " is not loaded");
    }

    return ret;
#endif
}

bool MMSShlHandler::isLoaded() {
#ifndef __HAVE_DL__
    throw MMSShlError(0, "Compile with use_dl (libdl support)");
#else
    return this->isloaded;
#endif

}
