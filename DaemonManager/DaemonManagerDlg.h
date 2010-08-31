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

	//修正移动时窗口的大小
	void FixMoving(UINT fwSide, LPRECT pRect);
	//修正改改变窗口大小时窗口的大小
	void FixSizing(UINT fwSide, LPRECT pRect);
	//从收缩状态显示窗口
	void DoShow();
	//从显示状态收缩窗口
	void DoHide();
	//重载函数,只是为了方便调用
	BOOL SetWindowPos(const CWnd* pWndInsertAfter,
		LPCRECT pCRect, UINT nFlags = SWP_SHOWWINDOW);

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDaemonManagerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAEMONMANAGERDLG_H__2BB9D3D0_DFDA_4F5F_A3CD_7B07148EBAE5__INCLUDED_)
