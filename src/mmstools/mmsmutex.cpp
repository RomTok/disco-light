/***************************************************************************
 *   Copyright (C) 2005-2008 by Berlinux Solutions                         *
 *                                                                         *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>                *
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
#include "mmsmutex.h"

MMSMutex::MMSMutex() {
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init( &mutex_attr );
    
    pthread_mutexattr_settype( &mutex_attr, PTHREAD_MUTEX_ERRORCHECK );

    pthread_mutex_init(&this->mutex, &mutex_attr);

}

MMSMutex::~MMSMutex() {
    pthread_mutex_destroy(&this->mutex);
}

int MMSMutex::lock() {
	return pthread_mutex_lock(&this->mutex);
}

int MMSMutex::unlock() {
	return pthread_mutex_unlock(&this->mutex);
}

int MMSMutex::trylock() {
	return pthread_mutex_trylock(&this->mutex);
}

