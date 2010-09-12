// ftpdDlg.h : header file
//

#if !defined(AFX_FTPDDLG_H__B4EE760A_3B9B_454A_A1B2_7046B57C2CC3__INCLUDED_)
#define AFX_FTPDDLG_H__B4EE760A_3B9B_454A_A1B2_7046B57C2CC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFtpdDlg dialog

class CFtpdDlg : public CDialog
{
// Construction
public:
	char userfile[256];
	char ipfile[256];
	char AppDir[256];
	CFtpdDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL	m_b_autostart;
// Dialog Data
	//{{AFX_DATA(CFtpdDlg)
	enum { IDD = IDD_FTPD_DIALOG };
	CTreeCtrl	m_ftpdtree;
	CComboBox	m_ip_combo_ctrl;
	CString	m_ftpserver_name;
	UINT	m_ftpd_port;
	CString	m_rootdir;
	BOOL	m_b_ipchk;
	BOOL	m_b_ipchk_pasv;
	BOOL	m_b_switch_pasv_ip;
	BOOL	m_specify_data_port;
	UINT	m_port_from;
	UINT	m_port_to;
	BOOL	m_b_auto_choose_ip;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFtpdDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLOAD();
	afx_msg void OnSAVE();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLaunch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPDDLG_H__B4EE760A_3B9B_454A_A1B2_7046B57C2CC3__INCLUDED_)
