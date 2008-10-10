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

#include "mmstools/mmstcpclient.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

MMSTCPClient::MMSTCPClient(string host, unsigned int port) {
    this->host = host;
    this->port = port;
    this->s = -1;
}

bool MMSTCPClient::isConnected() {
	return (this->s>=0);
}

bool MMSTCPClient::connectToServer() {
	struct hostent  	*he;
	struct in_addr  	ia;
	struct sockaddr_in	sa;

	if (this->s>=0) return true;

	/* get host ip in network byte order */
	he = gethostbyname(this->host.c_str());

	/* get host ip in numbers-and-dots */
	ia.s_addr = *((unsigned long int*)*(he->h_addr_list));
	this->hostip = inet_ntoa(ia);

	/* get a socket */
	if ((this->s = socket(AF_INET, SOCK_STREAM, 0))<=0) return false;

	/* connect to hostip */
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = this->port / 0x100 + (this->port % 0x100) * 0x100;
	sa.sin_addr = ia;
	if (connect(this->s, (struct sockaddr *)&sa, sizeof(struct sockaddr_in))!=0) {
		disconnectFromServer();
		return false;
	}

	/* connection established */
	return true;
}

bool MMSTCPClient::disconnectFromServer() {
	if (this->s<0) return true;
	close(this->s);
	this->s = -1;
	return true;
}

bool MMSTCPClient::sendString(string rbuf) {
	char 	mybuf[128000+1];
	int		len, from;

	if (!isConnected()) return false;

	/* send request */
	from = 0;
	do {
		strcpy(mybuf, (rbuf.substr(from, sizeof(mybuf)-1)).c_str());
		if (!*mybuf) break;
		if ((len = send(this->s, mybuf, strlen(mybuf), 0))<0) return false;
		from+=len;
	} while (len>0);
	send(this->s, "\0", 1, 0);

	return true;
}

bool MMSTCPClient::receiveString(string *abuf) {
	char 	mybuf[128000+1];
	int		len;

	if (!isConnected()) return false;

	/* receive answer */
	*abuf = "";
	do {
		if ((len = recv(this->s, mybuf, sizeof(mybuf)-1, 0))<0) return false;
		if (len>0) {
			mybuf[len]=0;
			(*abuf)+= mybuf;
		}
	} while ((len>0)&&(mybuf[len-1]!=0));

	return true;
}

bool MMSTCPClient::receiveString(string *abuf, int buflen) {
	//char 	mybuf[128000+1];
	char 	*mybuf;
	ssize_t		len;
	ssize_t received=0;

	if (!isConnected()) return false;

	mybuf = new char[buflen+1];

	memset(mybuf,0,buflen+1);
	/* receive answer */
	*abuf = "";
	do {
		if ((len = recv(this->s, &mybuf[received], buflen-received, MSG_WAITALL))<0) return false;

		received+=len;
		if (len>0) {
			mybuf[len]=0;
		}
	} while(received < buflen);

	*abuf= mybuf;
	delete mybuf;
	return true;
}

bool MMSTCPClient::peekString(string *abuf, int buflen) {
	char 	mybuf[128000+1];
	int		len;
	unsigned int received=0;

	if (!isConnected()) return false;
	memset(mybuf,0,128000+1);
	/* receive answer */
	*abuf = "";
	do {
		if ((len = recv(this->s, &mybuf[received], buflen-received, MSG_PEEK))<0) return false;

		received+=len;
		if (len>0) {
			mybuf[len]=0;
		}
	} while(received < buflen);

	(*abuf) = mybuf;
	return true;
}

bool MMSTCPClient::sendAndReceive(string rbuf, string *abuf) {
	bool	retcode = false;

	if (!connectToServer()) return false;
	if (sendString(rbuf))
		if (receiveString(abuf))
			retcode = true;
	disconnectFromServer();

	return retcode;
}


void MMSTCPClient::setAddress(string &host, unsigned int port)  {
	this->host = host;
	this->port = port;
}

void MMSTCPClient::setAddress(const char *host, unsigned int port) {
	this->host = host;
	this->port = port;
}
