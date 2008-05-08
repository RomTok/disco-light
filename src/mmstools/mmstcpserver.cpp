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

#include "mmstools/mmstcpserver.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>

MMSTCPServer::MMSTCPServer(vector<MMSServerInterface *> interfaces,
					 	   string host, unsigned int port) : MMSThread("MMSTCPServer") {

	/* create threads and link a interface to each */
    this->st_size = interfaces.size();
	for (unsigned int i = 0; i < this->st_size; i++)
		this->sthreads.push_back(new MMSTCPServerThread(interfaces.at(i)));
    this->st_cnt = 0;

	/* connection data */
    this->host = host;
    this->port = port;
    this->s = -1;
}

void MMSTCPServer::threadMain() {
	struct hostent  	*he;
	struct in_addr  	ia;
	struct sockaddr_in	sa;
	struct sockaddr_in	sac;
	fd_set				readfds;
	fd_set				writefds;
	fd_set				errorfds;
	struct timeval		timeout;
	int					new_s;
	socklen_t			saclen;

	/* get host ip in network byte order */
	he = gethostbyname(this->host.c_str());

	/* get host ip in numbers-and-dots */
	ia.s_addr = *((unsigned long int*)*(he->h_addr_list));
	this->hostip = inet_ntoa(ia);

	/* get a socket */
	if ((this->s = socket(AF_INET, SOCK_STREAM, 0))<=0) return;

	/* bind to hostip */
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = this->port / 0x100 + (this->port % 0x100) * 0x100;
	sa.sin_addr = ia;
	if (bind(this->s, (struct sockaddr *)&sa, sizeof(struct sockaddr_in))!=0) return;

	/* listen/select/accept loop */
	while (listen(this->s, SOMAXCONN) == 0) {
		/* set filedescriptor */
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&errorfds);
		FD_SET(this->s, &readfds);
		FD_SET(this->s, &writefds);
		FD_SET(this->s, &errorfds);

		/* set timeout */
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		/* check socket */
		if (select(this->s+1, &readfds, &writefds, &errorfds, &timeout)<0) return;
		if (FD_ISSET(this->s, &readfds)) {
			/* going to accept the new connection */
			if ((new_s = accept(this->s, (struct sockaddr *)&sac, &saclen))<0) continue;
			
			/* call next server thread */
			if (this->st_size<=0) {
				close(new_s);
				continue;
			}
			if (this->st_cnt >= this->st_size) this->st_cnt=0;
			this->sthreads.at(this->st_cnt)->setSocket(new_s);
			this->sthreads.at(this->st_cnt)->start();
			this->st_cnt++;
		}
		else
		if (FD_ISSET(this->s, &writefds)) {
			printf ("WRITEFDS\n");
			return;
		}
		else
		if (FD_ISSET(this->s, &errorfds)) {
			printf ("ERRORFDS\n");
			return;
		}
		else {
			/* timeout */
		}
	}
}
