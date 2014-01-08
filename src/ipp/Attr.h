/* -*-c++-*- */

#ifndef _IPP_ATTR_H_
#define _IPP_ATTR_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <utility>
#include <string>
#include <map>
#include <vector>

namespace dt {
        namespace ipp {

                enum tag {
                        tag_unknown = 0x0,
                        // Group/Delimiter tags (RFC 2910 3.5.1)
                        op_attr = 0x1,       // operation attributes group
                        job_attr,            // job attributes group
                        end_attrs,           // end of attributes
                        printer_attr,        // printer attributes group
                        unsupported_attr,    // unsupported attributes group
                        subscription_attr,   // subscription attributes group
                        event_notif_attr,    // event notification attributes group
                        resource_attr,       // resource attributes group
                        document_attr,       // document attributes group

                        // Value type tags below here (RFC 2910 3.5.2)
                        unsupported_val = 0x10,  // unsupported value
                        default_val,             // default value
                        unknown_val,             // unknown value
                        no_val,                  // no-value
                        not_settable_val = 0x15, // not-settable value
                        delete_attr_val,         // delete-attribute value
                        admindefined_val,        // admin-defined value
                        int_val = 0x21,          // integer
                        bool_val,                // boolean
                        enum_val,                // enum
                        string_val = 0x30,       // octetString
                        date_val,                // dateTime value
                        resolution_val,          // resolution value
                        range_val,               // rangeOfInteger value - two signed integers (4 octets each)
                        begin_collection_val,    // beginning of collection value
                        textwithlang_val,        // textWithLanguage value
                        namewithlang_val,        // nameWithLanguage value
                        end_collection_val,      // end of collection value
                        text_val = 0x41,         // text(WithoutLanguage) value
                        name_val,                // name(WithoutLangauge) value
                        reserved_str1,           // reserved for future string value (0x43)
                        keyword_val,             // keyword value
                        uri_val,                 // uri value
                        uri_scheme_val,          // uriScheme value
                        charset_val,             // charset value
                        language_val,            // naturalLanguage value
                        mimetype_val,            // mimeMediaType value
                        membername_val           // collection member name value
                };

                enum mval {
                        singlevalued = 0,
                        multivalued = 1
                };

                struct AttributeOptionTags
                {
                        ipp::tag groupTag;
                        ipp::tag valueTypeTag;
                        ipp::mval multivalue;
                };

                // Hold a map of attribute name to AttributeOptionTags (set up in Attr.cpp)
                extern const std::map<std::string, ipp::AttributeOptionTags> attributeOptionsTable;

                /*!
                 * An IPP Attribute. Has name, type, value.
                 *
                 * Contains an IPP attribute used in transferring print data
                 * over a network.
                 *
                 * The core types are int and string, and because
                 * there are just these two, I've not made this a
                 * templated class. If it turns out we need to
                 * consider other types, it may be worth templating
                 * this class.
                 */
                class Attr
                {
                public:
                        Attr ();
                        Attr (const std::string& attributeName);
                        ~Attr ();

                        /*!
                         * Reset the attribute - unset the name and zero any values.
                         */
                        void reset (void);

                        /*!
                         * Reset the attribute to be a new attribute
                         * with name attributeName.
                         */
                        void reset (const std::string& attributeName);

                        /*!
                         * Zero out any stored values.
                         */
                        void zero (void);

                        /*!
                         * Return the name of this attribute.
                         */
                        std::string getName (void) const;

                        /*!
                         * Set the attribute name to s, which will set
                         * its value type as well.
                         */
                        void setName (const std::string& s);

                        /*!
                         * Return the type tag of this attribute's value.
                         */
                        ipp::tag getValueType (void) const;

                        /*!
                         * Return the group tag of this attribute. This is
                         * sometimes called the group delimiter.
                         */
                        ipp::tag getGroup (void) const;

                        /*!
                         * Get the string value or values of this IPP attribute, for debugging.
                         */
                        std::string getValueDebugString (void) const;

                        //std::vector<std::string> getStrings (void) const;

                        /*!
                         * Get the integer value of this IPP attribute, if
                         * applicable.
                         */
                        int getInt (void) const;

                        /*!
                         * Set the stringValue to s (single valued types only)
                         */
                        void setValue (const std::string& s);

                        /*!
                         * Add a value to a multivalued attribute (can
                         * also be used to add the single value to a
                         * single valued attribute).
                         */
                        void addValue (const std::string& s);

                        /*!
                         * cf setValue (const string&)
                         */
                        void setValue (const int i);

                        /*!
                         * cf addValue (const string&)
                         */
                        void addValue (const int i);

                        /*!
                         * Pop value from a multivalued attribute.
                         */
                        void popValue (void);

                        /*!
                         * Return the name of the tag in argument t.
                         */
                        static std::string tagName (const ipp::tag& t);

                        /*!
                         * Output the name and values in a debug output format.
                         */
                        std::string debugOutput (void) const;

                        /*!
                         * Output the attribute in format suitable for
                         * inclusion in an IPP encoded message.
                         */
                        std::string encode (void) const;

                private:
                        std::string::size_type getNameLength (void) const;
                        //std::string::size_type getValueLength (const std::string& val) const;
                        std::string::size_type getValueLength (const std::vector<std::string>::const_iterator& valiter) const;
                        signed char getValueLength (const std::vector<int>::const_iterator& valiter) const;
                        std::string getValue (const std::vector<std::string>::const_iterator& valiter) const;
                        int getValue (const std::vector<int>::const_iterator& valiter) const;

                        /*!
                         * If this is a string value type held in
                         * stringValue or stringValues, then return true.
                         */
                        bool valueTypeIsString (void) const;

                        /*!
                         * Determine type from name.
                         */
                        void determineType (void);

                        /*!
                         * The attribute name, such as printer-name,
                         * printer-type, printer-info and so on.
                         */
                        std::string name;

                        /*!
                         * Next three class members could be struct
                         * AttrOptionTags.
                         */
                        //@{
                        /*!
                         * The attribute value type tag, which can be determined
                         * by comparing name with a lookup-table. Values for this
                         * found in IPP documentation and also in
                         * cups-1.x/cups/ipp.h
                         */
                        ipp::tag valueType;

                        /*!
                         * The attribute group, which can be
                         * determined by comparing name with a
                         * lookup-table. See ipp::tag and ipp::optionsTable.
                         */
                        ipp::tag group;

                        /*!
                         * If non zero, then this attribute may have multiple
                         * values.
                         */
                        ipp::mval multivalue;
                        //@}

                        /*!
                         * The string value of the attribute, if applicable. This
                         * is filled for IPP_TAG_TEXT, IPP_TAG_NAME etc types. If
                         * the type is IPP_TAG_INTEGER or IPP_TAG_ENUM, then the
                         * integer is formatted as a decimal and returned as a
                         * string.
                         *
                         * Vector version for multi valued attributes.
                         */
                        //@{
                        std::vector<std::string> stringValues;
                        //@}

                        /*!
                         * Integer value, if applicable. Filled for
                         * IPP_TAG_INTEGER or IPP_TAG_ENUM types (not a complete
                         * list). 0 returned if type is IPP_TAG_TEXT or some other
                         * string value.
                         *
                         * The vector version is for multi valued
                         * attributes.
                         */
                        //@{
                        std::vector<int> intValues;
                        //@}
                };

        } // namespace ipp
} // namespace dt

#endif // _IPP_ATTR_H_

