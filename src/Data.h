/* -*-c++-*- */
/*!
 * \file Data.h
 *
 * \brief Declares the class \c Data.
 */

#ifndef _DATA_H_
#define _DATA_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <string>

namespace dt {

        /*!
         * \headerfile Data.h "Data.h"
         *
         * \brief A data item in the Data Transport system.
         */
        class Data
        {
        public:

                /*!
                 * Constructor
                 */
                Data (const std::string& id);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~Data();

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                /*!
                 * \brief Get the id of this data.
                 * @return The value of this->id.
                 */
                std::string getId (void) const {
                        return this->id;
                }

                /*!
                 * \brief Set the id of this data.
                 * @param s The new value of this->id.
                 */
                void setId (const std::string& s) {
                        this->id = s;
                }

                //@}

        private:

                /*! \brief Id */
                std::string id;

        };
} // dt namespace

#endif // _DATA_H_
