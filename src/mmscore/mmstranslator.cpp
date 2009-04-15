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
		this->sourcelang = config.getSourceLang();
		this->targetlang = config.getDefaultTargetLang();
		loadTransLations();
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
    	for(MMSFILEENTRY_LIST::iterator it2 = ret.begin(); it2 != ret.end();it2++) {
    		processFile((*it2)->name);
    	}
    }
    MMSFileSearch search(config.getData(),"translation.*",false);
    MMSFILEENTRY_LIST ret =  search.execute();
	for(MMSFILEENTRY_LIST::iterator it2 = ret.begin(); it2 != ret.end();it2++) {
		processFile((*it2)->name);
	}



}

void MMSTranslator::translate(string &source, string &dest) {
	if(targetlang != MMSLANG_UKN) {
		MMSTRANSLATION_MAP::iterator it = transmap.find(source);
		map<MMS_LANGUAGE_TYPE, string>::iterator it2;

		if(it!=transmap.end()) {
			it2 = it->second.find(targetlang);
			if(it2 != it->second.end()) {
				dest = it2->second;
			}
		}
	} else {
		dest = source;
	}
}

void MMSTranslator::setTargetLang(MMS_LANGUAGE_TYPE lang) {
	this->targetlang = lang;
	this->onTargetLangChanged.emit(lang);
}

void MMSTranslator::processFile(string &file) {
	MMSFile transfile(file,MMSFM_READ,false);
	string line;
	string from, to;
	MMS_LANGUAGE_TYPE lang =  strToLang(file.substr(file.find("translation")+12).c_str());

	//transfile.
	while(transfile.getLine(line)) {
		size_t pos = line.find("===");
		if(pos != string::npos) {
			from = line.substr(0, pos);
			to = line.substr(pos+3, string::npos);

			MMSTRANSLATION_MAP::iterator f = transmap.find(from);
			if(f != transmap.end()) {
				//already have the source
				DEBUGMSG("MMSTranslator", "insert: '%s'", from.c_str());
				pair<map<MMS_LANGUAGE_TYPE, string>::iterator, bool > ret = f->second.insert(make_pair(lang,to));
				if(ret.second == false) {
					DEBUGMSG("MMSTranslator", "double tranlation for: '%s'", from.c_str());
				}
			} else {
				DEBUGMSG("MMSTranslator", "fresh insert: '%s'", from.c_str());
				map<MMS_LANGUAGE_TYPE, string> mymap;
				mymap.insert(make_pair(lang,to));
				transmap.insert(make_pair(from, mymap));
			}
		}
	}
}
