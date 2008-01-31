include ../../dep.def

#check private dependencies

#alsa
ifndef ALSA_LIBS
    $(info check for alsa)
    export ALSA_LIBS=$(shell pkg-config --libs alsa)
    ifeq ($(ALSA_LIBS),)
        $(error alsa was not found. Either adjust PKG_CONFIG_PATH or install the package)
    else
        export ALSA_INC=$(shell pkg-config --cflags alsa)
    endif
endif

SUBPACKAGE=mmsmedia
TARGET=libmmsmedia.so
LOCAL_INC=-I./inc -I../mmstools/inc  -I../mmsgui/inc -I../mmsconfig/inc -I./input_dvb_morphine/src $(SQLITE3_INC) $(CURL_INC) $(DIRECTFB_INC) $(SIGC_INC) $(ALSA_INC) $(XINE_INC) $(LIBXML_INC)
LOCAL_LIBS=-lmmstools  -lmmsconfig -lmmsgui $(SIGC_LIBS) $(ALSA_LIBS) $(XINE_LIBS) $(DIRECTFB_LIBS) $(LIBXML_LIBS)

include ../../shlib.def
