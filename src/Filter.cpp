#ifdef __GNUG__
#  pragma implementation
#endif

#include "Filter.h"
#include "Datastream.h"
#include "Data.h"
#include "FilterFeatureFactory.h"

using namespace std;
using namespace dt;

/*!
 * BaseFilter
 */
//@{
BaseFilter::BaseFilter(const string& thePath)
        : path (thePath)
{
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

void
BaseFilter::setFeatures (const string& feat, char delim)
{
        this->features.clear();
        if (!delim) {
                string::size_type pos (feat.find_first_of (" ,;:|@#+%$"));
                if (pos != string::npos) {
                        delim = feat[pos];
                } else {
                        delim = '\n';
                }
        }
        string token;
        stringstream ss (feat);
        while (getline (ss, token, delim)) {
                this->features.push_back (token);
        }
}
//@}

/*!
 * Filter
 */
//@{
Filter::Filter(const string& thePath)
        : BaseFilter (thePath)
{
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
        auto i (this->beginFeatures()), end (this->endFeatures());
        while (i != end) {

                unique_ptr<FilterFeature> pFeat (FilterFeatureFactory::create (*i));

                if (pFeat.get() && pFeat->isConfigurable()) {
                        // Populate with the options set for the Datastream ds.
                        list<pair<string, string> > l (pFeat->populateOptions (ds, this->getPath()));

                        stringstream ss;
                        ss << "--" << pFeat->getName() << "=\"";
                        bool first (true);
                        for (auto i : l) {
                                if (!first) {
                                        ss << ",";
                                } else {
                                        first = false;
                                }
                                ss << i.first << "=" << i.second;
                        }
                        ss << "\"";
                        args.push_back (ss.str());
                }
                ++i;
        }
}
//@}
