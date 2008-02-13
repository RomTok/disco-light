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

#ifndef MMSDVD_H_
#define MMSDVD_H_

#include "mmsmedia/mmsav.h"

/**
 * @brief   Handles DVD playback.
 * 
 * @ingroup     mmsmedia mmslibs
 *
 * @author      Matthias Hardt (mattmax@morphine.tv)
 * @version     1.0.3
 * @date        11/07/2007
 * 
 * This class is derived from MMSAV and specialized in
 * handling DVD playback.
 */
class MMSDVD : public MMSAV {
    private:
        string  device;             /**< dvd device to use                      */
        int     audioChannel,       /**< current audio channel                  */
                spuChannel;         /**< current subtitle channel               */
        int     maxAudioChannels,   /**< number of available audio channels     */
                maxSpuChannels;     /**< number of available subtitle channels  */ 
    
        void checkDevice(const string device);
        void initialize(MMSWindow *window, const string device, const bool verbose);
        
    protected:
    
    public:
        /* status constants */
        static const unsigned short STATUS_PREVIOUS                 =  100;     /**< playing previous chapter           */
        static const unsigned short STATUS_NEXT                     =  101;     /**< playing next chapter               */
        static const unsigned short STATUS_EJECT                    =  102;     /**< ejecting DVD                       */
        static const unsigned short STATUS_ANGLE_PREVIOUS           =  103;     /**< setting previous angle             */
        static const unsigned short STATUS_ANGLE_NEXT               =  104;     /**< setting next angle                 */
        static const unsigned short STATUS_AUDIO_PREVIOUS           =  105;     /**< using previous audio channel       */
        static const unsigned short STATUS_AUDIO_NEXT               =  106;     /**< using next audio channel           */
        static const unsigned short STATUS_SPU_PREVIOUS             =  107;     /**< using previous subtitle channel    */
        static const unsigned short STATUS_SPU_NEXT                 =  108;     /**< using next subtitle channel        */
        

        MMSDVD(MMSWindow *window, const string device = "/dev/dvd", const bool verbose = false);
        ~MMSDVD();
        
        void open();
        void play(const bool cont = true);
        void pause();
        void rewind();
        void previous();
        void next();
        void anglePrevious();
        void angleNext();
        void audioChannelPrevious();
        void audioChannelNext();
        void spuChannelPrevious();
        void spuChannelNext();
        
        void eject();

		void menuUp();
		void menuDown();
		void menuLeft();
		void menuRight();
		void menuSelect();
		void showMainMenu();
		bool inMenu();

        /* get DVD information */
        string getTitle();
        int    getChapterNumber();
        int    getChapterCount();
        int    getTitleNumber();
        int    getTitleCount();
        
        void   updateChannelInfo();
};

#endif /*MMSDVD_H_*/
