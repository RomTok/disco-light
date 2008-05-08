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

#ifndef MMSMENUCLASS_H_
#define MMSMENUCLASS_H_

#include "mmsgui/theme/mmswidgetclass.h"

//describe attributes
namespace MMSGUI_MENU_ATTR {

	#define MMSGUI_MENU_ATTR_ATTRDESC \
		{ "item_width", TAFF_ATTRTYPE_STRING }, \
		{ "item_height", TAFF_ATTRTYPE_STRING }, \
		{ "item_hmargin", TAFF_ATTRTYPE_UCHAR }, \
		{ "item_vmargin", TAFF_ATTRTYPE_UCHAR }, \
		{ "cols", TAFF_ATTRTYPE_INT }, \
		{ "dim_items", TAFF_ATTRTYPE_UCHAR }, \
		{ "fixed_pos", TAFF_ATTRTYPE_INT }, \
		{ "hloop", TAFF_ATTRTYPE_BOOL }, \
		{ "vloop", TAFF_ATTRTYPE_BOOL }, \
		{ "trans_items", TAFF_ATTRTYPE_UCHAR }, \
		{ "dim_top", TAFF_ATTRTYPE_UCHAR }, \
		{ "dim_bottom", TAFF_ATTRTYPE_UCHAR }, \
		{ "dim_left", TAFF_ATTRTYPE_UCHAR }, \
		{ "dim_right", TAFF_ATTRTYPE_UCHAR }, \
		{ "trans_top", TAFF_ATTRTYPE_UCHAR }, \
		{ "trans_bottom", TAFF_ATTRTYPE_UCHAR }, \
		{ "trans_left", TAFF_ATTRTYPE_UCHAR }, \
		{ "trans_right", TAFF_ATTRTYPE_UCHAR }, \
		{ "zoomsel_width", TAFF_ATTRTYPE_STRING }, \
		{ "zoomsel_height", TAFF_ATTRTYPE_STRING }, \
		{ "zoomsel_shiftx", TAFF_ATTRTYPE_STRING }, \
		{ "zoomsel_shifty", TAFF_ATTRTYPE_STRING }, \
		{ "smooth_scrolling", TAFF_ATTRTYPE_STRING }

	#define MMSGUI_MENU_ATTR_IDS \
		MMSGUI_MENU_ATTR_IDS_item_width, \
		MMSGUI_MENU_ATTR_IDS_item_height, \
		MMSGUI_MENU_ATTR_IDS_item_hmargin, \
		MMSGUI_MENU_ATTR_IDS_item_vmargin, \
		MMSGUI_MENU_ATTR_IDS_cols, \
		MMSGUI_MENU_ATTR_IDS_dim_items, \
		MMSGUI_MENU_ATTR_IDS_fixed_pos, \
		MMSGUI_MENU_ATTR_IDS_hloop, \
		MMSGUI_MENU_ATTR_IDS_vloop, \
		MMSGUI_MENU_ATTR_IDS_trans_items, \
		MMSGUI_MENU_ATTR_IDS_dim_top, \
		MMSGUI_MENU_ATTR_IDS_dim_bottom, \
		MMSGUI_MENU_ATTR_IDS_dim_left, \
		MMSGUI_MENU_ATTR_IDS_dim_right, \
		MMSGUI_MENU_ATTR_IDS_trans_top, \
		MMSGUI_MENU_ATTR_IDS_trans_bottom, \
		MMSGUI_MENU_ATTR_IDS_trans_left, \
		MMSGUI_MENU_ATTR_IDS_trans_right, \
		MMSGUI_MENU_ATTR_IDS_zoomsel_width, \
		MMSGUI_MENU_ATTR_IDS_zoomsel_height, \
		MMSGUI_MENU_ATTR_IDS_zoomsel_shiftx, \
		MMSGUI_MENU_ATTR_IDS_zoomsel_shifty, \
		MMSGUI_MENU_ATTR_IDS_smooth_scrolling
	
	#define MMSGUI_MENU_ATTR_INIT { \
		MMSGUI_BASE_ATTR_ATTRDESC, \
		MMSGUI_BORDER_ATTR_ATTRDESC, \
		MMSGUI_WIDGET_ATTR_ATTRDESC, \
		MMSGUI_MENU_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BASE_ATTR_IDS,
		MMSGUI_BORDER_ATTR_IDS,
		MMSGUI_WIDGET_ATTR_IDS,
		MMSGUI_MENU_ATTR_IDS
	} ids;
}

extern TAFF_ATTRDESC MMSGUI_MENU_ATTR_I[];


//! A data access class for the menu widget.
/*!
This class is the base for the MMSMenu widget class.
With this data store you have access to all changeable widget attributes. 
It is also one of the base classes for MMSThemeManager and MMSDialogManager
which are main features of the MMSGUI.
\author Jens Schneider
*/
class MMSMenuClass {
    private:
        string          className;      //! name of the theme class

        MMSTaffFile		*tafff;

        bool            isitemwidth;    //! is itemwidth set?
        string          itemwidth;      //! width of an item (percent or pixel)
        bool            isitemheight;   //! is itemheight set?
        string          itemheight;     //! height of an item (percent or pixel)
        bool            isitemhmargin;  //! is horizontal item margin set?
        unsigned int    itemhmargin;    //! horizontal item margin
        bool            isitemvmargin;  //! is vertical item margin set?
        unsigned int    itemvmargin;    //! vertical item margin
        bool            iscols;         //! is the number of columns set?
        unsigned int    cols;           //! number of columns
        bool            isdimitems;     //! is the dimitems value set?
        unsigned int    dimitems;       //! dimitems if the menu has not the focus
        bool            isfixedpos;     //! is the fixedpos value set?
        unsigned int    fixedpos;       //! fixed position of the selected item
        bool            ishloop;        //! is the hloop flag set?
        unsigned int    hloop;          //! loop horizontal (left/right) without jumping out of the menu
        bool            isvloop;        //! is the vloop flag set?
        unsigned int    vloop;          //! loop vertical (up/down) without jumping out of the menu
        bool            istransitems;   //! is the transitems value set?
        unsigned int    transitems;     //! transitems if the menu has not the focus
        bool            isdimtop;       //! is the dimtop value set?
        unsigned int    dimtop;         //! dim the items at the top of the menu
        bool            isdimbottom;    //! is the dimbottom value set?
        unsigned int    dimbottom;      //! dim the items at the bottom of the menu
        bool            isdimleft;      //! is the dimleft value set?
        unsigned int    dimleft;        //! dim the items at the left of the menu
        bool            isdimright;     //! is the dimright value set?
        unsigned int    dimright;       //! dim the items at the right of the menu
        bool            istranstop;     //! is the transtop value set?
        unsigned int    transtop;       //! transparent the items at the top of the menu
        bool            istransbottom;  //! is the transbottom value set?
        unsigned int    transbottom;    //! transparent the items at the bottom of the menu
        bool            istransleft;    //! is the transleft value set?
        unsigned int    transleft;      //! transparent the items at the left of the menu
        bool            istransright;   //! is the transright value set?
        unsigned int    transright;     //! transparent the items at the right of the menu

        bool			iszoomselwidth;	//! is zoom in (width) value set?
        string			zoomselwidth;	//! zoom in (width) the selected item (percent or pixel)
        bool			iszoomselheight;//! is zoom in (height) value set?
        string          zoomselheight;	//! zoom in (height) the selected item (percent or pixel)
        bool			iszoomselshiftx;//! is horizontal shift value set?
        string			zoomselshiftx;	//! horizontal shift of the selected item (percent or pixel)
        bool			iszoomselshifty;//! is vertical shift value set?
        string 			zoomselshifty;  //! vertical shift of the selected item (percent or pixel)
        
        bool			issmoothscrolling;
        bool  			smoothscrolling;

    public:
        MMSWidgetClass widgetClass; //! stores base widget attributes 

        ////////////////////////////////////////////////////////////////////////

        //! Constructor of class MMSMenuClass.
        MMSMenuClass();

        //! Destructor of class MMSMenuClass.
        ~MMSMenuClass();

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

        //! Saves a copy of an XML node including all child nodes.
        /*!
        \param node  pointer to the XML node to be copied
        */
        void duplicateXMLNode(MMSTaffFile *tafff);

        //! Get the copied XML node, see duplicateXMLNode().
        /*!
        \return pointer to the XML node
        */
        MMSTaffFile *getXMLNode();

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

        //! Check if the itemwidth is set.
        bool isItemWidth();

        //! Set the width of each item.
        /*!
        \param itemwidth  width of an item (percent or pixel)
        */
        void setItemWidth(string itemwidth);

        //! Mark the itemwidth as not set.
        void unsetItemWidth();

        //! Get the width of an item.
        /*!
        \return item width
        */
        string getItemWidth();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the itemheight is set.
        bool isItemHeight();

        //! Set the height of each item.
        /*!
        \param itemheight  height of an item (percent or pixel)
        */
        void setItemHeight(string itemheight);

        //! Mark the itemheight as not set.
        void unsetItemHeight();

        //! Get the height of an item.
        /*!
        \return item height
        */
        string getItemHeight();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the horizontal item margin is set.
        bool isItemHMargin();

        //! Set the horizontal item margin.
        /*!
        \param itemhmargin  horizontal margin
        */
        void setItemHMargin(unsigned int itemhmargin);

        //! Mark the itemhmargin as not set.
        void unsetItemHMargin();

        //! Get the horizontal item margin.
        /*!
        \return horizontal margin
        */
        unsigned int getItemHMargin();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the vertical item margin is set.
        bool isItemVMargin();

        //! Set the vertical item margin.
        /*!
        \param itemvmargin  vertical margin
        */
        void setItemVMargin(unsigned int itemvmargin);

        //! Mark the itemvmargin as not set.
        void unsetItemVMargin();

        //! Get the vertical item margin.
        /*!
        \return vertical margin
        */
        unsigned int getItemVMargin();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the number of columns is set.
        bool isCols();

        //! Set the number of columns.
        /*!
        \param cols  columns (equal or greater than 1)
        */
        void setCols(unsigned int cols);

        //! Mark the cols as not set.
        void unsetCols();

        //! Get the number of columns.
        /*!
        \return columns
        */
        unsigned int getCols();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the dimitems value is set.
        bool isDimItems();

        //! Set the dimitems value. The menu items will dim with this value if the menu has not the focus.
        /*!
        \param dimitems  dim the menu items with this value (0..255)
        */
        void setDimItems(unsigned int dimitems);

        //! Mark the dimitems as not set.
        void unsetDimItems();

        //! Get the dimitems value.
        /*!
        \return dimitems value
        */
        unsigned int getDimItems();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the fixedpos value is set.
        bool isFixedPos();

        //! Set the fixedpos value. The position of the selected menu item will not changed.
        /*!
        \param fixedpos  fixed position
        */
        void setFixedPos(int fixedpos);

        //! Mark the fixedpos as not set.
        void unsetFixedPos();

        //! Get the fixedpos value.
        /*!
        \return fixed position
        */
        int getFixedPos();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the hloop flag is set.
        bool isHLoop();

        //! Set the hloop flag. If true then it loops horizontal (left/right) without jumping out of the menu.
        /*!
        \param hloop  loop horizontal
        */
        void setHLoop(bool hloop);

        //! Mark the hloop as not set.
        void unsetHLoop();

        //! Get the hloop flag.
        /*!
        \return hloop flag
        */
        bool getHLoop();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the vloop flag is set.
        bool isVLoop();

        //! Set the vloop flag. If true then it loops vertical (up/down) without jumping out of the menu.
        /*!
        \param vloop  loop vertical
        */
        void setVLoop(bool vloop);

        //! Mark the vloop as not set.
        void unsetVLoop();

        //! Get the vloop flag.
        /*!
        \return vloop flag
        */
        bool getVLoop();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the transitems value is set.
        bool isTransItems();

        //! Set the transitems value. The menu items will get transparent with this value if the menu has not the focus.
        /*!
        \param transitems  transparent the menu items with this value (0..255)
        */
        void setTransItems(unsigned int transitems);

        //! Mark the transitems as not set.
        void unsetTransItems();

        //! Get the transitems value.
        /*!
        \return transitems value
        */
        unsigned int getTransItems();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the dimtop value is set.
        bool isDimTop();

        //! Set the dimtop value. The menu items at the top will dim with this value.
        /*!
        \param dimtop  dim the menu items with this value (0..255)
        */
        void setDimTop(unsigned int dimtop);

        //! Mark the dimtop as not set.
        void unsetDimTop();

        //! Get the dimtop value.
        /*!
        \return dimtop value
        */
        unsigned int getDimTop();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the dimbottom value is set.
        bool isDimBottom();

        //! Set the dimbottom value. The menu items at the bottom will dim with this value.
        /*!
        \param dimbottom  dim the menu items with this value (0..255)
        */
        void setDimBottom(unsigned int dimbottom);

        //! Mark the dimbottom as not set.
        void unsetDimBottom();

        //! Get the dimbottom value.
        /*!
        \return dimbottom value
        */
        unsigned int getDimBottom();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the dimleft value is set.
        bool isDimLeft();

        //! Set the dimleft value. The menu items at the left will dim with this value.
        /*!
        \param dimleft  dim the menu items with this value (0..255)
        */
        void setDimLeft(unsigned int dimleft);

        //! Mark the dimleft as not set.
        void unsetDimLeft();

        //! Get the dimleft value.
        /*!
        \return dimleft value
        */
        unsigned int getDimLeft();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the dimright value is set.
        bool isDimRight();

        //! Set the dimright value. The menu items at the right will dim with this value.
        /*!
        \param dimright  dim the menu items with this value (0..255)
        */
        void setDimRight(unsigned int dimright);

        //! Mark the dimright as not set.
        void unsetDimRight();

        //! Get the dimright value.
        /*!
        \return dimright value
        */
        unsigned int getDimRight();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the transtop value is set.
        bool isTransTop();

        //! Set the transtop value. The menu items at the top will get transparent with this value.
        /*!
        \param transtop  transparent the menu items with this value (0..255)
        */
        void setTransTop(unsigned int transtop);

        //! Mark the transtop as not set.
        void unsetTransTop();

        //! Get the transtop value.
        /*!
        \return transtop value
        */
        unsigned int getTransTop();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the transbottom value is set.
        bool isTransBottom();

        //! Set the transbottom value. The menu items at the bottom will get transparent with this value.
        /*!
        \param transbottom  transparent the menu items with this value (0..255)
        */
        void setTransBottom(unsigned int transbottom);

        //! Mark the transbottom as not set.
        void unsetTransBottom();

        //! Get the transbottom value.
        /*!
        \return transbottom value
        */
        unsigned int getTransBottom();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the transleft value is set.
        bool isTransLeft();

        //! Set the transleft value. The menu items at the left will get transparent with this value.
        /*!
        \param transleft  transparent the menu items with this value (0..255)
        */
        void setTransLeft(unsigned int transleft);

        //! Mark the transleft as not set.
        void unsetTransLeft();

        //! Get the transleft value.
        /*!
        \return transleft value
        */
        unsigned int getTransLeft();

        ////////////////////////////////////////////////////////////////////////

        //! Check if the transright value is set.
        bool isTransRight();

        //! Set the transright value. The menu items at the right will get transparent with this value.
        /*!
        \param transright  transparent the menu items with this value (0..255)
        */
        void setTransRight(unsigned int transright);

        //! Mark the transright as not set.
        void unsetTransRight();

        //! Get the transright value.
        /*!
        \return transright value
        */
        unsigned int getTransRight();

        
        bool isZoomSelWidth();
        void setZoomSelWidth(string zoomselwidth);
        void unsetZoomSelWidth();
        string getZoomSelWidth();
        bool isZoomSelHeight();
        void setZoomSelHeight(string zoomselheight);
        void unsetZoomSelHeight();
        string getZoomSelHeight();
        bool isZoomSelShiftX();
        void setZoomSelShiftX(string zoomselshiftx);
        void unsetZoomSelShiftX();
        string getZoomSelShiftX();
        bool isZoomSelShiftY();
        void setZoomSelShiftY(string zoomselshifty);
        void unsetZoomSelShiftY();
        string getZoomSelShiftY();
        
        bool isSmoothScrolling();
        void setSmoothScrolling(bool smoothscrolling);
        void unsetSmoothScrolling();
        bool getSmoothScrolling();
        
};

#endif /*MMSMENUCLASS_H_*/
