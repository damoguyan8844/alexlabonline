// Log4cxxWrapper.h: interface for the Log4cxxWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG4CXXWRAPPER_H__D2715DD3_A64C_41EF_AB7F_8AF2A526294D__INCLUDED_)
#define AFX_LOG4CXXWRAPPER_H__D2715DD3_A64C_41EF_AB7F_8AF2A526294D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class Log4cxxHelper
{
public:
	static void configure(const char * pref, long delay, HMODULE hMod = 0);
	static void logInfo(const char * fmt, ...);
	static void logError(const char * fmt, ...);
	static void logDebug(const char * fmt, ...);
private:
	Log4cxxHelper();
	static std::string _pref;
};

#define LOG_INFO Log4cxxHelper::logInfo
#define LOG_ERROR Log4cxxHelper::logError
#define LOG_DEBUG Log4cxxHelper::logDebug

// The macros used in STDMETHOD interface functions**************************
inline void AuxSetComErrorInfo(const _com_error& e) throw() {
	IErrorInfo* pErrorInfo = e.ErrorInfo();
	SetErrorInfo(0, pErrorInfo);
	if ( pErrorInfo ) 
		pErrorInfo->Release();
}

inline void AuxRaiseComError(HRESULT hr) throw(_com_error) {
	void __stdcall _com_raise_error(HRESULT, IErrorInfo*) throw(_com_error); // from COMMSUPP.LIB
	IErrorInfo* pErrorInfo = NULL;
	GetErrorInfo(0, &pErrorInfo);
	if ( pErrorInfo ) {
		// cannot check for ISupportErrorInfo as there is no the interface
		// but put a simple check for FACILITY_DISPATCH:
		// isn't it a trashy ErrorInfo?
		if ( HRESULT_FACILITY(hr) != FACILITY_DISPATCH ) {
			// yes, don't consider it
			// SetErrorInfo(0, pErrorInfo);
			pErrorInfo->Release();
			pErrorInfo = NULL;
		}
	}
	_com_raise_error(hr, pErrorInfo); // pass pErrorInfo ownership
}

inline void LogComErrorInfo(const _com_error& e, const char * file, int line) throw() {
	LOG_ERROR("%s [%s:%d]", e.Description().length()>0 ? (const char*)e.Description() : e.ErrorMessage(),
		file, line);
}
inline void LogAnyErrorInfo() {
}

#define _CATCH_COM() \
	catch (_com_error& e) { \
	LogComErrorInfo(e, __FILE__, __LINE__); \
AuxSetComErrorInfo(e);  } 

#define _CATCH_QL_ERROR() \
	catch(QuantLib::Error &e){ \
	LOG_ERROR(e.what());\
	CComBSTR strWhat(e.what()); \
AtlReportError(CLSID_NULL, strWhat, GUID_NULL, E_THC_QUANTLIB_ERROR); }

#define _CATCH_STD() \
	catch (std::exception & e) { \
	LOG_ERROR(e.what());\
AtlReportError(CLSID_NULL, e.what(), GUID_NULL, E_FAIL); }

#define _CATCH_ANY() \
	catch ( ... ) { \
	LogAnyErrorInfo(); \
	HRESULT hr = HRESULT_FROM_WIN32(GetLastError()); \
if ( hr == NOERROR ) hr = E_UNEXPECTED; }

#define _CATCH_SQLLITE() \
	catch (CppSQLite3Exception& e) {\
LOG_ERROR("SQL Error Code:%d,%s", e.errorCode() ,e.errorMessage() ); }

#define _CATCH_XXX() \
_CATCH_COM() _CATCH_SQLLITE() _CATCH_STD() _CATCH_ANY()

#endif // !defined(AFX_LOG4CXXWRAPPER_H__D2715DD3_A64C_41EF_AB7F_8AF2A526294D__INCLUDED_)
