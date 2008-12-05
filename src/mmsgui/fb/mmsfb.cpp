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

#include "mmsgui/fb/mmsfb.h"
#include "mmsgui/fb/mmsfbsurfacemanager.h"
#include <string.h>

/* initialize the mmsfb object */
MMSFB *mmsfb = new MMSFB();


#define INITCHECK  if(!this->initialized){MMSFB_SetError(0,"not initialized");return false;}


MMSFB::MMSFB() {
    /* init me */
    this->argc = 0;
    this->argv = NULL;
    this->initialized = false;
#ifdef  __HAVE_DIRECTFB__
    this->dfb = NULL;
#endif
    this->outputtype = "";
    this->w = 0;
    this->h = 0;

}

MMSFB::~MMSFB() {
}

bool MMSFB::init(int argc, char **argv, string outputtype, int w, int h, bool extendedaccel, bool fullscreen,
				 string appl_name, string appl_icon_name) {

#ifdef __HAVE_XLIB__
	XInitThreads();
    this->resized=false;
#endif

    // check if already initialized
    if (this->initialized) {
        MMSFB_SetError(0, "already initialized");
        return false;
    }

    // save arguments
    this->argc = argc;
    this->argv = argv;
    // init layer pointers
    memset(this->layer, 0, sizeof(MMSFBLayer *) * MMSFBLAYER_MAXNUM);

    // basic information mainly needed by X11 initialization
    this->outputtype = outputtype;
    this->w = w;
    this->h = h;

    // which backend should i use?
    this->backend = MMSFB_BACKEND_DFB;
#ifdef __HAVE_XLIB__
    if ((this->outputtype == MMS_OT_X11FB)&&(extendedaccel))
    	this->backend = MMSFB_BACKEND_X11;
#endif

    if (this->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
        DFBResult dfbres;

        /* init dfb */
		DirectFBInit(&this->argc,&this->argv);

		/* get interface to dfb */
		if ((dfbres = DirectFBCreate(&this->dfb)) != DFB_OK) {
			MMSFB_SetError(dfbres, "DirectFBCreate() failed");
			return false;
		}
#endif
    }
    else {
#ifdef __HAVE_XLIB__
		// initialize the X11 window
        if (!(this->x_display = XOpenDisplay((char*)0))) {
			MMSFB_SetError(0, "XOpenDisplay() failed");
        	return false;
        }
        if (!XShmQueryExtension(this->x_display)) {
			MMSFB_SetError(0, "XShmQueryExtension() failed");
        	return false;
        }
        this->x_screen = DefaultScreen(this->x_display);

        XF86VidModeModeLine line;
        int dot;
        XF86VidModeGetModeLine(this->x_display, 0,&dot, &line);
        this->display_w=line.hdisplay;
        this->display_h=line.vdisplay;
        printf("w: %d, h: %d\n", this->display_w, this->display_h);

        XSetWindowAttributes x_window_attr;
		x_window_attr.event_mask        = StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |PointerMotionMask|EnterWindowMask|ResizeRedirectMask;
        x_window_attr.background_pixel  = 0;
        x_window_attr.border_pixel      = 0;


        unsigned long x_window_mask;
        //this->window_w
        if(fullscreen) {
        	this->fullscreen = true;
        	x_window_mask = CWBackPixel | CWBorderPixel |  CWEventMask |CWOverrideRedirect;
        	x_window_attr.override_redirect = True;
        	int cnt;
        	XF86VidModeModeInfo **info;
        	XF86VidModeGetAllModeLines(this->x_display, 0, &cnt, &info);
        	int best=-1;
        	for(int i=0;i<cnt;i++) {
        		if((info[i]->hdisplay==w)&&(info[i]->vdisplay==h)) {
        			best=i;
        			break;
        		}
        		//printf("w,h: %d %d\n", info[i]->hdisplay,info[i]->vdisplay);
        	}

            int x_depth = DefaultDepth(this->x_display, this->x_screen);
            this->x_window = XCreateWindow(this->x_display, DefaultRootWindow(this->x_display), 0, 0, this->display_w, this->display_h, 0, x_depth,
    									   InputOutput, CopyFromParent, x_window_mask, &x_window_attr);
        } else {
        	this->fullscreen = false;
        	x_window_mask = CWBackPixel | CWBorderPixel |  CWEventMask ;
        	x_window_attr.override_redirect = 0;
            int x_depth = DefaultDepth(this->x_display, this->x_screen);
            this->x_window = XCreateWindow(this->x_display, DefaultRootWindow(this->x_display), 0, 0, this->w, this->h, 0, x_depth,
    									   InputOutput, CopyFromParent, x_window_mask, &x_window_attr);
        }


        XStoreName(this->x_display, this->x_window, appl_name.c_str());
        XSetIconName(this->x_display, this->x_window, appl_icon_name.c_str());
        this->x_gc = XCreateGC(this->x_display, this->x_window, 0, 0);
        XMapWindow(this->x_display, this->x_window);
        XEvent x_event;
        do {
            XNextEvent(this->x_display, &x_event);
        }
        while (x_event.type != MapNotify || x_event.xmap.event != this->x_window);
        XRaiseWindow(this->x_display, this->x_window);




		// hide X Cursor should we use X Cursor instead of our own?
        Pixmap bm_no;
		Colormap cmap;
		Cursor no_ptr;
		XColor black, dummy;
		static char bm_no_data[] = {0, 0, 0, 0, 0, 0, 0, 0};

		cmap = DefaultColormap(this->x_display, DefaultScreen(this->x_display));
		XAllocNamedColor(this->x_display, cmap, "black", &black, &dummy);
		bm_no = XCreateBitmapFromData(this->x_display, this->x_window, bm_no_data, 8, 8);
		no_ptr = XCreatePixmapCursor(this->x_display, bm_no, bm_no, &black, &black, 0, 0);

		XDefineCursor(this->x_display, this->x_window, no_ptr);
		XFreeCursor(this->x_display, no_ptr);
		if (bm_no != None)
				XFreePixmap(this->x_display, bm_no);
		XFreeColors(this->x_display, cmap, &black.pixel, 1, 0);


        XSetInputFocus(this->x_display, this->x_window,RevertToPointerRoot,CurrentTime);
//        int CompletionType = XShmGetEventBase(this->x_display) + ShmCompletion;

        unsigned int num_adaptors;
        XvAdaptorInfo *ai;
        if (XvQueryAdaptors(this->x_display, DefaultRootWindow(this->x_display), &num_adaptors, &ai)) {
			MMSFB_SetError(0, "XvQueryAdaptors() failed");
        	return false;
        }
        printf("DISKO: Available xv adaptors:\n");
        for(unsigned int cnt=0;cnt<num_adaptors;cnt++) {
        	printf("  %s\n", ai[cnt].name);
        }

        /* get the last xv blitter -> needs detection */
        printf("DISKO: Using xv adaptor '%s'\n", ai[num_adaptors-1].name);
        this->xv_port = ai[num_adaptors-1].base_id;
#endif
    }

    this->initialized = true;
    return true;
}

bool MMSFB::release() {
    if (this->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		if (this->dfb)
			this->dfb->Release(this->dfb);
#endif
    }
    else {
#ifdef __HAVE_XLIB__
#endif
    }

	this->initialized = false;
    return true;
}

bool MMSFB::isInitialized() {
	return this->initialized;
}

MMSFB_BACKEND MMSFB::getBackend() {
	return this->backend;
}

bool MMSFB::getLayer(int id, MMSFBLayer **layer) {

	// check if initialized
    INITCHECK;

    if (this->layer[id]) {
        // i have already the layer
        *layer = this->layer[id];
        return true;
    }

    // create a new layer instance
    *layer = new MMSFBLayer(id);
    if (!*layer) {
        MMSFB_SetError(0, "cannot create new instance of MMSFBLayer");
        return false;
    }
    if (!(*layer)->isInitialized()) {
    	delete *layer;
    	*layer = NULL;
		MMSFB_SetError(0, "cannot initialize MMSFBLayer");
		return false;
	}

    // save this for the next call
    this->layer[id] = *layer;

    return true;
}

void *MMSFB::getX11Window() {
    if (this->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
#endif
    }
    else {
#ifdef __HAVE_XLIB__
    	return &this->x_window;
#endif
    }
    return NULL;
}
void *MMSFB::getX11Display() {
    if (this->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
#endif
    }
    else {
#ifdef __HAVE_XLIB__
    	return this->x_display;
#endif
    }
    return NULL;
}

bool MMSFB::refresh() {
    // check if initialized
    INITCHECK;

    if (this->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
#endif
    }
    else {
#ifdef __HAVE_XLIB__
    	MMSFBSurface *suf;
    	if (this->layer[0]->getSurface(&suf))
    		suf->refresh();
#endif
    }

    return true;
}

bool MMSFB::createSurface(MMSFBSurface **surface, int w, int h, MMSFBSurfacePixelFormat pixelformat, int backbuffer, bool systemonly) {
    /* check if initialized */
    INITCHECK;

    /* create or reuse a surface */
    *surface = mmsfbsurfacemanager->createSurface(w, h, pixelformat, backbuffer, systemonly);

    if (*surface)
        return true;
    else
        return false;
}

#ifdef  __HAVE_DIRECTFB__
bool MMSFB::createImageProvider(IDirectFBImageProvider **provider, string filename) {
    if (this->backend == MMSFB_BACKEND_DFB) {
#ifdef  __HAVE_DIRECTFB__
		DFBResult   dfbres;

		/* check if initialized */
		INITCHECK;

		/* create the provider */
		if ((dfbres=this->dfb->CreateImageProvider(this->dfb, filename.c_str(), provider)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFB::CreateImageProvider(" + filename + ") failed");
			return false;
		}

		return true;
#endif
    }
    else {
#ifdef __HAVE_XLIB__
		*provider = NULL;
		return false;
#endif
    }
}
#endif

bool MMSFB::createFont(MMSFBFont **font, string filename, int width, int height) {
	// check if initialized
	INITCHECK;

	// create new instance of MMSFBFont
	*font = new MMSFBFont(filename, width, height);
	if (!*font) {
		MMSFB_SetError(0, "cannot create new MMSFBFont instance for " + filename);
		return false;
	}
	if (!(*font)->isInitialized()) {
		delete *font;
		*font = NULL;
		MMSFB_SetError(0, "cannot initialize new MMSFBFont instance for " + filename);
		return false;
	}
	return true;
}

#ifdef __HAVE_XLIB__
bool MMSFB::resizewindow() {
	printf("resize w,h: %d,%d\n", this->target_window_w, this->target_window_h );
	XWindowChanges chg;
	chg.width=this->target_window_w;
	chg.height=this->target_window_h;
	printf("rc %d\n",XConfigureWindow(this->x_display, this->x_window,CWWidth|CWHeight, &chg));
	//XMoveResizeWindow(this->x_display, this->x_window, this->target_window_w, this->target_window_h);
	return true;
}
#endif
