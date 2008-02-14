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
#ifndef MMSCONVERTER_H_
#define MMSCONVERTER_H_

#include "mmstools/base.h"
#include "mmstools/mmserror.h"
#include "mmstools/mmslogger.h"
#include "mmstools/mmsmutex.h"

#include <iconv.h>
#include <map>

class MMSConverter {
	private:
		static map<string, iconv_t> trans;
		static MMSMutex mutex;
	public:
		MMSConverter(string codepage);
		~MMSConverter();
		string convert(string frompage, string buffer);	
	
};

MMS_CREATEERROR(MMSConverterError);

#endif /*MMSCONVERTER_H_*/
