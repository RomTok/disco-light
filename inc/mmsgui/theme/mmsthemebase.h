/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#ifndef MMSTHEMEBASE_H_
#define MMSTHEMEBASE_H_

#include "mmstools/mmstafffile.h"
#include "mmsgui/mmsguitools.h"



//! alignment type, used e.g. for positioning of windows, text in labels or textboxes, ...
typedef enum {
	//! not set
    MMSALIGNMENT_NOTSET=0,
    //! centered, horizontal and vertical
    MMSALIGNMENT_CENTER,
    //! left aligned, vertical centered
    MMSALIGNMENT_LEFT,
    //! right aligned, vertical centered
    MMSALIGNMENT_RIGHT,
    //! horizontal justified, vertical centered
    MMSALIGNMENT_JUSTIFY,
    //! horizontal centered, top aligned
    MMSALIGNMENT_TOP_CENTER,
    //! left and top aligned
    MMSALIGNMENT_TOP_LEFT,
    //! right and top aligned
    MMSALIGNMENT_TOP_RIGHT,
    //! horizontal justified, top aligned
    MMSALIGNMENT_TOP_JUSTIFY,
    //! horizontal centered, bottom aligned
    MMSALIGNMENT_BOTTOM_CENTER,
    //! left and bottom aligned
    MMSALIGNMENT_BOTTOM_LEFT,
    //! right and bottom aligned
    MMSALIGNMENT_BOTTOM_RIGHT,
    //! horizontal justified, bottom aligned
    MMSALIGNMENT_BOTTOM_JUSTIFY
} MMSALIGNMENT;


MMSALIGNMENT getAlignmentFromString(string inputstr);

typedef enum {
    MMSDIRECTION_NOTSET=0,
    MMSDIRECTION_LEFT,
    MMSDIRECTION_RIGHT,
    MMSDIRECTION_UP,
    MMSDIRECTION_DOWN
} MMSDIRECTION;


MMSDIRECTION getDirectionFromString(string inputstr);


#define MMSTHEMECLASS_INIT_STRING(x) \
	this->ed.x = NULL; \
    this->id.is##x = false;

#define MMSTHEMECLASS_FREE_STRING(x) \
	if (this->ed.x) delete this->ed.x; \
	this->ed.x = NULL; \
    this->id.is##x = false;

#define MMSTHEMECLASS_SET_STRING(x) \
	if (!this->ed.x) this->ed.x = new string(x); \
    else *(this->ed.x) = x; \
	this->id.is##x = true;

#define MMSTHEMECLASS_GET_STRING(x) \
	if (!this->id.is##x) return false; \
	x = *(this->ed.x); \
	return true;



#define MMSTHEMECLASS_INIT_STRINGS(x,c) \
	for (int cc=0;cc<c;cc++) \
		this->ed.x[cc] = NULL; \
    this->id.is##x = false;

#define MMSTHEMECLASS_FREE_STRINGS(x,c) \
	for (int cc=0;cc<c;cc++) { \
		if (this->ed.x[cc]) delete this->ed.x[cc]; \
		this->ed.x[cc] = NULL; \
	} \
    this->id.is##x = false;

#define MMSTHEMECLASS_SET_STRINGS(x,c,y) \
	if (!this->ed.x[c]) this->ed.x[c] = new string(y); \
	else *(this->ed.x[c]) = y; \
	this->id.is##x = true;

#define MMSTHEMECLASS_GET_STRINGS(x,c,y) \
	if (!this->id.is##x) return false; \
	if (!this->ed.x[c]) return false; \
	y = *(this->ed.x[c]); \
	return true;

#define MMSTHEMECLASS_UNSET_STRINGS(x,c) \
	for (int cc=0;cc<c;cc++) \
		if (this->ed.x[cc]) *(this->ed.x[cc]) = ""; \
	this->id.is##x = false;



#define MMSTHEMECLASS_INIT_BASIC(x) \
    this->id.is##x = false;

#define MMSTHEMECLASS_FREE_BASIC(x) \
    this->id.is##x = false;

#define MMSTHEMECLASS_SET_BASIC(x) \
    this->id.x = x; \
    this->id.is##x = true;

#define MMSTHEMECLASS_GET_BASIC(x) \
    if (!this->id.is##x) return false; \
    x = this->id.x; \
    return true;


#define MMSTHEMECLASS_UNSET(x) \
    this->id.is##x = false;

#define MMSTHEMECLASS_ISSET(x) \
    return this->id.is##x;




#define startTAFFScan \
	{ bool first=true; int attrid; char *attrval_str; int attrval_int; \
	while (1) { \
		if (first) { attrid=tafff->getFirstAttribute(&attrval_str, &attrval_int); first=false; } else \
		attrid=tafff->getNextAttribute(&attrval_str, &attrval_int); \
		if (attrid<0) break;

#define endTAFFScan } }


#define startTAFFScan_WITHOUT_ID \
	{ bool first=true; int attrid; char *attrval_str; int attrval_int; char *attrname; \
	while (1) { \
		if (first) { attrid=tafff->getFirstAttribute(&attrval_str, &attrval_int, &attrname); first=false; } else \
		attrid=tafff->getNextAttribute(&attrval_str, &attrval_int, &attrname); \
		while((attrid>=0)&&(attrid!=MMSTAFF_ATTR_WITHOUT_ID)) attrid=tafff->getNextAttribute(&attrval_str,&attrval_int,&attrname); \
		if (attrid<0) break;

#define endTAFFScan_WITHOUT_ID } }





namespace MMSGUI_MMSDIALOG_ATTR {

	#define MMSGUI_MMSDIALOG_ATTR_ATTRDESC \
		{ "name", TAFF_ATTRTYPE_STRING }

	#define MMSGUI_MMSDIALOG_ATTR_IDS \
		MMSGUI_MMSDIALOG_ATTR_IDS_name

	#define MMSGUI_MMSDIALOG_ATTR_INIT { \
		MMSGUI_MMSDIALOG_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_MMSDIALOG_ATTR_IDS
	} ids;
}


namespace MMSGUI_BASE_ATTR {

	#define MMSGUI_BASE_ATTR_ATTRDESC \
		{ "name", TAFF_ATTRTYPE_NE_STRING }, \
		{ "type", TAFF_ATTRTYPE_NE_STRING }, \
		{ "class", TAFF_ATTRTYPE_NE_STRING }, \
		{ "size", TAFF_ATTRTYPE_STRING }, \
		{ "show", TAFF_ATTRTYPE_BOOL }

	#define MMSGUI_BASE_ATTR_IDS \
		MMSGUI_BASE_ATTR_IDS_name, \
		MMSGUI_BASE_ATTR_IDS_type, \
		MMSGUI_BASE_ATTR_IDS_class, \
		MMSGUI_BASE_ATTR_IDS_size, \
		MMSGUI_BASE_ATTR_IDS_show

	#define MMSGUI_BASE_ATTR_INIT { \
		MMSGUI_BASE_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BASE_ATTR_IDS
	} ids;
}


namespace MMSGUI_FONT_ATTR {

	#define MMSGUI_FONT_ATTR_ATTRDESC \
		{ "font.path", TAFF_ATTRTYPE_STRING }, \
		{ "font.size", TAFF_ATTRTYPE_UCHAR }, \
		{ "font.name", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.de", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.en", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.dk", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.es", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.fi", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.fr", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.it", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.nl", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.no", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.se", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.tr", TAFF_ATTRTYPE_STRING }, \
		{ "font.name.cn", TAFF_ATTRTYPE_STRING }

	#define MMSGUI_FONT_ATTR_IDS \
		MMSGUI_FONT_ATTR_IDS_font_path, \
		MMSGUI_FONT_ATTR_IDS_font_size, \
		MMSGUI_FONT_ATTR_IDS_font_name, \
		MMSGUI_FONT_ATTR_IDS_font_name_de, \
		MMSGUI_FONT_ATTR_IDS_font_name_en, \
		MMSGUI_FONT_ATTR_IDS_font_name_dk, \
		MMSGUI_FONT_ATTR_IDS_font_name_es, \
		MMSGUI_FONT_ATTR_IDS_font_name_fi, \
		MMSGUI_FONT_ATTR_IDS_font_name_fr, \
		MMSGUI_FONT_ATTR_IDS_font_name_it, \
		MMSGUI_FONT_ATTR_IDS_font_name_nl, \
		MMSGUI_FONT_ATTR_IDS_font_name_no, \
		MMSGUI_FONT_ATTR_IDS_font_name_se, \
		MMSGUI_FONT_ATTR_IDS_font_name_tr, \
		MMSGUI_FONT_ATTR_IDS_font_name_cn

	#define MMSGUI_FONT_ATTR_INIT { \
		MMSGUI_FONT_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_FONT_ATTR_IDS
	} ids;
}


#define SET_FONT_FROM_TAFF(w) \
	case w::MMSGUI_FONT_ATTR_IDS_font_path: \
		if (*attrval_str) \
			setFontPath(attrval_str); \
		else \
			setFontPath((path)?*path:""); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_size: \
		setFontSize(attrval_int); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name: \
		setFontName(attrval_str); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_de: \
		setFontName(attrval_str, MMSLANG_DE); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_en: \
		setFontName(attrval_str, MMSLANG_EN); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_dk: \
		setFontName(attrval_str, MMSLANG_DK); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_es: \
		setFontName(attrval_str, MMSLANG_ES); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_fi: \
		setFontName(attrval_str, MMSLANG_FI); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_fr: \
		setFontName(attrval_str, MMSLANG_FR); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_it: \
		setFontName(attrval_str, MMSLANG_IT); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_nl: \
		setFontName(attrval_str, MMSLANG_NL); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_no: \
		setFontName(attrval_str, MMSLANG_NO); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_se: \
		setFontName(attrval_str, MMSLANG_SE); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_tr: \
		setFontName(attrval_str, MMSLANG_TR); \
		break; \
	case w::MMSGUI_FONT_ATTR_IDS_font_name_cn: \
		setFontName(attrval_str, MMSLANG_CN); \
		break;


#define SET_FONT_FROM_TAFF_WITH_PREFIX \
	if (ISFONTATTRNAME(font_path)) { \
		if (*attrval_str) \
			setFontPath(attrval_str); \
		else \
			setFontPath((path)?*path:""); \
	} \
	else \
	if (ISFONTATTRNAME(font_size)) { \
		setFontSize(attrval_int); \
	} \
	else \
	if (ISFONTATTRNAME(font_name)) { \
		setFontName(attrval_str); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_de)) { \
		setFontName(attrval_str, MMSLANG_DE); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_en)) { \
		setFontName(attrval_str, MMSLANG_EN); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_dk)) { \
		setFontName(attrval_str, MMSLANG_DK); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_es)) { \
		setFontName(attrval_str, MMSLANG_ES); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_fi)) { \
		setFontName(attrval_str, MMSLANG_FI); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_fr)) { \
		setFontName(attrval_str, MMSLANG_FR); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_it)) { \
		setFontName(attrval_str, MMSLANG_IT); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_nl)) { \
		setFontName(attrval_str, MMSLANG_NL); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_no)) { \
		setFontName(attrval_str, MMSLANG_NO); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_se)) { \
		setFontName(attrval_str, MMSLANG_SE); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_tr)) { \
		setFontName(attrval_str, MMSLANG_TR); \
	} \
	else \
	if (ISFONTATTRNAME(font_name_cn)) { \
		setFontName(attrval_str, MMSLANG_CN); \
	}


class MMSGUI_FONTS {
private:
    typedef struct {
        //! is fontname set?
        bool            isfontname;

        //! name of the font
        string          fontname;
    } MMSFONTNAME;

    MMSFONTNAME fonts[MMSLANG_SIZE];

public:
	//! constructor
	MMSGUI_FONTS() {
		unsetFontNames();
	}

    //! Check if the fontname is set.
    bool isFontName(MMSLanguage lang = MMSLANG_NONE) {
    	if (lang < MMSLANG_NONE || lang >= MMSLANG_SIZE) return false;
        return this->fonts[lang].isfontname;
    }

    //! Set the fontname which is used to draw the text.
    /*!
    \param fontname  name of the font
    */
    void setFontName(string fontname, MMSLanguage lang = MMSLANG_NONE) {
    	if (lang < MMSLANG_NONE || lang >= MMSLANG_SIZE) return;
    	this->fonts[lang].fontname = fontname;
    	this->fonts[lang].isfontname = true;
    }

    //! Mark the fontname as not set.
    void unsetFontName(MMSLanguage lang = MMSLANG_NONE) {
    	if (lang < MMSLANG_NONE || lang >= MMSLANG_SIZE) return;
    	this->fonts[lang].isfontname = false;
    }

    //! Mark all fontnames as not set.
    void unsetFontNames() {
    	for (unsigned int i = MMSLANG_NONE; i < MMSLANG_SIZE; i++) {
    		unsetFontName((MMSLanguage)i);
    	}
    }

    //! Get the fontname which is used to draw the text.
    /*!
    \return name of the font
    */
    string getFontName(MMSLanguage lang = MMSLANG_NONE) {
    	if (lang < MMSLANG_NONE || lang >= MMSLANG_SIZE) return "";
   		return this->fonts[lang].fontname;
    }

};



#endif /*MMSTHEMEBASE_H_*/


