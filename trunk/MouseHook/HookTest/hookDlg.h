// hookDlg.h : header file
//

#if !defined(AFX_HOOKDLG_H__346C391E_14BC_11D5_A025_006067718D04__INCLUDED_)
#define AFX_HOOKDLG_H__346C391E_14BC_11D5_A025_006067718D04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHookDlg dialog

class CHookDlg : public CDialog
{
// Construction
public:
	CHookDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHookDlg)
	enum { IDD = IDD_HOOK_DIALOG };
	CStatic	c_Hint;
	CCat	c_Cat;
	CStatic	c_XY;
	CButton	c_Enable;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	afx_msg LRESULT OnMyMouseMove(WPARAM, LPARAM);
	afx_msg LRESULT OnGiveHint(WPARAM, LPARAM);
	// Generated message map functions
	//{{AFX_MSG(CHookDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnEnable();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOOKDLG_H__346C391E_14BC_11D5_A025_006067718D04__INCLUDED_)
