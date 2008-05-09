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

#ifndef MMSTEMPLATECLASS_H_
#define MMSTEMPLATECLASS_H_

#include "mmsgui/theme/mmsthemebase.h"

//! describe attributes
namespace MMSGUI_TEMPLATE_ATTR {

	#define MMSGUI_TEMPLATE_ATTR_INIT { \
		MMSGUI_BASE_ATTR_ATTRDESC, \
		{ NULL, TAFF_ATTRTYPE_NONE } \
	}

	typedef enum {
		MMSGUI_BASE_ATTR_IDS
	} ids;
}

extern TAFF_ATTRDESC MMSGUI_TEMPLATE_ATTR_I[];


class MMSTemplateClass {
    private:
    	//! name of the theme class
        string          className;
        
        //! the copied TAFF buffer
        MMSTaffFile		*tafff;

    public:
        MMSTemplateClass();
        ~MMSTemplateClass();
        //
        void unsetAll();

        //! Read and set all attributes from the given TAFF buffer.
        /*!
        \param tafff   pointer to the TAFF buffer
        */
        void setAttributesFromTAFF(MMSTaffFile *tafff);

        //! Saves a copy of an TAFF buffer including all child nodes.
        /*!
        \param tafff   pointer to the TAFF buffer
        */
        void duplicateTAFF(MMSTaffFile *tafff);

        //! Get the copied TAFF buffer, see duplicateTAFF().
        /*!
        \return pointer to the TAFF buffer
        */
        MMSTaffFile *getTAFF();

        void setClassName(string className);
        string getClassName();
};

#endif /*MMSTEMPLATECLASS_H_*/
