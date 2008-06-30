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
#include "mmstools/mmsconverter.h"
#include "mmstools/tools.h"
#include <errno.h>
#include <string.h>

map<string, iconv_t> MMSConverter::trans;

MMSConverter::MMSConverter(string codepage) {
	map<string, iconv_t>::iterator it;
	iconv_t newtrans;

	it = trans.find(codepage);
	if(it==trans.end()) {
		// TODO: memory is never freed by calling iconv_close()
		newtrans = iconv_open("UTF-8",codepage.c_str());
		if(newtrans !=(iconv_t)(-1)) {
			trans.insert(make_pair(codepage, newtrans));
		}else {
			throw new MMSConverterError(0,"cannot create translation descriptor");
		}
	}

}

MMSConverter::~MMSConverter() {

}

string MMSConverter::convert(string frompage, string buffer) {
	map<string, iconv_t>::iterator it;
	char *lineptr;
	char *retlineptr;
	char line[32000];
	char retline[32000];
	size_t sizein, sizeout, sizeret;
	mutex.lock();
	it=trans.find(frompage);
	if(it!=trans.end()) {
		sizeout=32000;
		retlineptr=retline;
		memset(line,0,32000);
		memset(retline,0,32000);
		sprintf(line,"%s",buffer.c_str());
		sizein = strlen(line);
		lineptr=line;
		//logger.writeLog("before sizein(" + iToStr(sizein) + ") sizeout(" + iToStr(sizeout) + ")");
		//logger.writeLog("string before: |BEGIN|" + string(line) + "|END|");
		sizeret=iconv(it->second,&lineptr, &sizein,&retlineptr,&sizeout);
		//logger.writeLog("before sizein(" + iToStr(sizein) + ") sizeout(" + iToStr(sizeout) + ") sizeret(" + iToStr(sizeret) + ") " + strerror(errno));
		//logger.writeLog("string after: |BEGIN|" + string(retline) + "|END|");
		mutex.unlock();
		return retline;
	} else {
			mutex.unlock();
			throw new MMSConverterError(0,"have no translation descriptor");
	}
	mutex.unlock();
	return "";
}

MMSMutex MMSConverter::mutex;
