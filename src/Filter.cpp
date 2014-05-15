#ifdef __GNUG__
#  pragma implementation
#endif

#include "Filter.h"
#include "Datastream.h"
#include "Data.h"

using namespace std;
using namespace dt;

/*!
 * BaseFilter
 */
//@{
BaseFilter::BaseFilter(const string& thePath)
        : path (thePath)
{
        // TODO Use a factory to assign features for given filters
        if (this->path.find ("wmlpdf") != string::npos) {
                this->features.push_back ("archive");
        }
}

BaseFilter::~BaseFilter()
{
}

//
// Methods
//

void
BaseFilter::populateArgs (const Datastream& ds,
                           const Data& d,
                           list<string>& args)
{
        // Standard CUPS filter argument list: queue, job id, user, title, copies, options
        args.assign ({
                        ds.getName()
                                , d.getId() // Use dummy job ID here?
                                , d.getUser()
                                , d.getName()
                                , d.getCopiesStr()
                                , "0" // d.getOptions()
                                });
}

//
// Accessors
//

string
BaseFilter::getPath (void) const
{
        return this->path;
}

void
BaseFilter::setPath (const string& s)
{
        this->path = s;
}

list<string>
BaseFilter::getFeatures (void) const
{
        return this->features;
}

//@}

/*!
 * Filter
 */
//@{
Filter::Filter(const string& thePath)
        : BaseFilter (thePath)
{
        // TODO Use a factory to assign features for given filters.
        // Factory could call filter program with '--features' option
        // to return a list of supported features.

        // if (this->path.find ("wmlpdf") != string::npos) {
        //         this->features.push_back ("archive");
        // }
}

Filter::~Filter()
{
}

//
// Methods
//

void
Filter::populateArgs (const Datastream& ds,
                      const Data& d,
                      list<string>& args)
{
        // Specify arguments to control the features for
        // this filter, as well as passing DT specific
        // attributes such as unique data ID and data logging
        // parameters.

        args.push_back ("--datastream="+ds.getName());
        args.push_back ("--data="+d.getId());
        args.push_back ("--user="+d.getUser());
        args.push_back ("--title="+d.getName());

        // For each supported feature, get its options and option
        // values for the specified datastream.

        // Add these option/value pairs as arguments.
}

//@}
