
#pragma once

#include "Stockdrv.h"
#include "SCStkdrv.h"

class CSTKDRV  
{
public:
	CSTKDRV();
	virtual ~CSTKDRV();

protected:
	HINSTANCE m_hSTKDrv;

protected:
	int (WINAPI* m_pfnStock_Init)(HWND hWnd, UINT Msg, int nWorkMode);
	int (WINAPI* m_pfnStock_Quit)(HWND hWnd);
	int	(WINAPI* m_pfnGetTotalNumber)();													
	int (WINAPI* m_pfnGetStockByNoEx)(int, RCV_REPORT_STRUCTEx*);
	int (WINAPI* m_pfnGetStockByCodeEx)(char*, int, RCV_REPORT_STRUCTEx*);
	int	(WINAPI* m_pfnSetupReceiver)(BOOL bSetup);
	DWORD (WINAPI* m_pfnGetStockDrvInfo)(int nInfo, void* pBuf);

	int (WINAPI* m_pfnGetStockMin)(char* StockCode, int nMarket);
	int (WINAPI* m_pfnGetStockPRP)(char* StockCode, int nMarket);
	int (WINAPI* m_pfnRequestStockData)(int nDataType, STOCK_STRUCTEx * pStocks, int nSize, int nKType, int nDataCount);

	// Êý³©
	BOOL (WINAPI* m_pfnSCInit)(void* pAsk, int nSize);
	void (WINAPI* m_pfnSCQuit)(void);
	UINT (WINAPI* m_pfnSCGetInfo)(TSCInfo Info, void* pData, int* nSize);
	void (WINAPI* m_pfnSCAskData)(void* pAsk, int nSize);
	BOOL (WINAPI* m_pfnSCStockInit)(HWND hWnd, UINT Msg, int nWorkMode);

public:
	virtual	BOOL LoadDriver(LPCTSTR lpszDrvPath);
	virtual	void UnloadDriver();

public:
	virtual	int Stock_Init(HWND hWnd,UINT uMsg, int nWorkMode);
	virtual	int Stock_Quit(HWND hWnd);
	virtual	int GetTotalNumber();
	virtual	int GetStockByNoEx(int nNo, RCV_REPORT_STRUCTEx* pBuf);
	virtual	int GetStockByCodeEx(char* pszStockCode, int nMarket, RCV_REPORT_STRUCTEx* pBuf);
	virtual	int SetupReceiver(BOOL bShowWindow);
	virtual	DWORD GetStockDrvInfo(int nInfo, void* pBuf);

	virtual int GetStockMin(char* StockCode, int nMarket);
	virtual int GetStockPRP(char* StockCode, int nMarket);
	virtual	int	RequestStockData(int nDataType, STOCK_STRUCTEx * pStocks, int nSize, int nKType, int nDataCount);

public:
	virtual BOOL SCInit(void* pInit, int nSize);
	virtual void SCQuit();
	virtual UINT SCGetInfo(TSCInfo Info, void* pData, int* nSize);
	virtual void SCAskData(void* pAsk, int nSize);
	virtual BOOL SCStockInit(HWND hWnd, UINT Msg, int nWorkMode);
};
