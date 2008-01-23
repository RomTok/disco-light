include ../../dep.def

#check private dependencies
$(info check for alsa)
ASK=$(shell pkg-config --list-all |grep alsa)
ifeq ($(ASK),)
$(error alsa was not found. Either adjust PKG_CONFIG_PATH or install the package)
endif
$(info check for xine-lib)
ASK=$(shell pkg-config --list-all |grep libxine)
ifeq ($(ASK),)
$(error xine-lib was not found. Either adjust PKG_CONFIG_PATH or install the package)
endif

#libxine
ifndef XINE_LIBS
export XINE_LIBS=$(shell pkg-config --libs libxine)
endif

#libxine includes
ifndef XINE_INC
export XINE_INC=$(shell pkg-config --cflags libxine)
endif

#alsa
ifndef ALSA_LIBS
export ALSA_LIBS=$(shell pkg-config --libs alsa)
endif

#alsa includes
ifndef ALSA_INC
export ALSA_INC=$(shell pkg-config --cflags alsa)
endif

SUBPACKAGE=mmsmedia
TARGET=libmmsmedia.so
LOCAL_INC=-I./inc -I../mmstools/inc  -I../mmsgui/inc -I../mmsconfig/inc -I./input_dvb_morphine/src $(SQLITE3_INC) $(CURL_INC) $(DIRECTFB_INC) $(SIGC_INC) $(ALSA_INC) $(XINE_INC) $(LIBXML_INC)
LOCAL_LIBS=-lmmstools  -lmmsconfig -lmmsgui $(SIGC_LIBS) $(ALSA_LIBS) $(XINE_LIBS) $(DIRECTFB_LIBS) $(LIBXML_LIBS)

include ../../shlib.def
