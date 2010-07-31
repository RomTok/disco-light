/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include <sys/types.h>
#include <linux/fb.h>
#include "mmsgui/fb/mmsfb.h"
#include "mmsgui/fb/mmsfbsurfacemanager.h"
#include <string.h>
#include <stdlib.h>

/* initialize the mmsfb object */
MMSFB *mmsfb = new MMSFB();


#define INITCHECK  if(!this->initialized){MMSFB_SetError(0,"not initialized");return false;}

void MMSFB_AtExit() {
	if (mmsfb)
		mmsfb->release();
}

MMSFB::MMSFB() :
	argc(0),
	argv(NULL),
	initialized(false),
#ifdef  __HAVE_DIRECTFB__
    dfb(NULL),
#endif
#ifdef  __HAVE_FBDEV__
    mmsfbdev(NULL),
#endif
#ifdef __HAVE_XLIB__
    x_display(NULL),
#endif
#ifdef __HAVE_XV__
	xv_port(0),
#endif
	outputtype(MMSFB_OT_NONE) {
	// set the atexit routine
	atexit(MMSFB_AtExit);
}

MMSFB::~MMSFB() {
#ifdef __HAVE_XV__
	if(this->x_display && this->xv_port) {
		XvUngrabPort(this->x_display, this->xv_port, CurrentTime);
	}
#endif
}


bool MMSFB::init(int argc, char **argv, MMSFBBackend backend, MMSFBOutputType outputtype, MMSFBRectangle x11_win_rect,
				 bool extendedaccel, MMSFBFullScreenMode fullscreen, MMSFBPointerMode pointer,
				 string appl_name, string appl_icon_name, bool hidden) {

    // check if already initialized
    if (this->initialized) {
        MMSFB_SetError(0, "already initialized");
        return false;
    }

    // save arguments
    this->argc = argc;
    this->argv = argv;
    this->appliconname = appl_icon_name;
    this->applname = appl_name;
    this->fullscreen = fullscreen;

    // init layer pointers
    memset(this->layer, 0, sizeof(MMSFBLayer *) * MMSFBLAYER_MAXNUM);

    // basic information mainly needed by X11 initialization
    this->outputtype = outputtype;
    this->x11_win_rect = x11_win_rect;

    // which backend should i use?
	this->backend = backend;
	if (this->backend == MMSFB_BE_DFB) {
#ifdef __HAVE_DIRECTFB__
#else
		MMSFB_SetError(0, "compile DFB support!");
		return false;
#endif
	}
    else
    if (this->backend == MMSFB_BE_X11) {
#ifdef __HAVE_XLIB__
		XInitThreads();
		this->resized=false;
#else
		MMSFB_SetError(0, "compile X11 support!");
		return false;
#endif
		if (this->outputtype == MMSFB_OT_XVSHM) {
#ifdef __HAVE_XV__
#else
			MMSFB_SetError(0, "compile X11/XV support!");
			return false;
#endif
		}
    }
    else
	if (this->backend == MMSFB_BE_FBDEV) {
#ifdef __HAVE_FBDEV__
#else
		MMSFB_SetError(0, "compile FBDEV support!");
		return false;
#endif
	}
    else
    if (this->backend == MMSFB_BE_OGL) {
#ifdef __HAVE_OPENGL__
		XInitThreads();
		this->resized=false;
#else
		MMSFB_SetError(0, "compile OPENGL support!");
		return false;
#endif
    }
	else
	if (this->backend == MMSFB_BE_NONE) {
		// fall back, auto detection
		this->backend = MMSFB_BE_DFB;
#ifdef __HAVE_XLIB__
		if ((this->outputtype == MMSFB_OT_X11)&&(extendedaccel)) {
			this->backend = MMSFB_BE_X11;
			XInitThreads();
			this->resized=false;
		}
#endif
	}
	else {
		MMSFB_SetError(0, "wrong backend " + getMMSFBBackendString(backend));
		return false;
	}

    if (this->backend == MMSFB_BE_DFB) {
#ifdef __HAVE_DIRECTFB__
        DFBResult dfbres;

        // init dfb
		DirectFBInit(&this->argc,&this->argv);

		// get interface to dfb
		if ((dfbres = DirectFBCreate(&this->dfb)) != DFB_OK) {
			MMSFB_SetError(dfbres, "DirectFBCreate() failed");
			return false;
		}
#endif
    }
    else
    if (this->backend == MMSFB_BE_FBDEV) {
#ifdef __HAVE_FBDEV__
		if (this->outputtype == MMSFB_OT_MATROXFB) {
			// matroxfb
    		this->mmsfbdev = new MMSFBDevMatrox();
		}
		else
		if (this->outputtype == MMSFB_OT_DAVINCIFB) {
			// davincifb
    		this->mmsfbdev = new MMSFBDevDavinci();
		}
		else
		if (this->outputtype == MMSFB_OT_OMAPFB) {
			// omapfb
		    DEBUGMSG("MMSGUI", "create new MMSFBDevOmap()");
    		this->mmsfbdev = new MMSFBDevOmap();
		    DEBUGMSG("MMSGUI", "created new MMSFBDevOmap()");
		}
		else {
			// default fbdev
		    DEBUGMSG("MMSGUI", "create generic fbdev");
    		this->mmsfbdev = new MMSFBDev();
		}

    	if (this->mmsfbdev)
			if (!this->mmsfbdev->openDevice()) {
				MMSFB_SetError(0, "MMSFBDEV device cannot be opened");
				return false;
			}
#endif
    }
	else {
#ifdef __HAVE_XLIB__
        if (!(this->x_display = XOpenDisplay((char*)0))) {
			MMSFB_SetError(0, "XOpenDisplay() failed");
        	return false;
        }

        this->x_screen = DefaultScreen(this->x_display);

        Window myroot=RootWindow(this->x_display, this->x_screen);
        Window root_ret;
        int myx,myy;
        unsigned int borderw, depthret;
        XGetGeometry(this->x_display, myroot, &root_ret, &myx, &myy, (unsigned int *)&(this->display_w), (unsigned int *)&(this->display_h), &borderw, &depthret);

        printf("w: %d, h: %d\n", this->display_w, this->display_h);

		x_depth=DefaultDepth(this->x_display, this->x_screen);

		this->hidden = hidden;
		this->pointer = pointer;



/*
		if (this->backend == MMSFB_BE_OGL) {
#ifdef __HAVE_OPENGL__
			// OGL
			int glxMajor, glxMinor;

		    glXQueryVersion(this->x_display, &glxMajor, &glxMinor);
		    printf("GLX-Version %d.%d\n", glxMajor, glxMinor);
		    int attribList[] =
		        {GLX_RGBA,
		        GLX_RED_SIZE, 8,
		        GLX_GREEN_SIZE, 8,
		        GLX_BLUE_SIZE, 8,
		        GLX_DEPTH_SIZE, 16,
		        GLX_DOUBLEBUFFER,
		        None};

		    this->xvi = glXChooseVisual(this->x_display, this->x_screen, attribList);
		    if (this->xvi == NULL) {
		    	int attribList[] =
		    	        {GLX_RGBA,
		    	        GLX_RED_SIZE, 8,
		    	        GLX_GREEN_SIZE, 8,
		    	        GLX_BLUE_SIZE, 8,
		    	        None};
		    	this->xvi = glXChooseVisual(this->x_display, this->x_screen, attribList);
		        printf("singlebuffered rendering will be used, no doublebuffering available\n");
		        if(this->xvi == NULL) {
		        	printf("shit happens.... \n");
		        	return false;
		        }
		    }
		    else {
		        printf("doublebuffered rendering available\n");
		    }

		    // create a GLX context
		    this->glx_context = glXCreateContext(this->x_display, this->xvi, 0, GL_TRUE);
		    if (!this->glx_context) {
		    	printf("context generation failed...\n");
		    	return false;
		    }

		    if(glXMakeCurrent(this->x_display, this->x_window, this->glx_context) != True) {
		    	printf("make current failed\n");
		    	return false;
		    }
		    printf("2222222222\n");
		    if (glXIsDirect(this->x_display, this->glx_context))
		        printf("DRI enabled\n");
		    else
		        printf("no DRI available\n");

		    XMapRaised(this->x_display, this->x_window);
			XFlush(this->x_display);

			// init extension pointers
			GLenum err=glewInit();
			if(err!=GLEW_OK) {
				//problem: glewInit failed, something is seriously wrong
				printf("Error: %s\n", glewGetErrorString(err));
				return false;
			}

			//set the coordinate system
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glViewport(0, 0, this->x11_win_rect.w, this->x11_win_rect.h);
			glOrtho(0, this->x11_win_rect.w, 0, this->x11_win_rect.h, 10.0, -10.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

#endif
		}
		*/


#endif
    }

    this->initialized = true;
    return true;
}

bool MMSFB::release() {
    if (this->backend == MMSFB_BE_DFB) {
#ifdef  __HAVE_DIRECTFB__
		if (this->dfb)
			this->dfb->Release(this->dfb);
#endif
    }
    else
    if (this->backend == MMSFB_BE_FBDEV) {
#ifdef __HAVE_FBDEV__
    	if (this->mmsfbdev) {
    		delete this->mmsfbdev;
    		this->mmsfbdev = NULL;
    	}
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

MMSFBBackend MMSFB::getBackend() {
	return this->backend;
}

bool MMSFB::lock() {
	this->Lock.lock();
	return true;
}

bool MMSFB::unlock() {
	this->Lock.unlock();
	return true;
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
    if (this->backend == MMSFB_BE_DFB) {
#ifdef  __HAVE_DIRECTFB__
#endif
    }
    else
	if (this->backend == MMSFB_BE_FBDEV) {
#ifdef  __HAVE_FBDEV__
#endif
	}
    else {
#ifdef __HAVE_XLIB__

    	return &this->input_window;
    	//return &this->x_window;
#endif
    }
    return NULL;
}
void *MMSFB::getX11Display() {
    if (this->backend == MMSFB_BE_DFB) {
#ifdef  __HAVE_DIRECTFB__
#endif
    }
    else
	if (this->backend == MMSFB_BE_FBDEV) {
#ifdef  __HAVE_FBDEV__
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

    if (this->backend == MMSFB_BE_DFB) {
#ifdef  __HAVE_DIRECTFB__
#endif
    }
    else
	if (this->backend == MMSFB_BE_FBDEV) {
#ifdef  __HAVE_FBDEV__
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
	*provider = NULL;
    if (this->backend == MMSFB_BE_DFB) {
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
    if (this->backend == MMSFB_BE_FBDEV) {
#ifdef __HAVE_FBDEV__
#endif
    }
    else {
#ifdef __HAVE_XLIB__
#endif
    }
	return false;
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
bool MMSFB::resizeWindow() {
	printf("resize w,h: %d,%d\n", this->target_window_w, this->target_window_h );
	XWindowChanges chg;
	chg.width=this->target_window_w;
	chg.height=this->target_window_h;
	printf("rc %d\n",XConfigureWindow(this->x_display, this->x_window,CWWidth|CWHeight, &chg));
	//XMoveResizeWindow(this->x_display, this->x_window, this->target_window_w, this->target_window_h);
	return true;
}
#endif
