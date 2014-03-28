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
#include <sstream>

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
                 * \brief Get the target datastream for this data.
                 * @return The value of this->targetDatastream.
                 */
                std::string getTargetDatastream (void) const {
                        return this->targetDatastream;
                }

                /*!
                 * \brief Set the target datastream for this data.
                 * @param s The new value of this->targetDatastream.
                 */
                void setTargetDatastream (const std::string& s) {
                        this->targetDatastream = s;
                }

                /*!
                 * \brief Get the name of this data.
                 * @return The value of this->name.
                 */
                std::string getName (void) const {
                        return this->name;
                }

                /*!
                 * \brief Set the name of this data.
                 * @param s The new value of this->name.
                 */
                void setName (const std::string& s) {
                        this->name = s;
                }

                /*!
                 * \brief Get the user of this data.
                 * @return The value of this->user.
                 */
                std::string getUser (void) const {
                        return this->user;
                }

                /*!
                 * \brief Set the user of this data.
                 * @param s The new value of this->user.
                 */
                void setUser (const std::string& s) {
                        this->user = s;
                }

                /*!
                 * \brief Get the number of copies of this data.
                 * @return The value of this->copies.
                 */
                int getCopies (void) const {
                        return this->copies;
                }

                /*!
                 * \brief Get the number of copies of this data.
                 * @return The value of this->copies.
                 */
                std::string getCopiesStr (void) const {
                        std::stringstream ss;
                        ss << this->copies;
                        return ss.str();
                }

                /*!
                 * \brief Set the number of copies of this data.
                 * @param i The new value of this->copies.
                 */
                void setCopies (const int i) {
                        this->copies = i;
                }

                /*!
                 * \brief Get the path of this data.
                 * @return The value of this->path.
                 */
                std::string getPath (void) const {
                        return this->path;
                }

                /*!
                 * \brief Set the path of this data.
                 * @param s The new value of this->path.
                 */
                void setPath (const std::string& s) {
                        this->path = s;
                }

                //@}

        private:

                /*! \brief The ID of this data. */
                std::string id;

                /*! The user-supplied job name. */
                std::string name;

                /*! The originating user. */
                std::string user;

                /*! Number of copies. */
                int copies;

                /*! Location of the data. */
                std::string path;

                /*! \brief Target datastream (ID?) on which to process the data. */
                std::string targetDatastream;

        };
} // dt namespace

#endif // _DATA_H_
