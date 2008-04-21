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

#include "mmstools/mmstafffile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


MMSTaffFile::MMSTaffFile(string taff_filename, TAFF_DESCRIPTION *taff_desc,
			             string external_filename, MMSTAFF_EXTERNAL_TYPE external_type,
			             bool trace, bool rewrite_taff) {
	this->taff_filename = taff_filename;
	this->taff_desc = taff_desc;
	this->taff_buf = NULL;
	this->taff_buf_size = 0;
	this->external_filename = external_filename;
	this->external_type = external_type;
	this->trace = trace;
	this->correct_version = false;

	/* read the file into taff_buf */
	bool ret = false;
	if (!rewrite_taff)
		ret = readFile();
	if (!ret)
		/* failed or rewrite, try to convert from external_filename */
		if (convertExternal2TAFF())
			/* auto read in */
			readFile();
}

MMSTaffFile::~MMSTaffFile() {
	if (this->taff_buf)
		free(this->taff_buf);
}

bool MMSTaffFile::convertXML2TAFF_throughDoc(int depth, xmlNode *node, MMSFile *taff_file) {

	xmlNode *cur_node;
	if (depth==0)
		/* work with root */
		cur_node = node;
	else
		/* iterate through childs */
		cur_node = node->children;
	
	while (cur_node) {

		int tagid = 0;
		bool tag_found = false;

		while (this->taff_desc->tagtable[tagid].name) {
			if (xmlStrcmp(cur_node->name, (const xmlChar *)this->taff_desc->tagtable[tagid].name)==0) {
				/* tag found in XML and tagtable, check the type? */
				if (this->taff_desc->tagtable[tagid].typeattr)
				{
					/* searching the typeattr and type */
					for (xmlAttr *cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) {
						if (xmlStrcmp(cur_attr->name, (const xmlChar *)this->taff_desc->tagtable[tagid].typeattr)==0) {
							xmlChar *attrVal = xmlGetProp(cur_node, cur_attr->name);
					    	if (attrVal) {
								if (xmlStrcmp(attrVal, (const xmlChar *)this->taff_desc->tagtable[tagid].type)==0) {
									tag_found = true;
									break;
								}
								xmlFree(attrVal);
					    	}
						}
					}
				}
				else
					tag_found = true;
				
				if (tag_found)
					break;
			}
			tagid++;
		}

		if (tag_found) {

			if (this->trace)
				printf("Tag \"%s\" found, ID=%d\n", cur_node->name, tagid);

			/* writing the new tag */
			if (taff_file) {
				size_t ritems;
				unsigned char wb[2];
				wb[0]=MMSTAFF_TAGTABLE_TYPE_TAG;
				wb[1]=(unsigned char)tagid;
				taff_file->writeBuffer(wb, &ritems, 1, 2);
			}

			/* get variables */
			for (xmlAttr *cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next) {

				int attrid = 0;
				TAFF_ATTRDESC *attr = this->taff_desc->tagtable[tagid].attr;
				bool attr_found = false;
				
				if (!attr) continue;

				xmlChar *attrVal = xmlGetProp(cur_node, cur_attr->name);
		    	if (!attrVal) continue;
				
				while (attr[attrid].name)
				{
					if (xmlStrcmp(cur_attr->name, (const xmlChar *)attr[attrid].name)==0) {
						attr_found = true;
						break;
					}
					attrid++;
				}

				if (attr_found) {

					/* now check the type of the variable */
					char *validvals = "";
					bool badval = false;
					switch (attr[attrid].type) {
					case TAFF_ATTRTYPE_NONE:
					case TAFF_ATTRTYPE_STRING:
						break;
					case TAFF_ATTRTYPE_BOOL:
						badval = ((xmlStrcmp(attrVal, (xmlChar *)"true"))&&(xmlStrcmp(attrVal, (xmlChar *)"false")));
						if (badval) {
							validvals = "\"true\", \"false\"";
							attr_found = false;
						}
						break;
					case TAFF_ATTRTYPE_UCHAR:
						badval = ((xmlStrlen(attrVal) < 1)||(xmlStrlen(attrVal) > 3));
						if (!badval) {
							int v = atoi((char*)attrVal);
							char vv[3+1];
							sprintf(vv, "%d", v);
							badval = (xmlStrcmp(attrVal, (xmlChar *)vv));
							if (!badval)
								badval = (v<0||v>255);
						}
						if (badval) {
							validvals = "\"0\"..\"255\"";
							attr_found = false;
						}
						break;
					}

					if (!attr_found) {
						printf("Error: Value \"%s\" is invalid for the attribute \"%s\" of tag \"%s\", line %d\n       valid values: %s\n",
										attrVal, cur_attr->name, cur_node->name, cur_node->line, validvals);
						return false;
					}
					
					
					/* all right */
					if (this->trace)
						printf(" Attribute \"%s\" found, ID=%d, value=\"%s\"\n", cur_attr->name, attrid, attrVal);

					/* write attribute value */
					if (taff_file) {
						/* get the length of the value INCLUDING the 0x00 because of fast read & parse of the TAFF */
						int attrvallen = xmlStrlen(attrVal) + 1;
	
						size_t ritems;
						unsigned char wb[3];
						wb[0]=MMSTAFF_TAGTABLE_TYPE_ATTR;
						wb[1]=(unsigned char)attrid;
						if (attrvallen >= 0xff) {
							/* in this case set 0xff as mark and append the full integer */
							wb[2]=0xff;
							taff_file->writeBuffer(wb, &ritems, 1, 3);
							taff_file->writeBuffer(&attrvallen, &ritems, 1, sizeof(int));
						}
						else {
							/* in this case write only one byte length */
							wb[2]=(unsigned char)attrvallen;
							taff_file->writeBuffer(wb, &ritems, 1, 3);
						}
						taff_file->writeBuffer(attrVal, &ritems, 1, attrvallen);
					}
				}
				else {
					printf("Error: Attribute \"%s\" is not defined for tag \"%s\", line %d\n", cur_attr->name,
										cur_node->name, cur_node->line);
					return false;
				}
				
				xmlFree(attrVal);
			}
			
			/* call recursive to find childs */
			if (!convertXML2TAFF_throughDoc(depth+1, cur_node, taff_file))
				return false;

			/* writing the close tag */
			if (taff_file) {
				size_t ritems;
				unsigned char wb[2];
				wb[0]=MMSTAFF_TAGTABLE_TYPE_CLOSETAG;
				wb[1]=(unsigned char)tagid;
				taff_file->writeBuffer(wb, &ritems, 1, 2);
			}
		}
		else {
			if (xmlStrcmp(cur_node->name, (const xmlChar *)"text")&&xmlStrcmp(cur_node->name, (const xmlChar *)"comment")) {
				printf("Error: Tag \"%s\" is not defined, line %d\n", cur_node->name, cur_node->line);
				return false;
			}
		}

		if (depth==0)
			break;
		else
			cur_node = cur_node->next;
	}

	return true;
	
}

bool MMSTaffFile::convertXML2TAFF() {

	bool   rc = false;
	xmlDoc *parser = NULL;
	
	LIBXML_TEST_VERSION

	/* check input parameters */
	if (!this->taff_desc) return false;
	if (this->external_filename=="") return false;
	
	
	/* read the XML file */
	parser = xmlReadFile(this->external_filename.c_str(), NULL, 0);
		
	if (parser) {

		/* open binary destination file */
		MMSFile *taff_file = NULL;
		if (this->taff_filename!="") {
			taff_file = new MMSFile(this->taff_filename.c_str(), MMSFM_WRITE);
			size_t ritems;
			taff_file->writeBuffer(&(this->taff_desc->type), &ritems, 1, sizeof(this->taff_desc->type));
			taff_file->writeBuffer(&(this->taff_desc->version), &ritems, 1, sizeof(this->taff_desc->version));
		}

		/* get the first element */
		xmlNode* node = xmlDocGetRootElement(parser);

		/* through the doc */
		rc = convertXML2TAFF_throughDoc(0, node, taff_file);

		/* close file and free */
		if (taff_file)
			delete taff_file;
	    xmlFreeDoc(parser);
	}
	else {
		printf("Error: cannot read external file\n");
	}
	
	return rc;
}

bool MMSTaffFile::convertExternal2TAFF() {
	switch (this->external_type) {
	case MMSTAFF_EXTERNAL_TYPE_XML:
		return convertXML2TAFF();
	}
}

bool MMSTaffFile::convertTAFF2XML_throughDoc(int depth, int tagid, MMSFile *external_file) {
	size_t ritems;
	char wb[1024];

	TAFF_TAGTABLE *tagt = &(this->taff_desc->tagtable[tagid]);
	TAFF_ATTRDESC *attr = tagt->attr;
	
	if (this->trace)
		printf("Tag \"%s\" found, ID=%d\n", tagt->name, tagid);

	/* write tag */
	if (external_file) {
		*wb = '\n';
		memset(&wb[1], ' ', depth*4);
		sprintf(&wb[1+depth*4], "<%s", tagt->name);
		external_file->writeBuffer(wb, &ritems, 1, strlen(wb));
	}

	/* write attributes */
	char *attrval;
	int attrid;
	while ((attrid = getNextAttribute(&attrval)) >= 0) {
		if (this->trace)
			printf(" Attribute \"%s\" found, ID=%d, value=\"%s\"\n", attr[attrid].name, attrid, attrval);

		if (external_file) {
			*wb = '\n';
			memset(&wb[1], ' ', depth*4+4);
			sprintf(&wb[1+depth*4+4], "%s = \"%s\"", attr[attrid].name, attrval);
			external_file->writeBuffer(wb, &ritems, 1, strlen(wb));
		}
	}

	/* close tag */
	if (external_file) {
		sprintf(wb, ">\n");
		external_file->writeBuffer(wb, &ritems, 1, strlen(wb));
	}

	/* through my child tags */
	while (1) {
		bool eof;
		int tid = getNextTag(eof);
		if (tid < 0) {
			/* close tag */
			if (external_file) {
				memset(wb, ' ', depth*4);
				sprintf(&wb[depth*4], "</%s>\n", tagt->name);
				external_file->writeBuffer(wb, &ritems, 1, strlen(wb));
			}
			return true;
		}
		if (convertTAFF2XML_throughDoc(depth+1, tid, external_file)==false)
			return false;
	}
}

bool MMSTaffFile::convertTAFF2XML() {
	if (!this->loaded) return false;
	reset();

	/* get root tag */
	bool eof;
	int tagid = getNextTag(eof);
	if (tagid < 0) return false;
	
	/* open binary destination file */
	MMSFile *external_file = NULL;
	if (this->external_filename!="")
		external_file = new MMSFile(this->external_filename.c_str(), MMSFM_WRITE);

	/* start with root */
	bool rc = convertTAFF2XML_throughDoc(0, tagid, external_file);

	if (external_file)
		delete external_file;
	
	return rc;
}

bool MMSTaffFile::convertTAFF2External() {
	switch (this->external_type) {
	case MMSTAFF_EXTERNAL_TYPE_XML:
		return convertTAFF2XML();
	}
}

bool MMSTaffFile::readFile() {
	if (this->taff_buf)
		free(this->taff_buf);
	this->loaded = false;

	if (!this->taff_desc) return false;
	if (this->taff_filename=="") return false;

	/* load the file */
	MMSFile *taff_file = new MMSFile(this->taff_filename.c_str(), MMSFM_READ, false);
	if (!taff_file) return false;
	size_t ritems;
	if (!taff_file->readBufferEx((void**)&(this->taff_buf), &ritems)) {
		delete taff_file;
		return false;
	}
	delete taff_file;

	if (ritems < 16) {
		/* wrong size */
		free(this->taff_buf);
		return false;
	}

	/* check the version of the file */
	this->correct_version = false;
	if (memcmp(this->taff_buf, &(this->taff_desc->type), sizeof(this->taff_desc->type))) {
		/* wrong type */
		free(this->taff_buf);
		return false;
	}
	if (memcmp(this->taff_buf+sizeof(this->taff_desc->type), &(this->taff_desc->version), sizeof(this->taff_desc->version))) {
		/* wrong version */
		free(this->taff_buf);
		return false;
	}
	this->correct_version = true;

	/* compare the modification time of the taff and external file */
	if (this->external_filename!="") {
        struct stat statbuf1;
        struct stat statbuf2;
        if (stat(this->taff_filename.c_str(), &statbuf1)!=0) {
    		free(this->taff_buf);
    		return false;
    	}
        if (stat(this->external_filename.c_str(), &statbuf2)==0) {
        	if (statbuf2.st_mtime >= statbuf1.st_mtime) {
                /* external file has been modified, therefor the taff file maybe not up-to-date */ 
        		free(this->taff_buf);
        		return false;
        	}
        }
	}

	/* all right */
	this->taff_buf_size = ritems;
	reset();
	this->loaded = true;
	return true;
}

bool MMSTaffFile::isLoaded() {
	return this->loaded;
}

bool MMSTaffFile::checkVersion() {
	return this->correct_version;
}

bool MMSTaffFile::setExternal(string external_filename, MMSTAFF_EXTERNAL_TYPE external_type) {
	this->external_filename = external_filename;
	this->external_type = external_type;
}

void MMSTaffFile::setTrace(bool trace) {
	this->trace = trace;
}

void MMSTaffFile::reset() {
	this->taff_buf_pos = sizeof(this->taff_desc->type) + sizeof(this->taff_desc->version);
}

int MMSTaffFile::getNextTag(bool &eof) {
	/* searching for next tag */
	do {
		switch (this->taff_buf[this->taff_buf_pos]) {
		case MMSTAFF_TAGTABLE_TYPE_TAG:
			this->taff_buf_pos+=2;
			return this->taff_buf[this->taff_buf_pos-1];
		case MMSTAFF_TAGTABLE_TYPE_ATTR: {
				this->taff_buf_pos+=2;
				int len = (int)this->taff_buf[this->taff_buf_pos];
				this->taff_buf_pos++;
				if (len >= 0xff) {
					len = *((int*)&this->taff_buf[this->taff_buf_pos]);
					this->taff_buf_pos+=sizeof(int);
				}
				this->taff_buf_pos+=len;
			}
			break;
		case MMSTAFF_TAGTABLE_TYPE_CLOSETAG:
			this->taff_buf_pos+=2;
			eof = false;
			return -1;
		default:
			eof = true;
			return -1;
		}
	} while (this->taff_buf_pos < this->taff_buf_size);
	eof = true;
	return -1;
}
		
int MMSTaffFile::getNextAttribute(char **value) {
	/* searching for next attribute */
	do {
		switch (this->taff_buf[this->taff_buf_pos]) {
		case MMSTAFF_TAGTABLE_TYPE_ATTR: {
			    int attrid = (int)this->taff_buf[this->taff_buf_pos+1];
				this->taff_buf_pos+=2;
				int len = (int)this->taff_buf[this->taff_buf_pos];
				this->taff_buf_pos++;
				if (len >= 0xff) {
					len = *((int*)&this->taff_buf[this->taff_buf_pos]);
					this->taff_buf_pos+=sizeof(int);
				}
				*value = (char*)&this->taff_buf[this->taff_buf_pos]; 
				this->taff_buf_pos+=len;
				return attrid;
			}
			break;
		default:
			return -1;
		}
	} while (this->taff_buf_pos < this->taff_buf_size);
	return -1;
}

