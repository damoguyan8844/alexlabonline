
#include "stdafx.h"
#include "GraphView.h"

#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphView

#define	IDC_GRAPHVIEW_REPORTTAB	200

#define	GV_REPORTTAB_PARAM			0	// 参数
#define	GV_REPORTTAB_COST			1	// 成本
#define	GV_REPORTTAB_FLAME			2	// 火焰山
#define	GV_REPORTTAB_ACTIVITY		3	// 活跃度

IMPLEMENT_DYNCREATE(CGraphView, CView)

BEGIN_MESSAGE_MAP(CGraphView, CView)
	//{{AFX_MSG_MAP(CGraphView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
// 	ON_COMMAND(ID_VIEW_PDAY, OnViewPday)
// 	ON_COMMAND(ID_VIEW_PWEEK, OnViewPweek)
// 	ON_COMMAND(ID_VIEW_PMONTH, OnViewPmonth)
// 	ON_COMMAND(ID_VIEW_PMIN5, OnViewPmin5)
// 	ON_COMMAND(ID_VIEW_PMIN15, OnViewPmin15)
// 	ON_COMMAND(ID_VIEW_PMIN30, OnViewPmin30)
// 	ON_COMMAND(ID_VIEW_PMIN60, OnViewPmin60)
// 	ON_COMMAND(ID_VIEW_LEFT, OnViewLeft)
// 	ON_COMMAND(ID_VIEW_RIGHT, OnViewRight)
// 	ON_COMMAND(ID_VIEW_PAGELEFT, OnViewPageleft)
// 	ON_COMMAND(ID_VIEW_PAGERIGHT, OnViewPageright)
// 	ON_COMMAND(ID_VIEW_HOME, OnViewHome)
// 	ON_COMMAND(ID_VIEW_END, OnViewEnd)
// 	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
// 	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
// 	ON_COMMAND(ID_VIEW_PREV, OnViewPrev)
// 	ON_COMMAND(ID_VIEW_NEXT, OnViewNext)
// 	ON_COMMAND(ID_VIEW_INSERT, OnViewInsert)
// 	ON_COMMAND(ID_VIEW_NORESUMEDR, OnViewNoresumedr)
// 	ON_COMMAND(ID_VIEW_RESUMEDRUP, OnViewResumedrup)
// 	ON_COMMAND(ID_VIEW_RESUMEDRDOWN, OnViewResumedrdown)
// 	ON_COMMAND(ID_VIEW_MAINDATACLOSE, OnViewMaindataclose)
// 	ON_COMMAND(ID_VIEW_MAINDATAOPEN, OnViewMaindataopen)
// 	ON_COMMAND(ID_VIEW_MAINDATAAVERAGE, OnViewMaindataaverage)
// 	ON_COMMAND(ID_VIEW_KLINECANDLE, OnViewKlinecandle)
// 	ON_COMMAND(ID_VIEW_KLINEAMERICA, OnViewKlineamerica)
// 	ON_COMMAND(ID_VIEW_KLINETOWER, OnViewKlinetower)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_PDAY, OnUpdateViewPday)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_PWEEK, OnUpdateViewPweek)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_PMONTH, OnUpdateViewPmonth)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN5, OnUpdateViewPmin5)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN15, OnUpdateViewPmin15)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN30, OnUpdateViewPmin30)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN60, OnUpdateViewPmin60)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_NORESUMEDR, OnUpdateViewNoresumedr)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_RESUMEDRUP, OnUpdateViewResumedrup)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_RESUMEDRDOWN, OnUpdateViewResumedrdown)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINDATACLOSE, OnUpdateViewMaindataclose)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINDATAOPEN, OnUpdateViewMaindataopen)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINDATAAVERAGE, OnUpdateViewMaindataaverage)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_KLINECANDLE, OnUpdateViewKlinecandle)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_KLINEAMERICA, OnUpdateViewKlineamerica)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_KLINETOWER, OnUpdateViewKlinetower)
// 	ON_COMMAND(ID_VIEW_HIDETHISTECH, OnViewHidethistech)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_HIDETHISTECH, OnUpdateViewHidethistech)
// 	ON_COMMAND(ID_VIEW_THISTECHPARAM, OnViewThistechparam)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_THISTECHPARAM, OnUpdateViewThistechparam)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
// 	ON_COMMAND_RANGE(ID_VIEW_TECH_START, ID_VIEW_TECH_END, OnViewTechRange)
// 	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_TECH_START, ID_VIEW_TECH_END, OnUpdateViewTechRange)
// 	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
// 	ON_MESSAGE(WM_USER_GETVIEWCMDID, OnGetViewCmdid)
// 	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
// 	ON_MESSAGE(WM_USER_MYRBUTTONDOWN, OnMyRButtonDown)
 	ON_MESSAGE(WM_APP_STKRECEIVER_DATA, OnStkReceiverData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphView construction/destruction

CGraphView::CGraphView()
{
}

CGraphView::~CGraphView()
{
}

void CGraphView::ResetClientRect()
{
	CRect rectClient;
	GetClientRect(&rectClient);
	m_graph.ResetClient(rectClient);
}

void CGraphView::ChangeKType(int ktype)
{
	m_graph.SetKType(ktype);
	m_graph.ResetIndex();
	Invalidate();
}

void CGraphView::ChangeKFormat(int kformat)
{
	m_graph.SetKFormat(kformat);
	m_graph.ResetIndex();
	Invalidate();
}

void CGraphView::OnToggleKType()
{
	int ktype = m_graph.GetKType();
	ktype	++;
	if (ktype > CKData::ktypeMax)
		ktype	=	CKData::ktypeMin;
	ChangeKType(ktype);
}

void CGraphView::OnToggleKFormat(int kformat)
{
	int nOld = m_graph.GetKFormat();
	if (nOld == kformat)
		ChangeKFormat(CKData::formatOriginal);
	else
		ChangeKFormat(kformat);
}

void CGraphView::ChangeMaindataType(int maindatatype)
{
	m_graph.SetMaindataType(maindatatype);
	m_graph.ResetIndex();
	Invalidate();
}

void CGraphView::ChangeKLineMode(int klineMode)
{
	m_graph.SetKLineMode(klineMode);
	m_graph.ResetIndex();
	Invalidate();
}

void CGraphView::OnTechParametersChange()
{
	m_graph.ClearTechniqueCache();
}

BOOL CGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CGraphView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	OnUpdate();
}

void CGraphView::OnUpdate() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!IsWindowVisible())
		return;
	SetFocus();

	CStockInfo	info;
	if (AfxGetProfile().GetCurrentStock(&info))
	{
		m_graph.SetCurStock(info);
		m_graph.PrepareStockData();
	}
	else
	{
		m_graph.SetCurStock(STKLIB_CODE_MAIN);
		m_graph.PrepareStockData();
		AfxGetProfile().SetCurrentStock(STKLIB_CODE_MAIN);
	}

	Invalidate();
}

BOOL CGraphView::PreTranslateMessage(MSG* pMsg) 
{
// 	if (WM_KEYDOWN == pMsg->message)
// 	{
// 		switch(pMsg->wParam)
// 		{
// 		case VK_F5:
// 			{
// 				AfxSwitchToStaticView(RUNTIME_CLASS(CRealTimeView));
// 				CRealTimeView * pView = AfxGetRealTimeView();
// 				if (pView)
// 					pView->ChangeDrawMode(CRealTime::modePriceLine);
// 			}
// 			return TRUE;
// 		}
// 	}
	
	return CView::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphView message handlers
/////////////////////////////////////////////////////////////////////////////
// CGraphView drawing

void CGraphView::OnDraw(CDC* pDC)
{
	CView::OnDraw(pDC);
}

int CGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 实时行情刷新
	AfxGetStkReceiver().AddRcvDataWnd(GetSafeHwnd());

	return 0;
}


void CGraphView::OnDestroy() 
{
	AfxGetStkReceiver().RemoveRcvDataWnd(GetSafeHwnd());

	CView::OnDestroy();
}

void CGraphView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	TRACE(_T("CGraphView::OnActivateView %d %d %d\n"), bActivate, pActivateView, pDeactiveView);

	if (bActivate)	OnUpdate();

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CGraphView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	if (pDC && pDC->IsKindOf(RUNTIME_CLASS(CDC)))
	{
		CRect rect;
		GetClientRect(&rect);
		CBrush brush;
		brush.CreateSolidBrush(AfxGetProfile().GetColor(CColorClass::clrGraphBK));
		pDC->FillRect(&rect, &brush);

		return TRUE;
	}

	return CView::OnEraseBkgnd(pDC);
}

LRESULT CGraphView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle = "技术图表";

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();

	return wParam;
}

LRESULT CGraphView::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	//m_wndReportTab.SetColorTabs(AfxGetProfile().GetColor(CColorClass::clrGraphBK));
	//m_wndReportTab.SetSelectTabColor(AfxGetProfile().GetColor(CColorClass::clrGraphBK),
	//								AfxGetProfile().GetColor(CColorClass::clrTitle));

	Invalidate();
	return 0L;
}

LRESULT CGraphView::OnStkReceiverData(WPARAM wParam, LPARAM lParam)
{
	return m_graph.OnStkReceiverData(wParam,lParam);
}

void CGraphView::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_graph.OnMouseMove(nFlags, point);
	CView::OnMouseMove(nFlags, point);
}

void CGraphView::OnViewPday()	{	ChangeKType(CKData::ktypeDay);	}
void CGraphView::OnViewPweek()	{	ChangeKType(CKData::ktypeWeek);	}
void CGraphView::OnViewPmonth()	{	ChangeKType(CKData::ktypeMonth);	}
void CGraphView::OnViewPmin5()	{	ChangeKType(CKData::ktypeMin5);	}
void CGraphView::OnViewPmin15()	{	ChangeKType(CKData::ktypeMin15);	}
void CGraphView::OnViewPmin30()	{	ChangeKType(CKData::ktypeMin30);	}
void CGraphView::OnViewPmin60()	{	ChangeKType(CKData::ktypeMin60);	}

void CGraphView::OnUpdateViewPday(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeDay == m_graph.GetKType());		}
void CGraphView::OnUpdateViewPweek(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeWeek == m_graph.GetKType());	}
void CGraphView::OnUpdateViewPmonth(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeMonth == m_graph.GetKType());	}
void CGraphView::OnUpdateViewPmin5(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeMin5 == m_graph.GetKType());	}
void CGraphView::OnUpdateViewPmin15(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeMin15 == m_graph.GetKType());	}
void CGraphView::OnUpdateViewPmin30(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeMin30 == m_graph.GetKType());	}
void CGraphView::OnUpdateViewPmin60(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeMin60 == m_graph.GetKType());	}

void CGraphView::OnViewLeft()		{	m_graph.MoveLeft();	}
void CGraphView::OnViewRight()		{	m_graph.MoveRight();	}
void CGraphView::OnViewPageleft()	{	m_graph.PageLeft();	}
void CGraphView::OnViewPageright()	{	m_graph.PageRight();	}
void CGraphView::OnViewHome()		{	m_graph.MoveHome();	}
void CGraphView::OnViewEnd()		{	m_graph.MoveEnd();		}

void CGraphView::OnViewZoomin()		{	m_graph.ExpandUp();	}
void CGraphView::OnViewZoomout()	{	m_graph.ExpandDown();	}

void CGraphView::OnViewPrev() 
{
	CStockInfo	info;
	if (AfxGetProfile().GetPrevStock(&info))
		OnUpdate();
}
void CGraphView::OnViewNext() 
{
	CStockInfo	info;
	if (AfxGetProfile().GetNextStock(&info))
		OnUpdate();
}
void CGraphView::OnViewInsert()
{
	m_graph.OnInsert();
}

// K Format
void CGraphView::OnViewNoresumedr()	{	OnToggleKFormat(CKData::formatOriginal);	}
void CGraphView::OnViewResumedrup()	{	OnToggleKFormat(CKData::formatXDRup);		}
void CGraphView::OnViewResumedrdown(){	OnToggleKFormat(CKData::formatXDRdown);		}

void CGraphView::OnUpdateViewNoresumedr(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck(CKData::formatOriginal == m_graph.GetKFormat());	}
void CGraphView::OnUpdateViewResumedrup(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck(CKData::formatXDRup == m_graph.GetKFormat());	}
void CGraphView::OnUpdateViewResumedrdown(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::formatXDRdown == m_graph.GetKFormat());	}

// Main data Type
void CGraphView::OnViewMaindataclose(){	ChangeMaindataType(CKData::mdtypeClose);	}
void CGraphView::OnViewMaindataopen(){	ChangeMaindataType(CKData::mdtypeOpen);		}
void CGraphView::OnViewMaindataaverage(){ChangeMaindataType(CKData::mdtypeAverage);	}

void CGraphView::OnUpdateViewMaindataclose(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::mdtypeClose == m_graph.GetMaindataType());	}
void CGraphView::OnUpdateViewMaindataopen(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::mdtypeOpen == m_graph.GetMaindataType());	}
void CGraphView::OnUpdateViewMaindataaverage(CCmdUI* pCmdUI){	pCmdUI->SetCheck(CKData::mdtypeAverage == m_graph.GetMaindataType());	}

// kline Mode
void CGraphView::OnViewKlinecandle()	{	ChangeKLineMode(CStockGraph::klineCandle);	}
void CGraphView::OnViewKlineamerica()	{	ChangeKLineMode(CStockGraph::klineAmerica);	}
void CGraphView::OnViewKlinetower()		{	ChangeKLineMode(CStockGraph::klineTower);	}

void CGraphView::OnUpdateViewKlinecandle(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CStockGraph::klineCandle == m_graph.GetKLineMode());		}
void CGraphView::OnUpdateViewKlineamerica(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CStockGraph::klineAmerica == m_graph.GetKLineMode());		}
void CGraphView::OnUpdateViewKlinetower(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck(CStockGraph::klineTower == m_graph.GetKLineMode());		}


void CGraphView::OnViewHidethistech() 
{
	m_graph.HideSelectedTech();
}

void CGraphView::OnUpdateViewHidethistech(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(STT_INVALID != m_graph.GetSelectedTech());
}

