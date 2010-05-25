/// @source      Time.c
/// @description Implementation of the Time formatting facility.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------

// includes

// common configuration options & declarations
#include "../config.h" // always include first

// C language includes
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// framework includes
#include "Time.h"    /* time_xxx functions */

// -----------------------------------------------------------------------------

// global declarations

/// Time formatting class.
/// This class provides an array of methods for formatting time.
/// @class Time

/// Identification of this source file in the Log file. 
/// @private @memberof Time
static cchar sourceID[] = "ZTM";

// helper functions
#if PLATFORM(Windows)
static void localtime_r(const time_t*, struct tm*);
#endif
      
// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Formats the current date as a string in the format DD/MM/YYYY (zero
    terminated).

    @param [out]
    formattedDate : address of the buffer which will receive the formatted date

    @memberof Time
*/

TC2API void time_formatCurrentDate(char * formattedDate)
{
   time_formatDate(formattedDate , time(NULL));
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Formats the current date as a string in the format DDMMYY (zero terminated).

    @param [out]
    formattedDate : address of the buffer which will receive the formatted date

    @memberof Time
*/

TC2API void time_formatCurrentShortDatePacked(char * formattedDate)
{
   time_formatShortDatePacked(formattedDate , time(NULL));
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Formats the current date as a string in the format YYMMDD (zero terminated).

    @param [out]
    formattedDate : address of the buffer which will receive the formatted date

    @memberof Time
*/

TC2API void time_formatCurrentShortDateReversePacked(char * formattedDate)
{
   time_formatShortDateReversePacked(formattedDate , time(NULL));
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Formats the current time as a string in the format HH:MM:SS (zero
    terminated).

    @param [out]
    formattedTime : address of the buffer which will receive the formatted time

    @memberof Time
*/

TC2API void time_formatCurrentTime(char * formattedTime)
{
   time_formatTime(formattedTime, time(NULL));
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Formats the current time as a string in the format HHMMSS, that is, without
    the ':' separators (zero terminated).

    @param [out]
    formattedTime : address of the buffer which will receive the formatted time

    @memberof Time
*/

TC2API void time_formatCurrentTimePacked(char * formattedTime)
{
   time_formatTimePacked(formattedTime, time(NULL));
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Formats a date as a string in the format DD/MM/YYYY (zero terminated).

    @param [in]
    timestamp : as returned by the <i> time </i> function

    @param [out]
    formattedDate : address of the buffer which will receive the formatted date

    @memberof Time
*/

TC2API void time_formatDate(char * formattedDate, time_t timestamp)
{
   struct tm result;

   assert(formattedDate);

   localtime_r(&timestamp, &result);

   sprintf(formattedDate+0, "%02d", result.tm_mday);
   formattedDate[2] = '/';
   sprintf(formattedDate+3, "%02d", result.tm_mon+1);
   formattedDate[5] = '/';
   sprintf(formattedDate+6, "%04d", result.tm_year+1900);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Formats a date as a string in the format DDMMYY (zero terminated).

    @param [in]
    timestamp : as returned by the <i> time </i> function

    @param [out]
    formattedDate : address of the buffer which will receive the formatted date

    @memberof Time
*/

TC2API void time_formatShortDatePacked(char * formattedDate, time_t timestamp)
{
   struct tm result;

   assert(formattedDate);

   localtime_r(&timestamp, &result);

   sprintf(formattedDate+0, "%02d", result.tm_mday);
   sprintf(formattedDate+2, "%02d", result.tm_mon+1);
   sprintf(formattedDate+4, "%02d", result.tm_year-100);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Formats a date as a string in the format YYMMDD (zero terminated).

    @param [in]
    timestamp : as returned by the <i> time </i> function

    @param [out]
    formattedDate : address of the buffer which will receive the formatted date

    @memberof Time
*/

TC2API void time_formatShortDateReversePacked(char * formattedDate, time_t timestamp)
{
   struct tm result;

   assert(formattedDate);

   localtime_r(&timestamp, &result);

   sprintf(formattedDate+0, "%02d", result.tm_year-100);
   sprintf(formattedDate+2, "%02d", result.tm_mon+1);
   sprintf(formattedDate+4, "%02d", result.tm_mday);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Formats a time value as a string in the format HH:MM:SS (zero terminated).

    @param [in]
    timestamp : as returned by the <i> time </i> function

    @param [out]
    formattedTime : address of the buffer which will receive the formatted time

    @memberof Time
*/

TC2API void time_formatTime(char * formattedTime, time_t timestamp)
{
   struct tm result;

   assert(formattedTime);

   localtime_r(&timestamp, &result);

   sprintf(formattedTime+0, "%02d", result.tm_hour);
   formattedTime[2] = ':';
   sprintf(formattedTime+3, "%02d", result.tm_min);
   formattedTime[5] = ':';
   sprintf(formattedTime+6, "%02d", result.tm_sec);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Formats a time value as a string in the format HHMMSS, that is, without the
    ':' separators (zero terminated).

    @param [in]
    timestamp : as returned by the <i> time </i> function

    @param [out]
    formattedTime : address of the buffer which will receive the formatted time

    @memberof Time
*/

TC2API void time_formatTimePacked(char * formattedTime, time_t timestamp)
{
   struct tm result;

   assert(formattedTime);

   localtime_r(&timestamp, &result);

   sprintf(formattedTime+0, "%02d", result.tm_hour);
   sprintf(formattedTime+2, "%02d", result.tm_min);
   sprintf(formattedTime+4, "%02d", result.tm_sec);
}

// -----------------------------------------------------------------------------
// INTERNAL HELPER FUNCTIONS
// -----------------------------------------------------------------------------

/** Windows-specific substitute for the localtime_r Thread safe function.

    @param [in]
    from: time as returned by the <i> time </i> function

    @param [out]
    to : address of a standard struct tm sctructure

    @private @memberof Time
*/

#if PLATFORM(Windows)

static void localtime_r(const time_t* from, struct tm* to)
{
   // according to MSDN, at least as of VS 8.0 localtime is thread-safe
   memcpy(to, localtime(from), sizeof(struct tm));
}

#endif
// -----------------------------------------------------------------------------
// the end
