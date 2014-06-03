/* -*-c++-*- */
/*!
 * \file DccFeature.h
 *
 * \brief Declares the class \c DccFeature.
 */

#ifndef _DCCFEATURE_H_
#define _DCCFEATURE_H_

#ifdef __GNUG__
# pragma interface
#endif

#include "FilterFeature.h"
#include "DatastreamOption.h"

namespace dt {

        /*!
         * \headerfile DccFeature.h "DccFeature.h"
         *
         * \brief \c DccFeature enables DCC for a filter in the Data
         * Transport system.
         */
        class DccFeature : public FilterFeature
        {
        public:

                /*!
                 * Constructor
                 */
                DccFeature();

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~DccFeature();

                static ListOption::valueList getProfilesDummy (void);

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                //@}

        private:

        };

} // dt namespace

#endif // _DCCFEATURE_H_
