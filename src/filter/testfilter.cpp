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

#define TESTFILTER_FEATURES "archive dcc"

// We make a struct of parameters for holding options parsed by popt.
struct params {
        char * datastream;
        char * data;
        char * user;
        char * title;
        int listFeatures;

        char * archive;
        char * dcc;
};

// And we have to zero it out before we start.
void zeroParams (struct params * p)
{
        p->datastream = nullptr;
        p->data = nullptr;
        p->user = nullptr;
        p->title = nullptr;
        p->listFeatures = 0;

        p->archive = nullptr;
        p->dcc = nullptr;
}

void usage (poptContext optCon, const string& error, const string& addl)
{
        poptPrintUsage(optCon, stderr, 0);
        if (!error.empty()) {
                fprintf(stderr, "%s%s%s\n",
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

                {"data", 'd',
                 POPT_ARG_STRING, &(p.data), 'd',
                 "Data file name", "myfile"},

                {"datastream", 's',
                 POPT_ARG_STRING, &(p.datastream), 's',
                 "Datastream ID", "mydatastream"},

                {"title", 't',
                 POPT_ARG_STRING, &(p.title), 't',
                 "Job title", "mytitle"},

                {"user", 'u',
                 POPT_ARG_STRING, &(p.user), 'u',
                 "User name", "anonymous"},

                {"list-features", 'l',
                 POPT_ARG_NONE, &(p.listFeatures), 'l',
                 "Display a list of features"},

                // Filter features

                {"archive", '\0',
                 POPT_ARG_STRING, &(p.archive), '\0',
                 "Archive feature options", "\"opt1=val1,opt2=val2,...\""},

                {"dcc", '\0',
                 POPT_ARG_STRING, &(p.dcc), '\0',
                 "DCC feature options", "\"opt1=val1,opt2=val2,...\""},

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
        } else if (p.data == nullptr) {
                usage (con, "Please specify a data file", "");
                return 1;
        }

        // Free up the memory associated with cmd line parsing.
        poptFreeContext(con);

        return 0;
}
