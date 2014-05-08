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

#include <string>
#include <list>
#include <memory>

#include "FilterFeature.h"

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
                Filter (const std::string& path);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~Filter();

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
                std::list<std::string> getFeatures (void) const {
                        return this->features;
                }


                //@}

        private:

                /*! \brief Path */
                std::string path;

                //std::list<std::shared_ptr<FilterFeature> > features;
                std::list<std::string> features;

        };
} // dt namespace

#endif // _FILTER_H_
