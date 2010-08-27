// Log4cxxWrapper.cpp: implementation of the Log4cxxWrapper class.
//
//////////////////////////////////////////////////////////////////////
#include "ZTableInc.h"
#include "Log4cxxHelper.h"
#	pragma warning( disable : 4250 )
#	pragma  warning( disable : 4251 )
#		include <log4cxx/logger.h>
#		include <log4cxx/helpers/properties.h>
#		include <log4cxx/propertyconfigurator.h>
#	pragma warning( default : 4250 )
#	pragma  warning( default : 4251 )

#if _MSC_VER < 1300
	#pragma comment (lib, "log4cxx.lib")
#else
	#ifdef _DEBUG
		#pragma comment (lib, "log4cxxd.lib")
	#else
		#pragma comment (lib, "log4cxxr.lib")
	#endif	
#endif

extern std::string gStrLogFile;
std::string Log4cxxHelper::_pref;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
log4cxx::LoggerPtr getLogger(const std::string& pref)
{
	static log4cxx::LoggerPtr logger_;
	static bool bGot_ = false;
	if (!bGot_) {
		logger_ = log4cxx::Logger::getLogger(pref);
		bGot_ = true;
	}
	return logger_;
}

void Log4cxxHelper::configure(const char * pref, long delay, HMODULE hMod)
{
	_pref = pref;
	char fn[MAX_PATH];
	GetModuleFileName(hMod, fn, sizeof(fn));
	char * p = strrchr(fn, '\\');
	if (p) {
		*(p+1) = '\0';
		strcat(fn, gStrLogFile.c_str());
	}
	log4cxx::PropertyConfigurator::configure(std::string(fn));
}

void Log4cxxHelper::logInfo(const char * fmt, ...)
{
	if (fmt == 0) return;
	va_list args;
	va_start(args, fmt);
	char temp[4096];
	memset(temp, 0, sizeof(temp));
	_vsnprintf(temp, sizeof(temp)-1, fmt, args);
	getLogger(_pref)->info(temp);
	va_end(args);
}

void Log4cxxHelper::logError(const char * fmt, ...)
{
	if (fmt == 0) return;
	va_list args;
	va_start(args, fmt);
	char temp[4096];
	memset(temp, 0, sizeof(temp));
	_vsnprintf(temp, sizeof(temp)-1, fmt, args);
	getLogger(_pref)->error(temp);
	va_end(args);
}

void Log4cxxHelper::logDebug(const char * fmt, ...)
{
	if (fmt == 0) return;
	va_list args;
	va_start(args, fmt);
	char temp[4096];
	memset(temp, 0, sizeof(temp));
	_vsnprintf(temp, sizeof(temp)-1, fmt, args);
	getLogger(_pref)->debug(temp);
	va_end(args);
}
