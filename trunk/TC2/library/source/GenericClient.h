/// @source      GenericClient.h
/// @description Public interface for class GenericClient.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------
#ifndef __GENERIC_CLIENT_H__
#define __GENERIC_CLIENT_H__
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// -----------------------------------------------------------------------------

// // GenericClient opaque type.
// typedef struct GenericClient GenericClient;

// the redundancy here is for doxygen documentation

/// see GenericClient::genCli_close
extern TC2API void genCli_close(void);

/// see Client::genCli_connect
extern TC2API void genCli_connect(void);

/// see Client::genCli_error
extern TC2API ushort genCli_error(void);

/// see Client::genCli_errorType
extern TC2API ushort genCli_errorType(void);

/// see Client::genCli_event
extern TC2API ushort genCli_event(void);

/// see Client::genCli_init
extern TC2API void genCli_init(void);

/// see Client::genCli_message
extern TC2API Message* genCli_message(void);

/// see Client::genCli_operation
extern TC2API cchar* genCli_operation(void);

/// see Client::genCli_recv
extern TC2API void genCli_recv(void);

/// see Client::genCli_send
extern TC2API void genCli_send(void);

/// see Client::genCli_setRetryConnect
extern TC2API void genCli_setRetryConnect(bool);

/// see Client::genCli_waitEvent
extern TC2API ushort genCli_waitEvent(void);

/// see Client::ClientEventId
enum ClientEventId
{
   CLI_EVT_CONNECTION_CREATED   , // connection has been created
   CLI_EVT_CONNECTION_DESTROYED , // connection has been destroyed
   CLI_EVT_CONNECT_ERROR        , // error when trying to connect
   CLI_EVT_RECV_COMPLETE        , // a complete message ahs arrived
   CLI_EVT_RECV_TIMEOUT         , // not one byte of a message arrived
   CLI_EVT_SEND_COMPLETE        , // a message was successfully sent
   CLI_EVT_INVALID_COMMAND      , // command (send, etc) invalid in the context
};

/// see Client::ClientErrorType
enum ClientErrorType
{
   CLI_ERR_CONNECTION_CLOSED = 1 ,  // connection closed by server
   CLI_ERR_SOCKET_ERROR          ,  // socket error
   CLI_ERR_FORMAT_ERROR          ,  // invalid format on the wire
   CLI_ERR_TIMEOUT_SEND          ,  // server has froze, got stuck, etc
   CLI_ERR_FORCED_CLOSE          ,  // connection closed by application
};

// -----------------------------------------------------------------------------
#ifdef __cplusplus
} // #ifdef __cplusplus
#endif
// -----------------------------------------------------------------------------
#endif // __GENERIC_CLIENT_H__
// -----------------------------------------------------------------------------
