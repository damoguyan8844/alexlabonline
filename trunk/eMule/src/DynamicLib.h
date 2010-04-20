#pragma once

class CDynamicLib
{
public:
	CDynamicLib(void);
	~CDynamicLib(void);
	static VOID PrepareLibs(void);
	static VOID RegisterAll(void);
	static BOOL IsRegistered(void);
	static BOOL IsPathOk(void);
private:
	static BOOL RegisterLibrary(LPCTSTR szName);
	static BOOL UnregisterLibrary(LPCTSTR szName);
protected:
	static BOOL CheckForUpdate( CString realpath );
};
