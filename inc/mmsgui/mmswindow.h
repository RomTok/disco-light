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

#ifndef MMSWINDOW_H_
#define MMSWINDOW_H_

#include "mmsgui/mmswidget.h"
#include "mmsgui/mmswindowaction.h"
#include "mmsgui/mmsimagemanager.h"
#include "mmsgui/mmsfontmanager.h"
#include "mmsgui/mmsfbmanager.h"
#include "mmsgui/interfaces/immswindowmanager.h"
#include "mmstools/mmsmutex.h"


//! available types of windows
typedef enum {
	MMSWINDOWTYPE_MAINWINDOW = 0,	/*! main windows will be displayed over the root window
									    only one main window can be shown at the same time
									    if a main window appears the currently shown main window will be disappear
									*/
    MMSWINDOWTYPE_POPUPWINDOW,		//! popup windows will be displayed over root and main windows
    								//! the popup window which appears finally is on the top of the screen  
    MMSWINDOWTYPE_ROOTWINDOW,		//! root windows will be displayed in the background 
    								//! only one root window can be shown at the same time
    								//! if a root window appears the currently shown root window will be disappear
    MMSWINDOWTYPE_CHILDWINDOW		//! child windows are parts of main, popup and root windows
    								//! a full window functionality is given
} MMSWINDOWTYPE;

//! available window flags
typedef enum {
    MMSW_NONE               = 0x00000000,	//! none
    MMSW_VIDEO              = 0x00000001,	//! the window displays a video stream and should be on the video layer if it exists 
    MMSW_USEGRAPHICSLAYER   = 0x00000002	//! the window should use the graphics layer
} MMSWINDOW_FLAGS;


//! This class is the base class for all windows.
/*!
This class includes the base functionality available for all windows within MMSGUI.
This class cannot be constructed. Only windows which are derived from this class can be constructed.
\author Jens Schneider
*/
class MMSWindow {
    private:
    	
    	//! describes a child window
        typedef struct {
            MMSWindow       *window;        	//! points to the child window
            DFBRegion       region;         	//! region of the window within parent window
            unsigned char   opacity;        	//! opacity of the window
            unsigned char   oldopacity;     	//! old opacity of the window
            unsigned int    focusedWidget;  	//! save the last focused widget here
        } CHILDWINS;

        //! status area for the arrow widgets
        typedef struct {
        	bool up;							//! currently navigate up is possible?
        	bool down;							//! currently navigate down is possible?
        	bool left;							//! currently navigate left is possible?
        	bool right;							//! currently navigate right is possible?
        } ARROW_WIDGET_STATUS;
        
        MMSTheme            *theme;           	//! access to the theme which is used
        MMSWindowClass      *baseWindowClass;	//! base attributes of the window
        										//! this can be initialization values from theme.cpp
        										//! or from theme.xml (NOT a <class/> definition, but e.g. tag <mainwindow/>)
        MMSWindowClass      *windowClass;		//! attributes set by <class/> tag in theme.xml
        										//! is NULL, if window has no theme class definition
        										//! attributes set here, prevails over attributes from baseWindowClass
        MMSWindowClass      myWindowClass;		//! attributes of the window which will be set during
        										//! the runtime (e.g. window->setAlignment(...) )
        										//! attributes set here, prevails over attributes from windowClass
        										//! and baseWindowClass

        MMSWINDOW_FLAGS		flags;				//! window creation flags

		MMSMutex      		Lock;               //! to make it thread-safe :)
        unsigned long       TID;                //! save the id of the thread which has locked the window
        unsigned long       Lock_cnt;           //! count the number of times the thread has call lock()
        										//! because it is not a problem, that the same thread calls lock() several times

        MMSMutex      		drawLock;			//! special draw lock
        MMSMutex      		flipLock;			//! special flip lock
        MMSMutex  			preCalcNaviLock;	//! lock the pre-calculation of the navigation

        bool                initialized;		//! is window initialized?

        string 				name;              	//! name of the window

        MMSWindow           *parent;            //! parent window (if window is a child window) or NULL
		
        MMSImageManager     *im;				//! image manager for the window 
        MMSFontManager      *fm;				//! font manager for the window

        MMSFBSurface        *bgimage;			//!	background image
        MMSFBSurface        *borderimages[8];	//! border images
        DFBRectangle        bordergeom[8];		//! border geometry
        bool                bordergeomset;		//! border geometry set?

        MMSFBLayer          *layer;				//! access to the MMSFBLayer on which the window has to be displayed
        MMSFBWindow         *window;			//! access to the MMSFBWindow which is behind of this class
        MMSFBSurface        *surface;			//! access to the MMSFBSurface of the window 

        DFBRectangle 		vrect;				//! visible screen area (that means the visible area e.g. on the TV set)
        										//! see the initialization of the MMSWindowManager

        int                 dxpix;				//! x-movement of the window based on the alignment attribute
        int                 dypix;				//! y-movement of the window based on the alignment attribute
        DFBRectangle        geom;				//! geometry of the window based on the margin attribute
        DFBRectangle        innerGeom;			//! inner geometry of the window based on the border margin attribute

        bool				draw_setgeom;		//! check and recalc the geometry of the widgets during the next draw()? 
        
        vector<MMSWidget *>	children;			//! widgets of the window
        MMSWidget 			*focusedwidget;		//! focused widget or NULL
        bool                shown;				//! is window shown?
        bool				willshow;			//! is show animation running?
        bool				willhide;			//! is hide animation running?
        bool                firstfocusset;		//! focus set the first time?

        vector<CHILDWINS>   childwins;			//! child windows of the window
        unsigned int        focusedChildWin;	//! focused child window

        MMSWidget       	*upArrowWidget;		//! widget which has to be selected if it is possible to navigate up
        MMSWidget       	*downArrowWidget;	//! widget which has to be selected if it is possible to navigate down
        MMSWidget       	*leftArrowWidget;	//! widget which has to be selected if it is possible to navigate left
        MMSWidget       	*rightArrowWidget;	//! widget which has to be selected if it is possible to navigate right
        bool            	initialArrowsDrawn;	//! up/down/left/right arrow widgets updated the first time?

        MMSWindow       	*navigateUpWindow;		//! child window which is to be focused if user navigates up
        MMSWindow       	*navigateDownWindow;	//! child window which is to be focused if user navigates down
        MMSWindow       	*navigateLeftWindow;	//! child window which is to be focused if user navigates left
        MMSWindow       	*navigateRightWindow;	//! child window which is to be focused if user navigates right

        MMSWidget			*buttonpress_widget;	//! widget on which the user has pressed the (mouse) button
        MMSWindow			*buttonpress_childwin;	//! child window on which the user has pressed the (mouse) button

        
        //! Internal method: Creates the window.
        bool create(string dx, string dy, string w, string h, MMSALIGNMENT alignment, MMSWINDOW_FLAGS flags,
        		    bool *own_surface);

        //! Internal method: Create the window.
        bool create(string w, string h, MMSALIGNMENT alignment, MMSWINDOW_FLAGS flags,
					bool *own_surface);

        //! Internal method: Resize the window.
        bool resize(bool refresh = true);

        //! Internal method: Add a child window.
        bool addChildWindow(MMSWindow *childwin);

        //! Internal method: Set the opacity of a child window.
        bool setChildWindowOpacity(MMSWindow *childwin, unsigned char opacity);

        //! Internal method: Set the region of a child window.
        bool setChildWindowRegion(MMSWindow *childwin, bool refresh = true);

        //! Internal method: Move a child window.
        bool moveChildWindow(MMSWindow *childwin, int x, int y, bool refresh = true);

        //! Internal method: Draw a child window.
        void drawChildWindows(MMSFBSurface *dst_surface, DFBRegion *region = NULL, int offsX = 0, int offsY = 0);

        //! Internal method: Flip a window.
        bool flipWindow(MMSWindow *win = NULL, DFBRegion *region = NULL,
                        MMSFBSurfaceFlipFlags flags = (MMSFBSurfaceFlipFlags)0,
                        bool flipChildSurface = true, bool locked = false);
        
        //! Internal method: Remove the focus from a child window.
        void removeFocusFromChildWindow();

        //! Internal method: Load widgets for up/down/left/right arrows.
        void loadArrowWidgets();

        //! Internal method: Get the navigation status. With this infos i can select/unselect the arrow widgets. 
        void getArrowWidgetStatus(ARROW_WIDGET_STATUS *setarrows);

        //! Internal method: Update the status of the arrow widgets. 
        void switchArrowWidgets();

        //! Internal method: Init me.
        virtual bool init();

        //! Internal method: Draw me.
        virtual void draw(bool toRedrawOnly = false, DFBRectangle *rect2update = NULL, bool clear = true);

        //! Internal method: Draw my border.
        void drawMyBorder();

        //! Internal method: Focus one widget/child window for the first time.
        bool setFirstFocus(bool cw = false);

        //! Internal method: Used to find best candidate to navigate up from currPos.
        double calculateDistGradCode_Up(DFBRectangle currPos, DFBRectangle candPos);

        //! Internal method: Used to find best candidate to navigate down from currPos.
        double calculateDistGradCode_Down(DFBRectangle currPos, DFBRectangle candPos);

        //! Internal method: Used to find best candidate to navigate left from currPos.
        double calculateDistGradCode_Left(DFBRectangle currPos, DFBRectangle candPos);

        //! Internal method: Used to find best candidate to navigate right from currPos.
        double calculateDistGradCode_Right(DFBRectangle currPos, DFBRectangle candPos);

        //! Internal method: Handle widget navigation (up/down/left/right).
        bool handleNavigationForWidgets(MMSInputEvent *inputevent);

        //! Internal method: Remove the focus from the currently focused child window. Goal: Change status of widgets. 
        void removeChildWinFocus();

        //! Internal method: Restore the focus to the currently focused child window. Goal: Change status of widgets. 
        bool restoreChildWinFocus();

        //! Internal method: Handle child window navigation (up/down/left/right).
        bool handleNavigationForChildWins(MMSInputEvent *inputevent);

        //! Internal method: Do the pre-calculation of the navigation routes.
        void preCalcNavigation();

        void lock();
        void unlock();
        
	protected:
		
        static class IMMSWindowManager 	*windowmanager;	//! interface to the window manager
        MMSWindowAction         		*action;		//! window action thread (used for animations) 

    public:
    	
    	//! The base constructor for all window types.
    	/*! This will internally used by the supported window types/classes (see MMSWINDOWTYPE).
    	*/
        MMSWindow();
        
        //! The base destructor for this class.
        virtual ~MMSWindow(); 

        //! Get the type of the window.
        /*!
        \return type of the window
        */
        virtual MMSWINDOWTYPE getType() = 0;


        string getName();
        void   setName(string name);
        MMSWindow* searchForWindow(string name);

        virtual bool show();
        virtual bool showAction(bool *stopaction);
        virtual bool hide(bool goback = false, bool wait = false);   
        virtual bool hideAction(bool *stopaction);   
        void waitUntilShown();
        void waitUntilHidden();
        void refresh();
        void refreshFromChild(MMSWidget *child, DFBRectangle *rect2update = NULL);
        void setFocusedWidget(MMSWidget *child, bool set, bool switchfocus = false);
        bool handleInput(vector<MMSInputEvent> *inputeventset);
        DFBRectangle getGeometry();
        DFBRectangle getRealGeometry();
        void add(MMSWidget *child);
        void remove(MMSWidget *child);
        MMSWidget *getFocusedWidget();
        int getNumberOfFocusableWidgets();
        int getNumberOfFocusableChildWins();
        MMSFBSurface *getSurface();
        void setWindowManager(IMMSWindowManager *wm);
        bool isShown(bool checkparents = false);
        bool willHide();

		void instantShow();
		void instantHide();

        void setFocus();
        bool getFocus();

        void recalculateChildren();

        MMSWidget* searchForWidget(string name);
        MMSWidget* operator[](string name);

        MMSWindow *getNavigateUpWindow();
        MMSWindow *getNavigateDownWindow();
        MMSWindow *getNavigateLeftWindow();
        MMSWindow *getNavigateRightWindow();
        void setNavigateUpWindow(MMSWindow *upWindow);
        void setNavigateDownWindow(MMSWindow *downWindow);
        void setNavigateRightWindow(MMSWindow *rightWindow);
        void setNavigateLeftWindow(MMSWindow *leftWindow);

        sigc::signal<bool, MMSWindow*>::accumulated<bool_accumulator> *onBeforeShow;
        sigc::signal<void, MMSWindow*, bool> *onAfterShow;
        sigc::signal<bool, MMSWindow*, bool>::accumulated<bool_accumulator> *onBeforeHide;
        sigc::signal<void, MMSWindow*, bool> *onHide;
        sigc::signal<void, MMSWindow*, MMSInputEvent*> *onHandleInput;

    public:
        /* theme access methods */
        bool getAlignment(MMSALIGNMENT &alignment);
        bool getDx(string &dx);
        int getDxPix();
        bool getDy(string &dy);
        int getDyPix();
        bool getWidth(string &width);
        bool getHeight(string &height);
        bool getBgColor(DFBColor &bgcolor);
        bool getBgImagePath(string &bgimagepath);
        bool getBgImageName(string &bgimagename);
        bool getOpacity(unsigned int &opacity);
        bool getFadeIn(bool &fadein);
        bool getFadeOut(bool &fadeout);
        bool getDebug(bool &debug);
        bool getMargin(unsigned int &margin);
        bool getUpArrow(string &uparrow);
        bool getDownArrow(string &downarrow);
        bool getLeftArrow(string &leftarrow);
        bool getRightArrow(string &rightarrow);
        bool getNavigateUp(string &navigateup);
        bool getNavigateDown(string &navigatedown);
        bool getNavigateLeft(string &navigateleft);
        bool getNavigateRight(string &navigateright);
        bool getOwnSurface(bool &ownsurface);
        bool getMoveIn(MMSDIRECTION &movein);
        bool getMoveOut(MMSDIRECTION &moveout);

        bool getBorderColor(DFBColor &color);
        bool getBorderImagePath(string &imagepath);
        bool getBorderImageNames(unsigned int num, string &imagename);
        bool getBorderThickness(unsigned int &thickness);
        bool getBorderMargin(unsigned int &margin);
        bool getBorderRCorners(bool &rcorners);
        
        
        
        void setAlignment(MMSALIGNMENT alignment, bool refresh = true, bool resize = true);
        void setDx(string dx, bool refresh = false, bool resize = true);
        void setDxPix(int dx, bool refresh = false, bool resize = true);
        void setDy(string dy, bool refresh = false, bool resize = true);
        void setDyPix(int dy, bool refresh = false, bool resize = true);
        void setWidth(string width, bool refresh = true, bool resize = true);
        void setHeight(string height, bool refresh = true, bool resize = true);
        void setBgColor(DFBColor bgcolor, bool refresh = true);
        void setBgImagePath(string bgimagepath, bool load = true, bool refresh = true);
        void setBgImageName(string bgimagename, bool load = true, bool refresh = true);
        void setOpacity(unsigned int opacity, bool refresh = true);
        void setFadeIn(bool fadein);
        void setFadeOut(bool fadeout);
        void setDebug(bool debug, bool refresh = true);
        void setMargin(unsigned int margin, bool refresh = true, bool resize = true);
        void setUpArrow(string uparrow, bool refresh = true);
        void setDownArrow(string downarrow, bool refresh = true);
        void setLeftArrow(string leftarrow, bool refresh = true);
        void setRightArrow(string rightarrow, bool refresh = true);
        void setNavigateUp(string navigateup);
        void setNavigateDown(string navigatedown);
        void setNavigateLeft(string navigateleft);
        void setNavigateRight(string navigateright);
        void setOwnSurface(bool ownsurface);
        void setMoveIn(MMSDIRECTION movein);
        void setMoveOut(MMSDIRECTION moveout);

        void setBorderColor(DFBColor color, bool refresh = true);
        void setBorderImagePath(string imagepath, bool load = true, bool refresh = true);
        void setBorderImageNames(string imagename_1, string imagename_2, string imagename_3, string imagename_4,
                                 string imagename_5, string imagename_6, string imagename_7, string imagename_8,
                                 bool load = true, bool refresh = true);
        void setBorderThickness(unsigned int thickness, bool refresh = true, bool resize = true);
        void setBorderMargin(unsigned int margin, bool refresh = true, bool resize = true);
        void setBorderRCorners(bool rcorners, bool refresh = true);

        void updateFromThemeClass(MMSWindowClass *themeClass);

    /* friends */
    friend class MMSMainWindow;
    friend class MMSPopupWindow;
    friend class MMSRootWindow;
    friend class MMSChildWindow;
    friend class MMSWidget;
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

#endif /*MMSWINDOW_H_*/
