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

#include "mmstools/mmsfilesearch.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fnmatch.h>
#include <string.h>


MMSFileSearch::MMSFileSearch(string directory, string mask, bool recursive, bool caseinsensitive, bool getdirs) {
	setDircetory(directory);
	this->mask = mask;
	this->recursive = recursive;
	this->caseinsensitive = caseinsensitive;
	this->getdirs = getdirs;
	this->option = MMSFILESEARCH_NONE;
	seperateMask();
}

bool MMSFileSearch::match(char *entry) {
	for(unsigned int i=0;i<singlemask.size();i++) {
		if(this->caseinsensitive == false) {
			if(fnmatch(singlemask.at(i).c_str(),entry,FNM_PATHNAME)==0) {
				return true;
			}
		} else {
			if(fnmatch(singlemask.at(i).c_str(),entry,FNM_PATHNAME|FNM_CASEFOLD)==0) {
				return true;
			}
		}
	}
	return false;
}

void MMSFileSearch::setRecursive(bool recursive) {
	this->recursive = recursive;
}

void MMSFileSearch::setDircetory(string directory) {
	if (directory=="")
		this->directory = "/";
	else
		this->directory = directory;
}

void MMSFileSearch::setString(string mask) {
	this->mask = mask;
}


void MMSFileSearch::setCaseInsensitive(bool caseinsensitive) {
	this->caseinsensitive = caseinsensitive;
}

void MMSFileSearch::seperateMask() {
	int pos = 0;
	int tmppos = 0;
	
	while(pos!=-1) {
		pos = this->mask.find_first_of(";",tmppos);
		if(pos != -1) {
			string tmpstring = this->mask.substr(tmppos,pos-tmppos);
			this->singlemask.push_back(tmpstring);
		}
		else {
			string tmpstring = this->mask.substr(tmppos,mask.size()-tmppos);
			this->singlemask.push_back(tmpstring);
		}
		tmppos = pos +1;
	}
	if(strncmp(this->singlemask.at(0).c_str(),MMSFILESEARCH_DEEPESTDIRENTRY,strlen(MMSFILESEARCH_DEEPESTDIRENTRY)-1)==0) {
		this->option = MMSFILESEARCH_DEEPESTDIR;
	} else if(strncmp(this->singlemask.at(0).c_str(),MMSFILESEARCH_DEEPESTDIRENTRY_OF_FILE,strlen(MMSFILESEARCH_DEEPESTDIRENTRY_OF_FILE)-1)==0) {
		this->option = MMSFILESEARCH_DEEPESTDIR_OF_FILE;
	}
	
}


list<MMSFILE_ENTRY *> MMSFileSearch::execute() {
    list<MMSFILE_ENTRY *> result;

    this->dirhandle = opendir(this->directory.c_str());
	scanDir(&result,this->dirhandle,(this->directory!="/")?this->directory:"");
    closedir(this->dirhandle);	 
    return result;
}

void MMSFileSearch::scanDir(list<MMSFILE_ENTRY *> *result,DIR *dirhandle, string cwd) {
	
	struct dirent *entry = readdir(dirhandle);
	bool filefound = false;
	bool dirfound = false;
	int  pos;	
	while(entry != NULL) {
		
		if(strcmp(entry->d_name,".")==0) {
			entry = readdir(dirhandle);
			continue;
		}
		if(strcmp(entry->d_name,"..")==0) {
			entry = readdir(dirhandle);
			continue;
		}
					
		if(entry->d_type == DT_DIR) {
			if (this->getdirs) {
				// put name of directory to the list
				if((this->option == MMSFILESEARCH_NONE)||(this->option == MMSFILESEARCH_DEEPESTDIR_OF_FILE)) {
					
					// we have a regular file -> and want to check it
					if(match(entry->d_name) == true) {
						filefound = true;

						// we have found sth;
						if((this->option != MMSFILESEARCH_DEEPESTDIR_OF_FILE)&&(this->option != MMSFILESEARCH_DEEPESTDIR)) {
														
							//we really really want it
							MMSFILE_ENTRY *file = new MMSFILE_ENTRY;
							file->isdir = true;
							file->name = cwd + "/" + entry->d_name;
							file->basename = entry->d_name;
							file->path = cwd;
							result->push_back(file);
						}
					}
				}
				
			}
			if(this->recursive == true) {
				// we have a directory -> recurse
				string newcwd = cwd + "/" + entry->d_name;
				DIR *directory = opendir(newcwd.c_str());
				if(directory != NULL) {
					dirfound = true;
					scanDir(result,directory,newcwd);
                    closedir(directory);
                }
			}
		}

		if(entry->d_type == DT_REG) {
			if((this->option == MMSFILESEARCH_NONE)||(this->option == MMSFILESEARCH_DEEPESTDIR_OF_FILE)) {
				
				// we have a regular file -> and want to check it
				if(match(entry->d_name) == true) {
					filefound = true;

					// we have found sth;
					if((this->option != MMSFILESEARCH_DEEPESTDIR_OF_FILE)&&(this->option != MMSFILESEARCH_DEEPESTDIR)) {
													
						//we really really want it
						MMSFILE_ENTRY *file = new MMSFILE_ENTRY;
						file->isdir = false;
						file->name = cwd + "/" + entry->d_name;
						file->basename = entry->d_name;
						file->path = cwd;
						result->push_back(file);
					}
				}
			}
		}
        /* if DT_UNKNOWN it is possible to check stat */
        else if(entry->d_type == DT_UNKNOWN) {
            struct stat sBuf;
            string fname = cwd + "/" + entry->d_name;
            if((stat(fname.c_str(), &sBuf) == 0) && (S_ISREG(sBuf.st_mode))) {
                if((this->option == MMSFILESEARCH_NONE)||(this->option == MMSFILESEARCH_DEEPESTDIR_OF_FILE)) {
                    // we have a regular file -> and want to check it
                    if(match(entry->d_name) == true) {
                        filefound = true;
    
                        // we have found sth;
                        if((this->option != MMSFILESEARCH_DEEPESTDIR_OF_FILE)&&(this->option != MMSFILESEARCH_DEEPESTDIR)) {
                                                        
                            //we really really want it
                            MMSFILE_ENTRY *file = new MMSFILE_ENTRY;
							file->isdir = false;
                            file->name = cwd + "/" + entry->d_name;
                            file->basename = entry->d_name;
                            file->path = cwd;
                            result->push_back(file);
                        }
                    }
                }
            }
            else if(S_ISDIR(sBuf.st_mode)) {
                if(this->recursive == true) {
                    // we have a directory -> recurse
                    string newcwd = cwd + "/" + entry->d_name;
                    DIR *directory = opendir(newcwd.c_str());
                    if(directory != NULL) {
                        dirfound = true;
                        scanDir(result,directory,newcwd);
                        closedir(directory);
                    }
                }
            }            
        }
        entry = readdir(dirhandle);
	}

	if((this->option == MMSFILESEARCH_DEEPESTDIR)&&(dirfound == false)) {

		// we have no dirs in here and should return the deepest directory entry
		MMSFILE_ENTRY *file = new MMSFILE_ENTRY;
		file->isdir = false;
		file->name = cwd;
		pos = cwd.find_last_of('/');
		file->basename = cwd.substr(pos+1);
		file->path = cwd.substr(0,pos+1);
		result->push_back(file);

	} else if((this->option == MMSFILESEARCH_DEEPESTDIR_OF_FILE)&&(filefound == true)) {

		//we should have at least one file until we really want it
		MMSFILE_ENTRY *file = new MMSFILE_ENTRY;
		file->isdir = false;
		file->name = cwd;
		pos = cwd.find_last_of('/');
		file->basename = cwd.substr(pos+1);
		file->path = cwd.substr(0,pos+1);
		result->push_back(file);

	}
}
