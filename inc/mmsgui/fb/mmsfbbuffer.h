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

#ifndef MMSFBBUFFER_H_
#define MMSFBBUFFER_H_

#include "mmstools/base.h"
#include "mmstools/mmstypes.h"
#include "mmsgui/fb/mmsfbgl.h"
#include <stdlib.h>

class MMSFBBuffer {
	public:
		//! list of index arrays
		typedef struct {
			//! index data
			MMS3D_INDEX_ARRAY	*arrays;
			//! maximum number of arrays
			unsigned short int	max_arrays;
			//! arrays which are initialized
			unsigned short int	num_arrays;
		} INDEX_BUFFER;

    	//! list of vertex arrays
    	typedef struct {
			//! vertex data
			MMS3D_VERTEX_ARRAY	*arrays;
			//! maximum number of arrays
			unsigned short int	max_arrays;
			//! arrays which are initialized
			unsigned short int	num_arrays;
		} VERTEX_BUFFER;

    	//! buffer types
    	typedef enum {
    		//! buffer not initialized
    		BUFFER_TYPE_NOTSET = 0,
    		//! index and vertex buffer
    		BUFFER_TYPE_INDEX_VERTEX
    	} BUFFER_TYPE;

		//! contains a OpenGL index buffer object with additional description
		typedef struct {
			//! OpenGL's buffer object
			unsigned int		bo;
			//! index data description
			MMS3D_INDEX_BUFFER	*buffers;
			//! number of buffers
			unsigned short int	num_buffers;
		} INDEX_BUFFER_OBJECT;

		//! contains a OpenGL vertex buffer object with additional description
		typedef struct {
			//! OpenGL's buffer object
			unsigned int		bo;
			//! vertex data description
			MMS3D_VERTEX_BUFFER	*buffers;
			//! number of buffers
			unsigned short int	num_buffers;
		} VERTEX_BUFFER_OBJECT;

    	//! extkey description
    	class EXTKEY {
			public:
				//! is extkey initialized?
				bool initialized;

				//! use count
				unsigned int	use_count;

				//! own key
				unsigned int	key;

#ifdef __HAVE_OPENGL__
				//! OpenGL's buffer object which contains indices
				unsigned int	ibo;
				unsigned int	ibo_size;
				unsigned int	ibo_used;

				//! OpenGL's buffer object which contains vertices
				unsigned int	vbo;
				unsigned int	vbo_size;
				unsigned int	vbo_used;
#endif

				EXTKEY(unsigned int key) : initialized(false), use_count(1) {
					this->key = key;
#ifdef __HAVE_OPENGL__
					this->ibo = 0;
					this->ibo_size = 0;
					this->ibo_used = 0;
					this->vbo = 0;
					this->vbo_size = 0;
					this->vbo_used = 0;
#endif
				}
				~EXTKEY();
				bool reserveIndexArray(unsigned int requested_size, unsigned int *offset) {
					if (!this->ibo) return false;
					if (this->ibo_used + requested_size > this->ibo_size) return false;
					*offset = this->ibo_used;
					this->ibo_used+= requested_size;
					return true;
				}
				bool reserveVertexArray(unsigned int requested_size, unsigned int *offset) {
					if (!this->vbo) return false;
					if (this->vbo_used + requested_size > this->vbo_size) return false;
					*offset = this->vbo_used;
					this->vbo_used+= requested_size;
					return true;
				}
    	};

    	//! buffer description
    	class BUFFER {
			public:
				//! is buffer initialized?
				bool initialized;

				//! use count
				unsigned int	use_count;

				//! type of buffer
				BUFFER_TYPE		type;

				//! index data
				INDEX_BUFFER	index_buffer;

				//! vertex data
				VERTEX_BUFFER	vertex_buffer;

#ifdef __HAVE_OPENGL__
				//! OpenGL's buffer object which contains indices
				INDEX_BUFFER_OBJECT		index_bo;

				//! OpenGL's buffer object which contains vertices
				VERTEX_BUFFER_OBJECT	vertex_bo;
#endif

				BUFFER() : initialized(false), use_count(1), type(BUFFER_TYPE_NOTSET) {
#ifdef __HAVE_OPENGL__
					this->index_bo.bo = 0;
					this->index_bo.buffers = NULL;
					this->index_bo.num_buffers = 0;
					this->vertex_bo.bo = 0;
					this->vertex_bo.buffers = NULL;
					this->vertex_bo.num_buffers = 0;
#endif
				}
				~BUFFER() {
					switch (this->type) {
					case BUFFER_TYPE_INDEX_VERTEX:
						if (this->index_buffer.arrays) {
							for (unsigned int i = 0; i < this->index_buffer.num_arrays; i++)
								if (this->index_buffer.arrays[i].buf) free(this->index_buffer.arrays[i].buf);
							free(this->index_buffer.arrays);
						}
						if (this->vertex_buffer.arrays) {
							for (unsigned int i = 0; i < this->vertex_buffer.num_arrays; i++)
								if (this->vertex_buffer.arrays[i].buf) free(this->vertex_buffer.arrays[i].buf);
							free(this->vertex_buffer.arrays);
						}
						break;
					default:
						break;
					}

					if (this->index_bo.buffers)
						free(this->index_bo.buffers);
					if (this->vertex_bo.buffers)
						free(this->vertex_bo.buffers);
				}
				bool getBuffers(MMSFBBuffer::INDEX_BUFFER **index_buffer, MMSFBBuffer::VERTEX_BUFFER **vertex_buffer) {
					if (this->type != BUFFER_TYPE_INDEX_VERTEX) return false;
					if (index_buffer) *index_buffer = &this->index_buffer;
					if (vertex_buffer) *vertex_buffer = &this->vertex_buffer;
					return true;
				}
#ifdef __HAVE_OPENGL__
				bool getBufferObjects(MMSFBBuffer::INDEX_BUFFER_OBJECT **index_bo, MMSFBBuffer::VERTEX_BUFFER_OBJECT **vertex_bo) {
					if (this->type != BUFFER_TYPE_INDEX_VERTEX) return false;
					if (index_bo) *index_bo = &this->index_bo;
					if (vertex_bo) *vertex_bo = &this->vertex_bo;
					return true;
				}
#endif
    	};

	private:

		//! external ID of buffer (64bit, extkey + subkey)
		unsigned long long ext_id;

    	//! defines mapping between extkey of buffer and content
		typedef std::map<unsigned int, MMSFBBuffer::EXTKEY*> EXTKEY_INDEX;

		//! static key index
		static EXTKEY_INDEX extkey_index;

		//! pointer to extkey content
		EXTKEY *extkey;

		//! defines mapping between external ID of buffer and content
		typedef std::map<unsigned long long, MMSFBBuffer::BUFFER*> BUFFER_INDEX;

		//! static buffer index
		static BUFFER_INDEX buffer_index;

		//! pointer to buffer content
		BUFFER *buffer;

	public:
        MMSFBBuffer(unsigned int extkey, unsigned int subkey);
        ~MMSFBBuffer();
        bool isInitialized();
        bool getExtKey(MMSFBBuffer::EXTKEY **extkey);
        bool initBuffer(INDEX_BUFFER index_buffer, VERTEX_BUFFER vertex_buffer);
        bool getBuffer(MMSFBBuffer::BUFFER **buffer);
};

#endif /* MMSFBBUFFER_H_ */
