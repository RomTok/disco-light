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

#include "mmstools/mmsshlhandler.h"
#include "mmstools/mmsfile.h"
#include <dlfcn.h>

MMSShlHandler::~MMSShlHandler() {
    if(this->isloaded) {
        dlclose(this->handle);
    }    
}

void MMSShlHandler::close() {
    if(this->isloaded) {
        dlclose(this->handle);
        this->isloaded = false;
    } else {
        throw new MMSShlError(0,"shared library " + this->name + " is not loaded");
    }
}

void MMSShlHandler::open() {
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
    this->name = name;
}

string MMSShlHandler::getName() {
    return this->name;
}

void *MMSShlHandler::getFunction(string name) {
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
    return this->isloaded;
}
