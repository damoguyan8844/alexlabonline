#if !defined(AFX_GRIDLIST_H__EB92A4BF_2387_498D_98DE_59C9FA3E1929__INCLUDED_)
#define AFX_GRIDLIST_H__EB92A4BF_2387_498D_98DE_59C9FA3E1929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridList window

class CGridList : public CListBox
{
// Construction
public:
	CGridList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGridList)
	afx_msg void OnDblclk();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDLIST_H__EB92A4BF_2387_498D_98DE_59C9FA3E1929__INCLUDED_)
