/* -*-c++-*- */
/*!
 * \file SyslogStorage.h
 *
 * \brief Declares the class \c SyslogStorage.
 *
 * Provides methods for storing an event to the system log.
 */

#ifndef _LOG_SYSLOGSTORAGE_H_
#define _LOG_SYSLOGSTORAGE_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <syslog.h>

#include "Storage.h"

namespace dt {
        namespace log {

                /*!
                 * \headerfile SyslogStorage.h "log/SyslogStorage.h"
                 *
                 * \brief \c SyslogStorage provides methods for storing
                 * an event to the system log.
                 */
                class SyslogStorage : public Storage
                {
                public:
                        /*!
                         * Create a SyslogStorage object using the
                         * specified facility.
                         *
                         * \param facility Indicates what type of
                         * program is generating the messages to be
                         * sent to the system logger.
                         */
                        SyslogStorage (int facility = LOG_USER);

                        /*!
                         * Destructor
                         */
                        virtual ~SyslogStorage();

                        /*!
                         * \brief Store an event.
                         *
                         * Sends a message containing details of the
                         * pointed to event to the system logger.
                         */
                        void store (dt::log::Event* e);

                        /*!
                         * \name Private attribute accessor methods
                         */
                        //@{
                        //@}

                private:

                };
        } // log namespace
} // dt namespace

#endif // _LOG_SYSLOGSTORAGE_H_
