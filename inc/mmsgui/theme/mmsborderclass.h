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

#ifndef MMSBORDERCLASS_H_
#define MMSBORDERCLASS_H_

#include "theme/mmsthemebase.h"
#include <directfb.h>

class MMSBorderClass {
    private:
    	struct {
	    	bool         iscolor;
	        DFBColor     color;
	        bool         isselcolor;
	        DFBColor     selcolor;
	        bool         isimagepath;
	        bool         isimagenames;
	        bool         isselimagepath;
	        bool         isselimagenames;
	        bool         isthickness;
	        unsigned int thickness;
	        bool         ismargin;
	        unsigned int margin;
	        bool         isrcorners;
	        bool         rcorners;
    	} id;

    	struct {
            string       *imagepath;
            string       *imagenames[8];
            string       *selimagepath;
            string       *selimagenames[8];
		} ed;

        /* init routines */
        void initColor();
        void initSelColor();

        void initImagePath();
        void initImageNames();
        void initSelImagePath();
        void initSelImageNames();

        void initThickness();
        void initMargin();
        void initRCorners();

        /* free routines */
        void freeColor();
        void freeSelColor();

        void freeImagePath();
        void freeImageNames();
        void freeSelImagePath();
        void freeSelImageNames();

        void freeThickness();
        void freeMargin();
        void freeRCorners();

    public:
        MMSBorderClass();
        ~MMSBorderClass();
        MMSBorderClass &operator=(const MMSBorderClass &c); 
        //
        void unsetAll();
        //
        void setAttributesFromXMLNode(xmlNode* node, string prefix = "", string path = "");
        //
        bool isColor();
        void unsetColor();
        void setColor(const DFBColor &color);
        bool getColor(DFBColor &color);
        //
        bool isSelColor();
        void unsetSelColor();
        void setSelColor(const DFBColor &selcolor);
        bool getSelColor(DFBColor &selcolor);
        //
        bool isImagePath();
        void unsetImagePath();
        void setImagePath(const string &imagepath);
        bool getImagePath(string &imagepath);
        //
        bool isImageNames();
        void unsetImageNames();
        void setImageNames(const string &imagename_1, const string &imagename_2, const string &imagename_3, const string &imagename_4,
        				   const string &imagename_5, const string &imagename_6, const string &imagename_7, const string &imagename_8);
        void setImageNames(unsigned int num, const string &imagename);
        bool getImageNames(unsigned int num, string &imagename);
        //
        bool isSelImagePath();
        void unsetSelImagePath();
        void setSelImagePath(const string &selimagepath);
        bool getSelImagePath(string &selimagepath);
        //
        bool isSelImageNames();
        void unsetSelImageNames();
        void setSelImageNames(const string &selimagename_1, const string &selimagename_2, const string &selimagename_3, const string &selimagename_4,
        					  const string &selimagename_5, const string &selimagename_6, const string &selimagename_7, const string &selimagename_8);
        void setSelImageNames(unsigned int num, const string &selimagename);
        bool getSelImageNames(unsigned int num, string &selimagename);
        //
        bool isThickness();
        void unsetThickness();
        void setThickness(unsigned int thickness);
        bool getThickness(unsigned int &thickness);
        //
        bool isMargin();
        void unsetMargin();
        void setMargin(unsigned int margin);
        bool getMargin(unsigned int &margin);
        //
        bool isRCorners();
        void unsetRCorners();
        void setRCorners(bool rcorners);
        bool getRCorners(bool &rcorners);
};

#endif /*MMSBORDERCLASS_H_*/
