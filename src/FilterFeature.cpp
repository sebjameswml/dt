#ifdef __GNUG__
#  pragma implementation
#endif

#include "FilterFeature.h"
#include "Datastream.h"
#include "DatastreamOption.h"

using namespace std;
using namespace dt;

FilterFeature::FilterFeature (const string& name, const string& label)
        : name (name)
        , label (label)
{
}

FilterFeature::~FilterFeature()
{
}

//
// Methods
//

void
FilterFeature::addOption (shared_ptr<DatastreamOption> pOpt)
{
        this->options.push_back (pOpt);
}

bool
FilterFeature::isConfigurable (void)
{
        return !this->options.empty();
}

void
FilterFeature::populateOptions (const Datastream& ds, const string& filter)
{
        auto i (this->options.begin()), end (this->options.end());
        while (i != end) {
                (*i)->populate (ds, filter, this->getName());
                ++i;
        }
}

//
// Accessors
//

string
FilterFeature::getName (void) const
{
        return this->name;
}

void
FilterFeature::setName (const string& s)
{
        this->name = s;
}

string
FilterFeature::getDesc (void) const
{
        return this->desc;
}

void
FilterFeature::setDesc (const string& s)
{
        this->desc = s;
}

string
FilterFeature::getLabel (void) const
{
        return this->label;
}

void
FilterFeature::setLabel (const string& s)
{
        this->label = s;
}
