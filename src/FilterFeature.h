/* -*-c++-*- */
/*!
 * \file FilterFeature.h
 *
 * \brief Declares the class \c FilterFeature.
 */

#ifndef _FILTERFEATURE_H_
#define _FILTERFEATURE_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <string>
#include <list>
#include <memory>

namespace dt {

        /*!
         * \brief Forward declarations.
         */
        class Datastream;
        class DatastreamOption;

        /*!
         * \headerfile FilterFeature.h "FilterFeature.h"
         *
         * \brief A filter feature in the Data Transport system.
         *
         * \c FilterFeature provides an abstract interface to a
         * feature offered by a filter in the Data Transport system.
         *
         * A \c FilterFeature has a name and a label and contains a
         * list of options for controlling the feature.
         */
        class FilterFeature
        {
        public:

                typedef std::list<std::shared_ptr<DatastreamOption> > optionList;
                typedef optionList::const_iterator ConstOptionIter;

                /*!
                 * Constructor
                 */
                FilterFeature (const std::string& name, const std::string& label);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~FilterFeature() = 0;

                /*!
                 * \brief Whether this feature is configurable.
                 *
                 * A feature is configurable if it has one or more
                 * options.
                 */
                bool isConfigurable (void);

                /*!
                 * \brief Populate the options for this feature for
                 * the specified Datastream and filter path.
                 */
                std::list<std::pair<std::string, std::string> >
                populateOptions (const Datastream& ds,
                                 const std::string& filt);

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                /*!
                 * \brief Get the name of this FilterFeature.
                 * @return The value of this->name.
                 */
                std::string getName (void) const;

                /*!
                 * \brief Set the name of this FilterFeature.
                 * @param s The new value of this->name.
                 */
                void setName (const std::string& s);

                /*!
                 * \brief Get the description of this FilterFeature.
                 * @return The value of this->desc.
                 */
                std::string getDesc (void) const;

                /*!
                 * \brief Set the description of this FilterFeature.
                 * @param s The new value of this->desc.
                 */
                void setDesc (const std::string& s);

                /*!
                 * \brief Get the label of this FilterFeature.
                 * @return The value of this->label.
                 */
                std::string getLabel (void) const;

                /*!
                 * \brief Set the label of this FilterFeature.
                 * @param s The new value of this->label.
                 */
                void setLabel (const std::string& s);

                /*!
                 * \brief Add an option to this->options.
                 * @param pOpt The option to be added.
                 */
                void addOption (std::shared_ptr<DatastreamOption> pOpt);

                //@}

                /*!
                 * \name Iterators for private containers
                 */
                //@{
                ConstOptionIter beginOptions() const { return options.cbegin(); }
                ConstOptionIter endOptions() const { return options.cend(); }
                //@}

        private:

                /*! \brief The name of this filter feature. */
                std::string name;

                /*! \brief The description of this filter feature. */
                std::string desc;

                /*! \brief The label of this filter feature. */
                std::string label;

                /*! \brief List of options for this filter feature. */
                optionList options;

                /*! \brief List of tasks for this filter feature. */
                //std::list<FilterTask*> tasks;

        };

} // dt namespace

#endif // _FILTERFEATURE_H_
