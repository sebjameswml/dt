#ifdef __GNUG__
#  pragma implementation
#endif

#include "Backend.h"
#include "Datastream.h"
#include "Data.h"
#include "DatastreamOption.h"

using namespace std;
using namespace dt;

/*!
 * Backend
 */
//@{
Backend::Backend (const string& thePath)
        : path (thePath)
{
}

Backend::~Backend()
{
}

//
// Methods
//

void
Backend::populateArgs (const Datastream& ds,
                           const Data& d,
                           list<string>& args)
{
        // Standard CUPS backend argument list: queue, job id, user, title, copies, options
        args.assign ({
                        ds.getName()
                                , d.getId() // Use dummy job ID here?
                                , d.getUser()
                                , d.getName()
                                , d.getCopiesStr()
                                , "0" // d.getOptions()
                                });
}

void
Backend::addOption (shared_ptr<DatastreamOption> pOpt)
{
        this->options.push_back (pOpt);
}

bool
Backend::isConfigurable (void)
{
        return !this->options.empty();
}

std::list<std::pair<std::string, std::string> >
Backend::populateOptions (const Datastream& ds)
{
        std::list<std::pair<std::string, std::string> > l, li;
        for (auto& i : this->options) {
                li = i->populate (ds, this->getPath());
                l.splice (l.end(), li);
        }
        return l;
}


//
// Accessors
//

string
Backend::getPath (void) const
{
        return this->path;
}

void
Backend::setPath (const string& s)
{
        this->path = s;
}
//@}

/*!
 * BackendPlus
 */
//@{
BackendPlus::BackendPlus(const string& thePath)
        : Backend (thePath)
{
}

BackendPlus::~BackendPlus()
{
}

//
// Methods
//

void
BackendPlus::populateArgs (const Datastream& ds,
                      const Data& d,
                      list<string>& args)
{
        // Specify arguments to control the features for
        // this backend, as well as passing DT specific
        // attributes such as unique data ID and data logging
        // parameters.

        args.push_back ("--datastream="+ds.getName());
        args.push_back ("--data="+d.getId());
        args.push_back ("--user="+d.getUser());
        args.push_back ("--title="+d.getName());

        // Get options and option values for the specified datastream.

        // Add these option/value pairs as arguments.
        if (this->isConfigurable()) {
                // Populate with the options set for the Datastream ds.
                std::list<std::pair<std::string, std::string> > l (this->populateOptions (ds));
                stringstream ss;
                for (auto i : l) {
                        ss << "--" << i.first << "=" << i.second;
                        args.push_back (ss.str());
                        ss.str("");
                        ss.clear();
                }
        }
}
//@}
