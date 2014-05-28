#ifdef __GNUG__
#  pragma implementation
#endif

#include "FilterFeatureFactory.h"
#include "FilterFeature.h"
#include "ArchiveFeature.h"
#include "DccFeature.h"

using namespace std;
using namespace dt;

unique_ptr<FilterFeature>
FilterFeatureFactory::create (const string& feature)
{
        unique_ptr<FilterFeature> pFeat;
        if (feature.find ("archive") != string::npos) {
                pFeat.reset (new ArchiveFeature());
        } else if (feature.find ("dcc") != string::npos) {
                pFeat.reset (new DccFeature());
        }
        return pFeat;
}
