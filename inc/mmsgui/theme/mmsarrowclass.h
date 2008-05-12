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

#ifndef MMSARROWCLASS_H_
#define MMSARROWCLASS_H_

#include "mmsgui/theme/mmswidgetclass.h"

//! describe attributes for MMSArrow which are additional to the MMSWidgetClass 
namespace MMSGUI_ARROW_ATTR {

	#define MMSGUI_ARROW_ATTR_ATTRDESC \
		{ "color", TAFF_ATTRTYPE_STRING }, \
		{ "color.a", TAFF_ATTRTYPE_UCHAR }, \
		{ "color.r", TAFF_ATTRTYPE_UCHAR }, \
		{ "color.g", TAFF_ATTRTYPE_UCHAR }, \
		{ "color.b", TAFF_ATTRTYPE_UCHAR }, \
		{ "selcolor", TAFF_ATTRTYPE_STRING }, \
		{ "selcolor.a", TAFF_ATTRTYPE_UCHAR }, \
		{ "selcolor.r", TAFF_ATTRTYPE_UCHAR }, \
		{ "selcolor.g", TAFF_ATTRTYPE_UCHAR }, \
		{ "selcolor.b", TAFF_ATTRTYPE_UCHAR }, \
		{ "direction", TAFF_ATTRTYPE_STRING }
	
	#define MMSGUI_ARROW_ATTR_IDS \
		MMSGUI_ARROW_ATTR_IDS_color, \
		MMSGUI_ARROW_ATTR_IDS_color_a, \
		MMSGUI_ARROW_ATTR_IDS_color_r, \
		MMSGUI_ARROW_ATTR_IDS_color_g, \
		MMSGUI_ARROW_ATTR_IDS_color_b, \
		MMSGUI_ARROW_ATTR_IDS_selcolor, \
		MMSGUI_ARROW_ATTR_IDS_selcolor_a, \
		MMSGUI_ARROW_ATTR_IDS_selcolor_r, \
		MMSGUI_ARROW_ATTR_IDS_selcolor_g, \
		MMSGUI_ARROW_ATTR_IDS_selcolor_b, \
		MMSGUI_ARROW_ATTR_IDS_direction
	
	#define MMSGUI_ARROW_ATTR_INIT { \
		MMSGUI_BASE_ATTR_ATTRDESC, \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WIDGET_ATTR_ATTRDESC, \
		MMSGUI_ARROW_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BASE_ATTR_IDS,
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WIDGET_ATTR_IDS,
		MMSGUI_ARROW_ATTR_IDS
	} ids;
}

extern TAFF_ATTRDESC MMSGUI_ARROW_ATTR_I[];


//! A data access class for the arrow widget.
/*!
This class is the base for the MMSArrow widget class.
With this data store you have access to all changeable widget attributes. 
It is also one of the base classes for MMSThemeManager and MMSDialogManager
which are main features of the MMSGUI.
\note This class will be internally used by class MMSArrow.
\author Jens Schneider
*/
class MMSArrowClass {
    private:
    	//! name of the theme class
        string       className;
        
        //! is color set?
        bool         iscolor;
        
        //! color if the arrow is not selected
        DFBColor     color;
        
        //! is selcolor set?
        bool         isselcolor;
        
        //! color if the arrow is selected
        DFBColor     selcolor;
        
        //! is direction set?
        bool         isdirection;
        
        //! direction of the arrow
        MMSDIRECTION direction;

        //! Read and set all attributes from the given TAFF buffer.
        /*!
        \param tafff   pointer to the TAFF buffer
        \param prefix  optional, prefix to all attribute names (<prefix><attrname>=<attrvalue>)
        \param path    optional, path needed for empty path values from the TAFF buffer
        */
        void setAttributesFromTAFF(MMSTaffFile *tafff, string *prefix = NULL, string *path = NULL);

    public:
    	//! stores base widget attributes
        MMSWidgetClass widgetClass; 

        //! Constructor of class MMSArrowClass.
        MMSArrowClass();

        //! Mark all attributes as not set.
        void unsetAll();

        //! Set the name of the theme class.
        /*!
        \param classname  name of the class
        */
        void setClassName(string className);

        //! Get the name of the theme class.
        /*!
        \return name of the class
        */
        string getClassName();

        //! Check if the color is set. This color will be used for the unselected arrow.
        /*!
        \return true if the color is set
        */
        bool isColor();

        //! Set the color which is used to draw the unselected arrow.
        /*!
        \param color  color for unselected arrow
        */
        void setColor(DFBColor color);

        //! Mark the color as not set.
        void unsetColor();

        //! Get the color which is used to draw the unselected arrow.
        /*!
        \return color which is used for the unselected arrow
        */
        DFBColor getColor();

        //! Check if the color is set. This color will be used for the selected arrow.
        /*!
        \return true if the color is set
        */
        bool isSelColor();

        //! Set the color which is used to draw the selected arrow.
        /*!
        \param selcolor  color for selected arrow
        */
        void setSelColor(DFBColor selcolor);

        //! Mark the color as not set.
        void unsetSelColor();

        //! Get the color which is used to draw the selected arrow.
        /*!
        \return color which is used for the selected arrow
        */
        DFBColor getSelColor();

        //! Check if the direction is set.
        /*!
        \return true if the direction is set
        */
        bool isDirection();

        //! Set the direction of the arrow.
        /*!
        \param direction  direction, see definition of MMSDIRECTION
        */
        void setDirection(MMSDIRECTION direction);

        //! Mark the direction as not set.
        void unsetDirection();

        //! Get the current direction of the arrow.
        /*!
        \return direction
        */
        MMSDIRECTION getDirection();

    /* friends */
    friend class MMSThemeManager;
    friend class MMSDialogManager;
};

#endif /*MMSARROWCLASS_H_*/
