
AUTOMAKE_OPTIONS = check-news dist-bzip2

# Add doc, later.
SUBDIRS = src

bin_PROGRAMS = dt testEncode testParse testEventStorage testDatastream testProcessData testCupsFilters

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

testDatastream_SOURCES = testDatastream.cpp
testDatastream_LDADD = src/libdt.la src/log/libdtlog.la src/dtmime/libdtmime.la \
                      src/dtppd/libdtppd.la \
                      $(XMLPP_LIBS) -luuid -lmagic \
                      -lfutil -lcurl $(JSONCPP_LIBS) -lwmlppcommon -lmysqlpp

testProcessData_SOURCES = testProcessData.cpp
testProcessData_LDADD = src/libdt.la src/log/libdtlog.la src/dtmime/libdtmime.la \
                      src/dtppd/libdtppd.la \
                      $(XMLPP_LIBS) -luuid -lmagic \
                      -lfutil -lcurl $(JSONCPP_LIBS) -lwmlppcommon -lmysqlpp

testCupsFilters_SOURCES = testCupsFilters.cpp
testCupsFilters_LDADD = src/libdt.la src/log/libdtlog.la src/dtmime/libdtmime.la \
                      src/dtppd/libdtppd.la \
                      $(XMLPP_LIBS) -luuid -lmagic \
                      -lfutil -lcurl $(JSONCPP_LIBS) -lwmlppcommon -lmysqlpp

INCLUDES = -I./usr/local/include -I/usr/include/mysql -I/usr/include/mysql++ $(JSONCPP_CFLAGS)

CLEANFILES = *~

MAINTAINERCLEANFILES = aclocal.m4 dt-*.tar.gz dt-*.tar.bz2 dt-*.diff
