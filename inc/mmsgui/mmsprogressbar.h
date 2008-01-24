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

#ifndef MMSPROGRESSBAR_H_
#define MMSPROGRESSBAR_H_

#include "mmsgui/mmswidget.h"

//! With this class you can display a progress bar.
/*!
Shows the progress of any action.
The progressbar widget cannot be focused.
\author Jens Schneider
*/
class MMSProgressBar : public MMSWidget {
	
	private:
        string              className;
        MMSProgressBarClass *progressBarClass;
        MMSProgressBarClass myProgressBarClass;

        bool create(MMSWindow *root, string className, MMSTheme *theme);

	public:
        MMSProgressBar(MMSWindow *root, string className, MMSTheme *theme = NULL);
        ~MMSProgressBar();
        MMSWIDGETTYPE getType() { return MMSWIDGETTYPE_PROGRESSBAR; }

        MMSWidget *copyWidget();

        bool init();
        bool draw(bool *backgroundFilled = NULL);

    public:
        /* theme access methods */
        DFBColor getColor();
        DFBColor getSelColor();
        unsigned int getProgress();

        void setColor(DFBColor color, bool refresh = true);
        void setSelColor(DFBColor selcolor, bool refresh = true);
        void setProgress(unsigned int progress, bool refresh = true);

        void updateFromThemeClass(MMSProgressBarClass *themeClass);
};

#endif /*MMSPROGRESSBAR_H_*/
