#include "StdAfx.h"
#include ".\debuglog.h"

#ifdef _LOGED_RELEASE
CDebugLog theDebugLog;
#endif

CDebugLog::CDebugLog(void)
{
	CString strFile = GetAppPath();
	strFile += _T("DebugLog.dat");
	Create(strFile, 20*1024*1024);
}

CDebugLog::~CDebugLog(void)
{
	Close();
}

CString CDebugLog::GetAppPath()
{
	CString	strAppPath;
	TCHAR szModuleFileName[MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	if (NULL == GetModuleFileName(NULL, szModuleFileName, MAX_PATH))
		return FALSE;

	_tsplitpath( szModuleFileName, drive, dir, fname, ext );

	strAppPath = drive;
	strAppPath += dir;

	return strAppPath;
}
