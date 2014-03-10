CLEANFILES = *~

#SUBDIRS =

INCLUDES = -I$(top_srcdir) -I$(top_builddir) $(UUID_CFLAGS)
# $(GLIBMM_CFLAGS)

lib_LTLIBRARIES = libdtlog.la

libdtlog_la_SOURCES = Event.cpp Storage.cpp MySQLStorage.cpp SyslogStorage.cpp

libdtlog_la_LDFLAGS = -version-info 3:0:0

pkginclude_HEADERS = Event.h Storage.h MySQLStorage.h SyslogStorage.h
