/* -*-c++-*- */
/*!
 * \file DatastreamOption.h
 *
 * \brief Declares the class \c DatastreamOption.
 */

#ifndef _DATASTREAMOPTION_H_
#define _DATASTREAMOPTION_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <string>
#include <list>
#include <memory>
#include <functional>

namespace dt {

        /*!
         * \brief Forward declarations.
         */
        class Datastream;
        class DatastreamOptionVisitor;

        /*!
         * \headerfile DatastreamOption.h "DatastreamOption.h"
         *
         * \brief Defines an option for a Datastream in the Data
         * Transport system.
         *
         * Defines an option relating to a filter feature or backend
         * for a Datastream object. An option has a type, name, label and value.
         *
         * The base class defines a simple string-valued option
         * without any constraints. Derived classes may specify
         * constraints on the value (e.g. TextOption has maxLength) or
         * may use a different value type (e.g. BooleanOption has
         * boolValue).
         */
        class DatastreamOption
        {
        public:

                typedef std::list<std::pair<std::string, std::string> > keyValList;

                /*!
                 * The type or format for specifying a filter option.
                 */
                enum TYPE {
                        TYPE_UNSET
                        , TEXT              /*<! Text */
                        , OPTION_LIST       /*<! Option list */
                        , BOOLEAN           /*<! Boolean */
                        , UINT              /*<! Unsigned int */
                        , INT               /*<! Int */
                        , FLOAT             /*<! Float */
                        , COMPOSITE         /*<! Multiple text fields */
                };

                /*!
                 * Constructor
                 */
                DatastreamOption (const std::string& name,
                                  const std::string& label,
                                  const std::string& units = "",
                                  const bool req = false);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~DatastreamOption();

                /*!
                 * \brief Populate this DatastreamOption from the
                 * stored Datastream settings.
                 */
                virtual keyValList populate (const Datastream& ds,
                                             const std::string& program,
                                             const std::string& feature = "");

                /*!
                 * \brief Accept a visit from the specified visitor.
                 */
                virtual void accept (const DatastreamOptionVisitor& visitor) = 0;

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                /*!
                 * \brief Get the type of this DatastreamOption.
                 * @return The value of this->type.
                 */
                DatastreamOption::TYPE getType (void) const;

                /*!
                 * \brief Get the name of this DatastreamOption.
                 * @return The value of this->name.
                 */
                std::string getName (void) const;

                /*!
                 * \brief Set the name of this DatastreamOption.
                 * @param s The new value of this->name.
                 */
                void setName (const std::string& s);

                /*!
                 * \brief Get the value of this DatastreamOption.
                 * @return The value of this->value.
                 */
                virtual std::string getValue (void) const;

                /*!
                 * \brief Set the value of this DatastreamOption.
                 * @param s The new value of this->value.
                 */
                virtual void setValue (const std::string& s);

                /*!
                 * \brief Get the label for this DatastreamOption.
                 * @return The value of this->label.
                 */
                std::string getLabel (void) const;

                /*!
                 * \brief Set the label for this DatastreamOption.
                 * @param s The new value of this->label.
                 */
                void setLabel (const std::string& s);

                /*!
                 * \brief Get the units label for this DatastreamOption.
                 * @return The value of this->unitsLabel.
                 */
                std::string getUnitsLabel (void) const;

                /*!
                 * \brief Set the units label for this DatastreamOption.
                 * @param s The new value of this->unitsLabel.
                 */
                void setUnitsLabel (const std::string& s);

                /*!
                 * \brief Get whether a value is required for this
                 * DatastreamOption.
                 * @return The value of this->required.
                 */
                bool getRequired (void) const;

                /*!
                 * \brief Set whether a value is required for this
                 * DatastreamOption.
                 * @param b The new value of this->required.
                 */
                void setRequired (const bool b);

                //@}

        protected:

                /*! \brief The type of this filter option. */
                DatastreamOption::TYPE type;

        private:

                /*! \brief The name of this filter option. */
                std::string name;

                /*! \brief The value of this filter option. */
                std::string value;

                /*! \brief The label for this filter option. */
                std::string label;

                /*! \brief The units label for this filter option. */
                std::string unitsLabel;

                /*! \brief Whether a value is required for this filter
                 *  option. */
                bool required;

        };

        /*!
         * \headerfile DatastreamOption.h "DatastreamOption.h"
         *
         * \brief A text format filter option in the Data Transport
         * system.
         *
         * \c TextOption defines a text-based option for a filter
         * feature in the Data Transport system.
         */
        class TextOption : public DatastreamOption
        {
        public:
                /*!
                 * Constructor
                 */
                TextOption (const std::string& name,
                            const std::string& label,
                            const std::string& units = "",
                            const bool req = false);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~TextOption();

                /*!
                 * \brief Accept a visit from a DatastreamOptionVisitor.
                 */
                void accept (const DatastreamOptionVisitor& visitor);

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                /*!
                 * \brief Get the maximum length of this TextOption.
                 * @return The value of this->maxLength.
                 */
                int getMaxLength (void) const;

                /*!
                 * \brief Set the maximum length of this TextOption.
                 * @param i The new value of this->maxLength.
                 */
                void setMaxLength (const int i);

                //@}

        private:

                /*! \brief The maximum length of this TextOption. */
                int maxLength;
        };

        /*!
         * \headerfile DatastreamOption.h "DatastreamOption.h"
         *
         * \brief A list format filter option in the Data Transport
         * system.
         *
         * \c ListOption defines a list-based option for a filter
         * feature in the Data Transport system.
         *
         * \todo Allow client to specify a function object that can be
         * used to get a list of the available options?
         */
        class ListOption : public DatastreamOption
        {
        public:
                /*!
                 * \brief List of label/value pairs for use in an option list.
                 */
                typedef std::list<std::pair<std::string, std::string> > optList;

                /*!
                 * Constructor
                 */
                ListOption (const std::string& name,
                            const std::string& label,
                            std::function<optList()> = nullptr);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~ListOption();

                /*!
                 * \brief Accept a visit from a DatastreamOptionVisitor.
                 */
                void accept (const DatastreamOptionVisitor& visitor);

                /*!
                 * \brief Get the list of options by invoking
                 * this->optionListBuilder.
                 *
                 * \note If this->optionListBuilder has not been
                 * assigned, an empty list is returned.
                 */
                optList getOptionList (void) const;

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                /*!
                 * \brief Set the function object that will provide
                 * the list of options.
                 */
                void setOptionListBuilder (std::function<optList()>);

                //@}

        private:

                std::function<optList(void)> optionListBuilder;
        };

        /*!
         * \headerfile DatastreamOption.h "DatastreamOption.h"
         *
         * \brief A boolean format filter option in the Data
         * Transport system.
         *
         * \c BoolOption defines a boolean-based option for a filter
         * in the Data Transport system.
         */
        class BoolOption : public DatastreamOption
        {
        public:
                /*!
                 * Constructor
                 */
                BoolOption (const std::string& name,
                            const std::string& label,
                            const std::string& units = "",
                            const bool req = false);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~BoolOption();

                /*!
                 * \brief Accept a visit from a DatastreamOptionVisitor.
                 */
                void accept (const DatastreamOptionVisitor& visitor);

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                /*!
                 * \brief Get the value of this BoolOption as a string.
                 * @return The value of this->boolValue, converted to a string value.
                 */
                std::string getValue (void) const;

                /*!
                 * \brief Set the value of this BoolOption.
                 * @param s The new value as a string.
                 */
                void setValue (const std::string& s);

                /*!
                 * \brief Set the value of this BoolOption.
                 *
                 * \note This version is specified in order to prevent
                 * a call of setValue("myString") from falling back to
                 * the bool overload due to implicit type conversion.
                 *
                 * @param s The new value as const char *.
                 */
                void setValue (const char * s);

                /*!
                 * \brief Set the boolean value of this BoolOption.
                 * @param b The new value of this->boolValue.
                 */
                void setValue (const bool b);

                /*!
                 * \brief Get the value of this BoolOption as a boolean.
                 * @return The value of this->boolValue.
                 */
                bool getBoolValue (void) const;

                //@}

        private:

                /*! \brief The boolean value of this filter option. */
                bool boolValue;
        };

        /*!
         * \headerfile DatastreamOption.h "DatastreamOption.h"
         *
         * \brief A unsigned integer format option in the Data
         * Transport system.
         *
         * \c UIntOption defines a unsigned int option in the Data
         * Transport system.
         */
        class UIntOption : public DatastreamOption
        {
        public:
                /*!
                 * Constructor
                 */
                UIntOption (const std::string& name,
                            const std::string& label,
                            const std::string& units = "",
                            const bool req = false,
                            const unsigned int maxValue = 0);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~UIntOption();

                /*!
                 * \brief Accept a visit from a DatastreamOptionVisitor.
                 */
                void accept (const DatastreamOptionVisitor& visitor);

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{

                /*!
                 * \brief Get the value of this UIntOption as a string.
                 * @return The value of this->uintValue, converted to a string value.
                 */
                std::string getValue (void) const;

                /*!
                 * \brief Set the value of this UIntOption.
                 * @param s The new value as a string.
                 */
                void setValue (const std::string& s);

                /*!
                 * \brief Set the unsigned int value of this UIntOption.
                 * @param i The new value of this->uIntValue.
                 */
                void setValue (const unsigned int i);

                /*!
                 * \brief Get the value of this UIntOption as an unsigned int.
                 * @return The value of this->uIntValue.
                 */
                unsigned int getUIntValue (void) const;

                /*!
                 * \brief Get the maximum value of this UIntOption.
                 * @return The value of this->maxValue.
                 */
                unsigned int getMaxValue (void) const;

                /*!
                 * \brief Set the maximum value of this UIntOption.
                 * @param i The new value of this->maxValue.
                 */
                void setMaxValue (const unsigned int i);

                //@}

        private:

                /*! \brief The maximum permitted value of this option. */
                unsigned int maxValue;

                /*! \brief The unsigned int value of this option. */
                unsigned int uIntValue;

        };

        /*!
         * \headerfile DatastreamOption.h "DatastreamOption.h"
         *
         * \brief A composite option group in the Data Transport
         * system.
         */
        class CompositeOption : public DatastreamOption
        {
        public:
                /*!
                 * Constructor
                 */
                CompositeOption (const std::string& name,
                                 const std::string& label);

                /*!
                 * Destructor
                 * \note Declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~CompositeOption();

                /*!
                 * \brief Populate this CompositeOption from the
                 * stored Datastream settings.
                 */
                keyValList populate (const Datastream& ds,
                                     const std::string& program,
                                     const std::string& feature = "");

                /*!
                 * \brief Accept a visit from a DatastreamOptionVisitor.
                 */
                void accept (const DatastreamOptionVisitor& visitor);

                /*!
                 * \name Composite operations
                 */
                //@{

                /*!
                 * \brief Add an option.
                 */
                void add (std::shared_ptr<DatastreamOption> option);

                //@}

                /*!
                 * \name Private attribute accessor methods
                 */
                //@{


                //@}

        private:

                /*! \brief The options in this option group */
                std::list<std::shared_ptr<DatastreamOption> > options;
        };


        /*!
         * \headerfile DatastreamOption.h "DatastreamOption.h"
         *
         * \brief Interface for visiting a DatastreamOption in the
         * Data Transport system.
         */
        class DatastreamOptionVisitor
        {
        public:

                /*! Destructor declared virtual to ensure proper
                 *  cleanup in derived classes. */
                virtual ~DatastreamOptionVisitor();

                virtual void visit (TextOption&) const = 0;
                virtual void visit (ListOption&) const = 0;
                virtual void visit (BoolOption&) const = 0;
                virtual void visit (UIntOption&) const = 0;
                virtual void visit (CompositeOption&) const = 0;

        private:

        };

} // dt namespace

#endif // _DATASTREAMOPTION_H_
