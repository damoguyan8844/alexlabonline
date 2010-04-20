#if !defined(AFX_CFGOPTS_H__38956A23_F5FF_11D4_B28A_50FE6A000000__INCLUDED_)
#define AFX_CFGOPTS_H__38956A23_F5FF_11D4_B28A_50FE6A000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CFGOpts.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ConfigOptions dialog

class ConfigOptions : public CDialog
{
  MatrixManager *m_pMatrixMgr;

// Construction
public:
  ConfigOptions(MatrixManager *pMatrixMgr, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ConfigOptions)
	enum { IDD = IDD_CONFIG_OPTS };
	CString	m_cPathName;
	BOOL	m_bRecalcOnChange;
	int		m_iRefreshIntervalIndex;
	//}}AFX_DATA

  BOOL m_bPathChanged;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ConfigOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ConfigOptions)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnConfigBrowsepath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFGOPTS_H__38956A23_F5FF_11D4_B28A_50FE6A000000__INCLUDED_)
