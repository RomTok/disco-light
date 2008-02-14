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

#ifndef TOOLS_H_
#define TOOLS_H_

#include "mmstools/base.h"
#include "mmstools/mmslogbackend.h"
#include "mmstools/mmserror.h"
#include <stdarg.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

#ifdef __ENABLE_DEBUG__
#define DEBUGMSG(ident, msg...) {writeDebugMessage(ident, __FILE__, __LINE__, msg);}
#else
#define DEBUGMSG(ident, msg...) 
#endif

string substituteEnvVars(string input);

string maskChars(string str);

string *strToUpr(string *src);

string strToUpr(string src);

int hexToInt(const char *in);

bool getCurrentTimeBuffer(char *dtbuf, char *datebuf=NULL, char *timebuf=NULL,
                          time_t *clock=NULL);

bool getCurrentTimeString(string *dtstr, string *datestr=NULL, string *timestr=NULL,
                          time_t *clock=NULL);

string getDayOfWeek(time_t *clock=NULL);

void initLogging(char *Iam, char *logfile);

string getSimpleTimeString();

void writeMessage(const char *ctrl,...);

int strToInt(string s);

string iToStr(int i);

string fToStr(double i);

string cpToStr(char *);

string cToStr(char);

char *scanForString(char *buf, char *toFind, char **ret=NULL,
					int offset=0, unsigned int length=0);

char *scanForString(char *buf, char *toFind, string *ret=NULL,
					int offset=0, unsigned int length=0);

string scanForString(string buf, string toFind, string *ret=NULL,
					int offset=0, unsigned int length=0);

void msleep(unsigned long msec);

bool scanString(string toscan, string frontkey, string backkey,
                unsigned int offset, unsigned int length, string *result, unsigned int *nextpos);

void trim(string& str);

bool strToBool(string s);

void executeCmd(string cmd);

bool file_exist( string filename );

void writeDebugMessage(string identity, const char *filename, const int lineno, const char *msg, ...);

#endif /*TOOLS_H_*/
