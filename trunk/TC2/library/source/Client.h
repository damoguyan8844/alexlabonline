/// @source      Client.h
/// @description Public interface for class Client.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------
#ifndef __CLIENT_H__
#define __CLIENT_H__
// -----------------------------------------------------------------------------

// includes

#if PLATFORM(Windows)
#include <windows.h>
#endif

#if PLATFORM(Linux)
#include <unistd.h> // for convenience (provides sleep)
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// -----------------------------------------------------------------------------

// framework includes
#include "Message.h"      /* message_xxx functions */
#include "util/Log.h"     /* log_xxx functions     */
#include "util/Time.h"    /* time_xxx functions    */
#include "util/Thread.h"  /* thread_xxx functions  */

// -----------------------------------------------------------------------------

// Client opaque type.
typedef struct Client Client;

// the redundancy here is for doxygen documentation

// basic operations
// ~~~~~~~~~~~~~~~~

/// see Client::client_init
extern TC2API void client_init(void);

// configuration
// ~~~~~~~~~~~~~

/// see Client::client_setReplyTimeout
extern TC2API void client_setReplyTimeout(ushort);

/// see Client::client_setServerAddress
extern TC2API void client_setServerAddress(cchar*);

/// see Client::client_setServicePort
extern TC2API void client_setServicePort(ushort);

// Socket support
// ~~~~~~~~~~~~~~

/// see Client::client_connect
extern TC2API bool client_connect(void);

/// see Client::closeConnection
extern TC2API void client_closeConnection(void);

/// see Client::connectionClosed
extern TC2API bool client_connectionClosed(void);

/// see Client::client_createClientSocket
extern TC2API void client_createClientSocket(void);

/// see Client::client_error
extern TC2API uint client_error(void);

/// see Client::client_errorType
extern TC2API ushort client_errorType(void); // for GenericClient

/// see Client::client_formatError
extern TC2API uint client_formatError(void);

/// see Client::client_timeout
extern TC2API bool client_timeout(void);

/// see Client::client_isConnected
extern TC2API bool client_isConnected(void);

/// see Client::client_recvMessage
extern TC2API bool client_recvMessage(Message*);

/// see Client::client_sendMessage
extern TC2API bool client_sendMessage(Message*);

/// see Client::client_setTrace
extern TC2API void client_setTrace(bool);

// class Client is also a small "façade" for the framework/library
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// what goes here is more or less discretionary

// Version of the framework
// ~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief version of the framework
#define client_getVersion() TC2_VERSION

// Message support
// ~~~~~~~~~~~~~~~

/// see Message::message_cloneFields
/// One field: offset, size
#define client_cloneMessageFields message_cloneFields

/// see Message::message_compareToMessage
#define client_compareToMessage message_compareToMessage

/// see Message::message_copyFields.
/// One field: dstOffset, srcOffset, size.
#define client_copyMessageFields message_copyFields

/// see Message::message_copyFromMessage
#define client_copyFromMessage message_copyFromMessage

/// see Message::message_copyToMessage
#define client_copyToMessage message_copyToMessage

/// see Message::message_create
#define client_createMessage()    message_create(0)

/// see Message::message_byte
#define client_messageByte        message_byte

/// see Message::message_fill
#define client_messageFill message_fill

/// see Message::message_maxBodySize
#define client_maxMessageSize     message_maxBodySize

/// see Message::message_buffer
#define client_messageBuffer      message_buffer

/// see Message::message_orgId
#define client_messageOrgId       message_orgId

/// see Message::message_orgSeqNo
#define client_messageOrgSeqNo    message_orgSeqNo

/// see Message::message_seqNo
#define client_messageSeqNo       message_seqNo

/// see Message::message_size
#define client_messageSize        message_size

/// see Message::message_setSize
#define client_setMessageSize     message_setSize

/// see Message::message_setOrgId
#define client_setMessageOrgId    message_setOrgId

/// see Message::message_setOrgSeqNo
#define client_setMessageOrgSeqNo message_setOrgSeqNo

/// see Message::message_setByte
#define client_setMessageByte     message_setByte

// Log support
// ~~~~~~~~~~~

/// see Log::log_setLevel
#define client_setLogLevel log_setLevel

/// see Log::log_error
#define client_logError    log_error

/// see Log::log_fatal
#define client_logFatal    log_fatal

/// see Log::log_info
#define client_logInfo     log_info

/// see Log::log_warn
#define client_logWarn     log_warn

/// see Log::log_debug
#define client_logDebug    log_debug

/// see Log::log_func
#define client_logFunc     log_func

/// see Log::log_trace
#define client_logTrace    log_trace

/// see Log::log_setDir
#define client_setLogDir   log_setDir

/// see Log::log_ferror
#define client_fLogError   log_ferror

/// see Log::log_ffatal
#define client_fLogFatal   log_ffatal

/// see Log::log_finfo
#define client_fLogInfo    log_finfo

/// see Log::log_fwarn
#define client_fLogWarn     log_fwarn

/// see Log::log_fdebug
#define client_fLogDebug    log_fdebug

// convenience
// ~~~~~~~~~~~

/// common sleep function
#define client_sleep(x) IF_LINUX(sleep((x))) IF_WINDOWS(Sleep(1000*(x)))

/// see Time::time_formatCurrentTime
#define client_formatCurrentTime time_formatCurrentTime

/// see Thread::thread_pid
#define client_pid thread_pid

// -----------------------------------------------------------------------------
#ifdef __cplusplus
} // #ifdef __cplusplus
#endif
// -----------------------------------------------------------------------------
#endif // __CLIENT_H__
// -----------------------------------------------------------------------------
