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

#ifndef MMSMORPHINERCPARSER_H_
#define MMSMORPHINERCPARSER_H_

#include "mmstools/mmstools.h"
#include "mmsconfig/mmsconfigdata.h"
#include <libxml/parser.h>
#include <libxml/tree.h>


class MMSRcParser {
	private:
    	MMSConfigDataGlobal global;
    	MMSConfigDataDB     configdb, datadb;
    	MMSConfigDataDFB    dfb;
	 	
    	void checkVersion(xmlNode* node);
    	void throughGlobal(xmlNode* node);
    	void throughDBSettings(xmlNode* node);
    	void throughDFBSettings(xmlNode* node);
    	void throughFile(xmlNode* node);

	public:
		MMSRcParser();
		~MMSRcParser();
		
		void parseFile(string filename);
		void getMMSRc(MMSConfigDataGlobal &global,
			          MMSConfigDataDB     &configdb,
			          MMSConfigDataDB     &datadb,
			          MMSConfigDataDFB    &dfb);
};

MMS_CREATEERROR(MMSRcParserError);

#endif /*MMSMORPHINERCPARSER_H_*/
