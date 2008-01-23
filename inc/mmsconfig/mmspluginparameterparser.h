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

#ifndef MMSPLUGINPARAMETERPARSER_H_
#define MMSPLUGINPARAMETERPARSER_H_


#include "mmstools.h"
#include "mmsconfigdata.h"
#include "mmsplugindata.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

#define MMSPLUGINPARAMETERPARSER_ERROR_NOPARAMETERS 1

class MMSPluginParameterParser {
    private:
//        xmlpp::DomParser *parser;
        MMSLogger logger;

    public:
        MMSPluginParameterParser();
        ~MMSPluginParameterParser();

        bool validate(MMSPluginData *plugin);
        void createProperty(MMSPluginData *plugin,string name);
        
        //fills properites from the db whith unset properties from xml
        void fillProperties(MMSPluginData *plugin);
               
};

MMS_CREATEERROR(MMSPluginParameterParserError);


#endif /*MMSPLUGINPARAMETERPARSER_H_*/
