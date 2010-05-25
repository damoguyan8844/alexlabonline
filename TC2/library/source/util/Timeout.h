/// @source      Timeout.h
/// @description Public interface for the Timeout facility.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------
#ifndef __TIMEOUT_H__
#define __TIMEOUT_H__
// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// -----------------------------------------------------------------------------

// the redundancy here is for doxygen documentation

/// Structure representing timeout.
/// This structure is used by client applications when receiving messages from
/// the server.
typedef struct
{
   time_t start;  ///< @private Time start on which to calculate timeout
   uint nSeconds; ///< @private Number of seconds defining timeout
} Timeout;

/// see Timeout::timeout_init
extern TC2API void timeout_init(Timeout*, uint);

/// see Time::timeout_remaining
extern TC2API uint timeout_remaining(const Timeout*);

/// No timeout
#define TIMEOUT_WAIT_FOREVER ((uint)-1)

// -----------------------------------------------------------------------------
#ifdef __cplusplus
} // #ifdef __cplusplus
#endif
// -----------------------------------------------------------------------------
#endif // __TIMEOUT_H__
// -----------------------------------------------------------------------------
