// RealTimeView.cpp : implementation of the CRealTimeView class
//

#include "stdafx.h"
#include "RealTimeView.h"

#include "memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView

#define	RTV_TIMER_REFRESHBIGTRADE	1
#define	RTV_TIMER_REFRESH			2

#define	IDC_REALTIMEVIEW_REPORTTAB	200

#define	RT_REPORTTAB_QUOTE		0	// 笔
#define	RT_REPORTTAB_PRICE		1	// 价
#define	RT_REPORTTAB_MINUTE		2	// 分
#define	RT_REPORTTAB_BUYSELLEX	3	// 盘
#define	RT_REPORTTAB_VALUE		4	// 值
#define	RT_REPORTTAB_DISTRIBUTE	5	// 筹
#define	RT_REPORTTAB_BIGTRADE	6	// 单


IMPLEMENT_DYNCREATE(CRealTimeView, CView)

BEGIN_MESSAGE_MAP(CRealTimeView, CView)
	//{{AFX_MSG_MAP(CRealTimeView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
// 	ON_COMMAND(ID_REALTIME_PREV, OnRealtimePrev)
// 	ON_COMMAND(ID_REALTIME_NEXT, OnRealtimeNext)
// 	ON_COMMAND(ID_REALTIME_LEFT, OnRealtimeLeft)
// 	ON_COMMAND(ID_REALTIME_RIGHT, OnRealtimeRight)
// 	ON_COMMAND(ID_REALTIME_HOME, OnRealtimeHome)
// 	ON_COMMAND(ID_REALTIME_END, OnRealtimeEnd)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
//	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
	ON_MESSAGE(WM_APP_STKRECEIVER_DATA, OnStkReceiverData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView construction/destruction

CRealTimeView::CRealTimeView()
{
	m_nMultiStockCount	=	1;

	ASSERT(RTV_MAX_MULTICOUNT > 0);
}

CRealTimeView::~CRealTimeView()
{
}

void CRealTimeView::ShowMultiStock(int nMultiStockCount)
{
	ASSERT(1 == nMultiStockCount || 2 == nMultiStockCount
			|| 4 == nMultiStockCount || 6 == nMultiStockCount
			|| 9 == nMultiStockCount);
	if(1 != nMultiStockCount && 2 != nMultiStockCount
			&& 4 != nMultiStockCount && 6 != nMultiStockCount
			&& 9 != nMultiStockCount)
		return;
	if(nMultiStockCount == m_nMultiStockCount)
		return;

	m_nMultiStockCount	=	nMultiStockCount;

	int	nDrawMode	=	m_realtime[0].GetDrawMode();
	int	nTechLine	=	m_realtime[0].GetDrawTechLine();
	int	nReportWhat	=	m_realtime[0].GetReportWhat();
	if(1 == m_nMultiStockCount)
	{
		for(int i = 0; i < RTV_MAX_MULTICOUNT; i++)
			m_realtime[i].SetDrawMode(nDrawMode, nTechLine, CRealTime::reportQuote);
		//m_wndReportTab.SetCurtab(RT_REPORTTAB_QUOTE);
	}
	else
	{
		for(int i = 0; i < RTV_MAX_MULTICOUNT; i++)
			m_realtime[i].SetDrawMode(CRealTime::modePriceLine, CRealTime::techNone, CRealTime::reportNone);
	}

	OnUpdate();
}

void CRealTimeView::ChangeDrawMode(int nDrawMode)
{
	int nTechLine	=	m_realtime[0].GetDrawTechLine();
	int	nReportWhat	=	m_realtime[0].GetReportWhat();
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].SetDrawMode(nDrawMode, nTechLine, nReportWhat);

	//if(::IsWindow(m_wndReportTab.GetSafeHwnd()))
	//{
	//	if(1 == m_nMultiStockCount && CRealTime::modePriceLine == nDrawMode)
	//		m_wndReportTab.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	//	else
	//		m_wndReportTab.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
	//}

	Invalidate();
}

void CRealTimeView::ToggleDrawTechLine(int nTechLine)
{
	int	nOld	=	m_realtime[0].GetDrawTechLine();
	int nNew	=	(nTechLine == nOld ? CRealTime::techNone : nTechLine);
	int	nDrawMode	=	m_realtime[0].GetDrawMode();
	int	nReportWhat	=	m_realtime[0].GetReportWhat();
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].SetDrawMode(nDrawMode, nNew, /*nReportWhat*/nNew);

	Invalidate();
}

void CRealTimeView::SendRequestQuote(BOOL bForced)
{
	BOOL bInTrade = CSPTime::InTradeTime(CSPTime::GetCurrentTime().GetTime(),900);
	if(bForced || bInTrade)
	{
		for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		{
			CStock & stock = m_realtime[i].GetCurStock();
			CStockInfo & info = stock.GetStockInfo();
			if(info.IsValidStock())
				AfxGetStkReceiver().RequestStockData(CStock::dataReport, &info, 1, 0, 0);
		}
	}
}

BOOL CRealTimeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CRealTimeView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	OnUpdate();
}

void CRealTimeView::OnUpdate() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!IsWindowVisible())
		return;
	SetFocus();

	ASSERT(RTV_MAX_MULTICOUNT > 0);

	CStockInfo	info;
	if(AfxGetProfile().GetCurrentStock(&info))
	{
		m_realtime[0].SetCurStock(info);
		m_realtime[0].PrepareStockData();
	}
	else
	{
		m_realtime[0].SetCurStock(STKLIB_CODE_MAIN);
		m_realtime[0].PrepareStockData();
		AfxGetProfile().SetCurrentStock(STKLIB_CODE_MAIN);
	}
	if(CRealTime::modeReportDetail == m_realtime[0].GetDrawMode())
		AfxGetStkReceiver().RequestStockData(CStock::dataDetail, &info, 1, 0, 0);
	else
		AfxGetStkReceiver().RequestStockData(CStock::dataMinute, &info, 1, 0, 0);

	int i=0;
	// 多股同列
	for( i=1; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
	{
		if(AfxGetProfile().GetNextStock(&info))
		{
			m_realtime[i].SetCurStock(info);
			m_realtime[i].PrepareStockData();
			if(CRealTime::modeReportDetail == m_realtime[0].GetDrawMode())
				AfxGetStkReceiver().RequestStockData(CStock::dataDetail, &info, 1, 0, 0);
			else
				AfxGetStkReceiver().RequestStockData(CStock::dataMinute, &info, 1, 0, 0);
		}
	}
	for( i=1; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		AfxGetProfile().GetPrevStock(&info);

	Invalidate();

	SendRequestQuote(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView drawing

void CRealTimeView::OnDraw(CDC* pDC)
{
	CView::OnDraw(pDC);
}

void CRealTimeView::RedrawAll(CDC * pDC, CRect rectClient)
{		
	int	i	=	0;
	CRect	rect[RTV_MAX_MULTICOUNT];
	for(i = 0; i < RTV_MAX_MULTICOUNT; i++)
		rect[i]	=	rectClient;
	switch(m_nMultiStockCount)
	{
	case 2:
		rect[0].bottom	=	rect[1].top	=	rectClient.top + rectClient.Height()/2;
		for(i = 0; i < m_nMultiStockCount; i++)
			m_realtime[i].Redraw(pDC, rect[i]);
		break;
	case 4:
		rect[0].bottom	=	rect[2].bottom	=	rect[1].top		=	rect[3].top		=	rectClient.top + rectClient.Height()/2;
		rect[0].right	=	rect[1].right	=	rect[2].left	=	rect[3].left	=	rectClient.left + rectClient.Width()/2;
		for(i = 0; i < m_nMultiStockCount; i++)
			m_realtime[i].Redraw(pDC, rect[i]);
		break;
	case 6:
		rect[0].bottom	=	rect[3].bottom	=	rect[1].top		=	rect[4].top		=	rectClient.top + rectClient.Height()/3;
		rect[1].bottom	=	rect[4].bottom	=	rect[2].top		=	rect[5].top		=	rectClient.top + 2*rectClient.Height()/3;
		rect[0].right	=	rect[1].right	=	rect[2].right	=	rect[3].left	=	rect[4].left	=	rect[5].left	=	rectClient.left + rectClient.Width()/2;
		for(i = 0; i < m_nMultiStockCount; i++)
			m_realtime[i].Redraw(pDC, rect[i]);
		break;
	case 9:
		rect[0].bottom	=	rect[3].bottom	=	rect[6].bottom	=	rect[1].top		=	rect[4].top		=	rect[7].top		=	rectClient.top + rectClient.Height()/3;
		rect[1].bottom	=	rect[4].bottom	=	rect[7].bottom	=	rect[2].top		=	rect[5].top		=	rect[8].top		=	rectClient.top + 2*rectClient.Height()/3;
		rect[0].right	=	rect[1].right	=	rect[2].right	=	rect[3].left	=	rect[4].left	=	rect[5].left	=	rectClient.left + rectClient.Width()/3;
		rect[3].right	=	rect[4].right	=	rect[5].right	=	rect[6].left	=	rect[7].left	=	rect[8].left	=	rectClient.left + 2*rectClient.Width()/3;
		for(i = 0; i < m_nMultiStockCount; i++)
			m_realtime[i].Redraw(pDC, rect[i]);
		break;
	default:
		m_realtime[0].Redraw(pDC, rectClient);	// 单股
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView printing

BOOL CRealTimeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRealTimeView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CView::OnBeginPrinting(pDC, pInfo);
}

void CRealTimeView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CView::OnEndPrinting(pDC, pInfo);
}

void CRealTimeView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add customized printing code here
	pDC->SetMapMode(MM_ISOTROPIC);
	CRect	rectClient(0, 0, 1020, 586);
	// GetClientRect(&rectClient);
	pDC->SetWindowOrg(rectClient.left, rectClient.top);
	pDC->SetWindowExt(rectClient.Width(), rectClient.Height());
	pDC->SetViewportOrg(pInfo->m_rectDraw.left, pInfo->m_rectDraw.top);
	pDC->SetViewportExt(pInfo->m_rectDraw.Width(), pInfo->m_rectDraw.Height());
	
	RedrawAll(pDC, rectClient);
	// CView::OnPrint(pDC,pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView message handlers

int CRealTimeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// m_realtime
	for(int i = 0; i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].SetParent(this);

	// 实时行情刷新
	AfxGetStkReceiver().AddRcvDataWnd(GetSafeHwnd());

	// 大单成交刷新
	SetTimer(RTV_TIMER_REFRESHBIGTRADE, 30000, NULL);
	SetTimer(RTV_TIMER_REFRESH, 30000, NULL);

	return 0;
}

void CRealTimeView::OnDestroy() 
{
	// 停止行情刷新通知消息
	AfxGetStkReceiver().RemoveRcvDataWnd(GetSafeHwnd());
	KillTimer(RTV_TIMER_REFRESHBIGTRADE);
	KillTimer(RTV_TIMER_REFRESH);

	CView::OnDestroy();
}

void CRealTimeView::OnTimer(UINT nIDEvent) 
{
	if(RTV_TIMER_REFRESHBIGTRADE == nIDEvent)
	{
		int	nDrawMode	=	m_realtime[0].GetDrawMode();
		int	nReportWhat	=	m_realtime[0].GetReportWhat();

		if(CRealTime::modeBigTradeDetail == nDrawMode)
			Invalidate();
		else if(CRealTime::modePriceLine == nDrawMode
			&& CRealTime::reportBigTrade == nReportWhat)
			m_realtime[0].DrawReportRegion(NULL);
	}
	if(RTV_TIMER_REFRESH == nIDEvent)
	{
		SendRequestQuote(FALSE);
	}

	CView::OnTimer(nIDEvent);
}

void CRealTimeView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(bActivate)	OnUpdate();

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CRealTimeView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

LRESULT CRealTimeView::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	Invalidate();

	return 0L;
}

LRESULT CRealTimeView::OnStkReceiverData(WPARAM wParam, LPARAM lParam)
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].OnStkReceiverData(wParam,lParam);
	return 0;
}

void CRealTimeView::OnRealtimePrev() 
{
	if(CRealTime::modePriceLine == m_realtime[0].GetDrawMode())
	{
		CStockInfo	info;
		for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
			AfxGetProfile().GetPrevStock(&info);
		OnUpdate();
	}
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].PageUp();
}

void CRealTimeView::OnRealtimeNext() 
{
	if(CRealTime::modePriceLine == m_realtime[0].GetDrawMode())
	{
		CStockInfo	info;
		for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
			AfxGetProfile().GetNextStock(&info);
		OnUpdate();
	}
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].PageDown();
}

void CRealTimeView::OnRealtimeLeft() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveLeft();
}

void CRealTimeView::OnRealtimeRight() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveRight();
}

void CRealTimeView::OnRealtimeUp() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveUp();
}

void CRealTimeView::OnRealtimeDown() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveDown();
}

void CRealTimeView::OnRealtimeHome() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveHome();
}

void CRealTimeView::OnRealtimeEnd() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveEnd();
}

