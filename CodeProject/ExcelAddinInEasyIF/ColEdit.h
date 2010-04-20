#if !defined(AFX_COLEDIT_H__E62E6404_05E5_4FB1_8624_6A553B8521C7__INCLUDED_)
#define AFX_COLEDIT_H__E62E6404_05E5_4FB1_8624_6A553B8521C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColEdit.h : header file
//
#include "colgen.h"
#include "colpos.h"
#include "colplist.h"

/////////////////////////////////////////////////////////////////////////////
// ColumnEdit

class ColumnEdit : public CPropertySheet
{
	DECLARE_DYNAMIC(ColumnEdit)

// Construction
public:
  Matrix *m_pMatrix;
  int     m_iColIndex;

  ColGeneral  *m_pGeneralTab;
  ColPosition *m_pPositionTab;
  ColPickList *m_pPickListTab;


  ColumnEdit(Matrix *pMatrix, int iColIndex, UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
  ColumnEdit(Matrix *pMatrix, int iColIndex, LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ColumnEdit)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ColumnEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(ColumnEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLEDIT_H__E62E6404_05E5_4FB1_8624_6A553B8521C7__INCLUDED_)
