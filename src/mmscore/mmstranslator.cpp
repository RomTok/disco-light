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

#include "mmscore/mmstranslator.h"
#include "mmsconfig/mmsconfigdata.h"
#include "mmsconfig/mmspluginservice.h"
#include "mmstools/mmsfile.h"

#include <string.h>
MMS_LANGUAGE_TYPE MMSTranslator::sourcelang = MMSLANG_UKN;
MMS_LANGUAGE_TYPE MMSTranslator::targetlang = MMSLANG_UKN;
MMSTRANSLATION_MAP MMSTranslator::transmap;

MMSTranslator::MMSTranslator() {
	if(this->sourcelang == MMSLANG_UKN) {
		MMSConfigData config;
		string lang;
		lang = config.getSourceLang();
		if(strncasecmp(lang.c_str(),"eng",3)==0) {
			this->sourcelang = MMSLANG_ENG;
		} else if(strncasecmp(lang.c_str(),"ger",3)==0) {
			this->sourcelang = MMSLANG_GER;
		} else if(strncasecmp(lang.c_str(),"msg",3)==0) {
			this->sourcelang = MMSLANG_MSG;
		}
	}
	if(this->targetlang == MMSLANG_UKN) {
		MMSConfigData config;
		string lang;
		lang = config.getDefaultTargetLang();
		if(strncasecmp(lang.c_str(),"eng",3)==0) {
			this->targetlang = MMSLANG_ENG;
		} else if(strncasecmp(lang.c_str(),"ger",3)==0) {
			this->targetlang = MMSLANG_GER;
		} else if(strncasecmp(lang.c_str(),"msg",3)==0) {
			this->targetlang = MMSLANG_MSG;
		}
	}
}

MMSTranslator::~MMSTranslator() {

}

void MMSTranslator::loadTransLations() {
	MMSConfigData config;
    DataSource source(config.getConfigDBDBMS(),
					  config.getConfigDBDatabase(),
					  config.getConfigDBAddress(),
					  config.getConfigDBPort(),
					  config.getConfigDBUser(),
					  config.getConfigDBPassword());

    MMSPluginService service(&source);
    vector<MMSPluginData *> data = service.getAllPlugins();

    for(vector<MMSPluginData *>::iterator it = data.begin();it!=data.end();it++) {
    	MMSFileSearch search((*it)->getPath(),"translation.*",false);
    	MMSFILEENTRY_LIST ret =  search.execute();
    	for(MMSFILEENTRY_LIST::iterator it = ret.begin(); it != ret.end();it++) {
    		processFile((*it)->name);
    	}
    }


}

void MMSTranslator::translate(string &source, string &dest) {
	dest = source;
}

void MMSTranslator::setTargetLang(MMS_LANGUAGE_TYPE lang) {
	this->targetlang = lang;
}

void MMSTranslator::processFile(string &file) {
	MMSFile transfile(file,MMSFM_READ,false);
	string line;
	MMS_LANGUAGE_TYPE lang =  strToLang(file.substr(10).c_str());

	while(transfile.getLine(line)) {

	}
}
