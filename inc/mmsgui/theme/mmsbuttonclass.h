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

#ifndef MMSBUTTONCLASS_H_
#define MMSBUTTONCLASS_H_

#include "mmsgui/theme/mmswidgetclass.h"

//describe attributes
namespace MMSGUI_BUTTON_ATTR {

	#define MMSGUI_BUTTON_ATTR_INIT { \
		MMSGUI_BASE_ATTR_ATTRDESC, \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WIDGET_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BASE_ATTR_IDS,
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WIDGET_ATTR_IDS
	} ids;
}

extern TAFF_ATTRDESC MMSGUI_BUTTON_ATTR_I[];


//! A data access class for the button widget.
/*!
This class is the base for the MMSButton widget class.
With this data store you have access to all changeable widget attributes. 
It is also one of the base classes for MMSThemeManager and MMSDialogManager
which are main features of the MMSGUI.
\author Jens Schneider
*/
class MMSButtonClass {
    private:
        string className;   //! name of the theme class

    public:
        MMSWidgetClass widgetClass; //! stores base widget attributes 

        ////////////////////////////////////////////////////////////////////////

        //! Constructor of class MMSButtonClass.
        MMSButtonClass();

        ////////////////////////////////////////////////////////////////////////

        //! Mark all attributes as not set.
        void unsetAll();

        ////////////////////////////////////////////////////////////////////////

        //! Read and set all attributes from the given XML node.
        /*!
        \param node    pointer to the XML node
        \param prefix  optional, prefix to all attribute names (<prefix><attrname>=<attrvalue>)
        \param path    optional, path needed for empty path values from the XML node
        */
        void setAttributesFromXMLNode(MMSTaffFile *tafff, string prefix = "", string path = "");

        ////////////////////////////////////////////////////////////////////////

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
};

#endif /*MMSBUTTONCLASS_H_*/
