#ifdef __GNUG__
#  pragma implementation
#endif

#include "Filter.h"

using namespace std;
using namespace dt;

Filter::Filter(const string& thePath)
        : path (thePath)
{
        // TODO Use a factory to assign features for given filters
        if (this->path.find ("wmlpdf") != string::npos) {
                this->features.push_back ("archive");
        }
}

Filter::~Filter()
{
}

//
// Methods
//

//
// Accessors
//

string
Filter::getPath (void) const
{
        return this->path;
}

void
Filter::setPath (const string& s)
{
        this->path = s;
}
