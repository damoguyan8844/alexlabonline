#include "StdAfx.h"
#include "DynamicLib.h"
#include "Preferences.h"
#include "otherfunctions.h"

BOOL CDynamicLib::RegisterLibrary(LPCTSTR szName)
{
	typedef HRESULT (_stdcall *FNDLLRS)(void);

	HMODULE hLib = ::LoadLibrary(szName);
	if(hLib == NULL)
		return FALSE;
	FNDLLRS m_pfnRegServer = (FNDLLRS)GetProcAddress(hLib, "DllRegisterServer");
	if(m_pfnRegServer == NULL)
	{
		FreeLibrary(hLib);
		return FALSE;
	}
	m_pfnRegServer();

	FreeLibrary(hLib);
	return TRUE;
}

BOOL CDynamicLib::UnregisterLibrary(LPCTSTR szName)
{
	typedef HRESULT (_stdcall *FNDLLRS)(void);

	HMODULE hLib = ::LoadLibrary(szName);
	if(hLib == NULL)
		return FALSE;
	FNDLLRS m_pfnUnregServer = (FNDLLRS)GetProcAddress(hLib, "DllUnregisterServer");
	if(m_pfnUnregServer == NULL)
	{
		FreeLibrary(hLib);
		return FALSE;
	}
	m_pfnUnregServer();

	FreeLibrary(hLib);
	return TRUE;
}

CDynamicLib::CDynamicLib(void)
{
}

CDynamicLib::~CDynamicLib(void)
{
}

BOOL CDynamicLib::CheckForUpdate( CString realpath )
{
	CString oldpath = realpath + _T(".old");
	CString newpath = realpath + _T(".new");
	if(!PathFileExists(newpath))
		return FALSE;
	if(PathFileExists(realpath))
	{
		if(PathFileExists(oldpath))
		{
			if(_tremove(oldpath) != 0)
				return FALSE;
		}
		if(_trename(realpath, oldpath) != 0)
		{
			return FALSE;
		}
		UnregisterLibrary(realpath);
	}
	_trename(newpath, realpath);
	return TRUE;
}

VOID CDynamicLib::PrepareLibs( void )
{
	if(CheckForUpdate(thePrefs.GetMuleDirectory(EMULE_EXECUTEABLEDIR) + _T("IE2EM.dll")) || !IsRegistered())
		RegisterAll();
}

VOID CDynamicLib::RegisterAll( void )
{
	RegisterLibrary(thePrefs.GetMuleDirectory(EMULE_EXECUTEABLEDIR) + _T("IE2EM.dll"));
}

BOOL CDynamicLib::IsRegistered( void )
{
	CRegKey checkkey;
	return checkkey.Open(HKEY_CLASSES_ROOT, _T("CLSID\\{48618374-565F-4CA0-B8CD-6F496C997FAF}")) == ERROR_SUCCESS &&
		checkkey.Open(HKEY_CLASSES_ROOT, _T("IE2EM.IE2EMUrlTaker")) == ERROR_SUCCESS;
}

BOOL CDynamicLib::IsPathOk( void )
{
	CRegKey checkkey;
	if(checkkey.Open(HKEY_CLASSES_ROOT, _T("CLSID\\{48618374-565F-4CA0-B8CD-6F496C997FAF}\\InprocServer32")) != ERROR_SUCCESS)
		return FALSE;
	TCHAR szVal[MAX_PATH];
	ULONG len = MAX_PATH;
	checkkey.QueryStringValue(NULL, szVal, &len);
	return thePrefs.GetMuleDirectory(EMULE_EXECUTEABLEDIR) + _T("IE2EM.dll") == szVal;
}
