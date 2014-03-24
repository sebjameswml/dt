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
                std::string getPath (void) const {
                        return this->path;
                }

                /*!
                 * \brief Set the path of this filter.
                 * @param s The new value of this->path.
                 */
                void setPath (const std::string& s) {
                        this->path = s;
                }

                //@}

        private:

                /*! \brief Path */
                std::string path;

        };
} // dt namespace

#endif // _FILTER_H_
