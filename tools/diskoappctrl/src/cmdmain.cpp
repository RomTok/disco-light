/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mms.h"
#include "cmd.h"
#include "console.h"

#include <libgen.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

using namespace std;

bool isdigit(char *what) {
    if(atoi(what)>0)
        return true;

    return false;
}


void usage() {
	Console cons(true);

	printf("\nusage\n");
	printf("\tcmd [command] [arguments]\n\n");
	cons.printText("This command modifies the application installation to suit your needs. To operate it properly you need to point to the application config file using the parmeter @[1mconfig@[0m pointing to the configuration file.");
	printf("\tcommands:\n\n");
	cons.printCommand("list","Prints a list of all plugins and/or parameters. There is one argument: @[1mtype@[0m=all|imports. type=all lists all plugins and parameters, type=import lists all import plugins and source defintions. If no argument is specified type=all will be assumed.");
    cons.printCommand("act","Activates a plugin within the application. There is one argument: @[1mplugin@[0m=name|id.");
    cons.printExample("cmd act plugin=Mixer");
	cons.printCommand("      ","This will acivate the plugin named Mixer. At the next startup of the application this plugin will be loaded.");
	cons.printCommand("deact","Deactivates a plugin within the application. There is one argument: @[1mplugin@[0m=name|id.");
    cons.printExample("cmd deact plugin=Mixer");
	cons.printCommand("      ","This will deacivate the plugin named Mixer. At the next startup of the application this plugin will no longer be loaded.");

	cons.printCommand("update","Updates a plugin parameter or an import source values. To update a plugin parameter you must supply a plugin name or id (@[1mplugin@[0m=name|id) a parameter (@[1mparam@[0m=name) and a parameter vaue (@[1mvalue@[0m=value). To Update an import source you have to supply an import source (@[1msource@[0m=name|id) and a value.");
	cons.printExample("cmd update plugin=Mixer param=channel value=PCM ");
	cons.printCommand("      ","This will update the parameter \"channel\" of the plugin named Mixer to the new value \"PCM\"");
    cons.printExample("cmd update source=music value=/media/data/music ");
    cons.printCommand("      ","This will update the import source with the name \"music\" to the specified value");
    cons.printCommand("execute","executes an action within the application. Currently only import plugins can be executed.");
    cons.printExample("cmd execute import=ChannelImport");
    cons.printCommand("      ","This will execute the import plugin named \"ChannelImport\" once.");
	exit(1);
}


void printworkingdata(MMSConfigData *config) {
	printf("\nPrefix:    %s\n",config->getPrefix().c_str());
	printf("Data DB:   %s\n",config->getDataDBDatabase().c_str());
	printf("Config DB: %s\n\n",config->getConfigDBDatabase().c_str());
}

void parseCommandLine(CmdLine &cmdline, int argc, char **argv) {
	Console cons(true);
	char tmpcmd[1024];

	if(argc<2) {
		usage();
		exit(1);
	}

	// go for command
	if(strncmp(argv[1],MMSCMD_CMD, strlen(MMSCMD_CMD))==0) {

		if(strcmp(&(argv[1][strlen(MMSCMD_CMD)+1]),MMSCMD_LIST)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,&(argv[1][strlen(MMSCMD_CMD)+1])));
		else if(strcmp(&(argv[1][strlen(MMSCMD_CMD)+1]),MMSCMD_UPDATE)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,&(argv[1][strlen(MMSCMD_CMD)+1])));
		else if(strcmp(&(argv[1][strlen(MMSCMD_CMD)+1]),MMSCMD_ACT)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,&(argv[1][strlen(MMSCMD_CMD)+1])));
		else if(strcmp(&(argv[1][strlen(MMSCMD_CMD)+1]),MMSCMD_DEACT)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,&(argv[1][strlen(MMSCMD_CMD)+1])));
		else if(strcmp(&(argv[1][strlen(MMSCMD_CMD)+1]),MMSCMD_EXEC)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,&(argv[1][strlen(MMSCMD_CMD)+1])));
		else if(strcmp(&(argv[1][strlen(MMSCMD_CMD)+1]),MMSCMD_EVENT)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,&(argv[1][strlen(MMSCMD_CMD)+1])));

	} else {

		if(strcmp(argv[1],MMSCMD_LIST)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,argv[1]));
		else if(strcmp(argv[1],MMSCMD_UPDATE)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,argv[1]));
		else if(strcmp(argv[1],MMSCMD_ACT)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,argv[1]));
		else if(strcmp(argv[1],MMSCMD_DEACT)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,argv[1]));
		else if(strcmp(argv[1],MMSCMD_EXEC)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,argv[1]));
		else if(strcmp(argv[1],MMSCMD_EVENT)==0)
			cmdline.insert(make_pair(MMSCMD_CMD,argv[1]));

	}

	if(cmdline.size()==0) {
		cons.linefeed();
		cons.printError("No or wrong command given. see cmd --help for more information.");
		exit(1);
	}

	//go for the rest
	for(int i = 2; i<argc;i++) {
		char *srch = strstr(argv[i],"=");
		if(srch==NULL) {
			cons.printError("wrong argument format. See cmd --help for more information.");
			exit(1);
		}

		memset(tmpcmd,0,1024);
		strncpy(tmpcmd,argv[i],srch-argv[i]);
		if(strcmp(tmpcmd,MMSCMD_PLUGIN)!=0) {
			if(strcmp(tmpcmd,MMSCMD_SOURCE)!=0) {
				if(strcmp(tmpcmd,MMSCMD_PLUGIN)!=0) {
					if(strcmp(tmpcmd,MMSCMD_IMPORT)!=0) {
						if(strcmp(tmpcmd,MMSCMD_DEBUG)!=0) {
							if(strcmp(tmpcmd,MMSCMD_TYPE)!=0) {
								if(strcmp(tmpcmd,MMSCMD_VALUE)!=0) {
									if(strcmp(tmpcmd,MMSCMD_CONFIG)!=0) {
										if(strcmp(tmpcmd,MMSCMD_PARAM)!=0) {
											if(strcmp(tmpcmd,MMSCMD_HEADING)!=0) {
												if(strcmp(tmpcmd,MMSCMD_PORT)!=0) {
													char text[1024];
													sprintf(text, "unknown parameter @[1m%s@[0m. See cmd --help for more information.",tmpcmd);
													cons.printError(text);
													exit(1);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		srch++;
		cmdline.insert(make_pair(tmpcmd, srch));
	}

}

int main(int argc, char *argv[]) {
	CmdLine cmdline;
	Cmd *cmd;


    if(argc<2)
    	usage();
    else {
    	if(strcmp(argv[1],"--help")==0)
    		usage();
    }

    try {
    	parseCommandLine(cmdline,argc,argv);

        cmd = new Cmd(cmdline);
        cmd->handleRequest();
        exit(0);

    } catch(MMSError *err) {
        printf("\nAn internal error occured: %s\n",err->getMessage().c_str());
    }

}

