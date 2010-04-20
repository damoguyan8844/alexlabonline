#if !defined(AFX_GRIDFROMCELLWIZ_H__C938CA66_1EA9_474A_8ED5_0903B6795E43__INCLUDED_)
#define AFX_GRIDFROMCELLWIZ_H__C938CA66_1EA9_474A_8ED5_0903B6795E43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridFromCellWiz.h : header file
//
#include "FldProps.h"
#include "GFCMatrixDetails.h"

/////////////////////////////////////////////////////////////////////////////
// GridFromCellWiz

class GridFromCellWiz : public CPropertySheet
{
	DECLARE_DYNAMIC(GridFromCellWiz)

  CObArray m_oFldProps;
  CObArray *m_pFields;

  GFCMatrixDetails *m_pGridInfo;

// Construction
public:
	GridFromCellWiz(CObArray *pFields, UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	GridFromCellWiz(CObArray *pFields, LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GridFromCellWiz)
	public:
	virtual int DoModal();
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~GridFromCellWiz();

	// Generated message map functions
protected:
	//{{AFX_MSG(GridFromCellWiz)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDFROMCELLWIZ_H__C938CA66_1EA9_474A_8ED5_0903B6795E43__INCLUDED_)
