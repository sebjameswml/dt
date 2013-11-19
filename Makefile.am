
AUTOMAKE_OPTIONS = check-news dist-bzip2

# Add doc, later.
SUBDIRS = src server3

bin_PROGRAMS = dt testserver3

#check_PROGRAMS = testdt

#TESTS = testThing1

dt_SOURCES = dt.cpp
dt_LDADD = src/dthttpd/libdthttpd.la -lboost_thread
# /usr/local/lib/libuv.a /usr/local/lib/http_parser.o

# Boost HTTP asio example 3 server:
testserver3_SOURCES = testserver3.cpp
testserver3_LDADD = server3/libserver3.la -lboost_thread

INCLUDES = -I./usr/local/include

CLEANFILES = *~

MAINTAINERCLEANFILES = aclocal.m4 dt-*.tar.gz dt-*.tar.bz2 dt-*.diff

