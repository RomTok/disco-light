# check for arch
ifeq ($(shell uname -m),x86_64)
export CFLAGS += -fPIC
endif

ifeq ($(nodep),)
include dep.def
endif

#project name
ifndef PROJECTNAME
    export PROJECTNAME=disko
    export PROJECTVERSION=1.1.1
endif

all clean update:
	@+make -C  ./src/mmsinfo   $@
	@+make -C  ./src/mmstools  $@
	@+make -C  ./src/mmsconfig $@
	@+make -C  ./src/mmsgui    $@
	@+make -C  ./src/mmsbase   $@
	@+make -C  ./src/mmsinput  $@
	@+make -C  ./src/mmsmedia  $@
	@+make -C  ./src/mmscore   $@
	@+make -C  ./tools/taff    $@

install-disko:
	@+make -C  ./src/mmstools  install
	@+make -C  ./src/mmsconfig install
	@+make -C  ./src/mmsgui    install
	@+make -C  ./src/mmsbase   install
	@+make -C  ./src/mmsinput  install
	@+make -C  ./src/mmsmedia  install
	@+make -C  ./src/mmscore   install
	@+make -C  ./src/mmsinfo   install
	@+make -C  ./tools/taff    install
	@install -T -m 644 ./inc/mms.h $(DESTDIR)$(incdir)/mms.h
	@install -d "$(DESTDIR)$(datadir)"
	@pushd share &> /dev/null; \
		for i in `find . -type f | grep -v /CVS/`; do \
			dir=`dirname $$i`; \
			install -d "$(DESTDIR)$(datadir)/$$dir"; \
			install -c -m 644 "$$i" "$(DESTDIR)$(datadir)/$$dir/"; \
		done; \
	popd &> /dev/null;

install-pkgconfig:
	@echo "writing pkgconfig information"
	@install -d "$(DESTDIR)$(libdir)/../pkgconfig"
	@echo "prefix=$(prefix)" > $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc
	@echo "exec_prefix=\$${prefix}/bin" >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc
	@echo "libdir=\$${prefix}/lib/disko" >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc
	@echo "includedir=\$${prefix}/include/disko" >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc
	@echo  >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc
	@echo "Name: $(PROJECTNAME)" >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc
	@echo "Description: Multimedia Framework on top of DirectFB" >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc
	@echo "Version: 1.1.1" >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc
	@echo "Requires: libxml-2.0 >= 2.6, sqlite3, libcurl, directfb, sigc++-2.0, libxine, alsa" >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc
	@echo "Libs: $(LDFLAGS) -L\$${libdir} -lmmsbase -lmmsconfig -lmmscore -lmmsgui -lmmsinput -lmmsmedia -lmmstools -lmmsinfo $(LIBXML_LIBS) $(SQLITE3_LIBS) $(FREETDS_LIBS) $(DIRECTFB_LIBS) $(XINE_LIBS) $(SIGC_LIBS) $(XINE_LIBS)" >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc 
	@echo "Cflags: $(CFLAGS) -I\$${includedir} $(LIBXML_INC) $(SQLITE3_INC) $(FREETDS_INC) $(DIRECTFB_INC) $(XINE_INC) $(SIGC_INC) $(XINE_INC)" >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc
	@echo "URL: http://www.directfb.org" >> $(DESTDIR)$(libdir)/../pkgconfig/$(PROJECTNAME).pc

install: install-disko install-pkgconfig

check:
	@echo check for libpng
	@pkg-config --print-errors --exists libpng
	@echo check for libxml++
	@pkg-config --print-errors --exists libxml++-2.6
	@echo check for sqlite3
	@pkg-config --print-errors --exists sqlite3
	@echo check for libcurl
	@pkg-config --print-errors --exists libcurl
	@echo check for directfb
	@pkg-config --print-errors --exists directfb
	@echo check for libsigc++
	@pkg-config --print-errors --exists sigc++-2.0
		
dox:
	@doxygen ./doc/conf/$(PROJECTNAME).conf
	
srcpkg:
	@source ./scripts/create_src_pkg.sh
