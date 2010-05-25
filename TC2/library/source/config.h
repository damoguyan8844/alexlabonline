/// @source      config.h
/// @description Common configuration options & declarations.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------
#ifndef __CONFIG_H__
#define __CONFIG_H__
// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// -----------------------------------------------------------------------------

#define TC2_MAJOR_VERSION 1
#define TC2_MINOR_VERSION 0
#define TC2_MICRO_VERSION 0
#define TC2_VERSION  "1.0.0"

// -----------------------------------------------------------------------------

/// Configuration macro used to specify the platform: Windows or Linux. 
#define PLATFORM(x) PLATFORM_ ## x

/// Generic status codes for success or error.
enum
{
   ST_OK ,     ///< the generic OK value
   ST_ERROR    ///< the generic ERROR value
};

/// Logical values.
enum
{
   ST_FALSE ,  ///< the logical FALSE value
   ST_TRUE     ///< the logical TRUE value 
};

/// A kind of logical values.
enum
{
   ST_NO,   ///< a kind of logical FALSE value
   ST_YES   ///< a kind of logical true value
};

#ifndef __cplusplus
// assumes it does no harm...
typedef unsigned int bool; ///< For compatibility with C++ 

/// Fake C++ logical values
enum { false, true };
#endif

typedef const char cchar;      ///< const char abbreviation
typedef unsigned char uchar;   ///< unsigned char abbreviation
typedef unsigned int uint;     ///< unsigned int abbreviation
typedef unsigned long ulong;   ///< unsigned long abbreviation
typedef unsigned short ushort; ///< unsigned short abbreviation

// -----------------------------------------------------------------------------

#ifdef _WIN32

#define PLATFORM_Windows 1

#define IF_WINDOWS(x) x
#define IF_LINUX(x)

// // enlarges the maximum number of sockets in type fd_set
// (not needed for client application)
// #define FD_SETSIZE 512

// needed for functions exported from a dll
#ifdef TC2_EXPORTS
#define TC2API __declspec(dllexport)
#else
#define TC2API __declspec(dllimport)
#endif

// disable because this forces us to use non-standard functions
#define _CRT_SECURE_NO_WARNINGS

// -----------------------------------------------------------------------------

#elif __linux__

#define PLATFORM_Linux 1

#define IF_WINDOWS(x)
#define IF_LINUX(x) x

#define TC2API /* nothing */

// -----------------------------------------------------------------------------

#else
#error "unsupported platform"

#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
} // #ifdef __cplusplus
#endif
// -----------------------------------------------------------------------------
#endif // __CONFIG_H__
// -----------------------------------------------------------------------------
