/// @source      Message.h
/// @description Public interface for class Message.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------
#ifndef __MESSAGE_H__
#define __MESSAGE_H__
// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// -----------------------------------------------------------------------------

// Message opaque type.
typedef struct Message Message;

// the redundancy here is for doxygen documentation

// constructor
//  ~~~~~~~~~~~

/// see Message::message_create
extern TC2API Message* message_create(ushort);

// destructor
//  ~~~~~~~~~

/// see Message::message_destroy
extern TC2API void message_destroy(Message*);

// query

/// see Message::message_flags
extern TC2API ushort message_flags(const Message*);

/// see Message::message_ipAddr
extern TC2API uint   message_ipAddr(const Message*);

/// see Message::message_isValidPrefix
extern TC2API bool   message_isValidPrefix(const Message*);

/// see Message::message_isValidSuffix
extern TC2API bool   message_isValidSuffix(const Message*);

/// see Message::message_maxBodySize
extern TC2API ushort message_maxBodySize(void);

/// see Message::message_prefix
extern TC2API char   message_prefix(const Message*);

/// see Message::message_seqNo
extern TC2API ushort message_seqNo(const Message*);

/// see Message::message_orgId
extern TC2API uint   message_orgId(const Message*);

/// see Message::message_orgSeqNo
extern TC2API ushort message_orgSeqNo(const Message*);

/// see Message::message_size
extern TC2API uint   message_size(const Message*);

/// see Message::message_size1
extern TC2API uint   message_size1(void);

/// see Message::message_size2
extern TC2API uint   message_size2(const Message*);

/// see Message::message_suffix
extern TC2API char   message_suffix(Message*);

// configuration
/// see Message::message_setMaxSize
extern TC2API void message_setMaxSize(uint);

// mutators
// ~~~~~~~~

/// see Message::message_setOrgId
extern TC2API void message_setOrgId(Message* dst, uint orgId);

/// see Message::message_setOrgSeqNo
extern TC2API void message_setOrgSeqNo(Message* dst, ushort orgSeqNo);

/// see Message::message_setSeqNo
extern TC2API void message_setSeqNo(Message* dst, ushort seqNo);

/// see Message::message_setSize
extern TC2API void message_setSize(const Message*, ushort size);

// accessors
// ~~~~~~~~~

/// see Message::message_buffer
extern TC2API char* message_buffer(Message*); // address of application data 

/// see Message::message_start
extern TC2API char* message_start(Message*); // address of on-the-wire data

// buffer operations
// ~~~~~~~~~~~~~~~~~

/// see Message::message_byte
extern TC2API uchar message_byte(const Message*, ushort msgOffset);

/// see Message::message_cloneFields.
/// One field: offset, size.
extern TC2API void message_cloneFields(Message* dst, const Message* src,
   ushort nFields, ... );

/// see Message::message_compareToMessage
extern TC2API int message_compareToMessage(cchar* src1, const Message* src2,
   ushort msgOffset, ushort size);

/// see Message::message_copyFields.
/// One field: dstOffset, srcOffset, size.
extern TC2API void message_copyFields(Message* dst, const Message* src,
   ushort nFields, ... );

/// see Message::message_copyFromMessage
extern TC2API void message_copyFromMessage(char* dst, const Message* src,
   ushort msgOffset, ushort size);

/// see Message::message_copyToMessage
extern TC2API void message_copyToMessage(const Message* dst, cchar* src,
   ushort msgOffset, ushort size);

/// see Message::message_fill
extern TC2API void message_fill(const Message* dst, char fill,
   ushort msgOffset, ushort size);

/// see Message::message_setByte
extern TC2API void message_setByte(const Message* dst, ushort msgOffset,
   uchar byte);

// -----------------------------------------------------------------------------
#ifdef __cplusplus
} // #ifdef __cplusplus
#endif
// -----------------------------------------------------------------------------
#endif // __MESSAGE_H__
// -----------------------------------------------------------------------------
