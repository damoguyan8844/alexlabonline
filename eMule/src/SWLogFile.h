#pragma once
#include "stackwalker.h"

class CSWLogFile :
	public StackWalker
{
public:
	CSWLogFile(void);
	~CSWLogFile(void);

private:
	virtual void OnSymInit(LPCSTR /*szSearchPath*/, DWORD /*symOptions*/, LPCSTR /*szUserName*/){}
	virtual void OnLoadModule(LPCSTR /*img*/, LPCSTR /*mod*/, DWORD64 /*baseAddr*/, DWORD /*size*/, DWORD /*result*/, LPCSTR /*symType*/, LPCSTR /*pdbName*/, ULONGLONG /*fileVersion*/){}
	//virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry);
	virtual void OnDbgHelpErr(LPCSTR /*szFuncName*/, DWORD /*gle*/, DWORD64 /*addr*/){}
	virtual void OnOutput(LPCSTR szText);
};
