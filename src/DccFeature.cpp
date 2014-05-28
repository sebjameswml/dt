#ifdef __GNUG__
#  pragma implementation
#endif

#include "DccFeature.h"
#include "DatastreamOption.h"

using namespace std;
using namespace dt;

DccFeature::DccFeature (void)
        : FilterFeature ("dcc", "Document content control")
{
        this->setDesc ("Apply document content control");

        // Set up options for DCC
        shared_ptr<DatastreamOption> pOpt;

        // DCC enabled
        pOpt.reset (new BoolOption ("dccEnabled", "DCC enabled", "", true));
        this->addOption (pOpt);

        // DCC profile - selected from list of options based on
        // profiles available on system
        pOpt.reset (new ListOption ("dccProfile", "DCC profile", bind(&DccFeature::getProfilesDummy)));
        this->addOption (pOpt);

        // Output datastream - selected from list of options based on
        // datastreams available on system
        pOpt.reset (new ListOption ("dccOutputDatastream", "Output datastream"));
        // pOpt.reset (new ListOption ("dccOutputDatastream", "Output datastream",
        //                             bind(&Scheduler::listDatastreams)));
        this->addOption (pOpt);

        // DCC alerts
        pOpt.reset (new CompositeOption ("dccAlerts", "DCC alerts"));
        CompositeOption * ps (dynamic_cast<CompositeOption *>(pOpt.get()));
        if (ps) {
                shared_ptr<DatastreamOption> pShareOpt (new TextOption ("dccAlerts_recipients", "Recipients"));
                ps->add (pShareOpt);
                pShareOpt.reset (new TextOption ("dccAlerts_fromAddress", "From"));
                ps->add (pShareOpt);
                pShareOpt.reset (new TextOption ("dccAlerts_subject", "Email subject"));
                ps->add (pShareOpt);
                pShareOpt.reset (new TextOption ("dccAlert_delay", "Time delay for held document alerts", "minutes"));
                ps->add (pShareOpt);
        }
        this->addOption (pOpt);
}

DccFeature::~DccFeature()
{
}

//
// Methods
//

ListOption::optList
DccFeature::getProfilesDummy (void)
{
        ListOption::optList l = { {"0", "None"}
                                  , {"1", "Alpha"}
                                  , {"2", "Beta"} };
        return l;
}
