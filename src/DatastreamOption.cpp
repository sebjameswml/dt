#ifdef __GNUG__
#  pragma implementation
#endif

#include <sstream>

#include "DatastreamOption.h"
#include "Datastream.h"

using namespace std;
using namespace dt;

/*!
 * DatastreamOption
 */
//@{
DatastreamOption::DatastreamOption (const string& name,
                                    const string& label,
                                    const string& units,
                                    const bool req)
        : type (DatastreamOption::TEXT)
        , name (name)
        , label (label)
        , unitsLabel (units)
        , required (req)
{
}

DatastreamOption::~DatastreamOption()
{
}

//
// Methods
//

DatastreamOption::keyValList
DatastreamOption::populate (const Datastream& ds,
                            const string& program,
                            const string& feature)
{
        // Get stored value for the specified filter and feature:
        string val (ds.getOption (program, feature, this->getName()));
        if (val != "Not found") {
                this->setValue (val);
        }
        DatastreamOption::keyValList l = { { this->getName(), this->getValue() } };
        return l;
}

//
// Accessors
//

DatastreamOption::TYPE
DatastreamOption::getType (void) const
{
        return this->type;
}

string
DatastreamOption::getName (void) const
{
        return this->name;
}

void
DatastreamOption::setName (const string& s)
{
        this->name = s;
}

string
DatastreamOption::getValue (void) const
{
        return this->value;
}
void
DatastreamOption::setValue (const string& s)
{
        this->value = s;
}

string
DatastreamOption::getLabel (void) const
{
        return this->label;
}

void
DatastreamOption::setLabel (const string& s)
{
        this->label = s;
}

string
DatastreamOption::getUnitsLabel (void) const
{
        return this->unitsLabel;
}

void
DatastreamOption::setUnitsLabel (const string& s)
{
        this->unitsLabel = s;
}

bool
DatastreamOption::getRequired (void) const
{
        return this->required;
}

void
DatastreamOption::setRequired (const bool b)
{
        this->required = b;
}

//@}

/*!
 * TextOption
 */
//@{

TextOption::TextOption (const string& name,
                        const string& label,
                        const string& units,
                        const bool req)
        : DatastreamOption (name, label, units, req)
        , maxLength (0)
{
        this->type = DatastreamOption::TEXT;
}

TextOption::~TextOption()
{
}

//
// Methods
//

void
TextOption::accept (const DatastreamOptionVisitor& visitor)
{
        visitor.visit (*this);
}

//
// Accessors
//
int
TextOption::getMaxLength (void) const
{
        return this->maxLength;
}

void
TextOption::setMaxLength (const int i)
{
        this->maxLength = i;
}

//@}

/*!
 * ListOption
 */
//@{

ListOption::ListOption (const string& name,
                        const string& label,
                        function<optList()> f)
        : DatastreamOption (name, label, "", false)
{
        this->type = DatastreamOption::OPTION_LIST;
        this->setOptionListBuilder (f);
}

ListOption::~ListOption()
{
}

//
// Methods
//

void
ListOption::accept (const DatastreamOptionVisitor& visitor)
{
        visitor.visit (*this);
}

//
// Accessors
//

void
ListOption::setOptionListBuilder (function<optList()> f)
{
        this->optionListBuilder = f;
}

ListOption::optList
ListOption::getOptionList (void) const
{
        optList l;
        try {
                l = this->optionListBuilder();
        } catch (const std::bad_function_call& e) {
                // No builder assigned
        }
        return l;
}

//@}

/*!
 * BoolOption
 */
//@{

BoolOption::BoolOption (const string& name,
                        const string& label,
                        const string& units,
                        const bool req)
        : DatastreamOption (name, label, units, req)
        , boolValue (false)
{
        this->type = DatastreamOption::BOOLEAN;
}

BoolOption::~BoolOption()
{
}

//
// Methods
//

void
BoolOption::accept (const DatastreamOptionVisitor& visitor)
{
        visitor.visit (*this);
}

//
// Accessors
//

string
BoolOption::getValue (void) const
{
        return (this->boolValue ? "true" : "false");
}

void
BoolOption::setValue (const string& s)
{
        if (s == "true" || s == "yes" || s == "on") {
                this->boolValue = true;
        } else {
                this->boolValue = false;
        }
}

void
BoolOption::setValue (const char * s)
{
        // Apply explicit conversion to string parameter in order to
        // select correct version of BoolOption::setValue().
        this->setValue (string(s));
}

void
BoolOption::setValue (const bool b)
{
        this->boolValue = b;
}

bool
BoolOption::getBoolValue (void) const
{
        return this->boolValue;
}

//@}

/*!
 * UIntOption
 */
//@{

UIntOption::UIntOption (const string& name,
                        const string& label,
                        const string& units,
                        const bool req,
                        const unsigned int maxVal)
        : DatastreamOption (name, label, units, req)
        , maxValue (maxVal)
        , uIntValue (0)
{
        this->type = DatastreamOption::UINT;
}

UIntOption::~UIntOption()
{
}

//
// Methods
//

void
UIntOption::accept (const DatastreamOptionVisitor& visitor)
{
        visitor.visit (*this);
}

//
// Accessors
//

string
UIntOption::getValue (void) const
{
        stringstream ss;
        ss << this->uIntValue;
        return ss.str();
}

void
UIntOption::setValue (const string& s)
{
        unsigned int i (0);
        stringstream ss;
        ss << s;
        ss >> i;
        this->setValue (i);
}

void
UIntOption::setValue (const unsigned int i)
{
        if (this->maxValue && i > maxValue) {
                this->uIntValue = maxValue;
        } else {
                this->uIntValue = i;
        }
}

unsigned int
UIntOption::getUIntValue (void) const
{
        return this->uIntValue;
}

unsigned int
UIntOption::getMaxValue (void) const
{
        return this->maxValue;
}

void
UIntOption::setMaxValue (const unsigned int i)
{
        this->maxValue = i;
}

//@}

/*!
 * CompositeOption
 */
//@{
CompositeOption::CompositeOption (const string& name,
                                  const string& label)
        : DatastreamOption (name, label)
{
        this->type = DatastreamOption::COMPOSITE;
}

CompositeOption::~CompositeOption()
{
}

//
// Methods
//

void
CompositeOption::accept (const DatastreamOptionVisitor& visitor)
{
        visitor.visit (*this);
        for(auto i : this->options) {
                i->accept (visitor);
        }
}

DatastreamOption::keyValList
CompositeOption::populate (const Datastream& ds,
                           const string& program,
                           const string& feature)
{
        DatastreamOption::keyValList l, li;
        for (auto& i : this->options) {
                li = i->populate (ds, program, feature);
                l.splice (l.end(), li);
        }
        return l;
}

void
CompositeOption::add (shared_ptr<DatastreamOption> option)
{
        this->options.push_back (option);
}

//@}

/*!
 * DatastreamOptionVisitor
 */
//@{
DatastreamOptionVisitor::~DatastreamOptionVisitor()
{
}
//@}
