// EDStockView.cpp : implementation of the CEDStockView class
//

#include "stdafx.h"
#include "EDStock.h"

#include "EDStockDoc.h"
#include "EDStockView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IDC_SLISTVIEW_GRID		100

/////////////////////////////////////////////////////////////////////////////
// CEDStockView

IMPLEMENT_DYNCREATE(CEDStockView, CView)

BEGIN_MESSAGE_MAP(CEDStockView, CView)
	//{{AFX_MSG_MAP(CEDStockView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEDStockView construction/destruction

CEDStockView::CEDStockView()
{
	// TODO: add construction code here

}

CEDStockView::~CEDStockView()
{
}

BOOL CEDStockView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEDStockView drawing

void CEDStockView::OnDraw(CDC* pDC)
{
	CEDStockDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CEDStockView printing

BOOL CEDStockView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEDStockView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEDStockView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CEDStockView diagnostics

#ifdef _DEBUG
void CEDStockView::AssertValid() const
{
	CView::AssertValid();
}

void CEDStockView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEDStockDoc* CEDStockView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEDStockDoc)));
	return (CEDStockDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEDStockView message handlers

int CEDStockView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void CEDStockView::OnDestroy() 
{

	CView::OnDestroy();
}

