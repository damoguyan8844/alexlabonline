#if !defined(AFX_EM_SENDCHAT_H__A645A12C_14C2_4C7D_AAC7_2E680ECF30DE__INCLUDED_)
#define AFX_EM_SENDCHAT_H__A645A12C_14C2_4C7D_AAC7_2E680ECF30DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EM_SendChat.h : header file
//

#include "WaveOut.h"
#include "EIMChatingWaiter.h"

/////////////////////////////////////////////////////////////////////////////
// EM_SendChat dialog

class EM_SendChat : public CDialog
{
// Construction
public:
	BOOL m_bChatting;
	CWaveOut *m_pwo;
	CEIMChatingWaiter *m_cw;
	BOOL DestAcceptChat(LPCTSTR ip);
	EM_SendChat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EM_SendChat)
	enum { IDD = IDD_VOICE2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EM_SendChat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EM_SendChat)
	afx_msg void OnChatCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EM_SENDCHAT_H__A645A12C_14C2_4C7D_AAC7_2E680ECF30DE__INCLUDED_)
