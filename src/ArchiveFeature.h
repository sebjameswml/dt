/* -*-c++-*- */
/*!
 * \file ArchiveFeature.h
 *
 * \brief Declares the class \c ArchiveFeature.
 */

#ifndef _ARCHIVEFEATURE_H_
#define _ARCHIVEFEATURE_H_

#ifdef __GNUG__
# pragma interface
#endif

#include "FilterFeature.h"

namespace dt {

        /*!
         * \headerfile ArchiveFeature.h "ArchiveFeature.h"
         *
         * \brief \c ArchiveFeature enables archiving for a filter in
         * the Data Transport system.
         */
        class ArchiveFeature : public FilterFeature
        {
        public:

                /*!
                 * Constructor
                 */
                ArchiveFeature();

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~ArchiveFeature();

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                //@}

        private:

        };

} // dt namespace

#endif // _ARCHIVEFEATURE_H_
