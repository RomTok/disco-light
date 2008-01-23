include ../../dep.def
SUBPACKAGE=mmscore
TARGET=libmmscore.so
LOCAL_INC=$(SQLITE3_INC) $(CURL_INC) $(MMSTOOLS_INC) $(SIGC_INC) -I../mmsbase/inc -I../mmsconfig/inc -I../mmstools/inc -I../mmsgui/inc -I../mmsmedia/inc $(LIBXML_INC) $(DIRECTFB_INC) 
LOCAL_LIBS=-lmmsbase -lmmsmedia -lmmsconfig
include ../../shlib.def
