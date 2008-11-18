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

#include "mmsinput/mmsinputsubscription.h"


MMSInputManager *MMSInputSubscription::manager;


MMSInputSubscription::MMSInputSubscription(MMSInputManager *manager) {
	this->manager = manager;
}

MMSInputSubscription::MMSInputSubscription(MMSKeySymbol key) {
	this->key = key;
}

MMSInputSubscription::MMSInputSubscription(MMSFBRectangle &pointer_area) {
	this->key = MMSKEY_UNKNOWN;
	this->pointer_area = pointer_area;
}

bool MMSInputSubscription::getKey(MMSKeySymbol &key) {
	if (this->key != MMSKEY_UNKNOWN) {
		key = this->key;
		return true;
	}
	else
		return false;
}

bool MMSInputSubscription::getPointerArea(MMSFBRectangle &pointer_area) {
	if (this->key == MMSKEY_UNKNOWN) {
		pointer_area = this->pointer_area;
		return true;
	}
	else
		return false;
}

void MMSInputSubscription::registerMe() {
	this->manager->addSubscription(this);
}

