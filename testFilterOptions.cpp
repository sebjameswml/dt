/*
 * Test gettings filters, options etc from dt::Datastream
 */
#include "config.h"
#include <futil/WmlDbg.h>
std::ofstream DBGSTREAM;

#include "src/Datastream.h"
#include "src/DatastreamOption.h"
#include "src/Filter.h"
#include "src/FilterFeature.h"
#include "src/ArchiveFeature.h"

using namespace std;
using namespace dt;

// FilterFeature factory method
shared_ptr<FilterFeature>
createFeature (const string& feature)
{
        shared_ptr<FilterFeature> pFeat;
        if (feature == "archive") {
                pFeat.reset (new ArchiveFeature());
        }
        return pFeat;
}

void
displayFeature (const Datastream& ds, const string& filter, const string& feature)
{
        shared_ptr<FilterFeature> pFeat (createFeature (feature));
        if (!pFeat->isConfigurable()) {
                cout << "Feature '" << pFeat->getName() << "' is not configurable" << endl;
                return;
        }

        // Populate with the options set for the Datastream
        pFeat->populateOptions (ds, filter);

        // Output feature stuff
        // Feature label
        // Feature description
        // Feature controls

        cout << "  * " << pFeat->getLabel() << endl;
        cout << "    " << pFeat->getDesc() << endl;

        FilterFeature::ConstOptionIter iOpt (pFeat->beginOptions()),
                optEnd (pFeat->endOptions());
        while (iOpt != optEnd) {
                cout << "\t" << (*iOpt)->show() << endl;
                ++iOpt;
        }
}

int main() {

        DBGOPEN ("./testFilterOptions.dbg");

        Datastream ds ("1300");

        cout << "Datastream ID: " << ds.getId() << endl;
        cout << "Datastream name: " << ds.getName() << endl;


        list<string> filters;
        filters.push_back ("/usr/lib/cups/filter/wmlkeyval");
//       filters.push_back ("/usr/lib/cups/filter/wmlpassthru");
        filters.push_back ("/usr/lib/cups/filter/wmlpdf");

        ds.setFilters (filters);


        filters = ds.getFilters();

        if (filters.empty()) {
                // Populate list of filters from filter programs
                // present on the system? Need to display all possible
                // filters having user-configurable features.
        }

        auto iFilt (filters.cbegin()), filtEnd (filters.cend());
        while (iFilt != filtEnd) {

                Filter f (*iFilt); // Create filter from filter
                                   // path. (Maybe via factory that
                                   // can assign features as
                                   // appropriate?)

                // Output filter info: name and description?

#ifdef USING_LIST_FILTERFEATURE_OBJ
                // If Filter holds list of objects of type FilterFeature
                Filter::ConstFeatureIter iFeat (f.beginFeatures()),
                        featEnd (f.endFeatures());
                while (iFeat != featEnd) {
                        if (iFeat->isConfigurable()) {
                                displayFeature (ds, filter, iFeat->getName());
                        }
                        ++iFeat;
                }
#else
                list<string> features (f.getFeatures());
                if (!features.empty()) {
                        cout << "Filter " << f.getPath() << " has the following features:" << endl;
                }
                auto iFeat (features.cbegin()), featEnd (features.cend());
                while (iFeat != featEnd) {
                        displayFeature (ds, *iFilt, *iFeat);
                        ++iFeat;
                }
#endif
                ++iFilt;
        }

        DBGCLOSE();
        return 0;
}
