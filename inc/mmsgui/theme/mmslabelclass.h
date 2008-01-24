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

#ifndef MMSLABELCLASS_H_
#define MMSLABELCLASS_H_

#include "mmsgui/theme/mmswidgetclass.h"

//! A data access class for the label widget.
/*!
This class is the base for the MMSLabel widget class.
With this data store you have access to all changeable widget attributes. 
It is also one of the base classes for MMSThemeManager and MMSDialogManager
which are main features of the MMSGUI.
\author Jens Schneider
*/
class MMSLabelClass {
    private:
        string          className;  //! name of the theme class
        bool            isfontpath; //! is fontpath set?
        string          fontpath;   //! path to the font
        bool            isfontname; //! is fontname set?
        string          fontname;   //! name of the font
        bool            isfontsize; //! is fontsize set?
        unsigned int    fontsize;   //! size of the font
        bool            isalignment;//! is alignment set?
        MMSALIGNMENT    alignment;  //! alignment of the text
        bool            iscolor;    //! is color set?
        DFBColor        color;      //! color of the text if the widget is not selected
        bool            isselcolor; //! is selcolor set?
        DFBColor        selcolor;   //! color of the text if the widget is selected
        bool            istext;     //! is text set?
        string          text;       //! text to draw

    public:
        MMSWidgetClass widgetClass; //! stores base widget attributes 

        ////////////////////////////////////////////////////////////////////////

        //! Constructor of class MMSLabelClass.
        MMSLabelClass();

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

        ////////////////////////////////////////////////////////////////////////

        //! Check if the fontname is set.
        bool isFontName();

        //! Set the fontname which is used to draw the text.
        /*!
        \param fontname  name of the font
        */
        void setFontName(string fontname);

        //! Mark the fontname as not set.
        void unsetFontName();

        //! Get the fontname which is used to draw the text.
        /*!
        \return name of the font
        */
        string getFontName();

        ////////////////////////////////////////////////////////////////////////

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

        ////////////////////////////////////////////////////////////////////////

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

        ////////////////////////////////////////////////////////////////////////

        //! Check if the color is set. This color will be used for the unselected text.
        bool isColor();

        //! Set the color which is used to draw the unselected text.
        /*!
        \param color  color for unselected text
        */
        void setColor(DFBColor color);

        //! Mark the color as not set.
        void unsetColor();

        //! Get the color which is used to draw the unselected text.
        /*!
        \return color for unselected text
        */
        DFBColor getColor();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the color is set. This color will be used for the selected text.
        bool isSelColor();

        //! Set the color which is used to draw the selected text.
        /*!
        \param selcolor  color for selected text
        */
        void setSelColor(DFBColor selcolor);

        //! Mark the color as not set.
        void unsetSelColor();

        //! Get the color which is used to draw the selected text.
        /*!
        \return color for selected text
        */
        DFBColor getSelColor();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the text is set.
        bool isText();

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
};

#endif /*MMSLABELCLASS_H_*/
