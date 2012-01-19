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

    private:
    	//! buffer types
    	typedef enum {
    		//! buffer not initialized
    		BUFFER_TYPE_NOTSET = 0,
    		//! index and vertex buffer
    		BUFFER_TYPE_INDEX_VERTEX
    	} BUFFER_TYPE;

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
				GLuint	index_bo;

				//! OpenGL's buffer object which contains vertices
				GLuint	vertex_bo;
#endif

				BUFFER() : initialized(false), use_count(1), type(BUFFER_TYPE_NOTSET) {
#ifdef __HAVE_OPENGL__
					this->index_bo = 0;
					this->vertex_bo = 0;
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
				}
    	};

    	//! defines mapping between external ID of buffer and content
		typedef std::map<std::string, MMSFBBuffer::BUFFER*> BUFFER_INDEX;

		//! static buffer index
		static BUFFER_INDEX index;

		//! external ID of buffer
		string ext_id;

		//! pointer to buffer content
		BUFFER *buffer;

	public:
        MMSFBBuffer(string ext_id);
        ~MMSFBBuffer();
        bool isInitialized();
        bool initBuffer(INDEX_BUFFER index_buffer, VERTEX_BUFFER vertex_buffer);
        bool getBuffer(INDEX_BUFFER **index_buffer, VERTEX_BUFFER **vertex_buffer);
};

#endif /* MMSFBBUFFER_H_ */
