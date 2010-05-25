/// @source      Timeout.c
/// @description Implementation of the Timeout facility.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------

// includes

// common configuration options & declarations
#include "../config.h" // always include first

// C language includes
#include <time.h>

// framework includes
#include "Timeout.h" /* timeout_xxx functions */

// -----------------------------------------------------------------------------

// global declarations

/// Identification of this source file in the Log file. 
/// @private @memberof Timeout
static cchar sourceID[] = "ZTO";

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Initializes a Timeout structure.

    @param
    timeout : the Timeout structure to be initialized.

    @param [in]
    nSeconds : number of seconds until timeout occurs. If 0 timeout always
    occurs, if (unsigned)-1 timeout never occurs.

    @memberof Timeout
*/

TC2API void timeout_init(Timeout* timeout, uint nSeconds)
{
   timeout->start = time(NULL);
   timeout->nSeconds = nSeconds;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Queries remaining time (in seconds) in a Timeout structure.

    @param [in]
    timeout : the Timeout structure to be queried

    @return
    number of seconds until timeout occurs

    @memberof Timeout
*/

TC2API uint timeout_remaining(const Timeout* timeout)
{
   time_t now;
   uint elapsed;

   // has timeout been configured to always expire?
   if (!timeout->nSeconds)
      return 0; // yes, so timeout expired

   // has timeout been configured to never expire ?
   if (timeout->nSeconds == TIMEOUT_WAIT_FOREVER)
      return TIMEOUT_WAIT_FOREVER; // yes, so timeout did not expire

   // now, checks the elapsed time

   now = time(NULL);
   elapsed = (uint)(now - timeout->start);

   // has timeout expired ?
   if (elapsed >= timeout->nSeconds)
      return 0; // yes, timeout expired

   // no, timeout did not expire
   return timeout->nSeconds - elapsed;
}

// -----------------------------------------------------------------------------
// the end
