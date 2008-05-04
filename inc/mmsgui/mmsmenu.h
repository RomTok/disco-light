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

#ifndef MMSMENU_H_
#define MMSMENU_H_

#include "mmsgui/mmswidget.h"

//! With this class you can display a list of items.
/*!
The menu widget is focusable. So the user can scroll in it.
Menu items are normal widgets. But the root widget of an menu item has to be a focusable widget.
The root widget is normally a MMSButton widget.
Please note that you can nesting all other widgets under the root widget of the menu item.
So you can build complex menu items. But think about the performance if you work with menus
with an high number of items. 
\author Jens Schneider
*/
class MMSMenu : public MMSWidget {
    private:
        string          className;
        MMSMenuClass    *menuClass;
        MMSMenuClass    myMenuClass;

        MMSWidget       *itemTemplate;

        int    			item_w;     /* width of an item */ 
        int    			item_h;     /* height of an item */
        int    			v_items;    /* number of visible vertical items */
        int    			h_items;    /* number of visible horizontal items */
        unsigned int    x;          /* selected x */
        unsigned int    y;          /* selected y */
        unsigned int    px;         /* left position */
        unsigned int    py;         /* top position */

        bool            firstFocus;
        bool            firstSelection;

        bool			zoomsel;		/* should the selected item zoomed? */
        unsigned int 	zoomselwidth;	/* this value will be added to item_w for the selected item */      
        unsigned int 	zoomselheight;	/* this value will be added to item_h for the selected item */
        int 			zoomselshiftx;	/* x-move the unselected items around the selected item */  
        int 			zoomselshifty;	/* y-move the unselected items around the selected item */

        bool 			smooth_scrolling;
        int  			scrolling_offset;

        DFBRectangle 	virtualGeom;

        
        bool create(MMSWindow *root, string className, MMSTheme *theme);

        void add(MMSWidget *widget);

        void adjustVirtualRect();

        bool getConfig(bool *firstTime = NULL);

        void recalculateChildren();

        void switchArrowWidgets();
        void setSliders();

        void selectItem(MMSWidget *item, bool set, bool refresh = true);

        bool scrollDownEx(unsigned int count, bool refresh, bool test);
        bool scrollUpEx(unsigned int count, bool refresh, bool test);
        bool scrollRightEx(unsigned int count, bool refresh, bool test);
        bool scrollLeftEx(unsigned int count, bool refresh, bool test);

    public:
        MMSMenu(MMSWindow *root, string className, MMSTheme *theme = NULL);
        ~MMSMenu();
        MMSWIDGETTYPE getType() { return MMSWIDGETTYPE_MENU; }

        MMSWidget *copyWidget();

        void setItemTemplate(MMSWidget *itemTemplate);
        MMSWidget *getItemTemplate();

        MMSWidget *newItem();
        void clear();

        void setFocus(bool set, bool refresh = true);

        bool setSelected(unsigned int item, bool refresh = true);
        unsigned int getSelected();

        MMSWidget *getItem(unsigned int item);
        MMSWidget *getSelectedItem();

        unsigned int getSize();

        unsigned int getVItems();
        unsigned int getHItems();

        bool init();
        bool draw(bool *backgroundFilled = NULL);

        bool scrollDown(unsigned int count = 1, bool refresh = true, bool test = false);
        bool scrollUp(unsigned int count = 1, bool refresh = true, bool test = false);
        bool scrollRight(unsigned int count = 1, bool refresh = true, bool test = false);
        bool scrollLeft(unsigned int count = 1, bool refresh = true, bool test = false);
        bool scrollTo(int posx, int posy, bool refresh = true);
        
        sigc::signal<void, MMSWidget*> *onSelectItem;
        sigc::signal<void, MMSWidget*> *onBeforeScroll;

    public:
        /* theme access methods */
        MMSTaffFile *getXMLNode();
        string getItemWidth();
        string getItemHeight();
        unsigned int getItemHMargin();
        unsigned int getItemVMargin();
        unsigned int getCols();
        unsigned int getDimItems();
        int getFixedPos();
        bool getHLoop();
        bool getVLoop();
        unsigned int getTransItems();
        unsigned int getDimTop();
        unsigned int getDimBottom();
        unsigned int getDimLeft();
        unsigned int getDimRight();
        unsigned int getTransTop();
        unsigned int getTransBottom();
        unsigned int getTransLeft();
        unsigned int getTransRight();
        string getZoomSelWidth();
        string getZoomSelHeight();
        string getZoomSelShiftX();
        string getZoomSelShiftY();
        bool getSmoothScrolling();

        void setItemWidth(string itemwidth, bool refresh = true);
        void setItemHeight(string itemheight, bool refresh = true);
        void setItemHMargin(unsigned int itemhmargin, bool refresh = true);
        void setItemVMargin(unsigned int itemvmargin, bool refresh = true);
        void setCols(unsigned int cols, bool refresh = true);
        void setDimItems(unsigned int dimitems, bool refresh = true);
        void setFixedPos(int fixedpos, bool refresh = true);
        void setHLoop(bool hloop);
        void setVLoop(bool vloop);
        void setTransItems(unsigned int transitems, bool refresh = true);
        void setDimTop(unsigned int dimtop, bool refresh = true);
        void setDimBottom(unsigned int dimbottom, bool refresh = true);
        void setDimLeft(unsigned int dimleft, bool refresh = true);
        void setDimRight(unsigned int dimright, bool refresh = true);
        void setTransTop(unsigned int transtop, bool refresh = true);
        void setTransBottom(unsigned int transbottom, bool refresh = true);
        void setTransLeft(unsigned int transleft, bool refresh = true);
        void setTransRight(unsigned int transright, bool refresh = true);
        void setZoomSelWidth(string zoomselwidth, bool refresh = true);
        void setZoomSelHeight(string zoomselheight, bool refresh = true);
        void setZoomSelShiftX(string zoomselshiftx, bool refresh = true);
        void setZoomSelShiftY(string zoomselshifty, bool refresh = true);
        void setSmoothScrolling(bool smoothscrolling);

        void updateFromThemeClass(MMSMenuClass *themeClass);
};

#endif /*MMSMENU_H_*/

