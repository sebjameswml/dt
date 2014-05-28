/* -*-c++-*- */
/*!
 * \file Backend.h
 *
 * \brief Declares the classes \c BaseBackend and \c Backend.
 */

#ifndef _BACKEND_H_
#define _BACKEND_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <string>
#include <list>
#include <memory>

namespace dt {

        /*! \name Forward declarations */
        //@{
        class Datastream;
        class Data;
        class DatastreamOption;
        //@}

        /*!
         * \headerfile Backend.h "Backend.h"
         *
         * \brief A basic backend in the Data Transport system.
         *
         * A backend is applied to output from the data transport
         * filter chain in order to deliver the data in a specific
         * way.
         */
        class Backend
        {
        public:

                typedef std::list<std::shared_ptr<DatastreamOption> > optionList;
                typedef optionList::const_iterator ConstOptionIter;

                /*!
                 * Constructor
                 */
                Backend (const std::string& path);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~Backend();

                /*!
                 * \brief Populate argument list for this backend.
                 *
                 * Populate the argument list for this backend, based
                 * on the specified Datastream and Data objects, from
                 * which the backend arguments required for the backend
                 * may be extracted.
                 *
                 * The implementation sets up the standard CUPS backend
                 * argument list: queue, job id, user, title, copies,
                 * options.
                 *
                 * Derived implementations may specify arguments to
                 * control backend features etc.
                 */
                virtual void populateArgs (const Datastream& ds,
                                           const Data& d,
                                           std::list<std::string>& args);


                /*!
                 * \brief Whether this backend is configurable.
                 *
                 * A backend is configurable if it has one or more
                 * options.
                 */
                bool isConfigurable (void);

                /*!
                 * \brief Populate the options for this backend for
                 * the specified Datastream.
                 */
                std::list<std::pair<std::string, std::string> >
                        populateOptions (const Datastream& ds);

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                /*!
                 * \brief Get the path of this backend.
                 * @return The value of this->path.
                 */
                std::string getPath (void) const;

                /*!
                 * \brief Set the path of this backend.
                 * @param s The new value of this->path.
                 */
                void setPath (const std::string& s);


                /*!
                 * \brief Add an option to this->options.
                 * @param pOpt The option to be added.
                 */
                void addOption (std::shared_ptr<DatastreamOption> pOpt);

                //@}

                /*!
                 * \name Iterators for private containers
                 */
                //@{
                ConstOptionIter beginOptions() const { return options.cbegin(); }
                ConstOptionIter endOptions() const { return options.cend(); }
                //@}

        private:

                /*! \brief Path */
                std::string path;

                /*! \brief List of options for this backend. */
                optionList options;

                /*! \brief List of tasks for this backend. */
                //std::list<FilterTask*> tasks;
        };

        /*!
         * \headerfile Backend.h "Backend.h"
         *
         * \brief An extended backend in the Data Transport system.
         *
         * \c BackendPlus defines an extended backend type which can be
         * used as an interface to backend programs in the data
         * transport system, enabling configurable features, custom
         * parameters, unique data ID, data logging, etc.
         */
        class BackendPlus : public Backend
        {
        public:

                /*!
                 * Constructor
                 */
                BackendPlus (const std::string& path);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~BackendPlus();

                /*!
                 * \brief Populate argument list for this backend.
                 *
                 * Populate the argument list for this backend, based
                 * on the specified Datastream and Data objects, from
                 * which the backend arguments required for the backend
                 * may be extracted.
                 *
                 * Specifies arguments to control the features for
                 * this backend, as well as passing DT specific
                 * attributes such as unique data ID and data logging
                 * parameters.
                 */
                virtual void populateArgs (const Datastream& ds,
                                           const Data& d,
                                           std::list<std::string>& args);

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                //@}

        private:

        };

} // dt namespace

#endif // _BACKEND_H_
