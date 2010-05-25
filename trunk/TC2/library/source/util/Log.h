/// @source      Log.h
/// @description Public interface for the Log facility.
//  See licensing information in the file README.TXT.

// -----------------------------------------------------------------------------
#ifndef __LOG_H__
#define __LOG_H__
// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// -----------------------------------------------------------------------------

// the redundancy here is for doxygen documentation

typedef void (*FormatFunc)(cchar*, ...);

/// see Log::log_header
extern TC2API FormatFunc log_header(cchar*, int, uint);

/// see Log::log_fheader
extern TC2API FormatFunc log_fheader(cchar*, cchar*, int, uint);

/// see Log::log_init
extern TC2API void log_init(void);

/// see Log::log_level
extern TC2API uint log_level(void);

/// see Log::log_setDir
extern TC2API void log_setDir(cchar*);

/// see Log::log_setLevel
extern TC2API void log_setLevel(uint);

/// see Log::log_writeTrace
extern TC2API void log_writeTrace(cchar*, int, cchar*, cchar*, uint);

/// standard source file name for inclusion in log
#define log_func(x) static cchar func[] = #x

// always uses the variable name "sourceID" to identify the current source file

/// log error record (see Log::log_header)
#define log_error log_header(sourceID, __LINE__, LOG_LEVEL_ERROR)

/// log fatal error record (see Log::log_header)
#define log_fatal log_header(sourceID, __LINE__, LOG_LEVEL_FATAL)

/// log informational record (see Log::log_header)
#define log_info  log_header(sourceID, __LINE__, LOG_LEVEL_INFO)

/// log warning record (see Log::log_header) 
#define log_warn  log_header(sourceID, __LINE__, LOG_LEVEL_WARN)

/// log debug record (see Log::log_header)
#define log_debug log_header(sourceID, __LINE__, LOG_LEVEL_DEBUG)

/// log trace record (see Log::log_writeTrace)
#define log_trace(msg,buf,len) log_writeTrace(sourceID, __LINE__, msg, buf, len)

/// log error record with function identification (see Log::log_fheader)
#define log_ferror log_fheader(func, sourceID, __LINE__, LOG_LEVEL_ERROR)

/// log fatal error record with function identification (see Log::log_fheader)
#define log_ffatal log_fheader(func, sourceID, __LINE__, LOG_LEVEL_FATAL)

/// log informational record with function identification (see Log::log_fheader)
#define log_finfo  log_fheader(func, sourceID, __LINE__, LOG_LEVEL_INFO)

/// log warning record with function identification (see Log::log_fheader)
#define log_fwarn  log_fheader(func, sourceID, __LINE__, LOG_LEVEL_WARN)

/// log debug record with function identification (see Log::log_fheader) 
#define log_fdebug log_fheader(func, sourceID, __LINE__, LOG_LEVEL_DEBUG)

// -----------------------------------------------------------------------------

enum LogLevel
{
   LOG_LEVEL_FATAL = 0  ,
   LOG_LEVEL_ERROR = 10 ,
   LOG_LEVEL_WARN  = 20 ,
   LOG_LEVEL_INFO  = 30 ,
   LOG_LEVEL_DEBUG = 40 ,
   LOG_LEVEL_TRACE = 50
};

// -----------------------------------------------------------------------------
#ifdef __cplusplus
} // #ifdef __cplusplus
#endif
// -----------------------------------------------------------------------------
#endif // __LOG_H__
// -----------------------------------------------------------------------------
