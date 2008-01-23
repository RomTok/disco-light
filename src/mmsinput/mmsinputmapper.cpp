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

#include "mmsinputmapper.h"
#include "mmstools.h"
#include <iostream>
#include <stdlib.h>
#include <cerrno>

static  DirectFBKeySymbolNames(keynames);

static int compare_symbol( const void *a, const void *b ) {
     DFBInputDeviceKeySymbol *symbol  = (DFBInputDeviceKeySymbol *) a;
     struct DFBKeySymbolName *symname = (struct DFBKeySymbolName *) b;

     return *symbol - symname->symbol;
}

static int compare_symbol_byname( const void *a, const void *b ) {
     char *symbol  = (char *) a;
     struct DFBKeySymbolName *symname = (struct DFBKeySymbolName *) b;

     return strcmp(symbol,symname->name);
}

string MMSInputMapper::lookUpKeyName(DFBInputDeviceKeySymbol key) {
    struct DFBKeySymbolName     *symbol_name;

   	symbol_name = (DFBKeySymbolName *)bsearch( &key,keynames,sizeof(keynames) / sizeof(keynames[0]) - 1, sizeof(keynames[0]), compare_symbol);

   	return symbol_name->name;
}

DFBInputDeviceKeySymbol MMSInputMapper::lookUpKeySymbol(const string keyname) {
	logger.writeLog("got key: " + keyname);
	
	if(strToUpr(keyname) == "BACKSPACE") return DIKS_BACKSPACE;
	if(strToUpr(keyname) == "TAB") return DIKS_TAB;
	if(strToUpr(keyname) == "RETURN") return DIKS_RETURN;
	if(strToUpr(keyname) == "CANCEL") return DIKS_CANCEL;
	if(strToUpr(keyname) == "ESCAPE") return DIKS_ESCAPE;
	if(strToUpr(keyname) == "SPACE") return DIKS_SPACE;
	if(strToUpr(keyname) == "EXCLAMATION_MARK") return DIKS_EXCLAMATION_MARK;
	if(strToUpr(keyname) == "QUOTATION") return DIKS_QUOTATION;
	if(strToUpr(keyname) == "NUMBER_SIGN") return DIKS_NUMBER_SIGN;
	if(strToUpr(keyname) == "DOLLAR_SIGN") return DIKS_DOLLAR_SIGN;
	if(strToUpr(keyname) == "PERCENT_SIGN") return DIKS_PERCENT_SIGN;
	if(strToUpr(keyname) == "AMPERSAND") return DIKS_AMPERSAND;
	if(strToUpr(keyname) == "APOSTROPHE") return DIKS_APOSTROPHE;
	if(strToUpr(keyname) == "PARENTHESIS_LEFT") return DIKS_PARENTHESIS_LEFT;
	if(strToUpr(keyname) == "PARENTHESIS_RIGHT") return DIKS_PARENTHESIS_RIGHT;
	if(strToUpr(keyname) == "ASTERISK") return DIKS_ASTERISK;
	if(strToUpr(keyname) == "PLUS_SIGN") return DIKS_PLUS_SIGN;
	if(strToUpr(keyname) == "COMMA") return DIKS_COMMA;
	if(strToUpr(keyname) == "MINUS_SIGN") return DIKS_MINUS_SIGN;
	if(strToUpr(keyname) == "PERIOD") return DIKS_PERIOD;
	if(strToUpr(keyname) == "SLASH") return DIKS_SLASH;
	if(strToUpr(keyname) == "0") return DIKS_0;
	if(strToUpr(keyname) == "1") return DIKS_1;
	if(strToUpr(keyname) == "2") return DIKS_2;
	if(strToUpr(keyname) == "3") return DIKS_3;
	if(strToUpr(keyname) == "4") return DIKS_4;
	if(strToUpr(keyname) == "5") return DIKS_5;
	if(strToUpr(keyname) == "6") return DIKS_6;
	if(strToUpr(keyname) == "7") return DIKS_7;
	if(strToUpr(keyname) == "8") return DIKS_8;
	if(strToUpr(keyname) == "9") return DIKS_9;
	if(strToUpr(keyname) == "COLON") return DIKS_COLON;
	if(strToUpr(keyname) == "SEMICOLON") return DIKS_SEMICOLON;
	if(strToUpr(keyname) == "LESS_THAN_SIGN") return DIKS_LESS_THAN_SIGN;
	if(strToUpr(keyname) == "EQUALS_SIGN") return DIKS_EQUALS_SIGN;
	if(strToUpr(keyname) == "GREATER_THAN_SIGN") return DIKS_GREATER_THAN_SIGN;
	if(strToUpr(keyname) == "QUESTION_MARK") return DIKS_QUESTION_MARK;
	if(strToUpr(keyname) == "AT") return DIKS_AT;
	if(strToUpr(keyname) == "CAPITAL_A") return DIKS_CAPITAL_A;
	if(strToUpr(keyname) == "CAPITAL_B") return DIKS_CAPITAL_B;
	if(strToUpr(keyname) == "CAPITAL_C") return DIKS_CAPITAL_C;
	if(strToUpr(keyname) == "CAPITAL_D") return DIKS_CAPITAL_D;
	if(strToUpr(keyname) == "CAPITAL_E") return DIKS_CAPITAL_E;
	if(strToUpr(keyname) == "CAPITAL_F") return DIKS_CAPITAL_F;
	if(strToUpr(keyname) == "CAPITAL_G") return DIKS_CAPITAL_G;
	if(strToUpr(keyname) == "CAPITAL_H") return DIKS_CAPITAL_H;
	if(strToUpr(keyname) == "CAPITAL_I") return DIKS_CAPITAL_I;
	if(strToUpr(keyname) == "CAPITAL_J") return DIKS_CAPITAL_J;
	if(strToUpr(keyname) == "CAPITAL_K") return DIKS_CAPITAL_K;
	if(strToUpr(keyname) == "CAPITAL_L") return DIKS_CAPITAL_L;
	if(strToUpr(keyname) == "CAPITAL_M") return DIKS_CAPITAL_M;
	if(strToUpr(keyname) == "CAPITAL_N") return DIKS_CAPITAL_N;
	if(strToUpr(keyname) == "CAPITAL_O") return DIKS_CAPITAL_O;
	if(strToUpr(keyname) == "CAPITAL_P") return DIKS_CAPITAL_P;
	if(strToUpr(keyname) == "CAPITAL_Q") return DIKS_CAPITAL_Q;
	if(strToUpr(keyname) == "CAPITAL_R") return DIKS_CAPITAL_R;
	if(strToUpr(keyname) == "CAPITAL_S") return DIKS_CAPITAL_S;
	if(strToUpr(keyname) == "CAPITAL_T") return DIKS_CAPITAL_T;
	if(strToUpr(keyname) == "CAPITAL_U") return DIKS_CAPITAL_U;
	if(strToUpr(keyname) == "CAPITAL_V") return DIKS_CAPITAL_V;
	if(strToUpr(keyname) == "CAPITAL_W") return DIKS_CAPITAL_W;
	if(strToUpr(keyname) == "CAPITAL_X") return DIKS_CAPITAL_X;
	if(strToUpr(keyname) == "CAPITAL_Y") return DIKS_CAPITAL_Y;
	if(strToUpr(keyname) == "CAPITAL_Z") return DIKS_CAPITAL_Z;
	if(strToUpr(keyname) == "SQUARE_BRACKET_LEFT") return DIKS_SQUARE_BRACKET_LEFT;
	if(strToUpr(keyname) == "BACKSLASH") return DIKS_BACKSLASH;
	if(strToUpr(keyname) == "SQUARE_BRACKET_RIGHT") return DIKS_SQUARE_BRACKET_RIGHT;
	if(strToUpr(keyname) == "CIRCUMFLEX_ACCENT") return DIKS_CIRCUMFLEX_ACCENT;
	if(strToUpr(keyname) == "UNDERSCORE") return DIKS_UNDERSCORE;
	if(strToUpr(keyname) == "GRAVE_ACCENT") return DIKS_GRAVE_ACCENT;
	if(strToUpr(keyname) == "SMALL_A") return DIKS_SMALL_A;
	if(strToUpr(keyname) == "SMALL_B") return DIKS_SMALL_B;
	if(strToUpr(keyname) == "SMALL_C") return DIKS_SMALL_C;
	if(strToUpr(keyname) == "SMALL_D") return DIKS_SMALL_D;
	if(strToUpr(keyname) == "SMALL_E") return DIKS_SMALL_E;
	if(strToUpr(keyname) == "SMALL_F") return DIKS_SMALL_F;
	if(strToUpr(keyname) == "SMALL_G") return DIKS_SMALL_G;
	if(strToUpr(keyname) == "SMALL_H") return DIKS_SMALL_H;
	if(strToUpr(keyname) == "SMALL_I") return DIKS_SMALL_I;
	if(strToUpr(keyname) == "SMALL_J") return DIKS_SMALL_J;
	if(strToUpr(keyname) == "SMALL_K") return DIKS_SMALL_K;
	if(strToUpr(keyname) == "SMALL_L") return DIKS_SMALL_L;
	if(strToUpr(keyname) == "SMALL_M") return DIKS_SMALL_M;
	if(strToUpr(keyname) == "SMALL_N") return DIKS_SMALL_N;
	if(strToUpr(keyname) == "SMALL_O") return DIKS_SMALL_O;
	if(strToUpr(keyname) == "SMALL_P") return DIKS_SMALL_P;
	if(strToUpr(keyname) == "SMALL_Q") return DIKS_SMALL_Q;
	if(strToUpr(keyname) == "SMALL_R") return DIKS_SMALL_R;
	if(strToUpr(keyname) == "SMALL_S") return DIKS_SMALL_S;
	if(strToUpr(keyname) == "SMALL_T") return DIKS_SMALL_T;
	if(strToUpr(keyname) == "SMALL_U") return DIKS_SMALL_U;
	if(strToUpr(keyname) == "SMALL_V") return DIKS_SMALL_V;
	if(strToUpr(keyname) == "SMALL_W") return DIKS_SMALL_W;
	if(strToUpr(keyname) == "SMALL_X") return DIKS_SMALL_X;
	if(strToUpr(keyname) == "SMALL_Y") return DIKS_SMALL_Y;
	if(strToUpr(keyname) == "SMALL_Z") return DIKS_SMALL_Z;
	if(strToUpr(keyname) == "CURLY_BRACKET_LEFT") return DIKS_CURLY_BRACKET_LEFT;
	if(strToUpr(keyname) == "VERTICAL_BAR") return DIKS_VERTICAL_BAR;
	if(strToUpr(keyname) == "CURLY_BRACKET_RIGHT") return DIKS_CURLY_BRACKET_RIGHT;
	if(strToUpr(keyname) == "TILDE") return DIKS_TILDE;
	if(strToUpr(keyname) == "DELETE") return DIKS_DELETE;
	if(strToUpr(keyname) == "CURSOR_LEFT") return DIKS_CURSOR_LEFT;
	if(strToUpr(keyname) == "CURSOR_RIGHT") return DIKS_CURSOR_RIGHT;
	if(strToUpr(keyname) == "CURSOR_UP") return DIKS_CURSOR_UP;
	if(strToUpr(keyname) == "CURSOR_DOWN") return DIKS_CURSOR_DOWN;
	if(strToUpr(keyname) == "INSERT") return DIKS_INSERT;
	if(strToUpr(keyname) == "HOME") return DIKS_HOME;
	if(strToUpr(keyname) == "END") return DIKS_END;
	if(strToUpr(keyname) == "PAGE_UP") return DIKS_PAGE_UP;
	if(strToUpr(keyname) == "PAGE_DOWN") return DIKS_PAGE_DOWN;
	if(strToUpr(keyname) == "PRINT") return DIKS_PRINT;
	if(strToUpr(keyname) == "PAUSE") return DIKS_PAUSE;
	if(strToUpr(keyname) == "OK") return DIKS_OK;
	if(strToUpr(keyname) == "SELECT") return DIKS_SELECT;
	if(strToUpr(keyname) == "GOTO") return DIKS_GOTO;
	if(strToUpr(keyname) == "CLEAR") return DIKS_CLEAR;
	if(strToUpr(keyname) == "POWER") return DIKS_POWER;
	if(strToUpr(keyname) == "POWER2") return DIKS_POWER2;
	if(strToUpr(keyname) == "OPTION") return DIKS_OPTION;
	if(strToUpr(keyname) == "MENU") return DIKS_MENU;
	if(strToUpr(keyname) == "HELP") return DIKS_HELP;
	if(strToUpr(keyname) == "INFO") return DIKS_INFO;
	if(strToUpr(keyname) == "TIME") return DIKS_TIME;
	if(strToUpr(keyname) == "VENDOR") return DIKS_VENDOR;
	if(strToUpr(keyname) == "ARCHIVE") return DIKS_ARCHIVE;
	if(strToUpr(keyname) == "PROGRAM") return DIKS_PROGRAM;
	if(strToUpr(keyname) == "CHANNEL") return DIKS_CHANNEL;
	if(strToUpr(keyname) == "FAVORITES") return DIKS_FAVORITES;
	if(strToUpr(keyname) == "EPG") return DIKS_EPG;
	if(strToUpr(keyname) == "PVR") return DIKS_PVR;
	if(strToUpr(keyname) == "MHP") return DIKS_MHP;
	if(strToUpr(keyname) == "LANGUAGE") return DIKS_LANGUAGE;
	if(strToUpr(keyname) == "TITLE") return DIKS_TITLE;
	if(strToUpr(keyname) == "SUBTITLE") return DIKS_SUBTITLE;
	if(strToUpr(keyname) == "ANGLE") return DIKS_ANGLE;
	if(strToUpr(keyname) == "ZOOM") return DIKS_ZOOM;
	if(strToUpr(keyname) == "MODE") return DIKS_MODE;
	if(strToUpr(keyname) == "KEYBOARD") return DIKS_KEYBOARD;
	if(strToUpr(keyname) == "PC") return DIKS_PC;
	if(strToUpr(keyname) == "SCREEN") return DIKS_SCREEN;
	if(strToUpr(keyname) == "TV") return DIKS_TV;
	if(strToUpr(keyname) == "TV2") return DIKS_TV2;
	if(strToUpr(keyname) == "VCR") return DIKS_VCR;
	if(strToUpr(keyname) == "VCR2") return DIKS_VCR2;
	if(strToUpr(keyname) == "SAT") return DIKS_SAT;
	if(strToUpr(keyname) == "SAT2") return DIKS_SAT2;
	if(strToUpr(keyname) == "CD") return DIKS_CD;
	if(strToUpr(keyname) == "TAPE") return DIKS_TAPE;
	if(strToUpr(keyname) == "RADIO") return DIKS_RADIO;
	if(strToUpr(keyname) == "TUNER") return DIKS_TUNER;
	if(strToUpr(keyname) == "PLAYER") return DIKS_PLAYER;
	if(strToUpr(keyname) == "TEXT") return DIKS_TEXT;
	if(strToUpr(keyname) == "DVD") return DIKS_DVD;
	if(strToUpr(keyname) == "AUX") return DIKS_AUX;
	if(strToUpr(keyname) == "MP3") return DIKS_MP3;
	if(strToUpr(keyname) == "PHONE") return DIKS_PHONE;
	if(strToUpr(keyname) == "AUDIO") return DIKS_AUDIO;
	if(strToUpr(keyname) == "VIDEO") return DIKS_VIDEO;
	if(strToUpr(keyname) == "INTERNET") return DIKS_INTERNET;
	if(strToUpr(keyname) == "MAIL") return DIKS_MAIL;
	if(strToUpr(keyname) == "NEWS") return DIKS_NEWS;
	if(strToUpr(keyname) == "DIRECTORY") return DIKS_DIRECTORY;
	if(strToUpr(keyname) == "LIST") return DIKS_LIST;
	if(strToUpr(keyname) == "CALCULATOR") return DIKS_CALCULATOR;
	if(strToUpr(keyname) == "MEMO") return DIKS_MEMO;
	if(strToUpr(keyname) == "CALENDAR") return DIKS_CALENDAR;
	if(strToUpr(keyname) == "EDITOR") return DIKS_EDITOR;
	if(strToUpr(keyname) == "RED") return DIKS_RED;
	if(strToUpr(keyname) == "GREEN") return DIKS_GREEN;
	if(strToUpr(keyname) == "YELLOW") return DIKS_YELLOW;
	if(strToUpr(keyname) == "BLUE") return DIKS_BLUE;
	if(strToUpr(keyname) == "CHANNEL_UP") return DIKS_CHANNEL_UP;
	if(strToUpr(keyname) == "CHANNEL_DOWN") return DIKS_CHANNEL_DOWN;
	if(strToUpr(keyname) == "BACK") return DIKS_BACK;
	if(strToUpr(keyname) == "FORWARD") return DIKS_FORWARD;
	if(strToUpr(keyname) == "FIRST") return DIKS_FIRST;
	if(strToUpr(keyname) == "LAST") return DIKS_LAST;
	if(strToUpr(keyname) == "VOLUME_UP") return DIKS_VOLUME_UP;
	if(strToUpr(keyname) == "VOLUME_DOWN") return DIKS_VOLUME_DOWN;
	if(strToUpr(keyname) == "MUTE") return DIKS_MUTE;
	if(strToUpr(keyname) == "AB") return DIKS_AB;
	if(strToUpr(keyname) == "PLAYPAUSE") return DIKS_PLAYPAUSE;
	if(strToUpr(keyname) == "PLAY") return DIKS_PLAY;
	if(strToUpr(keyname) == "STOP") return DIKS_STOP;
	if(strToUpr(keyname) == "RESTART") return DIKS_RESTART;
	if(strToUpr(keyname) == "SLOW") return DIKS_SLOW;
	if(strToUpr(keyname) == "FAST") return DIKS_FAST;
	if(strToUpr(keyname) == "RECORD") return DIKS_RECORD;
	if(strToUpr(keyname) == "EJECT") return DIKS_EJECT;
	if(strToUpr(keyname) == "SHUFFLE") return DIKS_SHUFFLE;
	if(strToUpr(keyname) == "REWIND") return DIKS_REWIND;
	if(strToUpr(keyname) == "FASTFORWARD") return DIKS_FASTFORWARD;
	if(strToUpr(keyname) == "PREVIOUS") return DIKS_PREVIOUS;
	if(strToUpr(keyname) == "NEXT") return DIKS_NEXT;
	if(strToUpr(keyname) == "BEGIN") return DIKS_BEGIN;
	if(strToUpr(keyname) == "DIGITS") return DIKS_DIGITS;
	if(strToUpr(keyname) == "TEEN") return DIKS_TEEN;
	if(strToUpr(keyname) == "TWEN") return DIKS_TWEN;
	if(strToUpr(keyname) == "BREAK") return DIKS_BREAK;
	if(strToUpr(keyname) == "EXIT") return DIKS_EXIT;
	if(strToUpr(keyname) == "SETUP") return DIKS_SETUP;
	if(strToUpr(keyname) == "CURSOR_LEFT_UP") return DIKS_CURSOR_LEFT_UP;
	if(strToUpr(keyname) == "CURSOR_LEFT_DOWN") return DIKS_CURSOR_LEFT_DOWN;
	if(strToUpr(keyname) == "CURSOR_UP_RIGHT") return DIKS_CURSOR_UP_RIGHT;
	if(strToUpr(keyname) == "CURSOR_DOWN_RIGHT") return DIKS_CURSOR_DOWN_RIGHT;
	if(strToUpr(keyname) == "F1") return DIKS_F1;
	if(strToUpr(keyname) == "F2") return DIKS_F2;
	if(strToUpr(keyname) == "F3") return DIKS_F3;
	if(strToUpr(keyname) == "F4") return DIKS_F4;
	if(strToUpr(keyname) == "F5") return DIKS_F5;
	if(strToUpr(keyname) == "F6") return DIKS_F6;
	if(strToUpr(keyname) == "F7") return DIKS_F7;
	if(strToUpr(keyname) == "F8") return DIKS_F8;
	if(strToUpr(keyname) == "F9") return DIKS_F9;
	if(strToUpr(keyname) == "F10") return DIKS_F10;
	if(strToUpr(keyname) == "F11") return DIKS_F11;
	if(strToUpr(keyname) == "F12") return DIKS_F12;
	if(strToUpr(keyname) == "SHIFT") return DIKS_SHIFT;
	if(strToUpr(keyname) == "CONTROL") return DIKS_CONTROL;
	if(strToUpr(keyname) == "ALT") return DIKS_ALT;
	if(strToUpr(keyname) == "ALTGR") return DIKS_ALTGR;
	if(strToUpr(keyname) == "META") return DIKS_META;
	if(strToUpr(keyname) == "SUPER") return DIKS_SUPER;
	if(strToUpr(keyname) == "HYPER") return DIKS_HYPER;
	if(strToUpr(keyname) == "CAPS_LOCK") return DIKS_CAPS_LOCK;
	if(strToUpr(keyname) == "NUM_LOCK") return DIKS_NUM_LOCK;
	if(strToUpr(keyname) == "SCROLL_LOCK") return DIKS_SCROLL_LOCK;
	if(strToUpr(keyname) == "DEAD_ABOVEDOT") return DIKS_DEAD_ABOVEDOT;
	if(strToUpr(keyname) == "DEAD_ABOVERING") return DIKS_DEAD_ABOVERING;
	if(strToUpr(keyname) == "DEAD_ACUTE") return DIKS_DEAD_ACUTE;
	if(strToUpr(keyname) == "DEAD_BREVE") return DIKS_DEAD_BREVE;
	if(strToUpr(keyname) == "DEAD_CARON") return DIKS_DEAD_CARON;
	if(strToUpr(keyname) == "DEAD_CEDILLA") return DIKS_DEAD_CEDILLA;
	if(strToUpr(keyname) == "DEAD_CIRCUMFLEX") return DIKS_DEAD_CIRCUMFLEX;
	if(strToUpr(keyname) == "DEAD_DIAERESIS") return DIKS_DEAD_DIAERESIS;
	if(strToUpr(keyname) == "DEAD_DOUBLEACUTE") return DIKS_DEAD_DOUBLEACUTE;
	if(strToUpr(keyname) == "DEAD_GRAVE") return DIKS_DEAD_GRAVE;
	if(strToUpr(keyname) == "DEAD_IOTA") return DIKS_DEAD_IOTA;
	if(strToUpr(keyname) == "DEAD_MACRON") return DIKS_DEAD_MACRON;
	if(strToUpr(keyname) == "DEAD_OGONEK") return DIKS_DEAD_OGONEK;
	if(strToUpr(keyname) == "DEAD_SEMIVOICED_SOUND") return DIKS_DEAD_SEMIVOICED_SOUND;
	if(strToUpr(keyname) == "DEAD_TILDE") return DIKS_DEAD_TILDE;
	if(strToUpr(keyname) == "DEAD_VOICED_SOUND") return DIKS_DEAD_VOICED_SOUND;
	if(strToUpr(keyname) == "CUSTOM0") return DIKS_CUSTOM0;
	if(strToUpr(keyname) == "CUSTOM1") return DIKS_CUSTOM1;
	if(strToUpr(keyname) == "CUSTOM2") return DIKS_CUSTOM2;
	if(strToUpr(keyname) == "CUSTOM3") return DIKS_CUSTOM3;
	if(strToUpr(keyname) == "CUSTOM4") return DIKS_CUSTOM4;
	if(strToUpr(keyname) == "CUSTOM5") return DIKS_CUSTOM5;
	if(strToUpr(keyname) == "CUSTOM6") return DIKS_CUSTOM6;
	if(strToUpr(keyname) == "CUSTOM7") return DIKS_CUSTOM7;
	if(strToUpr(keyname) == "CUSTOM8") return DIKS_CUSTOM8;
	if(strToUpr(keyname) == "CUSTOM9") return DIKS_CUSTOM9;
	if(strToUpr(keyname) == "NULL") return DIKS_NULL;

   	return (DFBInputDeviceKeySymbol)0;
}

MMSInputMapper::MMSInputMapper(string mapfile, string name) {
	xmlNode *walkNode = NULL;
	xmlNode *curNode = NULL;
    logger.setIdentity("MMSINPUT");

	/* map the keys */
	try {
		LIBXML_TEST_VERSION
				
		parser = xmlReadFile((char *)mapfile.c_str(), NULL, 0);		
			
		if(parser == NULL) {
			throw new MMSError(1, "Could not parse file:" + mapfile);
		}
		else {
			
			xmlNode* pNode = xmlDocGetRootElement(parser);
            
			// check if incorrent input mapfile found
			if(xmlStrcmp(pNode->name, (const xmlChar *) "mmsinputmaps")) {
				std::cout << "invalid mapfile (" << mapfile << ") - does not contain mmsinputmaps root node" << std::endl;
				throw new MMSError(1, "invalid file");
			}
			
			pNode = pNode->xmlChildrenNode;

			// need char buffers fpr mapping information
			xmlChar *keyName;
			xmlChar *mapTo;
			
			// walk trough the child node, until found the correct map node
			for (walkNode = pNode; walkNode; walkNode = walkNode->next) {

				if(!xmlStrcmp(walkNode->name, (const xmlChar *) name.c_str())) {
					logger.writeLog("using mapping set of " + name + " node");
							
					walkNode = walkNode->xmlChildrenNode;
					for (curNode = walkNode; curNode; curNode = curNode->next) {
						if(xmlStrcmp(curNode->name, (const xmlChar *) "key"))
							continue;
								
						keyName = xmlGetProp(curNode, (const xmlChar*)"name");
						mapTo   = xmlGetProp(curNode, (const xmlChar*)"mapto");
					    
					    this->keyMap.insert(pair<string, string> (string((const char *) keyName), string((const char *)mapTo)));
					    	
						xmlFree(keyName);
						xmlFree(mapTo);
					}
				}
				else {
					//ignore this node
					logger.writeLog("Ignore mapping set of " + name + " node");
				}
			}
        }
	}
	catch(MMSError *error) {
        logger.writeLog("Error loading inputmaps (" + mapfile + "." + name + ") [" + error->getMessage() + "]");
	}

    logger.writeLog("Parsing inputmap finished.");
}

MMSInputMapper::~MMSInputMapper() {
	
	this->keyMap.clear();
	delete this->parser;
}

void MMSInputMapper::mapkey(MMSInputEvent *inputevent, vector<MMSInputEvent> *inputeventset) {
    string symbol = lookUpKeyName(inputevent->key);
    MMSInputEvent evt; 

	/* parse the result nodes */
    for( multimap<string, string>::iterator iter = this->keyMap.begin(); iter != this->keyMap.end(); ++iter ) {
    	if(strToUpr(iter->first) == symbol) {
    		string foundkeyname = iter->second;
    	    DFBInputDeviceKeySymbol foundkey = lookUpKeySymbol(foundkeyname);
    	    if(foundkey) {
    	    	logger.writeLog("mapped to key '" + foundkeyname + "', id: " + iToStr(foundkey));
    	        evt.type = MMSINPUTEVENTTYPE_KEYPRESS;
    	        evt.key = foundkey;
    	        inputeventset->push_back(evt);
    	    }
    	}
    }

    if(inputeventset->empty()) {
        inputeventset->push_back(*inputevent);
	}
}


