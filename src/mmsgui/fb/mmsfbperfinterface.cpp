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

#include "mmsgui/fb/mmsfbperf.h"
#include "mmsgui/fb/mmsfbperfinterface.h"

MMSFBPerfInterface::MMSFBPerfInterface(class MMSFBPerf *mmsfbperf) : MMSServerInterface("MMSFBPerfInterface") {
	this->mmsfbperf = mmsfbperf;
}

MMSFBPerfInterface::~MMSFBPerfInterface() {
}

bool MMSFBPerfInterface::processRequest(string *request, string *answer) {
	if (this->mmsfbperf) {
		char retbuf[40960];
		char *rb = retbuf;

		rb+= sprintf(rb, "\n");
		rb+= sprintf(rb, "Function    Dest PF  Src PF   Flags Calls  MegaPix   MicroSecs   MP Per Sec\n");
		rb+= sprintf(rb, "---------------------------------------------------------------------------\n");
		rb+= this->mmsfbperf->getPerfVals(&this->mmsfbperf->fillrect, 	"FILLRECT", rb, sizeof(retbuf) - (unsigned int)(rb - retbuf));
		rb+= this->mmsfbperf->getPerfVals(&this->mmsfbperf->drawline, 	"DRAWLINE", rb, sizeof(retbuf) - (unsigned int)(rb - retbuf));
		rb+= this->mmsfbperf->getPerfVals(&this->mmsfbperf->drawstring, "DRAWSTRING", rb, sizeof(retbuf) - (unsigned int)(rb - retbuf));
		rb+= this->mmsfbperf->getPerfVals(&this->mmsfbperf->blit,		"BLIT", rb, sizeof(retbuf) - (unsigned int)(rb - retbuf));
		rb+= this->mmsfbperf->getPerfVals(&this->mmsfbperf->stretchblit,"STRETCHBLIT", rb, sizeof(retbuf) - (unsigned int)(rb - retbuf));
		rb+= sprintf(rb, "\n");

		*answer = retbuf;

		return true;
	}

	return false;
}
