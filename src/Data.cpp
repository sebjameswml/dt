#ifdef __GNUG__
#  pragma implementation
#endif

#include "Data.h"

using namespace std;
using namespace dt;

Data::Data (const string& dataId)
        : id (dataId)
        , name ("")
        , user ("")
        , copies (1)
{
        // TODO data in user configurable location
        this->path = "/tmp/" + this->id;
}

Data::~Data()
{
}

//
// Methods
//
