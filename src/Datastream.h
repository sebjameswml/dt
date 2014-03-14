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

#include <wmlppcommon/WmlSettings.h>

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
                 * Create a datastream with the given ID.
                 */
                Datastream (const std::string& id);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~Datastream();

                /*!
                 * \brief Get the id of this datastream.
                 * @return The value of this->id.
                 */
                std::string getId (void) const {
                        return this->id;
                }

                /*!
                 * \brief Set the id of this datastream.
                 * @param s The new value of this->id.
                 */
                void setId (const std::string& s) {
                        this->id = s;
                }

                /*!
                 * \brief Get the name of this datastream.
                 * @return The value of this->name.
                 */
                std::string getName (void) const {
                        return this->name;
                }

                /*!
                 * \brief Set the name of this datastream.
                 * @param s The new value of this->name.
                 */
                void setName (const std::string& s) {
                        this->name = s;
                }

        private:

                /*!
                 * \brief Initialise this datastream.
                 */
                void initialise (void);

                /*! \brief Unique ID */
                std::string id;

                /*! \brief Name */
                std::string name;

                /*! \brief The list of filters */
                std::list<Filter> filters;

                /*! \brief Settings object */
                wml::WmlSettings settings;

        };
} // dt namespace

#endif // _DATASTREAM_H_
