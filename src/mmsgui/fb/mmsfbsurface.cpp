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

#include "mmsgui/fb/mmsfbsurface.h"
#include "mmsgui/fb/mmsfb.h"
#include "mmsgui/fb/mmsfbsurfacemanager.h"
#include <math.h>


D_DEBUG_DOMAIN( MMS_Surface, "MMS/Surface", "MMS FB Surface" );


#define INITCHECK  if((!mmsfb->isInitialized())||(!this->dfbsurface)){MMSFB_SetError(0,"not initialized");return false;}

MMSFBSurface::MMSFBSurface(IDirectFBSurface *dfbsurface, IDirectFBSurface *parent_dfbsurface,
						   DFBRectangle *sub_surface_rect) {
    /* init me */
    this->dfbsurface = dfbsurface;
    this->TID = 0;
    this->Lock_cnt = 0;
    this->parent_dfbsurface = parent_dfbsurface;
    if (this->parent_dfbsurface) {
    	this->is_sub_surface = true;
    	this->sub_surface_rect = *sub_surface_rect;
    }
    else {
    	this->is_sub_surface = false;
    	this->sub_surface_rect.x = 0;
    	this->sub_surface_rect.y = 0;
    	this->sub_surface_rect.w = 0;
    	this->sub_surface_rect.h = 0;
    }
    	
    /* get the current config */
    if (this->dfbsurface) {
        getConfiguration();

        /* init the color */
        this->config.color.r = 0;
        this->config.color.g = 0;
        this->config.color.b = 0;
        this->config.color.a = 0;
        this->config.iswinsurface = false;
        this->config.islayersurface = false;
        this->config.blittingflags = DSBLIT_NOFX;
    }
}

MMSFBSurface::~MMSFBSurface() {

    if (!mmsfb->isInitialized()) return;

    /* release memory - only if not the layer surface */
    if (this->dfbsurface)
        if (!this->config.islayersurface)
//            this->dfbsurface->Release(this->dfbsurface);
        	if (!this->is_sub_surface)
        		mmsfbsurfacemanager->releaseSurface(this);
        	else
        		this->dfbsurface->Release(this->dfbsurface);
}

IDirectFBSurface *MMSFBSurface::getDFBSurface() {
    return this->dfbsurface;
}

bool MMSFBSurface::getConfiguration(MMSFBSurfaceConfig *config) {
    DFBResult               dfbres;
    DFBSurfacePixelFormat   mypf;
    DFBSurfaceCapabilities  caps;

    /* check if initialized */
    INITCHECK;

    /* get size */
    if ((dfbres=this->dfbsurface->GetSize(this->dfbsurface, &(this->config.w), &(this->config.h))) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetSize() failed");
        return false;
    }

    /* get pixelformat */
    if ((dfbres=this->dfbsurface->GetPixelFormat(this->dfbsurface, &mypf)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetPixelFormat() failed");
        return false;
    }

    /* build a format string */
    this->config.pixelformat = getDFBPixelFormatString(mypf);
    this->config.alphachannel = isAlphaPixelFormat(this->config.pixelformat);

    /* get capabilities */
    if ((dfbres=this->dfbsurface->GetCapabilities(this->dfbsurface, &caps)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetCapabilities() failed");
        return false;
    }

    /* get the buffer mode */
    this->config.backbuffer = 0;
    if (caps & DSCAPS_DOUBLE)
        this->config.backbuffer = 1;
    else
    if (caps & DSCAPS_TRIPLE)
        this->config.backbuffer = 2;

    /* fill return config */
    if (config)
        *config = this->config;

    return true;
}

bool MMSFBSurface::isWinSurface() {
    return this->config.iswinsurface;
}

bool MMSFBSurface::isLayerSurface() {
    return this->config.islayersurface;
}

bool MMSFBSurface::setWinSurface(bool iswinsurface) {

    /* check if initialized */
    INITCHECK;

    /* set the flag */
    this->config.iswinsurface = iswinsurface;

    return true;
}

bool MMSFBSurface::setLayerSurface(bool islayersurface) {

    /* check if initialized */
    INITCHECK;

    /* set the flag */
    this->config.islayersurface = islayersurface;

    return true;
}


bool MMSFBSurface::getPixelFormat(string *pixelformat) {

    /* check if initialized */
    INITCHECK;

    /* return the pixelformat */
    *pixelformat = config.pixelformat;

    return true;
}

bool MMSFBSurface::getSize(int *w, int *h) {

    /* check if initialized */
    INITCHECK;

    /* return values */
    *w = config.w;
    *h = config.h;

    return true;
}

bool MMSFBSurface::clear(unsigned char r, unsigned char g,
                         unsigned char b, unsigned char a) {
    DFBResult   dfbres;

    D_DEBUG_AT( MMS_Surface, "clear( argb %02x %02x %02x %02x ) <- %dx%d\n",
                a, r, g, b, this->config.w, this->config.h );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

#ifdef MMSGUI_STDOUT_TRACE
printf("  clear>>>, %x - %d,%d,%d,%d - tid=%u\n", this, r,g,b,a,  pthread_self());
#endif

    /* save color and set it to given values */
    MMSFBColor mc = this->config.color;
    setColor(r,g,b,a);

    /* clear surface */
    if ((dfbres=this->dfbsurface->Clear(this->dfbsurface, r, g, b, a)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::Clear() failed");
        return false;
    }

    /* reset color */
    setColor(mc.r,mc.g,mc.b,mc.a);

#ifdef MMSGUI_STDOUT_TRACE
printf("  <<<clear, %x - %d,%d,%d,%d - tid=%u\n", this, r,g,b,a,  pthread_self());
#endif

    return true;
}

bool MMSFBSurface::setColor(unsigned char r, unsigned char g,
                            unsigned char b, unsigned char a) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* set color */
    if ((dfbres=this->dfbsurface->SetColor(this->dfbsurface, r, g, b, a)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::SetColor() failed");
        return false;
    }

    /* save the color */
    this->config.color.r = r;
    this->config.color.g = g;
    this->config.color.b = b;
    this->config.color.a = a;

    return true;
}

bool MMSFBSurface::getColor(MMSFBColor *color) {

    /* check if initialized */
    INITCHECK;

    /* return the color */
    *color = this->config.color;
    
    return true;
}

bool MMSFBSurface::setClip(DFBRegion *clip) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

#ifdef MMSGUI_STDOUT_TRACE
if (clip)
printf("  setClip>>>, %x - %d,%d,%d,%d - tid=%u\n", this, clip->x1, clip->y1, clip->x2, clip->y2,  pthread_self());
else
printf("  setClip>>>, %x - tid=%u\n", this,  pthread_self());
#endif

    /* set color */
    if ((dfbres=this->dfbsurface->SetClip(this->dfbsurface, clip)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::SetClip() failed");
        return false;
    }

#ifdef MMSGUI_STDOUT_TRACE
if (clip)
printf("  <<<setClip, %x - %d,%d,%d,%d - tid=%u\n", this, clip->x1, clip->y1, clip->x2, clip->y2,  pthread_self());
else
printf("  <<<setClip, %x - tid=%u\n", this,  pthread_self());
#endif

    return true;
}

bool MMSFBSurface::setDrawingFlags(MMSFBSurfaceDrawingFlags flags) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* set the drawing flags */
    if ((dfbres=this->dfbsurface->SetDrawingFlags(this->dfbsurface, (DFBSurfaceDrawingFlags) flags)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::SetDrawingFlags() failed");
        return false;
    }
    return true;
}

bool MMSFBSurface::drawLine(int x1, int y1, int x2, int y2) {
    DFBResult   dfbres;

    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* draw a line */
    if ((dfbres=this->dfbsurface->DrawLine(this->dfbsurface, x1, y1, x2, y2)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::DrawLine() failed");
        return false;
    }
    return true;
}

bool MMSFBSurface::drawRectangle(int x, int y, int w, int h) {
    DFBResult   dfbres;

    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* draw rectangle */
    if ((dfbres=this->dfbsurface->DrawRectangle(this->dfbsurface, x, y, w, h)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::DrawRectangle() failed");
        return false;
    }
    return true;
}

bool MMSFBSurface::fillRectangle(int x, int y, int w, int h) {
    DFBResult   dfbres;

    D_DEBUG_AT( MMS_Surface, "fill( %d,%d - %dx%d ) <- %dx%d, %02x %02x %02x %02x\n",
                x, y, w, h, this->config.w, this->config.h,
                this->config.color.a, this->config.color.r, this->config.color.g, this->config.color.b );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

    /* fill rectangle */
    if ((dfbres=this->dfbsurface->FillRectangle(this->dfbsurface, x, y, w, h)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::FillRectangle() failed");
        return false;
    }
    return true;
}

bool MMSFBSurface::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* draw triangle */
    drawLine(x1, y1, x2, y2);
    drawLine(x1, y1, x3, y3);
    drawLine(x2, y2, x3, y3);

    return true;
}

bool MMSFBSurface::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    DFBResult   dfbres;

    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* fill rectangle */
    if ((dfbres=this->dfbsurface->FillTriangle(this->dfbsurface, x1, y1, x2, y2, x3, y3)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::FillTriangle() failed");
        return false;
    }
    return true;
}

bool MMSFBSurface::drawCircle(int x, int y, int radius, int start_octant, int end_octant) {

    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* draw circle */
    if (end_octant < start_octant) end_octant = start_octant;
    if ((start_octant<=4)&&(end_octant>=3))
        drawLine(x, y + radius, x, y + radius);
    if ((start_octant==0)||(end_octant==7))
        drawLine(x, y - radius, x, y - radius);
    if ((start_octant<=2)&&(end_octant>=1))
        drawLine(x + radius, y, x + radius, y);
    if ((start_octant<=6)&&(end_octant>=5))
        drawLine(x - radius, y, x - radius, y);
    int mr = radius * radius;
    int mx = 1;
    int my = (int) (sqrt(mr - 1) + 0.5);

    while (mx < my) {
        if ((start_octant<=0)&&(end_octant>=0))
            drawLine(x + mx, y - my, x + mx, y - my); /* octant 0 */
        if ((start_octant<=1)&&(end_octant>=1))
            drawLine(x + my, y - mx, x + my, y - mx); /* octant 1 */
        if ((start_octant<=2)&&(end_octant>=2))
            drawLine(x + my, y + mx, x + my, y + mx); /* octant 2 */
        if ((start_octant<=3)&&(end_octant>=3))
            drawLine(x + mx, y + my, x + mx, y + my); /* octant 3 */
        if ((start_octant<=4)&&(end_octant>=4))
            drawLine(x - mx, y + my, x - mx, y + my); /* octant 4 */
        if ((start_octant<=5)&&(end_octant>=5))
            drawLine(x - my, y + mx, x - my, y + mx); /* octant 5 */
        if ((start_octant<=6)&&(end_octant>=6))
            drawLine(x - my, y - mx, x - my, y - mx); /* octant 6 */
        if ((start_octant<=7)&&(end_octant>=7))
            drawLine(x - mx, y - my, x - mx, y - my); /* octant 7 */

        mx++;
        my = (int) (sqrt(mr - mx*mx) + 0.5);
    }

    if (mx == my) {
        if ((start_octant<=3)&&(end_octant>=2))
            drawLine(x + mx, y + my, x + mx, y + my);
        if ((start_octant<=1)&&(end_octant>=0))
            drawLine(x + mx, y - my, x + mx, y - my);
        if ((start_octant<=5)&&(end_octant>=4))
            drawLine(x - mx, y + my, x - mx, y + my);
        if ((start_octant<=7)&&(end_octant>=6))
            drawLine(x - mx, y - my, x - mx, y - my);
    }

    return true;
}



bool MMSFBSurface::setBlittingFlags(MMSFBSurfaceBlittingFlags flags) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

#ifdef MMSGUI_STDOUT_TRACE
printf("blitfl>>>,  %x - tid=%u\n", this,  pthread_self());
#endif

    if ((flags & DSBLIT_BLEND_ALPHACHANNEL)||(flags & DSBLIT_BLEND_COLORALPHA)) {
        /* if we do alpha channel blitting, we have to change the default settings to become correct results */
        dfbsurface->SetSrcBlendFunction(dfbsurface,(DFBSurfaceBlendFunction)(DSBF_ONE/*DSBF_DESTALPHA*/));
        dfbsurface->SetDstBlendFunction(dfbsurface,(DFBSurfaceBlendFunction)(DSBF_INVSRCALPHA));

        if (flags & DSBLIT_BLEND_COLORALPHA)
             flags = (MMSFBSurfaceBlittingFlags)(flags | DSBLIT_SRC_PREMULTCOLOR);
    }
    else {
        /* that's the default for no alpha channel blitting */
        //dfbsurface->SetSrcBlendFunction(dfbsurface,(DFBSurfaceBlendFunction)(DSBF_SRCALPHA));
        //dfbsurface->SetDstBlendFunction(dfbsurface,(DFBSurfaceBlendFunction)(DSBF_INVSRCALPHA));
    }

    /* set the blitting flags */
    if ((dfbres=this->dfbsurface->SetBlittingFlags(this->dfbsurface, (DFBSurfaceBlittingFlags) flags)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::SetBlittingFlags() failed");

        return false;
    }

    /* save the flags */
    this->config.blittingflags = flags;    

    return true;
}

bool MMSFBSurface::blit(MMSFBSurface *source, DFBRectangle *src_rect, int x, int y) {
    DFBResult    dfbres;
    DFBRectangle src;

    if (src_rect) {
         src = *src_rect;
    }
    else {
         src.x = 0;
         src.y = 0;
         src.w = source->config.w;
         src.h = source->config.h;
    }

    D_DEBUG_AT( MMS_Surface, "blit( %d,%d - %dx%d -> %d,%d ) <- %dx%d\n",
                DFB_RECTANGLE_VALS(&src), x, y, this->config.w, this->config.h );

    MMSFB_TRACE();


    /* check if initialized */
    INITCHECK;

    /* lock the source */
    //source->lock();

#ifdef MMSGUI_STDOUT_TRACE
printf("  blit>>>,  %x,%x - %dx%d - tid=%u\n", this, source, (src_rect)?src_rect->w:0, (src_rect)?src_rect->h:0,  pthread_self());
#endif

    /* stretch blit */
    if ((dfbres=this->dfbsurface->Blit(this->dfbsurface, source->getDFBSurface(), &src, x, y)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::Blit() failed, src="
        		               +iToStr(src.x)+","+iToStr(src.y)+","+iToStr(src.w)+","+iToStr(src.h)
        		               +", dst="+iToStr(x)+","+iToStr(y));

        //source->unlock();
        return false;
    }

#ifdef MMSGUI_STDOUT_TRACE
printf("    <<<blit,%x,%x - %dx%d - tid=%u\n", this, source, (src_rect)?src_rect->w:0, (src_rect)?src_rect->h:0,  pthread_self());
#endif

    /* unlock the source */
    //source->unlock();

    return true;
}


bool MMSFBSurface::stretchBlit(MMSFBSurface *source, DFBRectangle *src_rect, DFBRectangle *dest_rect) {
    DFBResult    dfbres;
    DFBRectangle src;
    DFBRectangle dst;
    bool         blit_done = false;

    if (src_rect) {
         src = *src_rect;
    }
    else {
         src.x = 0;
         src.y = 0;
         src.w = source->config.w;
         src.h = source->config.h;
    }

    if (dest_rect) {
         dst = *dest_rect;
    }
    else {
         dst.x = 0;
         dst.y = 0;
         dst.w = this->config.w;
         dst.h = this->config.h;
    }

    D_DEBUG_AT( MMS_Surface, "stretchBlit( %d,%d - %dx%d  ->  %d,%d - %dx%d ) <- %dx%d\n",
                DFB_RECTANGLE_VALS(&src), DFB_RECTANGLE_VALS(&dst), this->config.w, this->config.h );

    /* check if i can blit without stretching */    
    if (src.w == dst.w && src.h == dst.h)
        return blit(source, &src, dst.x, dst.y);


    MMSFB_BREAK();

    /* check if initialized */
    INITCHECK;

    /* lock the source */
    //source->lock();

    if (this->config.blittingflags != DSBLIT_NOFX) {
	    /* stretch blit with blitting flags */

    	/* we use a temporary surface to separate the stretchblit from the extra blitting functions */
    	MMSFBSurface *tempsuf = mmsfbsurfacemanager->getTemporarySurface(dst.w, dst.h);
    	
    	if (tempsuf) {
	    	DFBRectangle temp;
	    	temp.x=0;
	    	temp.y=0;
	    	temp.w=dst.w;
	    	temp.h=dst.h;

	    	if ((dfbres=tempsuf->getDFBSurface()->StretchBlit(tempsuf->getDFBSurface(), source->getDFBSurface(), &src, &temp)) == DFB_OK)
	    		if ((dfbres=this->dfbsurface->Blit(this->dfbsurface, tempsuf->getDFBSurface(), &temp, dst.x, dst.y)) == DFB_OK)
	    			blit_done = true;
	    	
	    	mmsfbsurfacemanager->releaseTemporarySurface(tempsuf);
    	}
    }

    if (!blit_done) {
        /* normal stretch blit */
        if ((dfbres=this->dfbsurface->StretchBlit(this->dfbsurface, source->getDFBSurface(), &src, &dst)) != DFB_OK) {
            MMSFB_SetError(dfbres, "IDirectFBSurface::StretchBlit() failed");
            //source->unlock();
            return false;
        }
    }
    
    /* unlock the source */
    //source->unlock();

    return true;
}

bool MMSFBSurface::flip(DFBRegion *region, MMSFBSurfaceFlipFlags flags) {
    DFBResult   dfbres;

    if (region)
         D_DEBUG_AT( MMS_Surface, "flip( %d,%d - %dx%d ) <- %dx%d\n",
                     DFB_RECTANGLE_VALS_FROM_REGION(region), this->config.w, this->config.h );
    else
         D_DEBUG_AT( MMS_Surface, "flip( %d,%d - %dx%d ) <- %dx%d\n",
                     0, 0, this->config.w, this->config.h, this->config.w, this->config.h );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

#ifdef MMSGUI_STDOUT_TRACE
if (region)
printf("  flip>>>,  %x - %d,%d,%d,%d - tid=%u\n", this, region->x1, region->y1, region->x2, region->y2, pthread_self());
else
printf("  flip>>>,  %x - tid=%u\n", this, pthread_self());
#endif

#ifdef USE_DFB_WINMAN

    /* flip */
    if ((dfbres=this->dfbsurface->Flip(this->dfbsurface, region, flags)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::Flip() failed");

        return false;
    }

#endif

#ifdef USE_MMSFB_WINMAN

    if (!this->config.islayersurface) {
        /* flip */
        if ((dfbres=this->dfbsurface->Flip(this->dfbsurface, region, flags)) != DFB_OK) {
            MMSFB_SetError(dfbres, "IDirectFBSurface::Flip() failed");

            return false;
        }
    }

    if (this->config.iswinsurface) {
        /* inform the window manager */
        mmsfbwindowmanager->flipSurface(this, region, flags);
    }

#endif

#ifdef MMSGUI_STDOUT_TRACE
printf("  <<<flip,  %x - tid=%u\n", this, pthread_self());
#endif

    return true;
}


bool MMSFBSurface::createCopy(MMSFBSurface **dstsurface, int w, int h,
                              bool copycontent, bool withbackbuffer) {

    D_DEBUG_AT( MMS_Surface, "createCopy( %dx%d -> %dx%d, %s, %s )\n",
                this->config.w, this->config.h, w, h,
                copycontent ? "CONTENT COPY!" : "no content copy",
                withbackbuffer ? "BACK BUFFER!" : "no back buffer" );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

    *dstsurface = NULL;

    if (!w) w = config.w;
    if (!h) h = config.h;

    /* create new surface */
    if (!mmsfb->createSurface(dstsurface, w, h, this->config.pixelformat,
                             (withbackbuffer)?this->config.backbuffer:0)) {
        if (*dstsurface)
            delete *dstsurface;
        *dstsurface = NULL;
        return false;
    }

    if (copycontent) {
        /* copy the content */
        DFBRectangle dstrect;
        dstrect.x = 0;
        dstrect.y = 0;
        dstrect.w = w;
        dstrect.h = h;
        (*dstsurface)->setBlittingFlags((MMSFBSurfaceBlittingFlags) DSBLIT_NOFX);
        (*dstsurface)->stretchBlit(this, NULL, &dstrect);
        if (withbackbuffer) {
            (*dstsurface)->flip();
        }
    }

    return true;
}

bool MMSFBSurface::resize(int w, int h) {

    D_DEBUG_AT( MMS_Surface, "resize( %dx%d -> %dx%d )\n",
                this->config.w, this->config.h, w, h );

    MMSFB_TRACE();

    /* check if initialized */
    INITCHECK;

    if (!this->is_sub_surface) {
        /* normal surface */
	    lock();

	    /* create a copy */
	    MMSFBSurface *dstsurface;
	    createCopy(&dstsurface, w, h, true, true);
	
	    /* move the dfb pointers */
	    IDirectFBSurface *s;
	    s = this->dfbsurface;
	    this->dfbsurface = dstsurface->dfbsurface;
	    dstsurface->dfbsurface = s;
	
	    /* load the new configuration */
	    this->getConfiguration();
	    dstsurface->getConfiguration();
	
	    /* free dstsurface */
	    delete dstsurface;
	
	    unlock();
	    return true;
    }
    else  {
    	/* sub surface */
	    DFBRectangle rect = this->sub_surface_rect;
	    rect.w = w;
	    rect.h = h;
	    return setSubSurface(&rect);
	    
    }
}



void MMSFBSurface::modulateBrightness(DFBColor *color, unsigned char brightness) {

    /* full brightness? */
    if (brightness == 255) return;

    /* full darkness? */
    if (brightness == 0) {
        color->r = 0;
        color->g = 0;
        color->b = 0;
        return;
    }
    
    /* modulate the color */
    unsigned int bn = 100000 * (255-brightness);
    if (color->r > 0) {
        unsigned int i = (10000 * 255) / (unsigned int)color->r;
        color->r = (5+((10 * (unsigned int)color->r) - (bn / i))) / 10;
    }
    if (color->g > 0) {
        unsigned int i = (10000 * 255) / (unsigned int)color->g;
        color->g = (5+((10 * (unsigned int)color->g) - (bn / i))) / 10;
    }
    if (color->b > 0) {
        unsigned int i = (10000 * 255) / (unsigned int)color->b;
        color->b = (5+((10 * (unsigned int)color->b) - (bn / i))) / 10;
    }
}

void MMSFBSurface::modulateOpacity(DFBColor *color, unsigned char opacity) {

    /* full opacity? */
    if (opacity == 255) return;

    /* complete transparent? */
    if (opacity == 0) {
        color->a = 0;
        return;
    }
    
    /* modulate the alpha value */
    unsigned int bn = 100000 * (255-opacity);
    if (color->a > 0) {
        unsigned int i = (10000 * 255) / (unsigned int)color->a;
        color->a = (5+((10 * (unsigned int)color->a) - (bn / i))) / 10;
    }
}


bool MMSFBSurface::setBlittingFlagsByBrightnessAlphaAndOpacity(
                    unsigned char brightness, unsigned char alpha, unsigned char opacity) {
    DFBColor color;

    /* check if initialized */
    INITCHECK;

    /* modulate the opacity into the color */
    color.a = alpha;
    modulateOpacity(&color, opacity);

    /* set color for blitting */
    setColor(brightness, brightness, brightness, color.a);

    /* set blitting flags */
    if (brightness != 255) {
        if (color.a == 255)
            setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_COLORIZE|DSBLIT_BLEND_ALPHACHANNEL));
        else
            setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_COLORIZE|DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA));
    }
    else {
        if (color.a == 255)
            setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL));
        else
            setBlittingFlags((MMSFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL|DSBLIT_BLEND_COLORALPHA));
    }
    
    return true;
}

bool MMSFBSurface::setDrawingFlagsByAlpha(unsigned char alpha) {

    /* check if initialized */
    INITCHECK;

    /* set the drawing flags */
    if (alpha == 255)
        setDrawingFlags((MMSFBSurfaceDrawingFlags)DSDRAW_NOFX);
    else
        setDrawingFlags((MMSFBSurfaceDrawingFlags)DSDRAW_BLEND);

    return true;
}


bool MMSFBSurface::setDrawingColorAndFlagsByBrightnessAndOpacity(
                        DFBColor color, unsigned char brightness, unsigned char opacity) {

    /* check if initialized */
    INITCHECK;

    /* modulate the brightness into the color */
    modulateBrightness(&color, brightness);

    /* modulate the opacity into the color */
    modulateOpacity(&color, opacity);

    /* set the color for drawing */
    setColor(color.r, color.g, color.b, color.a);

    /* set the drawing flags */
    setDrawingFlagsByAlpha(color.a);
    
    return true;
}

bool MMSFBSurface::setFont(IDirectFBFont *font) {
    DFBResult   dfbres;

    /* check if initialized */
    INITCHECK;

    /* set font */
    if ((dfbres=this->dfbsurface->SetFont(this->dfbsurface, font)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::SetFont() failed");
        return false;
    }
    return true;
}

bool MMSFBSurface::drawString(string text, int len, int x, int y) {
    DFBResult   dfbres;

    D_DEBUG_AT( MMS_Surface, "drawString( '%s', %d, %d,%d ) <- %dx%d\n",
                text.c_str(), len, x, y, this->config.w, this->config.h );

    MMSFB_TRACE();


    /* check if initialized */
    INITCHECK;

    /* set font */
    if ((dfbres=this->dfbsurface->DrawString(this->dfbsurface, text.c_str(), len, x, y, DSTF_TOPLEFT)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::DrawString() failed");

        return false;
    }

    return true;
}

void MMSFBSurface::lock() {

#ifdef MMSGUI_STDOUT_TRACE
printf("lock>>>,  %x - tid=%u\n", this, pthread_self());
fflush(stdout);
#endif
    if (this->Lock.trylock() == 0) {
        /* I have got the lock the first time */
        this->TID = pthread_self();
        this->Lock_cnt = 1;
    }
    else {
        if ((this->TID == pthread_self())&&(this->Lock_cnt > 0)) {
            /* I am the thread which has already locked this surface */
            this->Lock_cnt++;
        }
        else {
            /* another thread has already locked this surface, waiting for... */
            Lock.lock();
            this->TID = pthread_self();
            this->Lock_cnt = 1;
        }
    }

#ifdef MMSGUI_STDOUT_TRACE
printf("<<<lock,  %x - tid=%u,%d\n", this, pthread_self(),this->Lock_cnt);
fflush(stdout);
#endif
}

void MMSFBSurface::unlock() {
#ifdef MMSGUI_STDOUT_TRACE
printf("unlock>>>,  %x - tid=%u\n", this, pthread_self());
fflush(stdout);
#endif
    if (this->TID != pthread_self())
        return;
    
    if(this->Lock_cnt==0) 
    	return;
    	
    this->Lock_cnt--;

    if (this->Lock_cnt == 0)
        Lock.unlock();

#ifdef MMSGUI_STDOUT_TRACE
printf("<<<unlock,  %x - tid=%u\n", this, pthread_self());
fflush(stdout);
#endif
}


MMSFBSurface *MMSFBSurface::getSubSurface(DFBRectangle *rect) {
    DFBResult   		dfbres;
    IDirectFBSurface    *subsuf;
    MMSFBSurface 		*surface;

    /* check if initialized */
    INITCHECK;

    /* get a sub surface */
    if ((dfbres=this->dfbsurface->GetSubSurface(this->dfbsurface, rect, &subsuf)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetSubSurface() failed");
        return false;
    }

    /* create a new surface instance */
    surface = new MMSFBSurface(subsuf, this->dfbsurface, rect);
    if (!surface) {
        subsuf->Release(subsuf);
        MMSFB_SetError(0, "cannot create new instance of MMSFBSurface");
        return NULL;
    }
    
    return surface;
}

bool MMSFBSurface::setSubSurface(DFBRectangle *rect) {
    DFBResult   		dfbres;
    IDirectFBSurface    *subsuf;

	/* check if initialized */
    INITCHECK;

    /* only sub surfaces can be moved */
    if (!this->is_sub_surface)
		return false;

    lock();
    
    if (memcmp(rect, &(this->sub_surface_rect), sizeof(this->sub_surface_rect)) == 0) {
    	/* nothing changed */
    	unlock();
    	return false;
    }

    /* because dfb has no IDirectFBSurface::setSubSurface(), allocate a new and release the old one */
    if ((dfbres=this->parent_dfbsurface->GetSubSurface(this->parent_dfbsurface, rect, &subsuf)) != DFB_OK) {
        MMSFB_SetError(dfbres, "IDirectFBSurface::GetSubSurface() failed");
        unlock();
        return false;
    }

    if (this->dfbsurface)
    	this->dfbsurface->Release(this->dfbsurface);

    this->dfbsurface = subsuf;
    this->sub_surface_rect = *rect;
    
    unlock();

    return true;
}

bool MMSFBSurface::setSubSurface(DFBRegion *region) {
	DFBRectangle rect;
	
	if (!region)
		return false;
	
	rect.x = region->x1;
	rect.y = region->y1;
	rect.w = region->x2 - region->x1 + 1;
	rect.h = region->y2 - region->y1 + 1;

	return setSubSurface(&rect);
}

bool MMSFBSurface::moveTo(int x, int y) {
	DFBRectangle rect;
	
	rect = this->sub_surface_rect;
	rect.x = x;
	rect.y = y;
	
	return setSubSurface(&rect);
}

