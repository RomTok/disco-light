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

#ifndef MMSFBDEVMATROX_H_
#define MMSFBDEVMATROX_H_

#ifdef __HAVE_FBDEV__

#include "mmsgui/fb/mmsfbdev.h"

// matrox registers & flags needed to enable TVOut layer 2
// we do only support g450/g550!
#define C2CTRL						0x3C10
#define C2CTRL_C2EN					0x00000001
#define C2CTRL_C2PIXCLKSEL_CRISTAL	0x00004002
#define C2CTRL_C2PIXCLKDIS			0x00000008
#define C2CTRL_C2DEPTH_YCBCR420		0x00E00000
#define C2CTRL_C2INTERLACE			0x02000000

#define C2HPARAM         			0x3C14
#define C2HSYNC          			0x3C18
#define C2VPARAM         			0x3C1C
#define C2VSYNC          			0x3C20
#define C2PRELOAD        			0x3C24
#define C2PLANE1START0     			0x3C28
#define C2PLANE1START1     			0x3C2C
#define C2PLANE2START0   			0x3C30
#define C2PLANE2START1   			0x3C34
#define C2PLANE3START0   			0x3C38
#define C2PLANE3START1   			0x3C3C
#define C2OFFSET         			0x3C40

#define C2MISC						0x3C44
#define C2VCOUNT					0x3C48
#define C2DATACTRL					0x3C4C
#define C2DATACTRL_C2NTSCEN			0x00000010

#define DAC_INDEX					0x3C00
#define DAC_DATA					0x3C0A

#define XGENIOCTRL					0x2A
#define XGENIODATA					0x2B

#define XDISPCTRL					0x8A
#define XDISPCTRL_DAC2OUTSEL_DIS	0x00
#define XDISPCTRL_DAC2OUTSEL_TVE	0x0C
#define XDISPCTRL_DAC2OUTSEL_MASK	0x0C

#define XSYNCCTRL					0x8B
#define XSYNCCTRL_DAC2HSOFF			0x10
#define XSYNCCTRL_DAC2VSOFF			0x20
#define XSYNCCTRL_DAC2HSPOL			0x40
#define XSYNCCTRL_DAC2VSPOL			0x80

#define XPWRCTRL					0xA0
#define XPWRCTRL_DAC2PDN			0x01
#define XPWRCTRL_CFIFOPDN			0x10


#define mga_out8(mmioaddr, val, reg) *((volatile unsigned char*)(((volatile unsigned char *)mmioaddr) + reg)) = val;

#define mga_out32(mmioaddr, val, reg) *((volatile unsigned int*)(((volatile unsigned char *)mmioaddr) + reg)) = val;

#define mga_in8(mmioaddr, reg) *((volatile unsigned char*)(((volatile unsigned char*)mmioaddr)+reg))

#define mga_in32(mmioaddr, reg) *((volatile unsigned int*)(((volatile unsigned char*)mmioaddr)+reg))

#define mga_out_dac(mmioaddr, val, reg) \
	mga_out8(mmioaddr, reg, DAC_INDEX); \
	mga_out8(mmioaddr, val, DAC_DATA);

#define mga_in_dac(mmioaddr, reg) \
	mga_out8(mmioaddr, reg, DAC_INDEX); \
	mga_in8(mmioaddr, DAC_DATA)

#define maven_out8(mmioaddr, val, reg) \
	mga_out_dac(mmioaddr, reg, 0x87); \
	mga_out_dac(mmioaddr, val, 0x88);



class MMSFBDevMatrox : public MMSFBDev {
    private:
    	bool scart_rgb_cable;
    	bool tv_std_pal;

    	unsigned char *mmio_base;

		// crtc2 registers
		struct {
			unsigned int c2ctrl;
			unsigned int c2datactrl;
			unsigned int c2misc;
			unsigned int c2offset;
			unsigned int c2hparam;
			unsigned int c2vparam;
			unsigned int c2_plane1_start0;
			unsigned int c2_plane1_start1;
			unsigned int c2_plane2_start0;
			unsigned int c2_plane2_start1;
			unsigned int c2_plane3_start0;
			unsigned int c2_plane3_start1;
		} crtc2_regs;


    	void buildCRTC2Regs();
    	void setCRTC2Regs();

    	void buildCRTC2Buffer();
    	void setCRTC2Buffer();

    	void switchCRTC2(bool on);
    	bool enableCRTC2();
    	bool disableCRTC2();

    	void setMavenRegs();
    	void enableMaven();
    	void disableMaven();

    public:
        MMSFBDevMatrox();
        virtual ~MMSFBDevMatrox();

        bool openDevice(char *device_file = NULL, int console = -1);
        void closeDevice();

        bool testLayer(int layer_id);
        bool initLayer(int layer_id, int width, int height, MMSFBSurfacePixelFormat pixelformat);

        bool getFrameBufferPtr(int layer_id, void **ptr, int *pitch, int *width, int *height);
};

#endif

#endif /* MMSFBDEVMATROX_H_ */
