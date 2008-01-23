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

#ifndef MMSFILESEARCH_H_
#define MMSFILESEARCH_H_

#include <base.h>
#include <mmserror.h>
#include <list>
#include <sys/types.h>
#include <dirent.h>

MMS_CREATEERROR(MMSFileSearchError);

#define MMSFILESEARCH_DEEPESTDIRENTRY "<DEEPESTDIRENTRY>;"
#define MMSFILESEARCH_DEEPESTDIRENTRY_OF_FILE "<DEEPESTDIRENTRYOFFILE>;"

typedef struct _mmsfile_entry {
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
		string directory;
		string mask;
		vector<string>  singlemask;
		DIR *dirhandle;
		bool match(char *entry);
		void scanDir(list<MMSFILE_ENTRY *> *result,DIR *dirhandle, string cwd);
		void seperateMask();
		MMSFILESEARCH_OPTION option;
		
	public:
		MMSFileSearch(string directory, string mask, bool recursive=true, bool caseinsensitive=false);
		void setRecursive(bool recursive);
		void setDircetory(string directory);
		void setString(string mask);
		void setCaseInsensitive(bool caseinsensitive);
		MMSFILEENTRY_LIST execute();
		
};
#endif /*MMSFILESEARCH_H_*/
