include ../../dep.def
SUBPACKAGE=mmsmedia
TARGET=libmmsmedia.so
LOCAL_INC=-I./inc -I../mmstools/inc  -I../mmsgui/inc -I../mmsconfig/inc -I./input_dvb_morphine/src $(SQLITE3_INC) $(CURL_INC) $(DIRECTFB_INC) $(SIGC_INC) $(ALSA_INC) $(XINE_INC) $(LIBXML_INC)
LOCAL_LIBS=-lmmstools  -lmmsconfig -lmmsgui $(SIGC_LIBS) $(ALSA_LIBS) $(XINE_LIBS) $(DIRECTFB_LIBS) $(LIBXML_LIBS)
include ../../shlib.def
