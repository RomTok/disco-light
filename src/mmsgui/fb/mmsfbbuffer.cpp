/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2011 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mmsgui/fb/mmsfbbuffer.h"

MMSFBBuffer::BUFFER_INDEX MMSFBBuffer::index;

MMSFBBuffer::MMSFBBuffer(string ext_id) {
	this->ext_id = ext_id;

	BUFFER_INDEX::iterator it = this->index.find(this->ext_id);
	if (it == this->index.end()) {
		// new buffer
		this->buffer = new BUFFER;
		this->index.insert(make_pair(this->ext_id, this->buffer));
	}
	else {
		// use existing buffer
		this->buffer = it->second;
		this->buffer->use_count++;
	}
}

MMSFBBuffer::~MMSFBBuffer() {
	BUFFER_INDEX::iterator it = this->index.find(this->ext_id);
	if (it != this->index.end()) {
		// reduce use counter
		it->second->use_count--;
		if (it->second->use_count == 0) {
			// buffer is unused, free all resources
			delete it->second;
			this->index.erase(it);
		}
	}
}

bool MMSFBBuffer::isInitialized() {
	if (!this->buffer) return false;
	return this->buffer->initialized;
}

bool MMSFBBuffer::initBuffer(INDEX_BUFFER index_buffer, VERTEX_BUFFER vertex_buffer) {
	if (isInitialized()) return false;
	if (this->buffer) {
		this->buffer->type = BUFFER_TYPE_INDEX_VERTEX;
		this->buffer->index_buffer = index_buffer;
		this->buffer->vertex_buffer = vertex_buffer;
		this->buffer->initialized = true;
		return true;
	}
	return false;
}

bool MMSFBBuffer::getBuffer(MMSFBBuffer::BUFFER **buffer) {
	if (!isInitialized()) return false;
	if (buffer) *buffer = this->buffer;
	return true;
}

