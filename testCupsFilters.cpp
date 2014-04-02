extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include <iostream>

#include "src/dtmime/mime.h"

#include "config.h"
#include <futil/WmlDbg.h>
std::ofstream DBGSTREAM;

#define CUPS_SERVERBIN "/usr/lib/cups"

#ifdef DEBUG
# define debugf(format, ...) printf(format, __VA_ARGS__)
# define debuglog(type, format, ...) syslog(type, format, __VA_ARGS__)
#else
# define debugf(format, ...)
# define debuglog(type, format, ...)
#endif


extern "C" {
        int checkFilters (const char* file,
                          const char* dest_type)
        {

                const char    *filter_path;           /* Filter path */
                char          super[MIME_MAX_SUPER],  /* Super-type name */
                              type[MIME_MAX_TYPE];    /* Type name */
                int           compression;            /* Compression of file */
                int           cost;                   /* Cost of filters */
                mime_t        *mime;                  /* MIME database */
                mime_type_t   *src,                   /* Source type */
                              *dst;                   /* Destination type */
                cups_array_t  *filters;               /* Filters for the file */
                mime_filter_t *filter;                /* Current filter */


                mime        = NULL;
                src         = NULL;
                dst         = NULL;
                filter_path = "../filter:../pdftops:" CUPS_SERVERBIN "/filter";

                if (!mime) {
                        debugf ("%s: Calling mimeLoad\n", __FUNCTION__);
                        mime = mimeLoad("/etc/dt/mime", filter_path);
                }
                debugf ("%s: Calling mimeFileType\n", __FUNCTION__);
                src = mimeFileType(mime, file, NULL, &compression);

                if (src)
                        printf("%s: %s/%s%s\n", file, src->super, src->type,
                               compression ? " (gzipped)" : "");
                else if ((src = mimeType(mime, "application", "octet-stream")) != NULL)
                        printf("%s: application/octet-stream\n", file);
                else
                {
                        printf("%s: unknown\n", file);
                        if (mime)
                                mimeDelete(mime);
                        return (1);
                }

                sscanf(dest_type, "%15[^/]/%31s", super, type);
                dst = mimeType(mime, super, type);

                filters = mimeFilter(mime, src, dst, &cost);

                if (!filters)
                {
                        printf("No filters to convert from %s/%s to %s/%s.\n", src->super,
                               src->type, dst->super, dst->type);
                }
                else
                {
                        printf("Filter cost = %d\n", cost);

                        filter = (mime_filter_t *)cupsArrayFirst(filters);
                        fputs(filter->filter, stdout);

                        for (filter = (mime_filter_t *)cupsArrayNext(filters);
                             filter;
                             filter = (mime_filter_t *)cupsArrayNext(filters))
                                printf(" | %s", filter->filter);

                        putchar('\n');
                        cupsArrayDelete(filters);
                }
                return 0;
        }
}

using namespace std;

int main (int argc, char** argv) {

        DBGOPEN ("./testCupsFilters.dbg");

        if (argc < 2) {
                cerr << "Usage: testCupsFilters <file> <dest>\n";
                cerr << "  The file should contain the input data to be passed to the filter chain\n";
                cerr << "  The dest should specify the destination mime type\n";
                return -1;
        }

        string input (argv[1]), dest (argv[2]);
        if (checkFilters (input.c_str(), dest.c_str())) {
                cerr << "There was a problem checking filters\n";
                return -1;
        }

        DBGCLOSE();
        return 0;
}
