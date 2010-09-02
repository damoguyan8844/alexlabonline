#if !defined(AFX_EMRICHEDIT_H__0E4CB83C_C827_40A5_A589_0BBCB2BB1FD0__INCLUDED_)
#define AFX_EMRICHEDIT_H__0E4CB83C_C827_40A5_A589_0BBCB2BB1FD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EMRichEdit.h : header file
//
#include "stdemdef.h"

/////////////////////////////////////////////////////////////////////////////
// CEMRichEdit window

class EM_MsgRichEdit : public CRichEditCtrl
{
// Construction
public:
	EM_MsgRichEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEMRichEdit)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual ~EM_MsgRichEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEMRichEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMRICHEDIT_H__0E4CB83C_C827_40A5_A589_0BBCB2BB1FD0__INCLUDED_)
