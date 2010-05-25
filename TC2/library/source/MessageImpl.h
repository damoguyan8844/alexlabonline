/// @source      MessageImpl.h
/// @description Private interface for class Message.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------
#ifndef __MESSAGE_IMPL_H__
#define __MESSAGE_IMPL_H__
// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// -----------------------------------------------------------------------------

#include "time.h"

struct Connection;

// -----------------------------------------------------------------------------

/// Message data structure.
/// This structure abstracts and encapsulates the buffers used by the framework.
/// The buffer management policies used by the framework deals with messages.
/// Also, this class describes the format of the data exchanged over-the-wire
/// between the server and its clients; this format includes a counter giving
/// the size of the payload data, starting and ending flags, and other control
/// fields that can be used by the applications. 
/// Thread.
struct Message /* packed, no alignment holes assumed */
{
   // ------------------

   // identification of the connection that owns this Message, that is, the
   // Connection from which this message was received, or through which it will
   // be sent

   uint ipAddr; ///< @private The IP address of the Connection that owns this
                /// Message

   // ------------------
   
   // identification of the Message

   ushort seqNo; ///< @private The sequence number of this Message.
                 /// Valid within the Connection that owns it (meaningful only
                 /// for a just received Message)

   time_t   creationTime; ///< @private The time when this Message was created.
                          /// Initialized by the client application (not being
                          /// used for anything at the moment)

   ushort flags; ///< @private Assorted flags describing the Message

   // --------- begin Message on the wire

   /// @private Marker in the start of a Message, used for consistency check.
   /// Position 0, length 1, value 0x7E
   char prefix;

   /// @private Unique serial number of this Message.
   /// Created by the originating client application.
   /// Position 1, length 4
   uint orgSeqNo;

   /// @private Identification of the originating client application.
   /// Position 5, length 4
   uint orgId;

   /// @private Number of seconds the Message should be considered valid.
   /// Initialized by the client application, could be used by a business rule.
   /// Position 9, length 4
   ushort duration;

   /// @private Size in bytes of actual data in the Message.
   /// Position 13, length 2
   ushort size; // in host order!

   /// @private Bytes of actual data in the message.
   /// Position 15, length <i> size </i>
   char body[];

   /// @private Marker in the end of a message, used for consistency check.
   /// Position 15+<i>size</i>, length 1, value '\x7E'.
   // char suffix;

   // --------- end Message on the wire
};

// the message is received in two parts:

/// @private PART1: prefix (1), orgSeqNo (4), orgId (4), duration (2), size (2)
/// @memberof Message
#define MSG_SIZE_PART1    (1 + 4 + 4 + 2 + 2)

/// @private PART2: body (size), suffix (1)
/// @memberof Message
#define MSG_SIZE_PART2(m) ((m)->size + 1)

/// The boundary markers of a Message on the wire
/// @private @memberof Message
enum MessageMarkers
{
   MSG_PREFIX = '\x7e',
   MSG_SUFFIX = '\x7e'
};

// -----------------------------------------------------------------------------
#ifdef __cplusplus
} // #ifdef __cplusplus
#endif
// -----------------------------------------------------------------------------
#endif // __MESSAGE_IMPL_H__
// -----------------------------------------------------------------------------
