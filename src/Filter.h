/* -*-c++-*- */
/*!
 * \file Filter.h
 *
 * \brief Declares the classes \c BaseFilter and \c Filter.
 */

#ifndef _FILTER_H_
#define _FILTER_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <string>
#include <list>
#include <memory>

#include "FilterFeature.h"

namespace dt {

        /*! \name Forward declarations */
        //@{
        class Datastream;
        class Data;
        //@}

        /*!
         * \headerfile Filter.h "Filter.h"
         *
         * \brief A basic filter in the Data Transport system.
         *
         * A filter is applied to data in the data transport to
         * perform some action such as conversion between different
         * formats.
         */
        class BaseFilter
        {
        public:

                /*!
                 * Constructor
                 */
                BaseFilter (const std::string& path);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~BaseFilter();

                /*!
                 * \brief Populate argument list for this filter.
                 *
                 * Populate the argument list for this filter, based
                 * on the specified Datastream and Data objects, from
                 * which the filter arguments required for the filter
                 * may be extracted.
                 *
                 * The implementation sets up the standard CUPS filter
                 * argument list: queue, job id, user, title, copies,
                 * options.
                 *
                 * Derived implementations may specify arguments to
                 * control filter features etc.
                 */
                virtual void populateArgs (const Datastream& ds,
                                           const Data& d,
                                           std::list<std::string>& args);

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                /*!
                 * \brief Get the path of this filter.
                 * @return The value of this->path.
                 */
                std::string getPath (void) const;

                /*!
                 * \brief Set the path of this filter.
                 * @param s The new value of this->path.
                 */
                void setPath (const std::string& s);

                /*!
                 * \brief Get the list of features for this filter.
                 * @return The value of this->features.
                 */
                std::list<std::string> getFeatures (void) const;

                //@}

        private:

                /*! \brief Path */
                std::string path;

                //std::list<std::shared_ptr<FilterFeature> > features;
                std::list<std::string> features;

        };

        /*!
         * \headerfile Filter.h "Filter.h"
         *
         * \brief An extended filter in the Data Transport system.
         *
         * \c Filter defines an extended filter type which can be
         * used as an interface to filter programs in the data
         * transport system, enabling configurable features, custom
         * parameters, unique data ID, data logging, etc.
         */
        class Filter : public BaseFilter
        {
        public:

                /*!
                 * Constructor
                 */
                Filter (const std::string& path);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~Filter();

                /*!
                 * \brief Populate argument list for this filter.
                 *
                 * Populate the argument list for this filter, based
                 * on the specified Datastream and Data objects, from
                 * which the filter arguments required for the filter
                 * may be extracted.
                 *
                 * Specifies arguments to control the features for
                 * this filter, as well as passing DT specific
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

#endif // _FILTER_H_
