#if !defined(AFX_EM_RECVCHAT_H__AB48A9A7_0440_4AE6_BE55_94A63A920459__INCLUDED_)
#define AFX_EM_RECVCHAT_H__AB48A9A7_0440_4AE6_BE55_94A63A920459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EM_RecvChat.h : header file
//
#include "EIMChatingWaiter.h"
#include "WaveOut.h"

/////////////////////////////////////////////////////////////////////////////
// EM_RecvChat dialog

class EM_RecvChat : public CDialog
{
// Construction
public:
	EM_RecvChat(CWnd* pParent = NULL);   // standard constructor

	CProgressCtrl	m_process;
	BOOL m_bChatting;
	CString m_strIP;
	CWaveOut *m_pwo;

// Dialog Data
	//{{AFX_DATA(EM_RecvChat)
	enum { IDD = IDD_VOICE2 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EM_RecvChat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EM_RecvChat)
	afx_msg void OnAccept();
	afx_msg void OnDeny();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EM_RECVCHAT_H__AB48A9A7_0440_4AE6_BE55_94A63A920459__INCLUDED_)
