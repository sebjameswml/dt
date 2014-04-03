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

#include <futil/Process.h>
#include <wmlppcommon/WmlSettings.h>

#include "Data.h"

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
                 * \brief Store the datastream settings.
                 */
                void write (void);

                /*!
                 * \brief Process the passed in data.
                 *
                 * \remark Need to feedback outcome to caller in some
                 * way? Simply log any related events?
                 */
                void process (dt::Data& data);

                /*!
                 * \brief Respond to stdout from a filter process.
                 */
                void filterStdoutReady (void);

                /*!
                 * \brief Respond to stderr from a filter process.
                 */
                void filterStderrReady (void);

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

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

                /*!
                 * \brief Set the list of filters for this datastream.
                 * @param l The new value of this->filters.
                 */
                void setFilters (const std::list<std::string>& f) {
                        this->filters = f;
                }

                //@}

        private:

                /*!
                 * \brief Initialise this datastream.
                 */
                void initialise (void);

                /*!
                 * \brief Populate the list of filters according to
                 * the specified input data and the destination mime
                 * type for this datastream.
                 */
                void populateFilters (dt::Data& data);

                /*!
                 * \brief Log an event for the specified data.
                 *
                 * \todo Enable specification of log level.
                 */
                void logEvent (dt::Data& data, const std::string& msg);

                /*! \brief The unique ID of this datastream. */
                std::string id;

                /*! \brief The name of this datastream. */
                std::string name;

                /*!
                 * \brief The list of filters to be applied by this
                 * datastream.
                 *
                 * If this list is empty, automatic filter selection
                 * should be carried out, on the basis of the type of
                 * input data and the selection rules indicated by
                 * mime.types and mime.convs (as in CUPS).
                 */
                std::list<std::string> filters;

                /*! \brief Settings object. */
                wml::WmlSettings settings;

                /*!
                 * \brief Object for processing data through filters.
                 */
                wml::Process p;

                /*!
                 * \brief Storage for filter output.
                 */
                std::string filterOutput;

                /*!
                 * \brief Storage for filter error.
                 */
                std::string filterError;

                /*!
                 * \brief Iterator to the last filter run by this
                 * datastream.
                 */
                std::list<std::string>::const_iterator lastFilter;

        };
} // dt namespace

#endif // _DATASTREAM_H_
