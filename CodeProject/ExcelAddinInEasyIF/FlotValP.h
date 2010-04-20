#if !defined(AFX_FLOTVALP_H__2557CA5F_0278_41FB_8B4D_B65AE84CA016__INCLUDED_)
#define AFX_FLOTVALP_H__2557CA5F_0278_41FB_8B4D_B65AE84CA016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlotValP.h : header file
//
#include "mtxgrid.h"

/////////////////////////////////////////////////////////////////////////////
// FloatingValuesPage frame

class FloatingValuesPage : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(FloatingValuesPage)
protected:
  MatrixManager *m_pMatrixMgr;
  Matrix *m_pMatrix;

  BOOL m_bRecalcExcelOnChanges;

	CButton	m_oDeleteRowBtn;
	CButton	m_oInsertRowBtn;
	CButton	m_oWildCardBtn;

  CButton m_oRecalcOnChanges;
  CButton m_oRecalcNowBtn;
  CButton m_oSaveBtn;
  CButton m_oCloseBtn;

  HBITMAP m_hWildCardBmp;
  HBITMAP m_hInsertRowBmp;
  HBITMAP m_hDeleteRowBmp;
  HBITMAP m_hRecalcNowBmp;

  CFont m_oBtnFont;

  MatrixGrid m_oGrid;

	FloatingValuesPage();           // protected constructor used by dynamic creation

  void BuildGridStructure();

// Attributes
public:

  FloatingValuesPage(MatrixManager *pMatrixMgr, Matrix *pMatrix);

// Operations
public:

  void DeleteSelectedRows()     { OnDatavaluesDeleterow(); }
  void WildcardSelectedCells()  { OnDatavaluesWildcard(); }

  void EnableSaveBtn(BOOL bEnable = TRUE) { m_oSaveBtn.EnableWindow(bEnable); }
  void GridChanged();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FloatingValuesPage)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~FloatingValuesPage();

	// Generated message map functions
	//{{AFX_MSG(FloatingValuesPage)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnSaveClicked();
  afx_msg void OnCloseClicked();
  afx_msg void OnAutoRecalcClicked();
  afx_msg void OnRecalcNowClicked();
	afx_msg void OnDatavaluesDeleterow();
	afx_msg void OnDatavaluesInsertrow();
	afx_msg void OnDatavaluesWildcard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOTVALP_H__2557CA5F_0278_41FB_8B4D_B65AE84CA016__INCLUDED_)
