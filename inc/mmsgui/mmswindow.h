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

#include "mmswidget.h"
#include "mmswindowaction.h"
#include "mmsimagemanager.h"
#include "mmsfontmanager.h"
#include "mmsfbmanager.h"
#include "interfaces/immswindowmanager.h"
#include "mmsmutex.h"

/*
MMS_CREATEERROR(MMSWindowError);
*/

typedef enum {
	MMSWINDOWTYPE_MAINWINDOW = 0,
    MMSWINDOWTYPE_POPUPWINDOW,
    MMSWINDOWTYPE_ROOTWINDOW,
    MMSWINDOWTYPE_CHILDWINDOW
} MMSWINDOWTYPE;

typedef enum {
    MMSW_NONE               = 0x00000000,
    MMSW_VIDEO              = 0x00000001,
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
        typedef struct {
            MMSWindow       *window;        /* points to the child window */
            DFBRegion       region;         /* region of the window within parent window */
            unsigned char   opacity;        /* opacity of the window */
            unsigned char   oldopacity;     /* old opacity of the window */
            unsigned int    focusedWidget;  /* save the last focused widget here */
        } CHILDWINS;

        static MMSGuiLogger   	logger;

        MMSTheme                *theme;             /* access to the theme which is used */
        MMSWindowClass          *baseWindowClass;
        MMSWindowClass          *windowClass;
        MMSWindowClass          myWindowClass;
		MMSWINDOW_FLAGS			flags;

		MMSMutex      			Lock;               /* to make it thread-safe */
        unsigned long           TID;                /* save the id of the thread which has locked the window */
        unsigned long           Lock_cnt;           /* count the number of times the thread has call lock() */

        bool                    initialized;

        string name;                                /* name of the window */

        MMSWindow               *parent;            /* parent window or NULL */
		
        MMSImageManager         *im;
        MMSFontManager          *fm;

        MMSFBSurface            *bgimage;
        MMSFBSurface            *borderimages[8];
        DFBRectangle            bordergeom[8];
        bool                    bordergeomset;

        MMSFBLayer              *layer;
        MMSFBWindow             *window;
        MMSFBSurface            *surface;

        DFBRectangle 			vrect;				/* visible rectangle on the screen */

        int                     dxpix;
        int                     dypix;
/*        int                     posx;
        int                     posy;
        unsigned int            width;
        unsigned int            height;*/
        DFBRectangle            geom;
        DFBRectangle            innerGeom;

        vector<MMSWidget *>     children;
        MMSWidget 				*focusedwidget;
        bool                    shown;
        bool					willshow;
        bool					willhide;
        bool                    firstfocusset;

        MMSMutex      			drawLock;
        MMSMutex      			flipLock;

        vector<CHILDWINS>       childwins;
        unsigned int            focusedChildWin;

        MMSWidget       *upArrowWidget;
        MMSWidget       *downArrowWidget;
        MMSWidget       *leftArrowWidget;
        MMSWidget       *rightArrowWidget;

        bool            initialArrowsDrawn;

        MMSWindow       *navigateUpWindow;
        MMSWindow       *navigateDownWindow;
        MMSWindow       *navigateLeftWindow;
        MMSWindow       *navigateRightWindow;

        MMSMutex  		preCalcNaviLock;

        
        MMSWidget		*buttonpress_widget;
        MMSWindow		*buttonpress_childwin;

        bool create(string dx, string dy, string w, string h, MMSALIGNMENT alignment, MMSWINDOW_FLAGS flags,
        		    bool *own_surface);
		bool create(string w, string h, MMSALIGNMENT alignment, MMSWINDOW_FLAGS flags,
					bool *own_surface);
        bool resize(bool refresh = true);
        bool addChildWindow(MMSWindow *childwin);
        bool setChildWindowOpacity(MMSWindow *childwin, unsigned char opacity);
        bool setChildWindowRegion(MMSWindow *childwin, bool refresh = true);
        bool moveChildWindow(MMSWindow *childwin, int x, int y, bool refresh = true);
        void drawChildWindows(MMSFBSurface *dst_surface, DFBRegion *region = NULL, int offsX = 0, int offsY = 0);
        bool flipWindow(MMSWindow *win = NULL, DFBRegion *region = NULL,
                        MMSFBSurfaceFlipFlags flags = (MMSFBSurfaceFlipFlags)0,
                        bool flipChildSurface = true, bool locked = false);
        void removeFocusFromChildWindow();

        void loadArrowWidgets();
        void getArrowWidgetStatus(bool *setarrows);
        void switchArrowWidgets();

        virtual bool init();
        virtual void draw(bool toRedrawOnly = false, DFBRectangle *rect2update = NULL);
        void drawMyBorder();
        bool setFirstFocus(bool cw = false);

        double calculateDistGradCode_Up(DFBRectangle currPos, DFBRectangle candPos);
        double calculateDistGradCode_Down(DFBRectangle currPos, DFBRectangle candPos);
        double calculateDistGradCode_Left(DFBRectangle currPos, DFBRectangle candPos);
        double calculateDistGradCode_Right(DFBRectangle currPos, DFBRectangle candPos);
        bool handleNavigationForWidgets(MMSInputEvent *inputevent);
        void removeChildWinFocus();
        bool restoreChildWinFocus();
        bool handleNavigationForChildWins(MMSInputEvent *inputevent);
        void preCalcNavigation();

	protected:
        static class IMMSWindowManager *windowmanager;
        MMSWindowAction         *action;

    public:
        MMSWindow();
        virtual ~MMSWindow(); 
		virtual MMSWINDOWTYPE getType() = 0;

        void lock();
        void unlock();

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
        bool isShown();
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
