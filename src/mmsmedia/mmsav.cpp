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

#include "mmsmedia/mmsav.h"
#ifdef __HAVE_DIRECTFB__
#include <directfb_version.h>
#endif
#include <string.h>
#include <stdlib.h>

MMS_CREATEERROR(MMSAVError);

#ifdef __HAVE_DIRECTFB__
DFBResult dfbres;
#define THROW_DFB_ERROR(dfbres,msg) {if (dfbres) { string s1 = msg; string s2 = DirectFBErrorString((DFBResult)dfbres); throw new MMSAVError(dfbres,s1 + " [" + s2 + "]"); }else{ throw new MMSAVError(0,msg); }}
#endif

/**
 * Callback, that will be called before a frame is drawn.
 *
 * It checks if the ratio has changed and sets some variables if so.
 *
 * @param   cdata       [in/out]    pointer to VODESC structure
 * @param   width       [in]        width of dvd stream
 * @param   height      [in]        height of dvd stream
 * @param   ratio       [in]        ratio of dvd stream
 * @param   format      [in]        pixel format of dvd stream
 * @param   dest_rect   [out]       the current rectangle will be returned
 */
#ifdef __HAVE_DIRECTFB__
static void dfb_output_cb(void *cdata, int width, int height, double ratio,
						  DFBSurfacePixelFormat format, DFBRectangle* dest_rect) {
    VODESC *vodesc = (VODESC *) cdata;
    int    newH, newW;

	//DEBUGOUT("\noutput_cb %d:%d:%f:%d:%d",width,height,ratio,vodesc->windsc.width,vodesc->windsc.height);
    if (vodesc->ratio != ratio) {
    	newW = (int)((double)(vodesc->windsc.height) * ratio + 0.5);
    	newH = (int)((double)(vodesc->windsc.width) / ratio + 0.5);

    	/* ratio has changed */
        if (ratio<1.0) {
        	if(newW > vodesc->windsc.width) {
        		vodesc->rect.w = vodesc->windsc.width;
        		vodesc->rect.h = newH;
        	} else {
				vodesc->rect.w = newW;
				vodesc->rect.h = vodesc->windsc.height;
        	}
            vodesc->rect.x = (vodesc->windsc.width - vodesc->rect.w) / 2;
            vodesc->rect.y = 0;
        } else {
        	if(newH > vodesc->windsc.height) {
        		vodesc->rect.h = vodesc->windsc.height;
                vodesc->rect.w = newW;
        	} else {
        		vodesc->rect.w = vodesc->windsc.width;
        		vodesc->rect.h = newH;
        	}
            vodesc->rect.x = 0;
            vodesc->rect.y = (vodesc->windsc.height - vodesc->rect.h) / 2;
        }

        /* save other infos */
        vodesc->format = format;
        vodesc->ratio  = ratio;
        vodesc->width  = width;
        vodesc->height = height;
        /* clear surface */
        vodesc->winsurface->clear();
        vodesc->winsurface->flip();
        vodesc->winsurface->clear();
    }

    /* return the current rect */
    *dest_rect=(DFBRectangle&)vodesc->rect;
    //DEBUGOUT("\nrect %d:%d:%d:%d",dest_rect->x,dest_rect->y,dest_rect->w,dest_rect->h);

}
#endif


static void printFrameFormat(int frame_format) {
	switch(frame_format) {
		case XINE_VORAW_YV12:
			printf("YV12 frame\n");
			break;
		case XINE_VORAW_YUY2:
			printf("YUY2 frame\n");
			break;
		case XINE_VORAW_RGB:
			printf("RGB frame\n");
			break;
		default:
			printf("unknown frame format\n");
	}
}

static MMSFBSurface *interim = NULL;
static raw_overlay_t *overlays = NULL;
static int numOverlays = 0;

void raw_frame_cb(void *user_data, int frame_format, int frame_width, int frame_height, double frame_aspect, void *data0, void *data1, void *data2) {
	MMSRAW_USERDATA *userd =(MMSRAW_USERDATA *)user_data;
	int newW,newH;
/*	printf("-------\nframe format: ");
	printFrameFormat(frame_format);
	printf("frame_width: %d\n", frame_width);
	printf("frame_height: %d\n", frame_height);
	printf("frame_height: %f\n", frame_aspect);
	printf("plane0: %p\n", data0);
	printf("plane1: %p\n", data1);
	printf("plane2: %p\n", data2);
	printf("-------\n");*/

    if (userd->lastaspect != frame_aspect) {
    	// format changed
    	printf("format change %f\n", frame_aspect);
    	newW = (int)((double)(userd->size.h) * frame_aspect + 0.5);
    	newH = (int)((double)(userd->size.w) / frame_aspect + 0.5);

    	/* ratio has changed */
        if (frame_aspect<1.0) {
        	if(newW > userd->size.w) {
        		userd->dest.w = userd->size.w;
        		userd->dest.h = newH;
        	} else {
        		userd->dest.w = newW;
        		userd->dest.h = userd->size.w;
        	}

        	userd->dest.x = (userd->size.w - userd->dest.w) / 2;
        	userd->dest.y = 0;
        } else {
        	if(newH > userd->size.h) {
        		userd->dest.h = userd->size.h;
        		userd->dest.w = newW;
        	} else {
        		userd->dest.w = userd->size.w;
        		userd->dest.h = newH;
        	}
        	userd->dest.x = (userd->size.w - userd->dest.w) / 2;;
        	userd->dest.y = (userd->size.h - userd->dest.h) / 2;
        }

        userd->lastaspect  = frame_aspect;
        /* clear surface */
        userd->surf->clear();
    	userd->surf->flip(NULL);
        userd->surf->clear();

        // printf("w,h,x,y: %d, %d, %d, %d\n", userd->dest.w,userd->dest.h,userd->dest.x,userd->dest.y);
        userd->dest.w&=~0x01;
        userd->dest.h&=~0x01;
        userd->dest.x&=~0x01;
        userd->dest.y&=~0x01;
        // printf("w,h,x,y: %d, %d, %d, %d\n", userd->dest.w,userd->dest.h,userd->dest.x,userd->dest.y);

        // delete iterim surface
		if (interim) {
			delete interim;
			interim = NULL;
		}
    }

	if (userd->surf_pixelformat == MMSFB_PF_YV12) {
		// the destination has YV12 pixelformat
		if (frame_format == XINE_VORAW_RGB) {
    		// we get RGB24 data
			if (!interim) {
				// allocate interim buffer for RGB24 to YV12 convertion
				interim = new MMSFBSurface(frame_width, frame_height, MMSFB_PF_YV12);
			}
    	}

		if (interim) {
			// source is RGB24
			interim->blitBuffer(data0, frame_width*3, MMSFB_PF_RGB24,
							    frame_width, frame_height, NULL, 0, 0);
			userd->surf->stretchBlit(interim, NULL, &userd->dest);

		} else {
			// source is YV12
			MMSFBExternalSurfaceBuffer buf;
			buf.ptr = data0;
			buf.pitch = frame_width;
			buf.ptr2 = data1;
			buf.pitch2 = frame_width / 2;
			buf.ptr3 = data2;
			buf.pitch3 = frame_width / 2;

			userd->surf->stretchBlitBuffer(&buf, MMSFB_PF_YV12,
										   frame_width, frame_height, NULL, &userd->dest);
		}
	}
	else {
		// destination with any other pixelformat
		if (frame_format == XINE_VORAW_YV12) {
    		// we get YV12 data
			if (!interim) {
				// allocate interim buffer for YV12 stretch blit
				interim = new MMSFBSurface(userd->dest.w, userd->dest.h, MMSFB_PF_YV12);
			}
    	}

		if (interim) {
			// source is YV12
			MMSFBExternalSurfaceBuffer buf;
			buf.ptr = data0;
			buf.pitch = frame_width;
			buf.ptr2 = data1;
			buf.pitch2 = frame_width / 2;
			buf.ptr3 = data2;
			buf.pitch3 = frame_width / 2;
			MMSFBRectangle mydest = userd->dest;
			mydest.x = 0;
			mydest.y = 0;

			interim->stretchBlitBuffer(&buf, MMSFB_PF_YV12,
									   frame_width, frame_height, NULL, &mydest);
			userd->surf->blit(interim, NULL, userd->dest.x, userd->dest.y);

		} else {
			// source is RGB24
			userd->surf->stretchBlitBuffer(data0, frame_width*3, MMSFB_PF_RGB24,
										   frame_width, frame_height, NULL, &userd->dest);
		}
	}

    // TODO: what if interim == NULL?
    if(interim) {
    	for(int i = 0; i < numOverlays; ++i) {
    		raw_overlay_t ovl = overlays[i];

    		interim->setBlittingFlags(MMSFB_BLIT_BLEND_ALPHACHANNEL);
    		interim->blitBuffer(ovl.ovl_rgba, ovl.ovl_w * 4, MMSFB_PF_ARGB, ovl.ovl_w, ovl.ovl_h, NULL, ovl.ovl_x, ovl.ovl_y);
    		interim->setBlittingFlags(MMSFB_BLIT_NOFX);

    		userd->surf->stretchBlit(interim, NULL, &userd->dest);
    	}
    }

    userd->surf->flip(NULL);
}

/**
 * Callback, that will be called each time an overlay state changes @see: xine.h
 */
void raw_overlay_cb(void *user_data, int num_ovl, raw_overlay_t *overlays_array) {
	numOverlays = num_ovl;
	overlays = overlays_array;
}


/**
 * Callback, that will be called after a frame is drawn.
 *
 * It sets clipping areas and does the flipping.
 *
 * @param   cdata       [in/out]    pointer to VODESC structure
 */
#ifdef __HAVE_DIRECTFB__
#if DIRECTFB_MAJOR_VERSION == 1
static void dfb_frame_cb(void *cdata) {
#else
static int dfb_frame_cb(void *cdata) {
#endif
    VODESC *vodesc = (VODESC *) cdata;
/* this was for GUI optimization which is currently not active

    if (vodesc->rect.y > 0) {
        DFBRegion reg;
        reg.x1=0;
        reg.y1=0;
        reg.x2=vodesc->rect.w-1;
        reg.y2=vodesc->rect.y-1;
        vodesc->winsurface->setClip(&reg);
        vodesc->winsurface->clear();
        vodesc->winsurface->setClip(NULL);
    }

    if (vodesc->rect.h < vodesc->windsc.height) {
        DFBRegion reg;
        reg.x1=0;
        reg.y1=vodesc->rect.y+vodesc->rect.h;
        reg.x2=vodesc->rect.w-1;
        reg.y2=vodesc->windsc.height-1;
        vodesc->winsurface->setClip(&reg);
        vodesc->winsurface->clear();
        vodesc->winsurface->setClip(NULL);
    }

    vodesc->winsurface->flip(NULL, (MMSFBSurfaceFlipFlags)(DSFLIP_ONSYNC));
*/

    /*DFBRegion reg;
    reg.x1=vodesc->rect.x;
    reg.y1=vodesc->rect.y;
    reg.x2=reg.x1 + vodesc->rect.w-1;
    reg.y2=reg.y1 + vodesc->rect.h-1;*/


//    vodesc->winsurface->flip(NULL, (MMSFBSurfaceFlipFlags)(DSFLIP_WAITFORSYNC));
    vodesc->winsurface->flip();


    //vodesc->winsurface->lock();

    /*          frame->surface->Unlock( frame->surface );

          frame->surface->Lock( frame->surface, DSLF_WRITE,
                               (void*)&frame->vo_frame.base[0],
                               (int *)&frame->vo_frame.pitches[0] );
*/

    //vodesc->winsurface->flip(NULL, (MMSFBSurfaceFlipFlags)(DSFLIP_ONSYNC));
    //vodesc->winsurface->flip(NULL, (MMSFBSurfaceFlipFlags)DSFLIP_NONE);

    //vodesc->winsurface->unlock();

#if DIRECTFB_MAJOR_VERSION < 1
    return 0;
#endif
}
#endif

static void* stopRoutine(void *data) {
	if(!data) return NULL;
	xine_stream_t *stream= (xine_stream_t*)data;
    xine_stop(stream);

    return NULL;
}

/**
 * Initializes some xine stuff.
 *
 * It creates the xine object that is used for
 * audio/video stream creation.
 * Then it loads the user's xine configuration and sets
 * the given verbosity.
 *
 * @exception   MMSAVError  cannot get a new xine object
 */
void MMSAV::xineInit() {
    /* get a new xine object */
    if (!(this->xine = xine_new()))
        throw new MMSAVError(0, "Cannot get a new xine object");

    /* load xine config */
    string cfg;
    if(getenv("XINERC"))
        cfg = getenv("XINERC");
    else {
        if (getenv("HOME"))
            cfg = string(getenv("HOME")) + "/.xine";
        else
            cfg = "~/.xine";
        mkdir(cfg.c_str(), 755);
        cfg = cfg + "/config";
    }
    xine_config_load(this->xine, cfg.c_str());

    /* init xine */
    xine_init(this->xine);

    /* set verbosity */
    if(this->verbose)
        xine_engine_set_param(this->xine, XINE_PARAM_VERBOSITY, XINE_VERBOSITY_DEBUG);
    else
        xine_engine_set_param(this->xine, XINE_PARAM_VERBOSITY, XINE_VERBOSITY_NONE);
}

/**
 * Initializes everything that is needed my MMSAV.
 *
 * First it initializes xine.
 * Then it sets some internal variables and does some surface
 * flipping.
 *
 * @param   verbose [in]    if true the xine engine writes debug messages to stdout
 * @param   window  [in]    window that will be used for video output
 *
 * @see MMSAV::xineInit()
 *
 * @exception   MMSAVError  Cannot get a new xine object
 * @exception   MMSAVError  MMSFBSurface::clear() failed
 * @exception   MMSAVError  MMSFBSurface::flip() failed
 * @exception   MMSAVError  Cannot open the DFB video driver
 */
void MMSAV::initialize(const bool verbose, MMSWindow *window) {

    this->verbose          = verbose;

    DEBUGMSG("MMSMedia", "xineInit()...");

    /* initialize xine */
    xineInit();

    DEBUGMSG("MMSMedia", "xineInit() done.");

    if(window) {
    }

	if (mmsfb->getBackend() != MMSFB_BACKEND_DFB) {
		this->rawvisual.raw_output_cb = raw_frame_cb;
		this->rawvisual.supported_formats = XINE_VORAW_YV12;
		if(window) {
			this->rawvisual.user_data = (void *)&(this->userd);
		}
		this->rawvisual.raw_overlay_cb = raw_overlay_cb;
	}
	else {
#ifdef __HAVE_DIRECTFB__
		this->window           = window;
		this->vodesc.format    = DSPF_UNKNOWN;
		this->vodesc.ratio     = 1.25;
		this->vodesc.width     = 720;
		this->vodesc.height    = 576;
		this->vodesc.rect.x    = 0;
		this->vodesc.rect.y    = 0;

		if(window) {
			window->getSurface()->getSize(&(this->vodesc.windsc.width), &(this->vodesc.windsc.height));
			this->vodesc.winsurface = window->getSurface();
			if(!this->vodesc.winsurface->clear())
				THROW_DFB_ERROR(dfbres, "MMSFBSurface::clear() failed");
			if(!this->vodesc.winsurface->flip())
				THROW_DFB_ERROR(dfbres, "MMSFBSurface::flip() failed");
		}
		this->vodesc.rect.w = this->vodesc.windsc.width;
		this->vodesc.rect.h = (int)((double)(this->vodesc.windsc.width) / this->vodesc.ratio + 0.5);
#endif
	}

	//this->vodesc.winsurface
	/* clear surface */

	MMSConfigData config;
	if (mmsfb->getBackend() != MMSFB_BACKEND_DFB) {
		if(window) {
			this->userd.surf=window->getSurface();
			this->userd.surf->setBlittingFlags(MMSFB_BLIT_NOFX);
			this->userd.surf->getPixelFormat(&this->userd.surf_pixelformat);
			int w,h;
			this->userd.surf->getSize(&w,&h);
			this->userd.surf=window->getSurface();
			this->userd.size.x=0;
			this->userd.size.y=0;
			this->userd.size.w=w;
			this->userd.size.h=h;
			this->userd.lastaspect=0.0;
		}
        DEBUGMSG("MMSMedia", "opening video driver...");
        /* open the video output driver */
        if (!(this->vo = xine_open_video_driver(this->xine, "raw",
                                    XINE_VISUAL_TYPE_RAW, (void*) &this->rawvisual)))
                    throw new MMSAVError(0, "Cannot open the DFB video driver");
	}
	else {
#ifdef __HAVE_DIRECTFB__
            if(window) {
				if(((IDirectFBSurface *)vodesc.winsurface->getDFBSurface())->SetBlittingFlags((IDirectFBSurface *)vodesc.winsurface->getDFBSurface(), DSBLIT_NOFX) != DFB_OK)
					DEBUGMSG("MMSMedia", "set blitting failed");
				/* fill the visual structure for the video output driver */
				this->visual.destination  = (IDirectFBSurface *)vodesc.winsurface->getDFBSurface();
            }
            else
            	this->visual.destination = NULL;
            this->visual.subpicture   = NULL;
            this->visual.output_cb    = dfb_output_cb;
            this->visual.output_cdata = (void*) &(this->vodesc);
            this->visual.frame_cb     = dfb_frame_cb;
            this->visual.frame_cdata  = (void*) &(this->vodesc);
            //this->visual.destination->SetField( this->visual.destination, 0 );
            DEBUGMSG("MMSMedia", "opening video driver...");
            if (!(this->vo = xine_open_video_driver(this->xine, "DFB",
                                        XINE_VISUAL_TYPE_DFB, (void*) &this->visual)))
                        throw new MMSAVError(0, "Cannot open the DFB video driver");
#endif
	}
	DEBUGMSG("MMSMedia", "opening video driver done.");


    /* open the audio output driver */
    const char* const *ao_list;
    int i = 0;
    if(!(ao_list = xine_list_audio_output_plugins(this->xine)) || !*ao_list) {
        DEBUGMSG("MMSMedia", "No audio output plugins found");
        xine_engine_set_param(this->xine, XINE_PARAM_IGNORE_AUDIO, 1);
        this->ao=NULL;
        return;
    }
    do {
        DEBUGMSG("MMSMedia", "checking audio output '%s'...", ao_list[i]);

    	/* ignore file output */
        if(strcmp(ao_list[i], "file") == 0) {
        	i++;
        	continue;
        }
        else if(strcmp(ao_list[i], "none") == 0)
        {
            /* disable audio */
            xine_engine_set_param(this->xine, XINE_PARAM_IGNORE_AUDIO, 1);
            DEBUGMSG("MMSMedia", "Could not open audio driver, sound disabled!");
            break;
        }

        DEBUGMSG("MMSMedia", "opening audio output '%s'", ao_list[i]);
    }
    while(!(this->ao = xine_open_audio_driver(this->xine, ao_list[i++], NULL)));

    DEBUGMSG("MMSMedia", "Using audio driver '%s'", ao_list[i-1]);
}

/**
 * Constructor
 *
 * Initializes private variables
 */
MMSAV::MMSAV() : window(NULL),
				 didXineOpen(false),
                 verbose(false),
                 status(STATUS_NONE),
                 pos(0),
                 xine(NULL),
                 vo(NULL),
                 ao(NULL),
                 stream(NULL),
                 queue(NULL) {
    this->onError          = new sigc::signal<void, string>;
    this->onStatusChange   = new sigc::signal<void, const unsigned short, const unsigned short>;
}

/**
 * Destructor
 *
 * Deletes sigc++-callbacks and closes all xine related
 * stuff.
 */
MMSAV::~MMSAV() {
    if(this->onError) {
        this->onError->clear();
        delete this->onError;
    }

    if(this->onStatusChange) {
        this->onStatusChange->clear();
        delete this->onStatusChange;
    }

    if(this->queue)
        xine_event_dispose_queue(this->queue);
    if(this->stream)
        xine_dispose(this->stream);
    if(this->ao)
        xine_close_audio_driver(this->xine, this->ao);
    if(this->vo)
        xine_close_video_driver(this->xine, this->vo);

    // dispose all registered post plugins
    map<string, xine_post_t*>::const_iterator i;
    for(i = audioPostPlugins.begin(); i != audioPostPlugins.end(); ++i)
        xine_post_dispose(this->xine, i->second);
    audioPostPlugins.erase(audioPostPlugins.begin(), audioPostPlugins.end());

    for(i = videoPostPlugins.begin(); i != videoPostPlugins.end(); ++i)
        xine_post_dispose(this->xine, i->second);
    videoPostPlugins.erase(videoPostPlugins.begin(), videoPostPlugins.end());

    // exit xine
    xine_exit(this->xine);

}

/**
 * Opens an audio/video object.
 *
 * It creates the xine stream, wires all registered
 * audio/video post plugins, sets the verbosity and
 * registers the event queue if given.
 *
 * @note    You have to register post plugins before
 * calling open().
 *
 * @param   queue_cb    [in]    xine event queue callback
 * @param	userData	[in]	data to be used in xine event callbacks
 *
 * @see MMSAV::registerAudioPostPlugin
 * @see MMSAV::registerVideoPostPlugin
 *
 * @exception   MMSAVError  Cannot get a new stream
 */
void MMSAV::open(xine_event_listener_cb_t queue_cb, void *userData) {
    /* open stream */
    if (!(this->stream = xine_stream_new(this->xine, this->ao, this->vo)))
        throw new MMSAVError(0, "Cannot get a new stream");

    /* wire all post plugins */
    for(map<string, xine_post_t*>::const_iterator i = videoPostPlugins.begin(); i != videoPostPlugins.end(); ++i)
        xine_post_wire_video_port(xine_get_video_source(stream), i->second->video_input[0]);
    for(map<string, xine_post_t*>::const_iterator i = audioPostPlugins.begin(); i != audioPostPlugins.end(); ++i)
        xine_post_wire_audio_port(xine_get_audio_source(stream), i->second->audio_input[0]);

    /* set verbosity */
    if(this->verbose)
        xine_set_param(this->stream, XINE_PARAM_VERBOSITY, XINE_VERBOSITY_DEBUG);
    else
        xine_set_param(this->stream, XINE_PARAM_VERBOSITY, XINE_VERBOSITY_NONE);

    /*if(this->vo)
         xine_set_param(this->stream, XINE_PARAM_VO_DEINTERLACE, false);
*/
    if(this->ao) {
        xine_set_param(this->stream, XINE_PARAM_AUDIO_MUTE, false);
        xine_set_param(this->stream, XINE_PARAM_AUDIO_CHANNEL_LOGICAL, -1);
    }

    /* create event listener thread */
    if(queue_cb) {
        this->queue = xine_event_new_queue(this->stream);
        if(this->queue)
        	xine_event_create_listener_thread(this->queue, queue_cb, userData);
        else
        	DEBUGMSG("MMSMedia", "Could not create event listener");
    }
}

/**
 * Registers a xine audio post plugin.
 *
 * Post plugin will be initialized.
 *
 * @param   name    [in]    name of the post plugin
 *
 * @return  true if plugin could be initialized correctly
 */
bool MMSAV::registerAudioPostPlugin(string name) {
    xine_post_t *p;

    if(!(p = xine_post_init(this->xine, name.c_str(), 1, &this->ao, NULL)))
        DEBUGMSG("MMSMedia", "Could not initialize audio post plugin %s", name.c_str());
    else {
        audioPostPlugins[name] = p;
        return true;
    }

    return false;
}

/**
 * Registers a xine video post plugin.
 *
 * Post plugin will be initialized.
 *
 * @param   name    [in]    name of the post plugin
 *
 * @return  true if plugin could be initialized correctly
 */
bool MMSAV::registerVideoPostPlugin(string name) {
    xine_post_t *p;

    if(!(p = xine_post_init(this->xine, name.c_str(), 1, NULL, &this->vo)))
        DEBUGMSG("MMSMedia", "Could not initialize video post plugin %s", name.c_str());
    else {
        videoPostPlugins[name] = p;
        return true;
    }

    return false;
}

/**
 * Sets post plugin parameter.
 *
 * @param   plugins     [in]    map of post plugins
 * @param   name        [in]    name of post plugin that will be affected
 * @param   parameter   [in]    parameter to set
 * @param   value       [in]    value for the parameter
 *
 * @return  true if the parameter could be set
 */
bool MMSAV::setPostPluginParameter(map<string, xine_post_t*> plugins, string name, string parameter, string value) {
    xine_post_in_t              *postIn;
    xine_post_api_t             *postApi;
    xine_post_api_descr_t       *postApiDesc;
    xine_post_api_parameter_t   *postApiParam;
    char                        *data;

    // search for plugin
    if(!(postIn = (xine_post_in_t *)xine_post_input(plugins[name], "parameters"))) {
        DEBUGMSG("MMSMedia", "Could not set parameter for post plugin %s: Plugin not registered", name.c_str());
        return false;
    }

    postApi      = (xine_post_api_t *)postIn->data;
    postApiDesc  = postApi->get_param_descr();
    postApiParam = postApiDesc->parameter;
    data         = new char[postApiDesc->struct_size];
    postApi->get_parameters(plugins[name], (void*)data);

    while(postApiParam->type != POST_PARAM_TYPE_LAST)
    {
        if(strToUpr(string(postApiParam->name)) == strToUpr(parameter)) {
            if(postApiParam->type == POST_PARAM_TYPE_INT) {
                int iValue = atoi(value.c_str());

                // check if the name of an enumeration is used
                if(iValue == 0 && value.at(0) != '0') {
                    for(int i = 0; postApiParam->enum_values[i]; i++) {
                        if(value == postApiParam->enum_values[i]) {
                            iValue = i;
                            break;
                        }
                    }
                } else {
                    // check if value is out of range
                    if(iValue < postApiParam->range_min || iValue > postApiParam->range_max) {
                        DEBUGMSG("MMSMedia", "Could not set %s's %s to %s: Out of range", name.c_str(), parameter.c_str(), value.c_str());
                        return false;
                    }
                }

                // set value
                *(int *)(data + postApiParam->offset) = iValue;
                DEBUGMSG("MMSMedia", "%s: %s = %s", name.c_str(), parameter.c_str(), postApiParam->enum_values[*(int *)(data + postApiParam->offset)]);
            }
            else if(postApiParam->type == POST_PARAM_TYPE_DOUBLE) {
                double dValue = atof(value.c_str());

                // check if value is out of range
                if(dValue < postApiParam->range_min || dValue > postApiParam->range_max) {
                    DEBUGMSG("MMSMedia", "Could not set %s's %s to %s: Out of range", name.c_str(), parameter.c_str(), value.c_str());
                    return false;
                }

                // set value
               *(double *)(data + postApiParam->offset) = dValue;
               DEBUGMSG("MMSMedia", "%s: %s = %s", name.c_str(), parameter.c_str(), value.c_str());
            }
            else if(postApiParam->type == POST_PARAM_TYPE_BOOL) {
                bool bValue = false;
                if(value == "1" || strToUpr(value) == "TRUE")
                    bValue = true;
               *(bool *)(data + postApiParam->offset) = bValue;
               DEBUGMSG("MMSMedia", "%s: = %s", name.c_str(), parameter.c_str(), (bValue ? "true" : "false"));
            }
            else if(postApiParam->type == POST_PARAM_TYPE_CHAR) {
                char cValue = value.at(0);
               *(char *)(data + postApiParam->offset) = cValue;
               DEBUGMSG("MMSMedia", "%s: %s = %c", name.c_str(), parameter.c_str(), cValue);
            }
            else if(postApiParam->type == POST_PARAM_TYPE_STRING) {
                char *sValue = (char*)value.c_str();
               *(char **)(data + postApiParam->offset) = sValue;
               DEBUGMSG("MMSMedia", "%s: %s = %s", name.c_str(), parameter.c_str(), sValue);
            }
            break;
        }
        postApiParam++;
    }

    if(!postApi->set_parameters(plugins[name], (void *)data))
        DEBUGMSG("MMSMedia", "Error setting post plugin parameter");

    delete[] data;

    return true;
}

/**
 * Sets audio post plugin parameter.
 *
 * @param   name        [in]    name of post plugin that will be affected
 * @param   parameter   [in]    parameter to set
 * @param   value       [in]    value for the parameter
 *
 * @return  true if the parameter could be set
 *
 * @see     MMSAV::setPostPluginParameter
 * @see     MMSAV::setVideoPostPluginParameter
 */
bool MMSAV::setAudioPostPluginParameter(string name, string parameter, string value) {
    return setPostPluginParameter(this->audioPostPlugins, name, parameter, value);
}

/**
 * Sets video post plugin parameter.
 *
 * @param   name        [in]    name of post plugin that will be affected
 * @param   parameter   [in]    parameter to set
 * @param   value       [in]    value for the parameter
 *
 * @return  true if the parameter could be set
 *
 * @see     MMSAV::setPostPluginParameter
 * @see     MMSAV::setAudioPostPluginParameter
 */
bool MMSAV::setVideoPostPluginParameter(string name, string parameter, string value) {
    return setPostPluginParameter(this->videoPostPlugins, name, parameter, value);
}

/**
 * Sets internal status of sound/video playback.
 *
 * It also emits a signal, which can be handled using the
 * sigc++ connectors.
 *
 * @param   status  [in]    status to set
 *
 * @see MMSAV::onStatusChange
 */
void MMSAV::setStatus(int status) {
    switch(status) {
        case STATUS_PLAYING :
            if(this->status != this->STATUS_NONE)
                this->onStatusChange->emit(this->status, status);
            this->status = status;
            return;
        case STATUS_PAUSED  :
        case STATUS_STOPPED :
        case STATUS_FFWD    :
        case STATUS_FFWD2   :
        case STATUS_SLOW    :
        case STATUS_SLOW2   :
            this->onStatusChange->emit(this->status, status);
            this->status = status;
            return;
        default:
            break;
    }

    this->onStatusChange->emit(status, status);
}

/**
 * Determines if a stream is currently being played.
 *
 * @return true if stream is being played
 */
bool MMSAV::isPlaying() {
	if(this->status == STATUS_PLAYING) {
		if(xine_get_status(this->stream)!=XINE_STATUS_PLAY) {
	    	this->setStatus(STATUS_STOPPED);
	    	return false;
		}
		else return true;
	}
	return false;
}

/**
 * Determines if a stream is currently being paused.
 *
 * @return true if stream is being paused
 */
bool MMSAV::isPaused() {
	if(this->status == STATUS_PAUSED) {
		if(xine_get_status(this->stream)!=XINE_STATUS_PLAY) {
	    	this->setStatus(STATUS_STOPPED);
	    	return false;
		}
		else return true;
	}
	return false;
}

/**
 * Determines if a stream is in stopped status.
 *
 * @return true if stream is being stopped
 */
bool MMSAV::isStopped() {
	return (this->status == STATUS_STOPPED);
}

/**
 * Starts playing.
 *
 * If the continue flag is set it tries to continue
 * at the position where it was stopped before.
 *
 * @param   mrl     [in]    mrl to play
 * @param   cont    [in]    if true it tries to continue at a position stopped before
 *
 * @exception   MMSAVError stream could not be opened
 */
void MMSAV::startPlaying(const string mrl, const bool cont) {
    static string currentMRL = "";

    if(!this->stream)
        this->open();

    if(currentMRL != mrl) {
    	if(this->didXineOpen) {
    	    xine_close(this->stream);
    	    this->didXineOpen = false;
        }

        if(!xine_open(this->stream, mrl.c_str())) {
            string msg;
            switch(xine_get_error(this->stream)) {
                case XINE_ERROR_NO_INPUT_PLUGIN :
                    msg = "No input plugin";
                    break;
                case XINE_ERROR_NO_DEMUX_PLUGIN :
                    msg = "No demux plugin";
                    break;
                case XINE_ERROR_DEMUX_FAILED :
                    msg = "Error in demux plugin";
                    break;
                case XINE_ERROR_INPUT_FAILED :
                    msg = "Error in input plugin (" + mrl + ")";
                    break;
                default:
                    msg = "Cannot play stream";
                    break;
            }
            throw new MMSAVError(0, "Error in xine_open(): " + msg);
        }

        this->didXineOpen = true;
        currentMRL = mrl;
    }
    else if(this->isPlaying())
        return;

    /* playing... */
    if(!cont)
        this->pos = 0;
    if (!xine_play(this->stream, this->pos, 0)) {
        string msg;
        switch(xine_get_error(this->stream)) {
            case XINE_ERROR_NO_INPUT_PLUGIN :
                msg = "No input plugin";
                break;
            case XINE_ERROR_NO_DEMUX_PLUGIN :
                msg = "No demux plugin";
                break;
            case XINE_ERROR_DEMUX_FAILED :
                msg = "Error in demux plugin";
                break;
            case XINE_ERROR_INPUT_FAILED :
                msg = "Error in input plugin (" + mrl + ")";
                break;
            default:
                msg = "Cannot play stream";
                break;
        }
        throw new MMSAVError(0, "Error in xine_play(): " + msg);
    }
    this->setStatus(this->STATUS_PLAYING);
}

/**
 * Continues playing.
 *
 * It only works if playing was stopped or speed
 * changed to other than normal.
 */
void MMSAV::play() {
    if(this->status == this->STATUS_PAUSED  ||
       this->status == this->STATUS_SLOW    ||
       this->status == this->STATUS_SLOW2   ||
       this->status == this->STATUS_FFWD    ||
       this->status == this->STATUS_FFWD2) {
       this->setStatus(this->STATUS_PLAYING);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);
    }
}

/**
 * Stops playing.
 *
 * @param	savePosition	[in]	if true stream position will be saved for continuation
 *
 * It saves the position, so if you call MMSAV::play()
 * afterwards with the continue flag set, it will continue
 * at this position.
 */
void MMSAV::stop(const bool savePosition) {
    /* save position */
	if(savePosition)
		xine_get_pos_length(this->stream, &this->pos, NULL, NULL);

    this->setStatus(this->STATUS_STOPPED);

    /* stop xine in extra thread to avoid blocking the application */
    pthread_t thread;
    if(pthread_create(&thread, NULL, stopRoutine, this->stream) == 0) {
    	pthread_detach(thread);
    }
    else
    	xine_stop(this->stream);
}

/**
 * Pauses.
 *
 * It only works if stream is playing (can be slow,
 * ffwd etc.).
 */
void MMSAV::pause() {
    if(this->status == this->STATUS_PLAYING ||
       this->status == this->STATUS_SLOW    ||
       this->status == this->STATUS_SLOW2   ||
       this->status == this->STATUS_FFWD    ||
       this->status == this->STATUS_FFWD2) {
       this->setStatus(this->STATUS_PAUSED);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
    }
}

/**
 * Playback will be switched to slow motion.
 *
 * There are two different speed settings for slow motion.
 * Twice as slow and four times as slow.
 *
 * @see MMSAV::ffwd()
 * @see MMSAV::rewind()
 */
void MMSAV::slow() {
    if(this->status == this->STATUS_PLAYING || this->status == this->STATUS_PAUSED) {
        this->setStatus(this->STATUS_SLOW);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_SLOW_2);
    }
    else if(this->status == this->STATUS_SLOW) {
        this->setStatus(this->STATUS_SLOW2);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_SLOW_4);
    }
    else if(this->status == this->STATUS_FFWD) {
        this->setStatus(this->STATUS_PLAYING);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);
    }
    else if(this->status == this->STATUS_FFWD2) {
        this->setStatus(this->STATUS_FFWD);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_FAST_2);
    }
}

/**
 * Playback will be switched to fast forward.
 *
 * There are two different speed settings for fast forward.
 * Twice as fast and four times as fast.
 *
 * @see MMSAV::slow()
 * @see MMSAV::rewind()
 */
void MMSAV::ffwd() {
    if(this->status == this->STATUS_PLAYING || this->status == this->STATUS_PAUSED) {
        this->setStatus(this->STATUS_FFWD);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_FAST_2);
    }
    else if(this->status == this->STATUS_FFWD) {
        this->setStatus(this->STATUS_FFWD2);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_FAST_4);
    }
    else if(this->status == this->STATUS_SLOW) {
        this->setStatus(this->STATUS_PLAYING);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);
    }
    else if(this->status == this->STATUS_SLOW2) {
        this->setStatus(this->STATUS_SLOW);
        xine_set_param(this->stream, XINE_PARAM_SPEED, XINE_SPEED_SLOW_2);
    }
}

/**
 * Gets information about the length of the actual title
 * and the time of the current position in seconds.
 *
 * @param   pos     [out]   time in seconds of current position
 * @param   length  [out]   time in seconds of title length
 */
bool MMSAV::getTimes(int *pos, int *length) {
	if(!this->stream) return false;

	if(xine_get_pos_length(this->stream, NULL, pos, length)) {
        if(pos)    (*pos)    /= 1000;
        if(length) (*length) /= 1000;
        return true;
    }
    else
       return false;
}

/**
 * Sets the brightness if video output is done.
 *
 * @param   count   [in]    amount of brightness
 *
 * @see     MMSAV::brightnessUp()
 * @see     MMSAV::brightnessDown()
 */
void MMSAV::setBrightness(int count) {
    if(this->vo)
        xine_set_param(this->stream, XINE_PARAM_VO_BRIGHTNESS, count);
}

/**
 * Increases the brightness if video output is done.
 *
 * @param   count   [in]    amount of brightness to increase
 *
 * @see     MMSAV::setBrightness()
 * @see     MMSAV::brightnessDown()
 */
void MMSAV::brightnessUp(int count) {
    if(this->vo) {
        int value = xine_get_param(this->stream, XINE_PARAM_VO_BRIGHTNESS);
        xine_set_param(this->stream, XINE_PARAM_VO_BRIGHTNESS, value + count*500);
    }
}

/**
 * Decreases the brightness if video output is done.
 *
 * @param   count   [in]    amount of brightness to decrease
 *
 * @see     MMSAV::setBrightness()
 * @see     MMSAV::brightnessUp()
 */
void MMSAV::brightnessDown(int count) {
    if(this->vo) {
        int value = xine_get_param(this->stream, XINE_PARAM_VO_BRIGHTNESS);
        xine_set_param(this->stream, XINE_PARAM_VO_BRIGHTNESS, value - count*500);
    }
}

/**
 * Sets the contrast if video output is done.
 *
 * @param   count   [in]    amount of contrast
 *
 * @see     MMSAV::contrastUp()
 * @see     MMSAV::contrastDown()
 */
void MMSAV::setContrast(int count) {
    if(this->vo)
        xine_set_param(this->stream, XINE_PARAM_VO_CONTRAST, count);
}

/**
 * Increases the contrast if video output is done.
 *
 * @param   count   [in]    amount of contrast to increase
 *
 * @see     MMSAV::setContrast()
 * @see     MMSAV::contrastDown()
 */
void MMSAV::contrastUp(int count) {
    if(this->vo) {
        int value = xine_get_param(this->stream, XINE_PARAM_VO_CONTRAST);
        xine_set_param(this->stream, XINE_PARAM_VO_CONTRAST, value + count*500);
    }
}

/**
 * Decreases the contrast if video output is done.
 *
 * @param   count   [in]    amount of contrast to decrease
 *
 * @see     MMSAV::setContrast()
 * @see     MMSAV::contrastUp()
 */
void MMSAV::contrastDown(int count) {
    if(this->vo) {
        int value = xine_get_param(this->stream, XINE_PARAM_VO_CONTRAST);
        xine_set_param(this->stream, XINE_PARAM_VO_CONTRAST, value - count*500);
    }
}

/**
 * Sets the saturation if video output is done.
 *
 * @param   count   [in]    amount of saturation
 *
 * @see     MMSAV::saturationUp()
 * @see     MMSAV::saturationDown()
 */
void MMSAV::setSaturation(int count) {
    if(this->vo)
        xine_set_param(this->stream, XINE_PARAM_VO_SATURATION, count);
}

/**
 * Increases the saturation if video output is done.
 *
 * @param   count   [in]    amount of saturation to increase
 *
 * @see     MMSAV::setSaturation()
 * @see     MMSAV::saturationDown()
 */
void MMSAV::saturationUp(int count) {
    if(this->vo) {
        int value = xine_get_param(this->stream, XINE_PARAM_VO_SATURATION);
        xine_set_param(this->stream, XINE_PARAM_VO_SATURATION, value + count*500);
    }
}

/**
 * Decreases the saturation if video output is done.
 *
 * @param   count   [in]    amount of saturation to decrease
 *
 * @see     MMSAV::setSaturation()
 * @see     MMSAV::saturationUp()
 */
void MMSAV::saturationDown(int count) {
    if(this->vo) {
        int value = xine_get_param(this->stream, XINE_PARAM_VO_SATURATION);
        xine_set_param(this->stream, XINE_PARAM_VO_SATURATION, value - count*500);
    }
}

/**
 * Sets the hue if video output is done.
 *
 * @param   count   [in]    amount of hue
 *
 * @see     MMSAV::hueUp()
 * @see     MMSAV::hueDown()
 */
void MMSAV::setHue(int count) {
    if(this->vo)
        xine_set_param(this->stream, XINE_PARAM_VO_HUE, count);
}

/**
 * Increases the hue if video output is done.
 *
 * @param   count   [in]    amount of hue to increase
 *
 * @see     MMSAV::setHue()
 * @see     MMSAV::hueDown()
 */
void MMSAV::hueUp(int count) {
    if(this->vo) {
        int value = xine_get_param(this->stream, XINE_PARAM_VO_HUE);
        xine_set_param(this->stream, XINE_PARAM_VO_HUE, value + count*500);
    }
}

/**
 * Decreases the hue if video output is done.
 *
 * @param   count   [in]    amount of hue to decrease
 *
 * @see     MMSAV::setHue()
 * @see     MMSAV::hueUp()
 */
void MMSAV::hueDown(int count) {
    if(this->vo) {
        int value = xine_get_param(this->stream, XINE_PARAM_VO_HUE);
        xine_set_param(this->stream, XINE_PARAM_VO_HUE, value - count*500);
    }
}

/**
 * Sets the volume of the audio output.
 *
 * @param   percent	[in]    volume in percent
 */
void MMSAV::setVolume(int percent) {
	if(this->ao)
		xine_set_param(this->stream, XINE_PARAM_AUDIO_VOLUME, percent);
}

/**
 * Send a xine event to the engine
 *
 * @param   type    [in]    type of event
 * @param   data    [in]    event specific data
 * @param   datalen [in]    length of data
 */
void MMSAV::sendEvent(int type, void *data, int datalen) {
    xine_event_t evt;

	std::cerr << "6: " << this->stream << " | ";
    evt.stream      = this->stream;

	std::cerr << "5: " << data << " | ";
    evt.data        = data;

	std::cerr << "4: " << datalen << " | ";
    evt.data_length = datalen;

	std::cerr << "3: " << type << " | ";
    evt.type        = type;

	std::cerr << "2: xine_event_send | ";
    xine_event_send(this->stream, &evt);

	std::cerr << "1:" << std::endl;
}

/**
 * Returns true if stream contains a video stream.
 *
 * @return true if video stream
 */
bool MMSAV::hasVideo() {
	return (xine_get_stream_info(this->stream, XINE_STREAM_INFO_HAS_VIDEO) == 1);
}

/**
 * Returns true if stream contains an audio stream.
 *
 * @return true if audio stream
 */
bool MMSAV::hasAudio() {
	return (xine_get_stream_info(this->stream, XINE_STREAM_INFO_HAS_AUDIO) == 1);
}
