#if !defined(AFX_MTXEDDAT_H__9A497834_758C_4BC1_9557_1583B6E6DC48__INCLUDED_)
#define AFX_MTXEDDAT_H__9A497834_758C_4BC1_9557_1583B6E6DC48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MtxEdDat.h : header file
//

#include "matrix.h"
#include "mtxgrid.h"

/////////////////////////////////////////////////////////////////////////////
// MatrixEditData dialog

class MatrixEditData : public CPropertyPage
{
	DECLARE_DYNCREATE(MatrixEditData)

  HBITMAP m_hWildCardBmp;
  HBITMAP m_hInsertRowBmp;
  HBITMAP m_hDeleteRowBmp;
  HBITMAP m_hFullScreenBmp;

  Matrix *m_pMatrix;

  MatrixGrid m_oGrid;

  void BuildGridStructure();

// Construction
public:
  MatrixEditData();
  MatrixEditData(Matrix *pMatrix);
  ~MatrixEditData();

  void DeleteSelectedRows()     { OnDatavaluesDeleterow(); }
  void WildcardSelectedCells()  { OnDatavaluesWildcard(); }

// Dialog Data
	//{{AFX_DATA(MatrixEditData)
	enum { IDD = IDD_MTXEDIT_DATAVALUES };
	CButton	m_oDeleteRowBtn;
	CButton	m_oInsertRowBtn;
	CButton	m_oWildCardBtn;
	CButton	m_oFullScreenBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(MatrixEditData)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(MatrixEditData)
	virtual BOOL OnInitDialog();
	afx_msg void OnDatavaluesWildcard();
	afx_msg void OnDatavaluesDeleterow();
	afx_msg void OnDatavaluesInsertrow();
    afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
	afx_msg void OnDatavaluesFullscreen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MTXEDDAT_H__9A497834_758C_4BC1_9557_1583B6E6DC48__INCLUDED_)
