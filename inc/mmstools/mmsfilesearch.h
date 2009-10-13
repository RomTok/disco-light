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

#ifndef MMSFILESEARCH_H_
#define MMSFILESEARCH_H_

#include "mmstools/base.h"
#include "mmstools/mmserror.h"

#include <list>
#include <sys/types.h>
#include <dirent.h>

MMS_CREATEERROR(MMSFileSearchError);

#define MMSFILESEARCH_DEEPESTDIRENTRY "<DEEPESTDIRENTRY>;"
#define MMSFILESEARCH_DEEPESTDIRENTRY_OF_FILE "<DEEPESTDIRENTRYOFFILE>;"

typedef struct _mmsfile_entry {
	bool   isdir;
	string name;
	string basename;
	string path;
} MMSFILE_ENTRY;

typedef list<MMSFILE_ENTRY *> MMSFILEENTRY_LIST;

typedef enum {
	MMSFILESEARCH_NONE = 0,
	MMSFILESEARCH_DEEPESTDIR,
	MMSFILESEARCH_DEEPESTDIR_OF_FILE
} MMSFILESEARCH_OPTION;

class MMSFileSearch {

	private:
		bool recursive;
		bool caseinsensitive;
		bool getdirs;
		string directory;
		string mask;
		vector<string>  singlemask;
		DIR *dirhandle;
		bool match(char *entry);
		void scanDir(list<MMSFILE_ENTRY *> *result,DIR *dirhandle, string cwd);
		void seperateMask();
		MMSFILESEARCH_OPTION option;

	public:
		MMSFileSearch(string directory, string mask, bool recursive=true, bool caseinsensitive=false, bool getdirs = false);
		void setRecursive(bool recursive);
		void setDircetory(string directory);
		void setString(string mask);
		void setCaseInsensitive(bool caseinsensitive);
		MMSFILEENTRY_LIST execute();

};
#endif /*MMSFILESEARCH_H_*/
