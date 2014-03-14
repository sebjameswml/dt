#ifdef __GNUG__
#  pragma implementation
#endif

#include "Datastream.h"

using namespace std;
using namespace dt;

Datastream::Datastream()
        : id ("")
        , name ("")
{
}

Datastream::Datastream (const string& theId)
        : id (theId)
        , name ("")
{
        this->initialise();
}

Datastream::~Datastream()
{
}

//
// Methods
//
void
Datastream::initialise (void)
{
        // Check for settings for this ID
        try {
                this->settings.initialise (this->id);
        } catch (const exception& e) {
                this->settings.setUseDb (false);
                try {
                        string fp ("/etc/wml/pp/datastream/" + this->id);
                        this->settings.initialise (fp);
                } catch (const exception& e) {
                        throw runtime_error (e.what());
                }
        }
        if (this->settings.ready()) {
                // Set up datastream from settings
        }
}
