/// @source      Thread.h
/// @description Public interface for class Thread.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------
#ifndef __THREAD_H__
#define __THREAD_H__
// -----------------------------------------------------------------------------

#if PLATFORM(Windows)
#include <windows.h>
#endif

#if PLATFORM(Linux)
#include <pthread.h>
#include <unistd.h> // for convenience (provides sleep)
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// -----------------------------------------------------------------------------

// Linux-specific declarations
#if PLATFORM(Linux)

/// Return type of the main function of a thread.
typedef void* THREAD_RESULT;

/// Calling convention for the main function of a thread.
#define __THREAD__ /* nothing! */

/// Type of the main function of a thread main.
typedef THREAD_RESULT ( __THREAD__ *ThreadFunc)(void*);
#define threadfunc THREAD_RESULT  __THREAD__

/// Identification of the current thread.
#define currentThreadID pthread_self()

#define currentProcessID()  ((uint)getpid())

#endif // PLATFORM(Linux)

// -----------------------------------------------------------------------------

// Windows-specific declarations
#if PLATFORM(Windows)

// *** when using CreateThread
// *** /// Return type of the main function of a thread.
// *** typedef DWORD THREAD_RESULT;

// when using _beginthreadex
/// Return type of the main function of a thread.
typedef unsigned THREAD_RESULT;

// *** when using CreateThread
// *** /// Calling convention for the main function of a thread.
// *** #define __THREAD__ WINAPI

// when using _beginthreadex (in fact, WINAPI and __stdcall are identical!)
/// Calling convention for the main function of a thread.
#define __THREAD__ __stdcall

/// Type of the main function of a thread.
typedef THREAD_RESULT ( __THREAD__ *ThreadFunc)(void*);
#define threadfunc THREAD_RESULT  __THREAD__

/// Identification of the current thread.
#define currentThreadID GetCurrentThreadId()

#define currentProcessID()  ((uint)GetCurrentProcessId())

#endif // PLATFORM(Windows)

// -----------------------------------------------------------------------------

// the Thread opaque type
// typedef struct Thread Thread;

// class initialization
/// see Thread::thread_init
extern TC2API void thread_init(void);

// query
// ~~~~~

/// see Thread::thread_selfSeqNo
extern TC2API uint thread_selfSeqNo(void);

/// The current thread id provided by the operating system
#define thread_selfID() ((uint)(currentThreadID))

/// The current process id provided by the operating system
#define thread_pid() currentProcessID()

// operations
// ~~~~~~~~~~

/// see Thread::thread_addThreads
extern TC2API void thread_addThreads(uint, ThreadFunc, void*);

// convenience
// ~~~~~~~~~~~

/// see Thread::thread_printf
extern TC2API void thread_printf(cchar*, ...);

// -----------------------------------------------------------------------------
#ifdef __cplusplus
} // #ifdef __cplusplus
#endif
// -----------------------------------------------------------------------------
#endif // __THREAD_H__
// -----------------------------------------------------------------------------
