#include "StdAfx.h"
#include ".\swlogfile.h"
#include "DebugLog.h"


CSWLogFile::CSWLogFile(void)
: StackWalker(RetrieveSymbol | RetrieveLine)
{
}

CSWLogFile::~CSWLogFile(void)
{
}

void CSWLogFile::OnOutput(LPCSTR szText)
{
#ifdef _LOGED_RELEASE
	CString str;
	str = szText;
	theDebugLog.Log(str);
#else
	szText;
#endif
}
