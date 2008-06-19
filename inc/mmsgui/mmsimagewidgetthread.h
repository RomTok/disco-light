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

#ifndef MMSIMAGEWIDGETTHREAD_H_
#define MMSIMAGEWIDGETTHREAD_H_

#include "mmsgui/mmsimagewidget.h"
#include <sigc++/sigc++.h>

class MMSImageWidgetThread : public MMSThread {
    private:
        class MMSImageWidget *image;

        bool inWait;
        bool stopThread;
        bool pauseThread;

        void wait(unsigned int delaytime);
        void doIt();
        void threadMain();

    public:
        MMSImageWidgetThread(MMSImageWidget *image);
        void stop();
        void pause(bool pauseThread);
};

#endif /*MMSIMAGEWIDGETTHREAD_H_*/
