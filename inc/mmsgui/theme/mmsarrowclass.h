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

#ifndef MMSARROWCLASS_H_
#define MMSARROWCLASS_H_

#include "theme/mmswidgetclass.h"

//! A data access class for the arrow widget.
/*!
This class is the base for the MMSArrow widget class.
With this data store you have access to all changeable widget attributes. 
It is also one of the base classes for MMSThemeManager and MMSDialogManager
which are main features of the MMSGUI.
\author Jens Schneider
*/
class MMSArrowClass {
    private:
        string       className;     //! name of the theme class
        bool         iscolor;       //! is color set? 
        DFBColor     color;         //! color if the arrow is not selected
        bool         isselcolor;    //! is selcolor set?
        DFBColor     selcolor;      //! color if the arrow is selected
        bool         isdirection;   //! is direction set?
        MMSDIRECTION direction;     //! direction of the arrow

    public:
        MMSWidgetClass widgetClass; //! stores base widget attributes 

        ////////////////////////////////////////////////////////////////////////

        //! Constructor of class MMSArrowClass.
        MMSArrowClass();

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
        void setAttributesFromXMLNode(xmlNode* node, string prefix = "", string path = "");

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

        ////////////////////////////////////////////////////////////////////////

        //! Check if the color is set. This color will be used for the unselected arrow.
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

        ////////////////////////////////////////////////////////////////////////

        //! Check if the color is set. This color will be used for the selected arrow.
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

        ////////////////////////////////////////////////////////////////////////

        //! Check if the direction is set.
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
};

#endif /*MMSARROWCLASS_H_*/
