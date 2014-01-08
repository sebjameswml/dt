CLEANFILES = *~

#SUBDIRS = 

INCLUDES = -I$(top_srcdir) -I$(top_builddir)
# $(GLIBMM_CFLAGS) $(UUID_CFLAGS)

lib_LTLIBRARIES = libdtipp.la

libdtipp_la_SOURCES = Attr.cpp Encoding.cpp

libdtipp_la_LDFLAGS = -version-info 3:0:0

pkginclude_HEADERS = Encoding.h Attr.h
