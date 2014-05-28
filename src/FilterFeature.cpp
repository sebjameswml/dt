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

list<pair<string, string> >
FilterFeature::populateOptions (const Datastream& ds, const string& filter)
{
        list<pair<string, string> > l, li;
        for (auto& i : this->options) {
                li = i->populate (ds, filter, this->getName());
                l.splice (l.end(), li);
        }
        return l;
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
