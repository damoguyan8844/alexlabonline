/// @source      Mutex.c
/// @description Implementation of Mutex utility.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------

// includes

// common configuration options & declarations
#include "../config.h"  // always include first

// C language includes
#include <assert.h>
#include <stdlib.h> // calloc

#if PLATFORM(Windows)
#include <windows.h>
#endif

#if PLATFORM(Linux)
#include <pthread.h>
#endif

// framework support
#include "Mutex.h" /* mutex_xxx functions */

// -----------------------------------------------------------------------------

// global declarations

/// Mutex class.
/// This class that encapsulates a platform-specific mutex facility.
/// @class Mutex

// Linux-specific declarations
#if PLATFORM(Linux)
struct mutex_t
{
   /// Native Linux mutex handle.
   pthread_mutex_t handle;
};
#endif

// Windows-specific declarations
#if PLATFORM(Windows)
struct mutex_t
{
   /// Native Windows mutex handle.
   HANDLE handle;
};
#endif

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Creates a Mutex instance.
    @fn mutex_t* mutex_create(void)

    @return
    address of Mutex created

    @post
    Either succeeds or aborts application.

    @memberof Mutex
*/

#if PLATFORM(Linux)
mutex_t* mutex_create(void)
{
   int result;
   mutex_t* mutex = (mutex_t*)calloc(1, sizeof(mutex_t));

   // memory allocation successful ?
   assert(mutex);

   // initializes mutex
   result = pthread_mutex_init(&mutex->handle, NULL);
   assert(result == 0);

   return mutex;
}
#endif

#if PLATFORM(Windows)
TC2API mutex_t* mutex_create(void)
{
   mutex_t* mutex = (mutex_t*)calloc(1, sizeof(mutex_t));

   // memory allocation successful ?
   assert(mutex);

   mutex->handle = CreateMutex(
      NULL,  // LPSECURITY_ATTRIBUTES
      FALSE, // initial owner
      NULL   // name
      );

   assert(mutex->handle);

   return mutex;
}
#endif

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Locks a Mutex.
    @fn void mutex_lock(mutex_t* mutex)

    @param [in]
    mutex : address of mutex to be locked.

    @post
    Either succeeds or aborts application.

    @memberof Mutex
*/

#if PLATFORM(Linux)
void mutex_lock(const mutex_t* mutex)
{
   int result = pthread_mutex_lock(&(((mutex_t*)mutex)->handle));
   assert(result == 0);
}
#endif

#if PLATFORM(Windows)
TC2API void mutex_lock(const mutex_t* mutex)
{
   DWORD result = WaitForSingleObject(mutex->handle, INFINITE);
   assert(result == WAIT_OBJECT_0);
}
#endif

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Unlocks a mutex.
    @fn void mutex_unlock(mutex_t* mutex)

    @param [in]
    mutex : address of mutex to be unlocked.

    @post
    Either succeeds or aborts application.

    @memberof Mutex
*/

#if PLATFORM(Linux)
void mutex_unlock(const mutex_t* mutex)
{
   int result = pthread_mutex_unlock(&(((mutex_t*)mutex)->handle));
   assert(result == 0);
}
#endif

#if PLATFORM(Windows)
TC2API void mutex_unlock(const mutex_t* mutex)
{
   ReleaseMutex(mutex->handle);
}
#endif

// -----------------------------------------------------------------------------
// the end
