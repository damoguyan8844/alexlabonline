// EDStockView.h : interface of the CEDStockView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDSTOCKVIEW_H__36162BD1_1031_4790_BD0B_BB9A9AE4894F__INCLUDED_)
#define AFX_EDSTOCKVIEW_H__36162BD1_1031_4790_BD0B_BB9A9AE4894F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCtrl.h"

class CEDStockView : public CView
{
protected: // create from serialization only
	CEDStockView();
	DECLARE_DYNCREATE(CEDStockView)

// Attributes
public:
	CEDStockDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEDStockView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEDStockView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CGridCtrl	m_Grid;

// Generated message map functions
protected:
	//{{AFX_MSG(CEDStockView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EDStockView.cpp
inline CEDStockDoc* CEDStockView::GetDocument()
   { return (CEDStockDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDSTOCKVIEW_H__36162BD1_1031_4790_BD0B_BB9A9AE4894F__INCLUDED_)
