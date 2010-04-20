#if !defined(AFX_COLPLIST_H__58C88277_A5AA_4ABA_ABA6_EAAD94427918__INCLUDED_)
#define AFX_COLPLIST_H__58C88277_A5AA_4ABA_ABA6_EAAD94427918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColPList.h : header file
//
#include "matrix.h"

/////////////////////////////////////////////////////////////////////////////
// ColPickList dialog

class ColPickList : public CPropertyPage
{
	DECLARE_DYNCREATE(ColPickList)

// Construction
public:
  ColPickList();
  ColPickList(Matrix *pMatrix, int iColIndex);
  ~ColPickList();

// Dialog Data
	//{{AFX_DATA(ColPickList)
	enum { IDD = IDD_COLEDIT_PICKLIST };
	CString	m_cChoices;
	BOOL	m_bRestrictToList;
	BOOL	m_bUsePickList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(ColPickList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(ColPickList)
	virtual BOOL OnInitDialog();
	afx_msg void OnPicklistUsepicklist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLPLIST_H__58C88277_A5AA_4ABA_ABA6_EAAD94427918__INCLUDED_)
