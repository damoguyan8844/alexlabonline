#if !defined(AFX_FILESENDINGDLG_H__D7449FAB_D4C1_4C2E_9D08_5E6E6554DB80__INCLUDED_)
#define AFX_FILESENDINGDLG_H__D7449FAB_D4C1_4C2E_9D08_5E6E6554DB80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileSendingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FileSendingDlg dialog

class EM_FileSendingDlg : public CDialog
{
// Construction
public:
	enum{FILE_MAXFILECOUNT=6,FILE_BUFFER_LENGTH=FILE_SENDING_BUFFER_SIZE};
	void BeginToReceive(SOCKET &sock);
	EM_FileSendingDlg(CWnd* pParent = NULL);   // standard constructor

	static DWORD WINAPI FileSendingProc(LPVOID lParam);
	static DWORD WINAPI FileReceivingProc(LPVOID lParam);

	void BeginToSend(LPEM_FILEINFO fInfo);
	void AdjustDlgRect();
	void DoInitialize(DWORD dwIndex, CRect rt, DWORD dwY);

	DWORD m_dwFileLength;
	DWORD m_dwSendLength;
	BOOL m_bSendingDlg;
	DWORD m_dwCurrent;
	DWORD m_dwX, m_dwY;
	CProgressCtrl	m_percent;
	CBrush m_Brush;
	COLORREF m_crStatic;
	char m_szFileName[512];
	char m_szFileTitle[512];
	char m_szFileExt[512];
	char m_szUserIP[128];
	SOCKET Sock;
	BOOL m_bRunning;
	DOUBLE m_fDuration;
	EM_FILEINFO m_fileInfo;
// Dialog Data
	//{{AFX_DATA(FileSendingDlg)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FileSendingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FileSendingDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	afx_msg void FileReceivingComplete(WPARAM wParam, LPARAM lParam);
	afx_msg void FileSendingComplete(WPARAM wParam, LPARAM lParam);
//	afx_msg void FileSendingFailed(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESENDINGDLG_H__D7449FAB_D4C1_4C2E_9D08_5E6E6554DB80__INCLUDED_)
