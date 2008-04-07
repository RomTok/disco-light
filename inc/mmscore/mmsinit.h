#ifndef MMSINIT_H_
#define MMSINIT_H_

#include <stdlib.h>
#include <string>
#include <mmsbase/interfaces/immsswitcher.h>

typedef enum {
	MMSINIT_FULL = 0x00000000,
	MMSINIT_WINDOWMANAGER = 0x00000001, 
	MMSINIT_PLUGINMANAGER = 0x00000002, 
	MMSINIT_EVENTS = 0x00000003 
} MMSINIT_FLAGS;

bool mmsInit(MMSINIT_FLAGS flags, int argc = 0, char *argv[] = NULL, string configfile = "");

bool registerSwitcher(IMMSSwitcher *switcher);

#endif /*MMSINIT_H_*/
