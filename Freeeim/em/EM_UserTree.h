#if !defined(AFX_MYTREECTRL1_H__5FCF2A4B_A26B_4D4B_A247_647BC785FBB4__INCLUDED_)
#define AFX_MYTREECTRL1_H__5FCF2A4B_A26B_4D4B_A247_647BC785FBB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTreeCtrl1.h : header file
//
#include "stdemdef.h"
#include "EM_GroupInfo.h"
/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl window

class EM_UserTree : public CTreeCtrl
{
// Construction
public:
	EM_UserTree();

// Attributes
public:

// Operations
public:
	BOOL DeleteUser(EM_USERINFO &ui);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~EM_UserTree();

	// Generated message map functions
protected:

	//{{AFX_MSG(CMyTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTREECTRL1_H__5FCF2A4B_A26B_4D4B_A247_647BC785FBB4__INCLUDED_)
