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

MMSFBBuffer::EXTKEY_INDEX MMSFBBuffer::extkey_index;
MMSFBBuffer::BUFFER_INDEX MMSFBBuffer::buffer_index;

MMSFBBuffer::MMSFBBuffer(unsigned int extkey, unsigned int subkey) {
	// allocate new or use existing extkey
	EXTKEY_INDEX::iterator extkey_it = this->extkey_index.find(extkey);
	if (extkey_it == this->extkey_index.end()) {
		// new extkey
		this->extkey = new EXTKEY(extkey);
		this->extkey_index.insert(make_pair(extkey, this->extkey));
	}
	else {
		// use existing extkey
		this->extkey = extkey_it->second;
		this->extkey->use_count++;
	}

	// build external id
	this->ext_id = (((unsigned long long)extkey) << 32) + (unsigned long long)subkey;

	// allocate new or use existing buffer
	BUFFER_INDEX::iterator buffer_it = this->buffer_index.find(this->ext_id);
	if (buffer_it == this->buffer_index.end()) {
		// new buffer
		this->buffer = new BUFFER();
		this->buffer_index.insert(make_pair(this->ext_id, this->buffer));
	}
	else {
		// use existing buffer
		this->buffer = buffer_it->second;
		this->buffer->use_count++;
	}
}

MMSFBBuffer::~MMSFBBuffer() {
	// reduce use count of buffer
	BUFFER_INDEX::iterator buffer_it = this->buffer_index.find(this->ext_id);
	if (buffer_it != this->buffer_index.end()) {
		// reduce use counter
		buffer_it->second->use_count--;
		if (buffer_it->second->use_count == 0) {
			// buffer is unused, free all resources
			delete buffer_it->second;
			this->buffer_index.erase(buffer_it);
		}
	}

	// reduce use count of extkey
	EXTKEY_INDEX::iterator extkey_it = this->extkey_index.find(this->extkey->key);
	if (extkey_it != this->extkey_index.end()) {
		// reduce use counter
		extkey_it->second->use_count--;
		if (extkey_it->second->use_count == 0) {
			// extkey is unused, free all resources
			delete extkey_it->second;
			this->extkey_index.erase(extkey_it);
		}
	}
}

bool MMSFBBuffer::isInitialized() {
	if (!this->buffer) return false;
	return this->buffer->initialized;
}

bool MMSFBBuffer::getExtKey(MMSFBBuffer::EXTKEY **extkey) {
	if (!isInitialized()) return false;
	if (extkey) *extkey = this->extkey;
	return true;
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

