CLEANFILES = *~

INCLUDES = -I$(top_srcdir) -I$(top_builddir)
# $(GLIBMM_CFLAGS) $(UUID_CFLAGS)

lib_LTLIBRARIES = libserver3.la

libserver3_la_SOURCES = connection.cpp mime_types.cpp reply.cpp request_handler.cpp request_parser.cpp server.cpp

libserver3_la_LDFLAGS = -version-info 3:0:0

pkginclude_HEADERS = connection.hpp header.hpp mime_types.hpp reply.hpp request_handler.hpp request.hpp request_parser.hpp server.hpp
