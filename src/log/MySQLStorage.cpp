#ifdef __GNUG__
#  pragma implementation
#endif

#include "config.h"

#include "Event.h"
#include "MySQLStorage.h"

using namespace std;
using namespace dt;
using namespace dt::log;
using namespace mysqlpp;

const string MySQLStorage::defaultDb = "dt";
const string MySQLStorage::defaultDbServer = "127.0.0.1";
const string MySQLStorage::defaultDbUser = "dtuser";
const string MySQLStorage::defaultDbPass = "password";
const string MySQLStorage::defaultDbTable = "log";

// Constructors/Destructors
//

MySQLStorage::MySQLStorage()
{
}

MySQLStorage::~MySQLStorage()
{
        if (this->conn.connected()) {
                this->conn.disconnect();
        }
}

//
// Methods
//

void
MySQLStorage::store (Event* e)
{
        try {
                this->init();
        } catch (const exception& e) {
                stringstream errss;
                errss << "Couldn't initialise MySQL database: " << e.what();
                throw runtime_error (errss.str());
        }

        try {
                // Store the event to the database.
                this->dbCheck();
                Query query (this->conn.query());

                query << "INSERT INTO `log` "
                      << "( `time`"
                      << ", `ms`"
                      << ", `eventId`"
                      << ", `dataId`"
                      << ", `message`"
#ifdef GOT_DT_ERROR_CODE
                      << ", `errorCode`"
#endif
                      << ", `datastreamId`"
                      << ", `datastreamName`"
                      << ", `hostname`"
#ifdef GOT_DT_LOG_LEVEL
                      << ", `logLevel`"
#endif
                      << ", `pid`"
                      << ")"
                      << " VALUES "
                      << " ( FROM_UNIXTIME(" << e->getTime().tv_sec << ")"
                      << " , '" << e->getTime().tv_usec/1000
                      << "', '" << e->getId() // SQL safe? (uuid)
                      << "', '" << e->getDataId() // SQL safe? (uuid)
                      << "', '" << e->getMessage() // Need to escape
#ifdef GOT_DT_ERROR_CODE
                      << "', '" << e->getErrorCode() // SQL safe (int)
#endif
                      << "', '" << e->getDatastreamId() // SQL safe? (uuid)
                      << "', '" << e->getDatastreamName() // SQL safe? (datastreamNames can't contain '; etc)
                      << "', '" << e->getHostname() // SQL safe?
#ifdef GOT_DT_LOG_LEVEL
                      << "', '" << e->getLogLevel() // SQL safe (int)
#endif
                      << "', '" << e->getPid() // SQL safe (int)
                      << "');";

                //DBG ("INSERT Query string: " << query.str());

                if (query.exec() == false) {
                        stringstream ee;
                        ee << "Query failed, error: '" << query.error() << "'";
                        throw runtime_error (ee.str());
                }
        }
        catch (mysqlpp::BadQuery e) { this->handleBadQuery (e); }
        catch (mysqlpp::Exception e) { this->handleException (e); }
}

// Accessor methods
//


// Other methods
//

void
MySQLStorage::init (void)
{
        vector<pair<string, string> > fields;
        fields.push_back (make_pair ("id",             "int(10) unsigned NOT NULL auto_increment"));
        fields.push_back (make_pair ("eventId",        "varchar(255) collate latin1_bin NOT NULL default ''"));
        fields.push_back (make_pair ("dataId",         "varchar(255) collate latin1_bin NOT NULL default ''"));
        fields.push_back (make_pair ("message",        "varchar(255) collate latin1_bin NOT NULL default ''"));
        fields.push_back (make_pair ("errorCode",      "int(10) unsigned NOT NULL default 0"));
        fields.push_back (make_pair ("datastreamId",   "varchar(255) collate latin1_bin NOT NULL default ''"));
        fields.push_back (make_pair ("datastreamName", "varchar(255) collate latin1_bin NOT NULL default ''"));
        fields.push_back (make_pair ("hostname",       "varchar(255) collate latin1_bin NOT NULL default ''"));
        fields.push_back (make_pair ("logLevel",       "int(10) unsigned NOT NULL default 0")); // could be enum?
        fields.push_back (make_pair ("pid",            "int(10) unsigned NOT NULL default 0"));
        fields.push_back (make_pair ("time",           "DATETIME NOT NULL"));
        fields.push_back (make_pair ("ms",             "int(10) unsigned NOT NULL default 0"));

        string table (MySQLStorage::defaultDbTable);
        try {
                this->dbCheck();

                Query query (this->conn.query());

                // Create the database if necessary
                query << "CREATE TABLE IF NOT EXISTS `" << table <<  "` "
                      << "(";

                bool first (true);
                auto iField (fields.begin()), fEnd (fields.end());
                while (iField != fEnd) {
                        if (!first) {
                                query << ", ";
                        } else {
                                first = false;
                        }
                        query << "`" << iField->first << "` " << iField->second;
                        ++iField;
                }
                query << ", PRIMARY KEY (`id`)"
                      << ") "
                      << "ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_bin; ";

                if (query.exec() == false) {
                        stringstream ee;
                        ee << "Query failed, error: '" << query.error() << "'";
                        throw runtime_error (ee.str());
                }

                // Check that all of the columns exist
                iField = fields.begin();
                while (iField != fEnd) {
                        query.reset();
                        query << "SELECT * FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = 'dt'"
                              << " AND TABLE_NAME = '" << table << "' AND COLUMN_NAME = '" << iField->first << "';";

                        UseQueryResult res (query.use());
                        if (!res) {
                                stringstream ee;
                                ee << "Query failed, error: '" << query.error() << "'";
                                throw runtime_error (ee.str());
                        }

                        if (Row row = res.fetch_row()) {
                        } else {
                                // Need to add the column
                                //DBG ("Couldn't find the column '" << iField->first << "'");
                                query.reset();
                                query << "ALTER TABLE `" << table << "` ADD `" << iField->first << "` " << iField->second << ";";
                                //DBG (name << " ALTER query string: " << query.str());
                                if (query.exec() == false) {
                                        stringstream ee;
                                        ee << "Query failed, error: '" << query.error() << "'";
                                        throw runtime_error (ee.str());
                                }
                        }

                        ++iField;
                }
        }
        catch (mysqlpp::BadQuery e) { this->handleBadQuery (e); }
        catch (mysqlpp::Exception e) { this->handleException (e); }
}

bool
MySQLStorage::dbConnect (void)
{
        //DBG ("Connecting to " << this->db
             // << " on " << this->dbServer
             // << " with " << this->dbUser
             // << " and " << this->dbPass);
        return this->conn.connect (this->db.c_str(),
                                   this->dbServer.c_str(),
                                   this->dbUser.c_str(),
                                   this->dbPass.c_str());
}

void
MySQLStorage::dbCheck (void)
{
        if (!this->conn.connected()) {
                try {
                        if (this->dbConnect()) {
                                //DBG ("Successfully connected to database server");
                        } else {
                                stringstream ee;
                                ee << "Connection to '" << this->db
                                   << "' to access '" << this->dbServer
                                   << "' as user '" << this->dbUser
                                   << "' with the current password has failed.";
                                throw runtime_error (ee.str());
                        }
                }
                catch (mysqlpp::Exception e) { this->handleException (e); }
        }
        if (!this->conn.connected()) {
                throw runtime_error ("Not connected to MySQL database server. "
                                     "Please check database settings.");
        }
}

void
MySQLStorage::handleBadQuery (mysqlpp::BadQuery& e)
{
        //DBG ("mysqlpp::BadQuery exception occurred");
        stringstream ee;
        if (e.errnum() == ER_LOCK_DEADLOCK) {
                ee << "Connection::errnum = " << this->conn.errnum()
                   << ", BadQuery::errnum = " << e.errnum();
                throw runtime_error (ee.str());
        } else {
                ee << "Unexpected query error: " << e.what();
                throw runtime_error (ee.str());
        }
}

void
MySQLStorage::handleException (mysqlpp::Exception& e)
{
        //DBG ("mysqlpp::Exception occurred");
        stringstream ee;
        ee << "General mysqlpp error: " << e.what();
        throw runtime_error (ee.str());
}
