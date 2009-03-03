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

#ifndef MMSAV_H_
#define MMSAV_H_

#include <sys/stat.h>
#include <sigc++/sigc++.h>
#include <xine.h>
#include <map>

#include "mmsgui/mmsgui.h"
#include "mmstools/mmstools.h"

/**
 * @brief   Video output description structure.
 *
 * This structure is needed for the callbacks that are used by the
 * xine video output driver.
 */
typedef struct {
    MMSFBSurface            *winsurface;        /**< surface of window          */
#ifdef __HAVE_DIRECTFB__
    DFBWindowDescription    windsc;             /**< description of window      */
    DFBSurfacePixelFormat   format;             /**< pixel format of surface    */
#endif
    double                  ratio;              /**< ratio of video stream      */
    int                     width;              /**< width of video stream      */
    int                     height;             /**< height of video stream     */
    MMSFBRectangle          rect;               /**< rectangle for video stream  */
} VODESC;

#ifdef __HAVE_DIRECTFB__
typedef void (*DVOutputCallback) (void *cdata, int width, int height,
                                  double ratio, DFBSurfacePixelFormat format,
                                  DFBRectangle *dest_rect);

/**
 * @brief   Structure needed by xine_open_video_driver().
 *
 * This structure holds surfaces and callbacks that handle
 * video playback.
 */
typedef struct {
    IDirectFBSurface    *destination;           /**< destination surface                                */
    IDirectFBSurface    *subpicture;            /**< subpicture surface                                 */
    DVOutputCallback    output_cb;              /**< output callback for DirectFB's xine video driver   */
    void                *output_cdata;          /**< data given as an argument to output_cb             */
    DVFrameCallback     frame_cb;               /**< frame callback for DirectFB's xine video driver    */
    void                *frame_cdata;           /**< data given as an argument to frame_cb              */
} dfb_visual_t;
#endif

typedef struct {
	MMSFBSurface *surf;
	MMSFBSurfacePixelFormat surf_pixelformat;
	MMSFBRectangle size;
	MMSFBRectangle dest;
	double lastaspect;
	MMSFBSurface *interim;
	//! interim overlay surface
	MMSFBSurface *overlayInterim;
	//! number of currently available overlays
	int numOverlays;
	//! array of numOverlays overlays
	raw_overlay_t *overlays;
} MMSRAW_USERDATA;


/**
 * @brief   MMS Audio/Video handling class.
 *
 * @ingroup     mmsmedia
 *
 * @author      Stefan Schwarzer (SSchwarzer@berlinux-solutions.de)
 * @author      Matthias Hardt (MHardt@berlinux-solutions.de)
 * @author      Jens Schneider (pupeider@gmx.de)
 * @author      Guido Madaus (GMadaus@berlinux-solutions.de)
 * @version     1.5.0
 * @date        12/31/2008
 *
 * It is the base class for all audio and video related
 * classes.
 */
class MMSAV
{
    private:
        MMSWindow                       *window;                                /**< window for classes that use video  */
        VODESC                          vodesc;                                 /**< video output settings              */
#ifdef __HAVE_DIRECTFB__
        dfb_visual_t                    visual;                                 /**< visual structure for video output  */
#endif
        raw_visual_t					rawvisual;
        MMSRAW_USERDATA					userd;

        pthread_mutex_t					lock;

        bool setPostPluginParameter(map<string, xine_post_t*> plugins, string name, string parameter, string value);

    protected:
        bool                            verbose;                                /**< should logging be verbose?         */
        short                           status;                                 /**< current playback status            */
        int                             pos;                                    /**< remember position where the stream stopped last time */

        // xine related attributes
        xine_t                          *xine;                                  /**< global xine structure                  */
        xine_video_port_t               *vo;                                    /**< xine video ports                       */
        xine_audio_port_t               *ao;                                    /**< xine audio ports                       */
        xine_stream_t                   *stream;                                /**< xine stream                            */
        xine_event_queue_t              *queue;                                 /**< xine event queue                       */
        map<string, xine_post_t*>       audioPostPlugins,                       /**< map of registered audio post plugins   */
                                        videoPostPlugins;                       /**< map of registered video post plugins   */

        // internal methods
        void xineInit();
        void initialize(const bool verbose = false, MMSWindow *window = NULL);
        void open(xine_event_listener_cb_t queue_cb = NULL, void *userData = NULL);
        void setStatus(int status);
        void sendEvent(int type, void *data = NULL, int datalen = 0);

    public:
        /* status constants */
        static const unsigned short STATUS_NONE                     =  0;       /**< status not set                         */
        static const unsigned short STATUS_PLAYING                  =  1;       /**< stream is being played                 */
        static const unsigned short STATUS_PAUSED                   =  2;       /**< stream is being paused                 */
        static const unsigned short STATUS_STOPPED                  =  3;       /**< stream was stopped                     */
        static const unsigned short STATUS_REWIND                   =  4;       /**< stream is being rewinded               */
        static const unsigned short STATUS_FFWD                     =  5;       /**< stream is being fast forwarded (2x)    */
        static const unsigned short STATUS_FFWD2                    =  6;       /**< stream is being fast forwarded (4x)    */
        static const unsigned short STATUS_SLOW                     =  7;       /**< stream is being played slowly (2x)     */
        static const unsigned short STATUS_SLOW2                    =  8;       /**< stream is being played slowly (4x)     */

        MMSAV();
        virtual ~MMSAV();

        bool registerAudioPostPlugin(string name);
        bool registerVideoPostPlugin(string name);
        bool setAudioPostPluginParameter(string name, string parameter, string value);
        bool setVideoPostPluginParameter(string name, string parameter, string value);

        /* playing methods */
        bool isPlaying();
        bool isPaused();
        bool isStopped();
        void startPlaying(const string mrl, const bool cont = true);
        void play();
        void stop(const bool savePosition = true);
        void pause();
        void ffwd();
        void slow();

        bool getTimes(int *pos, int *length);

        void setBrightness(int count);
        void brightnessUp(int count);
        void brightnessDown(int count);
        void setContrast(int count);
        void contrastUp(int count);
        void contrastDown(int count);
        void setSaturation(int count);
        void saturationUp(int count);
        void saturationDown(int count);
        void setHue(int count);
        void hueUp(int count);
        void hueDown(int count);

        void setVolume(int percent);

        bool hasVideo();
        bool hasAudio();

        /**
         * Callback that is used to receive errors.
         */
        sigc::signal<void, string> *onError;
        /**
         * Callback that is used to receive status changes.
         */
        sigc::signal<void, const unsigned short, const unsigned short> *onStatusChange;

};

#endif /*MMSAV_H_*/
