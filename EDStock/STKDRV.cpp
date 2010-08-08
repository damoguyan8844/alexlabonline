
#include "stdafx.h"
#include "STKDRV.h"

CSTKDRV::CSTKDRV()
{
	m_hSTKDrv = NULL;

	m_pfnStock_Init = NULL;
	m_pfnStock_Quit = NULL;
	m_pfnGetTotalNumber = NULL;
	m_pfnGetStockByNoEx = NULL;
	m_pfnGetStockByCodeEx = NULL;
	m_pfnSetupReceiver = NULL;
	m_pfnGetStockDrvInfo = NULL;

	m_pfnGetStockMin = NULL;
	m_pfnGetStockPRP = NULL;

	// ����
	m_pfnSCInit = NULL;
	m_pfnSCQuit = NULL;
	m_pfnSCGetInfo = NULL;
	m_pfnSCAskData = NULL;
	m_pfnSCStockInit = NULL;
}

CSTKDRV::~CSTKDRV()
{
}

BOOL CSTKDRV::LoadDriver(LPCTSTR lpszDrvPath)
{
	ASSERT(NULL != lpszDrvPath);
	CString	strLibrary = lpszDrvPath;
	if (strLibrary.GetLength() <= 0)
		return FALSE;

	m_hSTKDrv = LoadLibrary(strLibrary);
	if (!m_hSTKDrv) 
		return FALSE;

	m_pfnStock_Init = (int (WINAPI*)(HWND,UINT,int)) GetProcAddress(m_hSTKDrv,"Stock_Init");
	m_pfnStock_Quit = (int (WINAPI*)(HWND)) GetProcAddress(m_hSTKDrv,"Stock_Quit");
	m_pfnGetTotalNumber = (int (WINAPI*)())GetProcAddress(m_hSTKDrv,"GetTotalNumber");
	m_pfnGetStockByNoEx = (int (WINAPI*)(int,RCV_REPORT_STRUCTEx *))GetProcAddress(m_hSTKDrv,"GetStockByNoEx");
	m_pfnGetStockByCodeEx = (int (WINAPI*)(char *,int,RCV_REPORT_STRUCTEx *))GetProcAddress(m_hSTKDrv,"GetStockByCodeEx");
	m_pfnSetupReceiver = (int (WINAPI*)(BOOL))GetProcAddress(m_hSTKDrv,"SetupReceiver");
	m_pfnGetStockDrvInfo = (DWORD (WINAPI*)(int,void *))GetProcAddress(m_hSTKDrv,"GetStockDrvInfo");

	m_pfnGetStockMin = (int (WINAPI*)(char*, int))GetProcAddress(m_hSTKDrv, "GetStockMin");
	m_pfnGetStockPRP = (int (WINAPI*)(char*, int))GetProcAddress(m_hSTKDrv, "GetStockPRP");

	m_pfnSCInit = (BOOL (WINAPI*)(void*, int))GetProcAddress(m_hSTKDrv, "SCInit");
	m_pfnSCQuit = (void (WINAPI*)(void))GetProcAddress(m_hSTKDrv, "SCQuit");
	m_pfnSCGetInfo = (UINT (WINAPI*)(TSCInfo, void*, int*))GetProcAddress(m_hSTKDrv, "SCGetInfo");
	m_pfnSCAskData = (void (WINAPI*)(void*, int))GetProcAddress(m_hSTKDrv, "SCAskData");
	m_pfnSCStockInit = (BOOL (WINAPI*)(HWND, UINT, int))GetProcAddress(m_hSTKDrv, "SCStockInit");

	return TRUE;
}

void CSTKDRV::UnloadDriver()
{
	if (m_hSTKDrv)
	{
		FreeLibrary(m_hSTKDrv);
		m_hSTKDrv = NULL;

		m_pfnStock_Init = NULL;
		m_pfnStock_Quit = NULL;
		m_pfnGetTotalNumber = NULL;
		m_pfnGetStockByNoEx = NULL;
		m_pfnGetStockByCodeEx = NULL;
		m_pfnSetupReceiver = NULL;
		m_pfnGetStockDrvInfo = NULL;

		m_pfnGetStockMin = NULL;
		m_pfnGetStockPRP = NULL;

		// ����
		m_pfnSCInit = NULL;
		m_pfnSCQuit = NULL;
		m_pfnSCGetInfo = NULL;
		m_pfnSCAskData = NULL;
		m_pfnSCStockInit = NULL;
	}
}

int CSTKDRV::Stock_Init(HWND hWnd, UINT uMsg, int nWorkMode)
{
	if (!m_pfnStock_Init)
		return -1;

	 return((*m_pfnStock_Init)(hWnd, uMsg, nWorkMode));
}

int CSTKDRV::Stock_Quit(HWND hWnd)
{
	if (!m_pfnStock_Quit)
		return -1;

	return((*m_pfnStock_Quit)(hWnd));
}

int CSTKDRV::GetTotalNumber()
{
	if (!m_pfnGetTotalNumber)
		return 0;

	return((*m_pfnGetTotalNumber)());
}

int CSTKDRV::GetStockByNoEx(int nNo, RCV_REPORT_STRUCTEx* pBuf)
{
	if (!m_pfnGetStockByNoEx)
		return 0;

	return((*m_pfnGetStockByNoEx)(nNo, pBuf));
}

int CSTKDRV::GetStockByCodeEx(char* pszStockCode, int nMarket, RCV_REPORT_STRUCTEx* pBuf)
{
	if (!m_pfnGetStockByCodeEx)
		return 0;

	return((*m_pfnGetStockByCodeEx)(pszStockCode, nMarket, pBuf));
}

int CSTKDRV::SetupReceiver(BOOL bShowWindow)
{
	if (!m_pfnSetupReceiver)
		return -1;

	return((*m_pfnSetupReceiver)(bShowWindow));
}

DWORD CSTKDRV::GetStockDrvInfo(int nInfo, void* pBuf)
{
	if (!m_pfnGetStockDrvInfo) 
		return 0;

	return((*m_pfnGetStockDrvInfo)(nInfo, pBuf));
}

int CSTKDRV::GetStockMin(char* StockCode, int nMarket)
{
	if (!m_pfnGetStockMin)
		return 0;

	return ((*m_pfnGetStockMin)(StockCode, nMarket));
}

int CSTKDRV::GetStockPRP(char* StockCode, int nMarket)
{
	if (!m_pfnGetStockPRP)
		return 0;

	return ((*m_pfnGetStockPRP)(StockCode, nMarket));
}

BOOL CSTKDRV::SCInit(void* pInit, int nSize)
{
	if (m_pfnSCInit)
		return (*m_pfnSCInit)(pInit, nSize);

	return FALSE;
}

void CSTKDRV::SCQuit()
{
	if (m_pfnSCQuit)
		(*m_pfnSCQuit)();
}

UINT CSTKDRV::SCGetInfo(TSCInfo Info, void* pData, int* nSize)
{
	if (m_pfnSCGetInfo)
		return (*m_pfnSCGetInfo)(Info, pData, nSize);

	return 0;
}

void CSTKDRV::SCAskData(void* pAsk, int nSize)
{
	if (m_pfnSCAskData)
		(*m_pfnSCAskData)(pAsk, nSize);
}

BOOL CSTKDRV::SCStockInit(HWND hWnd, UINT Msg, int nWorkMode)
{
	if (!m_pfnSCStockInit)
		return FALSE;

	return ((*m_pfnSCStockInit)(hWnd, Msg, nWorkMode));
}

int CSTKDRV::RequestStockData(int nDataType, STOCK_STRUCTEx * pStocks, int nSize, int nKType, int nDataCount)
{
	if (!m_pfnRequestStockData)
		return 0;
	
	return((*m_pfnRequestStockData)(nDataType,pStocks,nSize,nKType,nDataCount));
}
