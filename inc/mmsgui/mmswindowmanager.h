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

#ifndef MMSWINDOWMANAGER_H_
#define MMSWINDOWMANAGER_H_

#include "mmsgui/interfaces/immswindowmanager.h"
#include "mmstools/mmstools.h"

class MMSWindowManager : public IMMSWindowManager {
	private:
        DFBRectangle        vrect;  // visible screen area

        vector<MMSWindow*>  windows;
        MMSWindow           *toplevel;
        MMSWindow           *backgroundwindow;

	public:
		MMSWindowManager(DFBRectangle vrect);
		virtual ~MMSWindowManager();

        DFBRectangle getVRect();

		void addWindow(MMSWindow *window);
        void removeWindow(MMSWindow *window);
        
        bool hideAllMainWindows(bool goback = false);
        bool hideAllPopupWindows();
        bool hideAllRootWindows(bool willshown = false);

        void setToplevelWindow(MMSWindow *window);
        MMSWindow *getToplevelWindow();
		void removeWindowFromToplevel(MMSWindow *window);

        void setBackgroundWindow(MMSWindow *window);
        MMSWindow *getBackgroundWindow();

        void setPointerPosition(int pointer_posx, int pointer_posy);
};

/* access to global mmswindowmanager */
//extern MMSWindowManager mmswindowmanager;

#endif /*MMSWINDOWMANAGER_H_*/
