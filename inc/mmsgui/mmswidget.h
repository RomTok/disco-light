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

#ifndef MMSWIDGET_H_
#define MMSWIDGET_H_

#include "mmstools/mmstools.h"
#include "mmstools/mmslogger.h"
#include "mmsgui/theme/mmstheme.h"
#include "mmsgui/mmsguitools.h"
#include <sigc++/sigc++.h>



//example from: http://libsigc.sourceforge.net/libsigc2/docs/reference/html/classsigc_1_1signal_1_1accumulated.html
//this accumulator calculates the arithmetic mean value
struct arithmetic_mean_accumulator
{
  typedef double result_type;
  template<typename T_iterator>
  result_type operator()(T_iterator first, T_iterator last) const
  {
    result_type value_ = 0;
    int n_ = 0;
    for (; first != last; ++first, ++n_)
      value_ += *first;
    return value_ / n_;
  }
};

//example from: http://libsigc.sourceforge.net/libsigc2/docs/reference/html/classsigc_1_1signal_1_1accumulated.html
//this accumulator stops signal emission when a slot returns zero
struct interruptable_accumulator
{
  typedef bool result_type;
  template<typename T_iterator>
  result_type operator()(T_iterator first, T_iterator last) const
  {
    int n_ = 0;
    for (; first != last; ++first, ++n_)
      if (!*first) return false;
    return true;
  }
};

//with this accumulator the emit() method of a callback ends with
//  - true,  if the no callback methods are connected or all connected callback methods returns true
//  - false, if at least one connected callback method returns false
struct bool_accumulator
{
  typedef bool result_type;
  template<typename T_iterator>
  result_type operator()(T_iterator first, T_iterator last) const
  {
    bool ret_ = true;
    int n_ = 0;
    for (; first != last; ++first, ++n_)
      if (!*first) ret_ = false;
    return ret_;
  }
};




/*
#ifndef DFBCHECK
    #define DFBCHECK( x... ) \
    {\
         DFBResult err = x;\
         if (err != DFB_OK) {\
              fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );\
              DirectFBErrorFatal( #x, err );\
         }\
    }
#endif*/

typedef void(*GUIINPUTCALLBACK)(DFBInputDeviceKeySymbol);

typedef struct {
	DFBInputDeviceKeySymbol key;
	GUIINPUTCALLBACK cb;	        	
} INPUT_CB;

MMS_CREATEERROR(MMSWidgetError);

typedef enum {
    MMSWIDGETTYPE_HBOX = 0,
    MMSWIDGETTYPE_VBOX,
    MMSWIDGETTYPE_BUTTON,
    MMSWIDGETTYPE_IMAGE,
    MMSWIDGETTYPE_LABEL,
    MMSWIDGETTYPE_MENU,
    MMSWIDGETTYPE_PROGRESSBAR,
    MMSWIDGETTYPE_TEXTBOX,
    MMSWIDGETTYPE_ARROW,
    MMSWIDGETTYPE_SLIDER
} MMSWIDGETTYPE;

//! This class is the base class for all widgets.
/*!
This class includes the base functionality available for all widgets within MMSGUI.
This class cannot be constructed. Only widgets which are derived from this class can be constructed.
\author Jens Schneider
*/
class MMSWidget {
    private:
        MMSTheme            *theme;             /* access to the theme which is used */
        MMSWidgetClass      *baseWidgetClass;
        MMSWidgetClass      *widgetClass;
        MMSWidgetClass      myWidgetClass;

        bool                initialized;
        MMSFBSurface        *bgimage;
        MMSFBSurface        *selbgimage;
        MMSFBSurface        *bgimage_p;
        MMSFBSurface        *selbgimage_p;
        MMSFBSurface        *bgimage_i;
        MMSFBSurface        *selbgimage_i;

        MMSFBSurface        *borderimages[8];
        DFBRectangle        bordergeom[8];
        bool                bordergeomset;
        MMSFBSurface        *borderselimages[8];
        DFBRectangle        borderselgeom[8];
        bool                borderselgeomset;

        class MMSWindow *rootwindow;
        int    id;
        string name;

        bool drawable;
        bool needsparentdraw;
        bool focusable_initial;
        bool selectable_initial;
        bool canhavechildren;
        bool canselectchildren;

        bool focused;
        bool selected;
        bool activated;
        bool pressed;

        unsigned char brightness;
        unsigned char opacity;

//        vector<INPUT_CB *> inputs;

    /* fixme: must be sth more apreciable */
	void *bindata;
	string sizehint;

        bool visible;

        MMSWidget       *upArrowWidget;
        MMSWidget       *downArrowWidget;
        MMSWidget       *leftArrowWidget;
        MMSWidget       *rightArrowWidget;

        bool            initialArrowsDrawn;

        MMSWidget       *navigateUpWidget;
        MMSWidget       *navigateDownWidget;
        MMSWidget       *navigateLeftWidget;
        MMSWidget       *navigateRightWidget;

        MMSWidget       *vSliderWidget;
        MMSWidget       *hSliderWidget;

        MMSInputEvent 	last_inputevent;

        
        
        bool has_own_surface; 

        
        void loadArrowWidgets();
        virtual void switchArrowWidgets();

        bool create(MMSWindow *root, bool drawable, bool needsparentdraw, bool focusable, bool selectable,
                    bool canhavechildren, bool canselectchildren);
		    
        virtual bool init();
        virtual bool draw(bool *backgroundFilled = NULL);
        void drawMyBorder();
        bool drawDebug();

    public:
        MMSWidget();
        virtual ~MMSWidget();
        virtual MMSWIDGETTYPE getType() = 0;

        void copyWidget(MMSWidget *newWidget);
        virtual MMSWidget *copyWidget() = 0;

        MMSWidget* getChild(unsigned int atpos = 0);
        MMSWidget* disconnectChild(unsigned int atpos = 0);
        MMSWidget* searchForWidget(string name);
        MMSWidget* operator[](string name);

        virtual void add(MMSWidget *widget);
        MMSWindow *getRootWindow();
        virtual void setGeometry(DFBRectangle geom);
        DFBRectangle getGeometry();
        DFBRectangle getRealGeometry();
        DFBRectangle getInnerGeometry();
        DFBRectangle getSurfaceGeometry();

        int getId();
        string getName();
        void   setName(string name);
        void setParent(MMSWidget *parent);
        MMSWidget *getParent();

        virtual void setFocus(bool set, bool refresh = true);
        bool isFocused();
        virtual bool setSelected(bool set, bool refresh = true);
        bool isSelected();
        void unsetFocusableForAllChildren(bool refresh);

        void setActivated(bool set, bool refresh = true);
        bool isActivated();

        void setPressed(bool set, bool refresh = true);
        bool isPressed();
        
        void setASelected(bool set, bool refresh = true);
        void setPSelected(bool set, bool refresh = true);
        void setISelected(bool set, bool refresh = true);

        bool isDrawable();
        bool needsParentDraw(bool checkborder = true);
        bool canHaveChildren();
        bool canSelectChildren();

//        virtual void handleNavigation(DFBInputDeviceKeySymbol key, MMSWidget *requestingchild);
        void setBinData(void *data);
        void *getBinData();
        bool setSizeHint(string hint);
        string getSizeHint();
        bool isGeomSet();
        void setGeomSet(bool set);

        bool isVisible();
        virtual void setVisible(bool visible, bool refresh = true);

        unsigned char getBrightness();
        void setBrightness(unsigned char brightness, bool refresh = true);

        unsigned char getOpacity();
        void setOpacity(unsigned char opacity, bool refresh = true);

        MMSWidget *getNavigateUpWidget();
        MMSWidget *getNavigateDownWidget();
        MMSWidget *getNavigateLeftWidget();
        MMSWidget *getNavigateRightWidget();
        void setNavigateUpWidget(MMSWidget *upwidget);
        void setNavigateDownWidget(MMSWidget *downwidget);
        void setNavigateRightWidget(MMSWidget *rightwidget);
        void setNavigateLeftWidget(MMSWidget *leftwidget);

        bool canNavigateUp();
        bool canNavigateDown();
        bool canNavigateLeft();
        bool canNavigateRight();

        virtual bool scrollDown(unsigned int count = 1, bool refresh = true, bool test = false);
        virtual bool scrollUp(unsigned int count = 1, bool refresh = true, bool test = false);
        virtual bool scrollRight(unsigned int count = 1, bool refresh = true, bool test = false);
        virtual bool scrollLeft(unsigned int count = 1, bool refresh = true, bool test = false);
        virtual bool scrollTo(int posx, int posy, bool refresh = true);
        
        sigc::signal<void, MMSWidget*> *onSelect;
        sigc::signal<void, MMSWidget*, bool> *onFocus;
        sigc::signal<void, MMSWidget*> *onReturn;

    protected:
        void drawchildren(bool toRedrawOnly = false, bool *backgroundFilled = NULL);
        void setRootWindow(class MMSWindow *root);
        virtual void recalculateChildren();
//        void registerInput(DFBInputDeviceKeySymbol key, GUIINPUTCALLBACK cb);
        virtual void handleInput(MMSInputEvent *inputevent);
        
        bool geomset;


        bool toRedraw;
        bool redrawChildren;
        void markChildren2Redraw();
        virtual MMSWidget *getDrawableParent(bool mark2Redraw = false, bool markChildren2Redraw = false,
                                             bool checkborder = true, vector<MMSWidget*> *wlist = NULL, bool followpath = false);
        void refresh();

//        IDirectFB *dfb;
        MMSFBSurface *windowSurface;

        MMSFBSurface *surface;
        DFBRectangle surfaceGeom;

        virtual void setSurfaceGeometry(unsigned int width = 0, unsigned int height = 0);
        virtual void setInnerGeometry();

        unsigned int scrollPosX;
        unsigned int scrollPosY;
        unsigned int scrollDX;
        unsigned int scrollDY;
        bool setScrollSize(unsigned int dX = 8, unsigned int dY = 8);
        bool setScrollPos(int posX = 0, int posY = 0, bool refresh = true, bool test = false);
        DFBRectangle getVisibleSurfaceArea();
        void updateWindowSurfaceWithSurface(bool useAlphaChannel);

        MMSWidget *parent;
        vector<MMSWidget *> children;

        DFBRectangle geom;
        DFBRectangle innerGeom;
        
    public:
        /* theme access methods */
        bool 	getBgColor(DFBColor &bgcolor);
        bool 	getSelBgColor(DFBColor &selbgcolor);
        bool	getBgColor_p(DFBColor &bgcolor_p);
        bool	getSelBgColor_p(DFBColor &selbgcolor_p);
        bool	getBgColor_i(DFBColor &bgcolor_i);
        bool	getSelBgColor_i(DFBColor &selbgcolor_i);
        bool    getBgImagePath(string &bgimagepath);
        bool    getBgImageName(string &bgimagename);
        bool    getSelBgImagePath(string &selbgimagepath);
        bool    getSelBgImageName(string &selbgimagename);
        bool    getBgImagePath_p(string &bgimagepath_p);
        bool    getBgImageName_p(string &bgimagename_p);
        bool    getSelBgImagePath_p(string &selbgimagepath_p);
        bool    getSelBgImageName_p(string &selbgimagename_p);
        bool    getBgImagePath_i(string &bgimagepath_i);
        bool    getBgImageName_i(string &bgimagename_i);
        bool    getSelBgImagePath_i(string &selbgimagepath_i);
        bool    getSelBgImageName_i(string &selbgimagename_i);
        bool 	getMargin(unsigned int &margin);
        bool 	getFocusable(bool &focusable);
        bool 	getSelectable(bool &selectable);
        bool	getUpArrow(string &uparrow);
        bool	getDownArrow(string &downarrow);
        bool	getLeftArrow(string &leftarrow);
        bool	getRightArrow(string &rightarrow);
        bool	getData(string &data);
        bool	getNavigateUp(string &navigateup);
        bool	getNavigateDown(string &navigatedown);
        bool	getNavigateLeft(string &navigateleft);
        bool	getNavigateRight(string &navigateright);
        bool	getVSlider(string &vslider);
        bool	getHSlider(string &hslider);
        bool 	getImagesOnDemand(bool &imagesondemand);
        bool 	getBlend(unsigned int &blend);
        bool 	getBlendFactor(double &blendfactor);

        bool	getBorderColor(DFBColor &color);
        bool 	getBorderSelColor(DFBColor &selcolor);
        bool 	getBorderImagePath(string &imagepath);
        bool	getBorderImageNames(unsigned int num, string &imagename);
        bool	getBorderSelImagePath(string &selimagepath);
        bool	getBorderSelImageNames(unsigned int num, string &selimagename);
        bool	getBorderThickness(unsigned int &thickness);
        bool	getBorderMargin(unsigned int &margin);
        bool 	getBorderRCorners(bool &rcorners);

        void setBgColor(DFBColor bgcolor, bool refresh = true);
        void setSelBgColor(DFBColor selbgcolor, bool refresh = true);
        void setBgColor_p(DFBColor bgcolor_p, bool refresh = true);
        void setSelBgColor_p(DFBColor selbgcolor_p, bool refresh = true);
        void setBgColor_i(DFBColor bgcolor_i, bool refresh = true);
        void setSelBgColor_i(DFBColor selbgcolor_i, bool refresh = true);
        void setBgImagePath(string bgimagepath, bool load = true, bool refresh = true);
        void setBgImageName(string bgimagename, bool load = true, bool refresh = true);
        void setSelBgImagePath(string selbgimagepath, bool load = true, bool refresh = true);
        void setSelBgImageName(string selbgimagename, bool load = true, bool refresh = true);
        void setBgImagePath_p(string bgimagepath_p, bool load = true, bool refresh = true);
        void setBgImageName_p(string bgimagename_p, bool load = true, bool refresh = true);
        void setSelBgImagePath_p(string selbgimagepath_p, bool load = true, bool refresh = true);
        void setSelBgImageName_p(string selbgimagename_p, bool load = true, bool refresh = true);
        void setBgImagePath_i(string bgimagepath_i, bool load = true, bool refresh = true);
        void setBgImageName_i(string bgimagename_i, bool load = true, bool refresh = true);
        void setSelBgImagePath_i(string selbgimagepath_i, bool load = true, bool refresh = true);
        void setSelBgImageName_i(string selbgimagename_i, bool load = true, bool refresh = true);
        void setMargin(unsigned int margin, bool refresh = true);
        bool setFocusable(bool focusable, bool refresh = true);
        bool setSelectable(bool selectable, bool refresh = true);
        void setUpArrow(string uparrow, bool refresh = true);
        void setDownArrow(string downarrow, bool refresh = true);
        void setLeftArrow(string leftarrow, bool refresh = true);
        void setRightArrow(string rightarrow, bool refresh = true);
        void setData(string data);
        void setNavigateUp(string navigateup);
        void setNavigateDown(string navigatedown);
        void setNavigateLeft(string navigateleft);
        void setNavigateRight(string navigateright);
        void setVSlider(string vslider);
        void setHSlider(string hslider);
        void setImagesOnDemand(bool imagesondemand);
        void setBlend(unsigned int blend, bool refresh = true);
        void setBlendFactor(double blendfactor, bool refresh = true);

        void setBorderColor(DFBColor bordercolor, bool refresh = true);
        void setBorderSelColor(DFBColor borderselcolor, bool refresh = true);
        void setBorderImagePath(string borderimagepath, bool load = true, bool refresh = true);
        void setBorderImageNames(string imagename_1, string imagename_2, string imagename_3, string imagename_4,
                                 string imagename_5, string imagename_6, string imagename_7, string imagename_8,
                                 bool load = true, bool refresh = true);
        void setBorderSelImagePath(string borderselimagepath, bool load = true, bool refresh = true);
        void setBorderSelImageNames(string selimagename_1, string selimagename_2, string selimagename_3, string selimagename_4,
                                    string selimagename_5, string selimagename_6, string selimagename_7, string selimagename_8,
                                    bool load = true, bool refresh = true);
        void setBorderThickness(unsigned int borderthickness, bool refresh = true);
        void setBorderMargin(unsigned int bordermargin, bool refresh = true);
        void setBorderRCorners(bool borderrcorners, bool refresh = true);

        void updateFromThemeClass(MMSWidgetClass *themeClass);

    friend class MMSWindow;
    friend class MMSHBox;
    friend class MMSVBox;
    friend class MMSLabel;
    friend class MMSButton;
    friend class MMSImage;
    friend class MMSProgressBar;
    friend class MMSMenu;
    friend class MMSTextBox;
    friend class MMSArrow;
    friend class MMSSlider;
};

#include "mmswindow.h"

#endif /*MMSWIDGET_H_*/
