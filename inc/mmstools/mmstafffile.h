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

//! TAFF Eyecatcher
#define TAFF_IDENT	"TAFF"

//! Types of TAFF attributes
typedef enum {
	//! not set
	TAFF_ATTRTYPE_NONE,
	//! any characters
	TAFF_ATTRTYPE_STRING,
	//! any characters, but not empty
	TAFF_ATTRTYPE_NE_STRING,
	//! valid values: "true", "false"
	TAFF_ATTRTYPE_BOOL,
	//! valid values: "0".."255"
	TAFF_ATTRTYPE_UCHAR,
	//! valid values: "0".."100"
	TAFF_ATTRTYPE_UCHAR100,
	//! valid values: "-2147483648".."2147483647"
	TAFF_ATTRTYPE_INT,
	//! any binary data
	TAFF_ATTRTYPE_BINDATA
} TAFF_ATTRTYPE;

//! Describe a TAFF attribute
typedef struct {
	//! name of attribute
	const char		*name;
	//! type of attribute
	TAFF_ATTRTYPE 	type;
} TAFF_ATTRDESC;

//! Describe a TAFF tag
typedef struct {
	//! name of tag
	const char		*name;
	//! name of special type attribute
	const char		*typeattr;
	//! value of special type attribute
	const char		*type;
	//! attributes
	TAFF_ATTRDESC 	*attr;
} TAFF_TAGTABLE;

//! Describe a TAFF file format
typedef struct {
	//! type of TAFF file
	char			type[32];
	//! type-based version
	unsigned int	version;
	//! tags
	TAFF_TAGTABLE	*tagtable;
} TAFF_DESCRIPTION;

//! Internal tag/attribute identifiers
typedef enum {
	//! internally identifies a close tag
	MMSTAFF_TAGTABLE_TYPE_CLOSETAG,
	//! internally identifies a tag
	MMSTAFF_TAGTABLE_TYPE_TAG,
	//! internally identifies a attribute
	MMSTAFF_TAGTABLE_TYPE_ATTR
};

//! internally identifies attributes without id (attribute names will be stored)
#define MMSTAFF_ATTR_WITHOUT_ID		0xff

//! Supported types of external files
typedef enum {
	//! the external file is written in XML
	MMSTAFF_EXTERNAL_TYPE_XML,
	//! the external file is an image (currently we only support PNG images) */
	MMSTAFF_EXTERNAL_TYPE_IMAGE
} MMSTAFF_EXTERNAL_TYPE;


//! A data access class for Tagged Attributes File Format (TAFF).
/*!
This class is written to generate an simple to parse binary presentation of
high level markup languages such as XML. For now only the conversion XML to TAFF
or vice versa and PNG to TAFF is supported.
The user of this class must specify a description of which tags and attributes
are allowed. Further he specifies the type of an attribute. With this informations
this class also checks the types and ranges of attributes during the conversion.
For example the MMSGUI works completely with TAFF.
\author Jens Schneider
*/
class MMSTaffFile {
	private:
		//! taff filename
		string 				taff_filename;

		//! describe the tags and attributes
		TAFF_DESCRIPTION 	*taff_desc;

		//! binary presentation data
		unsigned char 		*taff_buf;

		//! size of the buffer
		int					taff_buf_size;

		//! current read position
		int					taff_buf_pos;

		//! name of the external file for conversion
		string 					external_filename;

		//! type of the external file
		MMSTAFF_EXTERNAL_TYPE	external_type;

		//! ignore blank values during the conversion from external file
		bool    ignore_blank_values;

		//! print trace messages?
		bool 	trace;

		//!	print warnings?
		bool    print_warnings;

		//! is the TAFF buffer loaded?
		bool	loaded;

		//! has the TAFF file the correct version?
		bool	correct_version;

		//! id of the current tag
		int		current_tag;

		//! buffer postion of the current tag
		int		current_tag_pos;

		//! Internal method: Read a PNG Image.
		bool readPNG(const char *filename, void **buf, int *width, int *height, int *pitch, int *size, bool premultiplied);

		//! Internal method: Recursive called method for XML to TAFF conversion.
        bool convertXML2TAFF_throughDoc(int depth, void *void_node, MMSFile *taff_file);

        //! Internal method: XML to TAFF conversion.
        bool convertXML2TAFF();

        //! Internal method: IMAGE to TAFF conversion.
        bool convertIMAGE2TAFF();

        //! Internal method: Recursive called method for TAFF to XML conversion.
        bool convertTAFF2XML_throughDoc(int depth, int tagid, MMSFile *external_file);

        //! Internal method: TAFF to XML conversion.
        bool convertTAFF2XML();

	public:
        //! Constructor of class MMSTaffFile.
        /*!
        \param taff_filename		under this name the converted TAFF buffer is/will be stored
        \param taff_desc			the user of this class have to support this tag/attribute description, use NULL
                                    here if you use the external type MMSTAFF_EXTERNAL_TYPE_IMAGE
        \param external_filename	name of the external file for conversion
        \param external_type		type of the external file
        \param ignore_blank_values	ignore blank values during the conversion from external file
        \param trace				print trace messages?
        \param print_warnings		print warnings?
        \param rewrite_taff			(re-)convert from external file before loading TAFF
        */
		MMSTaffFile(string taff_filename, TAFF_DESCRIPTION *taff_desc,
        			string external_filename = "", MMSTAFF_EXTERNAL_TYPE external_type = MMSTAFF_EXTERNAL_TYPE_XML,
        			bool ignore_blank_values = false, bool trace = false, bool print_warnings = false, bool rewrite_taff = false);

        //! Destructor of class MMSTaffFile.
		~MMSTaffFile();

		//! Convert external file to TAFF.
        bool convertExternal2TAFF();

		//! Convert TAFF to external.
        bool convertTAFF2External();

		//! Read the TAFF file. This will normally done in the constructor.
        bool readFile();

        //! Is TAFF buffer filled?
        bool isLoaded();

        //! Has the TAFF file the correct version described in TAFF description.
        bool checkVersion();

        //! Set or reset the external file and type.
        /*!
        \param external_filename	name of the external file for conversion
        \param external_type		type of the external file
        */
        void setExternal(string external_filename = "", MMSTAFF_EXTERNAL_TYPE external_type = MMSTAFF_EXTERNAL_TYPE_XML);

        //! Switch trace on/off.
        /*!
        \param trace	print trace messages?
        */
        void setTrace(bool trace);

        //! Switch print warnings on/off.
        /*!
        \param print_warnings	print warnings?
        */
        void setPrintWarnings(bool print_warnings);

        //! Get the first tag id.
        /*!
        \return id of the tag or -1 if an error has occurred
        */
        int  getFirstTag();

        //! Get the next tag id.
        /*!
        \param eof	if eof set to true after calling this method, the end of file is reached
        \return id of the tag or -1 in case of close tag or eof
        */
        int  getNextTag(bool &eof);

        //! Get the current tag id.
        /*!
        \return id of the current tag
        */
        int  getCurrentTag();

        //! Copy the complete current tag into a new MMSTaffFile.
        /*!
        \return pointer to the new MMSTaffFile or NULL in case of errors
        */
        MMSTaffFile *copyCurrentTag();

        //! Get the first attribute of the current tag.
        /*!
        \param value_str	return pointer to null terminated value string or NULL if value is returned by value_int parameter
        \param value_int	a few types of attributes will be directly stored in binary format and will be returned by this parameter
        \param name			optional, with this parameter you can get the name of the attribute
        \return id of the attribute or -1 in case of close tag has reached
        */
        int  getFirstAttribute(char **value_str, int *value_int, char **name = NULL);

        //! Get the next attributes of the current tag.
        /*!
        \param value_str	return pointer to null terminated value string or NULL if value is returned by value_int parameter
        \param value_int	a few types of attributes will be directly stored in binary format and will be returned by this parameter
        \param name			optional, with this parameter you can get the name of the attribute
        \return id of the attribute or -1 in case of close tag has reached
        */
        int  getNextAttribute(char **value_str, int *value_int, char **name = NULL);

        //! Searching for an attribute id within the current tag.
        /*!
        \param id           attribute id to search for
        \param value_str	return pointer to null terminated value string or NULL if value is returned by value_int parameter
        \param value_int	a few types of attributes will be directly stored in binary format and will be returned by this parameter
        \return true if attribute was found
        */
        bool getAttribute(int id, char **value_str, int *value_int);

        //! Searching for an attribute id within the current tag.
        /*!
        \param id  	attribute id to search for
        \return pointer to null terminated value string or NULL
        */
        char *getAttributeString(int id);
};


namespace MMSTAFF_IMAGE_RAWIMAGE_ATTR {

	#define MMSTAFF_IMAGE_RAWIMAGE_ATTR_ATTRDESC \
		{ "width", TAFF_ATTRTYPE_INT }, \
		{ "height", TAFF_ATTRTYPE_INT }, \
		{ "pitch", TAFF_ATTRTYPE_INT }, \
		{ "size", TAFF_ATTRTYPE_INT }, \
		{ "data", TAFF_ATTRTYPE_BINDATA }

	#define MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS \
		MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_width, \
		MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_height, \
		MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_pitch, \
		MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_size, \
		MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS_data

	#define MMSTAFF_IMAGE_RAWIMAGE_ATTR_INIT { \
		MMSTAFF_IMAGE_RAWIMAGE_ATTR_ATTRDESC, \
		{NULL, TAFF_ATTRTYPE_NONE} \
	}

	typedef enum {
		MMSTAFF_IMAGE_RAWIMAGE_ATTR_IDS
	} ids;
}

extern TAFF_DESCRIPTION mmstaff_image_taff_description;

typedef enum {
	MMSTAFF_IMAGE_TAGTABLE_TAG_RAWIMAGE
};


#endif /*MMSTAFFFILE_H_*/
