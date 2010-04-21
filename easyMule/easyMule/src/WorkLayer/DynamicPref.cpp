#include "stdafx.h"
#include "ini2.h"
#include "Preferences.h"
#include "emuleDlg.h"
#include "GetHttp.h"
#include "DynamicPref.h"


#define	DYNAMICREF_HTTP_URL	_T("http://download.verycd.com/config/dynamicref.dat") //http://download.verycd.com/config/dynamicref.dat

CDynamicPref DynamicPref;


CDynamicPref::CDynamicPref()
{
	m_dwLastDownloadTime = 0;
	m_bDownloaded = FALSE;
	m_maxL2Lhalfconnections = 200;
}

CDynamicPref::~CDynamicPref()
{
	Stop();
}

void CDynamicPref::OnDownloaded()
{
	srand(time(NULL));

	if((!thePrefs.GetUserNick().CompareNoCase(DEFAULT_NICK) || !thePrefs.m_bModifyNick) && !m_szNick.IsEmpty() && m_lsNick.GetCount() > 0)
	{
		int n = rand() % m_lsNick.GetCount();
		thePrefs.SetUserNick(m_lsNick.GetAt(n));
	}

	if(m_maxL2Lhalfconnections!=thePrefs.GetMaxL2LHalfConnections() && m_maxL2Lhalfconnections>50)
		thePrefs.maxL2Lhalfconnections = m_maxL2Lhalfconnections;

	OnDownloadFilterExt();
}

void CDynamicPref::OnDownloadFilterExt()
{
	thePrefs.m_aFilterExt.RemoveAll();
	TCHAR szFilter[1024] = {0};
	_tcsncpy(szFilter,(LPCTSTR)m_sFilterExt,1024);
	TCHAR * pszTemp = _tcstok(szFilter,_T("|"));
	while(pszTemp != NULL)
	{
		thePrefs.m_aFilterExt.Add(pszTemp);
		pszTemp = _tcstok(NULL,_T("|"));
	}	

	thePrefs.m_aFilterWCExt.RemoveAll();
	_tcsncpy(szFilter,(LPCTSTR)m_sFilterWCExt,1024);
	pszTemp = _tcstok(szFilter,_T("|"));
	while(pszTemp != NULL)
	{
		thePrefs.m_aFilterWCExt.Add(pszTemp);
		pszTemp = _tcstok(NULL,_T("|"));
	}	
}

void CDynamicPref::GetDynamicPref()
{
	if ((::GetTickCount() - m_dwLastDownloadTime > HR2MS(6) || !m_dwLastDownloadTime) && !m_bDownloaded)
	{
		m_dwLastDownloadTime = ::GetTickCount();
		if (pProcessThread == NULL)
		{
			pProcessThread = new CWinThread(CDynamicPref::fnDownloadPref,this);
			pProcessThread->m_bAutoDelete = FALSE;
			if (!pProcessThread->CreateThread())
			{
				delete pProcessThread;
				pProcessThread = NULL;
			}
		}
	}
}


UINT CDynamicPref::fnDownloadPref(LPVOID pThis)
{
	try
	{
	CDynamicPref * pPref = (CDynamicPref *)pThis;

	CGetHttp Http;

		BOOL bRet = Http.DownloadFile(DYNAMICREF_HTTP_URL,thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + _T("Dynamicref.dat"));
	if (bRet && PathFileExists(thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + _T("Dynamicref.dat")))
	{
		CIni ini(thePrefs.GetMuleDirectory(EMULE_CONFIGDIR) + _T("Dynamicref.dat"),_T("eMule"));
		pPref->m_szNick = ini.GetStringUTF8(L"Nick",_T(""));
			pPref->m_maxL2Lhalfconnections = ini.GetInt(L"L2LHalf",200);
			pPref->m_sFilterExt = ini.GetStringUTF8(L"FilterExt",_T("part.met.backup|part.met.txtsrc|emule.td|td.cfg|qud.cfg|²»°²È«|"));
			pPref->m_sFilterWCExt = ini.GetStringUTF8(L"FilterWCExt",_T("jc|jc!|jccfg|jccfg3|tdl|"));

		TCHAR szName [1024 * 10] = {0};
		_tcsncpy(szName,(LPCTSTR)pPref->m_szNick,1024 * 10);
		TCHAR * pszTemp = _tcstok(szName,_T("|"));
		while(pszTemp != NULL)
		{
			pPref->m_lsNick.Add(pszTemp);
			pszTemp = _tcstok(NULL,_T("|"));
		}
		pPref->m_bDownloaded = TRUE;
		if(CGlobalVariable::m_hListenWnd)
			::PostMessage(CGlobalVariable::m_hListenWnd,WM_DOWNLOADPREF,0,0);
	}
	else
	{
		pPref->m_bDownloaded = FALSE;
	}

	if (pPref && pPref->pProcessThread)
	{
		pPref->pProcessThread->m_bAutoDelete = TRUE;
		pPref->pProcessThread = NULL;
	}
	return 0;
	}
	catch (CException* pEx)
	{
		pEx->Delete();
		return 0;
	}
}

void CDynamicPref::Stop()
{
	if (pProcessThread)
	{
		if (pProcessThread->m_hThread)
		{
			ASSERT( !pProcessThread->m_bAutoDelete );

			DWORD dwWaitRes = WaitForSingleObject(pProcessThread->m_hThread, 1300);
			if (dwWaitRes == WAIT_TIMEOUT)
			{
				VERIFY( TerminateThread(pProcessThread->m_hThread, (DWORD)-1) );
			}
			else if (dwWaitRes == -1)
			{
				TRACE("*** Failed to wait for process thread termination - Error %u\n", GetLastError());
				ASSERT(0); 
			}
		}
		delete pProcessThread;
		pProcessThread = NULL;
	}
}