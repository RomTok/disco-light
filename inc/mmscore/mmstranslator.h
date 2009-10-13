/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
 ***************************************************************************/

#ifndef MMSTRANSLATOR_H_
#define MMSTRANSLATOR_H_

#include <map>
#include <string>
#include <sigc++/sigc++.h>

#include "mmsconfig/mmsconfigdata.h"

typedef std::map<std::string, std::map<MMS_LANGUAGE_TYPE, std::string> > MMSTRANSLATION_MAP;
typedef std::map<MMS_LANGUAGE_TYPE, std::string> MMSTRANSLATION_FILES;

#define TRANSLATION_FILE_NAME "translation"

class MMSTranslator {
	static MMS_LANGUAGE_TYPE sourcelang;
	static MMS_LANGUAGE_TYPE targetlang;
	static MMSTRANSLATION_MAP transmap;
	static bool addtranslations;
	static MMSTRANSLATION_FILES files;

	void loadTransLations();
	void processFile(string &file);
	void addMissing(string &phrase, bool completemiss = false);
	public:
		MMSTranslator();
		~MMSTranslator();

		void translate(std::string &source, std::string &dest);
		void setTargetLang(MMS_LANGUAGE_TYPE lang);

        static sigc::signal<void, MMS_LANGUAGE_TYPE> onTargetLangChanged;
};

#endif /* MMSTRANSLATOR_H_ */
