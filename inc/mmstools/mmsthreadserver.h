/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
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

#ifndef MMSTHREADSERVER_H_
#define MMSTHREADSERVER_H_

#include "mmstools/mmsthread.h"

//! This class is the base class for all threads which works as a server.
/*!
This class includes the base functionality e.g. the handshake between server and client threads.
This class cannot be constructed because processData() method is virtual and has to be
programmed in a derived class.
\author Jens Schneider
*/
class MMSThreadServer : public MMSThread {
	private:
		//! describes one item/request in the queue
		typedef struct {
			//! variable for conditional handling
			pthread_cond_t 	cond;
			//! mutex for conditional handling
			pthread_mutex_t	mutex;
			//! data to send
			void			*in_data;
			//! length of the in_data
			int				in_data_len;
			//! receive buffer
			void			**out_data;
			//! length of the out_data
			int				*out_data_len;
		} MMSTS_QUEUE_ITEM;

		//! request queue (ring buffer)
		MMSTS_QUEUE_ITEM **queue;

		//! number of items in the queue
		int queue_size;

		//! current item in the queue (read pointer)
		int queue_rp;

		//! first free item in the queue (write pointer)
		int queue_wp;

		//! mark the ring buffer as full
		bool buffer_full;

		//! variable for conditional handling (server side)
		pthread_cond_t 	cond;

		//! mutex for conditional handling (server side)
		pthread_mutex_t	mutex;

		//! server thread
		void threadMain();

	public:
		//! constructor
		MMSThreadServer(int queue_size = 1000, string identity = "MMSThreadServer");

		//! destructor
		~MMSThreadServer();

		//! will be called in the server loop
		virtual void processData(void *in_data, int in_data_len, void **out_data, int *out_data_len) = 0;

		//! Trigger a new event to the server.
        /*!
        This method sends data (in_data) from the caller (client) thread to the
        server thread. Afterwards the caller of this method will be blocked and is
        waiting for the answer from the server.
        \param in_data		pointer to data to put to the server
        \param in_data_len	length of in_data
        \param out_data		address of a pointer to receive data from the server
        \param out_data_len	address of a integer to get the length of out_data
        \return true if successful
        \note If server's queue is full, the caller is blocked until enough space in the queue.
        \note The handling of out_data and out_data_len is dependend on the implementation
              of the processData() method which is done in classes derived from MMSThreadServer.
        */
		bool trigger(void *in_data, int in_data_len, void **out_data = NULL, int *out_data_len = NULL);
};

#endif /*MMSTHREADSERVER_H_*/