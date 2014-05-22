/*
 * A test filter program.
 *
 * This filter program implements a number of features, which can be
 * determined using the --list-features command line argument.
 *
 * Parameters to control the features may be passed as command line
 * arguments.
 */

#include <stdexcept>
#include <iostream>
#include <vector>
#include <sstream>
#include "config.h"
#include <futil/WmlDbg.h>
//#include <futil/futil.h>

extern "C" {
#include <unistd.h>
#include <popt.h>
}

using namespace std;

#define TESTFILTER_FEATURES "archive destshare"

// We make a struct of parameters for holding options parsed by popt.
struct params {
        char * filename;
        int listFeatures;
};

// And we have to zero it out before we start.
void zeroParams (struct params * p)
{
        p->filename = nullptr;
        p->listFeatures = 0;
}

void usage (poptContext optCon, const string& error, const string& addl)
{
        poptPrintUsage(optCon, stderr, 0);
        if (!error.empty()) {
                fprintf(stderr, "%s%s%s",
                        error.c_str(), (addl.empty() ? "" : ": "), addl.c_str());
        }
        poptFreeContext(optCon);
}

int main (int argc, char * argv[])
{
        struct params p;
        zeroParams (&p);

        struct poptOption opt[] = {
                POPT_AUTOHELP

                // Filter options

                // {"post", 'p',
                //  POPT_ARG_STRING, &(f.post), 0,
                //  "The POST variables, comma separated: key1=val1,key2=val2"},

                // {"get", 'g',
                //  POPT_ARG_STRING, &(f.get), 0,
                //  "The GET variables, comma separated: key1=val1,key2=val2"},

                {"filename", 'f',
                 POPT_ARG_STRING, &(p.filename), 'f',
                 "The data filename", "myfile"},

                {"list-features", 'l',
                 POPT_ARG_NONE, &(p.listFeatures), 'l',
                 "Display a list of features"},

                POPT_TABLEEND
        };
        poptContext con;

        /*
         * Can pass POPT_CONTEXT_KEEP_FIRST for last argument, which
         * allows you to use the value of argv[0]
         */
        con = poptGetContext (argv[0], argc, (const char**)argv, opt, 0);
        int c;
        while ((c = poptGetNextOpt(con)) != -1) {} // Get all options

        if (c < -1) {
                /* an error occurred during option processing */
                fprintf(stderr, "%s: %s\n",
                        poptBadOption(con, POPT_BADOPTION_NOALIAS),
                        poptStrerror(c));
                poptFreeContext(con);
                return 1;
        }

        if (p.listFeatures) {
                fprintf(stdout, "%s\n", TESTFILTER_FEATURES);
        // } else if (p.filename == nullptr) {
        //         usage (con, "Please specify a filename", "");
        //         return 1;
        }

        // Free up the memory associated with cmd line parsing.
        poptFreeContext(con);

        return 0;
}
