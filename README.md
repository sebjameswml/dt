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




DT is Copyright William Matthew Ltd. 2014.

Authors: Seb James <sjames@wmltd.co.uk>
         Tamora James <tjames@wmltd.co.uk>

DT is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

DT is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with DT (see the file COPYING.LGPL).  If not, see
<http://www.gnu.org/licenses/>.

Seb James, 12th February 2014.
