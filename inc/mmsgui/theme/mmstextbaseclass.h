/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2010 BerLinux Solutions GmbH                       *
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

#ifndef MMSTEXTBASECLASS_H_
#define MMSTEXTBASECLASS_H_

#include "mmsgui/theme/mmswidgetclass.h"

// all definitions around fonts
///////////////////////////////////////////////////////////////////////////////

//! FONT macro for widget specific setAttributesFromTAFF() implementation
#define ISFONTATTRNAME(aname) ((strcmp(attrname, GETFONTATTRNAME(aname))==0)?(tafff->convertString2TaffAttributeType(GETFONTATTRTYPE(aname), attrval_str, &attrval_str_valid, &int_val_set, &byte_val_set, p_int_val, attrname, attrid, tafff->getCurrentTagName())):(0))


//! XML attributes for fonts
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


//! FONT macro for widget specific setAttributesFromTAFF() implementation
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


//! FONT macro for widget specific setAttributesFromTAFF() implementation
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





// all definitions around shadows
///////////////////////////////////////////////////////////////////////////////

//! SHADOW macro for widget specific setAttributesFromTAFF() implementation
#define ISSHADOWATTRNAME(aname) ((strcmp(attrname, GETSHADOWATTRNAME(aname))==0)?(tafff->convertString2TaffAttributeType(GETSHADOWATTRTYPE(aname), attrval_str, &attrval_str_valid, &int_val_set, &byte_val_set, p_int_val, attrname, attrid, tafff->getCurrentTagName())):(0))


//! XML attributes for shadows
namespace MMSGUI_SHADOW_ATTR {

	#define MMSGUI_SHADOW_ATTR_ATTRDESC \
		{ "shadow.top.color", TAFF_ATTRTYPE_COLOR }, \
		{ "shadow.bottom.color", TAFF_ATTRTYPE_COLOR }, \
		{ "shadow.left.color", TAFF_ATTRTYPE_COLOR }, \
		{ "shadow.right.color", TAFF_ATTRTYPE_COLOR }, \
		{ "shadow.top-left.color", TAFF_ATTRTYPE_COLOR }, \
		{ "shadow.top-right.color", TAFF_ATTRTYPE_COLOR }, \
		{ "shadow.bottom-left.color", TAFF_ATTRTYPE_COLOR }, \
		{ "shadow.bottom-right.color", TAFF_ATTRTYPE_COLOR }, \
		{ "selshadow.top.color", TAFF_ATTRTYPE_COLOR }, \
		{ "selshadow.bottom.color", TAFF_ATTRTYPE_COLOR }, \
		{ "selshadow.left.color", TAFF_ATTRTYPE_COLOR }, \
		{ "selshadow.right.color", TAFF_ATTRTYPE_COLOR }, \
		{ "selshadow.top-left.color", TAFF_ATTRTYPE_COLOR }, \
		{ "selshadow.top-right.color", TAFF_ATTRTYPE_COLOR }, \
		{ "selshadow.bottom-left.color", TAFF_ATTRTYPE_COLOR }, \
		{ "selshadow.bottom-right.color", TAFF_ATTRTYPE_COLOR }

	#define MMSGUI_SHADOW_ATTR_IDS \
		MMSGUI_SHADOW_ATTR_IDS_shadow_top_color, \
		MMSGUI_SHADOW_ATTR_IDS_shadow_bottom_color, \
		MMSGUI_SHADOW_ATTR_IDS_shadow_left_color, \
		MMSGUI_SHADOW_ATTR_IDS_shadow_right_color, \
		MMSGUI_SHADOW_ATTR_IDS_shadow_top_left_color, \
		MMSGUI_SHADOW_ATTR_IDS_shadow_top_right_color, \
		MMSGUI_SHADOW_ATTR_IDS_shadow_bottom_left_color, \
		MMSGUI_SHADOW_ATTR_IDS_shadow_bottom_right_color, \
		MMSGUI_SHADOW_ATTR_IDS_selshadow_top_color, \
		MMSGUI_SHADOW_ATTR_IDS_selshadow_bottom_color, \
		MMSGUI_SHADOW_ATTR_IDS_selshadow_left_color, \
		MMSGUI_SHADOW_ATTR_IDS_selshadow_right_color, \
		MMSGUI_SHADOW_ATTR_IDS_selshadow_top_left_color, \
		MMSGUI_SHADOW_ATTR_IDS_selshadow_top_right_color, \
		MMSGUI_SHADOW_ATTR_IDS_selshadow_bottom_left_color, \
		MMSGUI_SHADOW_ATTR_IDS_selshadow_bottom_right_color

	#define MMSGUI_SHADOW_ATTR_INIT { \
		MMSGUI_SHADOW_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_SHADOW_ATTR_IDS
	} ids;
}


//! SHADOW macro for widget specific setAttributesFromTAFF() implementation
#define SET_SHADOW_FROM_TAFF(w) \
	case w::MMSGUI_SHADOW_ATTR_IDS_shadow_top_color: \
		setShadowColor(MMSPOSITION_TOP, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_shadow_bottom_color: \
		setShadowColor(MMSPOSITION_BOTTOM, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_shadow_left_color: \
		setShadowColor(MMSPOSITION_LEFT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_shadow_right_color: \
		setShadowColor(MMSPOSITION_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_shadow_top_left_color: \
		setShadowColor(MMSPOSITION_TOP_LEFT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_shadow_top_right_color: \
		setShadowColor(MMSPOSITION_TOP_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_shadow_bottom_left_color: \
		setShadowColor(MMSPOSITION_BOTTOM_LEFT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_shadow_bottom_right_color: \
		setShadowColor(MMSPOSITION_BOTTOM_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_selshadow_top_color: \
		setSelShadowColor(MMSPOSITION_TOP, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_selshadow_bottom_color: \
		setSelShadowColor(MMSPOSITION_BOTTOM, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_selshadow_left_color: \
		setSelShadowColor(MMSPOSITION_LEFT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_selshadow_right_color: \
		setSelShadowColor(MMSPOSITION_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_selshadow_top_left_color: \
		setSelShadowColor(MMSPOSITION_TOP_LEFT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_selshadow_top_right_color: \
		setSelShadowColor(MMSPOSITION_TOP_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_selshadow_bottom_left_color: \
		setSelShadowColor(MMSPOSITION_BOTTOM_LEFT, MMSFBColor((unsigned int)attrval_int)); \
		break; \
	case w::MMSGUI_SHADOW_ATTR_IDS_selshadow_bottom_right_color: \
		setSelShadowColor(MMSPOSITION_BOTTOM_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
		break;


//! SHADOW macro for widget specific setAttributesFromTAFF() implementation
#define SET_SHADOW_FROM_TAFF_WITH_PREFIX \
	if (ISSHADOWATTRNAME(shadow_top_color)) { \
		setShadowColor(MMSPOSITION_TOP, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(shadow_bottom_color)) { \
		setShadowColor(MMSPOSITION_BOTTOM, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(shadow_left_color)) { \
		setShadowColor(MMSPOSITION_LEFT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(shadow_right_color)) { \
		setShadowColor(MMSPOSITION_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(shadow_top_left_color)) { \
		setShadowColor(MMSPOSITION_TOP_LEFT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(shadow_top_right_color)) { \
		setShadowColor(MMSPOSITION_TOP_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(shadow_bottom_left_color)) { \
		setShadowColor(MMSPOSITION_BOTTOM_LEFT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(shadow_bottom_right_color)) { \
		setShadowColor(MMSPOSITION_BOTTOM_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(selshadow_top_color)) { \
		setSelShadowColor(MMSPOSITION_TOP, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(selshadow_bottom_color)) { \
		setSelShadowColor(MMSPOSITION_BOTTOM, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(selshadow_left_color)) { \
		setSelShadowColor(MMSPOSITION_LEFT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(selshadow_right_color)) { \
		setSelShadowColor(MMSPOSITION_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(selshadow_top_left_color)) { \
		setSelShadowColor(MMSPOSITION_TOP_LEFT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(selshadow_top_right_color)) { \
		setSelShadowColor(MMSPOSITION_TOP_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(selshadow_bottom_left_color)) { \
		setSelShadowColor(MMSPOSITION_BOTTOM_LEFT, MMSFBColor((unsigned int)attrval_int)); \
	} \
	else \
	if (ISSHADOWATTRNAME(selshadow_bottom_right_color)) { \
		setSelShadowColor(MMSPOSITION_BOTTOM_RIGHT, MMSFBColor((unsigned int)attrval_int)); \
	}





//! A base class for MMSLabelWidgetClass, MMSTextBoxWidgetClass and MMSInputWidgetClass.
/*!
\note This class will be internally used by MMSLabelWidgetClass, MMSTextBoxWidgetClass and MMSInputWidgetClass.
\author Jens Schneider
*/
class MMSTextBaseClass {
    private:
        //! is fontpath set?
        bool            isfontpath;

        //! path to the font
        string          fontpath;

        //! is fontsize set?
        bool            isfontsize;

        //! size of the font
        unsigned int    fontsize;


        //! describes name of a font
        typedef struct {
            //! is fontname set?
            bool            isfontname;

            //! name of the font
            string          fontname;
        } MMSTEXTBASEFONTNAME;

        //! language dependent font filenames
        MMSTEXTBASEFONTNAME	fontname[MMSLANG_SIZE];



        //! is alignment set?
        bool            isalignment;

        //! alignment of the text
        MMSALIGNMENT    alignment;

        //! is color set?
        bool            iscolor;

        //! color of the text if the widget is not selected
        MMSFBColor      color;

        //! is selcolor set?
        bool            isselcolor;

        //! color of the text if the widget is selected
        MMSFBColor      selcolor;

        //! is text set?
        bool            istext;

        //! text to draw
        string          text;



        //! describes shadow of a text
        typedef struct {
            //! is color set?
            bool            iscolor;

            //! color of the text if the widget is not selected
            MMSFBColor      color;

            //! is selcolor set?
            bool            isselcolor;

            //! color of the text if the widget is selected
            MMSFBColor      selcolor;
        } MMSTEXTBASESHADOW;

        //! text shadows (eight directions)
        MMSTEXTBASESHADOW	shadow[MMSPOSITION_SIZE];


    public:
        //! Constructor of class MMSTextBaseClass.
        MMSTextBaseClass();

        //! Mark all attributes as not set.
        virtual void unsetAll();


        //! Check if the fontpath is set.
        bool isFontPath();

        //! Set the fontpath which is used to draw the text.
        /*!
        \param fontpath  path to the font
        */
        void setFontPath(string fontpath);

        //! Mark the fontpath as not set.
        void unsetFontPath();

        //! Get the fontpath which is used to draw the text.
        /*!
        \return path to the font
        */
        string getFontPath();


        //! Check if the fontsize is set.
        bool isFontSize();

        //! Set the fontsize which is used to draw the text.
        /*!
        \param fontsize  size of the font
        */
        void setFontSize(unsigned int fontsize);

        //! Mark the fontsize as not set.
        void unsetFontSize();

        //! Get the fontsize which is used to draw the text.
        /*!
        \return size of the font
        */
        unsigned int getFontSize();


        //! Check if the fontname is set.
        /*!
        \param lang  optional language
        */
        bool isFontName(MMSLanguage lang = MMSLANG_NONE);

        //! Set the fontname which is used to draw the text.
        /*!
        \param fontname  name of the font
        \param lang      optional language
        */
        void setFontName(string fontname, MMSLanguage lang = MMSLANG_NONE);

        //! Mark the fontname as not set.
        /*!
        \param lang  optional language
        */
        void unsetFontName(MMSLanguage lang = MMSLANG_NONE);

        //! Mark all fontnames as not set.
        void unsetFontNames();

        //! Get the fontname which is used to draw the text.
        /*!
        \param lang  optional language
        \return name of the font
        */
        string getFontName(MMSLanguage lang = MMSLANG_NONE);


        //! Check if alignment is set.
        bool isAlignment();

        //! Set the alignment of the text (see MMSALIGNMENT values).
        /*!
        \param alignment  text alignment
        */
        void setAlignment(MMSALIGNMENT alignment);

        //! Mark the alignment as not set.
        void unsetAlignment();

        //! Get the alignment of the text (see MMSALIGNMENT values).
        /*!
        \return size of the font
        */
        MMSALIGNMENT getAlignment();

        //! Check if the color is set. This color will be used for the unselected text.
        bool isColor();

        //! Set the color which is used to draw the unselected text.
        /*!
        \param color  color for unselected text
        */
        void setColor(MMSFBColor color);

        //! Mark the color as not set.
        void unsetColor();

        //! Get the color which is used to draw the unselected text.
        /*!
        \return color for unselected text
        */
        MMSFBColor getColor();

        //! Check if the color is set. This color will be used for the selected text.
        bool isSelColor();

        //! Set the color which is used to draw the selected text.
        /*!
        \param selcolor  color for selected text
        */
        void setSelColor(MMSFBColor selcolor);

        //! Mark the color as not set.
        void unsetSelColor();

        //! Get the color which is used to draw the selected text.
        /*!
        \return color for selected text
        */
        MMSFBColor getSelColor();

        //! Check if the text is set.
        bool isText();

        //! Set the text to be drawn.
        /*!
        \param text  pointer to any text string
        */
        void setText(string *text);

        //! Set the text to be drawn.
        /*!
        \param text  any text string
        */
        void setText(string text);

        //! Mark the text as not set.
        void unsetText();

        //! Get the current text.
        /*!
        \return text string
        */
        string getText();



        bool isShadowColor(MMSPOSITION position);
        void setShadowColor(MMSPOSITION position, MMSFBColor color);
        void unsetShadowColor(MMSPOSITION position);
        void unsetShadowColors();
        MMSFBColor getShadowColor(MMSPOSITION position);

        bool isSelShadowColor(MMSPOSITION position);
        void setSelShadowColor(MMSPOSITION position, MMSFBColor selcolor);
        void unsetSelShadowColor(MMSPOSITION position);
        void unsetSelShadowColors();
        MMSFBColor getSelShadowColor(MMSPOSITION position);


    // friends
    friend class MMSThemeManager;
    friend class MMSDialogManager;
};

#endif /*MMSTEXTBASECLASS_H_*/
