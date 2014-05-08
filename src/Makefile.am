CLEANFILES = *~

SUBDIRS = ipp log dtmime dtppd

INCLUDES = -I$(top_srcdir) -I$(top_builddir) $(JSONCPP_CFLAGS) -I/usr/include/mysql -I/usr/include/mysql++
# $(GLIBMM_CFLAGS) $(UUID_CFLAGS)

lib_LTLIBRARIES = libdt.la

libdt_la_SOURCES = Datastream.cpp Data.cpp DatastreamOption.cpp \
            Filter.cpp  FilterFeature.cpp FilterProcessCallbacks.cpp \
            ArchiveFeature.cpp

libdt_la_LDFLAGS = -version-info 3:0:0

pkginclude_HEADERS = Datastream.h Data.h DatastreamOption.h \
            Filter.h FilterFeature.h FilterProcessCallbacks.h \
            ArchiveFeature.h
