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


//! The available types of windows.
typedef enum {
	/*!
	Main windows will be displayed over the root window.
	Only one main window can be shown at the same time.
	If a main window appears the currently shown main window will be disappear.
	*/
	MMSWINDOWTYPE_MAINWINDOW = 0,
	/*!
	Popup windows will be displayed over root and main windows.
	The popup window which appears finally is on the top of the screen.
	*/
    MMSWINDOWTYPE_POPUPWINDOW,
    /*!
    Root windows will be displayed in the background.
	Only one root window can be shown at the same time.
	If a root window appears the currently shown root window will be disappear.
	*/
    MMSWINDOWTYPE_ROOTWINDOW,
    /*!
    Child windows are parts of main, popup and root windows.
    A full window functionality is given.
    */
    MMSWINDOWTYPE_CHILDWINDOW		
} MMSWINDOWTYPE;

//! The available window flags.
typedef enum {
	//! none
    MMSW_NONE               = 0x00000000,
    //! The window displays a video stream and should be on the video layer if it exists.
    MMSW_VIDEO              = 0x00000001,
    //! The window should use the graphics layer.
    MMSW_USEGRAPHICSLAYER   = 0x00000002
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
        	//! points to the child window
            MMSWindow       *window;
            //! region of the window within parent window
            DFBRegion       region;
            //! opacity of the window
            unsigned char   opacity;
         	//! old opacity of the window
            unsigned char   oldopacity;
            //! save the last focused widget here
            unsigned int    focusedWidget;
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

        MMSWindowAction   	*action;			//! window action thread (used for animations) 
        
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

        //! Internal method: Lock the window. Will be used by the widgets.
        void lock();

        //! Internal method: Unlock the window. Will be used by the widgets.
        void unlock();

        //! Internal method: Will be called from the MMSWindowAction thread if the window should appear.
        virtual bool showAction(bool *stopaction);

        //! Internal method: Will be called from the MMSWindowAction thread if the window should disappear.
        virtual bool hideAction(bool *stopaction);   

        //! Internal method: Refresh a part of a window. Will be used by the widgets.
        void refreshFromChild(MMSWidget *child, DFBRectangle *rect2update = NULL);
        
        //! Internal method: Set the focused widget.
        void setFocusedWidget(MMSWidget *child, bool set, bool switchfocus = false);

        //! Internal method: Will be called by MMSInputManager if the window has the input focus.
        bool handleInput(vector<MMSInputEvent> *inputeventset);

        //! Internal method: (Re-)calculate the position and size of all widgets.
        void recalculateChildren();
        
        //! Internal method: Show the window without animation.
		void instantShow();

        //! Internal method: Hide the window without animation.
		void instantHide();

    protected:
		
        static class IMMSWindowManager 	*windowmanager;	//! interface to the window manager

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

        //! Get the name of the window.
        /*!
        \return name of the window
        */
        string getName();

        //! Set the name of the window.
        /*!
        \param name		name of the window
        */
        void   setName(string name);

        //! Find a window over its name.
        /*!
        \param name		name of the window
        \return pointer to the MMSWindow object or NULL
        */
        MMSWindow* searchForWindow(string name);

        //! Makes a window visible.
        /*!
        \return true if the show action successfully started 
        \note The MMSWindowAction thread will be started here and show() returnes immediately!
        */
        virtual bool show();

        //! Hide a visible window.
        /*!
        \param goback	this parameter will be routed to the onHide callback, so the application can
        				decide which if and which window should appear afterwards
        \param wait		waiting until hideAction is finished?
        \return true if the hide action successfully started 
        \note The MMSWindowAction thread will be started here and hide() returnes immediately if wait=false!
        */
        virtual bool hide(bool goback = false, bool wait = false);   

        //! Can be called after show() waiting for end of showAction thread.
        void waitUntilShown();

        //! Can be called after hide() waiting for end of hideAction thread.
        void waitUntilHidden();
        
        //! Get the geometry of the window.
        /*!
        \return the rectangle of the window on the layer or parent window 
        */
        DFBRectangle getGeometry();

        //! Get the geometry of the window based on the layer.
        /*! 
        \return the rectangle of the window on the layer 
        */
        DFBRectangle getRealGeometry();

        //! Add a widget to the window.
        /*! 
        \param child	pointer to a widget
        */
        void add(MMSWidget *child);

        //! Remove a widget from the window.
        /*! 
        \param child	pointer to a widget
        */
        void remove(MMSWidget *child);

        //! Get the currently focused widget.
        /*! 
        \return pointer to the focused widget
        */
        MMSWidget *getFocusedWidget();

        //! Get the number of focusable widgets.
        /*! 
        \return number of focusable widgets
        */
        int getNumberOfFocusableWidgets();

        //! Get the number of focusable child windows.
        /*! 
        \return number of focusable child windows
        */
        int getNumberOfFocusableChildWins();

        //! Refresh (redraw) the whole window.
        /*!
        It is possible to update window attributes without refresh.
        So you have to refresh() the window 'manually'.

        For example you can call:
          a) setOpacity(100) and setBgColor(bgcolor)
         or
          b) setOpacity(100, false), setBgColor(bgcolor, false) and then refresh() 
        
        With variant b) you have a better performance because only one refresh() will be done. 
        */
        void refresh();
        
        //! Get access to the surface of the window needed to display video streams.
        /*! 
        \return pointer to the surface class
        */
        MMSFBSurface *getSurface();

        //! Set the window manager.
        /*!
        \param wm	interface to the window manager 
        */
        void setWindowManager(IMMSWindowManager *wm);

        //! Is the window shown?
        /*!
        \param checkparents		if true the parent(s) will be check too 
        \return true, if the window is shown
        */
        bool isShown(bool checkparents = false);

        //! Is the hide action running?
        /*!
        \return true, if the window action thread is going to hide the window
        */
        bool willHide();

        //! Set the focus to this window.
        void setFocus();

        //! Is the window focused?
        /*!
        \return true, if the window is focused
        */
        bool getFocus();

        //! Search a widget with a given name. 
        /*!
        \param name		name of the widget
        \return pointer to the widget which was found or NULL 
        */
        MMSWidget* searchForWidget(string name);

        //! Operator [] which you can use to find a widget. 
        /*!
        \param name		name of the widget
        \return pointer to the widget which was found or NULL
        \see searchForWidget()
        */
        MMSWidget* operator[](string name);

        //! Get the window to which the GUI navigates if the user press up.
        /*!
        \return pointer to the window or NULL
        */
        MMSWindow *getNavigateUpWindow();

        //! Get the window to which the GUI navigates if the user press down.
        /*!
        \return pointer to the window or NULL
        */
        MMSWindow *getNavigateDownWindow();

        //! Get the window to which the GUI navigates if the user press left.
        /*!
        \return pointer to the window or NULL
        */
        MMSWindow *getNavigateLeftWindow();
        
        //! Get the window to which the GUI navigates if the user press right.
        /*!
        \return pointer to the window or NULL
        */
        MMSWindow *getNavigateRightWindow();

        //! Set the window to which the GUI navigates if the user press up.
        /*!
        \param upWindow		pointer to the window or NULL
        */
        void setNavigateUpWindow(MMSWindow *upWindow);

        //! Set the window to which the GUI navigates if the user press down.
        /*!
        \param downWindow	pointer to the window or NULL
        */
        void setNavigateDownWindow(MMSWindow *downWindow);

        //! Set the window to which the GUI navigates if the user press left.
        /*!
        \param leftWindow	pointer to the window or NULL
        */
        void setNavigateLeftWindow(MMSWindow *leftWindow);

        //! Set the window to which the GUI navigates if the user press right.
        /*!
        \param rightWindow	pointer to the window or NULL
        */
        void setNavigateRightWindow(MMSWindow *rightWindow);

        //! Set one or more callbacks for the onBeforeShow event.  
        /*!
        The connected callbacks will be called during show().
        If at least one of the callbacks returns false, the show process of the window
        will be stopped and the window will not appear.

        A callback method must be defined like this:
        
        	bool myclass::mycallbackmethod(MMSWindow *win);
        	
        	Parameters:
        		win 	the pointer to the window which is to be shown
        		
        	Returns:
        		true if the show process should continue, else false if the window should not appear
        		
        To connect your callback to onBeforeShow do this:
        
        	mywindow->onBeforeShow->connect(sigc::mem_fun(myobject,&myclass::mycallbackmethod));
        */
        sigc::signal<bool, MMSWindow*>::accumulated<bool_accumulator> *onBeforeShow;

        //! Set one or more callbacks for the onAfterShow event.  
        /*!
        The connected callbacks will be called during show().

        A callback method must be defined like this:
        
        	void myclass::mycallbackmethod(MMSWindow *win, bool already_shown);
        	
        	Parameters:
        		win 			is the pointer to the window which is shown now
        		already_shown	the window was already shown
        		
        To connect your callback to onAfterShow do this:
        
        	mywindow->onAfterShow->connect(sigc::mem_fun(myobject,&myclass::mycallbackmethod));
        */
        sigc::signal<void, MMSWindow*, bool> *onAfterShow;

        //! Set one or more callbacks for the onBeforeHide event.  
        /*!
        The connected callbacks will be called during hide().
        If at least one of the callbacks returns false, the hide process of the window
        will be stopped and the window will not disappear.

        A callback method must be defined like this:
        
        	bool myclass::mycallbackmethod(MMSWindow *win, bool goback);
        	
        	Parameters:
        		win 	is the pointer to the window which is to be hidden
        		goback	the application can decide, what to do if true/false, see hide()

        	Returns:
        		true if the hide process should continue, else false if the window should not disappear
        		
        To connect your callback to onBeforeHide do this:
        
        	mywindow->onBeforeHide->connect(sigc::mem_fun(myobject,&myclass::mycallbackmethod));
        */
        sigc::signal<bool, MMSWindow*, bool>::accumulated<bool_accumulator> *onBeforeHide;
 
        //! Set one or more callbacks for the onHide event.  
        /*!
        The connected callbacks will be called during hide().

        A callback method must be defined like this:
        
        	void myclass::mycallbackmethod(MMSWindow *win, bool goback);
        	
        	Parameters:
        		win 	is the pointer to the window which is hidden now
        		goback	the application can decide, what to do if true/false, see hide()

        To connect your callback to onHide do this:
        
        	mywindow->onHide->connect(sigc::mem_fun(myobject,&myclass::mycallbackmethod));
        */
        sigc::signal<void, MMSWindow*, bool> *onHide;

        //! Set one or more callbacks for the onHandleInput event.  
        /*!
        The connected callbacks will be called during hide().

        A callback method must be defined like this:
        
        	void myclass::mycallbackmethod(MMSWindow *win, MMSInputEvent *inputevent);
        	
        	Parameters:
        		win 		is the pointer to the window
        		inputevent	the input event

        To connect your callback to onHandleInput do this:
        
        	mywindow->onHandleInput->connect(sigc::mem_fun(myobject,&myclass::mycallbackmethod));
        */
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
    friend class MMSInputManager;
    friend class MMSWindowAction;
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
