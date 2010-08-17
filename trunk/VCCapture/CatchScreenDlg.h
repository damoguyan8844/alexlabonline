// CatchScreenDlg.h : header file
//

#if !defined(AFX_CATCHSCREENDLG_H__13522D32_759B_45B5_9827_596270B7E4DF__INCLUDED_)
#define AFX_CATCHSCREENDLG_H__13522D32_759B_45B5_9827_596270B7E4DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg dialog
#define ICON_NOTIFY WM_USER+120
class CCatchScreenDlg : public CDialog
{
// Construction
public:
	int result;
	void Catch();
	void CaptureRect(HDC hdcScreen,LPRECT prc);
	CCatchScreenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCatchScreenDlg)
	enum { IDD = IDD_CATCHSCREEN_DIALOG };
	int		m_catchNum;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatchScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCatchScreenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnExit();
	afx_msg void OnAbout();
	afx_msg void OnStart();
	afx_msg void OnFull();
	afx_msg void OnAxtive();
	afx_msg void OnSelf();
	afx_msg LRESULT OnNotify(WPARAM uID,LPARAM lEvent);
	afx_msg void OnDestroy();
	afx_msg void OnShow();
	afx_msg void OnHide();
	afx_msg void OnFullScreen();
	afx_msg void OnAxtiveWindow();
	afx_msg void OnSelfWindow();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	NOTIFYICONDATA m_hIconTray;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATCHSCREENDLG_H__13522D32_759B_45B5_9827_596270B7E4DF__INCLUDED_)
