#pragma once
#include "log.h"

class CDebugLog :
	public CLogFile
{
public:
	CDebugLog(void);
	~CDebugLog(void);

	static CString GetAppPath();
};

#ifdef _LOGED_RELEASE
extern CDebugLog theDebugLog;
#endif