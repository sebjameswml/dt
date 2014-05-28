CLEANFILES = *~

SUBDIRS = ipp log dtmime dtppd filter

INCLUDES = -I$(top_srcdir) -I$(top_builddir) $(JSONCPP_CFLAGS) -I/usr/include/mysql -I/usr/include/mysql++
# $(GLIBMM_CFLAGS) $(UUID_CFLAGS)

lib_LTLIBRARIES = libdt.la

libdt_la_SOURCES = Datastream.cpp Data.cpp DatastreamOption.cpp \
            ProcessCallbacks.cpp Filter.cpp FilterFactory.cpp \
            FilterFeature.cpp ArchiveFeature.cpp DccFeature.cpp \
            FilterFeatureFactory.cpp Backend.cpp

libdt_la_LDFLAGS = -version-info 3:0:0

pkginclude_HEADERS = Datastream.h Data.h DatastreamOption.h \
            ProcessCallbacks.h Filter.h FilterFactory.h \
            FilterFeature.h ArchiveFeature.h DccFeature.h \
            FilterFeatureFactory.h Backend.h
