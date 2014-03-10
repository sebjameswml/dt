/* -*-c++-*- */
/*!
 * \file Storage.h
 *
 * \brief Declares the class \c Storage.
 *
 * Provides an abstract interface for storing a log event to a storage
 * facility, such as a database backend, a file system or a system
 * log.
 */

#ifndef _LOG_STORAGE_H_
#define _LOG_STORAGE_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <string>

namespace dt {
        namespace log {

                class Event;

                /*!
                 * \headerfile Storage.h "log/Storage.h"
                 *
                 * \brief \c Storage provides an abstract interface for
                 * storing a log event to a storage facility such as a
                 * database backend, a file system or a system log.
                 *
                 * \note Event storage implements the visitor pattern,
                 * with the Storage object as the visitor.
                 */
                class Storage
                {
                public:
                        /*!
                         * Constructor
                         */
                        Storage();

                        /*!
                         * \brief Destructor
                         * \note Declared virtual to ensure proper cleanup of
                         * derived class objects.
                         */
                        virtual ~Storage();

                        /*!
                         * \brief Store an event.
                         *
                         * This method must be implemented in derived
                         * classes with details specific to the
                         * storage type.
                         *
                         * \note This is the 'visit' method in the
                         * visitor pattern.
                         */
                        virtual void store (Event* e) = 0;

                        /*!
                         * \name Private attribute accessor methods
                         */
                        //@{

                        /*!
                         * Set the value of address
                         * @param addr the new value of address
                         */
                        void setAddress (const std::string& addr) {
                                address = addr;
                        }

                        /*!
                         * Get the value of address
                         * @return the value of address
                         */
                        std::string getAddress (void) const {
                                return address;
                        }

                        //@}

                private:

                        std::string address;

                };
        } // log namespace
} // dt namespace

#endif // _LOG_STORAGE_H_
