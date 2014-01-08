/* -*-c++-*- */
/*
 * IPP encoding, as described in Section 3.1 of RFC 2910.
 *
 * Our IPP encoding is a set of IPP attributes, forming an IPP
 * communication, along with the methods to decode and encode those
 * attributes in a manner suitable for transfer in an HTTP
 * communication.
 */

#ifndef _DT_IPP_ENCODING_H_
#define _DT_IPP_ENCODING_H_ 1

#ifdef __GNUG__
# pragma interface
#endif

#include <string>
#include <vector>
#include <map>
#include "Attr.h"

namespace dt
{
        namespace ipp
        {
                const unsigned int MIN_MESSAGE_LENGTH = 9;

                // RFC 2911 4.3.7. This may go into a Job header or somewhere like that; this may not be its final location.
                enum job_state {
                        pending = 0x3,
                        pending_held,
                        processing,
                        processing_stopped,
                        cancelled,
                        aborted,
                        completed
                };

                enum printer_state {
                        printer_idle = 0x3,
                        printer_processing,
                        printer_stopped
                };

                // Operation ids. Where are these listed in the RFCs? Find them in cups/ipp.h.
                enum operation_id {
                        print_job = 0x2,
                        print_uri,
                        validate_job,
                        create_job,
                        send_document,
                        send_uri,
                        cancel_job,
                        get_job_attributes,
                        get_jobs,
                        get_printer_attributes,
                        hold_job,
                        release_job,
                        restart_job,
                        pause_printer = 0x10,
                        resume_printer,
                        purge_jobs,
                        set_printer_attributes,
                        set_job_attributes,
                        get_printer_supported_values,
                        create_printer_subscription,
                        create_job_subscription,
                        get_subscription_attributes,
                        get_subscriptions,
                        renew_subscription,
                        cancel_subscription,
                        get_notifications,
                        send_notifications
                        // Further IPP operations see in cups/ipp.h are private/cups additions.
                };

                // IPP response status codes (RFC2911 13.1)
                enum status_code {
                        successful_ok = 0x0000,
                        successful_ok_ignored_or_substituted_attributes,
                        successful_ok_conflicting_attributes,

                        client_error_bad_request = 0x0400,
                        client_error_forbidden,
                        client_error_not_authenticated,
                        client_error_not_authorized,
                        client_error_not_possible,
                        client_error_timeout,
                        client_error_not_found,
                        client_error_gone,
                        client_error_request_entity_too_large,
                        client_error_request_value_too_long,
                        client_error_document_format_not_supported,
                        client_error_attributes_or_values_not_supported,
                        client_error_uri_scheme_not_supported,
                        client_error_charset_not_supported,
                        client_error_conflicting_attributes,
                        client_error_compression_not_supported,
                        client_error_compression_error,
                        client_error_document_format_error,
                        client_error_document_access_error,

                        server_error_internal_error = 0x0500,
                        server_error_operation_not_supported,
                        server_error_service_unavailable,
                        server_error_version_not_supported,
                        server_error_device_error,
                        server_error_temporary_error,
                        server_error_not_accepting_jobs,
                        server_error_busy,
                        server_error_job_cancelled,
                        server_error_multiple_document_jobs_not_supported
                };

                class Encoding
                {
                public:
                        Encoding() : rawMessage(""), rawMessageDataPos(std::string::npos) {}
                        Encoding(const std::string& msg) : rawMessage(msg), rawMessageDataPos(std::string::npos) { this->parse(); }
                        ~Encoding() {}

                        /*! First 2 bytes of rawMessage form the version */
                        std::vector<char> getVersionNumber (void) const;
                        /*! Bytes 2 to 3 of rawMessage. */
                        short getOpIdOrStatusCode (void) const;
                        /*! Bytes 4, 5, 6 and 7. */
                        int getRequestId (void) const;
                        /*! The attributes */
                        std::vector<ipp::Attr> getAttributes (void) const;
#ifdef NEED_GET_BY_KEY
                        /*! Get attribute by key */
                        std::string getAttribute (const std::string& key) const;
#endif
                        /*! Optional data (follows the attributes) */
                        std::string getData (void) const;

                        /*! Is there data to get with getData? */
                        bool hasData (void) const;

                        /*!
                         * Output the decoded information.
                         */
                        std::string debugOutput (void) const;

                        void setVersionNumber (const char major, const char minor);

                        void setOperationId (const short id);

                        void setStatusCode (const ipp::status_code code);

                        void setRequestId (const int id);

                        void addAttribute (const ipp::Attr& a);

                        /*!
                         * Encode the attributes and produce a
                         * rawMessage suitable for sending over the
                         * wire as an IPP payload. When done, return
                         * rawMessage.
                         */
                        std::string encode (void);

                private:

                        /*!
                         * Parse rawMessage to extract attributes.
                         */
                        void parse (void);

                private:

                        /*!
                         * 1 1 or 1 0 or 2 0 etc.
                         */
                        std::vector<char> versionNumber;

                        short opIdOrStatusCode = 0;

                        int requestId = 0;

                        /*!
                         * Used when encoding.
                         */
                        ipp::tag currentGroupTag;

                        /*!
                         * The IPP attributes.
                         */
                        std::vector<ipp::Attr> attributes;

                        /*!
                         * The raw message, as transmitted via HTTP.
                         */
                        std::string rawMessage;

                        /*!
                         * The position at which any additional IPP
                         * data (i.e. the document) starts.
                         */
                        std::string::size_type rawMessageDataPos;
                };

                class Request : public Encoding
                {
                };

                class Response : public Encoding
                {
                };

        } // namespace ipp
} // namespace dt

#endif // _DT_IPP_ENCODING_H_
