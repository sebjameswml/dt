
AUTOMAKE_OPTIONS = check-news dist-bzip2

# Add doc, later.
SUBDIRS = src

bin_PROGRAMS = dt testEncode testParse testEventStorage

#check_PROGRAMS = testdt

#TESTS = testThing1

dt_SOURCES = dt.cpp
dt_LDADD = src/ipp/libdtipp.la /usr/local/lib/libuv.a /usr/local/lib/http_parser.o

testEncode_SOURCES = testEncode.cpp
testEncode_LDADD = src/ipp/libdtipp.la

testParse_SOURCES = testParse.cpp
testParse_LDADD = src/ipp/libdtipp.la

testEventStorage_SOURCES = testEventStorage.cpp
testEventStorage_LDADD = src/log/libdtlog.la -luuid -lmysqlpp

INCLUDES = -I./usr/local/include -I/usr/include/mysql -I/usr/include/mysql++

CLEANFILES = *~

MAINTAINERCLEANFILES = aclocal.m4 dt-*.tar.gz dt-*.tar.bz2 dt-*.diff
