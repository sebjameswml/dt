/* -*-c++-*- */
/*!
 * \file Event.h
 *
 * \brief Declares the class \c Event.
 */

#ifndef _LOG_EVENT_H_
#define _LOG_EVENT_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <sys/time.h>

#include <string>

#include "Storage.h"

namespace dt {
        namespace log {

                /*!
                 * \headerfile Event.h "log/Event.h"
                 *
                 * \brief An event to be logged in the Data Transport
                 * system.
                 *
                 * Each event has its own unique ID and is related to
                 * data in the data transport. Thus an audit trail of
                 * events relating to a particular item can be built
                 * up as a series of Event objects linked by data ID.
                 */
                class Event
                {
                public:

                        /*!
                         * Constructor
                         */
                        Event();

                        /*!
                         * Destructor
                         * \note Declared virtual to ensure proper
                         * cleanup of derived class objects.
                         */
                        virtual ~Event();

                        /*!
                         * Accept a Storage object to store this event.
                         *
                         * \note Event storage implements the visitor
                         * pattern.
                         */
                        void accept (Storage& s);

                        /*!
                         * \name Private attribute accessor methods
                         */
                        //@{

                        /*!
                         * \brief Get the ID of this event.
                         * @return The value of this->id.
                         */
                        std::string getId (void) const {
                                return this->id;
                        }

                        /*!
                         * \brief Set the associated data ID.
                         * @param theDataId The new value of this->dataId.
                         */
                        void setDataId (const std::string& theDataId) {
                                this->dataId = theDataId;
                        }

                        /*!
                         * \brief Get the associated data ID.
                         * @return The value of this->dataId.
                         */
                        std::string getDataId (void) const {
                                return this->dataId;
                        }

                        /*!
                         * \brief Set the value of the message for this event.
                         * @param msg The new value of this->message.
                         */
                        void setMessage (const std::string& msg) {
                                this->message = msg;
                        }

                        /*!
                         * \brief Get the value of the message for this event.
                         * @return The value of this->message.
                         */
                        std::string getMessage (void) const {
                                return this->message;
                        }

#ifdef GOT_DT_ERROR_CODE
                        /*!
                         * \brief Set the value of the error code for this event.
                         * @param err The new value of this->errorCode.
                         */
                        void setErrorCode (dt::errorCode err) {
                                this->errorCode = err;
                        }

                        /*!
                         * \brief Get the value of the error code for this event.
                         * @return The value of this->errorCode.
                         */
                        dt::errorCode getErrorCode (void) const {
                                return this->errorCode;
                        }
#endif

                        /*!
                         * \brief Set the value of the datastream ID.
                         * @param dsId The new value of this->datastreamId.
                         */
                        void setDatastreamId (const std::string& dsId) {
                                this->datastreamId = dsId;
                        }

                        /*!
                         * \brief Get the value of the datastream ID.
                         * @return The value of this->datastreamId.
                         */
                        std::string getDatastreamId (void) const {
                                return this->datastreamId;
                        }

                        /*!
                         * \brief Set the value of the datastream name.
                         * @param dsName The new value of this->datastreamName.
                         */
                        void setDatastreamName (const std::string& dsName) {
                                this->datastreamName = dsName;
                        }

                        /*!
                         * \brief Get the value of the datastream name.
                         * @return The value of this->datastreamName.
                         */
                        std::string getDatastreamName (void) const {
                                return this->datastreamName;
                        }

                        /*!
                         * \brief Set the value of the datastream hostname.
                         * @param hName The new value of this->hostname.
                         */
                        void setHostname (const std::string& hName) {
                                this->hostname = hName;
                        }

                        /*!
                         * \brief Get the value of the datastream hostname.
                         * @return The value of this->hostname.
                         */
                        std::string getHostname (void) const {
                                return this->hostname;
                        }

#ifdef GOT_DT_LOG_LEVEL
                        /*!
                         * \brief Set the log level for this event.
                         * @param lvl The new value of this->level.
                         */
                        void setLevel (dt::logLevel lvl) {
                                this->level = lvl;
                        }

                        /*!
                         * \brief Get the log level for this event.
                         * @return The value of this->level.
                         */
                        dt::logLevel getLevel (void) const {
                                return this->level;
                        }
#endif

                        /*!
                         * \brief Set the process ID for this event.
                         * @param thePid The new value of this->pid.
                         */
                        void setPid (pid_t thePid) {
                                this->pid = thePid;
                        }

                        /*!
                         * \brief Get the process ID for this event.
                         * @return The value of this->pid.
                         */
                        pid_t getPid (void) const {
                                return this->pid;
                        }

                        /*!
                         * \brief Set the time for this event.
                         * @param theTime The new value of this->time.
                         */
                        void setTime (struct timeval theTime) {
                                this->time = theTime;
                        }

                        /*!
                         * \brief Get the time for this event.
                         * @return The value of this->time.
                         */
                        struct timeval getTime (void) const {
                                return this->time;
                        }

                        //@}

                private:

                        /*!
                         * \brief Generate the ID for this event.
                         */
                        void generateId (void);

                        /*! \brief The ID of this event. */
                        std::string id;
                        /*! \brief The associated data ID. */
                        std::string dataId;
                        /*! \brief The message for this event. */
                        std::string message;
#ifdef GOT_DT_ERROR_CODE
                        /*! \brief The error code for this event. */
                        dt::errorCode errorCode;
#endif
                        /*! \brief The datastream ID. */
                        std::string datastreamId;
                        /*! \brief The datastream name. */
                        std::string datastreamName;
                        /*! \brief The datastream host name. */
                        std::string hostname;
#ifdef GOT_DT_LOG_LEVEL
                        /*! \brief The log level. */
                        dt::logLevel level;
#endif
                        /*! \brief The process ID. */
                        pid_t pid;

                        /*! \brief The time of the event. */
                        struct timeval time;

                };

        } // log namespace
} // dt namespace

#endif // _LOG_EVENT_H_
