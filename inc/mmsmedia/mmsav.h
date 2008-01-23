/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
 *      Matthias Hardt   <mattmax@morphine.tv>                             *
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
#include "mmsgui.h"
#include "mmsfbmanager.h"
#include "mmstools.h"

/**
 * @brief   Video output description structure.
 * 
 * This structure is needed for the callbacks that are used by the
 * xine video output driver.
 */
typedef struct {
    MMSFBSurface            *winsurface;        /**< surface of window          */
    DFBWindowDescription    windsc;             /**< description of window      */
    DFBSurfacePixelFormat   format;             /**< pixel format of surface    */
    double                  ratio;              /**< ratio of video stream      */
    int                     width;              /**< width of video stream      */
    int                     height;             /**< height of video stream     */
    DFBRectangle            rect;               /**< rectangle for video stream  */
} VODESC;

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

/**
 * @brief   MMS Audio/Video handling class.
 * 
 * @ingroup     mmsmedia mmslibs
 *
 * @author      Matthias Hardt (mattmax@morphine.tv)
 * @version     1.0.3
 * @date        11/07/2007
 * 
 * It is the base class for all audio and video related
 * classes.
 */
class MMSAV
{
    private:
        MMSWindow                       *window;                                /**< window for classes that use video  */
        VODESC                          vodesc;                                 /**< video output settings              */
        dfb_visual_t                    visual;                                 /**< visual structure for video output  */
    
        bool setPostPluginParameter(map<string, xine_post_t*> plugins, string name, string parameter, string value);
    
    protected:
        MMSLogger                       logger;                                 /**< logging object                     */
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
        void open(xine_event_listener_cb_t queue_cb = NULL);
        void setStatus(int status);
        void sendEvent(int type);

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
        void play(const string mrl, const bool cont = true);
        void stop();
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
