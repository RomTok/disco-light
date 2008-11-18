include ../../dep.def
SUBPACKAGE=mmsgui
TARGET=libmmsgui.so
LOCAL_INC=$(CURL_INC) $(SIGC_INC) $(DIRECTFB_INC) $(X11_INC) -I/usr/include/freetype2
LOCAL_LIBS=-L/usr/X11R6/lib/ -lX11 -lXv -lfreetype -Xf86 -lXxf86vm
include ../../shlib.def
