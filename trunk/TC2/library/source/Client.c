/// @source      Client.c
/// @description Implementation of class Client.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------

//  Application Description

/** @mainpage A C-Language Library for Writing Generic TCP %Client Applications

    A library for writing simple TCP client applications, written in the
    C-language with an object oriented approach, using the Berkeley sockets
    interface, and intended to be cross-platform.  @par

    A <b> generic client </b> written with this library is a simple TCP client
    application created following a specific pattern that is built around three
    concepts: <b> state</b>, <b> event</b> and <b> command</b>. @par

    <b> State </b> is one of these     <br>
       (1) <b> CONNECTED_IDLE </b>     <br>
       (2) <b> NOT_CONNECTED </b>.     @par

    <b> Command </b> is one of these:  <br>
       (1) <b>CONNECT</b>              <br>
       (2) <b>SEND</b>                 <br>
       (3) <b>RECV</b>                 <br>
       (4) <b>CLOSE</b>.               @par

    <b>Event</b> is one of these:         <br>
       (1) <b>CONNECTION_CREATED</b>      <br>
       (2) <b>CONNECTION_DESTROYED</b>    <br>
       (3) <b>CONNECT_ERROR</b>           <br>
       (4) <b>RECV_COMPLETE</b>           <br>
       (5) <b>RECV_TIMEOUT</b>            <br>
       (6) <b>SEND_COMPLETE</b>.          @par

    There are also two other entities involved: an instance of the GenericClient
    class, provided by the library, and the <b>application code</b> written by
    the user of the library. The GenericClient class is informally derived from
    the Client class, which provides the basic methods for writing client
    applications. @par

    The general flow of control in an application written with this library is
    like this: <br>
       (1) the <b> application code </b> passes up a <b> command</b> to
           the GenericClient instance, <br>
       (2) which processes the received <b> command</b> and passes down an
           <b>event</b> to the <b> application code</b>; <br>
       (3) the <b> application code </b> then processes the <b> event </b>
           received from the GenericClient instance, and the cycle 
           repeats again. @par

    The GenericClient instance is driven by a state machine, which is described 
    below in pseudo-code:

    @code

    1. if state is NOT_CONNECTED

          // only accepts the CONNECT command
          do connect

          if result is OK
             set state to CONNECTED_IDLE
             return event CONNECTION_CREATED
          else
             // remains in the state NOT_CONNECTED
             return event CONNECT_ERROR

    2. if state is CONNECTED_IDLE

          2.1 if command is SEND
              do send
              if result is OK
                 // remains in the state CONNECTED_IDLE
                 return event SEND_COMPLETE
              else
                set state to NOT_CONNECTED
                return event CONNECTION_DESTROYED

          2.2 if command is RECV
              do recv
              if result is OK
                 // remains in the state CONNECTED_IDLE
                 return event RECV_COMPLETE
              else if occurred TIMEOUT
                 // remains in the state CONNECTED_IDLE
                 return event RECV_TIMEOUT
              else // error
                 set state to NOT_CONNECTED
                 return event CONNECTION_DESTROYED

          2.3 if command is CLOSE
              do close
              set state to NOT_CONNECTED
              return event CONNECTION_DESTROYED
    @endcode

    The <b> application code</b> doesn't have to be concerned with how the
    GenericClient instance works; its only concern is how to handle the events
    it receives from the GenericClient instance. @par

    The pseudo-code for the <b>application code</b> is open, but generally will
    be like the following:

    @verbatim

    1. if event is CONNECTION_CREATED
          prepare message to send to the server
          pass the command SEND to the library

    2. if event is SEND_COMPLETE
          update whatever controls needed by the application
          pass the command RECV to the library

    3. if event is RECV_COMPLETE
          process reply received from the server
          prepare another message to send to the server
          pass the command SEND to the library

    4. if event is RECV_TIMEOUT (*** optional ***)
          do whatever the application needs in case of timeout
          if business rules says try again
             prepare another message to send to the server
             pass the command SEND to the library
          else
             pass the command CLOSE to the library

    5. if event is CONNECTION_DESTROYED
          // can be the result of SEND, RECV or CLOSE commands
          // (most errors will automatically destroy the connection)
          do whatever cleanup the application needs
          pass the command CONNECT to the library

    6. if event is CONNECT_ERROR
          do whatever the application needs in this case
          if business rules says try again
             pass the command CONNECT to the library
          else
             end application

    @endverbatim

    Using the pattern described above, writing simple TCP client applications
    is a snap, because nowhere the TCP/IP networking code is seen. In practice,
    only the application code needs to be written. @par

    The goal here is not writing production-grade applications, but small
    utilities for testing scenarios, for prototyping new functionalities, for
    finding application bugs in servers, etc. In these cases, speed of
    development is of paramount importance, because often the programs created
    are discardable, throwaway utilities, with limited scope and functionality,
    which do no justify spending too much time in their development. @par


    The C code that implements the pseudo-code above is something like this:
    @code

    switch (genCli_waitEvent())
    {
       case CLI_EVT_CONNECTION_CREATED:
          prepareFirstMessage();
          genCli_send();
          break;

       case CLI_EVT_RECV_COMPLETE:
          processServerReply();
          prepareAnotherMessage();
          genCli_send();
          break;

       case CLI_EVT_SEND_COMPLETE:
          genCli_recv();
          break;

       case CLI_EVT_RECV_TIMEOUT:
          prepareAnotherMessage();
          genCli_send();
          break;

       case CLI_EVT_CONNECT_ERROR:
          printConnectionError();
          client_sleep(5);
          genCli_connect();
          break;

       case CLI_EVT_CONNECTION_DESTROYED:
          printOperationError();
          genCli_connect();
          break;

       default:
          printf("*invalid event %d\n", genCli_event());
          abort();
          break;
    } // switch

    @endcode

    Please refer to the <b>gen_client_1</b> example project for more details.
    @par

    The library also provides additional funcionality, presented by the Client
    class but in fact implemented by other classes (the Client class here acting
    as a façade). @par

    The most important of the additional functionality is related to the Message
    class. It provides an encapsulation for the buffers used when exchanging
    messages between client and server, and also provides the framing that
    delimits messages on the wire. Please see the documentation for the Message
    class, for the Client class, and the <b>gen_client_1</b> example that is
    shipped with this project. For the record, the more important methods
    provided by the Message class for the use of applications are
    (encapsulated by the Client class):
       @link Client::client_messageBuffer client_messageBuffer @endlink,
       @link Client::client_messageSize client_messageSize @endlink, and
       @link Client::client_setMessageSize client_setMessageSize @endlink.
       @par

    There is also a Log class, that is used internally by the library but
    can also be used by applications. Its main methods are (again,
    encapsulated by the Client class):
       @link Client::client_logInfo  client_logInfo @endlink,
       @link Client::client_logWarn  client_logWarn @endlink,
       @link Client::client_logDebug client_logDebug @endlink,
       @link Client::client_logTrace client_logTrace @endlink,
       @link Client::client_logError client_logError @endlink and
       @link Client::client_logFatal client_logFatal @endlink.
       @par

    There are some other classes used internally by the library, that may or
    may not be useful when writing the client applications: Mutex, Thread, Time
    and Timeout.

    This library is written to inter-operate with servers written using the
    library described in this
    <a href="http://www.codeproject.com/KB/IP/TCPServerFramework.aspx">
    article</a>. Specifically, the <b>gen_client_1</b> example shipped
    with this project works with the <b> server_2 </b> and <b> server_3 </b>
    examples shipped with the article above.

    To adapt the library to work with other servers it's very likely that the
    Message structure declared in the MessageImpl.h file will have to be 
    changed. Specifically, refer to the on-the-wire format of the messages
    exchanged between the client and the server.
*/

// -----------------------------------------------------------------------------

// includes

// common configuration options & declarations
#include "config.h" // always include first

// C language includes
#include <assert.h>
#include <stdlib.h> // abort
#include <string.h>

// Windows-specific includes
#if PLATFORM(Windows)
#pragma warning(disable: 4115)
#include <windows.h>
#endif

// Linux-specific includes
#if PLATFORM(Linux)
#include <arpa/inet.h>  // inet_ntoa
 #endif

// framework includes
#include "Client.h"        /* client_xxx functions  */
#include "Socket.h"        /* socket_xxx functions  */
#include "util/Log.h"      /* log_xxx functions     */
#include "util/Timeout.h"  /* timeout_xxx functions */

// -----------------------------------------------------------------------------

// global declarations

/// Client class.
/// This class provides convenience members and methods to write simple client
/// applications.
/// @class Client

/// Identification for the Log file
/// @private @memberof Client
static cchar sourceID[] = "ZCL";

/// Global counter used when sending messages
/// @private @memberof Client
static uint orgSeqNo;

/// Default server IP address in binary (127.1)
/// @private @memberof Client
static uint serverAddress = 0x7F000001;

/// Default server IP address as string
/// @private @memberof Client
static char serverAddressStr[50] = "127.1";

/// Default server TCP port
/// @private @memberof Client
static ushort servicePort = 4000;

/// The client socket
/// @private @memberof Client
static int clientSocket;

/// Default server reply timeout in seconds.
/// (0 means immediate, -1 means infinite).
/// @private @memberof Client
static uint replyTimeout = TIMEOUT_WAIT_FOREVER;

/// Default timeout in seconds when sending a message to the server.
/// In principle the client doesn't need to mess with this, but it's here anyway
/// @private @memberof Client
static ushort sendTimeout = 5;

/// State of the connection
/// @private @memberof Client
static bool isConnected;

// ---------------

// State of the client, valid only immediately after a socket operation

/// Tells if the connection has been closed after an operation end in error.
/// @private @memberof Client
static bool connectionClosed;  

/// Tells if there has been a format error on the wire right after a receive
/// operation.
/// @private @memberof Client
static bool formatError;

/// Contains the number of the error that occurred immediately after a socket
/// operation.                                                                     
/// @private @memberof Client
static int lastSocketError;

/// Contains the type of the error that occurred immediately after a socket
/// operation. 
/// @private @memberof Client
static int lastErrorType;

// private interface with GenericClient (sync if changed in either place)
// see also client_errorType
enum // for lastErrorType
{
   ERRTYPE_CLOSED = 1  ,  // connection closed by server
   ERRTYPE_SOCKET      ,  // socket error
   ERRTYPE_FORMAT      ,  // invalid format on the wire
   ERRTYPE_SENDTIMEOUT ,  // timeout in send
};

/// Tells id there has been a timeout after a receive operations ends in
/// error.
/// @private @memberof Client
static bool timeoutOccurred;

// ---------------

/// Tells if tracing is enabled.
/// @private @memberof Client
static bool isTracing;

// ---------------

// internal helper functions
/// @cond hides_from_doxygen
static int recvBuffer(int, char*, int, uint, bool);
/// @endcond

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Closes the connection.

    @memberof Client
*/

TC2API void client_closeConnection(void)
{
   log_func(client_closeConnection);
   log_finfo("closing the connection with the server");

   socket_close(clientSocket);
   isConnected = false;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Tries to connect with the server.

    @memberof Client
*/

TC2API bool client_connect(void)
{
   log_func(client_connect);
   log_finfo("trying to connect with the server %s", serverAddressStr);

   isConnected = false;

   if (socket_connect(clientSocket, serverAddress, servicePort))
   {
      log_finfo("ok, connection successful");
      isConnected = true;
      return true;
   }

   lastSocketError = socket_error();

   log_ferror("error %d trying to connect with the server", lastSocketError);
   return false;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Tells if the connection has been closed right after an operation
    (send/receive) ends in error.

    @return
    true (connection has been closed) / false (connection has not been closed)

    @memberof Client
*/

TC2API bool client_connectionClosed(void)
{
   return connectionClosed;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Creates the client socket.

    @post
    either succeeds or aborts the application

    @memberof Client
*/

TC2API void client_createClientSocket(void)
{
   log_func(client_createClientSocket);
   log_finfo("creating the client socket");

   clientSocket = socket_createClientSocket();

// don't do it for now 
// socket_setNonBlocking(clientSocket);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the value of the error that occurred immediately after a socket
    operation.

    @return
    number of error that occurred immediately after a socket operation

    @memberof Client
*/

TC2API uint client_error(void)
{
   return lastSocketError;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the type of the error that occurred immediately after a socket
    operation.

    @return
    type of the error (only for use of the class GenericClient)

    @memberof Client
*/

TC2API ushort client_errorType(void)
{
   return lastErrorType;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Reports if there has been a format error on the wire right after a receive
    operation.

    @return
    true (a format error occurred) / false (a format error not occurred)

    @memberof Client
*/

TC2API bool client_formatError(void)
{
   return formatError;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Initializes the Client façade class.

    @memberof Client
*/

TC2API void client_init(void)
{
   log_func(client_init);
   log_finfo("Client class inicialization");

   socket_init();
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Checks if the client is connected to the server.

    @return
    true (is connected) / false (is not connected)

    @memberof Client
*/

TC2API bool client_isConnected(void)
{
   return isConnected;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Receives a message from the server.

    @param
    msg : address of a Message structure which will be used to store data
    received from the server

    @return
    true (reception successful) / false (reception failure)

    @memberof Client
*/

TC2API bool client_recvMessage(Message* msg)
{
   int ret;
   Timeout timeout;
   log_func(client_recvMessage);
   static cchar msgclose[] = "now closing connection";

   /// @cond hides_from_doxygen
   #define REM_TIME timeout_remaining(&timeout)
   /// @endcond

   assert(msg);

   timeout_init(&timeout, replyTimeout);

   // tries to receive the message's first part
   log_fdebug("trying to receive %d bytes (first part of message)",
      message_size1());

   ret = recvBuffer(clientSocket, message_start(msg), message_size1(),
      REM_TIME, true /*first part of message*/);

   if (ret != (int)message_size1())
   {
      log_ferror("error when receiving first part of message from server");

      // if timeout and not one byte has arrived then don't close connection
      if (timeoutOccurred && (lastErrorType != ERRTYPE_FORMAT))
         return false;
      goto ERROR_WITH_CLOSE;
   }

   // consistency check
   if (!message_isValidPrefix(msg))
   {
      log_ferror("invalid message prefix [%c] received from server",
         message_prefix(msg));
      goto FORMAT_ERROR_WITH_CLOSE;
   }

   // consistency check
   if (message_size(msg) > message_maxBodySize())
   {
      log_ferror("invalid message size [%d] received from server",
          message_size(msg));
      goto FORMAT_ERROR_WITH_CLOSE;
   }

   // tries to receive the message's second part
   log_fdebug("trying to receive %d bytes (second part of message)",
      message_size2(msg));

   ret = recvBuffer(clientSocket, message_start(msg)+message_size1(),
      message_size2(msg), REM_TIME, false /*not first part of message*/);

   if (ret != (int)message_size2(msg))
   {
      log_ferror("error when receiving seconf part of message from server");
      goto ERROR_WITH_CLOSE;
   }

   // consistency check
   if (!message_isValidSuffix(msg))
   {
      log_ferror("invalid message suffix [%c] received from server",
         message_suffix(msg));
      goto FORMAT_ERROR_WITH_CLOSE;
   }

   log_fdebug("received %d bytes from server, body:%d orgId:%d orgSeq:%d",
      message_size1()+message_size2(msg), message_size(msg),
      message_orgId(msg), message_orgSeqNo(msg));

   return true;

FORMAT_ERROR_WITH_CLOSE:
   formatError = true;
   lastErrorType = ERRTYPE_FORMAT;
ERROR_WITH_CLOSE:
   log_fwarn(msgclose);
   client_closeConnection();
   return false;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Tries to send a message to the server.

    @param
    msg : address of a Message structure with the data that will be sent to the
    server

    @return
    true (operation successful) / false (operation failed)

    @memberof Client
*/

TC2API bool client_sendMessage(Message* msg)
{
   int ret, size;
   log_func(client_sendMessage);

   assert(msg);

   // to make it easier to find this message in the server's log
   message_setOrgId(msg, thread_pid());
   message_setOrgSeqNo(msg, orgSeqNo++);

   size = message_size1() + message_size2(msg);

   // clears indicators
   lastSocketError = 0;
   lastErrorType = 0;
   timeoutOccurred = false;

   // tries to send the message
   log_fdebug("sending %d bytes to server, body:%d orgId:%d orgSeq:%d",
      size, message_size(msg), message_orgId(msg), message_orgSeqNo(msg));

   ret = socket_sendBuffer(clientSocket, message_start(msg), size, sendTimeout,
      isTracing);

   if (ret == size)
   {
      log_fdebug("ok, message sent successfully");
      return true; // ok
   }

   if (ret == E_SOCKET_TIMEOUT)
   {
      log_fwarn("timeout occurred when writing socket");
      timeoutOccurred = true;
      lastErrorType = ERRTYPE_SENDTIMEOUT;
   }

   else // if (ret == SOCKET_ERROR)
   {
      lastSocketError = socket_error();
      log_ferror("error %d when writing socket", lastSocketError);
      lastErrorType = ERRTYPE_SOCKET;
   }

   log_fwarn("now closing connection");
   client_closeConnection();

   return false;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Configures how many seconds to wait for a reply from the server.

    @param [in]
    seconds : how many seconds to wait for a reply from the server.

    @memberof Client
*/

TC2API void client_setReplyTimeout(ushort seconds)
{
   log_func(client_setReplyTimeout);
   log_finfo("reply timeout: %d seconds", seconds);

   replyTimeout = seconds;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Configures the server IP address.

    @param
    ipAddress : IP address of the server

    @memberof Client
*/

TC2API void client_setServerAddress(cchar* ipAddress)
{
   log_func(client_setServerAddress);
   log_finfo("using server address: %s", ipAddress);

   strncpy(serverAddressStr, ipAddress, sizeof(serverAddressStr)-1);
   serverAddress = inet_addr(serverAddressStr);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Configures the server TCP port.

    @param
    port : service port on which the server is waiting.

    @memberof Client
*/

TC2API void client_setServicePort(ushort port)
{
   log_func(client_setServicePort);
   log_finfo("using port: %d", port);

   servicePort = port;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Enables or disables tracing.

    @param
    flag : true/false

    @memberof Client
*/

TC2API void client_setTrace(bool flag)
{
   isTracing = flag;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Reports if there has been a timeout after a receive operations ends in
    error.

    @return
    true (timeout occurred) / false (timeout not occurred)

    @memberof Client
*/

TC2API bool client_timeout(void)
{
   return (timeoutOccurred && (lastErrorType == ERRTYPE_FORMAT));
}

// -----------------------------------------------------------------------------
// INTERNAL HELPER FUNCTIONS
// -----------------------------------------------------------------------------

/** Receives a specific-length buffer from peer application, and checks for
    error.

    @return
    number of bytes received, or error

    @retval
    the number of bytes received, or error

    @retval
     0 : (SOCKET_CLOSED) connection was closed by the server.

    @retval
    -1 : (SOCKET_ERROR) an error occurred.

    @retval
    -2 : (SOCKET_TIMEOUT) not all bytes arrived within the specified time
    interval.

    @private @memberof Client
*/

static int recvBuffer(int socket, char* buf, int len, unsigned nSeconds,
   bool firstPart)
{
   int ret;
   log_func(recvBuffer);

   // clears indicators
   connectionClosed = false;
   formatError = false;
   lastSocketError = 0;
   lastErrorType = 0;
   timeoutOccurred = false;

   ret = socket_recvBuffer(socket, buf, len, nSeconds, isTracing);

   if (ret == len) // most likely result
      goto END;

   if (ret == E_SOCKET_CLOSED) // second most likely result
   {
      log_fwarn("connection closed when reading socket");
      connectionClosed = true;
      lastErrorType = ERRTYPE_CLOSED;
      goto END;
   }

   if (ret == E_SOCKET_TIMEOUT) // third most likely result
   {
      log_fwarn("timeout occurred when reading socket");
      // timeout not considered error if not one byte received,
      // only considered error if occurs int the middle of a message
      if (!firstPart || socket_partialLenReceived())
         lastErrorType = ERRTYPE_FORMAT;
      timeoutOccurred = true;
      goto END;
   }

   if (ret == E_SOCKET_ERROR) // fourth most likely result
   {
      lastSocketError = socket_error();
      log_ferror("error %d when reading socket", lastSocketError);
      lastErrorType = ERRTYPE_SOCKET;
      goto END;
   }

   // should not happen (!)
   abort();

END:
   return ret;
}

// -----------------------------------------------------------------------------
// the end
