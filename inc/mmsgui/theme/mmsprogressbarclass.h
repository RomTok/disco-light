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

#ifndef MMSPROGRESSBARCLASS_H_
#define MMSPROGRESSBARCLASS_H_

#include "mmsgui/theme/mmswidgetclass.h"

//! describe attributes for MMSProgressBar which are additional to the MMSWidgetClass 
namespace MMSGUI_PROGRESSBAR_ATTR {

	#define MMSGUI_PROGRESSBAR_ATTR_ATTRDESC \
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
		{ "progress", TAFF_ATTRTYPE_UCHAR100 }
	
	#define MMSGUI_PROGRESSBAR_ATTR_IDS \
		MMSGUI_PROGRESSBAR_ATTR_IDS_color, \
		MMSGUI_PROGRESSBAR_ATTR_IDS_color_a, \
		MMSGUI_PROGRESSBAR_ATTR_IDS_color_r, \
		MMSGUI_PROGRESSBAR_ATTR_IDS_color_g, \
		MMSGUI_PROGRESSBAR_ATTR_IDS_color_b, \
		MMSGUI_PROGRESSBAR_ATTR_IDS_selcolor, \
		MMSGUI_PROGRESSBAR_ATTR_IDS_selcolor_a, \
		MMSGUI_PROGRESSBAR_ATTR_IDS_selcolor_r, \
		MMSGUI_PROGRESSBAR_ATTR_IDS_selcolor_g, \
		MMSGUI_PROGRESSBAR_ATTR_IDS_selcolor_b, \
		MMSGUI_PROGRESSBAR_ATTR_IDS_progress
	
	#define MMSGUI_PROGRESSBAR_ATTR_INIT { \
		MMSGUI_BASE_ATTR_ATTRDESC, \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WIDGET_ATTR_ATTRDESC, \
		MMSGUI_PROGRESSBAR_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BASE_ATTR_IDS,
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WIDGET_ATTR_IDS,
		MMSGUI_PROGRESSBAR_ATTR_IDS
	} ids;
}

extern TAFF_ATTRDESC MMSGUI_PROGRESSBAR_ATTR_I[];


//! A data access class for the MMSProgressBar widget class.
/*!
This class is the base for the MMSProgressBar widget class.
With this data store you have access to all changeable widget attributes. 
It is also one of the base classes for MMSThemeManager and MMSDialogManager
which are main features of the MMSGUI.
\note This class will be internally used by class MMSProgressBar.
\author Jens Schneider
*/
class MMSProgressBarClass {
    private:
    	//! name of the theme class
        string          className;
        
        //! is color set?
        bool            iscolor;
        
        //! color if the widget is not selected
        DFBColor        color;
        
        //! is selcolor set?
        bool            isselcolor;
        
        //! color if the widget is selected
        DFBColor        selcolor;
        
        //! is progress value set?
        bool            isprogress;
        
        //! progress value in percent
        unsigned int    progress;

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

        //! Constructor of class MMSProgressBarClass.
        MMSProgressBarClass();

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

        //! Check if the color is set. This color will be used to draw the unselected progress bar.
        bool isColor();

        //! Set the color which is used to draw the unselected progress bar.
        /*!
        \param color  color for unselected bar
        */
        void setColor(DFBColor color);

        //! Mark the color as not set.
        void unsetColor();

        //! Get the color which is used to draw the unselected progress bar.
        /*!
        \return color which is used for the unselected progress bar
        */
        DFBColor getColor();

        //! Check if the color is set. This color will be used for the selected progress bar.
        bool isSelColor();

        //! Set the color which is used to draw the selected progress bar.
        /*!
        \param selcolor  color for selected bar
        */
        void setSelColor(DFBColor selcolor);

        //! Mark the color as not set.
        void unsetSelColor();

        //! Get the color which is used to draw the selected progress bar.
        /*!
        \return color which is used for the selected progress bar
        */
        DFBColor getSelColor();

        //! Check if the progress value is set.
        bool isProgress();

        //! Set the progress.
        /*!
        \param progress  progress in percent (0-100)
        */
        void setProgress(unsigned int progress);

        //! Mark the progress as not set.
        void unsetProgress();

        //! Get the progress.
        /*!
        \return progress
        */
        unsigned int getProgress();

    /* friends */
    friend class MMSThemeManager;
    friend class MMSDialogManager;
};

#endif /*MMSPROGRESSBARCLASS_H_*/
