#ifdef __GNUG__
#  pragma implementation
#endif

#include "config.h"

#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <utility>
#include <stdlib.h> // for int abs(int)

#include "Attr.h"

using namespace std;
using namespace dt;

// Constant initialization of map. This map forms a lookup table
// linking IPP attributes to the format of those attributes; whether
// they're job/op/printer attributes, what kind of value they have,
// and whether they have multiple values.
const std::map<std::string, ipp::AttributeOptionTags> ipp::attributeOptionsTable = {
        { "attributes-charset", { ipp::op_attr, ipp::charset_val, ipp::singlevalued } },
        { "attributes-natural-language", { ipp::op_attr, ipp::language_val, ipp::singlevalued } },
        { "auth-info", { ipp::job_attr, ipp::text_val, ipp::multivalued } },
        { "auth-info-required", { ipp::printer_attr, ipp::keyword_val, ipp::multivalued } },
        { "blackplot", { ipp::job_attr, ipp::bool_val, ipp::singlevalued } },
        { "blackplot-default", { ipp::printer_attr, ipp::bool_val, ipp::singlevalued } },
        { "brightness", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "brightness-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "columns", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "columns-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "compression", { ipp::op_attr, ipp::keyword_val, ipp::singlevalued } },
        { "copies", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "copies-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        // copies-supported from RFC 3380 - IPP: Job and Printer Set Operations (which is an
        // optional extension to IPP 1.0 and IPP 1.1.
        { "copies-supported", { ipp::printer_attr, ipp::range_val, ipp::singlevalued } },
        { "device-uri", { ipp::printer_attr, ipp::uri_val, ipp::singlevalued } },
        { "document-format", { ipp::op_attr, ipp::mimetype_val, ipp::singlevalued } },
        { "document-format-default", { ipp::printer_attr, ipp::mimetype_val, ipp::singlevalued } },
        { "document-format-supported", { ipp::printer_attr, ipp::mimetype_val, ipp::multivalued } },
        { "exclude-schemes", { ipp::op_attr, ipp::name_val, ipp::singlevalued } },
        { "finishings", { ipp::job_attr, ipp::enum_val, ipp::multivalued } },
        { "finishings-default", { ipp::printer_attr, ipp::enum_val, ipp::multivalued } },
        { "fit-to-page", { ipp::job_attr, ipp::bool_val, ipp::singlevalued } },
        { "fit-to-page-default", { ipp::printer_attr, ipp::bool_val, ipp::singlevalued } },
        { "fitplot", { ipp::job_attr, ipp::bool_val, ipp::singlevalued } },
        { "fitplot-default", { ipp::printer_attr, ipp::bool_val, ipp::singlevalued } },
        { "gamma", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "gamma-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "hue", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "hue-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "include-schemes", { ipp::op_attr, ipp::name_val, ipp::multivalued } },
        { "job-hold-until", { ipp::job_attr, ipp::name_val, ipp::singlevalued } },
        { "job-impressions", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "job-impressions-completed", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "job-k-limit", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "job-name", { ipp::printer_attr, ipp::name_val, ipp::singlevalued } },
        { "job-page-limit", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "job-priority", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "job-quota-period", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "job-sheets", { ipp::job_attr, ipp::name_val, ipp::multivalued } },
        { "job-sheets-default", { ipp::printer_attr, ipp::name_val, ipp::multivalued } },
        { "job-uuid", { ipp::job_attr, ipp::uri_val, ipp::singlevalued } },
        { "job-uri", { ipp::job_attr, ipp::uri_val, ipp::singlevalued } },
        { "job-id", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "job-media-sheets-completed", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "job-state", { ipp::job_attr, ipp::enum_val, ipp::singlevalued } },
        { "job-state-message", { ipp::job_attr, ipp::text_val, ipp::singlevalued } },
        { "job-state-reasons", { ipp::job_attr, ipp::keyword_val, ipp::multivalued } },
        { "landscape", { ipp::job_attr, ipp::bool_val, ipp::singlevalued } },
        { "marker-change-time", { ipp::printer_attr, ipp::int_val, ipp::multivalued } },
        { "marker-colors", { ipp::printer_attr, ipp::name_val, ipp::multivalued } },
        { "marker-high-levels", { ipp::printer_attr, ipp::int_val, ipp::multivalued } },
        { "marker-levels", { ipp::printer_attr, ipp::int_val, ipp::multivalued } },
        { "marker-low-levels", { ipp::printer_attr, ipp::int_val, ipp::multivalued } },
        { "marker-message", { ipp::printer_attr, ipp::text_val, ipp::singlevalued } },
        { "marker-names", { ipp::printer_attr, ipp::name_val, ipp::multivalued } },
        { "marker-types", { ipp::printer_attr, ipp::keyword_val, ipp::multivalued } },
        { "media", { ipp::job_attr, ipp::keyword_val, ipp::multivalued } },
        { "media-col", { ipp::job_attr, ipp::begin_collection_val, ipp::singlevalued } },
        { "media-col-default", { ipp::printer_attr, ipp::begin_collection_val, ipp::singlevalued } },
        { "media-color", { ipp::job_attr, ipp::keyword_val, ipp::singlevalued } },
        { "media-default", { ipp::printer_attr, ipp::keyword_val, ipp::multivalued } },
        { "media-key", { ipp::job_attr, ipp::keyword_val, ipp::singlevalued } },
        { "media-size", { ipp::job_attr, ipp::begin_collection_val, ipp::singlevalued } },
        { "media-type", { ipp::job_attr, ipp::keyword_val, ipp::singlevalued } },
        { "member-uris", { ipp::printer_attr, ipp::uri_val, ipp::singlevalued } },
        { "mirror", { ipp::job_attr, ipp::bool_val, ipp::singlevalued } },
        { "mirror-default", { ipp::printer_attr, ipp::bool_val, ipp::singlevalued } },
        { "natural-scaling", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "natural-scaling-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "notify-charset", { ipp::subscription_attr, ipp::charset_val, ipp::singlevalued } },
        { "notify-events", { ipp::subscription_attr, ipp::keyword_val, ipp::multivalued } },
        { "notify-events-default", { ipp::printer_attr, ipp::keyword_val, ipp::multivalued } },
        { "notify-lease-duration", { ipp::subscription_attr, ipp::int_val, ipp::singlevalued } },
        { "notify-lease-duration-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "notify-natural-language", { ipp::subscription_attr, ipp::language_val, ipp::singlevalued } },
        { "notify-pull-method", { ipp::subscription_attr, ipp::keyword_val, ipp::singlevalued } },
        { "notify-recipient-uri", { ipp::subscription_attr, ipp::uri_val, ipp::singlevalued } },
        { "notify-time-interval", { ipp::subscription_attr, ipp::int_val, ipp::singlevalued } },
        { "notify-user-data", { ipp::subscription_attr, ipp::string_val, ipp::singlevalued } },
        { "number-up", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "number-up", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "orientation-requested", { ipp::job_attr, ipp::enum_val, ipp::singlevalued } },
        { "orientation-requested-default", { ipp::printer_attr, ipp::enum_val, ipp::singlevalued } },
        { "page-bottom", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "page-bottom-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "page-left", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "page-left", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "page-ranges", { ipp::job_attr, ipp::range_val, ipp::multivalued } },
        { "page-ranges-default", { ipp::printer_attr, ipp::range_val, ipp::multivalued } },
        { "page-right", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "page-right-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "page-top", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "page-top-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "penwidth", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "penwidth-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "port-monitor", { ipp::printer_attr, ipp::name_val, ipp::singlevalued } },
        { "ppd-name", { ipp::printer_attr, ipp::name_val, ipp::singlevalued } },
        { "ppi", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "ppi-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "prettyprint", { ipp::job_attr, ipp::bool_val, ipp::singlevalued } },
        { "prettyprint-default", { ipp::printer_attr, ipp::bool_val, ipp::singlevalued } },
        { "print-quality", { ipp::job_attr, ipp::enum_val, ipp::singlevalued } },
        { "print-quality-default", { ipp::printer_attr, ipp::enum_val, ipp::singlevalued } },
        { "printer-commands", { ipp::printer_attr, ipp::keyword_val, ipp::multivalued } },
        { "printer-error-policy", { ipp::printer_attr, ipp::name_val, ipp::singlevalued } },
        { "printer-info", { ipp::printer_attr, ipp::text_val, ipp::singlevalued } },
        { "printer-is-accepting-jobs", { ipp::printer_attr, ipp::bool_val, ipp::singlevalued } },
        { "printer-is-shared", { ipp::printer_attr, ipp::bool_val, ipp::singlevalued } },
        { "printer-location", { ipp::printer_attr, ipp::text_val, ipp::singlevalued } },
        { "printer-make-and-model", { ipp::printer_attr, ipp::text_val, ipp::singlevalued } },
        { "printer-more-info", { ipp::printer_attr, ipp::uri_val, ipp::singlevalued } },
        { "printer-name", { ipp::printer_attr, ipp::name_val, ipp::singlevalued } },
        { "printer-op-policy", { ipp::printer_attr, ipp::name_val, ipp::singlevalued } },
        { "printer-resolution", { ipp::job_attr, ipp::resolution_val, ipp::singlevalued } },
        { "printer-state", { ipp::printer_attr, ipp::enum_val, ipp::singlevalued } },
        { "printer-state-message", { ipp::printer_attr, ipp::text_val, ipp::singlevalued } },
        { "printer-state-change-time", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "printer-state-time", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "printer-state-reasons", { ipp::printer_attr, ipp::keyword_val, ipp::multivalued } },
        { "printer-type", { ipp::printer_attr, ipp::enum_val, ipp::singlevalued } },
        { "printer-uri", { ipp::op_attr, ipp::uri_val, ipp::singlevalued } },
        { "printer-uri-supported", { ipp::printer_attr, ipp::uri_val, ipp::singlevalued } },
        { "queued-job-count", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "raw", { ipp::op_attr, ipp::mimetype_val, ipp::singlevalued } },
        // For this attribute, CUPS code says this is name_val, but IPP server sets keyword_val.
        //{ "requested-attributes", { ipp::op_attr, ipp::name_val, ipp::multivalued } },
        { "requested-attributes", { ipp::op_attr, ipp::keyword_val, ipp::multivalued } },
        { "requesting-user-name", { ipp::printer_attr, ipp::name_val, ipp::singlevalued } },
        { "requesting-user-name-allowed", { ipp::printer_attr, ipp::name_val, ipp::multivalued } },
        { "requesting-user-name-denied", { ipp::printer_attr, ipp::name_val, ipp::multivalued } },
        { "resolution", { ipp::job_attr, ipp::resolution_val, ipp::singlevalued } },
        { "resolution-default", { ipp::printer_attr, ipp::resolution_val, ipp::singlevalued } },
        { "saturation", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "saturation-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "scaling", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "scaling-default", { ipp::printer_attr, ipp::int_val, ipp::singlevalued } },
        { "sides", { ipp::job_attr, ipp::keyword_val, ipp::singlevalued } },
        { "sides-default", { ipp::printer_attr, ipp::keyword_val, ipp::singlevalued } },
        { "status-message", { ipp::op_attr, ipp::text_val, ipp::singlevalued } },
        { "wrap", { ipp::job_attr, ipp::bool_val, ipp::singlevalued } },
        { "wrap-default", { ipp::printer_attr, ipp::bool_val, ipp::singlevalued } },
        { "x-dimension", { ipp::job_attr, ipp::int_val, ipp::singlevalued } },
        { "y-dimension", { ipp::job_attr, ipp::int_val, ipp::singlevalued } }
}; // ipp::attributeOptionsTable

dt::ipp::Attr::Attr () :
        name(""),
        valueType (ipp::unknown_val),
        group (ipp::unknown_val),
        multivalue (ipp::singlevalued)
{
        this->stringValues.reserve(16);
        this->intValues.reserve(16);
}

dt::ipp::Attr::Attr (const string& attributeName) :
        name(attributeName),
        valueType (ipp::unknown_val),
        group (ipp::unknown_val),
        multivalue (ipp::singlevalued)
{
        // We have an attribute name to work with, so get the type.
        this->determineType();

        // Capacity starts out at zero.
        //cerr << "stringValues capacity: " << this->stringValues.capacity() << " intValues: " << this->intValues.capacity() << endl;
        this->stringValues.reserve(16);
        this->intValues.reserve(16);
}

dt::ipp::Attr::~Attr ()
{
}

void
dt::ipp::Attr::reset (void)
{
        this->name = "";
        this->valueType = ipp::unknown_val;
        this->group = ipp::unknown_val;
        this->multivalue = ipp::singlevalued;
        this->zero();
}

void
dt::ipp::Attr::reset (const string& attributeName)
{
        this->name = "";
        this->valueType = ipp::unknown_val;
        this->group = ipp::unknown_val;
        this->multivalue = ipp::singlevalued;
        this->zero();

        this->setName (attributeName);
}

void
dt::ipp::Attr::zero (void)
{
        // Make sure integers are cleared
        this->intValues.clear();
        // As well as strings
        this->stringValues.clear();
}

string
dt::ipp::Attr::getName (void) const
{
        return this->name;
}

void
dt::ipp::Attr::setName (const string& s)
{
        this->name = s;
        this->determineType();
}

ipp::tag
dt::ipp::Attr::getValueType (void) const
{
        return this->valueType;
}

ipp::tag
dt::ipp::Attr::getGroup (void) const
{
        return this->group;
}

string
dt::ipp::Attr::getValueDebugString (void) const
{
        stringstream ss;

        switch (this->valueType) {
        case ipp::int_val:
        case ipp::bool_val:
        case ipp::enum_val:
        {
                auto i = this->intValues.cbegin();
                while (i != this->intValues.cend()) {
                        ss << *i << endl;
                        ++i;
                }
                break;
        }
        case ipp::string_val:
        case ipp::date_val:
        case ipp::resolution_val:
        case ipp::range_val:
        case ipp::begin_collection_val:
        case ipp::textwithlang_val:
        case ipp::namewithlang_val:
        case ipp::end_collection_val:
        case ipp::text_val:
        case ipp::name_val:
        case ipp::keyword_val:
        case ipp::uri_val:
        case ipp::language_val:
        case ipp::charset_val:
        case ipp::mimetype_val:
        {
                auto i = this->stringValues.cbegin();
                while (i != this->stringValues.cend()) {
                        ss << *i << endl;
                        ++i;
                }
                break;
        }
        default:
                // Unknown type, don't set s.
                cerr << "Unknown type '" << this->valueType << "', for attr '" << this->name << "' can't get string\n";
                break;
        }
        return ss.str();
}

bool
dt::ipp::Attr::valueTypeIsString (void) const
{
        bool typeIsString = false;
        switch (this->valueType) {
        case ipp::int_val:
        case ipp::bool_val:
        case ipp::enum_val:
        case ipp::range_val: // two 4-octet signed integers
        case ipp::date_val:  // 11 bytes of RFC1903 date
        case ipp::resolution_val: // OCTET-STRING consisting of nine
                                  // octets of 2 SIGNED-INTEGERs
                                  // followed by a SIGNED-BYTE. The
                                  // first SIGNED-INTEGER contains the
                                  // value of cross feed direction
                                  // resolution. The second
                                  // SIGNED-INTEGER contains the value
                                  // of feed direction resolution. The
                                  // SIGNED-BYTE contains the units
                typeIsString = false;
                break;
        case ipp::string_val:
        case ipp::begin_collection_val:
        case ipp::textwithlang_val:
        case ipp::namewithlang_val:
        case ipp::end_collection_val:
        case ipp::text_val:
        case ipp::name_val:
        case ipp::keyword_val:
        case ipp::uri_val:
        case ipp::language_val:
        case ipp::charset_val:
        case ipp::mimetype_val:
                typeIsString = true;
                break;
        case ipp::unknown_val:
        case ipp::tag_unknown:
        default:
                stringstream ss;
                ss << "Attr::valueTypeIsString: unknown type " << this->valueType << " for " << this->name;
                throw runtime_error (ss.str());
                break;
        }
        return typeIsString;
}

int
dt::ipp::Attr::getInt (void) const
{
        return this->intValues.back();
}

void
dt::ipp::Attr::setValue (const string& s)
{
        // Set the zeroth member of stringValues.
        if (this->stringValues.empty()) {
                this->stringValues.push_back (s);
        } else {
                this->stringValues[0] = s;
        }
}

void
dt::ipp::Attr::setValue (const int i)
{
        if (this->intValues.empty()) {
                this->intValues.push_back (i);
        } else {
                this->intValues[0] = i;
        }
}

void
dt::ipp::Attr::addValue (const string& s)
{
        this->stringValues.push_back (s);
}

void
dt::ipp::Attr::addValue (const int i)
{
        this->intValues.push_back (i);
}

void
dt::ipp::Attr::popValue (void)
{
        if (this->valueTypeIsString() == true) {
                this->stringValues.pop_back();
        } else {
                this->intValues.pop_back();
        }
}

void
dt::ipp::Attr::determineType (void)
{
        // Can throw exception:
        try {
                const AttributeOptionTags& t = ipp::attributeOptionsTable.at (this->name);
                this->multivalue = t.multivalue;
                this->valueType = t.valueTypeTag;
                this->group = t.groupTag;
        } catch (std::out_of_range& e) {
                this->valueType = ipp::unknown_val;
                this->group = ipp::unknown_val;
                this->multivalue = ipp::singlevalued;
        }
}

std::string
dt::ipp::Attr::tagName (const ipp::tag& t)
{
        string s("");
        switch (t) {
        case op_attr:
                s = "operation attributes group";
                break;
        case job_attr:
                s = "job attributes group";
                break;
        case end_attrs:
                s = "end of attributes";
                break;
        case printer_attr:
                s = "printer attributes group";
                break;
        case unsupported_attr:
                s = "unsupported attributes group";
                break;
        case subscription_attr:
                s = "subscription attributes group";
                break;
        case event_notif_attr:
                s = "event notification attributes group";
                break;
        case resource_attr:
                s = "resource attributes group";
                break;
        case document_attr:
                s = "document attributes group";
                break;

                // Value type tags below here (RFC 2910 3.5.2)
        case unsupported_val:
                s = "unsupported value";
                break;
        case default_val:
                s = "default value";
                break;
        case unknown_val:
                s = "unknown value";
                break;
        case no_val:
                s = "no-value";
                break;
        case not_settable_val:
                s = "not-settable value";
                break;
        case delete_attr_val:
                s = "delete-attribute value";
                break;
        case admindefined_val:
                s = "admin-defined value";
                break;
        case int_val:
                s = "integer value";
                break;
        case bool_val:
                s = "boolean value";
                break;
        case enum_val:
                s = "enumerated value";
                break;
        case string_val:
                s = "octet string";
                break;
        case date_val:
                s = "dateTime value";
                break;
        case resolution_val:
                s = "resolution value";
                break;
        case range_val:
                s = "rangeOfInteger value";
                break;
        case begin_collection_val:
                s = "beginning of collection value";
                break;
        case textwithlang_val:
                s = "textWithLanguage value";
                break;
        case namewithlang_val:
                s = "nameWithLanguage value";
                break;
        case end_collection_val:
                s = "end of collection value";
                break;
        case text_val:
                s = "text(WithoutLanguage) value";
                break;
        case name_val:
                s = "name(WithoutLanguage) value";
                break;
        case reserved_str1:
                s = "reserved for future string value";
                break;
        case keyword_val:
                s = "keyword value";
                break;
        case uri_val:
                s = "uri value";
                break;
        case uri_scheme_val:
                s = "uriScheme value";
                break;
        case charset_val:
                s = "charset value";
                break;
        case language_val:
                s = "naturalLanguage value";
                break;
        case mimetype_val:
                s = "mimeMediaType value";
                break;
        case membername_val:
                s = "collection member name value";
                break;
        case tag_unknown:
        default:
                s = "unknown tag";
                break;
        }

        return s;
}

std::string
dt::ipp::Attr::debugOutput (void) const
{
        stringstream ss;
        ss << "Attribute: " << this->getName()
           << " (" << Attr::tagName(valueType) << ", "
           << Attr::tagName(this->group) << ")" << endl;
        if (this->multivalue == ipp::multivalued) {
                ss << "  Values:\n" << this->getValueDebugString();
        } else {
                ss << "  Value: " << this->getValueDebugString() << endl;
        }
        return ss.str();
}

string::size_type
dt::ipp::Attr::getNameLength (void) const
{
        string::size_type sz = this->name.size();
        if (sz > 255) {
                throw runtime_error ("name is too long");
        }
        return sz;
}

string::size_type
dt::ipp::Attr::getValueLength (const std::vector<std::string>::const_iterator& valiter) const
{
        string::size_type sz = valiter->size();
        if (sz > 255) {
                throw runtime_error ("value is too long");
        }
        return sz;
}

signed char
dt::ipp::Attr::getValueLength (const std::vector<int>::const_iterator& valiter) const
{
        signed char len = 1;
        if (abs(*valiter) <= 0xff) {
                // No further action
        } else if (abs(*valiter) <= 0xffff) {
                len = 2;
        } else if (abs(*valiter) <= 0xffffff) {
                len = 3;
        } else {
                len = 4;
        }
        return len;
}

string
dt::ipp::Attr::getValue (const std::vector<std::string>::const_iterator& valiter) const
{
        return *valiter;
}

int
dt::ipp::Attr::getValue (const std::vector<int>::const_iterator& valiter) const
{
        return *valiter;
}

std::string
dt::ipp::Attr::encode (void) const
{
        stringstream ss;

        ss.put(static_cast<char>(this->valueType));
        string::size_type nlen = this->getNameLength();
        ss.put(static_cast<char>((nlen>>8) & 0xff));
        ss.put(static_cast<char>(nlen & 0xff));
        ss << this->name;

        // Do additional values one by one.
        if (this->valueTypeIsString() == true) {

#ifdef VALUETYPE_MATTERS
                switch (this->valueType) {
                case ipp::string_val:
                case ipp::date_val:
                case ipp::resolution_val:
                case ipp::begin_collection_val:
                case ipp::textwithlang_val:
                case ipp::namewithlang_val:
                case ipp::end_collection_val:
                case ipp::text_val:
                case ipp::name_val:
                case ipp::keyword_val:
                case ipp::uri_val:
                case ipp::charset_val:
                case ipp::language_val:
                case ipp::mimetype_val:
                        // Anything special to do?
                        break;
                default:
                        break;
                }
#endif
                auto i = this->stringValues.cbegin();
                bool first = true;
                while (i != this->stringValues.cend()) {
                        if (!first) { // additional value have 0x0000 for name length
                                ss << static_cast<char>(this->valueType);
                                ss << static_cast<char>(0) << static_cast<char>(0);
                        } else {
                                first = false;
                        }
                        string::size_type vlen = this->getValueLength (i);
                        ss.put ((vlen>>8) & 0xff);
                        ss.put (vlen & 0xff);
                        ss << this->getValue (i);
                        ++i;
                }

        } else {

                if (this->valueType == ipp::range_val) {
                        // a range_val is two integers, so we store
                        // these as pairs in our intValues vector.
                        auto i = this->intValues.cbegin();
                        bool first = true;
                        while (i != this->intValues.cend()) {
                                if (!first) { // additional value has 0x0000 for name length
                                        ss << static_cast<char>(this->valueType);
                                        ss << static_cast<char>(0) << static_cast<char>(0);
                                } else {
                                        first = false;
                                }

                                // Always 8 octets for a range_val:
                                ss.put (0); ss.put (8);

                                // 4 octets of the lower bound value
                                int val = this->getValue(i);
                                ss.put (static_cast<char>(val>>24 & 0xff));
                                ss.put (static_cast<char>(val>>16 & 0xff));
                                ss.put (static_cast<char>(val>>8 & 0xff));
                                ss.put (static_cast<char>(val & 0xff));

                                // Now 4 octets of the upper bound value:
                                ++i;
                                if (i == this->intValues.cend()) {
                                        // error - require pairs of ints.
                                        ss.put (0);
                                        ss.put (0);
                                        ss.put (0);
                                        ss.put (0);
                                        continue;
                                }
                                val = this->getValue(i);
                                ss.put (static_cast<char>(val>>24 & 0xff));
                                ss.put (static_cast<char>(val>>16 & 0xff));
                                ss.put (static_cast<char>(val>>8 & 0xff));
                                ss.put (static_cast<char>(val & 0xff));
                                ++i;
                        }

                } else if (this->valueType == ipp::date_val) {
                        throw runtime_error ("implement date_val");
                } else if (this->valueType == ipp::resolution_val) {
                        throw runtime_error ("implement resolution_val");

                } else { // int_val or enum_val

                        auto i = this->intValues.cbegin();
                        bool first = true;
                        while (i != this->intValues.cend()) {

                                // Name length (if required).
                                if (!first) { // additional value have 0x0000 for name length
                                        ss << static_cast<char>(this->valueType);
                                        ss << static_cast<char>(0) << static_cast<char>(0);
                                } else {
                                        first = false;
                                }

                                // Value length.
                                // Enumerated values and int values have to be 4 in size.
                                signed char vlen = 4;
                                if (this->valueType == ipp::bool_val) {
                                        // Bools have to be 1 byte in size.
                                        vlen = 1;
                                }
                                //cerr << "vlen is " << static_cast<int>(vlen) << endl;
                                ss << static_cast<char>(0); // First 0 of the value length.
                                ss.put (vlen);

                                // Value.
                                int val = this->getValue(i);
                                cerr << "value is " << val << endl;
                                switch (vlen) {
                                case 1:
                                        ss.put (static_cast<char>(val & 0xff));
                                case 4:
                                default:
                                        ss.put (static_cast<char>(val>>24 & 0xff));
                                        ss.put (static_cast<char>(val>>16 & 0xff));
                                        ss.put (static_cast<char>(val>>8 & 0xff));
                                        ss.put (static_cast<char>(val & 0xff));
                                        break;
                                }

                                ++i;
                        }
                }
        }

        return ss.str();
}
