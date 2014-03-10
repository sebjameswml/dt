/* -*-c++-*- */
/*!
 * \file MySQLStorage.h
 *
 * \brief Declares the class \c MySQLStorage.
 *
 * Provides methods for storing a log event to a MySQL database
 * backend.
 */

#ifndef _LOG_MYSQLSTORAGE_H_
#define _LOG_MYSQLSTORAGE_H_

#ifdef __GNUG__
# pragma interface
#endif

#include "Storage.h"

namespace dt {
        namespace log {

                /*!
                 * \headerfile MySQLStorage.h "log/MySQLStorage.h"
                 *
                 * \brief \c MySQLStorage provides methods for storing
                 * a log event to a MySQL database backend.
                 */
                class MySQLStorage : public Storage
                {
                public:
                        /*!
                         * Constructor
                         */
                        MySQLStorage();

                        /*!
                         * Destructor
                         */
                        virtual ~MySQLStorage();

                        /*!
                         * \brief Store an event.
                         */
                        void store (dt::log::Event* e);

                        /*!
                         * \name Accessors for database credentials
                         */
                        //@{
                        std::string getDb (void) const;
                        std::string getDbServer (void) const;
                        std::string getDbUser (void) const;
                        std::string getDbPass (void) const;
                        void setDb (const std::string& s);
                        void setDbServer (const std::string& s);
                        void setDbUser (const std::string& s);
                        void setDbPass (const std::string& s);
                        //@}

                private:

                        /*!
                         * \name Database credentials.
                         */
                        //@{
                        std::string db;
                        std::string dbServer;
                        std::string dbUser;
                        std::string dbPass;
                        //@}

                };
        } // log namespace
} // dt namespace

#endif // _LOG_MYSQLSTORAGE_H_
