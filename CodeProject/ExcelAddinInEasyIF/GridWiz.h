#if !defined(AFX_GRIDWIZ_H__6CDF35A7_F741_47AB_BFDC_BA9E0D767BB3__INCLUDED_)
#define AFX_GRIDWIZ_H__6CDF35A7_F741_47AB_BFDC_BA9E0D767BB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridWiz.h : header file
//

#include "wizgrpos.h"
#include "wizgrsel.h"
#include "wizgrrng.h"

/////////////////////////////////////////////////////////////////////////////
// GridWiz

class GridWiz : public CPropertySheet
{
	DECLARE_DYNAMIC(GridWiz)

  WizGrSel *m_pSelect;
  WizGrPos *m_pInsertPos;
  WizGrRng *m_pRange;

// Construction
public:
	GridWiz(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	GridWiz(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GridWiz)
	public:
	virtual int DoModal();
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~GridWiz();

	// Generated message map functions
protected:
	//{{AFX_MSG(GridWiz)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDWIZ_H__6CDF35A7_F741_47AB_BFDC_BA9E0D767BB3__INCLUDED_)
