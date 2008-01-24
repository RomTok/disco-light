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

#ifndef MMSWIDGETCLASS_H_
#define MMSWIDGETCLASS_H_

#include "mmsgui/theme/mmsborderclass.h"

//! A data access class as base for all widgets.
/*!
This class is the base for all widget classes.
With this data store you have access to all changeable widget attributes
used for all widgets. 
\author Jens Schneider
*/
class MMSWidgetClass {
    private:
    	struct {
	    	bool            isbgcolor;          //! is bgcolor set?
	        DFBColor        bgcolor;            //! background color if the widget is not selected
	        bool            isselbgcolor;       //! is selbgcolor set?
	        DFBColor        selbgcolor;         //! background color if the widget is selected
	    	bool            isbgcolor_p;        //! is pressed bgcolor set?
	        DFBColor        bgcolor_p;          //! pressed background color if the widget is not selected
	        bool            isselbgcolor_p;     //! is pressed selbgcolor set?
	        DFBColor        selbgcolor_p;       //! pressed background color if the widget is selected
	        bool            isbgcolor_i;        //! is inactive bgcolor set?
	        DFBColor        bgcolor_i;          //! inactive background color if the widget is not selected
	        bool            isselbgcolor_i;     //! is inactive selbgcolor set?
	        DFBColor        selbgcolor_i;       //! inactive background color if the widget is selected
	        bool            isbgimagepath;      //! is bgimagepath set?
	        bool            isbgimagename;      //! is bgimagename set?
	        bool            isselbgimagepath;   //! is selbgimagepath set?
	        bool            isselbgimagename;   //! is selbgimagename set?
	        bool            isbgimagepath_p;    //! is pressed bgimagepath set?
	        bool            isbgimagename_p;    //! is pressed bgimagename set?
	        bool            isselbgimagepath_p; //! is pressed selbgimagepath set?
	        bool            isselbgimagename_p; //! is pressed selbgimagename set?
	        bool            isbgimagepath_i;    //! is inactive bgimagepath set?
	        bool            isbgimagename_i;    //! is inactive bgimagename set?
	        bool            isselbgimagepath_i; //! is inactive selbgimagepath set?
	        bool            isselbgimagename_i; //! is inactive selbgimagename set?
	        bool            ismargin;           //! is margin set?
	        unsigned int    margin;             //! margin in pixel 
	        bool            isfocusable;        //! is the focusable flag set?
	        bool            focusable;          //! widget can get the focus true/false
	        bool            isselectable;       //! is the selectable flag set?
	        bool            selectable;         //! widget can be selected true/false
	        bool            isuparrow;          //! is the uparrow set?
	        bool            isdownarrow;        //! is the downarrow set?
	        bool            isleftarrow;        //! is the leftarrow set?
	        bool            isrightarrow;       //! is the rightarrow set?
	        bool            isdata;             //! is the data value set?
	        bool            isnavigateup;       //! is the navigateup set?
	        bool            isnavigatedown;     //! is the navigatedown set?
	        bool            isnavigateleft;     //! is the navigateleft set?
	        bool            isnavigateright;    //! is the navigateright set?
	        bool            isvslider;          //! is the vslider set?
	        bool            ishslider;          //! is the hslider set?
	        bool            isimagesondemand;   //! is images on demand flag set?
	        bool            imagesondemand;     //! use images on demand (true/false)
	        bool            isblend;            //! is blend set?
	        unsigned int    blend;              //! blend 0..255, default 0 
	        bool            isblendfactor;      //! is blend factor set?
	        double          blendfactor;        //! blend factor 0.0.., default 0.0 
    	} id;

    	struct {
            string          *bgimagepath;       //! path to the background image if the widget is not selected
            string          *bgimagename;       //! background image filename if the widget is not selected
            string          *selbgimagepath;    //! path to the background image if the widget is selected
            string          *selbgimagename;    //! background image filename if the widget is selected
            string          *bgimagepath_p;     //! path to the pressed background image if the widget is not selected
            string          *bgimagename_p;     //! pressed background image filename if the widget is not selected
            string          *selbgimagepath_p;  //! path to the pressed background image if the widget is selected
            string          *selbgimagename_p;  //! pressed background image filename if the widget is selected
            string          *bgimagepath_i;     //! path to the inactive background image if the widget is not selected
            string          *bgimagename_i;     //! inactive background image filename if the widget is not selected
            string          *selbgimagepath_i;  //! path to the inactive background image if the widget is selected
            string          *selbgimagename_i;  //! inactive background image filename if the widget is selected
            string          *uparrow;           //! the name of the widget which represents the scroll up arrow
            string          *downarrow;         //! the name of the widget which represents the scroll down arrow
            string          *leftarrow;         //! the name of the widget which represents the scroll left arrow
            string          *rightarrow;        //! the name of the widget which represents the scroll right arrow
            string          *data;              //! any string which can store additional information (will not displayed)
            string          *navigateup;        //! the name of the widget to which should navigate up
            string          *navigatedown;      //! the name of the widget to which should navigate down
            string          *navigateleft;      //! the name of the widget to which should navigate left
            string          *navigateright;     //! the name of the widget to which should navigate right
            string          *vslider;           //! the name of the widget which represents the vertical slider
            string          *hslider;           //! the name of the widget which represents the horizontal slider
    	} ed;

    	/* init routines */
        void initBgColor();
        void initSelBgColor();
        void initBgColor_p();
        void initSelBgColor_p();
        void initBgColor_i();
        void initSelBgColor_i();

        void initBgImagePath();
        void initBgImageName();
        void initSelBgImagePath();
        void initSelBgImageName();
        void initBgImagePath_p();
        void initBgImageName_p();
        void initSelBgImagePath_p();
        void initSelBgImageName_p();
        void initBgImagePath_i();
        void initBgImageName_i();
        void initSelBgImagePath_i();
        void initSelBgImageName_i();

        void initMargin();
        void initFocusable();
        void initSelectable();

        void initUpArrow();
        void initDownArrow();
        void initLeftArrow();
        void initRightArrow();

        void initData();

        void initNavigateUp();
        void initNavigateDown();
        void initNavigateLeft();
        void initNavigateRight();
        
        void initVSlider();
        void initHSlider();

        void initImagesOnDemand();

        void initBlend();
        void initBlendFactor();


        /* free routines */
        void freeBgColor();
        void freeSelBgColor();
        void freeBgColor_p();
        void freeSelBgColor_p();
        void freeBgColor_i();
        void freeSelBgColor_i();

        void freeBgImagePath();
        void freeBgImageName();
        void freeSelBgImagePath();
        void freeSelBgImageName();
        void freeBgImagePath_p();
        void freeBgImageName_p();
        void freeSelBgImagePath_p();
        void freeSelBgImageName_p();
        void freeBgImagePath_i();
        void freeBgImageName_i();
        void freeSelBgImagePath_i();
        void freeSelBgImageName_i();

        void freeMargin();
        void freeFocusable();
        void freeSelectable();

        void freeUpArrow();
        void freeDownArrow();
        void freeLeftArrow();
        void freeRightArrow();

        void freeData();

        void freeNavigateUp();
        void freeNavigateDown();
        void freeNavigateLeft();
        void freeNavigateRight();
        
        void freeVSlider();
        void freeHSlider();

        void freeImagesOnDemand();

        void freeBlend();
        void freeBlendFactor();

    public:
        MMSBorderClass border;           	//! stores base border attributes

        ////////////////////////////////////////////////////////////////////////

        //! Constructor of class MMSWidgetClass.
        MMSWidgetClass();

        //! Destructor of class MMSWidgetClass.
        ~MMSWidgetClass();

        //! operator=
        MMSWidgetClass &operator=(const MMSWidgetClass &c); 
        
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

        //! Check if the background color is set. This color will be used for the unselected widget.
        bool isBgColor();

        //! Mark the bgcolor as not set.
        void unsetBgColor();

        //! Set the background color which is used to draw the unselected widget.
        /*!
        \param bgcolor  color for unselected background
        */
        void setBgColor(const DFBColor &bgcolor);

        //! Get the background color which is used to draw the unselected widget.
        /*!
        \param bgcolor  background color
        \return true if set
        */
        bool getBgColor(DFBColor &bgcolor);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the background color is set. This color will be used for the selected widget.
        bool isSelBgColor();

        //! Mark the selbgcolor as not set.
        void unsetSelBgColor();

        //! Set the background color which is used to draw the selected widget.
        /*!
        \param selbgcolor  color for selected background
        */
        void setSelBgColor(const DFBColor &selbgcolor);

        //! Get the background color which is used to draw the selected widget.
        /*!
        \param selbgcolor  background color
        \return true if set
        */
        bool getSelBgColor(DFBColor &selbgcolor);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the pressed background color is set. This color will be used for the unselected widget.
        bool isBgColor_p();

        //! Mark the pressed bgcolor as not set.
        void unsetBgColor_p();

        //! Set the pressed background color which is used to draw the unselected widget.
        /*!
        \param bgcolor_p  pressed background color
        */
        void setBgColor_p(const DFBColor &bgcolor_p);

        //! Get the pressed background color which is used to draw the unselected widget.
        /*!
        \param bgcolor_p  pressed background color
        \return true if set
        */
        bool getBgColor_p(DFBColor &bgcolor_p);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the pressed background color is set. This color will be used for the selected widget.
        bool isSelBgColor_p();

        //! Mark the pressed selbgcolor as not set.
        void unsetSelBgColor_p();

        //! Set the pressed background color which is used to draw the selected widget.
        /*!
        \param selbgcolor_p  pressed color for selected background
        */
        void setSelBgColor_p(const DFBColor &selbgcolor_p);

        //! Get the pressed background color which is used to draw the selected widget.
        /*!
        \param selbgcolor_p  pressed background color
        \return true if set
        */
        bool getSelBgColor_p(DFBColor &selbgcolor_p);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the inactive background color is set. This color will be used for the unselected widget.
        bool isBgColor_i();

        //! Mark the inactive bgcolor as not set.
        void unsetBgColor_i();

        //! Set the inactive background color which is used to draw the unselected widget.
        /*!
        \param bgcolor_i  color for inactive unselected background
        */
        void setBgColor_i(const DFBColor &bgcolor_i);

        //! Get the inactive background color which is used to draw the unselected widget.
        /*!
        \param bgcolor_i  inactive background color
        \return true if set
        */
        bool getBgColor_i(DFBColor &bgcolor_i);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the inactive background color is set. This color will be used for the selected widget.
        bool isSelBgColor_i();

        //! Mark the inactive selbgcolor as not set.
        void unsetSelBgColor_i();

        //! Set the inactive background color which is used to draw the selected widget.
        /*!
        \param selbgcolor_i  color for inactive selected background
        */
        void setSelBgColor_i(const DFBColor &selbgcolor_i);

        //! Get the inactive background color which is used to draw the selected widget.
        /*!
        \param selbgcolor_i  inactive background color
        \return true if set
        */
        bool getSelBgColor_i(DFBColor &selbgcolor_i);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the imagepath for background is set. This path will be used for the unselected widget.
        bool isBgImagePath();
 
        //! Mark the bgimagepath as not set.
        void unsetBgImagePath();

        //! Set the imagepath for background which is used to draw the unselected widget.
        /*!
        \param bgimagepath  path to unselected background image
        */
        void setBgImagePath(const string &bgimagepath);

        //! Get the imagepath for background which is used to draw the unselected widget.
        /*!
        \param bgimagepath  path to unselected background image
        \return true if set
        */
        bool getBgImagePath(string &bgimagepath);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the imagename for background is set. This name will be used for the unselected widget.
        bool isBgImageName();
 
        //! Mark the bgimagename as not set.
        void unsetBgImageName();

        //! Set the imagename for background which is used to draw the unselected widget.
        /*!
        \param bgimagename  name of the unselected background image
        */
        void setBgImageName(const string &bgimagename);

        //! Get the imagename for background which is used to draw the unselected widget.
        /*!
        \param bgimagename  name of the unselected background image
        \return true if set
        */
        bool getBgImageName(string &bgimagename);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the selimagepath for background is set. This path will be used for the selected widget.
        bool isSelBgImagePath();
 
        //! Mark the selbgimagepath as not set.
        void unsetSelBgImagePath();

        //! Set the selimagepath for background which is used to draw the selected widget.
        /*!
        \param selbgimagepath  path to selected background image
        */
        void setSelBgImagePath(const string &selbgimagepath);

        //! Get the selimagepath for background which is used to draw the selected widget.
        /*!
        \param selbgimagepath  path to the selected background image
        \return true if set
        */
        bool getSelBgImagePath(string &selbgimagepath);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the selimagename for background is set. This name will be used for the selected widget.
        bool isSelBgImageName();
 
        //! Mark the selbgimagename as not set.
        void unsetSelBgImageName();

        //! Set the selimagename for background which is used to draw the selected widget.
        /*!
        \param selbgimagename  name of the selected background image
        */
        void setSelBgImageName(const string &selbgimagename);

        //! Get the selimagename for background which is used to draw the selected widget.
        /*!
        \param selbgimagename  name of the selected background image
        \return true if set
        */
        bool getSelBgImageName(string &selbgimagename);

        ////////////////////////////////////////////////////////////////////////


        
        
        
        //! Check if the pressed imagepath for background is set. This path will be used for the unselected widget.
        bool isBgImagePath_p();
 
        //! Mark the pressed bgimagepath as not set.
        void unsetBgImagePath_p();

        //! Set the pressed imagepath for background which is used to draw the unselected widget.
        /*!
        \param bgimagepath_p  path to pressed & unselected background image
        */
        void setBgImagePath_p(const string &bgimagepath_p);

        //! Get the pressed imagepath for background which is used to draw the unselected widget.
        /*!
        \param bgimagepath_p  path to pressed & unselected background image
        \return true if set
        */
        bool getBgImagePath_p(string &bgimagepath_p);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the pressed imagename for background is set. This name will be used for the unselected widget.
        bool isBgImageName_p();
 
        //! Mark the pressed bgimagename as not set.
        void unsetBgImageName_p();

        //! Set the pressed imagename for background which is used to draw the unselected widget.
        /*!
        \param bgimagename_p  name of the pressed & unselected background image
        */
        void setBgImageName_p(const string &bgimagename_p);

        //! Get the pressed imagename for background which is used to draw the unselected widget.
        /*!
        \param bgimagename_p  name of the pressed & unselected background image
        \return true if set
        */
        bool getBgImageName_p(string &bgimagename_p);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the pressed selimagepath for background is set. This path will be used for the selected widget.
        bool isSelBgImagePath_p();
 
        //! Mark the pressed selbgimagepath as not set.
        void unsetSelBgImagePath_p();

        //! Set the pressed selimagepath for background which is used to draw the selected widget.
        /*!
        \param selbgimagepath_p  path to pressed & selected background image
        */
        void setSelBgImagePath_p(const string &selbgimagepath_p);

        //! Get the pressed selimagepath for background which is used to draw the selected widget.
        /*!
        \param selbgimagepath_p  path to the pressed & selected background image
        \return true if set
        */
        bool getSelBgImagePath_p(string &selbgimagepath_p);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the pressed selimagename for background is set. This name will be used for the selected widget.
        bool isSelBgImageName_p();
 
        //! Mark the pressed selbgimagename as not set.
        void unsetSelBgImageName_p();

        //! Set the pressed selimagename for background which is used to draw the selected widget.
        /*!
        \param selbgimagename_p  name of the pressed & selected background image
        */
        void setSelBgImageName_p(const string &selbgimagename_p);

        //! Get the pressed selimagename for background which is used to draw the selected widget.
        /*!
        \param selbgimagename_p  name of the pressed & selected background image
        \return true if set
        */
        bool getSelBgImageName_p(string &selbgimagename_p);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the inactive imagepath for background is set. This path will be used for the unselected widget.
        bool isBgImagePath_i();
 
        //! Mark the inactive bgimagepath as not set.
        void unsetBgImagePath_i();

        //! Set the imagepath for inactive background which is used to draw the unselected widget.
        /*!
        \param bgimagepath_i  path to inactive unselected background image
        */
        void setBgImagePath_i(const string &bgimagepath_i);

        //! Get the imagepath for inactive background which is used to draw the unselected widget.
        /*!
        \param bgimagepath_i  path to the inactive unselected background image
        \return true if set
        */
        bool getBgImagePath_i(string &bgimagepath_i);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the imagename for inactive background is set. This name will be used for the unselected widget.
        bool isBgImageName_i();
 
        //! Mark the inactive bgimagename as not set.
        void unsetBgImageName_i();

        //! Set the imagename for inactive background which is used to draw the unselected widget.
        /*!
        \param bgimagename_i  name of the inactive unselected background image
        */
        void setBgImageName_i(const string &bgimagename_i);

        //! Get the imagename for inactive background which is used to draw the unselected widget.
        /*!
        \param bgimagename_i  name of the inactive unselected background image
        \return true if set
        */
        bool getBgImageName_i(string &bgimagename_i);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the selimagepath for inactive background is set. This path will be used for the selected widget.
        bool isSelBgImagePath_i();
 
        //! Mark the inactive selbgimagepath as not set.
        void unsetSelBgImagePath_i();

        //! Set the selimagepath for inactive background which is used to draw the selected widget.
        /*!
        \param selbgimagepath_i  path to inactive selected background image
        */
        void setSelBgImagePath_i(const string &selbgimagepath_i);

        //! Get the selimagepath for inactive background which is used to draw the selected widget.
        /*!
        \param selbgimagepath_i  path to the inactive selected background image
        \return true if set
        */
        bool getSelBgImagePath_i(string &selbgimagepath_i);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the selimagename for inactive background is set. This name will be used for the selected widget.
        bool isSelBgImageName_i();
 
        //! Mark the inactive selbgimagename as not set.
        void unsetSelBgImageName_i();

        //! Set the selimagename for inactive background which is used to draw the selected widget.
        /*!
        \param selbgimagename  name of the inactive selected background image
        */
        void setSelBgImageName_i(const string &selbgimagename_i);

        //! Get the selimagename for inactive background which is used to draw the selected widget.
        /*!
        \param selbgimagename_i  name of the inactive selected background image
        \return true if set
        */
        bool getSelBgImageName_i(string &selbgimagename_i);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the margin is set.
        bool isMargin();

        //! Mark the margin as not set.
        void unsetMargin();

        //! Set the margin.
        /*!
        \param margin  margin in pixel
        */
        void setMargin(unsigned int margin);

        //! Get the margin.
        /*!
        \param margin  margin
        \return true if set
        */
        bool getMargin(unsigned int &margin);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the focusable flag is set.
        bool isFocusable();

        //! Mark the focusable flag as not set.
        void unsetFocusable();

        //! Set the focusable flag.
        /*!
        \param focusable  the widget can get the focus if set to true
        \note There is a difference between focused and selected. Only one widget
              can get the focus at the same time. The focused widget gets the keyboard input.
              A focused widget is also selected.  
        */
        void setFocusable(bool focusable);

        //! Get the focusable flag.
        /*!
        \param focusable  focusable true or false
        \return true if set
        */
        bool getFocusable(bool &focusable);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the selectable flag is set.
        bool isSelectable();

        //! Mark the selectable flag as not set.
        void unsetSelectable();

        //! Set the selectable flag.
        /*!
        \param selectable  the widget can be selected if set to true
        \note There is a difference between focused and selected. Only one widget
              can get the focus at the same time. But all other widgets can be switched
              between selected and unselected independently.    
        */
        void setSelectable(bool selectable);

        //! Get the selectable flag.
        /*!
        \param selectable  selectable true or false
        \return true if set
        */
        bool getSelectable(bool &selectable);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the uparrow is set.
        bool isUpArrow();

        //! Mark the uparrow as not set.
        void unsetUpArrow();

        //! Set the uparrow.
        /*!
        \param uparrow  the name of the widget which represents the scroll up arrow
        */
        void setUpArrow(const string &uparrow);

        //! Get the uparrow.
        /*!
        \param uparrow  name of the up arrow widget
        \return true if set
        */
        bool getUpArrow(string &uparrow);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the downarrow is set.
        bool isDownArrow();

		//! Mark the downarrow as not set.
		void unsetDownArrow();

        //! Set the downarrow.
        /*!
        \param downarrow  the name of the widget which represents the scroll down arrow
        */
        void setDownArrow(const string &downarrow);

        //! Get the downarrow.
        /*!
        \param downarrow  name of the down arrow widget
        \return true if set
        */
        bool getDownArrow(string &downarrow);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the leftarrow is set.
        bool isLeftArrow();

        //! Mark the leftarrow as not set.
        void unsetLeftArrow();

        //! Set the leftarrow.
        /*!
        \param leftarrow  the name of the widget which represents the scroll left arrow
        */
        void setLeftArrow(const string &leftarrow);

        //! Get the leftarrow.
        /*!
        \param leftarrow  name of the left arrow widget
        \return true if set
        */
        bool getLeftArrow(string &leftarrow);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the rightarrow is set.
        bool isRightArrow();

        //! Mark the rightarrow as not set.
        void unsetRightArrow();

        //! Set the rightarrow.
        /*!
        \param rightarrow  the name of the widget which represents the scroll right arrow
        */
        void setRightArrow(const string &rightarrow);


        //! Get the rightarrow.
        /*!
        \param rightarrow  name of the right arrow widget
        \return true if set
        */
        bool getRightArrow(string &rightarrow);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the data is set.
        bool isData();

        //! Mark the data as not set.
        void unsetData();

        //! Set the additional data value.
        /*!
        \param data  any string which can store additional information (will not displayed)
        */
        void setData(const string &data);

        //! Get the data.
        /*!
        \param data  additional data string
        \return true if set
        */
        bool getData(string &data);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the navigateup is set.
        bool isNavigateUp();

        //! Mark the navigateup as not set.
        void unsetNavigateUp();

        //! Set the navigateup widget.
        /*!
        \param navigateup  the name of the widget to which should navigate up
        */
        void setNavigateUp(const string &navigateup);

        //! Get the navigateup widget.
        /*!
        \param navigateup name of the navigate up widget
        \return true if set
        */
        bool getNavigateUp(string &navigateup);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the navigatedown is set.
        bool isNavigateDown();

        //! Mark the navigatedown as not set.
        void unsetNavigateDown();

        //! Set the navigatedown widget.
        /*!
        \param navigatedown  the name of the widget to which should navigate down
        */
        void setNavigateDown(const string &navigatedown);

        //! Get the navigatedown widget.
        /*!
        \param navigatedown  name of the navigate down widget
        \return true if set
        */
        bool getNavigateDown(string &navigatedown);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the navigateleft is set.
        bool isNavigateLeft();

        //! Mark the navigateleft as not set.
        void unsetNavigateLeft();

        //! Set the navigateleft widget.
        /*!
        \param navigateleft  the name of the widget to which should navigate left
        */
        void setNavigateLeft(const string &navigateleft);

        //! Get the navigateleft widget.
        /*!
        \param navigateleft  name of the navigate left widget
        \return true if set
        */
        bool getNavigateLeft(string &navigateleft);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the navigateright is set.
        bool isNavigateRight();

        //! Mark the navigateright as not set.
        void unsetNavigateRight();

        //! Set the navigateright.
        /*!
        \param navigateright  the name of the widget to which should navigate right
        */
        void setNavigateRight(const string &navigateright);

        //! Get the navigateright widget.
        /*!
        \param navigateright  name of the navigate right widget
        \return true if set
        */
        bool getNavigateRight(string &navigateright);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the vslider is set.
        bool isVSlider();

        //! Mark the vslider as not set.
        void unsetVSlider();

        //! Set the vslider.
        /*!
        \param vslider  the name of the widget which represents the vertical slider
        */
        void setVSlider(const string &vslider);

        //! Get the vslider.
        /*!
        \param vslider  name of the vslider widget
        \return true if set
        */
        bool getVSlider(string &vslider);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the hslider is set.
        bool isHSlider();

        //! Mark the hslider as not set.
        void unsetHSlider();

        //! Set the hslider.
        /*!
        \param hslider  the name of the widget which represents the horizontal slider
        */
        void setHSlider(const string &hslider);

        //! Get the hslider.
        /*!
        \param hslider  name of the hslider widget
        \return true if set
        */
        bool getHSlider(string &hslider);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the images on demand is set.
        bool isImagesOnDemand();
 
        //! Mark the images on demand flag as not set.
        void unsetImagesOnDemand();

        //! Set the images on demand flag.
        /*!
        \param imagesondemand  use imagesondemand true/false
        */
        void setImagesOnDemand(bool imagesondemand);

        //! Get the images on demand flag.
        /*!
        \return images on demand flag
        \return true if set
        */
        bool getImagesOnDemand(bool &imagesondemand);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the blend is set.
        bool isBlend();

        //! Mark the blend as not set.
        void unsetBlend();

        //! Set the blend.
        /*!
        \param blend  blend with values 0..255
        */
        void setBlend(unsigned int blend);

        //! Get the blend.
        /*!
        \param blend  blend
        \return true if set
        */
        bool getBlend(unsigned int &blend);

        ////////////////////////////////////////////////////////////////////////

        //! Check if the blend factor is set.
        bool isBlendFactor();

        //! Mark the blend factor as not set.
        void unsetBlendFactor();

        //! Set the blend factor.
        /*!
        \param blendfactor  blend factor with values 0.0..
        */
        void setBlendFactor(double blendfactor);

        //! Get the blend factor.
        /*!
        \param blendfactor  blend factor
        \return true if set
        */
        bool getBlendFactor(double &blendfactor);

};

#endif /*MMSWIDGETCLASS_H_*/
