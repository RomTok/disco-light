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

#include "mmstools/mmstcpserverthread.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <mmstools/tools.h>

MMSTCPServerThread::MMSTCPServerThread(MMSServerInterface *interface, int s, string identity) : MMSThread(identity) {
	this->interface = interface;
    this->s = s;
    this->request_buffer = "";
    this->answer_buffer = "";
}

bool MMSTCPServerThread::setSocket(int s) {
    this->s = s;
    return true;
}

void MMSTCPServerThread::threadMain() {
	char 	mybuf[4096+1];
	int		len, from;

	DEBUGMSG("MMSTCPServerThread", "process TCP Request");
	
	/* check somthing */
	if (!this->s) return;
	if (!this->interface) {
		close(this->s);
		this->s=-1;
		return;
	}

	/* receive request */
	this->request_buffer = "";
	do {
		if ((len = recv(this->s, mybuf, sizeof(mybuf)-1, 0))<0) {
			close(this->s);
			this->s=-1;
			return;
		}
		if (len>0) {
			mybuf[len]=0;
			this->request_buffer+= mybuf;
		}
	} while ((len>0)&&(mybuf[len-1]!=0));

	/* process request */
	this->answer_buffer = "";
	if (!this->interface->processRequest(&this->request_buffer, &this->answer_buffer)) {
		close(this->s);
		this->s=-1;
		return;
	}

	/* send answer */
	from = 0;
	do {
		strcpy(mybuf, (this->answer_buffer.substr(from, sizeof(mybuf)-1)).c_str());
		if (!*mybuf) break;
		if ((len = send(this->s, mybuf, strlen(mybuf), 0))<0) {
			close(this->s);
			this->s=-1;
			return;
		}
		from+=len;
	} while (len>0);
	send(this->s, "\0", 1, 0);

	close(this->s);
	this->s=-1;
}
