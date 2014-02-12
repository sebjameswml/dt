DT - Data Transport
===================

Fully audited digital data transport. Ideal for document transfer and printing applications. Replaces CUPS.

Very very alpha at present; in fact, it's at the design stage.

This project seeks to replace CUPS as the data transport used in William Matthew Ltd. software.

It will address a number of deficiencies in CUPS:

1. CUPS doesn't have unique job ids between servers - we want UUID style job ids to improve reporting.
2. CUPS reports to a syslog file; we want to report into a database.
3. CUPS doesn't do parallel processing of jobs on a single queue; we want this to be possible.
4. CUPS has MaxJobs, which catches us out and can cause us to lose data when we're accepting raw input.
5. CUPS has separate daemons for raw/lpd input; we'd rather integrate this functionality into DT.
6. CUPS has an automatic filter chain system, which we'd rather was more manual and user-specified.
7. CUPS is inflexible about a filter having no output.
8. CUPS is inflexible about having a single PPD per queue.

Licence: GPL.
