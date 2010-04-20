#include "StdAfx.h"
#include ".\threadsmgr.h"

CThreadsMgr	theThreadsMgr;

CThreadsMgr::CThreadsMgr(void)
{
}

CThreadsMgr::~CThreadsMgr(void)
{
	m_mapRecs.RemoveAll();
}

void CThreadsMgr::CleanProc_WaitAndDelWinThd(HANDLE hThread)
{
	if (NULL == hThread)
		return;

	DWORD dwErr = WaitForSingleObject(hThread, 5000);

	if (WAIT_TIMEOUT == dwErr)
	{
		TerminateThread(hThread, 1);
	}

	return;
}

void CThreadsMgr::CleanProc_DelWinThd(HANDLE hThread)
{
	if (NULL == hThread)
		return;

	TerminateThread(hThread, 1);
	return;
}

CWinThread* CThreadsMgr::BegingThreadAndRecDown(CThreadsMgr::CLEAN_PROC cleanProc, AFX_THREADPROC pfnThreadProc, LPVOID pParam,
								   int nPriority, UINT nStackSize,
								   DWORD dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
	BOOL bResumeThread;
	bResumeThread = (dwCreateFlags & CREATE_SUSPENDED) ? FALSE : TRUE;

	dwCreateFlags |= CREATE_SUSPENDED;
	CWinThread *pThread = ::AfxBeginThread(pfnThreadProc, pParam, nPriority, nStackSize, dwCreateFlags, lpSecurityAttrs);

	if (NULL != pThread)
	{
		theThreadsMgr.RegThread(pThread->m_nThreadID, cleanProc);

		if (bResumeThread)
			pThread->ResumeThread();
	}

	return pThread;
}

CWinThread* CThreadsMgr::BegingThreadAndRecDown(CThreadsMgr::CLEAN_PROC cleanProc, CRuntimeClass* pThreadClass,
									  int nPriority, UINT nStackSize,
									  DWORD dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
	BOOL bResumeThread;
	bResumeThread = (dwCreateFlags & CREATE_SUSPENDED) ? FALSE : TRUE;

	dwCreateFlags |= CREATE_SUSPENDED;
	CWinThread *pThread = ::AfxBeginThread(pThreadClass, nPriority, nStackSize, dwCreateFlags, lpSecurityAttrs);

	if (NULL != pThread)
	{
		theThreadsMgr.RegThread(pThread->m_nThreadID, cleanProc);
		
		if (bResumeThread)
			pThread->ResumeThread();
	}

	return pThread;
}

void CThreadsMgr::RegThread(DWORD dwThreadId, CLEAN_PROC pfnCleanProc)
{
	ASSERT(NULL != pfnCleanProc);
	if (NULL == pfnCleanProc)
		return;
	
	CSingleLock	localLock(&m_cs, TRUE);
	m_mapRecs.SetAt(dwThreadId, pfnCleanProc);
}

void CThreadsMgr::UnregThread(DWORD dwThreadId)
{
	CSingleLock	localLock(&m_cs, TRUE);
	m_mapRecs.RemoveKey(dwThreadId);
}

void CThreadsMgr::CleanAllThreads()
{
	POSITION	pos;
	DWORD		dwThreadId;
	HANDLE		hThread;
	CLEAN_PROC	pfnCleanProc;

	CSingleLock	localLock(&m_cs, TRUE);
	pos = m_mapRecs.GetStartPosition();
	while (NULL != pos)
	{
		m_mapRecs.GetNextAssoc(pos, dwThreadId, pfnCleanProc);

		if (NULL != pfnCleanProc)
		{
			hThread = OpenThread(THREAD_ALL_ACCESS, TRUE, dwThreadId);
			pfnCleanProc(hThread);
			CloseHandle(hThread);
		}
	}
	m_mapRecs.RemoveAll();
}
