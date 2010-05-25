/// @source      Thread.c
/// @description Implementation of class Thread.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------

// includes

// common configuration options & declarations
#include "../config.h" // always include first

// C language includes
#include <assert.h>
#include <stdarg.h> // calloc
#include <stdio.h>  // vprintf
#include <stdlib.h> // calloc
#if PLATFORM(Windows)
#include <process.h> // _beginthreadex
#endif

// framework includes
#include "Thread.h"    /* thread_xxx functions */
#include "Mutex.h"     /* mutex_xxx functions  */
#include "Log.h"       /* log_xxx functions    */

// -----------------------------------------------------------------------------

// global declarations

/// Thread class.
/// This class that encapsulates a platform-specific thread facility.
/// @class Thread

/// Identification of this source file in the Log file. 
/// @private @memberof Thread
static cchar sourceID[] = "ZTH";

// ---------------------
/// @cond hides_from_doxygen (it gets confused)

/// The operating-system handle of a Thread
IF_LINUX(typedef pthread_t hthread;)
IF_WINDOWS(typedef HANDLE hthread;)

/// The unique sequential number of a Thread
IF_LINUX(__thread uint currentThreadSeqNo);
IF_WINDOWS(__declspec(thread) uint currentThreadSeqNo);

/// @endcond
// ---------------------

/// Structure passed as parameter for a Thread.
/// This structure is used internally by the framework when creating an
/// application Thread.
/// @private @memberof Thread
typedef struct
{
   ThreadFunc threadFunc; ///< @private Thread funtion
   void* parm;            ///< @private parameter for the Thread function
} ThreadParm;

/// Next unique sequential Thread number
/// @private @memberof Thread
static uint currentSeqNo;  

/// Contention mutex for Thread-aware printf substitute
/// @private @memberof Thread
static mutex_t* printfMutex;

// prototypes
/// @cond hides_from_doxygen
static void createThread(ThreadFunc, void*);
static threadfunc proxyThreadFunc(void*);
/// @endcond

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Starts n instances of a Thread.

    @param
    n : the number of instances of a Thread to instantiate

    @param
    threadFunc : the main Thread function of the @link Thread Threads @endlink
    being instantiated

    @param
    parm : the parameter passes to the main Thread function
    being instantiated

    @memberof Thread
*/

TC2API void thread_addThreads(uint n, ThreadFunc threadFunc, void* parm)
{
   uint i;

   log_func(thread_addThreads);
   log_finfo("adding %d threads", n);

   if (n == 0)
      log_fwarn("strange, adding 0 threads here...");

   for (i = 0; i < n; i++)
   {
      ThreadParm* threadParm = (ThreadParm*)calloc(1, sizeof(ThreadParm));
      assert(threadParm);

      threadParm->threadFunc = threadFunc;
      threadParm->parm = parm;

      // platform-specific creation of a thread running the proxy function
      log_finfo("creating a thread");
      createThread(proxyThreadFunc, threadParm);
   }
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Thread class initialization.

    @memberof Thread
*/

TC2API void thread_init(void)
{
   log_func(thread_init);
   log_finfo("Thread class inicialization");

   log_finfo("creating the printf contention mutex");
   printfMutex = mutex_create();
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Thread-safe printf substitute.

    @memberof Thread
*/

TC2API void thread_printf(const char* format, ...)
{
   va_list parms;
   va_start(parms,format);

   assert(format);

   mutex_lock(printfMutex);
   vprintf(format, parms);
   fflush(stdout);
   mutex_unlock(printfMutex);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** The chronological activation number of the running Thread

    @return
    chronological activation number of the running Thread

    @memberof Thread
*/

TC2API uint thread_selfSeqNo(void)
{
   return currentThreadSeqNo;
}

// -----------------------------------------------------------------------------
// INTERNAL HELPER FUNCTIONS
// -----------------------------------------------------------------------------

/** Operating-system-level function that creates and runs a new Thread.
    @fn void createThread(ThreadFunc threadFunc, void* arg)

    @param [in]
    threadFunc : the Thread main function

    @param [in]
    arg : argument to the Thread main function

    @return
    identification of the Thread just created

    @private @memberof Thread
*/

#if PLATFORM(Linux)
static void createThread(ThreadFunc threadFunc, void* arg)
{
   pthread_t tid;;
   int ret = pthread_create(
     &tid,        // thread id (result)
      NULL,       // default attributes
      threadFunc, // start routine
      arg         // arguments
      );

   assert(ret == 0);
}
#endif

#if PLATFORM(Windows)
static void createThread(ThreadFunc threadFunc, void* arg)
{
// HANDLE hThread = CreateThread(
   HANDLE hThread = (HANDLE)_beginthreadex(
      NULL,       // LPSECURITY_ATTRIBUTES
      0,          // stack size (0=default)
      threadFunc, // LPTHREAD_START_ROUTINE
      arg,        // arguments
      0,          // creation flags (0 = runs immediately)
      NULL        // thread id
      );

   assert(hThread);
}  
#endif

// -----------------------------------------------------------------------------

/** Auxiliary function used to start a new Thread.

    @param [in]
    _parm : parameter passed to the Thread main function

    @private @memberof Thread
*/

static threadfunc proxyThreadFunc(void* _parm)
{
   // the only reason for the existence of this function is to initialize the
   // thread-local-storage (TLS) variable currentThreadSeqNo, which provides
   // platform-independent way of identifying threads; it could be used to
   // provide other pre-activation thread initialization

   // parameter received by this proxy threadfunc
   ThreadParm* threadParm = (ThreadParm*)_parm;

   // this is the real threadfunc
   ThreadFunc threadFunc = threadParm->threadFunc;

   // this is the real parm
   void* parm = threadParm->parm;

   // call free ThreadParm now, doesn't need it anymore
   free(threadParm);

   currentThreadSeqNo = ++currentSeqNo;

   // finally, runs the user thread...
   return threadFunc(parm);
}

// -----------------------------------------------------------------------------
// the end
