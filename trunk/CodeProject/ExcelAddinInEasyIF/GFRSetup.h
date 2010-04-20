#if !defined(AFX_GFRSETUP_H__8AD17763_2711_4F35_99BD_346AA0714504__INCLUDED_)
#define AFX_GFRSETUP_H__8AD17763_2711_4F35_99BD_346AA0714504__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GFRSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GFRSetup dialog

class GFRSetup : public CDialog
{
  CRect m_oSelRange;

  void RowColToA1(int iRow, int iCol, char *cpBuffer);
// Construction
public:
  GFRSetup(CRect *pSelRange, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GFRSetup)
	enum { IDD = IDD_GRWZ_GRIDFROMRANGE };
	BOOL	m_bFirstRowColHeadings;
	CString	m_cMatrixName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GFRSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GFRSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeGwfrMatrixname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GFRSETUP_H__8AD17763_2711_4F35_99BD_346AA0714504__INCLUDED_)
