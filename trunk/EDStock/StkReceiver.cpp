// StkReceiver.cpp : implementation file
//
//		��Ʊ������մ��ڳ���
//	
//	
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Stockdrv.h"		// ͨ�ӿ����ݽṹ����
#include "Tongshi.h"		// ͨ�ӿ�ʹ�ú���
#include "STKDRV.h"
#include "StkReceiver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static	CSTKDRV	gSTOCKDLL;


HINSTANCE hInst=NULL;
typedef BOOL(WINAPI * MYFUNC)(LPTHREAD_START_ROUTINE, PVOID,ULONG);
MYFUNC MyQueueUserWorkItem=NULL;


CStkReceiver & AfxGetStkReceiver()
{
	static	CStkReceiver	gStkReceiver;
	return gStkReceiver;
}


/////////////////////////////////////////////////////////////////////////////
// CStkReceiver

#define	STKRECEIVER_TIMER_YONGXIN	1

CCriticalSection	g_mutexCommPacket;

// ������յ���Report���ݵ��߳�
DWORD WINAPI StkProcessReceiveReport(LPVOID lParam)
{	
	RCV_DATA *	pHeader = (RCV_DATA *) lParam;

	// ���䱸�����ݴ洢�ռ�
	COMMPACKET * pCommPacket = AllocCommPacket(CStock::dataReport, pHeader->m_nPacketNum);
	if (NULL == pCommPacket) return -1L;

	int nBufSize = pHeader->m_pReport[0].m_cbSize;
	PBYTE pBaseBuf = (PBYTE)&pHeader->m_pReport[0];
	for(int i=0; i<pHeader->m_nPacketNum; i++)
	{
		PRCV_REPORT_STRUCTEx pReport = (PRCV_REPORT_STRUCTEx)(pBaseBuf + nBufSize*i);
		if (convert_TSREPORT_to_REPORT(pReport, &(pCommPacket->m_pReport[i])))
		{
// 			TRACE("\n");
// 			TRACE(pReport->m_szName);
		}
	}

	if (CStock::dataReport == pCommPacket->m_dwDataType)
	{
		CSingleLock		lock(&g_mutexCommPacket,TRUE);

		// �޸�AfxGetStockContainer()
		for(DWORD i=0; i<pCommPacket->m_dwCount; i++)
		{
//			if(!strcmp(pCommPacket->m_pReport[i].m_szCode, "002167"))
			AfxGetStkReceiver().OnReceiveReport(&(pCommPacket->m_pReport[i]));
		}
		
		// �����������ļ�
		AfxGetDB().StoreReport(pCommPacket->m_pReport, pCommPacket->m_dwCount, FALSE);
		
		// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
		AfxGetStkReceiver().NotifyWndRcvData(CStock::dataReport, (LPARAM)pCommPacket);
		
		lock.Unlock();
	}

	// �ͷű������ݴ洢�ռ�
	FreeCommPacket(pCommPacket);

	return 0;
}

CStkReceiver::CStkReceiver()
{
	m_bFirst = TRUE;
	m_bTongshiRunning	=	FALSE;
}

CStkReceiver::~CStkReceiver()
{
}

BOOL CStkReceiver::CreateReceiver(CWnd * pParentWnd)
{
	return CreateEx(0,NULL,"",WS_OVERLAPPEDWINDOW|WS_CHILDWINDOW,
		CRect(0,0,0,0),pParentWnd,0,NULL);
}

void CStkReceiver::OnReceiveReport(REPORT * pReport)
{
	CStockContainer & container = AfxGetStockContainer();
	REPORT reportLast;
	if (!UpdateStockContainerByREPORT(container, pReport, TRUE, &reportLast))
		return;

	CStockInfo infoCur;

	// ������
	if (container.GetStockInfo(pReport->m_szCode, &infoCur, NULL))
	{
		if (0 != reportLast.m_time && reportLast.m_time < pReport->m_time
			&& pReport->m_fVolume - reportLast.m_fVolume >= 100000
			&& !infoCur.IsIndex() && !infoCur.IsBond() && !infoCur.IsFund() && !infoCur.IsBondIndex())
		{
			REPORT	report;
			memcpy(&report, pReport, sizeof(report));
			report.m_fVolume	=	pReport->m_fVolume - infoCur.m_reportLatest.m_fVolume;
			report.m_fAmount	=	pReport->m_fAmount - infoCur.m_reportLatest.m_fAmount;
			AfxGetDB().StoreReport(&report, 1, TRUE);
		}
	}

	// ָ�������仯
	if (0 == strncmp(pReport->m_szCode, STKLIB_CODE_MAIN, sizeof(pReport->m_szCode))
		|| 0 == strncmp(pReport->m_szCode, STKLIB_CODE_MAINSZN, sizeof(pReport->m_szCode)))
	{
		NotifyWndRcvAlarm(STKRCV_ALARM_REFRESH, 0);
	}

	// ����Ԥ���ڴ˴�����
	if (AfxGetAlarmContainer().OnReceiveReport(&infoCur, pReport, &reportLast))
	{
		NotifyWndRcvAlarm(STKRCV_ALARM_WARNING, 0);
	}
}

void CStkReceiver::AddRcvAlarmWnd(HWND hWnd)
{
	m_awndRcvAlarm.Add(hWnd);
}

BOOL CStkReceiver::RemoveRcvAlarmWnd(HWND hWnd)
{
	for(int i=0; i<m_awndRcvAlarm.GetSize(); i++)
	{
		if (m_awndRcvAlarm.ElementAt(i) == hWnd)
		{
			m_awndRcvAlarm.RemoveAt(i,1);
			return TRUE;
		}
	}
	return FALSE;
}

void CStkReceiver::NotifyWndRcvAlarm(WPARAM wParam, LPARAM lParam)
{
	for(int i=0; i<m_awndRcvAlarm.GetSize(); i++)
		::PostMessage(m_awndRcvAlarm[i], WM_APP_STKRECEIVER_ALARM, wParam, lParam);
}

void CStkReceiver::AddRcvDataWnd(HWND hWnd)
{
	m_awndRcvData.Add(hWnd);
}

BOOL CStkReceiver::RemoveRcvDataWnd(HWND hWnd)
{
	for(int i=0; i<m_awndRcvData.GetSize(); i++)
	{
		if (m_awndRcvData.ElementAt(i) == hWnd)
		{
			m_awndRcvData.RemoveAt(i,1);
			return TRUE;
		}
	}
	return FALSE;
}

void CStkReceiver::NotifyWndRcvData(WPARAM wParam, LPARAM lParam)
{
	for(int i=0; i<m_awndRcvData.GetSize(); i++)
		::SendMessage(m_awndRcvData[i], WM_APP_STKRECEIVER_DATA , wParam, lParam);
}

int CStkReceiver::RequestStockData(int nDataType/*CStock::DataType*/, CStockInfo * pStockInfo, int nSize, int nKType, int nDataCount)
{
	int nDataTypeTS = DataType_to_TSDataType(nDataType);
	
	if (nSize < 0)
		return 0;
	
	STOCK_STRUCTEx * pStocks = new STOCK_STRUCTEx[nSize+1];
	memset(pStocks, 0, sizeof(STOCK_STRUCTEx)*(nSize+1));
	
	for(int i=0; i<nSize; i++)
	{
		pStocks[i].m_type = (BYTE)pStockInfo[i].GetType();
		strncpy(pStocks[i].m_code, pStockInfo[i].GetStockCode(), sizeof(pStocks[i].m_code));
	}
	int nRet = gSTOCKDLL.RequestStockData(nDataTypeTS, pStocks, nSize, nKType, nDataCount);
	
	delete [] pStocks;
	return nRet;
}

int	CStkReceiver::RequestKData(CStock *pstock, int period)
{
	int nDataCount = 480;

	CStockInfo & info = pstock->GetStockInfo();
	CKData & kdata = pstock->GetKData(period);

	DWORD dateLatest = info.m_datebegin;
	if (kdata.GetSize() > 0)
		dateLatest = kdata.ElementAt(kdata.GetSize()-1).m_date;
	CSPTime	tmLatest;
	tmLatest.FromStockTime(dateLatest, CKData::IsDayOrMin(period));
	CSPTime tmInitial = CStDatabase::GetTimeInitial();
	if (tmLatest < tmInitial)
		tmLatest	=	tmInitial;
	CSPTime tmNow = CSPTime::GetCurrentTime();
	CSPTimeSpan span = tmNow - tmLatest;
	switch(period)
	{
	case CKData::ktypeDay:		nDataCount = span.GetDays()*5/7+1;	break;
	case CKData::ktypeWeek:		nDataCount = span.GetDays()/7+1;	break;
	case CKData::ktypeMonth:	nDataCount = span.GetDays()/30+1;	break;
	default: ;
	}
	if (nDataCount < 100) nDataCount = 300;
	return RequestStockData(CStock::dataK, &info, 1, period, nDataCount);
}

BOOL CStkReceiver::EngineBeginWorking(BOOL bShowSelectEngineDialog)
{
	return SendMessage(WM_APP_STKRECEIVER_INIT, !bShowSelectEngineDialog);	// ����ͨ�ӽ���ϵͳ
}

BOOL CStkReceiver::EngineSetup()
{
	return SendMessage(WM_APP_STKRECEIVER_SETUP);
}

BOOL CStkReceiver::EngineEndWorking()
{
	return SendMessage(WM_APP_STKRECEIVER_QUIT);
}

BOOL CStkReceiver::EngineIsWorking()
{
	return m_bTongshiRunning;
}

int CStkReceiver::RefreshStockContainer(CStockContainer & container, BOOL bAddIfNotExist)
{
	if (m_bTongshiRunning)
	{
		int	nMaxNumber	=	gSTOCKDLL.GetTotalNumber();
		ASSERT(nMaxNumber >= 0);
		if (nMaxNumber <= 0)
			return 0;

		int	nNO	=	0;
		while (TRUE)
		{
			RCV_REPORT_STRUCTEx	tsreport;
			memset(&tsreport,0,sizeof(tsreport));
			int	nRet	=	gSTOCKDLL.GetStockByNoEx(nNO, &tsreport);
			if (nRet <= 0)
				break;
			REPORT	report;
			if (convert_TSREPORT_to_REPORT(&tsreport, &report))
				UpdateStockContainerByREPORT(container, &report, bAddIfNotExist);

			nNO	++;
		}
		return nNO;
	}

	return 0;
}

int CStkReceiver::GetReport(CStockInfo &info, CReport &aReport)
{
	if (m_bTongshiRunning)
	{
		int	nMarket	=	SH_MARKET_EX;
		if (info.IsShangHai())
			nMarket	=	SH_MARKET_EX;
		else if (info.IsShenZhen())
			nMarket	=	SZ_MARKET_EX;
		RCV_REPORT_STRUCTEx tsreport;
		REPORT	 report;
		int	nRet	=	gSTOCKDLL.GetStockByCodeEx((char *)info.GetStockCode(), nMarket, &tsreport);
		if (nRet > 0 && 0 == strncmp(info.GetStockCode(),tsreport.m_szLabel,min(strlen(info.GetStockCode()),sizeof(tsreport.m_szLabel)))
			&& convert_TSREPORT_to_REPORT(&tsreport, &report))
		{
			UpdateStockInfoByREPORT(info, &report);
			return 1;
		}
	}

	return 0;
}

BEGIN_MESSAGE_MAP(CStkReceiver, CWnd)
	//{{AFX_MSG_MAP(CStkReceiver)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_APP_STKRECEIVER_INIT,OnStkReceiverInit)
	ON_MESSAGE(WM_APP_STKRECEIVER_SETUP,OnStkReceiverSetup)
	ON_MESSAGE(WM_APP_STKRECEIVER_QUIT,OnStkReceiverQuit)
	ON_MESSAGE(WM_APP_STKRECEIVER_TSDATA,OnStkReceiverTsdata)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStkReceiver message handlers

int CStkReceiver::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	hInst=LoadLibrary("Kernel32.DLL");
	if(hInst)
	{
		//ȡ��QueueUserWorkItem����ָ��
		MyQueueUserWorkItem=(MYFUNC)GetProcAddress(hInst,"QueueUserWorkItem");
		if(!MyQueueUserWorkItem)
		{
			TRACE("\n*************��ȡ������ַʧ��***************\n");
			FreeLibrary(hInst); 
			hInst = NULL;
			return -1;
		}
	}

	return 0;
}

void CStkReceiver::OnDestroy()
{
	OnStkReceiverQuit(0, 0);

	if(hInst!=NULL) FreeLibrary(hInst); 

	CWnd::OnDestroy();
}

LRESULT CStkReceiver::OnStkReceiverInit(WPARAM wParam, LPARAM lParam)
{
	// ͨ�Ӽ������豸
	CString strProvider, strDriver;
	strProvider = AfxRegQueryString(HKEY_LOCAL_MACHINE, _T("Software"), _T("stockdrv"), "", "", _T("Provider"), "Tongshi");
	strDriver = AfxRegQueryString(HKEY_LOCAL_MACHINE, _T("Software"), _T("stockdrv"), "", "", _T("Driver"), "Stock.dll");
	
	// ���ݹ�������
	gSTOCKDLL.Stock_Quit(m_hWnd);
	gSTOCKDLL.UnloadDriver();
	if (gSTOCKDLL.LoadDriver(strDriver)
		&& gSTOCKDLL.GetStockDrvInfo(RI_SUPPORTEXTHQ, NULL)
		&& gSTOCKDLL.Stock_Init(m_hWnd,WM_APP_STKRECEIVER_TSDATA,RCV_WORK_SENDMSG) > 0)
		m_bTongshiRunning	=	TRUE;
	else if (!wParam)
		AfxMessageBox("�޷����ؽ���ϵͳ��", MB_OK | MB_ICONINFORMATION);

	return TRUE;
}

LRESULT CStkReceiver::OnStkReceiverSetup(WPARAM wParam, LPARAM lParam)
{
	return gSTOCKDLL.SetupReceiver(TRUE);
}

LRESULT CStkReceiver::OnStkReceiverQuit(WPARAM wParam, LPARAM lParam)
{
	gSTOCKDLL.Stock_Quit(m_hWnd);
	m_bTongshiRunning	=	FALSE;
	gSTOCKDLL.UnloadDriver();

	return TRUE;
}

// ͨ�Ӵ�������
LRESULT CStkReceiver::OnStkReceiverTsdata(
		UINT wFileType,				// �ļ�����, �μ� StockDrv.H
		LONG lPara)
{
	int	i = 0;
	PBYTE pFileBuf = NULL;
	PBYTE pBuffx = NULL;
	PGZLXBINDATA pGZLX = NULL;
	RCV_DATA *	pHeader = (RCV_DATA *) lPara;

	// ������
	ASSERT(pHeader && pHeader->m_pData);
	if (!pHeader || !pHeader->m_pData)
		return -1L;

	if (pHeader->m_nPacketNum <= 0 && !pHeader->m_bDISK)
		return -1L;

	//	���ڴ����������������,���齫���ݱ���,����һ�̴߳���
	switch(wFileType)
	{
	case RCV_REPORT:
		{
			// ������
			if (pHeader->m_nPacketNum <= 0 || pHeader->m_bDISK)
				return -1L;
			
			MyQueueUserWorkItem(StkProcessReceiveReport, pHeader, WT_EXECUTEDEFAULT);
		}
		break;
	
	case RCV_FILEDATA:
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:						// ����������
			{
				RCV_HISTORY_STRUCTEx * pDay = pHeader->m_pDay;
				int	nPacketNum	=	pHeader->m_nPacketNum;
				CKData	kdata(CKData::ktypeDay);
				CString	sCode;
				DWORD	dwMarket = CStock::marketUnknown;

				// ��ȡ�ļ�
				if (pHeader->m_bDISK)
				{
					pDay	=	NULL;
					CFile	file;
					if (file.Open(pHeader->m_File.m_szFileName,CFile::modeRead))
					{
						ASSERT(file.GetLength() == pHeader->m_File.m_dwLen);
						nPacketNum	=	(int)(file.GetLength()/sizeof(RCV_HISTORY_STRUCTEx));
						if (nPacketNum > 0)
							pDay = new RCV_HISTORY_STRUCTEx[nPacketNum];
						if (pDay)
							file.Read(pDay, sizeof(RCV_HISTORY_STRUCTEx)*nPacketNum);
						file.Close();
					}
				}

				// ���䱸�����ݴ洢�ռ�
				COMMPACKET * pCommPacket = AllocCommPacket(CStock::dataK, nPacketNum);
				if (NULL == pCommPacket)
					return -1L;

				// ��������
				DWORD	dwCount	=	0;
				for(i=0; i<nPacketNum && pDay; i++)
				{
					if (pDay[i].m_head.m_dwHeadTag == EKE_HEAD_TAG)
					{
						if (kdata.GetSize() > 0)
						{
							kdata.AutoSetKType();
							UpdateStockContainerByKData(AfxGetStockContainer(), sCode, kdata);
							AfxGetDB().InstallKData(kdata);
							kdata.RemoveAll();
						}
						sCode		=	pDay[i].m_head.m_szLabel;
						dwMarket	=	TSMarket_to_Market(pDay[i].m_head.m_wMarket);
						continue;
					}
					convert_TSHISTORY_to_KDATA(dwMarket, sCode, &(pDay[i]), &(pCommPacket->m_pKdata[dwCount]));
					kdata.Add(pCommPacket->m_pKdata[dwCount]);
					dwCount	++;
				}
				pCommPacket->m_dwCount	=	dwCount;
				if (pHeader->m_bDISK && pDay)
					delete [] pDay;
				if (kdata.GetSize() > 0)
				{
					kdata.AutoSetKType();
					UpdateStockContainerByKData(AfxGetStockContainer(), sCode, kdata);
					AfxGetDB().InstallKData(kdata);
				}

				// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
				for(i=0; i<m_awndRcvData.GetSize(); i++)
					::SendMessage(m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataK, (LPARAM)pCommPacket);
	
				// �ͷű������ݴ洢�ռ�
				FreeCommPacket(pCommPacket);
			}
			break;

		case FILE_MINUTE_EX:						// ������������
		case 83:	// ���ֱ�����
			{
				RCV_MINUTE_STRUCTEx * pMin = pHeader->m_pMinute;
				ASSERT(pMin->m_head.m_dwHeadTag == EKE_HEAD_TAG);
				CMinute	minute;
				BOOL bAutoSaveKDataMin = AfxGetProfile().GetAutoSaveKDataMin(); // ��Ϊ����K�߱���
				CString	sCode;
				DWORD	dwMarket = CStock::marketUnknown;
				
				// ���䱸�����ݴ洢�ռ�
				COMMPACKET * pCommPacket = AllocCommPacket(CStock::dataMinute, pHeader->m_nPacketNum);
				if (NULL == pCommPacket)
					return -1L;

				DWORD	dwCount	=	0;
				for(i=0; i<pHeader->m_nPacketNum; i++)
				{
					if (pMin[i].m_head.m_dwHeadTag == EKE_HEAD_TAG)
					{
						if (minute.GetSize() > 0)
						{
							CKData	kdMin(CKData::ktypeMin5);
							if (minute.ToKData(kdMin))
								AfxGetDB().InstallKData(kdMin);
							minute.RemoveAll();
						}
						sCode		=	pMin[i].m_head.m_szLabel;
						dwMarket	=	TSMarket_to_Market(pMin[i].m_head.m_wMarket);
						continue;
					}
					convert_TSMINUTE_to_MINUTE(dwMarket, sCode, &(pMin[i]), &(pCommPacket->m_pMinute[dwCount]));
					if (bAutoSaveKDataMin)
						minute.Add(pCommPacket->m_pMinute[dwCount]);
					dwCount	++;
				}
				pCommPacket->m_dwCount	=	dwCount;
				if (minute.GetSize() > 0)
				{
					CKData	kdMin(CKData::ktypeMin5);
					if (minute.ToKData(kdMin))
						AfxGetDB().InstallKData(kdMin);
				}

				// �����������ļ�
				AfxGetDB().StoreMinute(pCommPacket->m_pMinute, pCommPacket->m_dwCount);

				// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
				for(i=0; i<m_awndRcvData.GetSize(); i++)
					::SendMessage(m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataMinute, (LPARAM)pCommPacket);

				// �ͷű������ݴ洢�ռ�
				FreeCommPacket(pCommPacket);
			}
			break;

		case FILE_POWER_EX:							// �����Ȩ����
			{
				RCV_POWER_STRUCTEx * pPower = pHeader->m_pPower;
				int	nPacketNum	=	pHeader->m_nPacketNum;
				CDRData	drdata;
				CString	sCode;
				DWORD	dwMarket = CStock::marketUnknown;

				if (pHeader->m_bDISK)
				{
					pPower	=	NULL;
					CFile	file;
					if (file.Open(pHeader->m_File.m_szFileName,CFile::modeRead))
					{
						ASSERT(file.GetLength() == pHeader->m_File.m_dwLen);
						nPacketNum	=	(int)(file.GetLength()/sizeof(RCV_POWER_STRUCTEx));
						if (nPacketNum > 0)
							pPower = new RCV_POWER_STRUCTEx[nPacketNum];
						if (pPower)
							file.Read(pPower, sizeof(RCV_POWER_STRUCTEx)*nPacketNum);
						file.Close();
					}
				}

				// ���䱸�����ݴ洢�ռ�
				COMMPACKET * pCommPacket = AllocCommPacket(CStock::dataDR, nPacketNum);
				if (NULL == pCommPacket)
					return -1L;

				DWORD	dwCount	=	0;
				for(i=0; i<nPacketNum && pPower; i++)
				{
					if (pPower[i].m_head.m_dwHeadTag == EKE_HEAD_TAG)
					{
						if (drdata.GetSize() > 0)
							AfxGetDB().InstallDRData(drdata);
						drdata.RemoveAll();
						sCode		=	pPower[i].m_head.m_szLabel;
						dwMarket	=	TSMarket_to_Market(pPower[i].m_head.m_wMarket);
						continue;
					}
					convert_TSPOWER_to_DRDATA(dwMarket, sCode, &(pPower[i]), &(pCommPacket->m_pDrdata[dwCount]));
					drdata.Add(pCommPacket->m_pDrdata[dwCount]);
					dwCount	++;
				}
				if (pHeader->m_bDISK && pPower)
					delete [] pPower;
				if (drdata.GetSize() > 0)
					AfxGetDB().InstallDRData(drdata);
				pCommPacket->m_dwCount	=	dwCount;

				// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
				for(i=0; i<m_awndRcvData.GetSize(); i++)
					::SendMessage(m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataDR, (LPARAM)pCommPacket);

				// �ͷű������ݴ洢�ռ�
				FreeCommPacket(pCommPacket);
			}
			break;

		case FILE_MULTISORT_EX:					// �ۺ�����
			{
				RCV_MULTISORT_STRUCTEx * pMultisort = pHeader->m_pMultisort;
				
				// ���䱸�����ݴ洢�ռ�
				COMMPACKET * pCommPacket = AllocCommPacket(CStock::dataMultisort, pHeader->m_nPacketNum);
				if (NULL == pCommPacket)
					return -1L;

				for(i=0; i<pHeader->m_nPacketNum; i++)
				{
					convert_TSMULTISORT_to_MULTISORT(&(pMultisort[i]), &(pCommPacket->m_pMultisort[i]));
				}
				pCommPacket->m_dwCount	=	pHeader->m_nPacketNum;

				// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
				for(i=0; i<m_awndRcvData.GetSize(); i++)
					::SendMessage(m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataMultisort, (LPARAM)pCommPacket);

				// �ͷű������ݴ洢�ռ�
				FreeCommPacket(pCommPacket);
			}
			break;

		case FILE_BASE_EX:						// Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
			{
				if (pHeader->m_bDISK)
				{
					AfxGetDB().InstallBaseText(pHeader->m_File.m_szFileName, AfxGetFileTitle(pHeader->m_File.m_szFileName));
				}
				else
				{
					AfxGetDB().InstallBaseText(LPCTSTR(pHeader->m_pData), pHeader->m_File.m_dwLen, AfxGetFileTitle(pHeader->m_File.m_szFileName));
				}

				// ���䱸�����ݴ洢�ռ�
				COMMPACKET * pCommPacket = AllocCommPacket(CStock::dataCode, 1);
				if (NULL == pCommPacket)
					return -1L;
				CString	sTemp	=	AfxGetFileTitleNoExt(pHeader->m_File.m_szFileName);
				strncpy(pCommPacket->m_pStockcode[0].m_szCode, sTemp, min(sizeof(pCommPacket->m_pStockcode[0].m_szCode)-1,sTemp.GetLength()));
				pCommPacket->m_pStockcode[0].m_dwMarket	=	CStock::marketUnknown;

				// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
				for(i=0; i<m_awndRcvData.GetSize(); i++)
					::SendMessage(m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataBasetext, (LPARAM)pCommPacket);

				// �ͷű������ݴ洢�ռ�
				FreeCommPacket(pCommPacket);
			}
			break;

		case FILE_NEWS_EX:						// ������,��������m_szFileName����Ŀ¼������
			if (pHeader->m_bDISK)
			{
				AfxGetDB().InstallNewsText(pHeader->m_File.m_szFileName, AfxGetFileTitle(pHeader->m_File.m_szFileName));
			}
			else
			{
				AfxGetDB().InstallNewsText(LPCTSTR(pHeader->m_pData), pHeader->m_File.m_dwLen, AfxGetFileTitle(pHeader->m_File.m_szFileName));
			}
			// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
			for(i=0; i<m_awndRcvData.GetSize(); i++)
				::SendMessage(m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataNews, NULL);
			break;

		case FILE_HTML_EX:						// HTML�ļ�,m_szFileNameΪURL
			AfxGetProfile().AddHTMLURLToExplorer(pHeader->m_File.m_szFileName);
			break;

		case FILE_SOFTWARE_EX:
			TRACE("%s\n",pHeader->m_File.m_szFileName);
			TRACE("%d\n",pHeader->m_File.m_dwLen);
			pBuffx = (PBYTE)pHeader->m_pData;
			break;

		case FILE_SHAZQDATA_EX:		//h�Ϻ���ծ���۽���
			break;
		case FILE_TYPE_RES:
			TRACE("MSG: CStkReceiver::OnStkDataOK, Replaced data \n");
			break;
		default:
			TRACE("Msg: CStkReceiver::OnStkReceiverData,Unkonw data type\n");
		}
		break;
	}

	return 0L;
}
