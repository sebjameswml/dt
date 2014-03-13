/* -*-c++-*- */
/*!
 * \file Filter.h
 *
 * \brief Declares the class \c Filter.
 */

#ifndef _FILTER_H_
#define _FILTER_H_

#ifdef __GNUG__
# pragma interface
#endif

namespace dt {

        /*!
         * \headerfile Filter.h "Filter.h"
         *
         * \brief A filter in the Data Transport system.
         *
         * A filter is applied to data in the data transport.
         */
        class Filter
        {
        public:

                /*!
                 * Constructor
                 */
                Filter();

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~Filter();

        private:

        };
} // dt namespace

#endif // _FILTER_H_
