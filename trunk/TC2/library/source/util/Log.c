/// @source      Log.c
/// @description Implementation of the Log facility.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------

// includes

// common configuration options & declarations
#include "../config.h"  // always include first

// C language includes
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h> // memset

// framework includes
#include "Log.h"     /* log_xxx functions    */
#include "Mutex.h"   /* mutex_xxx functions  */
#include "Thread.h"  /* thread_xxx functions */
#include "Time.h"    /* time_xxx functions   */

// -----------------------------------------------------------------------------

// global declarations

/// Log class.
/// The log facility used by the framework and available for use by the
/// applications.
/// @class Log

/// File pointer of the Log file
/// @private @memberof Log
static FILE* fpLog;

/// Maximum size of the name of the Log file
/// @private @memberof Log
enum LogNameSize
{
   LOGNAMESIZE = 256
};

/// Maximum size of the name of the Log directory
/// @private @memberof Log
enum
{
   LOGDIRSIZE = 256
};

/// Log file directory
/// @private @memberof Log
static char logDir[LOGDIRSIZE] = ".";

/// Log file name
/// @private @memberof Log
static char logName[LOGNAMESIZE];

/// Contention mutex, because the Log functions can be called from any Thread
/// @private @memberof Log
static mutex_t* logMutex;

/// Identification of this source file for Log purposes
/// @private @memberof Log
static cchar sourceID[] = "ZLO";

/// Current function name
/// @private @memberof Log
static cchar* funcName;

/// Current Log level.
/// By default, write everything to the log, except debug info.
/// @private @memberof Log
static uint currentLogLevel = LOG_LEVEL_INFO;

// -----------------------------------------------------------------------------

// layout of a line that is written to the log file
//
// pos  len content
// 00   06  HHMMSS time
// 06   01  space 1
// 07   01  level    "I" for info, "W" for warning, "E" for error
// 08   01  space 2
// 09   03  sourceID
// 12   01  space 3
// 13   04  lineNum  source line number where the log function was called
// 17   01  space 4
// 18   03   thread seq no
// 21   01  space 5
// 22   xx  additional data

/// Offsets in the Log line layout
/// @private @memberof Log
enum LogLineLayout
{
   LOG_OFF_TIME   = 0  , LOG_OFF_SPC1 = 6  ,
   LOG_OFF_LEVEL  = 7  , LOG_OFF_SPC2 = 8  ,
   LOG_OFF_SRC_ID = 9  , LOG_OFF_SPC3 = 12 ,
   LOG_OFF_LINE   = 13 , LOG_OFF_SPC4 = 17 ,
   LOG_OFF_THRD   = 18 , LOG_OFF_SPC5 = 21 ,
   LOG_OFF_DATA   = 22
};

/// Length of Log line buffer
/// @private @memberof Log
enum LogBufferLength
{
   LOG_BUF_LEN = 256
};

/// Maximum length of data in the Log line buffer
/// @private @memberof Log
enum MaximumLogDataLength
{
   LOG_DATA_LEN = (LOG_BUF_LEN - LOG_OFF_DATA - 1)
};

/// The Log line buffer
/// @private @memberof Log
static char logBuf[LOG_BUF_LEN];

// internal helper functions
/// @cond hides_from_doxygen
static void createLogName(void);
static char hiHex(char);
static char levelChar(uint);
static char loHex(char);
static void nullLog(cchar* fmt, ...) { fmt = 0; }
static FormatFunc writeCommonHeader(cchar*, int, uint);
static void writeTraceTitle(cchar*, int, cchar*, uint);
/// @endcond

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Finishes writing a Log line.

    @param [in]
    format : printf-like fomat specification.

    @memberof Log
*/

TC2API void log_body(cchar* format, ...)
{
   va_list parms;
   int bufLen = LOG_DATA_LEN;
   int funcLen = 0;

   assert(format);

   if (funcName)
   {
      funcLen = strlen(funcName) + 3; // +3 for open/close parms and space
      sprintf(logBuf+LOG_OFF_DATA,"(%s) ", funcName);
      bufLen -= funcLen;
   }

   va_start(parms, format);
   vsnprintf(logBuf+LOG_OFF_DATA+funcLen, bufLen, format, parms);
   va_end(parms);

   fputs(logBuf, fpLog);
   fputc('\n', fpLog);
   fflush(fpLog);

   // see lock call in log_header function
   mutex_unlock(logMutex);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Starts writing a log line.

    @param [in]
    _funcName : name of the function that is writing this line, or NULL

    @param [in]
    _sourceID : 3-chars string identifying the current source file

    @param [in]
    lineNum : line number in the source file where this function was called

    @param [in]
    level : severity of the Log record, see Log.h.

    @memberof Log
*/

TC2API FormatFunc log_fheader(cchar* _funcName, cchar* _sourceID, int lineNum,
   uint level)
{
   // checks to see if the log facility is not initialized
   if (!fpLog)
      log_init();

   // doesn't write if severity not allowed (bigger level is less severity)
   if (level > currentLogLevel)
      return nullLog;

   assert(_sourceID);
   assert(_funcName);

   mutex_lock(logMutex); // see unlock in log_body function

   funcName = _funcName;

   return writeCommonHeader(_sourceID, lineNum, level);
}

// -----------------------------------------------------------------------------

/** Retrieves the current log level.

    @return
    the current log level

    @memberof Log
*/

TC2API uint log_level(void)
{
   return currentLogLevel;
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Writes an hexadecimal trace in the Log file.

    @param [in]
    _sourceID : 3-chars string identifying the current source file

    @param [in]
    lineNum : line number in the source file where this function was called

    @param [in]
    msg : message identifying the hex dump

    @param [in]
    buf : address of the buffer from which to write the hex dump

    @param [in]
    bufLen : number of bytes of source buffer

    @memberof Log
*/

TC2API void log_writeTrace(cchar* _sourceID, int lineNum, cchar* msg, cchar* buf,
   uint bufLen)
{
   cchar* pBuf = buf; // pointer runs over the data buffer

   // dump line specification
   int offset = 0;                     // initial offset
   int offSize = 4;                    // length of offset when formatted
   int nBlocks = 6;                    // number of blocks per line
   int blockBytes = 4;                 // number of byter per block
   int bufStep = nBlocks*blockBytes;   // increment to add for the next line
   int nLines = bufLen / bufStep;      // number of complete lines
   int lastStep = bufLen % bufStep;    // length of residual last line

   // off  1(4byt)  2        3        4        5        6        24 bytes
   // xxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxxxxxxxxxxxxxx

   // boilerplate code
   if (!fpLog)
      log_init();
   assert(_sourceID);
   mutex_lock(logMutex);

   writeTraceTitle(_sourceID, lineNum, msg, bufLen); 

   // now loops over the lines
   for (;;)
   {
      uchar* pCurBuf; // uchar, or else problems with sign extension!
      char* pLogBuf = logBuf + LOG_OFF_DATA;
      int i, stepLen, savStepLen;

      if (nLines--)           // still one full line to go ?
         stepLen = bufStep;   // ok, full step then
      else if (lastStep)      // last (partial) line to go ?
         stepLen = lastStep;  // uses last step
      else                    // else done,
         break;               // breaks out of loop
      savStepLen = stepLen;

      writeCommonHeader(_sourceID, lineNum, LOG_LEVEL_TRACE);
      sprintf(pLogBuf, "%0*X ", offSize, offset);  // writes offset
      offset += bufStep;  // prepares offset for next line
      pLogBuf += offSize; // advances log buffer

      // now loops over the blocks
      pCurBuf = (uchar*)pBuf;
      for (i = 0; i < nBlocks; i++) // a block
      {
         int j;
         *pLogBuf++ = ' '; // writes separator
         // now, writes 1 block
         for (j = 0; j < blockBytes; j++)
             if (stepLen) // are there still bytes to write ?
             {
                *pLogBuf++ = hiHex(*pCurBuf);
                *pLogBuf++ = loHex(*pCurBuf);
                pCurBuf++, stepLen--;
             }
             else
                *pLogBuf++ = ' ', *pLogBuf++ = ' ';
      } // a block

      // now writes the plain bytes
      *pLogBuf++ = ' '; // last separator
      pCurBuf = (uchar*)pBuf;   // resets the current data pointer
      for (stepLen = savStepLen, i = 0; i < bufStep; i++)
      {
         if (stepLen) // are there still bytes to write ?
         {
            *pLogBuf++ = isprint(*pCurBuf) ? *pCurBuf : '.';
            pCurBuf++, stepLen--;
         }
//       else
//          *pLogBuf++ = ' '; // no more data bytes
      }
      *pLogBuf = 0; // final binary zero

      // now writes the line
      fputs(logBuf, fpLog);
      fputc('\n', fpLog);
      fflush(fpLog);

      // checks to see if it was the last line
      if (savStepLen != bufStep)
         break;

      // now advances data buffer
      pBuf += bufStep;
   } // for

   mutex_unlock(logMutex);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Starts writing a Log line.

    @param [in]
    _sourceID : 3-chars string identifying the current source file

    @param [in]
    lineNum : line number in the source file where this function was called

    @param [in]
    level : severity of the Log record, see Log.h.

    @memberof Log
*/

TC2API FormatFunc log_header(cchar* _sourceID, int lineNum, uint level)
{
   // checks to see if the log facility is not initialized
   if (!fpLog)
      log_init();

   // doesn't write if severity not allowed (bigger level is less severity)
   if (level > currentLogLevel)
      return nullLog;

   assert(_sourceID);

   mutex_lock(logMutex); // see unlock in log_body function

   funcName = 0; // here we won't write the fucntion name

   return writeCommonHeader(_sourceID, lineNum, level);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Initializes the Log class.

    Initialization of the log facility: opens the log file and creates the
    contention mutex.

    @memberof Log
*/

TC2API void log_init(void)
{
   // checks to see if the log facility is already initialized
   if (fpLog)
      return;

   logMutex = mutex_create();

   if (!*logName)
      createLogName();

   fpLog = fopen(logName,"a");
   assert(fpLog);

   {
      char buf[20];
      time_formatCurrentDate(buf);
      log_info("******** start of execution in DD/MM/YYYY=%s ********", buf);
      log_info("TC2 Generic TCP Client Application Library version %s",
         TC2_VERSION);
   }
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Set the directory where the Log file will be created.

    @param [in]
    dir : directory name where to write the Log file

    @memberof Log
*/

TC2API void log_setDir(cchar* dir)
{
   strncpy(logDir, dir, LOGDIRSIZE);
}

// -----------------------------------------------------------------------------
// PUBLIC INTERFACE
// -----------------------------------------------------------------------------

/** Set the new Log level.

    @param [in]
    newLevel : new Log level.

    @memberof Log
*/

TC2API void log_setLevel(uint newLevel)
{
   log_func(log_setLevel);

   if (!newLevel)
      newLevel = LOG_LEVEL_INFO;

   if (newLevel < LOG_LEVEL_INFO)
      log_fwarn("ignoring invalid level: %d", newLevel);
   else
   {
      currentLogLevel = newLevel;
      log_finfo("new log level: %d", newLevel);
   }
}

// -----------------------------------------------------------------------------
// INTERNAL HELPER FUNCTIONS
// -----------------------------------------------------------------------------

/** Creates the Log file name.

    The Log file name is in the format YYMMDD_HHMMSS_pid.tse, where YYMMDD is
    the current date, HHMMSS is the current time, and <i>pid</i> is the current
    process identification provided by the operating system.

    @private @memberof Log
*/

static void createLogName(void)
{
   char * name;
   uint logDirSize = strlen(logDir);

   assert(logDirSize + 30 < LOGNAMESIZE);

   strcpy(logName, logDir);
   logName[logDirSize++] = '/';
   name = logName + logDirSize;

   // 0....5....1...*         (assuming max 30 chars)
   // YYMMDD_HHMMSS_pid.tse
   time_formatCurrentShortDateReversePacked(name);
   name[6] = '_';
   time_formatCurrentTimePacked(name+7);
   sprintf(name+13,"_%d.tse",thread_pid());
}

// -----------------------------------------------------------------------------

/** Returns the high 4 bits of a byte, formatted as an hexadecimal character.

    @param [in]
    b : the byte

    @return
    the high 4 bits of a byte, formatted as an hexadecimal character

    @private @memberof Log
*/

static char hiHex(char b)
{
   char c = ((b >> 4) & 0x0F) + 0x30;
   return (c > 0x39 ? (c+7) : c);
}

// -----------------------------------------------------------------------------

/** Maps the numeric log level to an indicator letter.

    @param [in]
    level : level number to be mapped to a letter.

    @return
    the letter corresponding to the numeric Log level

    @private @memberof Log
*/

static char levelChar(uint level)
{
   if (level == LOG_LEVEL_FATAL)
      return 'F';

   if (level == LOG_LEVEL_ERROR)
      return 'E';

   if (level == LOG_LEVEL_WARN)
      return 'W';

   if (level == LOG_LEVEL_INFO)
      return 'I';

   if (level == LOG_LEVEL_DEBUG)
      return 'D';

   if (level == LOG_LEVEL_TRACE)
      return 'T';

   return '?';
}

// -----------------------------------------------------------------------------

/** Returns the low 4 bits of a byte, formatted as an hexadecimal character.

    @param [in]
    b : the byte

    @return
    the low 4 bits of a byte, formatted as an hexadecimal character

    @private @memberof Log
*/

static char loHex(char b)
{
   char c = (b & 0x0F) + 0x30;
   return (c > 0x39 ? (c+7) : c);
}

// -----------------------------------------------------------------------------

/** Starts writing a Log line, common part.

    @param [in]
    _sourceID : 3-chars string identifying the current source file

    @param [in]
    lineNum : line number in the source file where this function was called

    @param [in]
    level : severity of the Log record, see Log.h.

    @remarks
    this function runs protected by the Log mutex
   
    @private @memberof Log
*/

static FormatFunc writeCommonHeader(cchar* _sourceID, int lineNum, uint level)
{
   // nothing special here, just formats the header of the log record

   time_formatCurrentTimePacked(logBuf+LOG_OFF_TIME); // without the ':'s
   logBuf[LOG_OFF_SPC1] = ' '; // gets rid of '\0' before

   logBuf[LOG_OFF_LEVEL] = levelChar(level);
   logBuf[LOG_OFF_SPC2] = ' ';

   memcpy(logBuf+LOG_OFF_SRC_ID, _sourceID, 3);
   logBuf[LOG_OFF_SPC3] = ' ';

   sprintf(logBuf+LOG_OFF_LINE, "%04d", lineNum);
   logBuf[LOG_OFF_SPC4] = ' ';

   sprintf(logBuf+LOG_OFF_THRD, "%03d", thread_selfSeqNo());
   logBuf[LOG_OFF_SPC5] = ' ';

   // return the function which will write the record body
   return log_body;
}

// -----------------------------------------------------------------------------

/** Writes the title of a trace block.

    @param [in]
    _sourceID : 3-chars string identifying the current source file

    @param [in]
    lineNum : line number in the source file where this function was called

    @param [in]
    msg : a message identifying the hex trace

    @param [in]
    bufLen : length of the trace source buffer, for information purpose
   
    @private @memberof Log
*/

static void writeTraceTitle(cchar* _sourceID, int lineNum, cchar* msg,
   uint bufLen)
{
   if (!msg) // no message ?
      return;

   writeCommonHeader(_sourceID, lineNum, LOG_LEVEL_TRACE);
   sprintf(logBuf + LOG_OFF_DATA, "trace: %d bytes (%s)", bufLen, msg);
   fputs(logBuf, fpLog);
   fputc('\n', fpLog);
   fflush(fpLog);
}

// -----------------------------------------------------------------------------
// the end
