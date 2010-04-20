#if !defined(AFX_FSGRID_H__C9C4FBB9_E52C_4079_A29A_05EC5F728BC4__INCLUDED_)
#define AFX_FSGRID_H__C9C4FBB9_E52C_4079_A29A_05EC5F728BC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FSGrid.h : header file
//

#include "matrix.h"
#include "mtxgrid.h"

/////////////////////////////////////////////////////////////////////////////
// FullScreenGrid dialog

class FullScreenGrid : public CDialog
{
  HBITMAP m_hWildCardBmp;
  HBITMAP m_hInsertRowBmp;
  HBITMAP m_hDeleteRowBmp;

  Matrix *m_pMatrix;

  MatrixGrid m_oGrid;

  void BuildGridStructure();

// Construction
public:
  FullScreenGrid(Matrix *pMatrix, CWnd* pParent = NULL);   // standard constructor
  virtual ~FullScreenGrid();

  void DeleteSelectedRows()     { OnDatavaluesDeleterow(); }
  void WildcardSelectedCells()  { OnDatavaluesWildcard(); }

// Dialog Data
	//{{AFX_DATA(FullScreenGrid)
	enum { IDD = IDD_MTXEDIT_DATAVALUESFULL };
	CButton	m_oDeleteRowBtn;
	CButton	m_oInsertRowBtn;
	CButton	m_oWildCardBtn;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FullScreenGrid)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FullScreenGrid)
	virtual BOOL OnInitDialog();
	afx_msg void OnDatavaluesDeleterow();
	afx_msg void OnDatavaluesInsertrow();
	afx_msg void OnDatavaluesWildcard();
    afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FSGRID_H__C9C4FBB9_E52C_4079_A29A_05EC5F728BC4__INCLUDED_)
