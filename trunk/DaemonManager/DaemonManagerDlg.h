// DaemonManagerDlg.h : header file
//

#if !defined(AFX_DAEMONMANAGERDLG_H__2BB9D3D0_DFDA_4F5F_A3CD_7B07148EBAE5__INCLUDED_)
#define AFX_DAEMONMANAGERDLG_H__2BB9D3D0_DFDA_4F5F_A3CD_7B07148EBAE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDaemonManagerDlg dialog

class CDaemonManagerDlg : public CDialog
{
// Construction
public:
	CDaemonManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDaemonManagerDlg)
	enum { IDD = IDD_DAEMONMANAGER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaemonManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDaemonManagerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAEMONMANAGERDLG_H__2BB9D3D0_DFDA_4F5F_A3CD_7B07148EBAE5__INCLUDED_)
