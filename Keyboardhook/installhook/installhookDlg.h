// installhookDlg.h : header file
//

#if !defined(AFX_INSTALLHOOKDLG_H__2FB825E9_C1CA_47FC_91F0_06FBB635D15D__INCLUDED_)
#define AFX_INSTALLHOOKDLG_H__2FB825E9_C1CA_47FC_91F0_06FBB635D15D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInstallhookDlg dialog

class CInstallhookDlg : public CDialog
{
// Construction
public:
	CInstallhookDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInstallhookDlg)
	enum { IDD = IDD_INSTALLHOOK_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstallhookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CInstallhookDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOk();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTALLHOOKDLG_H__2FB825E9_C1CA_47FC_91F0_06FBB635D15D__INCLUDED_)
