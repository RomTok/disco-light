/***************************************************************************
 *   Copyright (C) 2005-2008 by Berlinux Solutions                         *
 *                                                                         *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>                *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
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

#include "mms.h"


void help() {
	printf("\nTAFF Converter\n\n");
	printf("parameter:\n\n");
	printf("--mode <mode>             specifies what is to do\n");
	printf("  --mode c2t              convert external file to taff file\n");
	printf("  --mode c2e              convert taff file to external file\n");
	printf("  --mode tc2t             test the convertion of external file to taff file\n");
	printf("  --mode tc2e             test the convertion of taff file to external file\n");
	printf("--tafffile <filename>     file for reading/writing the taff format\n");
	printf("--tafftype mmsgui         type of the taff file, default is mmsgui\n");
	printf("--extfile <filename>      file for reading/writing the external format\n");
	printf("--exttype xml|image       type of the external file, default is xml\n");
	printf("                          (we do only support PNG images for exttype image)\n");
	printf("--ignore_blanks yes|no    ignore blank values?, default no\n");
	printf("--trace yes|no            print trace messages?, default no\n");
	printf("--warnings yes|no         print warnings?, default yes\n");
	printf("\nexamples:\n\n");
	printf("taff --mode c2t --tafffile theme.taff --extfile theme.xml\n\n");
	printf("taff --mode c2t --exttype image --tafffile myimage.taff --extfile myimage.png\n\n");
}

bool getparams(int argc, char *argv[],
			   string &mode, string &tafffile, TAFF_DESCRIPTION **tafftype,
			   string &extfile, MMSTAFF_EXTERNAL_TYPE *exttype, bool &ignore_blanks, bool &trace, bool &print_warnings) {
	
	if ((argc<3)||((argc-1)%2)) {
		printf("Error: wrong parameter list\n");
		return false;
	}

	*tafftype = &mmsgui_taff_description;
	*exttype = MMSTAFF_EXTERNAL_TYPE_XML;
	ignore_blanks = false;
	trace = false;
	print_warnings = true;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--mode")==0)
		{
			i++;
			if   ((strcmp(argv[i], "c2t")==0)
				||(strcmp(argv[i], "c2e")==0)
				||(strcmp(argv[i], "tc2t")==0)
				||(strcmp(argv[i], "tc2e")==0))
				mode = argv[i];
			else {
				printf("Error: --mode is invalid\n");
				return false;
			}
		}
		else
		if (strcmp(argv[i], "--tafffile")==0)
		{
			i++;
			tafffile = argv[i];
		}
		else
		if (strcmp(argv[i], "--tafftype")==0)
		{
			i++;
			if (strcmp(argv[i], "mmsgui")==0)
				*tafftype = &mmsgui_taff_description;
			else {
				printf("Error: --tafftype is invalid\n");
				return false;
			}
		}
		else
		if (strcmp(argv[i], "--extfile")==0)
		{
			i++;
			extfile = argv[i];
		}
		else
		if (strcmp(argv[i], "--exttype")==0)
		{
			i++;
			if (strcmp(argv[i], "xml")==0)
				*exttype = MMSTAFF_EXTERNAL_TYPE_XML;
			else
			if (strcmp(argv[i], "image")==0) {
				*exttype = MMSTAFF_EXTERNAL_TYPE_IMAGE;
				*tafftype = NULL;
			}
			else {
				printf("Error: --exttype is invalid\n");
				return false;
			}
		}
		else
		if (strcmp(argv[i], "--ignore_blanks")==0)
		{
			i++;
			if (strcmp(argv[i], "yes")==0)
				ignore_blanks = true;
			else
			if (strcmp(argv[i], "no")==0)
				ignore_blanks = false;
			else {
				printf("Error: --ignore_blanks is invalid\n");
				return false;
			}
		}
		else
		if (strcmp(argv[i], "--trace")==0)
		{
			i++;
			if (strcmp(argv[i], "yes")==0)
				trace = true;
			else
			if (strcmp(argv[i], "no")==0)
				trace = false;
			else {
				printf("Error: --trace is invalid\n");
				return false;
			}
		}
		else
		if (strcmp(argv[i], "--warnings")==0)
		{
			i++;
			if (strcmp(argv[i], "yes")==0)
				print_warnings = true;
			else
			if (strcmp(argv[i], "no")==0)
				print_warnings = false;
			else {
				printf("Error: --warnings is invalid\n");
				return false;
			}
		}
		else {
			printf("Error: unknown parameter %s\n", argv[i]);
			return false;
		}
	}

	if (mode=="") {
		printf("Error: --mode is not specified\n");
		return false;
	}
	
	return true;
}


int main(int argc, char *argv[]) {

	string mode, tafffile, extfile;
	bool ignore_blanks, trace, print_warnings;
	TAFF_DESCRIPTION *tafftype;
	MMSTAFF_EXTERNAL_TYPE exttype;
	
	if (!getparams(argc, argv, mode, tafffile, &tafftype, extfile, &exttype, ignore_blanks, trace, print_warnings)) {
		help();
		return 1;
	}

	if (mode=="c2t") {
		/* convert to taff */

		printf("\nCreating TAFF...\n");
		if (tafffile=="") {
			printf("Error: taff file not specified\n");
			return 1;
		}
		if (extfile=="") {
			printf("Error: external file not specified\n");
			return 1;
		}
		
		MMSTaffFile *tafff = new MMSTaffFile(tafffile, tafftype, extfile, exttype, ignore_blanks, trace, print_warnings, true);
		if (!tafff) {
			printf("Error: creating MMSTaffFile()\n");
			help();
			return 2;
		}
		if (!tafff->isLoaded()) {
			printf("Error: creating MMSTaffFile()\n");
			help();
			return 2;
		}

		printf("TAFF created.\n");
		
		delete tafff;
		return 0;
	}
	else
	if (mode=="tc2t") {
		/* convert to taff - test mode */

		printf("\nCreating TAFF - test mode...\n");
		if (extfile=="") {
			printf("Error: external file not specified\n");
			return 1;
		}
		
		MMSTaffFile *tafff = new MMSTaffFile("", tafftype, "", exttype, ignore_blanks, trace, print_warnings, true);
		if (!tafff) {
			printf("Error: creating MMSTaffFile()\n");
			help();
			return 2;
		}
		
		tafff->setExternal(extfile, exttype);
		if (!tafff->convertExternal2TAFF()) {
			printf("Error: conversion external to TAFF\n");
			return 2;
		}

		printf("Creation test was successful.\n");
		
		delete tafff;
		return 0;
	}
	else
	if (mode=="c2e") {
		/* convert to external */

		printf("\nCreating external file...\n");
		if (tafffile=="") {
			printf("Error: taff file not specified\n");
			return 1;
		}
		if (extfile=="") {
			printf("Error: external file not specified\n");
			return 1;
		}
		
		MMSTaffFile *tafff = new MMSTaffFile(tafffile, tafftype, "", exttype, ignore_blanks, trace, print_warnings, false);
		if (!tafff) {
			printf("Error: creating MMSTaffFile()\n");
			help();
			return 2;
		}
		if (!tafff->isLoaded()) {
			if (!tafff->checkVersion())
				printf("Error: taff file has wrong format/version\n");
			else
				printf("Error: cannot load taff file\n");
			help();
			return 2;
		}

		tafff->setExternal(extfile, exttype);
		if (!tafff->convertTAFF2External())
			return 2;
		
		printf("External file created.\n");
		
		delete tafff;
		return 0;
	}
	else
	if (mode=="tc2e") {
		/* convert to external - test mode */

		printf("\nCreating external file - test mode...\n");
		if (tafffile=="") {
			printf("Error: taff file not specified\n");
			return 1;
		}

		MMSTaffFile *tafff = new MMSTaffFile(tafffile, tafftype, "", exttype, ignore_blanks, trace, print_warnings, false);
		if (!tafff) {
			printf("Error: creating MMSTaffFile()\n");
			help();
			return 2;
		}
		if (!tafff->isLoaded()) {
			if (!tafff->checkVersion())
				printf("Error: taff file has wrong verion\n");
			else
				printf("Error: cannot load taff file\n");
			help();
			return 2;
		}

		if (!tafff->convertTAFF2External()) {
			printf("Error: conversion TAFF to external\n");
			return 2;
		}

		printf("Creation test was successful.\n");
		
		delete tafff;
		return 0;
	}
	
	
	return 0;
}

