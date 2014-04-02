/*
 * "$Id: cups-private.h 10996 2013-05-29 11:51:34Z msweet $"
 *
 *   Private definitions for CUPS.
 *
 *   Copyright 2007-2013 by Apple Inc.
 *   Copyright 1997-2007 by Easy Software Products, all rights reserved.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Apple Inc. and are protected by Federal copyright
 *   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
 *   which should have been included with this file.  If this file is
 *   file is missing or damaged, see the license at "http://www.cups.org/".
 *
 *   This file is subject to the Apple OS-Developed Software exception.
 */

#ifndef _CUPS_CUPS_PRIVATE_H_
#  define _CUPS_CUPS_PRIVATE_H_

/*
 * Include necessary headers...
 */

#  include "string-private.h"
#  include "debug-private.h"
#  include "array-private.h"
//#  include "ipp-private.h"
//#  include "http-private.h" // Instead of this, defined HTTP_MAX_HOST: (Tam)
#  define HTTP_MAX_HOST		256	/* Max length of hostname string */

// Include file.h for definition of cups_file_t: (Tam)
#  include <cups/file.h>

//#  include "language-private.h"
//#  include "pwg-private.h"
//#  include "ppd-private.h"
//#  include "thread-private.h"
//#  include <cups/cups.h>
#  ifdef __APPLE__
#    include <sys/cdefs.h>
#    include <CoreFoundation/CoreFoundation.h>
#  endif /* __APPLE__ */


/*
 * C++ magic...
 */

#  ifdef __cplusplus
extern "C" {
#  endif /* __cplusplus */


/*
 * Types...
 */

// This is copied from cups.h: (Tam)
typedef struct cups_option_s		/**** Printer Options ****/
{
  char		*name;			/* Name of option */
  char		*value;			/* Value of option */
} cups_option_t;

#ifndef _CUPS_LANGUAGE_PRIVATE_H_
// cups_lang_t is used in file.c, also requires cups_encoding_t: (Tam)
typedef enum cups_encoding_e		/**** Language Encodings ****/
{
  CUPS_AUTO_ENCODING = -1,		/* Auto-detect the encoding @private@ */
  CUPS_US_ASCII,			/* US ASCII */
  CUPS_ISO8859_1,			/* ISO-8859-1 */
  CUPS_ISO8859_2,			/* ISO-8859-2 */
  CUPS_ISO8859_3,			/* ISO-8859-3 */
  CUPS_ISO8859_4,			/* ISO-8859-4 */
  CUPS_ISO8859_5,			/* ISO-8859-5 */
  CUPS_ISO8859_6,			/* ISO-8859-6 */
  CUPS_ISO8859_7,			/* ISO-8859-7 */
  CUPS_ISO8859_8,			/* ISO-8859-8 */
  CUPS_ISO8859_9,			/* ISO-8859-9 */
  CUPS_ISO8859_10,			/* ISO-8859-10 */
  CUPS_UTF8,				/* UTF-8 */
  CUPS_ISO8859_13,			/* ISO-8859-13 */
  CUPS_ISO8859_14,			/* ISO-8859-14 */
  CUPS_ISO8859_15,			/* ISO-8859-15 */
  CUPS_WINDOWS_874,			/* CP-874 */
  CUPS_WINDOWS_1250,			/* CP-1250 */
  CUPS_WINDOWS_1251,			/* CP-1251 */
  CUPS_WINDOWS_1252,			/* CP-1252 */
  CUPS_WINDOWS_1253,			/* CP-1253 */
  CUPS_WINDOWS_1254,			/* CP-1254 */
  CUPS_WINDOWS_1255,			/* CP-1255 */
  CUPS_WINDOWS_1256,			/* CP-1256 */
  CUPS_WINDOWS_1257,			/* CP-1257 */
  CUPS_WINDOWS_1258,			/* CP-1258 */
  CUPS_KOI8_R,				/* KOI-8-R */
  CUPS_KOI8_U,				/* KOI-8-U */
  CUPS_ISO8859_11,			/* ISO-8859-11 */
  CUPS_ISO8859_16,			/* ISO-8859-16 */
  CUPS_MAC_ROMAN,			/* MacRoman */
  CUPS_ENCODING_SBCS_END = 63,		/* End of single-byte encodings @private@ */

  CUPS_WINDOWS_932,			/* Japanese JIS X0208-1990 */
  CUPS_WINDOWS_936,			/* Simplified Chinese GB 2312-80 */
  CUPS_WINDOWS_949,			/* Korean KS C5601-1992 */
  CUPS_WINDOWS_950,			/* Traditional Chinese Big Five */
  CUPS_WINDOWS_1361,			/* Korean Johab */
  CUPS_ENCODING_DBCS_END = 127,		/* End of double-byte encodings @private@ */

  CUPS_EUC_CN,				/* EUC Simplified Chinese */
  CUPS_EUC_JP,				/* EUC Japanese */
  CUPS_EUC_KR,				/* EUC Korean */
  CUPS_EUC_TW,				/* EUC Traditional Chinese */
  CUPS_JIS_X0213,			/* JIS X0213 aka Shift JIS */
  CUPS_ENCODING_VBCS_END = 191		/* End of variable-length encodings @private@ */
} cups_encoding_t;

typedef struct cups_lang_s		/**** Language Cache Structure ****/
{
  struct cups_lang_s	*next;		/* Next language in cache */
  int			used;		/* Number of times this entry has been used. */
  cups_encoding_t	encoding;	/* Text encoding */
  char			language[16];	/* Language/locale name */
  cups_array_t		*strings;	/* Message strings @private@ */
} cups_lang_t;
#endif

typedef struct _cups_buffer_s		/**** Read/write buffer ****/
{
  struct _cups_buffer_s	*next;		/* Next buffer in list */
  size_t		size;		/* Size of buffer */
  char			used,		/* Is this buffer used? */
			d[1];		/* Data buffer */
} _cups_buffer_t;


typedef struct _cups_globals_s		/**** CUPS global state data ****/
{
  /* Multiple places... */
  const char		*cups_datadir,	/* CUPS_DATADIR environment var */
			*cups_serverbin,/* CUPS_SERVERBIN environment var */
			*cups_serverroot,
					/* CUPS_SERVERROOT environment var */
			*cups_statedir,	/* CUPS_STATEDIR environment var */
			*localedir;	/* LOCALDIR environment var */

  /* adminutil.c */
  time_t		cupsd_update;	/* Last time we got or set cupsd.conf */
  char			cupsd_hostname[HTTP_MAX_HOST];
					/* Hostname for connection */
  int			cupsd_num_settings;
					/* Number of server settings */
  cups_option_t		*cupsd_settings;/* Server settings */

  /* auth.c */
#  ifdef HAVE_GSSAPI
  char			gss_service_name[32];
  					/* Kerberos service name */
#  endif /* HAVE_GSSAPI */

  /* backend.c */
  char			resolved_uri[1024];
					/* Buffer for cupsBackendDeviceURI */

  /* debug.c */
#  ifdef DEBUG
  int			thread_id;	/* Friendly thread ID */
#  endif /* DEBUG */

  /* file.c */
  cups_file_t		*stdio_files[3];/* stdin, stdout, stderr */

  /* http.c */
  char			http_date[256];	/* Date+time buffer */

  /* http-addr.c */
  unsigned		ip_addr;	/* Packed IPv4 address */
  char			*ip_ptrs[2];	/* Pointer to packed address */
  struct hostent	hostent;	/* Host entry for IP address */
#  ifdef HAVE_GETADDRINFO
  char			hostname[1024];	/* Hostname */
#  endif /* HAVE_GETADDRINFO */
  int			need_res_init;	/* Need to reinitialize resolver? */

  /* ipp.c */
// Not using: (Tam)
//  ipp_uchar_t		ipp_date[11];	/* RFC-1903 date/time data */
  _cups_buffer_t	*cups_buffers;	/* Buffer list */

  /* ipp-support.c */
  int			ipp_port;	/* IPP port number */
  char			ipp_unknown[255];
					/* Unknown error statuses */

  /* language.c */
  cups_lang_t		*lang_default;	/* Default language */
#  ifdef __APPLE__
  char			language[32];	/* Cached language */
#  endif /* __APPLE__ */

  /* ppd.c */
// Not using: (Tam)
//  ppd_status_t		ppd_status;	/* Status of last ppdOpen*() */
  int			ppd_line;	/* Current line number */
// Not using: (Tam)
//  ppd_conform_t		ppd_conform;	/* Level of conformance required */

  /* pwg-media.c */
  cups_array_t		*leg_size_lut,	/* Lookup table for legacy names */
			*ppd_size_lut,	/* Lookup table for PPD names */
			*pwg_size_lut;	/* Lookup table for PWG names */
// Not using: (Tam)
//  pwg_media_t		pwg_media;	/* PWG media data for custom size */
  char			pwg_name[65];	/* PWG media name for custom size */

  /* request.c */
// Not using: (Tam)
//  http_t		*http;		/* Current server connection */
// Not using: (Tam)
//  ipp_status_t		last_error;	/* Last IPP error */
// Removed to avoid use of _cupsStrFree() in globals.c: (Tam)
//  char			*last_status_message;
					/* Last IPP status-message */

  /* snmp.c */
  char			snmp_community[255];
					/* Default SNMP community name */
  int			snmp_debug;	/* Log SNMP IO to stderr? */

  /* tempfile.c */
  char			tempfile[1024];	/* cupsTempFd/File buffer */

  /* usersys.c */
// Not using: (Tam)
//  http_encryption_t	encryption;	/* Encryption setting */
  char			user[65],	/* User name */
			user_agent[256],/* User-Agent string */
			server[256],	/* Server address */
			servername[256],/* Server hostname */
			password[128];	/* Password for default callback */
// Not using: (Tam)
//  cups_password_cb2_t	password_cb;	/* Password callback */
  void			*password_data;	/* Password user data */
// Not using: (Tam)
//  http_tls_credentials_t tls_credentials;
//					/* Default client credentials */
// Not using: (Tam)
//  cups_client_cert_cb_t	client_cert_cb;	/* Client certificate callback */
  void			*client_cert_data;
					/* Client certificate user data */
// Not using: (Tam)
//  cups_server_cert_cb_t	server_cert_cb;	/* Server certificate callback */
  void			*server_cert_data;
					/* Server certificate user data */
  int			server_version,	/* Server IPP version */
			any_root,	/* Allow any root */
			expired_certs,	/* Allow expired certs */
			expired_root;	/* Allow expired root */

  /* util.c */
  char			def_printer[256];
					/* Default printer */
// Not using: (Tam)
//  char			ppd_filename[HTTP_MAX_URI];
//					/* PPD filename */
} _cups_globals_t;

typedef struct _cups_media_db_s		/* Media database */
{
  char		*color,			/* Media color, if any */
		*key,			/* Media key, if any */
		*info,			/* Media human-readable name, if any */
		*size_name,		/* Media PWG size name, if provided */
		*source,		/* Media source, if any */
		*type;			/* Media type, if any */
  int		width,			/* Width in hundredths of millimeters */
		length,			/* Length in hundredths of
					 * millimeters */
		bottom,			/* Bottom margin in hundredths of
					 * millimeters */
		left,			/* Left margin in hundredths of
					 * millimeters */
		right,			/* Right margin in hundredths of
					 * millimeters */
		top;			/* Top margin in hundredths of
					 * millimeters */
} _cups_media_db_t;

typedef struct _cups_dconstres_s	/* Constraint/resolver */
{
  char	*name;				/* Name of resolver */
// Not using: (Tam)
//  ipp_t	*collection;			/* Collection containing attrs */
} _cups_dconstres_t;

struct _cups_dinfo_s			/* Destination capability and status
					 * information */
{
  int			version;	/* IPP version */
  const char		*uri;		/* Printer URI */
  char			*resource;	/* Resource path */
// Not using: (Tam)
//  ipp_t			*attrs;		/* Printer attributes */
  int			num_defaults;	/* Number of default options */
  cups_option_t		*defaults;	/* Default options */
  cups_array_t		*constraints;	/* Job constraints */
  cups_array_t		*resolvers;	/* Job resolvers */
  cups_array_t		*localizations;	/* Localization information */
  cups_array_t		*media_db;	/* Media database */
  _cups_media_db_t	min_size,	/* Minimum size */
			max_size;	/* Maximum size */
  unsigned		cached_flags;	/* Flags used for cached media */
  cups_array_t		*cached_db;	/* Cache of media from last index/default */
  time_t		ready_time;	/* When xxx-ready attributes were last queried */
// Not using: (Tam)
//  ipp_t			*ready_attrs;	/* xxx-ready attributes */
  cups_array_t		*ready_db;	/* media[-col]-ready media database */
};


/*
 * Prototypes...
 */

#  ifdef __APPLE__
extern CFStringRef	_cupsAppleCopyDefaultPaperID(void);
extern CFStringRef	_cupsAppleCopyDefaultPrinter(void);
extern int		_cupsAppleGetUseLastPrinter(void);
extern void		_cupsAppleSetDefaultPaperID(CFStringRef name);
extern void		_cupsAppleSetDefaultPrinter(CFStringRef name);
extern void		_cupsAppleSetUseLastPrinter(int uselast);
#  endif /* __APPLE__ */

extern char		*_cupsBufferGet(size_t size);
extern void		_cupsBufferRelease(char *b);

// Not using: (Tam)
//extern http_t		*_cupsConnect(void);
extern int		_cupsGet1284Values(const char *device_id,
			                   cups_option_t **values);
// Not using: (Tam)
//extern const char	*_cupsGetDestResource(cups_dest_t *dest, char *resource,
//			                      size_t resourcesize);
// Not using: (Tam)
//extern int		_cupsGetDests(http_t *http, ipp_op_t op,
//			              const char *name, cups_dest_t **dests,
//			              cups_ptype_t type, cups_ptype_t mask);
extern const char	*_cupsGetPassword(const char *prompt);
extern void		_cupsGlobalLock(void);
extern _cups_globals_t	*_cupsGlobals(void);
extern void		_cupsGlobalUnlock(void);
#  ifdef HAVE_GSSAPI
extern const char	*_cupsGSSServiceName(void);
#  endif /* HAVE_GSSAPI */
extern int		_cupsNextDelay(int current, int *previous);
extern void		_cupsSetDefaults(void);
// Not using: (Tam)
//extern void		_cupsSetError(ipp_status_t status, const char *message,
//			              int localize);
// Not using: (Tam)
//extern void		_cupsSetHTTPError(http_status_t status);
#  ifdef HAVE_GSSAPI
extern int		_cupsSetNegotiateAuthString(http_t *http,
			                            const char *method,
						    const char *resource);
#  endif /* HAVE_GSSAPI */
extern char		*_cupsUserDefault(char *name, size_t namesize);


/*
 * C++ magic...
 */

#  ifdef __cplusplus
}
#  endif /* __cplusplus */
#endif /* !_CUPS_CUPS_PRIVATE_H_ */

/*
 * End of "$Id: cups-private.h 10996 2013-05-29 11:51:34Z msweet $".
 */
