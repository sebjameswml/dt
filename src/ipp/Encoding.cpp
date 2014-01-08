#ifdef __GNUG__
# pragma implementation
#endif

#include "Encoding.h"
#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace std;
using namespace dt;

string
dt::ipp::Encoding::debugOutput (void) const
{
        stringstream ss;

        ss << "Version number: " << static_cast<unsigned int>(this->versionNumber[0])
           << "." << static_cast<unsigned int>(this->versionNumber[1]) << endl;
        ss << "Operation ID/Status code: " << this->opIdOrStatusCode << endl;
        ss << "Request ID: " << this->requestId << endl;

        vector<ipp::Attr>::const_iterator i = this->attributes.cbegin();

        while (i != this->attributes.cend()) {
                ss <<  i->debugOutput();
                ++i;
        }

        ss << "END IPP message details." << endl;
        return ss.str();
}

std::string
dt::ipp::Encoding::encode (void)
{
        this->rawMessage = "01234567";

        this->rawMessage[0] = this->versionNumber[0];
        this->rawMessage[1] = this->versionNumber[1];

        this->rawMessage[2] = static_cast < char > ((this->opIdOrStatusCode & 0xff00)   >> 8 );
        this->rawMessage[3] = static_cast<char>(this->opIdOrStatusCode & 0x00ff);

        this->rawMessage[4] = static_cast<char>((this->requestId & 0xff000000) >> 24);
        this->rawMessage[5] = static_cast<char>((this->requestId & 0x00ff0000) >> 16);
        this->rawMessage[6] = static_cast<char>((this->requestId & 0x0000ff00) >> 8);
        this->rawMessage[7] = static_cast<char>(this->requestId & 0x000000ff);

        // cerr << "this->rawMessage size: " << this->rawMessage.size() << endl;

        // rawMessage[8] is attribute group tag. What should that be?
        auto i = this->attributes.cbegin();

        if (i == this->attributes.cend()) {
                // There are no attributes to include.
        }

        this->currentGroupTag = i->getGroup();

        while (i != this->attributes.cend()) {

                if (i->getGroup() != this->currentGroupTag) {
                        this->currentGroupTag = i->getGroup();
                        this->rawMessage += static_cast<char>(this->currentGroupTag);
                } else {
                        this->currentGroupTag = i->getGroup();
                }

                this->rawMessage += i->encode();
                ++i;
        }

        this->rawMessage += static_cast<char>(ipp::end_attrs);

        return this->rawMessage;
}

void
dt::ipp::Encoding::parse (void)
{
        if (this->rawMessage.empty()) {
                throw runtime_error ("rawMessage is empty");
        }

        string::size_type rawMessageSize = this->rawMessage.size();
        if (rawMessageSize < ipp::MIN_MESSAGE_LENGTH) {
                throw runtime_error ("rawMessage is too short");
        }

        cerr << "rawMessage:'" << rawMessage << "'" << endl;

        this->versionNumber.clear();
        this->versionNumber.push_back (static_cast<char>(this->rawMessage[0]));
        this->versionNumber.push_back (static_cast<char>(this->rawMessage[1]));

        this->opIdOrStatusCode = 0xffff & (static_cast<short>(this->rawMessage[2])<<8
                                           | static_cast<short>(this->rawMessage[3]));

        this->requestId = 0xffffffff & (static_cast<int>(this->rawMessage[4])<<24
                                        | static_cast<int>(this->rawMessage[5])<<16
                                        | static_cast<int>(this->rawMessage[6])<<8
                                        | static_cast<int>(this->rawMessage[7]));

        string::size_type attrGroupPtr = 8; // We're now on the begin attribute group tag

        //ipp::tag beginAttributeGroupTag = (ipp::tag)this->rawMessage[attrGroupPtr];
        //cerr << "first attr group: " << (int)beginAttributeGroupTag  << ": " << Attr::tagName(beginAttributeGroupTag)<< endl;

        // Now get first attribute, and create an IppAttr, passing in rawMessage.substr(relevantBit);

        attrGroupPtr++; // We're now on the value tag byte

        ipp::Attr currentAttribute;

        bool finished = false;
        bool firstloop = true;
        while (!finished) {

                //ipp::tag valueTag = (ipp::tag)this->rawMessage[attrGroupPtr];
                //cerr << "value tag: " << (int)valueTag << ": " << Attr::tagName(valueTag) <<  endl;

                attrGroupPtr++; // We're now on the first byte of the name length
                //cerr << "attrGroupPtr:" << attrGroupPtr << endl;
                //cerr << "rawMessage[nameLength1Position] (MSB): " << hex << (int)rawMessage[attrGroupPtr] << endl;
                short nameLength1 = static_cast<short>((this->rawMessage[attrGroupPtr++] & 0xff)<<8);
                //cerr << "name length1: " << nameLength1 << endl;
                short nameLength2 = static_cast<short>(this->rawMessage[attrGroupPtr] & 0xff);
                //cerr << "rawMessage[nameLength2Position] (LSB): " << hex << (int)rawMessage[attrGroupPtr] << endl;
                //cerr << "name length2: " << nameLength2 << endl;
                short nameLength = nameLength1 | nameLength2;
                //cerr << "name length: " << nameLength << endl;

                attrGroupPtr++; // We're now on the first byte of the name
                if (attrGroupPtr + nameLength > rawMessageSize) {
                        // We're going to fall off the end of the raw message...
                        cerr << "attrGroupPtr:" << attrGroupPtr
                             << " nameLength:" << nameLength
                             << " rawMessageSize:" << rawMessageSize << endl;
                        throw runtime_error ("Malformed IPP payload");
                }
                string name = this->rawMessage.substr (attrGroupPtr, nameLength);
                if (nameLength > 0) {
                        cerr << "\nname: " << name << endl;
                }

                attrGroupPtr += nameLength; // We're now on the first byte of the value length

                short valueLength1 = static_cast<short>(this->rawMessage[attrGroupPtr++]<<8);
                short valueLength2 = static_cast<short>(this->rawMessage[attrGroupPtr]);
                short valueLength = valueLength1 | valueLength2;
                //cerr << "value length: " << valueLength << endl;

                attrGroupPtr++; // We're now on the first byte of the value
                if (attrGroupPtr + valueLength > rawMessageSize) {
                        // We're going to fall off the end of the raw message...
                        cerr << "attrGroupPtr:" << attrGroupPtr
                             << " nameLength:" << nameLength
                             << " rawMessageSize:" << rawMessageSize << endl;
                        throw runtime_error ("Malformed IPP payload (2)");
                }
                string value = this->rawMessage.substr (attrGroupPtr, valueLength);
                cerr << "value: " << value << endl;

                // If nameLength is 0, then we have an additional value.
                if (nameLength > 0) {
                        if (!firstloop) { // then the attribute name has changed
                                //cerr << "push_back the last currentAttribute before setting the currentAttribute" << endl;
                                this->attributes.push_back (currentAttribute);
                        }
                        //cerr << "*** We have an attribute name (" << name << ") ***" << endl;
                        currentAttribute.reset (name);

                        // Debugging:
#ifdef DEBUG__
                        if (valueTag == currentAttribute.getValueType()) {
                                cerr << "value type tags match" << endl;
                        } else {
                                cerr << "Warning: value type tags DON'T match:" << endl;
                                cerr << "valueTag is " << Attr::tagName (valueTag) << " currentAttribute.getValueType() is "
                                     << Attr::tagName (currentAttribute.getValueType()) << endl;
                        }
#endif
                }
                currentAttribute.setValue (value);

                if (firstloop) { firstloop = false; }

                attrGroupPtr += valueLength; // Should now be on a tag; either end-tag or a new value.

                // could re-use valueTag here, really.
                ipp::tag delimiterTag = (ipp::tag)this->rawMessage[attrGroupPtr];
                //cerr << "delimiter tag: " << ipp::Attr::tagName(delimiterTag) << endl;
                if (delimiterTag == ipp::end_attrs || attrGroupPtr >= rawMessageSize) {
                        cerr << "Finished is true" << endl;
                        finished = true;
                } else if (delimiterTag == ipp::job_attr) {
                        // Starting job attributes group now.
                        cerr << "Job attributes group starts here." << endl;
                        attrGroupPtr++;
                } else if (delimiterTag == ipp::op_attr) {
                        cerr << "Operation attributes group starts here." << endl;
                        attrGroupPtr++;
                } else if (delimiterTag == ipp::printer_attr) {
                        cerr << "Printer attributes group starts here." << endl;
                        attrGroupPtr++;
                } else if (delimiterTag == ipp::unsupported_attr) {
                        cerr << "Unsupported attributes group starts here." << endl;
                        attrGroupPtr++;
                } else if (delimiterTag == ipp::subscription_attr) {
                        cerr << "Subscription attributes group starts here." << endl;
                        attrGroupPtr++;
                } else if (delimiterTag == ipp::event_notif_attr) {
                        cerr << "Event notification attributes group starts here." << endl;
                        attrGroupPtr++;
                } else if (delimiterTag == ipp::resource_attr) {
                        cerr << "Resource attributes group starts here." << endl;
                        attrGroupPtr++;
                } else if (delimiterTag == ipp::document_attr) {
                        cerr << "Document attributes group starts here." << endl;
                        attrGroupPtr++;
                }
        }

        // Push back the last one.
        cerr << "push_back the final currentAttribute" << endl;
        this->attributes.push_back (currentAttribute);

        // Was there any data? If so, record the start position of the data.
        //cerr << "message size: " << rawMessageSize << endl;
        //cerr << "attrGroupPtr: " << attrGroupPtr << endl;
        //cerr << "attrGroupPtr points to: " << (int)rawMessage[attrGroupPtr] << endl;
        if (rawMessage[attrGroupPtr++] == ipp::end_attrs && rawMessageSize > attrGroupPtr) {
                this->rawMessageDataPos = attrGroupPtr;
                //cerr << "IPP data is at " << attrGroupPtr << endl;
        }
}

// First 2 bytes of rawMessage
vector<char>
dt::ipp::Encoding::getVersionNumber (void) const
{
        return this->versionNumber;
}

// Bytes 2 to 3 of rawMessage.
short
dt::ipp::Encoding::getOpIdOrStatusCode (void) const
{
        return this->opIdOrStatusCode;
}

// Bytes 4, 5, 6 and 7.
int
dt::ipp::Encoding::getRequestId (void) const
{
        return this->requestId;
}

// The IPP attributes
vector<ipp::Attr>
dt::ipp::Encoding::getAttributes (void) const
{
        return this->attributes;
}

#ifdef NEED_GET_BY_KEY
string
dt::ipp::Encoding::getAttribute (const string& key) const
{
}
#endif

// Optional data (follows the attributes)
std::string
dt::ipp::Encoding::getData (void) const
{
        return rawMessage.substr (rawMessageDataPos);
}

bool
dt::ipp::Encoding::hasData (void) const
{
        return this->rawMessageDataPos != string::npos;
}

void
dt::ipp::Encoding::setVersionNumber (const char major, const char minor)
{
        this->versionNumber.clear();
        this->versionNumber.push_back (major);
        this->versionNumber.push_back (minor);
}

void
dt::ipp::Encoding::setOperationId (const short id)
{
        this->opIdOrStatusCode = id;
}

void
dt::ipp::Encoding::setStatusCode (const ipp::status_code code)
{
        this->opIdOrStatusCode = static_cast<short>(code);
}

void
dt::ipp::Encoding::setRequestId (const int id)
{
        this->requestId = id;
}

void
dt::ipp::Encoding::addAttribute (const ipp::Attr& a)
{
        this->attributes.push_back (a);
}
