
AUTOMAKE_OPTIONS = check-news dist-bzip2

# Add doc, later.
SUBDIRS = src

bin_PROGRAMS = dt

#check_PROGRAMS = testdt

#TESTS = testThing1

dt_SOURCES = dt.cpp
dt_LDADD = /usr/local/lib/libuv.a /usr/local/lib/http_parser.o

INCLUDES = -I./usr/local/include

CLEANFILES = *~

MAINTAINERCLEANFILES = aclocal.m4 dt-*.tar.gz dt-*.tar.bz2 dt-*.diff

