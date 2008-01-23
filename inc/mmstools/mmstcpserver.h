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

#ifndef MMSTCPSERVER_H_
#define MMSTCPSERVER_H_

#include "mmstcpserverthread.h"

class MMSTCPServer : public MMSThread {
	private:
		vector<MMSTCPServerThread *> 	sthreads;
		unsigned int					st_size;
		unsigned int					st_cnt;
		string							host;
		string							hostip;
		unsigned int					port;
		int 							s;

	public:
		MMSTCPServer(vector<MMSServerInterface *> interfaces,
					 string host = "0.0.0.0", unsigned int port = 9999);
		virtual void threadMain();
};

#endif /*MMSTCPSERVER_H_*/
