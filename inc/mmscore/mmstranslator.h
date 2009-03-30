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

#ifndef MMSTRANSLATOR_H_
#define MMSTRANSLATOR_H_

#include <map>
#include <string>

#include "mmsconfig/mmsconfigdata.h"

typedef std::map<MMS_LANGUAGE_TYPE,std::map<std::string, std::string> > MMSTRANSLATION_MAP;

#define TRANSLATION_FILE_NAME "translation"

class MMSTranslator {
	static MMS_LANGUAGE_TYPE sourcelang;
	static MMS_LANGUAGE_TYPE targetlang;
	static MMSTRANSLATION_MAP transmap;
	static bool addtranslations; 

	void loadTransLations();

	public:
		MMSTranslator();
		~MMSTranslator();

		void translate(std::string &source, std::string &dest);
		void setTargetLang(MMS_LANGUAGE_TYPE lang);
};

#endif /* MMSTRANSLATOR_H_ */
