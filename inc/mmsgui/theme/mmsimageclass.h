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

#ifndef MMSIMAGECLASS_H_
#define MMSIMAGECLASS_H_

#include "mmsgui/theme/mmswidgetclass.h"

//! A data access class for the image widget.
/*!
This class is the base for the MMSImage widget class.
With this data store you have access to all changeable widget attributes. 
It is also one of the base classes for MMSThemeManager and MMSDialogManager
which are main features of the MMSGUI.
\author Jens Schneider
*/
class MMSImageClass {
    private:
        string       className;     //! name of the theme class
        bool         isimagepath;   //! is imagepath set?
        string       imagepath;     //! path to the image if the widget is not selected
        bool         isimagename;   //! is imagename set?
        string       imagename;     //! image filename if the widget is not selected
        bool         isselimagepath;//! is selimagepath set?
        string       selimagepath;  //! path to the image if the widget is selected
        bool         isselimagename;//! is selimagename set?
        string       selimagename;  //! image filename if the widget is selected
        bool         isimagepath_p;     //! is pressed imagepath set?
        string       imagepath_p;       //! path to the pressed image if the widget is not selected
        bool         isimagename_p;     //! is pressed imagename set?
        string       imagename_p;       //! pressed image filename if the widget is not selected
        bool         isselimagepath_p;  //! is pressed selimagepath set?
        string       selimagepath_p;    //! path to the pressed image if the widget is selected
        bool         isselimagename_p;  //! is pressed selimagename set?
        string       selimagename_p;    //! pressed image filename if the widget is selected
        bool         isimagepath_i;     //! is inactive imagepath set?
        string       imagepath_i;       //! path to the inactive image if the widget is not selected
        bool         isimagename_i;     //! is inactive imagename set?
        string       imagename_i;       //! inactive image filename if the widget is not selected
        bool         isselimagepath_i;  //! is inactive selimagepath set?
        string       selimagepath_i;    //! path to the inactive image if the widget is selected
        bool         isselimagename_i;  //! is inactive selimagename set?
        string       selimagename_i;    //! inactive image filename if the widget is selected
        bool         isuseratio;        //! is use aspect ratio flag set?
        bool         useratio;          //! use aspect ratio of the images (true/false)
        bool         isfitwidth;
        bool         fitwidth;
        bool         isfitheight;
        bool         fitheight;
        bool         isalignment;
        MMSALIGNMENT alignment;

    public:
        MMSWidgetClass widgetClass; //! stores base widget attributes 

        ////////////////////////////////////////////////////////////////////////

        //! Constructor of class MMSImageClass.
        MMSImageClass();

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

        //! Check if the imagepath is set. This path will be used for the unselected widget.
        bool isImagePath();
 
        //! Set the imagepath which is used to draw the unselected widget.
        /*!
        \param imagepath  path to unselected image
        */
        void setImagePath(string imagepath);

        //! Mark the imagepath as not set.
        void unsetImagePath();

        //! Get the imagepath which is used to draw the unselected widget.
        /*!
        \return path to the unselected image
        */
        string getImagePath();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the imagename is set. This name will be used for the unselected widget.
        bool isImageName();
 
        //! Set the imagename which is used to draw the unselected widget.
        /*!
        \param imagename  name of the unselected image
        */
        void setImageName(string imagename);

        //! Mark the imagename as not set.
        void unsetImageName();

        //! Get the imagename which is used to draw the unselected widget.
        /*!
        \return name of the unselected image
        */
        string getImageName();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the selimagepath is set. This path will be used for the selected widget.
        bool isSelImagePath();
 
        //! Set the selimagepath which is used to draw the selected widget.
        /*!
        \param selimagepath  path to selected image
        */
        void setSelImagePath(string selimagepath);

        //! Mark the selimagepath as not set.
        void unsetSelImagePath();

        //! Get the selimagepath which is used to draw the selected widget.
        /*!
        \return path to the selected image
        */
        string getSelImagePath();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the selimagename is set. This name will be used for the selected widget.
        bool isSelImageName();
 
        //! Set the selimagename which is used to draw the selected widget.
        /*!
        \param selimagename  name of the selected image
        */
        void setSelImageName(string selimagename);

        //! Mark the selimagename as not set.
        void unsetSelImageName();

        //! Get the selimagename which is used to draw the selected widget.
        /*!
        \return name of the selected image
        */
        string getSelImageName();


        ////////////////////////////////////////////////////////////////////////

        //! Check if the pressed imagepath is set. This path will be used for the unselected widget.
        bool isImagePath_p();
 
        //! Set the pressed imagepath which is used to draw the unselected widget.
        /*!
        \param imagepath_p  path to pressed unselected image
        */
        void setImagePath_p(string imagepath_p);

        //! Mark the pressed imagepath as not set.
        void unsetImagePath_p();

        //! Get the pressed imagepath which is used to draw the unselected widget.
        /*!
        \return path to the pressed unselected image
        */
        string getImagePath_p();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the pressed imagename is set. This name will be used for the unselected widget.
        bool isImageName_p();
 
        //! Set the pressed imagename which is used to draw the unselected widget.
        /*!
        \param imagename_p  name of the pressed unselected image
        */
        void setImageName_p(string imagename_p);

        //! Mark the pressed imagename as not set.
        void unsetImageName_p();

        //! Get the pressed imagename which is used to draw the unselected widget.
        /*!
        \return name of the pressed unselected image
        */
        string getImageName_p();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the pressed selimagepath is set. This path will be used for the selected widget.
        bool isSelImagePath_p();
 
        //! Set the pressed selimagepath which is used to draw the selected widget.
        /*!
        \param selimagepath_p  path to pressed selected image
        */
        void setSelImagePath_p(string selimagepath_p);

        //! Mark the pressed selimagepath as not set.
        void unsetSelImagePath_p();

        //! Get the pressed selimagepath which is used to draw the selected widget.
        /*!
        \return path to the pressed selected image
        */
        string getSelImagePath_p();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the pressed selimagename is set. This name will be used for the selected widget.
        bool isSelImageName_p();
 
        //! Set the pressed selimagename which is used to draw the selected widget.
        /*!
        \param selimagename_p  name of the pressed selected image
        */
        void setSelImageName_p(string selimagename_p);

        //! Mark the pressed selimagename as not set.
        void unsetSelImageName_p();

        //! Get the pressed selimagename which is used to draw the selected widget.
        /*!
        \return name of the pressed selected image
        */
        string getSelImageName_p();

        
        ////////////////////////////////////////////////////////////////////////

        //! Check if the inactive imagepath is set. This path will be used for the unselected widget.
        bool isImagePath_i();
 
        //! Set the inactive imagepath which is used to draw the unselected widget.
        /*!
        \param imagepath_i  path to inactive unselected image
        */
        void setImagePath_i(string imagepath_i);

        //! Mark the inactive imagepath as not set.
        void unsetImagePath_i();

        //! Get the inactive imagepath which is used to draw the unselected widget.
        /*!
        \return path to the inactive unselected image
        */
        string getImagePath_i();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the inactive imagename is set. This name will be used for the unselected widget.
        bool isImageName_i();
 
        //! Set the inactive imagename which is used to draw the unselected widget.
        /*!
        \param imagename_i  name of the inactive unselected image
        */
        void setImageName_i(string imagename_i);

        //! Mark the inactive imagename as not set.
        void unsetImageName_i();

        //! Get the inactive imagename which is used to draw the unselected widget.
        /*!
        \return name of the inactive unselected image
        */
        string getImageName_i();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the inactive selimagepath is set. This path will be used for the selected widget.
        bool isSelImagePath_i();
 
        //! Set the inactive selimagepath which is used to draw the selected widget.
        /*!
        \param selimagepath_i  path to inactive selected image
        */
        void setSelImagePath_i(string selimagepath_i);

        //! Mark the inactive selimagepath as not set.
        void unsetSelImagePath_i();

        //! Get the inactive selimagepath which is used to draw the selected widget.
        /*!
        \return path to the inactive selected image
        */
        string getSelImagePath_i();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the inactive selimagename is set. This name will be used for the selected widget.
        bool isSelImageName_i();
 
        //! Set the inactive selimagename which is used to draw the selected widget.
        /*!
        \param selimagename_i  name of the inactive selected image
        */
        void setSelImageName_i(string selimagename_i);

        //! Mark the inactive selimagename as not set.
        void unsetSelImageName_i();

        //! Get the inactive selimagename which is used to draw the selected widget.
        /*!
        \return name of the inactive selected image
        */
        string getSelImageName_i();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the use acpect ratio is set.
        bool isUseRatio();
 
        //! Set the use aspect ratio flag.
        /*!
        \param useratio  use aspect ratio true/false
        */
        void setUseRatio(bool useratio);

        //! Mark the use aspect ratio as not set.
        void unsetUseRatio();

        //! Get the use aspect ratio flag.
        /*!
        \return use aspect ratio flag
        */
        bool getUseRatio();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the fitwidth flag is set.
        bool isFitWidth();
 
        //! Set the fitwidth flag.
        /*!
        \param fitwidth  fit width true/false
        */
        void setFitWidth(bool fitwidth);

        //! Mark the fitwidth as not set.
        void unsetFitWidth();

        //! Get the fitwidth flag.
        /*!
        \return fitwidth flag
        */
        bool getFitWidth();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the fitheight flag is set.
        bool isFitHeight();
 
        //! Set the fitheight flag.
        /*!
        \param fitheight  fit height true/false
        */
        void setFitHeight(bool fitheight);

        //! Mark the fitheight as not set.
        void unsetFitHeight();

        //! Get the fitheight flag.
        /*!
        \return fitheight flag
        */
        bool getFitHeight();

        ////////////////////////////////////////////////////////////////////////

        bool isAlignment();
        void setAlignment(MMSALIGNMENT alignment);
        void unsetAlignment();
        MMSALIGNMENT getAlignment();
};

#endif /*MMSIMAGECLASS_H_*/
