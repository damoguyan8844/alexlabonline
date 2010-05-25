/// @source      Message.c
/// @description Implementation of class Message.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------

// includes

// common configuration options & declarations
#include "config.h" // always include first

// C language includes
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>  // calloc
#include <string.h>  // memcmp

// framework includes
#include "Message.h"       /* message_xxx functions */
#include "MessageImpl.h"   /* message_xxx functions */
#include "util/Log.h"      /* log_xxx functions     */

// -----------------------------------------------------------------------------

// global declarations

/// Default maximum Message data area size
/// @private @memberof Message
enum MaxMsgBodySizeDefault
{
   MAX_MSG_BODY_SIZE_DEFAULT = 256
};

/// For identification in the log file
/// @private @memberof Message
static char sourceID[] = "ZMS";

/// Actual maximum Message body size
/// @private @memberof Message
static uint maxBodySize = MAX_MSG_BODY_SIZE_DEFAULT;

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the data area of a Message.

    @param [in]
    message : the Message that is being queried

    @return
    the data area of the Message

    @memberof Message
*/

TC2API char* message_buffer(Message* message)
{
   assert(message);
   return message->body;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Returns one byte from the message body.

    @param [in]
    srcMsg : source Message

    @param [in]
    msgOffset : offset into source Message buffer from where to copy byte

    @return
    one byte

    @memberof Message
*/

TC2API uchar message_byte(const Message* srcMsg, ushort msgOffset)
{
   log_func(message_byte);

   if (!srcMsg)
   {
      log_ferror("source message is null");
      return 0xFF;
   }

   if (msgOffset >= srcMsg->size)
   {
      log_ferror("invalid message offset: %d", msgOffset);
      return 0xFF;
   }

   return *((srcMsg->body) + msgOffset);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Clones fields from one message to another.

    Usage: @code
    message_cloneFields(dstMsg, srcMsg, nFields,
      offset1, size1,
      offset2, size2,
      etc
      );
    @endcode

    @param
    dstMsg : destination Message

    @param [in]
    srcMsg : source Message

    @param [in]
    nFields : number of fields to copy

    @pre
    both dstMsg and srcMsg must have valid sizes (see #message_setSize)

    @memberof Message
*/

TC2API void message_cloneFields(Message* dstMsg, const Message* srcMsg,
   ushort nFields, ... ) // int offset, int size

{
   ushort i;
   va_list ap;
   int offset, size; 
   char *srcAddr, *dstAddr;

   log_func(message_cloneFields);

   if (!srcMsg)
   {
      log_ferror("source message is null");
      return;
   }

   if (!dstMsg)
   {
      log_ferror("destination address is null");
      return;
   }

   va_start(ap, nFields);

   for (i = 0; i < nFields; i++)
   {
      offset = va_arg(ap, int); 
      if (offset >= dstMsg->size)
      {
         log_ferror("invalid message offset: %d", offset);
         return;
      }

      size = va_arg(ap, int); 
      if (size > srcMsg->size - offset || size > dstMsg->size - offset)
      {
         log_ferror("invalid copy size: %d", size);
         return;
      }

      srcAddr = (char*)(srcMsg->body) + offset;
      dstAddr = (char*)(dstMsg->body) + offset;
      memcpy(dstAddr, srcAddr, size);
   } // for
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Compares data from an application buffer with the message body.

    Usage: @code
    int result = message_compareToMessage(buffer, message, offset, size);
    @endcode

    @param [in]
    srcAddr1 : first operand of comparison

    @param [in]
    srcMsg2 : second operand of comparison 

    @param [in]
    msgOffset : offset into destination Message buffer where to begin compare

    @param [in]
    size : number of bytes to compare

    @return
    like memcmp: -1, 0, 1

    @pre
    srcMsg2 must have valid size (see #message_setSize)

    @memberof Message
*/

TC2API int message_compareToMessage(cchar* srcAddr1, const Message* srcMsg2, 
   ushort msgOffset, ushort size)
{
   char* srcAddr2;
   log_func(message_compareToMessage);

   if (!srcAddr1)
   {
      log_ferror("source address 1 is null");
      return -1; // tries to be consistent
   }

   if (!srcMsg2)
   {
      log_ferror("source Message 2 is null");
      return 1;  // tries to be consistent 
   }

   if (msgOffset >= srcMsg2->size)
   {
      log_ferror("invalid message 2 offset: %d", msgOffset);
      return 1;  // tries (a little harder) to be consistent 
   }

   if (size > (srcMsg2->size - msgOffset))
   {
      log_ferror("invalid compare size: %d", size);
      return 1; // tries (a little more harder) to be consistent  
   }

   srcAddr2 = (char*)(srcMsg2->body) + msgOffset;
   return memcmp(srcAddr1, srcAddr2, size);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Copies fields from one message to another.

    Usage: @code
    message_copyFields(dstMsg, srcMsg, nFields,
      dstOffset1, srcOffset1, size1,
      dstOffset2, srcOffset2, size2,
      etc
      );
    @endcode

    @param
    dstMsg : destination Message

    @param [in]
    srcMsg : source Message

    @param [in]
    nFields : number of fields to copy

    @pre
    both dstMsg and srcMsg must have valid sizes (see #message_setSize)

    @memberof Message
*/

TC2API void message_copyFields(Message* dstMsg, const Message* srcMsg,
   ushort nFields, ... ) // int msgOffDst, int msgOffSrc, int size
{
   ushort i;
   va_list ap;
   char *srcAddr, *dstAddr;
   int msgOffDst, msgOffSrc, size; 

   log_func(message_copyFields);

   if (!srcMsg)
   {
      log_ferror("source message is null");
      return;
   }

   if (!dstMsg)
   {
      log_ferror("destination address is null");
      return;
   }

   va_start(ap, nFields);

   for (i = 0; i < nFields; i++)
   {
      msgOffDst = va_arg(ap, int); 
      if (msgOffDst >= dstMsg->size)
      {
         log_ferror("invalid dest message offset: %d", msgOffDst);
         return;
      }

      msgOffSrc = va_arg(ap, int); 
      if (msgOffSrc >= srcMsg->size)
      {
         log_ferror("invalid source message offset: %d", msgOffSrc);
         return;
      }

      size = va_arg(ap, int); 
      if (size > srcMsg->size - msgOffSrc || size > dstMsg->size - msgOffDst)
      {
         log_ferror("invalid copy size: %d", size);
         return;
      }

      srcAddr = (char*)(srcMsg->body) + msgOffSrc;
      dstAddr = (char*)(dstMsg->body) + msgOffDst;
      memcpy(dstAddr, srcAddr, size);
   } // for
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Copies data from the message body to an application buffer.

    Usage: @code
    message_copyFromMessage(dstBuffer, srcMsg, msgOffset, size);
    @endcode

    @param
    dstAddr : destination address

    @param [in]
    srcMsg : source Message

    @param [in]
    msgOffset : offset into source Message buffer where to begin copy

    @param [in]
    size : number of bytes to copy

    @pre
    srcMsg must have valid size (see #message_setSize)

    @memberof Message
*/

TC2API void message_copyFromMessage(char* dstAddr, const Message* srcMsg,
   ushort msgOffset, ushort size)
{
   char* srcAddr;
   log_func(message_copyFromMessage);

   if (!srcMsg)
   {
      log_ferror("source message is null");
      return;
   }

   if (!dstAddr)
   {
      log_ferror("destination address is null");
      return;
   }

   if (msgOffset >= srcMsg->size)
   {
      log_ferror("invalid message offset: %d", msgOffset);
      return;
   }

   if (size > (srcMsg->size - msgOffset))
   {
      log_ferror("invalid copy size: %d", size);
      return;
   }

   srcAddr = (char*)(srcMsg->body) + msgOffset;
   memcpy(dstAddr, srcAddr, size);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Copies data from an application buffer to the message body.

    Usage: @code
    message_copyToMessage(dstMsg, srcBuffer, msgOffset, size);
    @endcode

    @param [out]
    dstMsg : destination Message

    @param [in]
    srcAddr : source address

    @param [in]
    msgOffset : offset into destination Message buffer where to begin copy

    @param [in]
    size : number of bytes to copy

    @pre
    dstMsg must have valid size (see #message_setSize)

    @memberof Message
*/

TC2API void message_copyToMessage(const Message* dstMsg, cchar* srcAddr,
   ushort msgOffset, ushort size)
{
   char* dstAddr;
   log_func(message_copyToMessage);

   if (!srcAddr)
   {
      log_ferror("source address is null");
      return;
   }

   if (!dstMsg)
   {
      log_ferror("destination Message is null");
      return;
   }

   if (msgOffset >= dstMsg->size)
   {
      log_ferror("invalid message offset: %d", msgOffset);
      return;
   }

   if (size > (dstMsg->size - msgOffset))
   {
      log_ferror("invalid copy size: %d", size);
      return;
   }

   dstAddr = (char*)(dstMsg->body) + msgOffset;
   memcpy(dstAddr, srcAddr, size);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Creates a Message structure.

    @param [in]
    flags : flags to be applied to the Message

    @return
    address of the Message structure just created

    @memberof Message
*/

TC2API Message* message_create(ushort flags)
{
   Message* message;

   log_func(message_create);
   log_finfo("creating a message with flags %04X", flags);

   message= (Message*) calloc(1, sizeof(Message)+maxBodySize+1);
   // +maxBodySize: accounts for the body data area
   // +1:           accounts for message suffix
   assert(message);

   message->flags |= flags;
   message->prefix = MSG_PREFIX;

   // not yet known!
   // *(message->body + message->size) = MSG_SUFFIX;

   return message;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Fills message body with a character.

    Usage: @code
    message_fill(dstMsg, ch, msgOffset, size);
    @endcode

    @param [out]
    dstMsg : destination Message

    @param [in]
    ch : character to be used for filling

    @param [in]
    msgOffset : offset into destination Message buffer where to begin fill

    @param [in]
    size : number of bytes to fill

    @remark like <i>memset</i>

    @pre
    dstMsg must have valid size (see #message_setSize)

    @memberof Message
*/

TC2API void message_fill(const Message* dstMsg, char ch, ushort msgOffset,
   ushort size)
{
   char* dstAddr;
   log_func(message_fill);

   if (!dstMsg)
   {
      log_ferror("destination message is null");
      return;
   }

   if (msgOffset >= dstMsg->size)
   {
      log_ferror("invalid destination message offset: %d", msgOffset);
      return;
   }

   if (size > (dstMsg->size - msgOffset))
   {
      log_ferror("invalid compare fill: %d", size);
      return;
   }

   dstAddr = (char*)(dstMsg->body) + msgOffset;
   memset(dstAddr, ch, size);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the sequence number of the Message that was set by the client

    @param [in]
    message : the Message that is being queried

    @return
    the client-originated sequence number of the Message

    @memberof Message
*/

TC2API ushort message_orgSeqNo(const Message* message)
{
   assert(message);
   return message->orgSeqNo;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Destroys a Message structure.

    @param
    message : the Message that is being destroyed

    @memberof Message
*/

TC2API void message_destroy(Message* message)
{
   log_func(message_destroy);
   log_finfo("destroying a message");

   assert(message);
   free(message);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the message flags.

    @param [in]
    message : the Message that is being queried

    @return
    the Message flags

    @memberof Message
*/

TC2API ushort message_flags(const Message* message)
{
   assert(message);
   return message->flags;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the IP address associated with this Message.

    @param [in]
    message : the Message that is being queried

    @return
    the IP address associated with this Message

    @memberof Message
*/

TC2API uint message_ipAddr(const Message* message)
{
   assert(message);
   return message->ipAddr;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Checks if the Message prefix is valid.

    @param [in]
    message : the Message for which the prefix is being checked

    @return
    true (prefix is valid) /false (prefix is not valid)

    @memberof Message
*/

TC2API uint message_isValidPrefix(const Message* message)
{
   assert(message);
   return (message->prefix == MSG_PREFIX);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Checks if the Message suffix is valid.

    @param [in]
    message : the Message for which the suffix is being checked

    @return
    true (suffix is valid) / false (suffix is not valid)

    @memberof Message
*/

TC2API uint message_isValidSuffix(const Message* message)
{
   assert(message);
   return ((*(message->body + message->size)) == MSG_SUFFIX);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the value configured for the maximum data area size of a message.

    @return
    the maximum data area size of a Message

    @memberof Message
*/

TC2API ushort message_maxBodySize(void)
{
   return maxBodySize;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the Message prefix.

    @param [in]
    message : the Message that is being queried

    @return
    the Message prefix

    @memberof Message
*/

TC2API char message_prefix(const Message* message)
{
   assert(message);
   return message->prefix;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the sequence number of this Message.

    @param [in]
    message : the Message that is being queried

    @return
    the sequence number of the Message

    @memberof Message
*/

TC2API ushort message_seqNo(const Message* message)
{
   assert(message);
   return message->seqNo;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Sets one byte in the message body with a byte from the application.

    @param [out]
    dstMsg : destination Message

    @param [in]
    msgOffset : offset into destination Message buffer where to copy byte

    @param [in]
    byte : byte being copy into the message

    @remark like msg[offset] = byte;

    @pre
    dstMsg must have valid size (see #message_setSize)

    @memberof Message
*/

TC2API void message_setByte(const Message* dstMsg, ushort msgOffset, uchar byte)
{
   log_func(message_setByte);

   if (!dstMsg)
   {
      log_ferror("destination message is null");
      return;
   }

   if (msgOffset >= dstMsg->size)
   {
      log_ferror("invalid message offset: %d", msgOffset);
      return;
   }

   *(uchar*)((dstMsg->body) + msgOffset) = byte;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Sets the local sequence number of this Message.

    @param
    message : the Message that is being modified

    @param [in]
    orgSeqNo : the local sequence number of this Message

    @memberof Message
*/

TC2API void message_setOrgSeqNo(Message* message, ushort orgSeqNo)
{
   assert(message);
   message->orgSeqNo = orgSeqNo;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Configures the maximum size of the data area for all @link Message Messages
    @endlink.

    @param [in]
    size : the maximum size allowed for the data area when creating a Message.

    @memberof Message
*/

TC2API void message_setMaxSize(uint size)
{
   if (size)
      maxBodySize = size;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Sets the sequence number of this Message within its Connection.

    @param
    message : the Message that is being modified

    @param [in]
    seqNo : the sequence number assigned to this Message

    @memberof Message
*/

TC2API void message_setSeqNo(Message* message, ushort seqNo)
{
   assert(message);
   message->seqNo = seqNo;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Sets the size of the Message data area.

    @param
    message : the Message that is being modified

    @param [in]
    size : size to be used for the data area of this Message

    @memberof Message
*/

TC2API void message_setSize(const Message* message, ushort size)
{
   log_func(message_setSize);

   if (!message)
   {
      log_ferror("message is null!");
      return;
   }

   if (message->prefix != MSG_PREFIX)
   {
      log_ferror("message with invalid prefix %02X!", (uchar)(message->prefix));
      return;
   }

   if (size > maxBodySize)
   {
      log_ferror("invalid size %d!", size);
      size = maxBodySize;
   }

   ((Message*)message)->size = size;
   *(char*)(message->body + size) = MSG_SUFFIX;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Sets the source identification of this Message

    @param
    message : the Message that is being modified

    @param [in]
    orgId : the source identification of this Message

    @memberof Message
*/

TC2API void message_setOrgId(Message* message, uint orgId)
{
   assert(message);
   message->orgId = orgId;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the size of the data area of this Message.

    @param [in]
    message : the Message that is being queried

    @return
    the size of the data area of this Message

    @memberof Message
*/

TC2API uint message_size(const Message* message)
{
   assert(message);
   return message->size;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the size of the part 1 of a Message being received.

    @return
    the size of the part 1 of the Message being received (constant)

    @remark
    this size is the same for all @link Message Messages @endlink

    @memberof Message
*/

TC2API uint message_size1(void)
{
   return MSG_SIZE_PART1;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the size of the part 2 of a Message being received.

    @param [in]
    message : the Message being received

    @return
    the size of the part 2 of the Message being received

    @memberof Message
*/

TC2API uint message_size2(const Message* message)
{
   assert(message);
   return MSG_SIZE_PART2(message);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the source identification of this Message.

    @param [in]
    message : the Message that is being queried

    @return
    the source identification

    @memberof Message
*/

TC2API uint message_orgId(const Message* message)
{
   assert(message);
   return message->orgId;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the address of on-the-wire contents of this Message.

    @param [in]
    message : the Message that is being queried

    @return
    address of the first byte of the Message which is sent on the wire

    @memberof Message
*/

TC2API char* message_start(Message* message)
{
   assert(message);
   return &message->prefix;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrives the message suffix.

    @param [in]
    message : the Message that is being queried

    @return
    the message suffix

    @memberof Message
*/

TC2API char message_suffix(Message* message)
{
   assert(message);
   return  (*(message->body + message->size)); 
}

// -----------------------------------------------------------------------------
// the end
