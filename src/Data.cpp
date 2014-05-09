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


//
// Accessors
//

string
Data::getId (void) const
{
        return this->id;
}

string
Data::getTargetDatastream (void) const
{
        return this->targetDatastream;
}

void
Data::setTargetDatastream (const string& s)
{
        this->targetDatastream = s;
}

string
Data::getName (void) const
{
        return this->name;
}

void
Data::setName (const string& s)
{
        this->name = s;
}

string
Data::getUser (void) const
{
        return this->user;
}

void
Data::setUser (const string& s)
{
        this->user = s;
}

int
Data::getCopies (void) const
{
        return this->copies;
}

string
Data::getCopiesStr (void) const
{
        stringstream ss;
        ss << this->copies;
        return ss.str();
}

void
Data::setCopies (const int i)
{
        this->copies = i;
}

string
Data::getPath (void) const
{
        return this->path;
}

void
Data::setPath (const string& s)
{
        this->path = s;
}
