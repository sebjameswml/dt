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

#include <mysql++.h>
#include <mysqld_error.h>

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
                         * \name Internal database handling methods
                         */
                        //@{

                        /*!
                         * \brief Initialise the database.
                         */
                        void init (void);

                        /*!
                         * \brief Connect this->conn.
                         */
                        bool dbConnect (void);

                        /*!
                         * \brief Check this->conn is connected, throwing an
                         * exception if not.
                         */
                        void dbCheck (void);

                        /*!
                         * \brief Take a mysqlpp::BadQuery and throw useful
                         * exceptions.
                         *
                         * \exception runtime_error Provides details about
                         * query error.
                         */
                        void handleBadQuery (mysqlpp::BadQuery& e);

                        /*!
                         * \brief Take a mysqlpp::Exception and throw useful
                         * exception.
                         *
                         * \exception runtime_error Provides details about
                         * MySQL error.
                         */
                        void handleException (mysqlpp::Exception& e);

                        //@}

                        /*!
                         * \brief The database connection.
                         */
                        mysqlpp::Connection conn;

                        /*!
                         * \name Database credentials.
                         */
                        //@{
                        std::string db;
                        std::string dbServer;
                        std::string dbUser;
                        std::string dbPass;
                        //@}

                        /*!
                         * \name Default database credentials and settings.
                         */
                        //@{
                        static const std::string defaultDb;
                        static const std::string defaultDbServer;
                        static const std::string defaultDbUser;
                        static const std::string defaultDbPass;
                        static const std::string defaultDbTable;
                        //@}
                };
        } // log namespace
} // dt namespace

#endif // _LOG_MYSQLSTORAGE_H_
