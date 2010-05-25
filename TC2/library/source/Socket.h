/// @source      Socket.h
/// @description Public interface for the Socket facility.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------
#ifndef __SOCKET_H__
#define __SOCKET_H__
// -----------------------------------------------------------------------------

// Windows-specific includes
#if PLATFORM(Windows)
// ...
#endif

// Linux-specific includes
#if PLATFORM(Linux)
#include <errno.h>
#include <unistd.h>
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// -----------------------------------------------------------------------------

// the redundancy here is for doxygen documentation

// common platform declarations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// see Socket::socket_connect
extern TC2API bool socket_connect(int, uint, ushort);

/// see Socket::socket_createClientSocket
extern TC2API int  socket_createClientSocket(void);

/// see Socket::socket_init
extern TC2API void socket_init(void);

/// see Socket::socket_partialLenReceived
extern TC2API int socket_partialLenReceived(void);

/// see Socket::socket_recvBuffer
extern TC2API int  socket_recvBuffer(int, char*, int, uint, bool);

/// see Socket::socket_sendBuffer
extern TC2API int  socket_sendBuffer(int, cchar*, int, uint, bool);

/// see Socket::socket_setNonBlocking
extern TC2API void socket_setNonBlocking(int, bool);

// Linux-specific declarations
#if PLATFORM(Linux)
#define socket_shouldRetry(err) (err == EINTR)
#define socket_close close
#define socket_error() errno
#define socket_wouldBlock(err) (err == EWOULDBLOCK)
extern int errno;
#endif

// Windows-specific declarations
#if PLATFORM(Windows)
#define socket_close closesocket
#define socket_error() WSAGetLastError()
#define socklen_t int
#define socket_shouldRetry(err) 0
#define socket_wouldBlock(err) (err == WSAEWOULDBLOCK)
#endif

/// Generic Socket error definition
#define E_SOCKET_ERROR     -1

/// Generic timeout Socket error
#define E_SOCKET_TIMEOUT   -2

/// Socket closed on reception
#define E_SOCKET_CLOSED     0

// -----------------------------------------------------------------------------
#ifdef __cplusplus
} // #ifdef __cplusplus
#endif
// -----------------------------------------------------------------------------
#endif // __SOCKET_H__
// -----------------------------------------------------------------------------
