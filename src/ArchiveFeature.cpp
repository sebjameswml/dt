#ifdef __GNUG__
#  pragma implementation
#endif

#include "ArchiveFeature.h"
#include "DatastreamOption.h"

using namespace std;
using namespace dt;

ArchiveFeature::ArchiveFeature (void)
        : FilterFeature ("archive", "Archiving")
{
        this->setDesc ("Archive data to network attached storage");

        // Set up options for archiving
        shared_ptr<DatastreamOption> pOpt;

        // Archiving enabled
        pOpt.reset (new BoolOption ("archiveEnabled", "Archiving enabled", "", true));
        this->addOption (pOpt);

        // Archive expiry period
        unsigned int maxVal (49710); // The maximum archive expiry
                                     // period is 49710 days,
                                     // corresponding to 4294944000
                                     // seconds, which can be held in
                                     // a 32 bit unsigned int.
        pOpt.reset (new UIntOption ("archiveExpiry", "Archive expiry", "days", false, maxVal));
        this->addOption (pOpt);

        // Archive share
        pOpt.reset (new CompositeOption ("archiveShare", "Archive share"));
        CompositeOption * ps (dynamic_cast<CompositeOption *>(pOpt.get()));
        if (ps) {
                shared_ptr<DatastreamOption> pShareOpt (new TextOption ("archiveShare_sharePath", "Share path", "", true));
                ps->add (pShareOpt);
                pShareOpt.reset (new TextOption ("archiveShare_shareUser", "Share username"));
                ps->add (pShareOpt);
                pShareOpt.reset (new TextOption ("archiveShare_sharePass", "Share password"));
                ps->add (pShareOpt);
                pShareOpt.reset (new TextOption ("archiveShare_shareAddr", "Server IP"));
                ps->add (pShareOpt);
                // TODO add Share type - option list
        }
        this->addOption (pOpt);

        // Archive file name
        pOpt.reset (new CompositeOption ("archiveFilename", "Archive file name"));
        CompositeOption * pfn (dynamic_cast<CompositeOption *>(pOpt.get()));
        if (pfn) {
                shared_ptr<DatastreamOption> pFilenameOpt (new TextOption ("archiveFilename_fields", "Fields", "", true));
                pfn->add (pFilenameOpt);
                pFilenameOpt.reset (new TextOption ("archiveFilename_separator", "Field sparator"));
                pfn->add (pFilenameOpt);
                pFilenameOpt.reset (new TextOption ("archiveFilename_suffix", "Suffix"));
                pfn->add (pFilenameOpt);
        }
        this->addOption (pOpt);
}

ArchiveFeature::~ArchiveFeature()
{
}

//
// Methods
//
