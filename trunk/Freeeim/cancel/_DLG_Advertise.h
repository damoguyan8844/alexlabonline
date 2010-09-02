//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX__DLG_ADVERTISE_H__5F2E12DE_E8F4_4BEC_9A06_D0BAABF76864__INCLUDED_)
#define AFX__DLG_ADVERTISE_H__5F2E12DE_E8F4_4BEC_9A06_D0BAABF76864__INCLUDED_

#include "thirdpartclass/HtmlEditCtrl2.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// _DLG_Advertise.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// _DLG_Advertise dialog
#define TIMER_REFRESH			22
#define TIMER_REFRESH_TIME		1000 * 60 * 15//  ÆŒÂ∑÷÷”

class _DLG_Advertise : public CDialog
{
// Construction
public:
	void _NavNewAdve(LPCTSTR lpszURL);
	_DLG_Advertise(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(_DLG_Advertise)
	enum { IDD = IDD_DIALOG_AD };
	CHtmlEditCtrl2	m_web;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(_DLG_Advertise)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(_DLG_Advertise)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT FAR* URL);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__DLG_ADVERTISE_H__5F2E12DE_E8F4_4BEC_9A06_D0BAABF76864__INCLUDED_)
