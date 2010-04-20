#if !defined(AFX_GFCMATRIXDETAILS_H__FA6A69D0_90B7_4C7B_B18C_5FE0EE3C53A9__INCLUDED_)
#define AFX_GFCMATRIXDETAILS_H__FA6A69D0_90B7_4C7B_B18C_5FE0EE3C53A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GFCMatrixDetails.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GFCMatrixDetails dialog

class GFCMatrixDetails : public CPropertyPage
{
	DECLARE_DYNCREATE(GFCMatrixDetails)

// Construction
public:
	GFCMatrixDetails();
	~GFCMatrixDetails();

// Dialog Data
	//{{AFX_DATA(GFCMatrixDetails)
	enum { IDD = IDD_WIZ_MATRIXINFO };
	CString	m_cDefValue;
	CString	m_cMatrixName;
	int		m_iReturnType;
	BOOL	m_bLaunchEditor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(GFCMatrixDetails)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(GFCMatrixDetails)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeWizgridinfoGridname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GFCMATRIXDETAILS_H__FA6A69D0_90B7_4C7B_B18C_5FE0EE3C53A9__INCLUDED_)
