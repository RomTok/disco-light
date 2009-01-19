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

#ifndef MMSFBDEV_H_
#define MMSFBDEV_H_

#include "mmsgui/fb/mmsfbbase.h"
#include "mmsgui/fb/fb.h"
#include <termios.h>

// added missing ioctl
#ifndef FBIO_WAITFORVSYNC
#define FBIO_WAITFORVSYNC	_IOW('F', 0x20, u_int32_t)
#endif

#define MMSFBDEV_MAX_MODES	128

class MMSFBDev {
    private:
    	//! is initialized?
    	bool	isinitialized;

        //! name of the device file
        string  device_file;

        //! device abbreviation
        char    device[8];

        //! file descriptor of the framebuffer
        int     fd;

        //! virtual framebuffer address
        void    *framebuffer_base;

        //! have to reset the console acceleration
        bool	reset_console_accel;

		//! saved settings from general terminal interface
        struct termios saved_ts;

        //! fix screen infos
        struct fb_fix_screeninfo    fix_screeninfo;

        //! variable screen infos
        struct fb_var_screeninfo    var_screeninfo;

        //! original variable screen infos
        struct fb_var_screeninfo    org_var_screeninfo;

        //! pixelformat of the screen
        MMSFBSurfacePixelFormat		pixelformat;

        //! available modes read from /etc/fb.modes
        struct fb_var_screeninfo	modes[MMSFBDEV_MAX_MODES];

        //! number of loaded modes
        int modes_cnt;

        void printFixScreenInfo();
        void printVarScreenInfo();
        bool buildPixelFormat();

        bool readModes();

    public:
        MMSFBDev();
        ~MMSFBDev();

        virtual bool openDevice(char *device_file = NULL, int console = -1);
        virtual void closeDevice();
        bool isInitialized();
        void waitForVSync();


        bool getPixelFormat(MMSFBSurfacePixelFormat *pf);
        bool getFrameBufferBase(unsigned char **base);
        bool getFrameBufferPtr(unsigned int number, void **ptr, int *pitch, int *width, int *height);

    private:
        typedef struct {
        	//! /dev/tty0 file descriptor
			int fd0;

			//! file descriptor of the allocated tty
			int fd;

			//! number of vt where disko is running
			int number;

			//! number of vt from which disko was started
			int previous;

			//! save original fb
			int org_fb;
        } VT;

        VT	vt;

        bool vtOpen(int console = -1);
        void vtClose();
        bool vtGetFd(int *fd);

        friend class MMSFBDevVesa;
        friend class MMSFBDevMatrox;
        friend class MMSInputLISHandler;

};

#endif /* MMSFBDEV_H_ */
