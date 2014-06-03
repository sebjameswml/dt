/*
 * Test gettings filters, options etc from dt::Datastream
 */
#include "config.h"
#include <futil/WmlDbg.h>
std::ofstream DBGSTREAM;

#include "src/Datastream.h"
#include "src/DatastreamOption.h"
#include "src/Filter.h"
#include "src/FilterFactory.h"
#include "src/FilterFeature.h"
#include "src/FilterFeatureFactory.h"

using namespace std;
using namespace dt;


class StdoutDatastreamOptionVisitor : public DatastreamOptionVisitor
{
public:

        void visitFeature (const FilterFeature& feat)
                {
                        FilterFeature::ConstOptionIter iOpt (feat.beginOptions()),
                                optEnd (feat.endOptions());
                        while (iOpt != optEnd) {
                                (*iOpt)->accept(*this);
                                ++iOpt;
                        }
                        cout << endl;
                }

        void visit (TextOption& o) const
                {
                        cout << "\t" << o.getLabel() << ": " << o.getValue() << endl;
                }
        void visit (BoolOption& o) const
                {
                        cout << "\t" << o.getLabel() << ": " << o.getValue() << endl;
                }
        void visit (UIntOption& o) const
                {
                        cout << "\t" << o.getLabel() << ": " << o.getValue() << endl;
                }
        void visit (ListOption& o) const
                {
                        cout << "\t" << o.getLabel() << ":" << endl;
                        ListOption::valueList l (o.getValueList());
                        if (l.empty()) {
                                cout << "\t\t! No options available" << endl;
                        } else {
                                string val (o.getValue());
                                for (auto i : l) {
                                        cout << "\t\t- " << i.second << " (" << i.first << ")";
                                        if (i.second == val) {
                                                cout << " *";
                                        }
                                        cout << endl;
                                }
                        }
                }
        void visit (CompositeOption& o) const
                {
                        cout << "\t" << o.getLabel() << " (group):" << endl;

                        // We iterate through the child options here
                        // to give more control over display.
                        CompositeOption::const_iterator iOpt (o.begin()), optEnd (o.end());
                        while (iOpt != optEnd) {
                                cout << "\t";
                                (*iOpt)->accept (*this);
                                ++iOpt;
                        }
                }
};

void
displayFeature (const Datastream& ds, const string& filter, const string& feature)
{
        unique_ptr<FilterFeature> pFeat (FilterFeatureFactory::create (feature));
        if (!pFeat.get()) {
                cout << "* Unknown feature: " << feature << endl;
                return;
        }

        // Populate with the options set for the Datastream
        pFeat->populateOptions (ds, filter);

        // Output feature stuff
        // Feature label
        // Feature description
        // Feature controls

        cout << "* " << pFeat->getLabel() << endl;
        cout << "  " << pFeat->getDesc() << endl;

        if (!pFeat->isConfigurable()) {
                cout << "\tThis feature is not configurable" << endl;
        }

        StdoutDatastreamOptionVisitor visitor;
        visitor.visitFeature (*pFeat);
}

int main (int argc, char** argv) {

        DBGOPEN ("./testFilterOptions.dbg");

        Datastream ds ("1300");

        cout << "Datastream ID: " << ds.getId() << endl;
        cout << "Datastream name: " << ds.getName() << endl;

        list<string> filters;
        if (argc > 1) {
                int i(1);
                while (i < argc) {
                        filters.push_back (argv[i]);
                        ++i;
                }
                ds.setFilters (filters);
        }

        filters = ds.getFilters();

        if (filters.empty()) {
                // Populate list of filters from filter programs
                // present on the system? Need to display all possible
                // filters having user-configurable features.
        }

        auto iFilt (filters.cbegin()), filtEnd (filters.cend());
        while (iFilt != filtEnd) {

                // Create filter from filter path.
                unique_ptr<BaseFilter> pf (FilterFactory::create (*iFilt));
                if (pf.get() == nullptr) {
                        throw runtime_error ("Error creating filter object");
                }

                // Output filter info: name and description?

#ifdef USING_LIST_FILTERFEATURE_OBJ
                // If Filter holds list of objects of type FilterFeature
                Filter::ConstFeatureIter iFeat (pf->beginFeatures()),
                        featEnd (pf->endFeatures());
                while (iFeat != featEnd) {
                        if (iFeat->isConfigurable()) {
                                displayFeature (ds, filter, iFeat->getName());
                        }
                        ++iFeat;
                }
#else
                list<string> features (pf->getFeatures());
                if (!features.empty()) {
                        cout << "Filter " << pf->getPath()
                             << " has the following features:" << endl << endl;
                }
                for (auto i : features) {
                        displayFeature (ds, *iFilt, i);
                }
#endif
                ++iFilt;
        }

        DBGCLOSE();
        return 0;
}
