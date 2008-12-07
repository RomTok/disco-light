/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#ifndef MMSTCPCLIENT_H_
#define MMSTCPCLIENT_H_

#include "mmstools/base.h"

class MMSTCPClient {
	private:
		string			host;
		string			hostip;
		unsigned int	port;
		int 			s;

	public:
		MMSTCPClient(string host = "0.0.0.0", unsigned int port = 9999);
		void setAddress(string &host, unsigned int port);
		void setAddress(const char *host, unsigned int port);
		bool isConnected();
		bool connectToServer();
		bool disconnectFromServer();
		bool sendString(string rbuf);
		bool receiveString(string *abuf);
		bool receiveString(string *abuf, int buflen);
		bool peekString(string *abuf, int buflen);
		bool sendAndReceive(string rbuf, string *abuf);
};

#endif /*MMSTCPCLIENT_H_*/
