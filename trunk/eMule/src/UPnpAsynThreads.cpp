#include "StdAfx.h"
#include ".\upnpasynthreads.h"
#include <AfxMt.h>
#include "ThreadsMgr.h"

CUPnpAsynThreads::CUPnpAsynThreads(void)
{
}

CUPnpAsynThreads::~CUPnpAsynThreads(void)
{
}

void CUPnpAsynThreads::AddNatPortMappingAsyn(CUPnpMgr	*pMgr,
											const CUPnpNatMapping &mapping,
											HWND hNotifyWnd,
											UINT uNotifyMessage,
											BOOL bRetryRand,
											DWORD dwCustomParam,
											BOOL bHasCleanedFillupBug)
{
	CUPnpAsynThreadsParam	*pParam = new CUPnpAsynThreadsParam;
	pParam->pUpnpMgr = pMgr;
	pParam->mapping = mapping;
	pParam->notifyParam.hwnd = hNotifyWnd;
	pParam->notifyParam.message = uNotifyMessage;
	pParam->bRetryRand = bRetryRand;
	pParam->dwCustomParam = dwCustomParam;
	pParam->bHasCleanedFillupBug = bHasCleanedFillupBug;

	//MODIFIED by VC-fengwen 2007/08/23 <begin> : ���̼߳ӵ��̹߳�����Ա�رճ���ʱ��ֹ��
	//::AfxBeginThread(CUPnpAsynThreads::AddNATPortMappingAsynProc, (LPVOID) pParam);
	CThreadsMgr::BegingThreadAndRecDown(CThreadsMgr::CleanProc_WaitAndDelWinThd,
										CUPnpAsynThreads::AddNATPortMappingAsynProc, (LPVOID) pParam);
	//MODIFIED by VC-fengwen 2007/08/23 <end> : ���̼߳ӵ��̹߳�����Ա�رճ���ʱ��ֹ��
}

UINT AFX_CDECL CUPnpAsynThreads::AddNATPortMappingAsynProc(LPVOID lpParam)
{
	//MODIFIED by VC-fengwen 2007/08/23 <begin> : ���̼߳ӵ��̹߳�����Ա�رճ���ʱ��ֹ��
	CUnregThreadAssist	uta(GetCurrentThreadId());
	//MODIFIED by VC-fengwen 2007/08/23 <end> : ���̼߳ӵ��̹߳�����Ա�رճ���ʱ��ֹ��

	CUPnpAsynThreadsParam *pThreadParam = (CUPnpAsynThreadsParam*) lpParam;
	if (NULL == pThreadParam)
		return 0;
	if (NULL == pThreadParam->pUpnpMgr)
		return 0;

	CSingleLock		localLock(pThreadParam->pUpnpMgr->GetSyncObject(), TRUE);

	CUPnpAsynThreadsResult	*pResult = new CUPnpAsynThreadsResult;
	pResult->dwCustomParam = pThreadParam->dwCustomParam;

	//	���EntryFillup��Bug
	if (!pThreadParam->bHasCleanedFillupBug
		&& pThreadParam->pUpnpMgr->CleanedFillupBug())
	{
		pResult->bCleanedFillupBug = TRUE;
	}
	else
		pResult->bCleanedFillupBug = FALSE;

	//	����ϴγ�������ʱû�гɹ�ɾ����ӳ��
	static bool bCleanedMappingInLastRun = false;
	if (!bCleanedMappingInLastRun)
	{
		pThreadParam->pUpnpMgr->ReadAddedMappingFromFile();
		pThreadParam->pUpnpMgr->CleanupAllEverMapping();
		bCleanedMappingInLastRun = true;
	}

	
	//	��Ӷ˿�ӳ��
	HRESULT		hr;
	hr = pThreadParam->pUpnpMgr->AddNATPortMapping(pThreadParam->mapping, pThreadParam->bRetryRand);
	pResult->wInternalPort = pThreadParam->mapping.m_wInternalPort;
	pResult->wExternalPort = pThreadParam->mapping.m_wExternalPort;
	if (E_UNAT_ACTION_HTTP_ERRORCODE == hr)
		pResult->dwActionErrorCode = pThreadParam->pUpnpMgr->GetLastActionErrorCode();
	else
		pResult->dwActionErrorCode = 0;
	
	Sleep(2000); //ADDED by VC-fengwen on 2007/09/04 : Ϊ��ֹ·������û׼���ã���һ��ʱ���ٷ��ͽ����

	//	���ͽ��
	if (! ::PostMessage(pThreadParam->notifyParam.hwnd,
						pThreadParam->notifyParam.message,
						hr, (LPARAM) pResult))
	{
		delete pResult;
		pResult = NULL;
	}

	delete pThreadParam;
	pThreadParam = NULL;


	return 0;
}

void CUPnpAsynThreads::CleanMapedPortQuickly(CUPnpMgr *pMgr)
{
	int		iOldTimeout;

	//MODIFIED by VC-fengwen 2007/08/23 <begin> : ����̱߳�����ִ�в�����������ٽ�����
	//CSingleLock		localLock(pMgr->GetSyncObject(), TRUE);

	if (!pMgr->TryLock())
		return;
	//MODIFIED by VC-fengwen 2007/08/23 <end> : ����̱߳�����ִ�в�����������ٽ�����
	
	iOldTimeout = pMgr->GetActionTimeout();
	pMgr->SetActionTimeout(0);

	pMgr->CleanupAllEverMapping();

	pMgr->SetActionTimeout(iOldTimeout);

	//ADDED by VC-fengwen 2007/08/23 <begin> : ����̱߳�����ִ�в�����������ٽ�����
	pMgr->Unlock();
	//ADDED by VC-fengwen 2007/08/23 <end> : ����̱߳�����ִ�в�����������ٽ�����
}

