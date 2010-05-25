/// @source      Socket.c
/// @description Implementation of the Socket facility.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------

// includes

// common configuration options & declarations
#include "config.h"  // always include first

// C language includes
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>  // abort

// Windows-specific includes
#if PLATFORM(Windows)
#pragma warning(disable: 4115)
#include <windows.h>
#endif

// Linux-specific includes
#if PLATFORM(Linux)
#include <fcntl.h>
#include <arpa/inet.h>  // inet_ntoa
#include <netinet/in.h> // sockets
#include <signal.h>
#endif

// framework includes
#include "Socket.h"        /* socket_xxx functions  */
#include "util/Log.h"      /* log_xxx functions     */
#include "util/Thread.h"   /* thread_xxx functions  */
#include "util/Timeout.h"  /* timeout_xxx functions */

// -----------------------------------------------------------------------------

// global declarations

/// Socket class.
/// A class that provides low level access to the Berkeley sockets used in the
/// framework.
/// @class Socket

/// Source identification for the Log file
/// @private @memberof Socket
static cchar sourceID[] = "ZSO";

/// Convenience macro: length of socket address structure
/// @private @memberof Socket
enum SockAddrLen
{
   SOCKADDR_LEN = sizeof(struct sockaddr)
};

// for use by GenericClient
static int partialLenReceived;

// internal helper function prototypes
/// @cond hides_from_doxygen
 static int  createTCPsocket(void);
static int waitRecvEvent(int, uint);
static int waitSendEvent(int, uint);
/// @endcond

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Connects a client socket with a server.

    @param [in]
    clientSocket : the client socket to be connected

    @param [in]
    serverAddress : IP address of the server

    @param [in]
    port : the port on which the server waits for connection requests

    @return
    true if successful, false if not

    @memberof Socket
*/

TC2API bool socket_connect(int clientSocket, uint serverAddress, ushort port)
{
   struct sockaddr_in sin; // server socket control structure 

   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = serverAddress;
   sin.sin_port = htons(port);

   return (connect(clientSocket, (struct sockaddr*)&sin, sizeof(sin)) == 0);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Creates a client TCP socket.

    @return
    clientSocket : a socket descriptor

    @memberof Socket
*/

TC2API int socket_createClientSocket(void)
{
   int clientSocket;

   log_func(socket_createClientSocket);
   log_finfo("creating a client socket");

   clientSocket = createTCPsocket();

   return clientSocket;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Initializes the Socket class.
    @fn void socket_init(void)

    Also, platform-specific initialization.

    @memberof Socket
*/

#if PLATFORM(Linux)
void socket_init(void)
{
   log_func(socket_init);
   log_finfo("Socket class inicialization");

   // don't let the system abort the application when it tries to send bytes
   // through a connection already closed by the client
   signal(SIGPIPE, SIG_IGN);
}
#endif

#if PLATFORM(Windows)
TC2API void socket_init(void)
{
   WSADATA wsaData;

   log_func(socket_init);
   log_finfo("Socket class inicialization");

   // mumbo-jumbo needed by Windows
   if (WSAStartup(0x202, &wsaData) == E_SOCKET_ERROR)
   {
      // I guess there's nothing more we can do...
      int err = socket_error();
      log_ffatal("error %d in WSAStartup", err);
      WSACleanup();
      abort();
   }
}
#endif

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Retrieves the partial number of bytes received up until a timeout.

    @return
    the number of bytes received up until timeout occured

    @memberof Socket
*/

TC2API int socket_partialLenReceived(void)
{
   return partialLenReceived;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Receives an exact number of bytes from the peer application.

    @param [in]
    socket : socket descritor

    @param [out]
    buf : reception buffer

    @param [in]
    len : number of bytes that <i> must </i> be received in this call

    @param [in]
    nSeconds : number of seconds in which the buffer <i> must </i> arrive

    @param [in]
    trace : whether to write a trace of the received bytes

    @return
    number of bytes received, or 0 if connection closed, or -1 if error
    or -2 if timeout

    @memberof Socket
*/

TC2API int socket_recvBuffer(int socket, char* buf, int len, uint nSeconds,
   bool trace)
{
   Timeout timeout;
   int pendingLen = len;
   int nEvents, receivedLen = 0;

   timeout_init(&timeout, nSeconds);

   socket_setNonBlocking(socket, true);

   partialLenReceived = 0;

   // while there's something to receive
   while (pendingLen > 0)
   {
      // waits for some bytes to arrive
      nEvents = waitRecvEvent(socket, timeout_remaining(&timeout));

      if (nEvents == -1) // error
      {
         receivedLen = -1;
         break;
      }

      if (nEvents == 0)
      {
         receivedLen = E_SOCKET_TIMEOUT; // timeout 
         break;
      }

      // tries to receive the remaining data
      len = recv(socket, buf, pendingLen, 0);

      if (len == 0)
      {
         receivedLen =  0; // connection closed
         break;
      }

      if (len < 0) // error ?
      {
         int err = socket_error();
         if (socket_shouldRetry(err) || socket_wouldBlock(err))
            continue;
         receivedLen =  -1; // error
         break;
      }

      // ok, got "len" bytes
      if (trace)
      {
         ushort port;
         struct sockaddr_in sa;
         socklen_t addrlen = sizeof(sa);
         char bufPort[50];
         getsockname(socket, (struct sockaddr*)&sa, &addrlen);
         port = htons(sa.sin_port);
         sprintf(bufPort, "received in port:%d", port);
         log_trace(bufPort, buf, len);
      }

      partialLenReceived += len; // for use by GenericClient

      pendingLen -= len;   // adjusts pending len
      receivedLen += len;  // adjusts received len so far
      buf += len;          // advances reception buffer

   } // while

   socket_setNonBlocking(socket, false);
   return receivedLen;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Sends  an exact number of bytes to the peer application.

    @param [in]
    socket : socket descritor

    @param [in]
    msgBuf : message buffer

    @param [in]
    msgLen : number of bytes that <i> must </i> be sent in this call

    @param [in]
    nSeconds : number of seconds to conclude the operation

    @param [in]
    trace : whether to write a trace of the bytes sent

    @return
    number of bytes that were sent, or -1 if error, or -2 if timeout

    @memberof Socket
*/

TC2API int socket_sendBuffer(int socket, cchar* msgBuf, int msgLen, uint nSeconds,
   bool trace)
{
   Timeout timeout;
   int pendingLen = msgLen;
   char* buf = (char*)msgBuf;

   timeout_init(&timeout, nSeconds);

   socket_setNonBlocking(socket, true);

   // tries to send exactly "msgLen" bytes to peer
   while (pendingLen > 0)
   {
      int partialLen;
      int nEvents = waitSendEvent(socket, timeout_remaining(&timeout)); 

      if (nEvents == -1) // error
      {
         msgLen = -1;
         break;
      }

      if (nEvents == 0)
      {
         msgLen = E_SOCKET_TIMEOUT; // timeout 
         break;
      }

      // tries to send the remaining data
      partialLen = send(socket, buf, pendingLen, 0);
      if (partialLen < 0) // error ?
      {
         int err = socket_error();
         if (socket_shouldRetry(err) || socket_wouldBlock(err))
            continue;
         msgLen = -1; // error
         break;
      }

      if (trace)
      {
         ushort port;
         struct sockaddr_in sa;
         socklen_t addrlen = sizeof(sa);
         char bufPort[50];
         getsockname(socket, (struct sockaddr*)&sa, &addrlen);
         port = htons(sa.sin_port);
         sprintf(bufPort, "sent from port:%d", port);
         log_trace(bufPort, buf, partialLen);
      }
      pendingLen -= partialLen;
      buf += partialLen;
   } // while

   socket_setNonBlocking(socket, false);
   return msgLen;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Sets socket to nonblocking mode.
    @fn void socket_setNonBlocking(int socket, bool nonBlocking)

    @param [in]
    socket : descriptor of socket to be put in nonblocking mode

    @param [in]
    nonBlocking : flag that says to enter or leave nonblocking mode

    @post
    aborts application if not successful

    @memberof Socket
*/

#if PLATFORM(Linux)
TC2API void socket_setNonBlocking(int socket, bool nonBlocking)
{
   int flags = fcntl(socket,F_GETFL,0);
   if (nonBlocking)
      flags |= O_NONBLOCK;
   else
      flags &= ~O_NONBLOCK;
   fcntl(socket, F_SETFL, flags);
}
#endif

#if PLATFORM(Windows)
TC2API void socket_setNonBlocking(int socket, bool _mode)
{
   ulong mode = _mode;
   ioctlsocket(socket, FIONBIO, &mode);
}
#endif

// -----------------------------------------------------------------------------

/** Creates a TCP socket.

    @return
    a TCP socket descriptor

    @post
    Either succeeds or aborts application

    @private @memberof Socket
*/

static int createTCPsocket(void)
{
   int tcpSocket;

   log_func(createTCPsocket);
   log_finfo("creating a TCP socket");

   if ((tcpSocket=socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      // if we cannot create a socket there's something very wrong with the
      // system, so we abort the application
      int err = socket_error();
      log_ffatal("* error creating socket: %d", err);
      abort();
   }

   return tcpSocket;
}

// -----------------------------------------------------------------------------

/** Waits until there's some bytes available to receive, or timeout occurs.

    @param [in]
    socket : connected socket descriptor

    @param [in]
    nSeconds : timeout to wait until some bytes are available

    @return
    number of events available (-1, 0, 1)

    @private @memberof Socket
*/

static int waitRecvEvent(int socket, uint nSeconds)
{
   fd_set readfds;
   struct timeval timeout = { nSeconds, 0 };
   struct timeval* pTimeout = (nSeconds == TIMEOUT_WAIT_FOREVER) ? NULL
      : &timeout;

   FD_ZERO(&readfds);
   FD_SET(socket,&readfds);

   return select(socket+1, &readfds, NULL, NULL, pTimeout);
}

// -----------------------------------------------------------------------------

/** Waits until it's possible to send some bytes through the socket without
    blocking.

    @param [in]
    socket : connected socket descriptor

    @param [in]
    nSeconds : timeout to wait until the send event occurs

    @return
    number of events available (-1, 0, 1)

    @private @memberof Socket
*/

static int waitSendEvent(int socket, uint nSeconds)
{
   fd_set writefds;
   struct timeval timeout = { nSeconds, 0 };
   struct timeval* pTimeout = (nSeconds == TIMEOUT_WAIT_FOREVER) ? NULL
      : &timeout;

   FD_ZERO(&writefds);
   FD_SET(socket,&writefds);

   return select(socket+1, NULL, &writefds, NULL, pTimeout);
}

// -----------------------------------------------------------------------------
// the end
