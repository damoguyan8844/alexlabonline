#if !defined(AFX_FLOTGRID_H__37B8EF96_05B7_44D1_9292_E1C86FAF1E35__INCLUDED_)
#define AFX_FLOTGRID_H__37B8EF96_05B7_44D1_9292_E1C86FAF1E35__INCLUDED_

#include "matrix.h"
#include "gridlist.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlotGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FloatingGridWnd frame

class FloatingGridWnd : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(FloatingGridWnd)
protected:
  MatrixManager *m_pMatrixMgr;

  HBITMAP m_hNewBmp;
  HBITMAP m_hRefreshBmp;
  HBITMAP m_hHideBmp;

  BOOL m_bIsWindowVisible;

	FloatingGridWnd();           // protected constructor used by dynamic creation

  BOOL CheckModifyRights(Matrix *pMatrix);
  BOOL CheckAccessRights(Matrix *pMatrix);

  void DeleteGrid(LPCSTR cpGridName);
  void CopyGrid(LPCSTR cpGridName);

  void SaveSizeAndVisibility();

// Attributes
public:

  CGridList m_oListBox;

  CFont     m_oLBFont;

  CButton   m_oCreateNewBtn;
  CButton   m_oRefreshBtn;
  CButton   m_oHideBtn;

// Operations
public:
  FloatingGridWnd(MatrixManager *pMatrixMgr);           // protected constructor used by dynamic creation

  void LoadMatrices();
  void ToggleDisplay();

  void EditGrid(LPCSTR cpGridName);
  void DoContextMenu(LPCSTR cpGridName, CPoint *opOrigPt);

  void ShowValuesPage(LPCSTR cpGridName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FloatingGridWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~FloatingGridWnd();

	// Generated message map functions
	//{{AFX_MSG(FloatingGridWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnListRefresh();
  afx_msg void OnCreateNew();
  afx_msg void OnHideGridPad();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOTGRID_H__37B8EF96_05B7_44D1_9292_E1C86FAF1E35__INCLUDED_)
