#ifndef __DYNAMICPREF__H__
#define __DYNAMICPREF__H__

#include "UIMessage.h"
#define WM_DOWNLOADPREF				(WM_UIMSG_BASE + 200)

class CDynamicPref
{
public:
	CDynamicPref();
	virtual ~CDynamicPref();

public:
	static UINT AFX_CDECL fnDownloadPref(LPVOID pThis);
	void GetDynamicPref();
	void OnDownloaded();
	void OnDownloadFilterExt();
	void Stop();

public:
	CWinThread* pProcessThread;	
	DWORD m_dwLastDownloadTime;
	BOOL m_bDownloaded;

public:
	CString m_szNick;
	CStringArray m_lsNick;
	int			 m_maxL2Lhalfconnections;
	CString	m_sFilterExt;
	CString m_sFilterWCExt; 
};

extern CDynamicPref DynamicPref;
#endif