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

        FilterFeature::ConstOptionIter iOpt (pFeat->beginOptions()),
                optEnd (pFeat->endOptions());
        while (iOpt != optEnd) {
                cout << "\t" << (*iOpt)->show() << endl;
                ++iOpt;
        }
        cout << endl;
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
