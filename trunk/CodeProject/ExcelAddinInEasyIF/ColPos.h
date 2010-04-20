#if !defined(AFX_COLPOS_H__86E8F2C8_AC02_4055_BCBC_157F1AAE3297__INCLUDED_)
#define AFX_COLPOS_H__86E8F2C8_AC02_4055_BCBC_157F1AAE3297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColPos.h : header file
//
#include "matrix.h"

/////////////////////////////////////////////////////////////////////////////
// ColPosition dialog

class ColPosition : public CPropertyPage
{
	DECLARE_DYNCREATE(ColPosition)

  Matrix *m_pMatrix;

// Construction
public:
  ColPosition();
  ColPosition(Matrix *pMatrix);
  ~ColPosition();

// Dialog Data
	//{{AFX_DATA(ColPosition)
	enum { IDD = IDD_COLEDIT_POSITION };
	int		m_iInsertBefore;
	//}}AFX_DATA
    BOOL m_bAppend;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(ColPosition)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(ColPosition)
	virtual BOOL OnInitDialog();
	afx_msg void OnColeditAppend();
	afx_msg void OnColeditInsert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLPOS_H__86E8F2C8_AC02_4055_BCBC_157F1AAE3297__INCLUDED_)
