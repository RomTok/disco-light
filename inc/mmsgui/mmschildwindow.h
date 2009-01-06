/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#ifndef MMSCHILDWINDOW_H_
#define MMSCHILDWINDOW_H_

#include "mmsgui/mmswindow.h"

class MMSChildWindow : public MMSWindow {
    private:
        string              className;
        MMSChildWindowClass *childWindowClass;
        MMSChildWindowClass myChildWindowClass;

        bool create(string className, MMSWindow *parent,
                    string dx, string dy, string w, string h, MMSALIGNMENT alignment,
                    MMSWINDOW_FLAGS flags, MMSTheme *theme, bool *own_surface);

    public:
        MMSChildWindow(string className, MMSWindow *parent,
                       string dx, string dy, string w, string h, MMSALIGNMENT alignment = MMSALIGNMENT_CENTER,
                       MMSWINDOW_FLAGS flags = MMSW_NONE, MMSTheme *theme = NULL, bool *own_surface = NULL);
        MMSChildWindow(string className, MMSWindow *parent,
                       string w, string h, MMSALIGNMENT alignment = MMSALIGNMENT_CENTER,
                       MMSWINDOW_FLAGS flags = MMSW_NONE, MMSTheme *theme = NULL, bool *own_surface = NULL);
        virtual ~MMSChildWindow();

    public:
        /* begin of theme access methods *************************************/

    	//! Detect if the child window has to be modal if it is focused.
        /*!
        \param modal	returns the modal status
        \return true, if modal is successfully returned
        */
        bool getModal(bool &modal);

        //! Set the modal status of the child window.
        /*!
        \param modal	if true, the child window is marked as modal
        */
        void setModal(bool modal);

        //! Update the current window settings with settings from another theme class.
        /*!
        \param themeClass	source theme class
        \note Only this parameters which are set within the source theme class will be updated.
        */
        void updateFromThemeClass(MMSChildWindowClass *themeClass);
};

#endif /*MMSCHILDWINDOW_H_*/
