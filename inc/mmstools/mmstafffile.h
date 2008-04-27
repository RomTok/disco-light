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

#ifndef MMSTAFFFILE_H_
#define MMSTAFFFILE_H_

#include "mmstools/base.h"
#include "mmstools/mmsfile.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

typedef enum {
	TAFF_ATTRTYPE_NONE,			/* not set */
	TAFF_ATTRTYPE_STRING,		/* any characters */
	TAFF_ATTRTYPE_NE_STRING,	/* any characters, but not empty */
	TAFF_ATTRTYPE_BOOL,			/* valid values: "true", "false" */
	TAFF_ATTRTYPE_UCHAR			/* valid values: "0".."255" */
} TAFF_ATTRTYPE;

typedef struct {
	char 			*name;
	TAFF_ATTRTYPE 	type;
} TAFF_ATTRDESC;

typedef struct {
	char 			*name;
	char 			*typeattr;
	char 			*type;
	TAFF_ATTRDESC 	*attr;
} TAFF_TAGTABLE;

typedef struct {
	char			type[8];
	unsigned int	version;
	TAFF_TAGTABLE	*tagtable;
} TAFF_DESCRIPTION;

typedef enum {
	MMSTAFF_TAGTABLE_TYPE_CLOSETAG,
	MMSTAFF_TAGTABLE_TYPE_TAG,
	MMSTAFF_TAGTABLE_TYPE_ATTR
};

typedef enum { 
	MMSTAFF_EXTERNAL_TYPE_XML
} MMSTAFF_EXTERNAL_TYPE;

class MMSTaffFile {
	private:
		string 				taff_filename;
		TAFF_DESCRIPTION 	*taff_desc;
		unsigned char 		*taff_buf;
		int					taff_buf_size;
		int					taff_buf_pos;

		string 					external_filename;
		MMSTAFF_EXTERNAL_TYPE	external_type;

		bool    ignore_blank_values;
		bool 	trace;
		bool	loaded;
		bool	correct_version;

		int		current_tag;
		int		current_tag_pos;
		
        bool convertXML2TAFF_throughDoc(int depth, xmlNode *node, MMSFile *taff_file);
        bool convertXML2TAFF();

        bool convertTAFF2XML_throughDoc(int depth, int tagid, MMSFile *external_file);
        bool convertTAFF2XML();

	public:
        MMSTaffFile(string taff_filename, TAFF_DESCRIPTION *taff_desc,
        			string external_filename = "", MMSTAFF_EXTERNAL_TYPE external_type = MMSTAFF_EXTERNAL_TYPE_XML,
        			bool ignore_blank_values = false, bool trace = false, bool rewrite_taff = false);
        ~MMSTaffFile(); 

        bool convertExternal2TAFF();
        bool convertTAFF2External();

        bool readFile();
        bool isLoaded();
        bool checkVersion();

        bool setExternal(string external_filename = "", MMSTAFF_EXTERNAL_TYPE external_type = MMSTAFF_EXTERNAL_TYPE_XML);
        void setTrace(bool trace);
        
        int  getFirstTag();
        int  getNextTag(bool &eof);
        int  getCurrentTag();
        MMSTaffFile *copyCurrentTag();

        int  getFirstAttribute(char **value_str, int *value_int);
        int  getNextAttribute(char **value_str, int *value_int);
        bool getAttribute(int id, char **value_str, int *value_int);
        char *getAttributeString(int id);
};


#endif /*MMSTAFFFILE_H_*/
