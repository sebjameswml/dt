/* -*-c++-*- */
/*!
 * \file Datastream.h
 *
 * \brief Declares the class \c Datastream.
 */

#ifndef _DATASTREAM_H_
#define _DATASTREAM_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <list>

#include "Filter.h"

namespace dt {

        /*!
         * \headerfile Datastream.h "Datastream.h"
         *
         * \brief A datastream in the Data Transport system.
         *
         * A datastream defines a filter chain and/or backend
         * to be applied to data in the data transport.
         */
        class Datastream
        {
        public:

                /*!
                 * Constructor
                 */
                Datastream();

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~Datastream();

        private:

                std::list<Filter> filters;

        };
} // dt namespace

#endif // _DATASTREAM_H_
