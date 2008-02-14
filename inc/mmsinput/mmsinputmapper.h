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

#ifndef MMSINPUTMAPPER_H_
#define MMSINPUTMAPPER_H_

#include "mmstools/base.h"
#include "mmsbase/mmsbase.h"
#include "mmsinput/mmsinputhandler.h"

#include <directfb.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <directfb_keynames.h> 
#include <map>

typedef vector<DFBInputDeviceKeySymbol> MMS_KEYSET;

/* this class is used to map device input to keyset known to my application */
class MMSInputMapper {
	private:
        xmlDoc 			*parser;
        std::multimap	<string, string> keyMap;

		DFBInputDeviceKeySymbol lookUpKeySymbol(const string keyname);

	public:
		MMSInputMapper(string mapfile, string name = "default");
		~MMSInputMapper();
		void mapkey(MMSInputEvent *inputevent, vector<MMSInputEvent> *inputeventset);
        string lookUpKeyName(DFBInputDeviceKeySymbol key);
};
 
#endif /*MMSINPUTMAPPER_H_*/
